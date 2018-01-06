/**
 * @file libLCD
 * @brief allows for selection of autonomous routines with the LCD
 */
#ifndef _LCD_H_
#define _LCD_H_

#include <API.h>

// ARRAYS OF SCRIPTS AND TITLES, NEED TO BE DEFINED ELSEWHERE //
#define NUM_SCRIPTS 2
extern const char* titles[];
extern void (*scripts[])();

// SCRIPT SELECTION //

/**
 * @brief Initializes the LCD on the given port and sets the port for future use with liblcd.
 *
 * @param port
 *        The uart port for the LCD to be initialized.
 */
void lcdScriptInit(FILE* port);

/**
 * @brief Runs the LCD Script Selection process. This runs in a task and will not prevent normal execution.
 */
void lcdScriptSelect();

/**
 * @brief Runs the selected autonomous script. Nothing will run if a script was not selected with lcdScriptSelect().
 */
void lcdScriptExecute();

// GENERAL USE //

/**
 * @brief Prints to the LCD initialized with lcdScriptInit with the text centered on the display.
 */
void lcdPrintCentered(unsigned char line, const char* string);

#endif
