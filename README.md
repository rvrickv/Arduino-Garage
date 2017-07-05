# Arduino-Garage
Parking distance sensor using ESP8266, HC-SR04, and Addressable LED strips/rings

I started this project to solve a personal issue -- parking the car just right so that I have space to walk around it in the garage while leaving a reasonable gap to the garage door.

It started as a Python/Pi Zero project and now I have decided to port it to Arduino/ESP8266. Eventually I plan to move it to an ATTiny85 for an even smaller footprint/power profile.

Two important notes:
1) I've got very limited coding skills
2) I have no idea what I'm doing

So if you have a better way of doing some of this stuff, I'm totally open to suggestions.

The biggest challenge so far is "noise" in the sensor data. They are rock solid with large/flat objects. But can struggle with curvy car shapes and especially with empty spaces or cluttered garages. I've tried many techniques to smooth the data but now am running into performance issues with the MCUs. It will likely be magnified with the ATTiny85.

The code uses a few awesome libraries that made this project super easy. Among them:

NewPing   // sensor library. Much more flexible than the UltraSonic library in Arduino

FastLED   // library for controlling individually addressable LEDs

<H1>Usage</h1>

Usage is simple. A parking distance is set by default (100cm). As the car approaches the sensor, the LED strip/ring fills with Green LEDs, until the parking position is reached, where the LEDs turn Red.

After a few seconds parked (Red), the LEDs fade out.

To change the distance, begin by parking your car at the intended parking distance. Put your hand over the sensor at about 5 cm. The LEDs will change to a rainbow pattern. Keep your hand over the sensor until a countdown starts. Move your hand out of the way, and the MCU will take several readings, and set the parking distance. The distance is written to EEPROM so it is not lost if power is cycled.

That's it!

I'll be adding schematics and more soon.

Thanks for stopping by.

rvrickv
