# 64 Button Matrix & LED Strip MIDI for Teensy - controlling LED strips via MIDI

This Arduino project allows you to control a 64 button matrix and LED strips from a DAW program (via MIDI) using a Teensy microcontroller.

## System Requirements

* LED Strip
	* The **WS8218B** strips are implemented in separate Arduino projects. 
* USB Development Board
    * A **32-bit PJRC Teensy** is highly recommended. A [Teensy-LC](https://www.pjrc.com/store/teensylc.html) was used while developing this project and is officially supported alongside other Teensy 3 models.
    * Teensy 2 (8-bit) and certain Arduino boards might work, but have not been tested and are not officially supported.
    * ESP-32 S3
* [Arduino Software](https://www.arduino.cc/en/Main/Software)
    * Developed with **version 1.8.5**, but older versions will likely work.
    * Also requires [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html), and the [FastLED Arduino library](https://github.com/FastLED/FastLED), and the [Bounce2 Arduino library](https://github.com/thomasfredericks/Bounce2)
* OS
    * **Windows, Linux and macOS** are officially supported.
* DAW Software
    * [Ableton Live](https://www.ableton.com/en/live/) is recommended due to its flexibility in creating MIDI Effects.

## Software

If you haven't already, install Teensyduino for Arduino, and the FastLED Library

Modify the Arduino script according to your LED strip configuration (`_nLED` = Number of LEDs < 64 `_dLED` = Data pin)

Under Tools, make sure `USB Type: "MIDI" or "Serial+MIDI"`, `Optimize Code: Fastest` and your board are selected, then compile and upload to your board. You can also change your MIDI device name by editing `name.c`.

## PIN MAP

* LED Strip
	* PIN = 0
	* Power = 5V, Gnd

* Row PIN
	* PIN = 2, 3, 4, 5, 6, 7, 8, 9

* Col PIN
	* PIN = 15, 16, 17, 18, 19, 20, 21, 22

## Matrix
* [Matrix PIN MAP](https://github.com/peeldok/DIY-64_Button_Matrix_MIDI_Device_With_LED/blob/main/64_Button_Matrix_LED_Strip/Matrix%20PIN%20MAP.jpg?raw=true)

## LED Strip
* [LED Strip PIN MAP](https://github.com/peeldok/DIY-64_Button_Matrix_MIDI_Device_With_LED/blob/main/64_Button_Matrix_LED_Strip/LED%20STRIP.jpg?raw=true)
