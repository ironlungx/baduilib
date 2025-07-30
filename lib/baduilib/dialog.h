#pragma once
#include "baduilib.h"

class Dialog : public Scene {
  uint8_t num_buttons; // 2 or 3
  const char *button1;
  const char *button2;
  const char *button3;

  const char *header;

  uint8_t selected;

public:
  Dialog(const char *header, const char *button1, const char *button2);
  Dialog(const char *header, const char *button1, const char *button2, const char *button3);

  void render();
  void handleInput(ActionType t) {}
};
