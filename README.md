Bitcoin-Mate conversion rate display
====================================

Building the Arduino image
--------------------------

	$ wget https://github.com/dnet/arscons/raw/master/SConstruct
	$ scons build

Uploading the Arduino image
---------------------------

### Default config ###

	$ scons upload

### Example for ATmega168-based boards ###

	$ ARDUINO_BOARD=atmega168 scons upload

Connections between the Arduino and the LPT port
------------------------------------------------

 - LPT data bit 0 (DB-25 pin 2) to Arduino analog pin 0
 - LPT data bit 1 (DB-25 pin 3) to Arduino analog pin 1
 - LPT data bit 2 (DB-25 pin 4) to Arduino analog pin 2
 - LPT ground (DB-25 pin 20) to Arduino ground (any one of them)

Dependencies for arscons build process
--------------------------------------

 - Arduino 1.0+ (if you have an older version, rename `*.{ino -> pde}`)
 - Scons (Debian/Ubuntu package: `scons`)

Building prelay
---------------

Compilation is pretty straightforward using GCC, but you have to set
the set-UID (SUID) bit in order to use the tool without root privileges
(and you don't want any network communication with root, right?)

	# gcc prelay.c -Wall -o prelay
	# chmod +s prelay

Running it regularly
--------------------

Just put the contents of `example.crontab` into your crontab to query
and display the current value of 2 EUR in mBTC on the display.

Since bitcoinchars.com is a free service, don't query it too often, as
they may ban you from using it by your IP address.

License
-------

The whole project is available under MIT license, see `LICENSE.txt`.
