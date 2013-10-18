from __future__ import with_statement
from subprocess import check_call
from time import sleep
from os import devnull
from urllib2 import urlopen
import json

DATA = "4"
CLOCK = "3"
RESET = "2"
DELAY = 0.1

def set_bit(bit, value):
	with file(devnull, 'w') as dn:
		return check_call(["/path/to/prelay", "on" if value else "off", bit], stdout=dn)

def pulse_bit(bit):
		set_bit(bit, True)
		sleep(DELAY)
		set_bit(bit, False)

def echo_byte(byte):
	for bit in xrange(8):
		set_bit(DATA, (128 >> bit) & byte)
		pulse_bit(CLOCK)

def echo_digits(digits):
	pulse_bit(RESET)
	for digit in digits:
		sleep(DELAY)
		echo_byte(ord(digit))
	set_bit(DATA, False)

if __name__ == '__main__':
	prices = json.load(urlopen("http://bitcoincharts.com/t/weighted_prices.json"))
	eur24 = 2 / float(prices["EUR"]["24h"])
	echo_digits(str(int(round(eur24 * 1000))).rjust(4, '.'))
