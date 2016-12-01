/**
 * @file buttons.c
 * @author Elliot Berman, Brian Hanford
 *
 * @details The Buttons API enables press-once to trigger control, allowing button pressed to be made which act like taps.
 * The API works by holding a boolean "wasPressed" for each button and when buttonIsNewPress() is called,
 * the method compares whether the press is a new press that has not previously been detected. <br>
 * <br>
 * This is useful in applications where the operator needs to press a button to change a state, but does not want
 * the state to to change the state every time the button is checked. For example, toggling a solenoid on/off with
 * a single button. <br>
 *
 ********************************************************************************/

#include "API.h"
#include "buttons.h"

/**
 * Represents the array of "wasPressed" for all 27 available buttons.
 */
bool buttonPressed[27];

void initButtons()
{
  for (int i = 0; i <= 27; i++)
    buttonPressed[i] = false;
}

bool buttonGetState(button_t button) {
  bool currentButton = false;

  // Determine how to get the current button value (from what function) and where it
  // is, then get it.
  if (button < LCD_LEFT) // button is a joystick button
  {
    unsigned char joystick;
    unsigned char buttonGroup;
    unsigned char buttonLocation;

    button_t newButton;
    if (button <= 11)
    { // button is on joystick 1
      joystick = 1;
      newButton = button;
    }
    else
    { // button is on joystick 2
      joystick = 2;
      // shift button down to joystick 1 buttons in order to
      // detect which button on joystick is queried
      newButton = (button_t)(button - 12);
    }

    switch (newButton)
    {
    case 0:
      buttonGroup = 5;
      buttonLocation = JOY_DOWN;
      break;
    case 1:
      buttonGroup = 5;
      buttonLocation = JOY_UP;
      break;
    case 2:
      buttonGroup = 6;
      buttonLocation = JOY_DOWN;
      break;
    case 3:
      buttonGroup = 6;
      buttonLocation = JOY_UP;
      break;
    case 4:
      buttonGroup = 7;
      buttonLocation = JOY_UP;
      break;
    case 5:
      buttonGroup = 7;
      buttonLocation = JOY_LEFT;
      break;
    case 6:
      buttonGroup = 7;
      buttonLocation = JOY_RIGHT;
      break;
    case 7:
      buttonGroup = 7;
      buttonLocation = JOY_DOWN;
      break;
    case 8:
      buttonGroup = 8;
      buttonLocation = JOY_UP;
      break;
    case 9:
      buttonGroup = 8;
      buttonLocation = JOY_LEFT;
      break;
    case 10:
      buttonGroup = 8;
      buttonLocation = JOY_RIGHT;
      break;
    case 11:
      buttonGroup = 8;
      buttonLocation = JOY_DOWN;
      break;
    default:
      break;
    }
    currentButton = joystickGetDigital(joystick, buttonGroup, buttonLocation);
  }
  else // button is on LCD
  {
    if (button == LCD_LEFT)
      currentButton = (lcdReadButtons(uart1) == LCD_BTN_LEFT);

    if (button == LCD_CENT)
      currentButton = (lcdReadButtons(uart1) == LCD_BTN_CENTER);

    if (button == LCD_RIGHT)
      currentButton = (lcdReadButtons(uart1) == LCD_BTN_RIGHT);
  }
  return currentButton;
}

bool buttonIsNewPress(button_t button)
{
  bool currentButton = buttonGetState(button);

  if (!currentButton) // buttons is not currently pressed
    buttonPressed[button] = false;

  if (currentButton && !buttonPressed[button])
  { // button is currently pressed and was not detected as being pressed during last check
    buttonPressed[button] = true;
    return true;
  }
  else return false; // button is not pressed or was already detected
}
