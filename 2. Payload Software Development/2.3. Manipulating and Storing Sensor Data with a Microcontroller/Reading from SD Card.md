Reading from SD card:

```c++
//Now let's try to open file "test.txt"
    fr = f_open(&f, "hello.txt", FA_READ);
    if (fr != FR_OK) {
    sprintf(string_buffer, "f_open error (%i)\r\n", fr);
    HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1
    while(1); // stop here if there was an error
        }
    sprintf(string_buffer, "I was able to open \"hello.txt\" for reading!\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

    //Read 30 bytes from "test.txt" on the SD card
    BYTE readBuf[30];

    //We can either use f_read OR f_gets to get data out of files
    //f_gets is a wrapper on f_read that does some string formatting for us
    TCHAR* r_res = f_gets((TCHAR*)readBuf, 30, &f);
    if(r_res != 0) {
        sprintf(string_buffer, "Read string from 'hello_world.txt' contents: %s\r\n", readBuf);
        HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1
    } else {
        sprintf(string_buffer, "f_gets error (%i)\r\n", fr);
        HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1
    }

    //close file
    f_close(&f);
    ```