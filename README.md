# LCD16x2_DRIVERS

For STM32F4

First (small) project controlling a LCD screen with 16 columns and 2 rows using a STM32F446 nucleo board.

I have used the HAL function and STM32CubeIDE, it should be easy to port it to another nucleo board, but I haven't test it myself.

Under /Drivers/LCD16x2/ you can find the header and source files.

It uses a timer to implement the delay function (/utilities) to achieve the enable waiting time required.

The datasheet used as reference can be found [here](https://datasheetspdf.com/pdf-file/519148/CA/LCD-1602A/1). 

