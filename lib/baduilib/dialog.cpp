#include "dialog.h"

Dialog::Dialog(const char *header, const char *button1, const char *button2) :
    num_buttons(2),
    button1(button1),
    button2(button2),
    button3(""),
    header(header) {
  setID(header);
}

Dialog::Dialog(const char *header, const char *button1, const char *button2, const char *button3) :
    num_buttons(3),
    button1(button1),
    button2(button2),
    button3(button3),
    header(header) {
  setID(header);
}

void Dialog::render() {
  if (!u8g2)
    return;

  u8g2_SetFont(u8g2, u8g2_font_haxrcorp4089_tr);

  int dialog_width = u8g2_GetDisplayWidth(u8g2);
  int button_y = u8g2->width - 5; // 5px from bottom
  int gap = 10;

  int button1_width = u8g2_GetStrWidth(u8g2, button1);
  int button2_width = u8g2_GetStrWidth(u8g2, button2);
  int button3_width = num_buttons == 3 ? u8g2_GetStrWidth(u8g2, button3) : 0;

  int total_width = button1_width + button2_width + (num_buttons - 1) * gap;
  if (num_buttons == 3)
    total_width += button3_width;

  int start_x = (dialog_width - total_width) / 2;

  wordWrap(0, 0, header, true);

  // Draw Buttons
  u8g2_DrawStr(u8g2, start_x, button_y, button1);
  u8g2_DrawStr(u8g2, start_x + button1_width + gap, button_y, button2);
  if (num_buttons == 3)
    u8g2_DrawStr(u8g2, start_x + button1_width + gap + button2_width + gap, button_y, button3);
}
