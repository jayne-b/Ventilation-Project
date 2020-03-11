/*
 * IntegerEdit.cpp
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#include "IntegerEdit.h"
#include <cstdio>


IntegerEdit::~IntegerEdit() {
}

void IntegerEdit::increment() {
	if(edit < upper) {
		edit++;
	}
	save();
}

void IntegerEdit::decrement() {
	if(edit > lower){
		edit--;
	}
	save();
}

void IntegerEdit::accept() {
	save();
}

void IntegerEdit::cancel() {
	edit = value;
}


void IntegerEdit::setFocus(bool focus) {
	this->focus = focus;
}

bool IntegerEdit::getFocus() {
	//return this->focus;
	return true;
}

void IntegerEdit::display() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print("Mode: ");
	lcd->setCursor(0,1);
	char s[17];
	snprintf(s, 17, "   %s     ", status[mode]);
	lcd->print(s);
}

void IntegerEdit::displayMan() {
	lcd->clear();
	lcd->setCursor(0,0);
	focus = true;
	char s[17];
	if(focus) {
		snprintf(s, 17, "Set fan: %3d %% ", edit);
	}
	else {
		snprintf(s, 17, "      %4d      ", edit);
	}
	lcd->print(s);
	lcd->setCursor(0,1);
	//lcd->print("Cur pres: %4d pA", getPressure());
	lcd->print("Cur pres: NUM pA");
}

void IntegerEdit::displayAuto() {
	lcd->clear();
	lcd->setCursor(0,0);
	focus = true;
	char s[17];
	if(focus) {
		snprintf(s, 17, "Set pres: %3d pA", edit);
	}
	else {
		snprintf(s, 17, "      %3d      ", edit);
	}
	lcd->print(s);
	lcd->setCursor(0,1);
	//lcd->print("Cur pres: %4d pA", getPressure());
	lcd->print("Cur pres: NUM pA");

}


void IntegerEdit::save() {
	// set current value to be same as edit value
	value = edit;
	// todo: save current value for example to EEPROM for permanent storage
}


int IntegerEdit::getValue() {
	return value;
}

void IntegerEdit::setValue(int value) {
	edit = value;
	save();
}
