#ifndef _LIBBLRS_BUTTONS_H_
#define _LIBBLRS_BUTTONS_H_

#include <stdbool.h>

/// Represents all available
typedef enum
{
  JOY1_5D = 0,
  JOY1_5U = 1,
  JOY1_6D = 2,
  JOY1_6U = 3,
  JOY1_7U = 4,
  JOY1_7L = 5,
  JOY1_7R = 6,
  JOY1_7D = 7,
  JOY1_8U = 8,
  JOY1_8L = 9,
  JOY1_8R = 10,
  JOY1_8D = 11,

  JOY2_5D = 12,
  JOY2_5U = 13,
  JOY2_6D = 14,
  JOY2_6U = 15,
  JOY2_7U = 16,
  JOY2_7L = 17,
  JOY2_7R = 18,
  JOY2_7D = 19,
  JOY2_8U = 20,
  JOY2_8L = 21,
  JOY2_8R = 22,
  JOY2_8D = 23,

  LCD_LEFT = 24,
  LCD_CENT = 25,
  LCD_RIGHT = 26
} button_t;

/**
 * @brief Initializes the buttons array for keeping states.
 */
void initButtons();

/**
 * @brief Detects if button is a new press from most recent check by comparing previous
 *        value to current value.
 *
 * @param button
 *        The button to detect from the button_t enumeration (see include/buttons.h).
 *
 * @return true or false depending on if there was a change in button state.
 *
 * Example code:
 * @code
 *    ...
 *    if(buttonIsNewPress(JOY1_8D))
 *      digitalWrite(1, !digitalRead(1));
 *    ...
 * @endcode
 */
bool buttonIsNewPress(button_t button);

/**
 * @brief Gets the current state of a button (pressed or not). Essentially acts as a wrapper
 *        function for digitalRead, etc. but at this layer of abstraction using button_t
 *
 * @param button
 *        The button to detect from the button_t enumeration (see include/buttons.h).
 *
 * @return true or false depending on if there was a change in button state.
 *
 * Example code:
 * @code
 *    ...
 *    if(buttonGetState(JOY1_8D))
 *      digitalWrite(1, !digitalRead(1));
 *    ...
 * @endcode
 */
bool buttonGetState(button_t b);

#endif
