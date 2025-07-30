#include "keyboard.h"

#define LEN(x) (sizeof(x) / sizeof((x)[0]))

static const unsigned char image_KeyBackspace_bits[] U8X8_PROGMEM = {
    0xfe, 0x7f, 0x01, 0x80, 0x11, 0x80, 0x19, 0x80, 0xfd,
    0xbf, 0x19, 0x80, 0x11, 0x80, 0x01, 0x80, 0xfe, 0x7f};

static const unsigned char image_KeySave_bits[] U8X8_PROGMEM = {
    0xfe, 0xff, 0x7f, 0x01, 0x00, 0x80, 0x01, 0x00, 0x80, 0x31, 0x97,
    0x8c, 0x89, 0x94, 0x92, 0x91, 0x94, 0x9e, 0xa1, 0x94, 0x82, 0x19,
    0x67, 0x8c, 0x01, 0x00, 0x80, 0x01, 0x00, 0x80, 0xfe, 0xff, 0x7f};

// clang-format off
const Keyboard::TextInputKey Keyboard::keyboard_keys_row_1[] = {
  {'q', 1, 8},
  {'w', 10, 8},
  {'e', 19, 8},
  {'r', 28, 8},
  {'t', 37, 8},
  {'y', 46, 8},
  {'u', 55, 8},
  {'i', 64, 8},
  {'o', 73, 8},
  {'p', 82, 8},
  {'0', 91, 8},
  {'1', 100, 8},
  {'2', 110, 8},
  {'3', 120, 8},
};

const Keyboard::TextInputKey Keyboard::keyboard_keys_row_2[] = {
  {'a', 1, 20},
  {'s', 10, 20},
  {'d', 19, 20},
  {'f', 28, 20},
  {'g', 37, 20},
  {'h', 46, 20},
  {'j', 55, 20},
  {'k', 64, 20},
  {'l', 73, 20},
  {Keyboard::BACKSPACE_KEY, 82, 12}, // Use Keyboard::BACKSPACE_KEY
  {'4', 100, 20},
  {'5', 110, 20},
  {'6', 120, 20},
};

const Keyboard::TextInputKey Keyboard::keyboard_keys_row_3[] = {
  {'z', 1, 32},
  {'x', 10, 32},
  {'c', 19, 32},
  {'v', 28, 32},
  {'b', 37, 32},
  {'n', 46, 32},
  {'m', 55, 32},
  {'_', 65, 32},
  {Keyboard::ENTER_KEY, 74, 23}, // Use Keyboard::ENTER_KEY
  {'7', 100, 32},
  {'8', 110, 32},
  {'9', 120, 32},
};
// clang-format on

Keyboard::Keyboard(Type t) :
    t(t) {}

void Keyboard::setDisplay(u8g2_t *u8g2) {}

void Keyboard::render() {
}
