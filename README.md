# SPI-Communication

 * This program demonstrates SPI communication between an STM32f407 microcontroller and an Arduino board.
 * When one of the colored buttons is pressed, the STM32 board sends a message to the Arduino to turn on the corresponding LED.

### Technical Details 
 
 * The STM32 board is configured as the master device and the Arduino is configured as the slave device.
 * The STM32 board is connected to four push buttons.

    | GPIO Pin | Push Button        |
    |----------|--------------------|
    | PC00     | Red Push Button    |
    | PC01     | Yellow Push Button |
    | PC02     | Green Push Button  |
    | PC03     | Blue Push Button   |
 
 * The STM32 board and the Arduino have different output voltage levels (STM32 outputs 3V and Arduino outputs 5V), so a voltage level converter module is connected between the STM32 and the Arduino to make their output levels compatible.
   <img width="383" height="285" alt="logic level converter" src="https://github.com/user-attachments/assets/1dcc58f6-d698-472f-9ffa-f68c1ad30052" />
 * The arduino is connected to four LEDs that correspond to the STM's push buttons.

    | Arduino Pin | LED Connection |
    |-------------|-------------|
    | 2           | Red LED     |
    | 3           | Yellow LED  |
    | 4           | Green LED   |
    | 5           | Blue LED    |
 * When a push button is pressed, the STM32 runs an Interrupt Service Routine to send a byte of data to the arduino via the SPI2 peripheral. When the Arduion receives the data, its own Interrupt Service Routine triggers and turns on the corresponding LED.
 * The baud rate for the SPI communication is 500kHz.
 * The pins used for each device are as follows:

    | STM32 GPIO Pin | Arduino Pin | SPI Connection  |
    |----------------|-------------|-----------------|
    | PB12           | 10          | Enable          |
    | PB13           | 13          | Clock           |
    | PB14           | 12          | MISO (Not Used) |
    | PB15           | 11          | MOSI            |

<img width="4000" height="1800" alt="circuit" src="https://github.com/user-attachments/assets/501697c4-bcca-498d-a716-196b0f45e361" />
