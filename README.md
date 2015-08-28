# AltOSCompanion
An Arduino library for the companion board interface available on <a href="http://altusmetrum.org">AltusMetrum</a> flight computers.

## What is the AltOSCompanion Library? ##
<a href="http://altusmetrum.org">AltusMetrum</a> flight computers like the <a href="http://altusmetrum.org/TeleMetrum/">TeleMetrum</a> have a companion interface that can be used to feed auxilary data to the ground station over the RF link. This data is captured and displayed by <a href="http://altusmetrum.org/AltOS/">AltOS</a> UI. This library allows you to use an Arduino as a companion board that communicates with the flight computer over that interface. This means you can use your Arduino to capture sensor readings (like adding an IMU to display roll/pitch/yaw; see example) and have that information transmitted to the ground station before, during and after launch. <p/>


<img src="https://github.com/robderstadt/AltOSCompanion/blob/master/images/TeleMetrum_ArduinoCompanion.JPG"/>

## How do I use it? ##

1. You'll need a flight computer from <a href="http://altusmetrum.org">AltusMetrum</a>. This code was tested on the TeleMetrum. The current models do not contain a header for the companion port on the board so you'll need to get an 8-pin MicroMaTch connector and solder it in place (this isn't too hard, just be careful and take your time). It's  also a good idea to get a matching  8-pin MicroMaTch connector that can be crimped onto standard ribbon cable so you can connect it to your Arduino. The other end of the cable can be whatever you'd like to use on the Arduino side of things, e.g. an IDC connector. My example uses a larger IDC connector that I sacrificed (see photo).

2. For now, you'll also need to download and build your own copy of AltOS and the driver for your board from <a href="http://git.gag.com/fw/altos"> http://git.gag.com/fw/altos</a>. Version 1.6.1 does not have companion board support enabled, although the latest source in master has corrected this.It's easiest to setup a build environment on Debian using `sudo apt-get build-dep altos` to get started (running VirtualBox on OSX with a Debian VM will work just fine). You'll want to build and install the image `altos/src/telemetrum-v2.0/telemetrum-v2.0-1.6.1.ihx` on your TeleMetrum and then launch your new build of AltOS UI using `./altosui-test` from the `altos\altosui` directory. If you do not do this the TeleMetrum will still connect to the companion board, but will not transmit data over the RF link and the UI will not display the companion data.

3. Hook up your Arduino as a SPI slave to the flight computer using the companion port. You can look at the <a href="http://altusmetrum.org/TeleMetrum/v2.0/telemetrum-sch.pdf">schematic for your flight computer</a> to see how the companion port pins are setup. Basically, you just need to attach the  MOSI2, MISO2, SCK2 and SC_COMPANION0 lines to the Arduino's pins 11, 12, 13 and 10. If your Arduino runs at 5v you'll need to run all of these connections through a <a href="https://www.sparkfun.com/products/12009">logic level converter</a>.

4. Add the AltOSCompanion library to your Arduino project. Setup your Arduino to collect data from a sensor (the example code uses the <a href="https://www.adafruit.com/product/1604">Adafruit 10DOF IMU</a>) or whatever data source you'd like to use. In your setup routine, configure your Arduino as a companion board:
  
  `AltOSCompanion::configure(companion_id, fetch_rate, num_channels);`

  The companion id can be any unsigned integer value you'd like to use to identify your board. For some reason only    the later byte is displayed in AltOS. 

  The fetch rate determines how frequently you'd like the flight computer to request data from your Arduino. This      value is the minimum number of 100Hz clock ticks that occur between requests. The flight computer will wait <i>at    least</i> this long before requesting more data from your Arduino. On my TeleMetrum, setting this to 1 resulted in   a fetch request from the flight computer every 18.53ms (52.72Hz). Keep in mind that telemetry from the flight        computer is only sent every second while the rocket is on the pad and every 10ms while in flight. 

  Finally, the number of channels should be specified. Each channel is two bytes (displayed as an unsigned int         through AltOS). There is a maximum of 16 channels. You'll need to consider how you want to encode your data, i.e.    sending floats cannot be done w/o some kind of conversion on each side of the link. In the example code, the IMU     values are floats that are normalized, multiplied by 1000 and sent as unsigned ints (and need to be decoded as       such).

  In order to actually set data, call:

  `AltOSCompanion::updateChannel(channel, value);`

  The channel is the zero-based channel number (i.e. 0 for channel 1, 1 for channel 2, etc). The value is whatever     two bytes you'd like sent over the link. The library currently ignores this call when the flight computer is         actively fetching companion data.

5. Fire up AltosUI, start your Arduino, then start your flight computer (in pad mode). Once a link is established you should see a "companion" tab show up in AltosUI displaying data from your companion board, similar to this:

<img src="https://github.com/robderstadt/AltOSCompanion/blob/master/images/AltosUI_Companion.png"/>




