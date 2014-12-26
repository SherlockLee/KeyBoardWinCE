#ifndef _MODBUS_TCP_H
#define _MODBUS_TCP_H


// 04 Read Input Registers
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 100

// 03 Read Holding Registers
#define REG_HOLDING_START 100
#define REG_HOLDING_NREGS 100

// 02 Read Input Status
#define INPUT_STATUS_START	100
#define INPUT_STATUS_NREGS	100

// 01 Read Coil Status
#define INPUT_COIL_START	100
#define INPUT_COIL_NREGS	100


int StartModbusTcp();
void StopModbusTcp();







#endif