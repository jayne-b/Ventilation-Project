/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */


#include "chip.h"
#include "board.h"

#include <cr_section_macros.h>
#include "DigitalIoPin.h"
#include "I2C.h"
#include "LiquidCrystal.h"
#include "SerialPort.h"
#include "LpcUart.h"
#include "IntegerEdit.h"
#include "SimpleMenu.h"
#define TICKRATE_HZ 1000

SimpleMenu *menu;
std::atomic_int counter, timer;


static IntegerEdit* fan, *pressure;

std::atomic<bool> mode { false };		//false = auto, true = manual
std::atomic<bool> mainMenu { false };

extern "C" {
void PIN_INT0_IRQHandler(void) {
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
	if (menu != nullptr) {
		menu->event(SimpleMenu::up);
	}
}


void PIN_INT1_IRQHandler(void) {
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
	if (menu != nullptr) {
		menu->event(SimpleMenu::down);
	}
}

void PIN_INT2_IRQHandler(void) {
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));
	if (menu != nullptr) {
		mainMenu = true;
		mode = !mode;
		menu->event(SimpleMenu::show);
		timer=0;
	}
}
}


extern "C" {
void SysTick_Handler(void) {
	if(counter > 0) counter--;
	if (++timer > 2000 && mainMenu) {
		mainMenu = false;
		menu->event(SimpleMenu::show);
	}
}
}

void Sleep(int ms) {
	counter = ms;
	while(counter > 0) {
		__WFI();
	}
}


int main(void) {

	SystemCoreClockUpdate();
	Board_Init();
	Chip_Clock_SetSysTickClockDiv(1);
	SysTick_Config(Chip_Clock_GetSysTickClockRate() / TICKRATE_HZ);
	Chip_PININT_Init(LPC_GPIO_PIN_INT);
	Chip_RIT_Init(LPC_RITIMER);

	//testing on board
	DigitalIoPin sw1(0, 17, true, true, true), sw2(1, 11, true, true, true), sw3(1, 9, true, true, true);

	//DigitalIoPin B1(0,9, true, true, true), B2(0,10,true,true, true), B3(0, 16, true, true, true);
	//B1 - Mode/OK, B2 - up, B3 - down
	DigitalIoPin rs(0,8, false), en(1,6,false), d4(1, 8, false), d5(0,5,false), d6(0,6,false), d7(0,7,false);
	LiquidCrystal *lcd = new LiquidCrystal(&rs, &en, &d4, &d5, &d6, &d7);

	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PININT);

	/* Reset the PININT block */
	Chip_SYSCTL_PeriphReset(RESET_PININT);

	/* Configure interrupt channel for the GPIO pin in INMUX block
	Chip_INMUX_PinIntSel(0, 0, 9); // putting sw1 on int channel 0
	Chip_INMUX_PinIntSel(1, 0, 10); // putting sw1 on int channel 0
	Chip_INMUX_PinIntSel(2, 0, 16); // putting sw1 on int channel 0*/

	/* testing */
	Chip_INMUX_PinIntSel(0, 0, 17); // putting sw1 on int channel 0
	Chip_INMUX_PinIntSel(1, 1, 11); // putting sw2 on int channel 0
	Chip_INMUX_PinIntSel(2, 1, 9); // putting sw3 on int channel 0


	/* Configure channel interrupt as edge sensitive and falling edge interrupt */
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(0));
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(0));
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(1));
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(1));
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(2));
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(2));

	/* Enable interrupt in the NVIC */
	NVIC_ClearPendingIRQ(PIN_INT0_IRQn);
	NVIC_EnableIRQ(PIN_INT0_IRQn);
	NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
	NVIC_EnableIRQ(PIN_INT1_IRQn);
	NVIC_ClearPendingIRQ(PIN_INT2_IRQn);
	NVIC_EnableIRQ(PIN_INT2_IRQn);

	lcd->begin(16,2);
	lcd->setCursor(0,0);
	lcd->print("Starting up");

	menu = new SimpleMenu;
	pressure = new IntegerEdit(lcd, std::string("Pressure"), 0, 120);
	fan = new IntegerEdit(lcd, std::string("Fan"), 0, 100);
	menu->addItem(pressure);
	menu->addItem(fan);


	while(1);

	return 0;
}

