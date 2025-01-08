/**
 * @file mmc5983ma.h
 * @author Chelsea Tay, Stuart Buchan, Binar Space Program
 * @brief This file contains the function prototypes for interacting with the
 * mmc5983ma magnetometer device.
 *
 * Library adapted from
 * https://github.com/kriswiner/MMC5983MA/tree/master/LSM6DSM_MMC5983MA_LPS22HB_Dragonfly
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
#define X_OUT_0 0x00   // Base address for reading the field measurements.
#define TEMP_OUT 0x07  // Temperature measurement.
#define STATUS 0x08    // Status register.
#define INTERNAL_CONTROL_0 0x09  // Control register 0.
#define PRODUCT_ID 0x2F          // Product ID.

// Values to write to registers.
#define MAG_SET 0x08           // set current.
#define MAG_RESET 0x10         // reset current.
#define MEAS_T_DONE 0x02       // mag field measurement status.
#define MEAS_M_DONE 0x01       // temp measurement status.
#define INT_MEAS_DONE_EN 0x04  // interrupt for completed measurements.
#define TM_T 0x02              // magnetic field measurement.
#define TM_M 0x01              // temp measurement.

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
uint32_t Mmc5983maWrite(const mmc5983ma_t *magnetometer, uint8_t *write_buf,
                        uint16_t len);

/**
 * @brief Read data from the device.
 *
 * @param magnetometer Magnetometer read/write interface.
 * @param read_buf Pointer to buffer to store data read.
 * @param len Number of bytes to read.
 * @return uint32_t Number of bytes read.
 */
uint32_t Mmc5983maRead(const mmc5983ma_t *magnetometer, uint8_t *read_buf,
                       uint16_t len);

/**
 * @brief Get the product ID.
 *
 * @param magnetometer Magnetometer read/write interface.
 * @return uint8_t Product ID.
 */
uint8_t GetPid(const mmc5983ma_t *magnetometer);

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
