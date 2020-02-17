# Morse Decoder for Aurduino

![Board](https://github.com/UnalignedByte/morse-decoder/raw/master/board.jpg)

## What is it
This project allow you to enter a morse code message using a button. It decodes the input and displays it on the LCD screen.

## How does it work
There are two LEDs, the first one lights up straithg away, indicating a dot. If the button is held, the second LED will be lit, indicating a dash. Up to 5 dots/dashes can be entered. Afterwards (or if no further input is performed), both LEDs will blink shortly, input sequence will be decoded and the result will be displayed on the LCD.