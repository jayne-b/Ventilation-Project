/*
 * IntegerEdit.h
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#ifndef INTEGEREDIT_H_
#define INTEGEREDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include <string>

const char status[2][7] {"Auto", "Manual"};

class IntegerEdit: public PropertyEdit {
public:
	IntegerEdit(LiquidCrystal* lcd, std::string title, int lower, int upper) : lcd(lcd), title(title), value{ lower }, edit{ lower }, focus{ false },
	lower(lower), upper(upper) {}
	virtual ~IntegerEdit();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	bool getFocus();
	void display();
	void displayAuto();
	void displayMan();
	int getValue();
	void setValue(int value);
	void setCallback(void(*callback_)()) { callback = callback_; }
private:
	void save();
	void displayEditValue();
	LiquidCrystal *lcd;
	std::string title;
	int value;
	int edit;
	bool focus;
	int lower;
	int upper;
	void(*callback)();
};

#endif /* INTEGEREDIT_H_ */
