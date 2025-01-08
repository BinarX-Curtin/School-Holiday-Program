/**
 * @file mmc5983ma.h
 * @author Chelsea Tay, Stuart Buchan, Binar Space Program
 * @brief This file contains the function prototypes for interacting with the
 * mmc5983ma magnetometer device.
 *
 * Library adapted from
 * https://github.com/kriswiner/MMC5983MA/tree/master/LSM6DSM_MMC5983MA_LPS22HB_Dragonfly
 *
 * MMC5983MA Datasheet
 * https://au.mouser.com/datasheet/2/821/Memsic_09102019_MMC5983MA_Datasheet_Rev_A-1635338.pdf
 * 
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MMC5983MA_DRIVER_H
#define MMC5983MA_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Register map.
#define MMC5983MA_XOUT_0      0x00 // MSB of X-Axis 
#define MMC5983MA_XOUT_1      0x01 // LSB of X-Axis
#define MMC5983MA_YOUT_0      0x02 // MSB of Y-Axis
#define MMC5983MA_YOUT_1      0x03 // LSB of Y-Axis
#define MMC5983MA_ZOUT_0      0x04 // MSB of Z-Axis
#define MMC5983MA_ZOUT_1      0x05 // LSB of Z-Axis
#define MMC5983MA_XYZOUT_2    0x06 // Xout[7:6], Yout[5:4], Zout[3:2]
#define MMC5983MA_TOUT        0x07 // 8-bit Temperature reading 
#define MMC5983MA_STATUS      0x08 // Measurement Boolean Status [OTP[4],Temp[1],Meas[0]]
#define MMC5983MA_CONTROL_0   0x09 // Main Measurement Control Register
#define MMC5983MA_CONTROL_1   0x0A // Software Reset, Bandwidth Control and channel disables
#define MMC5983MA_CONTROL_2   0x0B // Continuous meausrement mode, Period of measurement and/or frequency 
#define MMC5983MA_CONTROL_3   0x0C // Testing/Calibration and SPI mode
#define MMC5983MA_PRODUCT_ID  0x2F // Should be 0x30 hence good to check for sanity

// CONTROL 1 Register
// Bandwidths 
typedef enum {
  MBW_100Hz = 0b00,
  MBW_200Hz = 0b01,
  MBW_400Hz = 0b10,
  MBW_800Hz = 0b11
} mmc5983ma_bandwidth_t;

// CONTROL 2 Register
// Sample rates 
typedef enum {
  MODR_ONESHOT = 0b000,
  MODR_1Hz = 0b001,
  MODR_10Hz = 0b010,
  MODR_20Hz = 0b011,
  MODR_50Hz = 0b100,
  MODR_100Hz = 0b101,
  MODR_200Hz = 0b110,
  MODR_1000Hz = 0b111
} mmc5983ma_sample_rate_t;

typedef enum {
  MSET_1 = 0b000,
  MSET_25 = 0b001,
  MSET_75 = 0b010,
  MSET_100 = 0b011,
  MSET_250 = 0b100,
  MSET_500 = 0b101,
  MSET_1000 = 0b110,
  MSET_2000 = 0b111
} mmc5983ma_measurement_amout_t;


// Values to write to registers.
#define MAG_SET           0x08 // set current.
#define MAG_RESET         0x10 // reset current.
#define MEAS_T_DONE       0x02 // mag field measurement status.
#define MEAS_M_DONE       0x01 // temp measurement status.
#define INT_MEAS_DONE_EN  0x04 // interrupt for completed measurements.
#define TM_T              0x02 // magnetic field measurement.
#define TM_M              0x01 // temp measurement.

// MMC5983 I2C Address
#define MMC5983MA_ADDRESS 0x30 // 0110000


/**
 * @brief A datatype for a function pointer for writing to the mmc5983ma device.
 *
 */
typedef uint32_t (*mmc5983ma_write_ptr)(const uint8_t *write_buf,
                                        const uint32_t len);

/**
 * @brief A datatype for a function pointer for reading from the mmc5983ma
 * device.
 *
 */
typedef uint32_t (*mmc5983ma_read_ptr)(uint8_t *read_buf, const uint32_t len);

/**
 * @brief Specifies the read and write implementations for interacting with the
 * mmc5983ma.
 *
 */
typedef struct {
  mmc5983ma_write_ptr write_command;
  mmc5983ma_read_ptr read_command;
} mmc5983ma_t;

/**
 * @brief The raw magnetic field readings sampled from the device.
 *
 */
typedef struct {
  uint16_t x_axis;
  uint16_t y_axis;
  uint16_t z_axis;
} field_axes_t;

/**
 * @brief Write data to register.
 *
 * @param magnetometer Magnetometer read/write interface.
 * @param write_buf Pointer to buffer to write.
 * @param len Length of message.
 * @return uint32_t Number of bytes written.
 */
uint32_t MMC5983MAWrite(const mmc5983ma_t *magnetometer, uint8_t *write_buf,
                        uint16_t len);

/**
 * @brief Read data from the device.
 *
 * @param magnetometer Magnetometer read/write interface.
 * @param read_buf Pointer to buffer to store data read.
 * @param len Number of bytes to read.
 * @return uint32_t Number of bytes read.
 */
uint32_t MMC5983MARead(const mmc5983ma_t *magnetometer, uint8_t *read_buf,
                       uint16_t len);

/**
 * @brief Get the product ID.
 *
 * @param magnetometer Magnetometer read/write interface.
 * @return uint8_t Product ID.
 */
uint8_t GetMMC5983ID(const mmc5983ma_t *magnetometer);

/**
 * @brief Perform a magnetic field measurement.
 *
 * @param magnetometer Magnetometer read/write interface.
 * @return Number of bytes written to the device.
 */
uint32_t MagneticFieldMeasurement(const mmc5983ma_t *magnetometer);

/**
 * @brief Perform a temperature measurement
 *
 * @param magnetometer Magnetometer read/write interface.
 * @return uint32_t Number of bytes written to the device.
 */
uint32_t TemperatureMeasurement(const mmc5983ma_t *magnetometer);

/**
 * @brief Get the magnetic field measurement.
 *
 * @param magnetometer Magnetometer read/write interface.
 * @return field_axes_t The values read from the device.
 */
field_axes_t GetField(const mmc5983ma_t *magnetometer);

/**
 * @brief Get the sampled temperature.
 *
 * @param magnetometer Magnetometer read/write interface.
 * @return uint8_t The raw temperature read from the device.
 */
uint8_t GetTemp(const mmc5983ma_t *magnetometer);

/**
 * @brief Clear the magnetic field interrupt.
 *
 * @param magnetometer Magnetometer read/write interface.
 * @return uint132_t The number of bytes written to the device.
 */
uint32_t ClearMagFieldInt(const mmc5983ma_t *magnetometer);

/**
 * @brief Clear the temperature interrupt.
 *
 * @param magnetometer Magnetometer read/write interface.
 * @return uint32_t The number of bytes written to the device.
 */
uint32_t ClearTempInt(const mmc5983ma_t *magnetometer);

/**
 * @brief Provide a large set current to the sensor coils for 500ns.
 *
 * @param magnetometer Magnetometer read/write interface
 * @return uint32_t The number of bytes written to the device.
 */
uint32_t Set(const mmc5983ma_t *magnetometer);

/**
 * @brief Provide a large reset current to the sensor coils for 500ns.
 *
 * @param magnetometer Magnetometer read/write interface
 * @return uint32_t The number of bytes written to the device.
 */
uint32_t Reset(const mmc5983ma_t *magnetometer);

#ifdef __cplusplus
}
#endif

#endif  // MMC5983MA_DRIVER_H
