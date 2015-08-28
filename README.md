# AltOSCompanion
An Arduino library for the companion board interface available on <a href="http://altusmetrum.org">AltusMetrum</a> flight computers.

## What is the AltOSCompanion Library? ##
<a href="http://altusmetrum.org">AltusMetrum</a> flight computers like the <a href="http://altusmetrum.org/TeleMetrum/">TeleMetrum</a> have a companion interface that can be used to feed auxilary data to the ground station over the RF link. This data is captured and displayed by AltOS. This library allows you to use an Arduino as a companion board that communicates with the flight computer over that interface. This means you can use your Arduino to capture sensor readings (like adding an IMU to display roll/pitch/yaw; see example) and have that information transmitted to the ground station before, during and after launch. 

## How do I use it? ##

1. You'll need a flight computer from <a href="http://altusmetrum.org">AltusMetrum</a>. This code was tested on the TeleMetrum. In addition, the shipping models do not contain a header for the companion port on the board. You'll need to get an 8-pin MicroMaTch connector and solder it in place on your board (this isn't too hard, just be careful and take your time). It's probably also a good idea to get the matching  8-pin MicroMaTch connector that can be crimped onto standard ribbon cable so you can connect it to your Arduino. The other end of the cable can be whatever you'd like to use on the Arduino side of things, e.g. an IDC connector.

2. For now, you'll also need to download and build your own copy of AltOS and the driver for your board from <a href="http://git.gag.com/fw/altos"> http://git.gag.com/fw/altos</a>. Version 1.6.1 does not have companion board support enabled, although the latest source in master has corrected this.It's easiest to setup a build environment on Debian using `sudo apt-get build-dep altos` to get started (running VirtualBox on OSX with a Debian VM will work just fine). You'll want to build and install the image `altos/src/telemetrum-v2.0/telemetrum-v2.0-1.6.1.ihx` on your TeleMetrum and then launch your new build of AltOS UI using `./altosui-test` from the `altos\altosui` directory. If you do not do this you're TeleMetrum will still connect to the companion board, but will not transmit data over the RF link and the UI will not display the companion data.

3. Hook up your Arduino as a SPI slave to the flight computer using the companion port. You can look at the <a href="http://altusmetrum.org/TeleMetrum/v2.0/telemetrum-sch.pdf">schematic for your flight computer</a> to see how the companion port pins are setup. Basically, you just need to attach the  MOSI2, MISO2, SCK2 and SC_COMPANION0 lines to the Arduino's pins 11, 12, 13 and 10. If your Arduino runs at 5v you'll need to run all of these connections through a <a href="https://www.sparkfun.com/products/12009">logic level converter</a>.

4. Setup your Arduino to collect data from a sensor (the example code uses the Adafruit 10DOF IMU)

