#pragma once
#include "baduilib.h"
#include "settings_types.h"

#include <algorithm>
#include <cstddef>
#include <stdarg.h>

class Menu;

class Setting {
private:
  const char        *title;
  SettingType        type;
  union SettingValue value;
  bool               focusable;

  Setting(const char *t, SettingType ty, bool focusable) :
      title(t),
      type(ty),
      focusable(focusable) {}

public:
  Setting() :
      title(""),
      type(SettingType::EMPTY) {}

  static Setting Separator() {
    Setting s("", SettingType::SEPARATOR, false);
    return s;
  }

  static Setting Bool(const char *title, bool *state) {
    Setting s(title, SettingType::BOOL, true);
    s.value._bool = BoolSetting{state};
    return s;
  }

  static Setting Submenu(const char *title, Scene *submenu) {
    Setting s(title, SettingType::SUBMENU, true);
    s.value._subm = SubmenuSetting{submenu};
    return s;
  }

  static Setting Button(const char *title, void (*callback)(void *), void *params) {
    Setting s(title, SettingType::BUTTON, true);
    s.value._btn = ButtonSetting{callback, params};
    return s;
  }

  static Setting Int(const char *title, int *val, const int min, const int max,
                     const int step = 1) {
    Setting s(title, SettingType::INT, true);
    s.value._int = IntSetting{val, min, max, step};
    return s;
  }

  static Setting String(const char *title, char *str, size_t max_len,
                        const char *placeholder = nullptr) {
    Setting s(title, SettingType::STRING, true);
    s.value._string = StringSetting{str, max_len, placeholder};
    return s;
  }

  static Setting Enum(const char *title, uint8_t *selected, size_t n_items, ...) {
    Setting s(title, SettingType::ENUM, true);

    va_list args;
    va_start(args, n_items);

    for (uint8_t i = 0; i < n_items && i < MAX_ENUM_ITEMS; i++) {
      s.value._enum.items[i] = va_arg(args, const char *);
    }

    s.value._enum.selected = selected;
    s.value._enum.n_items  = n_items;
    va_end(args);
    return s;
  }

  static Setting Info(const char *title, bool center = false, bool heading = false) {
    Setting s(title, SettingType::INFO, false);
    s.value._info = {.center = center, .header = heading};

    return s;
  }

  static Setting Back() { return Setting("Back", SettingType::BACK, false); }

  void onClick() {
    switch (type) {
      case SettingType::STRING: {
      } break;
      case SettingType::INT: {
      } break;
      case SettingType::BOOL: {
        *(value._bool.state) = !*(value._bool.state);
      } break;
      case SettingType::SUBMENU: {
      } break;
      case SettingType::BUTTON: {
        value._btn.callback(
            value._btn.param); // NOTE: Maybe have a return value and flags? probably a bad idea
      } break;
      case SettingType::INFO: {
      } break;
      case SettingType::SEPARATOR: { // Radiohead reference??!!
      } break;
      case SettingType::EMPTY: {
      } break;
      case SettingType::ENUM: {
      } break;
      case SettingType::BACK: {
      } break;
    }
  }

  const char *getTitle() const { return title; }
  SettingType getType() const { return type; }
  bool        isFocusable() const { return focusable; }

  // const getters
  const StringSetting  &getString() const { return value._string; }
  const IntSetting     &getInt() const { return value._int; }
  const BoolSetting    &getBool() const { return value._bool; }
  const SubmenuSetting &getSubmenu() const { return value._subm; }
  const ButtonSetting  &getButton() const { return value._btn; }
  const EnumSetting    &getEnum() const { return value._enum; }
  const InfoSetting    &getInfo() const { return value._info; }

  // non-const getters
  StringSetting  &getString() { return value._string; }
  IntSetting     &getInt() { return value._int; }
  BoolSetting    &getBool() { return value._bool; }
  SubmenuSetting &getSubmenu() { return value._subm; }
  ButtonSetting  &getButton() { return value._btn; }
  EnumSetting    &getEnum() { return value._enum; }
  InfoSetting    &getInfo() { return value._info; }
};

struct MenuConfig {
  struct {
    uint16_t left   = 5;
    uint16_t right  = 5;
    uint16_t bottom = 2;
    uint16_t top    = 2;
  } margin;

  uint16_t item_padding = 2;

  struct {
    uint16_t width  = 3;    // Width of the slider
    bool     round  = true; // Makes the slider round yaaya
    bool     dotted = true; // Have a dotted line instead of a solid line in the bg.
  } scrollbar;

  const uint8_t *title_font = u8g2_font_helvB08_tr;
  const uint8_t *item_font  = u8g2_font_haxrcorp4089_tr;
  const uint8_t *value_font = u8g2_font_4x6_tr;
};

class Menu : public Scene {
  void updateViewportSize();

  size_t highlight_idx;  // highlighted item
  size_t viewport_start; // index to begin rendering the viewport
  size_t viewport_size;  // size of the viewport

  const char *title;
  Setting    *root;
  size_t      n_root;

  MenuConfig *config;

  size_t getViewportEnd() const { return std::min(viewport_start + viewport_size, n_root); }

public:
  Menu(const char *root_header, Setting *root, size_t n_root, MenuConfig *config);

  void setDisplay(u8g2_t *u8g2);

  void reset();
  void render();
  void handleInput(ActionType t);

  void setConfig(MenuConfig c) {}
};
