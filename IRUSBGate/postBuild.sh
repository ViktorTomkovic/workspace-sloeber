#!/bin/bash
MAIN_SKETCH=IRUSBGate
OBJ_DUMP=${SLOEBER_HOME}/arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/avr/bin/objdump
${OBJ_DUMP} -S ${BUILD_PATH}/${MAIN_SKETCH}.elf > ${BUILD_PATH}/${MAIN_SKETCH}.S
${OBJ_DUMP} -d ${BUILD_PATH}/${MAIN_SKETCH}.elf | awk -v RS= '/^[[:xdigit:]]+ [[:graph:]]+timeCritical/' | wc -l  > ${BUILD_PATH}/instructionCount.txt