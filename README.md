# MouseMover
Arduino program to move a computer's mouse cursor at specified intervals to follow randomly generated cubic bezier curves.

You will need to use an Arduino which has HID support such as the Pro Micro or the Leonardo.
The Arduino Nano / UNO might work if you upload a custom bootloader like https://github.com/NicoHood/HoodLoader2, but I didn't bother with that and I just opted for a Pro Micro.

The computer you plan to plug into will need to support absolute mouse mode. I tested on Windows 10 and Ubuntu, both worked great. Also, the computer you plan to use to program the Arduino will need to have AbsMouse installed into the Arduino IDE library manager. https://github.com/jonathanedgecombe/absmouse

Assuming you define the correct screen resolution, the generated curves should never try to go outside the edges of the screen.
