# MouseMover
Arduino program to move a computer's mouse cursor at random intervals to follow randomly generated cubic bezier curves.

Assuming you define the correct screen resolution in the program, the generated curves should never try to go outside the edges of the screen.

I would strongly reccomend unplugging the arduino from the computer USB side rather than from the arduino side since the arduino usb ports tend to fall off with heavy use.

You will need to use an Arduino which has HID support such as the Pro Micro or the Leonardo.
The Arduino Nano / UNO might work if you upload a custom bootloader like https://github.com/NicoHood/HoodLoader2, but I didn't bother with that and I just opted for a Pro Micro. If you choose not to go with a pro micro you may need to adjust or remove some of the led indicator code.

The computer you plan to plug into will need to support absolute mouse mode. I tested on Windows 10, Ubuntu and Mac, all worked great. Also, the computer you plan to use to program the Arduino will need to have AbsMouse installed into the Arduino IDE library manager. https://github.com/jonathanedgecombe/absmouse

If you plan on using a pro micro, it may be a good idea to load the sparkfun avr boards into your boards manager, I had some issues programming the pro micro using the "micro" and "leonardo" boards from the built in boards list. Here is a guide I found helpful for that: https://www.shellhacks.com/arduino-pro-micro-board-selection/

Also, if you need to make the arduino not be detected as an arduino by the computer, have a look at the spoofed boards entry. This makes the arduino appear as a Logitech mouse.

If you are looking for a case to protect your pro micro I've been using this one: https://www.thingiverse.com/thing:4764638
