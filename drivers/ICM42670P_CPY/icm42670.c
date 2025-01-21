/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (c) 2022 Jan Veeh <jan.veeh@motius.de>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * @file icm42670.c
 *
 * ESP-IDF driver for TDK ICM-42670-P IMU (found on ESP-RS board)
 *
 * Copyright (c) 2022 Jan Veeh (jan.veeh@motius.de)
 *
 * ISC Licensed as described in the file LICENSE
 *
 * Open TODOs:
 * - FIFO reading and handling
 * - APEX functions like pedometer, tilt-detection, low-g detection, freefall detection, ...
 *
 *
 */

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_idf_lib_helpers.h>
#include <ets_sys.h>
#include "icm42670.h"

#define I2C_FREQ_HZ 1000000 // 1MHz

static const char *TAG = "icm42670";

// register structure definitions
#define ICM42670_MCLK_RDY_BITS  0x08 // ICM42670_REG_MCLK_RDY<3>
#define ICM42670_MCLK_RDY_SHIFT 3    // ICM42670_REG_MCLK_RDY<3>

#define ICM42670_SPI_AP_4WIRE_BITS  0x04 // ICM42670_REG_DEVICE_CONFIG<2>
#define ICM42670_SPI_AP_4WIRE_SHIFT 2    // ICM42670_REG_DEVICE_CONFIG<2>
#define ICM42670_SPI_MODE_BITS      0x01 // ICM42670_REG_DEVICE_CONFIG<0>
#define ICM42670_SPI_MODE_SHIFT     0    // ICM42670_REG_DEVICE_CONFIG<0>

#define ICM42670_SOFT_RESET_DEVICE_CONFIG_BITS  0x10 // ICM42670_REG_SIGNAL_PATH_RESET<4>
#define ICM42670_SOFT_RESET_DEVICE_CONFIG_SHIFT 4    // ICM42670_REG_SIGNAL_PATH_RESET<4>
#define ICM42670_FIFO_FLUSH_BITS                0x04 // ICM42670_REG_SIGNAL_PATH_RESET<2>
#define ICM42670_FIFO_FLUSH_SHIFT               2    // ICM42670_REG_SIGNAL_PATH_RESET<2>

#define ICM42670_I3C_DDR_SLEW_RATE_BITS  0x38 // ICM42670_REG_DRIVE_CONFIG1<5:3>
#define ICM42670_I3C_DDR_SLEW_RATE_SHIFT 3    // ICM42670_REG_DRIVE_CONFIG1<5:3>
#define ICM42670_I3C_SDR_SLEW_RATE_BITS  0x07 // ICM42670_REG_DRIVE_CONFIG1<2:0>
#define ICM42670_I3C_SDR_SLEW_RATE_SHIFT 0    // ICM42670_REG_DRIVE_CONFIG1<2:0>

#define ICM42670_I2C_DDR_SLEW_RATE_BITS  0x38 // ICM42670_REG_DRIVE_CONFIG2<5:3>
#define ICM42670_I2C_DDR_SLEW_RATE_SHIFT 3    // ICM42670_REG_DRIVE_CONFIG2<5:3>
#define ICM42670_I2C_SDR_SLEW_RATE_BITS  0x07 // ICM42670_REG_DRIVE_CONFIG2<2:0>
#define ICM42670_I2C_SDR_SLEW_RATE_SHIFT 0    // ICM42670_REG_DRIVE_CONFIG2<2:0>

#define ICM42670_SPI_SLEW_RATE_BITS  0x07 // ICM42670_REG_DRIVE_CONFIG3<2:0>
#define ICM42670_SPI_SLEW_RATE_SHIFT 0    // ICM42670_REG_DRIVE_CONFIG3<2:0>

#define ICM42670_INT2_MODE_BITS           0x20 // ICM42670_REG_INT_CONFIG<5>
#define ICM42670_INT2_MODE_SHIFT          5    // ICM42670_REG_INT_CONFIG<5>
#define ICM42670_INT2_DRIVE_CIRCUIT_BITS  0x10 // ICM42670_REG_INT_CONFIG<4>
#define ICM42670_INT2_DRIVE_CIRCUIT_SHIFT 4    // ICM42670_REG_INT_CONFIG<4>
#define ICM42670_INT2_POLARITY_BITS       0x08 // ICM42670_REG_INT_CONFIG<3>
#define ICM42670_INT2_POLARITY_SHIFT      3    // ICM42670_REG_INT_CONFIG<3>
#define ICM42670_INT1_MODE_BITS           0x04 // ICM42670_REG_INT_CONFIG<2>
#define ICM42670_INT1_MODE_SHIFT          2    // ICM42670_REG_INT_CONFIG<2>
#define ICM42670_INT1_DRIVE_CIRCUIT_BITS  0x02 // ICM42670_REG_INT_CONFIG<1>
#define ICM42670_INT1_DRIVE_CIRCUIT_SHIFT 1    // ICM42670_REG_INT_CONFIG<1>
#define ICM42670_INT1_POLARITY_BITS       0x01 // ICM42670_REG_INT_CONFIG<0>
#define ICM42670_INT1_POLARITY_SHIFT      0    // ICM42670_REG_INT_CONFIG<0>

#define ICM42670_ACCEL_LP_CLK_SEL_BITS  0x80 // ICM42670_REG_PWR_MGMT0<7>
#define ICM42670_ACCEL_LP_CLK_SEL_SHIFT 7    // ICM42670_REG_PWR_MGMT0<7>
#define ICM42670_IDLE_BITS              0x10 // ICM42670_REG_PWR_MGMT0<4>
#define ICM42670_IDLE_SHIFT             4    // ICM42670_REG_PWR_MGMT0<4>
#define ICM42670_GYRO_MODE_BITS         0x0C // ICM42670_REG_PWR_MGMT0<3:2>
#define ICM42670_GYRO_MODE_SHIFT        2    // ICM42670_REG_PWR_MGMT0<3:2>
#define ICM42670_ACCEL_MODE_BITS        0x03 // ICM42670_REG_PWR_MGMT0<1:0>
#define ICM42670_ACCEL_MODE_SHIFT       0    // ICM42670_REG_PWR_MGMT0<1:0>

#define ICM42670_GYRO_UI_FS_SEL_BITS  0x60 // ICM42670_REG_GYRO_CONFIG0<6:5>
#define ICM42670_GYRO_UI_FS_SEL_SHIFT 5    // ICM42670_REG_GYRO_CONFIG0<6:5>
#define ICM42670_GYRO_ODR_BITS        0x0F // ICM42670_REG_GYRO_CONFIG0<3:0>
#define ICM42670_GYRO_ODR_SHIFT       0    // ICM42670_REG_GYRO_CONFIG0<3:0>

#define ICM42670_ACCEL_UI_FS_SEL_BITS  0x60 // ICM42670_REG_ACCEL_CONFIG0<6:5>
#define ICM42670_ACCEL_UI_FS_SEL_SHIFT 5    // ICM42670_REG_ACCEL_CONFIG0<6:5>
#define ICM42670_ACCEL_ODR_BITS        0x0F // ICM42670_REG_ACCEL_CONFIG0<3:0>
#define ICM42670_ACCEL_ODR_SHIFT       0    // ICM42670_REG_ACCEL_CONFIG0<3:0>

#define ICM42670_TEMP_FILT_BW_BITS  0x70 // ICM42670_REG_TEMP_CONFIG0<6:4>
#define ICM42670_TEMP_FILT_BW_SHIFT 4    // ICM42670_REG_TEMP_CONFIG0<6:4>

#define ICM42670_GYRO_UI_FILT_BW_BITS  0x07 // ICM42670_REG_GYRO_CONFIG1<2:0>
#define ICM42670_GYRO_UI_FILT_BW_SHIFT 0    // ICM42670_REG_GYRO_CONFIG1<2:0>

#define ICM42670_ACCEL_UI_AVG_BITS      0x70 // ICM42670_REG_ACCEL_CONFIG1<6:4>
#define ICM42670_ACCEL_UI_AVG_SHIFT     4    // ICM42670_REG_ACCEL_CONFIG1<6:4>
#define ICM42670_ACCEL_UI_FILT_BW_BITS  0x07 // ICM42670_REG_ACCEL_CONFIG1<2:0>
#define ICM42670_ACCEL_UI_FILT_BW_SHIFT 0    // ICM42670_REG_ACCEL_CONFIG1<2:0>

#define ICM42670_DMP_POWER_SAVE_EN_BITS  0x08 // ICM42670_REG_APEX_CONFIG0<3>
#define ICM42670_DMP_POWER_SAVE_EN_SHIFT 3    // ICM42670_REG_APEX_CONFIG0<3>
#define ICM42670_DMP_INIT_EN_BITS        0x04 // ICM42670_REG_APEX_CONFIG0<2>
#define ICM42670_DMP_INIT_EN_SHIFT       2    // ICM42670_REG_APEX_CONFIG0<2>
#define ICM42670_DMP_MEM_RESET_EN_BITS   0x01 // ICM42670_REG_APEX_CONFIG0<0>
#define ICM42670_DMP_MEM_RESET_EN_SHIFT  0    // ICM42670_REG_APEX_CONFIG0<0>

#define ICM42670_SMD_ENABLE_BITS   0x40 // ICM42670_REG_APEX_CONFIG1<6>
#define ICM42670_SMD_ENABLE_SHIFT  6    // ICM42670_REG_APEX_CONFIG1<6>
#define ICM42670_FF_ENABLE_BITS    0x20 // ICM42670_REG_APEX_CONFIG1<5>
#define ICM42670_FF_ENABLE_SHIFT   5    // ICM42670_REG_APEX_CONFIG1<5>
#define ICM42670_TILT_ENABLE_BITS  0x10 // ICM42670_REG_APEX_CONFIG1<4>
#define ICM42670_TILT_ENABLE_SHIFT 4    // ICM42670_REG_APEX_CONFIG1<4>
#define ICM42670_PED_ENABLE_BITS   0x08 // ICM42670_REG_APEX_CONFIG1<3>
#define ICM42670_PED_ENABLE_SHIFT  3    // ICM42670_REG_APEX_CONFIG1<3>
#define ICM42670_DMP_ODR_BITS      0x03 // ICM42670_REG_APEX_CONFIG1<1:0>
#define ICM42670_DMP_ODR_SHIFT     0    // ICM42670_REG_APEX_CONFIG1<1:0>

#define ICM42670_WOM_INT_DUR_BITS   0x18 // ICM42670_REG_WOM_CONFIG<4:3>
#define ICM42670_WOM_INT_DUR_SHIFT  3    // ICM42670_REG_WOM_CONFIG<4:3>
#define ICM42670_WOM_INT_MODE_BITS  0x04 // ICM42670_REG_WOM_CONFIG<2>
#define ICM42670_WOM_INT_MODE_SHIFT 2    // ICM42670_REG_WOM_CONFIG<2>
#define ICM42670_WOM_MODE_BITS      0x02 // ICM42670_REG_WOM_CONFIG<1>
#define ICM42670_WOM_MODE_SHIFT     1    // ICM42670_REG_WOM_CONFIG<1>
#define ICM42670_WOM_EN_BITS        0x01 // ICM42670_REG_WOM_CONFIG<0>
#define ICM42670_WOM_EN_SHIFT       0    // ICM42670_REG_WOM_CONFIG<0>

#define ICM42670_FIFO_MODE_BITS    0x02 // ICM42670_REG_FIFO_CONFIG1<1>
#define ICM42670_FIFO_MODE_SHIFT   1    // ICM42670_REG_FIFO_CONFIG1<1>
#define ICM42670_FIFO_BYPASS_BITS  0x01 // ICM42670_REG_FIFO_CONFIG1<0>
#define ICM42670_FIFO_BYPASS_SHIFT 0    // ICM42670_REG_FIFO_CONFIG1<0>

#define ICM42670_ST_INT1_EN_BITS          0x80 // ICM42670_REG_INT_SOURCE0<7>
#define ICM42670_ST_INT1_EN_SHIFT         7    // ICM42670_REG_INT_SOURCE0<7>
#define ICM42670_FSYNC_INT1_EN_BITS       0x40 // ICM42670_REG_INT_SOURCE0<6>
#define ICM42670_FSYNC_INT1_EN_SHIFT      6    // ICM42670_REG_INT_SOURCE0<6>
#define ICM42670_PLL_RDY_INT1_EN_BITS     0x20 // ICM42670_REG_INT_SOURCE0<5>
#define ICM42670_PLL_RDY_INT1_EN_SHIFT    5    // ICM42670_REG_INT_SOURCE0<5>
#define ICM42670_RESET_DONE_INT1_EN_BITS  0x10 // ICM42670_REG_INT_SOURCE0<4>
#define ICM42670_RESET_DONE_INT1_EN_SHIFT 4    // ICM42670_REG_INT_SOURCE0<4>
#define ICM42670_DRDY_INT1_EN_BITS        0x08 // ICM42670_REG_INT_SOURCE0<3>
#define ICM42670_DRDY_INT1_EN_SHIFT       3    // ICM42670_REG_INT_SOURCE0<3>
#define ICM42670_FIFO_THS_INT1_EN_BITS    0x04 // ICM42670_REG_INT_SOURCE0<2>
#define ICM42670_FIFO_THS_INT1_EN_SHIFT   2    // ICM42670_REG_INT_SOURCE0<2>
#define ICM42670_FIFO_FULL_INT1_EN_BITS   0x02 // ICM42670_REG_INT_SOURCE0<1>
#define ICM42670_FIFO_FULL_INT1_EN_SHIFT  1    // ICM42670_REG_INT_SOURCE0<1>
#define ICM42670_AGC_RDY_INT1_EN_BITS     0x01 // ICM42670_REG_INT_SOURCE0<0>
#define ICM42670_AGC_RDY_INT1_EN_SHIFT    0    // ICM42670_REG_INT_SOURCE0<0>

#define ICM42670_I3C_PROTOCOL_ERROR_INT1_EN_BITS  0x40 // ICM42670_REG_INT_SOURCE1<6>
#define ICM42670_I3C_PROTOCOL_ERROR_INT1_EN_SHIFT 6    // ICM42670_REG_INT_SOURCE1<6>
#define ICM42670_SMD_INT1_EN_BITS                 0x08 // ICM42670_REG_INT_SOURCE1<3>
#define ICM42670_SMD_INT1_EN_SHIFT                3    // ICM42670_REG_INT_SOURCE1<3>
#define ICM42670_WOM_Z_INT1_EN_BITS               0x04 // ICM42670_REG_INT_SOURCE1<2>
#define ICM42670_WOM_Z_INT1_EN_SHIFT              2    // ICM42670_REG_INT_SOURCE1<2>
#define ICM42670_WOM_Y_INT1_EN_BITS               0x02 // ICM42670_REG_INT_SOURCE1<1>
#define ICM42670_WOM_Y_INT1_EN_SHIFT              1    // ICM42670_REG_INT_SOURCE1<1>
#define ICM42670_WOM_X_INT1_EN_BITS               0x01 // ICM42670_REG_INT_SOURCE1<0>
#define ICM42670_WOM_X_INT1_EN_SHIFT              0    // ICM42670_REG_INT_SOURCE1<0>

// ICM42670_REG_INT_SOURCE3 and ICM42670_REG_INT_SOURCE4 same as 0 and 1

#define ICM42670_DMP_IDLE_BITS        0x04 // ICM42670_REG_APEX_DATA3<2>
#define ICM42670_DMP_IDLE_SHIFT       2    // ICM42670_REG_APEX_DATA3<2>
#define ICM42670_ACTIVITY_CLASS_BITS  0x03 // ICM42670_REG_APEX_DATA3<1:0>
#define ICM42670_ACTIVITY_CLASS_SHIFT 0    // ICM42670_REG_APEX_DATA3<1:0>

#define ICM42670_FIFO_COUNT_FORMAT_BITS   0x40 // ICM42670_REG_INTF_CONFIG0<6>
#define ICM42670_FIFO_COUNT_FORMAT_SHIFT  6    // ICM42670_REG_INTF_CONFIG0<6>
#define ICM42670_FIFO_COUNT_ENDIAN_BITS   0x20 // ICM42670_REG_INTF_CONFIG0<5>
#define ICM42670_FIFO_COUNT_ENDIAN_SHIFT  5    // ICM42670_REG_INTF_CONFIG0<5>
#define ICM42670_SENSOR_DATA_ENDIAN_BITS  0x10 // ICM42670_REG_INTF_CONFIG0<4>
#define ICM42670_SENSOR_DATA_ENDIAN_SHIFT 4    // ICM42670_REG_INTF_CONFIG0<4>

#define ICM42670_I3C_SDR_EN_BITS  0x08 // ICM42670_REG_INTF_CONFIG1<3>
#define ICM42670_I3C_SDR_EN_SHIFT 3    // ICM42670_REG_INTF_CONFIG1<3>
#define ICM42670_I3C_DDR_EN_BITS  0x04 // ICM42670_REG_INTF_CONFIG1<2>
#define ICM42670_I3C_DDR_EN_SHIFT 2    // ICM42670_REG_INTF_CONFIG1<2>
#define ICM42670_CLKSEL_BITS      0x03 // ICM42670_REG_INTF_CONFIG1<1:0>
#define ICM42670_CLKSEL_SHIFT     0    // ICM42670_REG_INTF_CONFIG1<1:0>

#define ICM42670_DATA_RDY_INT_BITS  0x01 // ICM42670_REG_INT_STATUS_DRDY<0>
#define ICM42670_DATA_RDY_INT_SHIFT 0    // ICM42670_REG_INT_STATUS_DRDY<0>



uint32_t Icm42670Write(icm42670_t *icm, uint8_t *write_buf, uint16_t len) {
    return icm->write_command(write_buf, len);
}

uint32_t Icm42670Read(icm42670_t *icm, uint8_t *read_buf, uint16_t len) {
    return icm->read_command(write_buf, len);
}

uint32_t manipulate_register(icm42670_t *icm, uint8_t reg_addr, uint8_t mask, uint8_t shift,
    uint8_t value) {
    uint8_t reg;
    Icm42670Write(icm,reg_addr,sizeof(reg_addr));
    Icm42670Read(icm,reg,sizeof(reg));
    reg = (reg & ~mask) | (value << shift);
    return Icm42670Write(icm, (uint16_t) ((reg_addr << 7) | reg), sizeof(uint16_t));

}
void Icm42670_init(icm42670_t *icm) {
    uint8_t write_buf[1] = {ICM42670_REG_WHO_AM_I};
    uint8_t read_buf[1] = {0};

    // check who_am_i register
    Icm42670Write(icm, write_buf, sizeof(write_buf));
    Icm42670Read(icm, read_buf, sizeof(read_buf));
    if (read_buf != 0x67)
    {
        //debugWrite("Error: ICM42670 did not return the correct WHOAMI.")
        return 0;
    }
    // flush FIFO
    Icm42670_flush_fifo(icm);
    // perform signal path reset
    Icm42670_reset(icm);

    // set device in IDLE power state
    Icm42670_set_idle_pwr_mode(icm,true);

    // check if internal clock is running
    bool mclk_rdy = Icm42670_get_mclk_rdy(icm);
    if (!mclk_rdy)
    {
        //debugWrite("Error initializing icm42670, Internal clock not running");
        return 0;
    }
}

uint32_t Icm42670_set_idle_pwr_mode(icm42670_t *icm, bool idle_power) {
    return manipulate_register(icm, ICM42670_REG_PWR_MGMT0, ICM42670_IDLE_BITS, ICM42670_IDLE_SHIFT, (uint8_t)idle_power); 
}

esp_err_t Icm42670_set_gyro_pwr_mode(icm42670_t *dev, icm42670_gyro_pwr_mode_t pwr_mode)
{
    return manipulate_register(icm, ICM42670_REG_PWR_MGMT0, ICM42670_GYRO_MODE_BITS, ICM42670_GYRO_MODE_SHIFT, pwr_mode);
}

esp_err_t icm42670_set_accel_pwr_mode(icm42670_t *dev, icm42670_accel_pwr_mode_t pwr_mode)
{
    CHECK_ARG(dev);

    // certain odr and avg settings are not allowed in LP or LN mode
    icm42670_accel_odr_t odr;
    icm42670_accel_avg_t avg;
    CHECK(icm42670_get_accel_odr(dev, &odr));
    CHECK(icm42670_get_accel_avg(dev, &avg));

    if ((pwr_mode == ICM42670_ACCEL_ENABLE_LP_MODE)
        && ((odr == ICM42670_ACCEL_ODR_800HZ) || (odr == ICM42670_ACCEL_ODR_1_6KHZ)
            || ((odr == ICM42670_ACCEL_ODR_200HZ) && (avg == ICM42670_ACCEL_AVG_64X))))
    {
        ESP_LOGE(TAG, "Accel ODR and AVG settings invalid for Low-power mode");
        return ESP_ERR_INVALID_ARG;
    }

    if ((pwr_mode == ICM42670_ACCEL_ENABLE_LN_MODE)
        && ((odr == ICM42670_ACCEL_ODR_6_25HZ) || (odr == ICM42670_ACCEL_ODR_3_125HZ)
            || (odr == ICM42670_ACCEL_ODR_1_5625HZ)))
    {
        ESP_LOGE(TAG, "Accel ODR settings invalid for Low-noise mode");
        return ESP_ERR_INVALID_ARG;
    }

    CHECK(manipulate_register(dev, ICM42670_REG_PWR_MGMT0, ICM42670_ACCEL_MODE_BITS, ICM42670_ACCEL_MODE_SHIFT,
        pwr_mode));

    return ESP_OK;
}

esp_err_t icm42670_set_low_power_clock(icm42670_t *dev, icm42670_lp_clock_source_t clock_source)
{
    CHECK_ARG(dev);

    CHECK(manipulate_register(dev, ICM42670_REG_PWR_MGMT0, ICM42670_ACCEL_LP_CLK_SEL_BITS,
        ICM42670_ACCEL_LP_CLK_SEL_SHIFT, clock_source));

    return ESP_OK;
}

esp_err_t icm42670_read_raw_data(icm42670_t *dev, uint8_t data_register, int16_t *data)
{
    CHECK_ARG(dev && data);

    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
    I2C_DEV_CHECK(&dev->i2c_dev, read_register_16(dev, data_register, data));
    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);

    return ESP_OK;
}

void icm42670_read_temperature(icm42670_t *dev, float *temperature)
{
    uint8_t write_buf[1] = {ICM42670_REG_TEMP_DATA1};
    uint8_t read_buf[2] = {0};

    Icm42670Write(icm, write_buf, sizeof(write_buf));
    Icm42670Read(icm, read_buf, sizeof(read_buf));

    uint16_t reg = (read_buf[0] << 8) | read_buf[1];
    *temperature = (reg / 128.0) + 25;
}

esp_err_t icm42670_reset(icm42670_t *dev) {
    uint8_t write_buf[2] = {ICM42670_REG_SIGNAL_PATH_RESET, (1<<ICM42670_SOFT_RESET_DEVICE_CONFIG_SHIFT)};
    return Icm42670Write(icm, write_buf, sizeof(write_buf));
}

esp_err_t icm42670_flush_fifo(icm42670_t *dev) {
    uint8_t write_buf[2] = {ICM42670_REG_SIGNAL_PATH_RESET, (1<<ICM42670_FIFO_FLUSH_SHIFT)};
    return Icm42670Write(icm, write_buf, sizeof(write_buf));
}

esp_err_t icm42670_set_gyro_fsr(icm42670_t *dev, icm42670_gyro_fsr_t range)
{
    CHECK_ARG(dev);
    return manipulate_register(dev, ICM42670_REG_GYRO_CONFIG0, ICM42670_GYRO_UI_FS_SEL_BITS,
        ICM42670_GYRO_UI_FS_SEL_SHIFT, range);
}

esp_err_t icm42670_set_gyro_odr(icm42670_t *dev, icm42670_gyro_odr_t odr)
{
    CHECK_ARG(dev);
    return manipulate_register(dev, ICM42670_REG_GYRO_CONFIG0, ICM42670_GYRO_ODR_BITS, ICM42670_GYRO_ODR_SHIFT, odr);
}

esp_err_t icm42670_set_accel_fsr(icm42670_t *dev, icm42670_accel_fsr_t range)
{
    CHECK_ARG(dev);
    return manipulate_register(dev, ICM42670_REG_ACCEL_CONFIG0, ICM42670_ACCEL_UI_FS_SEL_BITS,
        ICM42670_ACCEL_UI_FS_SEL_SHIFT, range);
}

esp_err_t icm42670_set_accel_odr(icm42670_t *dev, icm42670_accel_odr_t odr)
{
    CHECK_ARG(dev);
    return manipulate_register(dev, ICM42670_REG_ACCEL_CONFIG0, ICM42670_ACCEL_ODR_BITS, ICM42670_ACCEL_ODR_SHIFT, odr);
}

esp_err_t icm42670_set_temp_lpf(icm42670_t *dev, icm42670_temp_lfp_t lpf_bw)
{
    CHECK_ARG(dev);
    return manipulate_register(dev, ICM42670_REG_TEMP_CONFIG0, ICM42670_TEMP_FILT_BW_BITS, ICM42670_TEMP_FILT_BW_SHIFT,
        lpf_bw);
}

esp_err_t icm42670_set_gyro_lpf(icm42670_t *dev, icm42670_gyro_lfp_t lpf_bw)
{
    CHECK_ARG(dev);
    return manipulate_register(dev, ICM42670_REG_GYRO_CONFIG1, ICM42670_GYRO_UI_FILT_BW_BITS,
        ICM42670_GYRO_UI_FILT_BW_SHIFT, lpf_bw);
}

esp_err_t icm42670_set_accel_lpf(icm42670_t *dev, icm42670_accel_lfp_t lpf_bw)
{
    CHECK_ARG(dev);
    return manipulate_register(dev, ICM42670_REG_ACCEL_CONFIG1, ICM42670_ACCEL_UI_FILT_BW_BITS,
        ICM42670_ACCEL_UI_FILT_BW_SHIFT, lpf_bw);
}

esp_err_t icm42670_set_accel_avg(icm42670_t *dev, icm42670_accel_avg_t avg)
{
    CHECK_ARG(dev);
    return manipulate_register(dev, ICM42670_REG_ACCEL_CONFIG1, ICM42670_ACCEL_UI_AVG_BITS, ICM42670_ACCEL_UI_AVG_SHIFT,
        avg);
}

esp_err_t icm42670_config_int_pin(icm42670_t *dev, uint8_t int_pin, icm42670_int_config_t config)
{
    CHECK_ARG(dev && int_pin < 3 && int_pin > 0);

    uint8_t reg = config.mode << 2 | config.drive << 1 | config.polarity;
    if (int_pin == 2)
    {
        return manipulate_register(dev, ICM42670_REG_INT_CONFIG, 0b00111000, ICM42670_INT2_POLARITY_SHIFT, reg);
    }
    else
    {
        return manipulate_register(dev, ICM42670_REG_INT_CONFIG, 0b00000111, ICM42670_INT1_POLARITY_SHIFT, reg);
    }
}

esp_err_t icm42670_set_int_sources(icm42670_t *dev, uint8_t int_pin, icm42670_int_source_t sources)
{
    CHECK_ARG(dev && int_pin < 3 && int_pin > 0);

    uint8_t reg1 = 0, reg2 = 0;
    if (sources.self_test_done)
        reg1 = reg1 | (1 << ICM42670_ST_INT1_EN_SHIFT);
    if (sources.fsync)
        reg1 = reg1 | (1 << ICM42670_FSYNC_INT1_EN_SHIFT);
    if (sources.pll_ready)
        reg1 = reg1 | (1 << ICM42670_PLL_RDY_INT1_EN_SHIFT);
    if (sources.reset_done)
        reg1 = reg1 | (1 << ICM42670_RESET_DONE_INT1_EN_SHIFT);
    if (sources.data_ready)
        reg1 = reg1 | (1 << ICM42670_DRDY_INT1_EN_SHIFT);
    if (sources.fifo_threshold)
        reg1 = reg1 | (1 << ICM42670_FIFO_THS_INT1_EN_SHIFT);
    if (sources.fifo_full)
        reg1 = reg1 | (1 << ICM42670_FIFO_FULL_INT1_EN_SHIFT);
    if (sources.agc_ready)
        reg1 = reg1 | (1 << ICM42670_AGC_RDY_INT1_EN_SHIFT);
    if (sources.i3c_error)
        reg2 = reg2 | (1 << ICM42670_I3C_PROTOCOL_ERROR_INT1_EN_SHIFT);
    if (sources.smd)
        reg2 = reg2 | (1 << ICM42670_SMD_INT1_EN_SHIFT);
    if (sources.wom_z)
        reg2 = reg2 | (1 << ICM42670_WOM_Z_INT1_EN_SHIFT);
    if (sources.wom_y)
        reg2 = reg2 | (1 << ICM42670_WOM_Y_INT1_EN_SHIFT);
    if (sources.wom_x)
        reg2 = reg2 | (1 << ICM42670_WOM_X_INT1_EN_SHIFT);

    if (int_pin == 1)
    {
        I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
        I2C_DEV_CHECK(&dev->i2c_dev, write_register(dev, ICM42670_REG_INT_SOURCE0, reg1));
        I2C_DEV_CHECK(&dev->i2c_dev, write_register(dev, ICM42670_REG_INT_SOURCE1, reg2));
        I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    }
    else
    {
        I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
        I2C_DEV_CHECK(&dev->i2c_dev, write_register(dev, ICM42670_REG_INT_SOURCE3, reg1));
        I2C_DEV_CHECK(&dev->i2c_dev, write_register(dev, ICM42670_REG_INT_SOURCE4, reg2));
        I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    }

    return ESP_OK;
}

bool icm42670_get_mclk_rdy(icm42670_t *icm)
{
    bool mclk_rdy = false;
    uint8_t write_buf[1] = {ICM42670_REG_MCLK_RDY};
    uint8_t read_buf[1] = {0};
    
    Icm42670Write(icm, write_buf, sizeof(write_buf));
    Icm42670Read(icm, read_buf, sizeof(read_buf));

    if ((read_buf & ICM42670_MCLK_RDY_BITS) >> ICM42670_MCLK_RDY_SHIFT)
        mclk_rdy = true;

    return mclk_rdy;
}

esp_err_t icm42670_get_accel_odr(icm42670_t *dev, icm42670_accel_odr_t *odr)
{
    CHECK_ARG(dev && odr);

    uint8_t reg;
    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
    I2C_DEV_CHECK(&dev->i2c_dev, read_register(dev, ICM42670_REG_ACCEL_CONFIG0, &reg));
    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    *odr = (reg & ICM42670_ACCEL_ODR_BITS) >> ICM42670_ACCEL_ODR_SHIFT;

    return ESP_OK;
}

esp_err_t icm42670_get_accel_avg(icm42670_t *dev, icm42670_accel_avg_t *avg)
{
    CHECK_ARG(dev && avg);

    uint8_t reg;
    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
    I2C_DEV_CHECK(&dev->i2c_dev, read_register(dev, ICM42670_REG_ACCEL_CONFIG1, &reg));
    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    *avg = (reg & ICM42670_ACCEL_UI_AVG_BITS) >> ICM42670_ACCEL_UI_AVG_SHIFT;

    return ESP_OK;
}