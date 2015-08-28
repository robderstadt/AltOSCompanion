# AltOSCompanion
An Arduino library for the companion board interface available on <a href="http://altusmetrum.org">AltusMetrum</a> flight computers.

## What is the AltOSCompanion Library? ##
<a href="http://altusmetrum.org">AltusMetrum</a> flight computers like the <a href="http://altusmetrum.org/TeleMetrum/">TeleMetrum</a> have a companion interface that can be used to feed auxilary data to the ground station over the RF link. This data is captured and displayed by AltOS. This library allows you to use an Arduino as a companion board that communicates with the flight computer over that interface. This means you can use your Arduino to capture sensor readings (like adding an IMU to display roll/pitch/yaw; see example) and have that information transmitted to the ground station before, during and after launch. 

## How do I use it? ##

1. You'll need a flight computer from <a href="http://altusmetrum.org">AltusMetrum</a>. This code was tested on the TeleMetrum. In addition, the shipping models do not contain a header for the companion port on the board. You'll need to get an 8-pin MicroMaTch connector and solder it in place on your board (this isn't too hard, just be careful and take your time). It's probably also a good idea to get the matching  8-pin MicroMaTch connector that can be crimped onto standard ribbon cable so you can connect it to your Arduino. The other end of the cable can be whatever you'd like to use on the Arduino side of things, e.g. an IDC connector.

2. TBD...
