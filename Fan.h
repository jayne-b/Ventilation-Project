/*
 * Fan.h
 *
 *  Created on: Mar 6, 2020
 *      Author: thiag
 */

#ifndef FAN_H_
#define FAN_H_

#include "ModBusRegister.h"
#include "ModBusMaster.h"

void Sleep(int ms);

class Fan {
public:
	Fan();
	virtual ~Fan();

	bool setFrequency(uint16_t freq);
private:
	ModbusMaster node;
};

#endif /* FAN_H_ */
