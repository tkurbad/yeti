# IR transceiver enabled Arduino bootloader for Atmega 328p

## Flashing the bootloader manually using `avrdude` and a USBtiny AVR programmer

First, set the fuses:
```
avrdude -b19200 -c usbtiny -p m328p -v -e -U efuse:w:0xfd:m -U hfuse:w:0xda:m -U lfuse:w:0xff:m
```

Now, flash the bootloader:
```
avrdude -b19200 -c usbtiny -p m328p -v -e -U flash:w:ATmegaBOOT_168_atmega328ir.hex -U lock:w:0x0f:m
```
