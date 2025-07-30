#pragma once
#include "baduilib.h"
#include <cstdio>

class StackViewer : public Scene {
public:
  StackViewer() { setID("Stack Viewer"); }

  void render() {
    u8g2_SetFont(u8g2, u8g2_font_haxrcorp4089_tr);
    const uint16_t lineHeight = u8g2_GetMaxCharHeight(u8g2); // Height of one line
    uint16_t y = 0;                                          // Y position for the first line

    for (int8_t i = scene_manager->scenes_top; i >= 0; i--) {
      char buf[sizeof("1234. ") + sizeof(char[64])];
      snprintf(buf, sizeof(buf), "%d. %s", i + 1, scene_manager->scenes[i].scene->ID);

      u8g2_DrawStr(u8g2, 0, y, buf);

      y += lineHeight; // Move to the next line
    }
  }

  void handleInput(ActionType t) {
    if (t == ActionType::SEL)
      this->scene_manager->pop();
  }
};
