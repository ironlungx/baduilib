#pragma once
#include <cstddef>
#include <cstdint>
#include <baduilib.h>

class Setting;

struct StringSetting {
  const char *str;
  size_t max_len; // NOTE: I don't think we need this
  const char *placeholder;
};

struct IntSetting {
  int *value;
  int min;
  int max;
  int step;
};

struct BoolSetting {
  bool *state;
};

struct SubmenuSetting {
  Scene *submenu;
};

struct ButtonSetting {
  void (*callback)(void *);
  void *param;
};

struct InfoSetting {
  bool center;
  bool header;
};

constexpr int MAX_ENUM_ITEMS = 8; // Probably a bad idea to have more that 8 enum items
                                  // because it isn't legible to scroll th much
struct EnumSetting {
  const char *items[MAX_ENUM_ITEMS]; // ["C1", "C2", "C3", "C4", ... "C8"]
  uint8_t n_items;                   // How many items of the max MAX_ENUM_ITEMS are being used
                                     // BUG: If we have more than 255 items for some reason, this won't work
  uint8_t *selected;                 // Currently selected index
};

enum class SettingType {
  STRING, // "Hello"
  INT,    // 281
  BOOL,   // true/false
  BUTTON, // Callback
  ENUM,   // Scrollable list

  INFO,      // Not focusable
  SEPARATOR, // -----------

  SUBMENU, // Links to another page
  BACK,    // Go back a page
  EMPTY    // Not initialized yet
};

union SettingValue {
  StringSetting _string;
  IntSetting _int;
  BoolSetting _bool;
  SubmenuSetting _subm;
  ButtonSetting _btn;
  EnumSetting _enum;
  InfoSetting _info;
};
