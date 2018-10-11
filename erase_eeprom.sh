#!/bin/bash

dfu-programmer atmega32u4 erase
dfu-programmer atmega32u4 flash --eeprom quantum/tools/eeprom_reset.hex
