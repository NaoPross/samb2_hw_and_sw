CC=arduino
BOARD=arduino:avr:nano:cpu=atmega328

SOURCE=src/ultrasonic/ultrasonic.ino

build:
	$(CC) --verify --board $(BOARD) -v $(SOURCE)

flash:
	$(CC) --upload --board $(BOARD) -v $(SOURCE)
