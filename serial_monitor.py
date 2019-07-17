# ------------- #
# UART RECEIVER #
# ------------- #

import serial

if __name__ == "__main__":
	
	SERIAL_PORT = "/dev/ttyUSB0"
	BAUD_RATE = 9600
	TIMEOUT=5

	import sys
	if len(sys.argv) == 3:
		porta = sys.argv[1]
		baudrate = int(sys.argv[2])
	else:
		print("Utilizzo la porta seriale e il baudrate di default")
		print("Porta: /dev/ttyUSB0 --- baud rate: 9600")
		porta, baudrate = (SERIAL_PORT, BAUD_RATE)

	serial_port = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)
	serial_port.flush()

	while(1):
		rx_byte = serial_port.read(1)
		if rx_byte == "M":
			print "MPR121 status"
		else:
			print rx_byte

