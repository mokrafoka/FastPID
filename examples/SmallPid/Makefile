
# make sure this points to a avr-gcc that supports c++20 at least
GCC_PATH ?= /usr/bin/

native: CXX = $(GCC_PATH)/g++
avr: CXX = $(GCC_PATH)/avr-g++
MCU = atmega328p

CXXFLAGS += -MMD
avr: CXXFLAGS += -mmcu=$(MCU) -Wall -Wextra --pedantic -std=c++20 -Os -flto -ffunction-sections -fdata-sections -Wl,--gc-sections
native: CXXFLAGS += -g -ggdb -std=c++20

CPPFLAGS = -I ../../src

all : native

avr: size
native: smallPid smallPidControl

size : smallPid
	$(GCC_PATH)/avr-objdump -Pmem-usage $<

clean :
	rm -f smallPid smallPidControl

.PHONY: clean

#-include smallPidControl.d smallPid.d
