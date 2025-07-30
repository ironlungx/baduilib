#pragma once
#include "baduilib.h"

class Keyboard : public Scene {
public:
  enum class Type {
    NUMERIC,    // 0-9, +, -, .
    TEXT_FULL,  // all printable characters
    TEXT_ALPHA, // a-z, A-Z only
  };

private:
  Type t = Type::TEXT_FULL;
  union {
    struct {
      const char *val;
      size_t len;
    } string;
    int *val;
  } target;

  struct TextInputKey {
    const char text;
    const uint8_t x;
    const uint8_t y;
  };

  static constexpr char ENTER_KEY = '\r';
  static constexpr char BACKSPACE_KEY = '\b';

  static const TextInputKey keyboard_keys_row_1[];
  static const TextInputKey keyboard_keys_row_2[];
  static const TextInputKey keyboard_keys_row_3[];

  static const uint8_t keyboard_origin_x = 1;
  static const uint8_t keyboard_origin_y = 29;

public:
  Keyboard(Type t);

  void setDisplay(u8g2_t * u8g2);
  void setInt(int *val);
  void setStr(const char *val, size_t max_len);

  void render();
  

  void handleInput(ActionType t) {}
};
