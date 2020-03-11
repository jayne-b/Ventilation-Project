/*
 * PropertyEdit.h
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#ifndef PROPERTYEDIT_H_
#define PROPERTYEDIT_H_
#include <atomic>

extern std::atomic_int counter, timer;
extern std::atomic<bool> mode;		//false - manual mode, true - auto mode
extern std::atomic<bool> mainMenu;


class PropertyEdit {
public:
	virtual ~PropertyEdit() {};
	virtual void increment() = 0;
	virtual void decrement() = 0;
	virtual void accept() = 0;
	virtual void cancel() = 0;
	virtual void setFocus(bool focus) = 0;
	virtual bool getFocus() = 0;
	virtual void display() = 0;
	virtual void displayAuto() = 0;
	virtual void displayMan() = 0;
};

#endif /* PROPERTYEDIT_H_ */
