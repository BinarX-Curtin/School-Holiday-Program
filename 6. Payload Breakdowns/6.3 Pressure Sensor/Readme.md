## Resources

- Bosch Sensortec BMP390 Datasheet: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp390-ds002.pdf
- Adafruit BMP3XX Library on GitHub: https://github.com/adafruit/Adafruit_BMP3XX (for reference, not used directly)
- "Getting Started with STM32 - How to Use SPI" Digikey article by Shawn Hymel: https://www.digikey.com.au/en/maker/projects/getting-started-with-stm32-how-to-use-spi/09eab3dfe74c4d0391aaaa99b0a8ee17
- STM32 SPI Gotcha article: http://efton.sk/STM32/gotcha/g21.html

## Payload Electronics Hardware

### Rocket Payload Microcontroller Board Pin Labels
![payload board pin labels](figures/BinarX%20Rocket%20Payload%20Microcontroller%20Board%20Pin%20Labels.png)


### Microcontroller IO Connections
![microcontroller io connections](figures/micro_io_connections.png)

1. Wire up the pressure sensor on the breakout board according to the reccomended conection diagram for 3 or 4 wire SPI operation in section 6.2 on page 47 of the datasheet:

![bmp390 connection diagram](figures/bmp390_connection_diagram.png)

> *We will be using 4 wire SPI with separate signal wires for serial data in and serial data out.*

**TODO**

## Payload Software

1. Pressure Sensor Pin Assignments

    ![pressure sensor pin assignments](figures/pressure_sensor_pin_assignments.png)


2. Pressure Sensor SPI1 Configuration

    ![pressure sensor spi1 configuration](figures/pressure_sensor_spi1_configuration.png)

3. Generate code

<!-- 4. In "main.h" in "Private defines" user code section between "USER CODE BEGIN Private defines" and "USER CODE END Private defines":

    ```c++
    //write bit for BMP390 pressure sensor SPI communications (MSB)
    #define BMP390_WB 0x80
    ``` -->

4. In "main.c in "PV" user code section between "USER CODE BEGIN PV" and "USER CODE END PV":

    ```c++
    const uint8_t BMP390_READ_SERIAL_NUMBER[3] = {0x80, 0x00, 0x00}; //address to read pressure sensor chip serial number including read-write bit (MSB) and dummy byte (second byte)
    static uint8_t spi_rx_buffer[3] = 0; //buffer for SPI Rx
    ```

5. In "main.c in "2" user code section between "USER CODE BEGIN 2" and "USER CODE END 2":

    ```c++
    
    ```



## Extension

- Use hardware interrupts and the sensor's data ready pin to make the communication with the sensor more efficient (with it's use of CPU time).