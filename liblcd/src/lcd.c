/**
 * @file libLCD
 */
#include "lcd.h"
#include "string.h"

#define LCD_TASK_DELTA 150

static FILE* lcdPort;
static int selection;

void lcdPrintCentered(unsigned char line, const char* string) {
	char out[16];
	for (int i = 0; i < 16; i++)
		out[i] = ' ';
	for (unsigned int i = (16 - (strlen(string) % 16)) / 2, j = 0; j < strlen(string); i++, j++)
		out[i] = string[j];

	lcdPrint(lcdPort, line, out);
}

void lcdScriptInit(FILE* port) {
	lcdInit(port);
	lcdClear(port);
	lcdSetBacklight(port, true);
	lcdPort = port;
}

void _lcdScriptSelect() {
	unsigned int scriptNumber = 0;
	lcdPrintCentered(1, "<Script>");
	selection = -1;
	while (selection == -1) {
		unsigned int button = lcdReadButtons(lcdPort);
		if (button == 1) {
			if (scriptNumber != 0)
				scriptNumber--;
		}

		else if (button == 4) {
			if (scriptNumber < (NUM_SCRIPTS - 1))
				scriptNumber++;
		}

		if (button == 2) {
			selection = scriptNumber;
			lcdPrintCentered(1, "<Selected>");
		}

		lcdPrintCentered(2, titles[scriptNumber]);
		delay(150);
	}
  // Wait until comp control is changed
  while (true) {
    delay(20);
  }
}

void lcdScriptSelect() {
	taskRunLoop(_lcdScriptSelect, LCD_TASK_DELTA);
}

void lcdScriptExecute() {
	if (selection < 0)
		return; // Don't run a script if one has not been selected.
	scripts[selection]();
}
