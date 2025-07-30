#include "settings.h"
#include <cstdio>

void Menu::render() {
  if (n_root == 0) {
    return;
  }

  if (viewport_size == 0) {
    updateViewportSize(); // Try to update it
    if (viewport_size == 0) {
      return;
    }
  }

  const uint16_t height = u8g2->height;
  const uint16_t width = u8g2->width;

  const uint8_t padding_x = 5;
  const uint8_t padding_y = 2;

  u8g2_SetFont(u8g2, u8g2_font_helvB08_tr);

  const uint16_t title_height = u8g2_GetMaxCharHeight(u8g2);
  const uint16_t header_width = u8g2_GetUTF8Width(u8g2, header);

  // Scrollbar configuration
  const uint8_t scrollbar_width = 3;
  const uint16_t scrollbar_x = width - scrollbar_width;
  const uint16_t scrollbar_top = padding_y;
  const uint16_t scrollbar_bottom = height - title_height - 5;
  const uint8_t scrollbar_height = scrollbar_bottom - scrollbar_top;

  // Calculate visible items in viewport
  size_t visible_items = getViewportEnd() - viewport_start;

  // Only show scrollbar if there are more items than can fit in viewport
  bool show_scrollbar = n_root > visible_items;

  for (size_t i = viewport_start; i < getViewportEnd(); i++) {
    u8g2_SetFont(u8g2, u8g2_font_haxrcorp4089_tr);

    const uint16_t item_font_height = u8g2_GetMaxCharHeight(u8g2);
    uint16_t item_right_edge = show_scrollbar ? scrollbar_x - 2 : width - padding_x;
    size_t display_index = i - viewport_start;
    uint16_t title_y = (padding_y * (display_index + 1)) + (item_font_height * display_index);

    const uint16_t item_center_y = title_y + (item_font_height / 2); // center point of the line

    switch (root[i].getType()) {
      case SettingType::SEPARATOR: {
        u8g2_DrawLine(u8g2, padding_x, title_y + item_font_height / 2, item_right_edge,
                       title_y + item_font_height / 2);
        continue;
      } break;
      case SettingType::INFO: {
        // We have to center the text and/or render it in a bigger font
        const InfoSetting s = root[i].getInfo();
        if (s.center) {
          const char *title = root[i].getTitle();
          uint16_t x = ((width - padding_x - scrollbar_width) - u8g2_GetUTF8Width(u8g2, title)) / 2;
          u8g2_DrawStr(u8g2, x, title_y, title);
          continue;
        }
      } break;
      case SettingType::STRING:
      case SettingType::INT:
      case SettingType::BOOL:
      case SettingType::BUTTON:
      case SettingType::ENUM:
      case SettingType::SUBMENU:
      case SettingType::BACK:
      case SettingType::EMPTY:
        break;
      }

    if (highlight_idx == i) {
      // Adjust width to avoid scrollbar if present
      uint16_t box_width = u8g2_GetUTF8Width(u8g2, root[i].getTitle()) + 2;
      if (show_scrollbar && (padding_x + box_width + scrollbar_width + 2) > width) {
        box_width = width - padding_x - scrollbar_width - 4;
      }

      u8g2_DrawRBox(u8g2, padding_x - 1, title_y - 1, box_width, item_font_height + 2, 1);

      u8g2_SetDrawColor(u8g2, 0); // black
    }

    u8g2_DrawStr(u8g2, padding_x, title_y, root[i].getTitle());

    if (highlight_idx == i) {
      u8g2_SetDrawColor(u8g2, 1);
    }

    // Adjust item rendering positions to account for scrollbar

    switch (root[i].getType()) {
      case SettingType::BOOL: {
        constexpr size_t frame_len = 8; // 8x8 outer frame
        const BoolSetting &b = root[i].getBool();

        const uint16_t frame_x = item_right_edge - frame_len;
        const uint16_t frame_y = item_center_y - (frame_len / 2);

        u8g2_DrawFrame(u8g2, frame_x, frame_y, frame_len, frame_len);

        if (*b.state) {
          constexpr size_t box_len = 4; // 4x4 inner box

          const uint16_t box_x = frame_x + (frame_len - box_len) / 2;
          const uint16_t box_y = frame_y + (frame_len - box_len) / 2;

          u8g2_DrawBox(u8g2, box_x, box_y, box_len, box_len);
        }
      } break;

      case SettingType::SUBMENU: {
        constexpr size_t len = 5;
        uint16_t x0, y0;
        uint16_t x1, y1;
        uint16_t x2, y2;

        x0 = item_right_edge - len;
        y0 = title_y;

        x1 = x0;
        y1 = title_y + item_font_height;

        x2 = item_right_edge;
        y2 = title_y + item_font_height / 2;

        u8g2_DrawTriangle(u8g2, x0, y0, x1, y1, x2, y2);
      } break;

      case SettingType::INT: {
        const IntSetting &val = root[i].getInt();
        u8g2_SetFont(u8g2, u8g2_font_4x6_tn);

        char buf[16];
        snprintf(buf, 16, "%d", *val.value);

        constexpr size_t border_padding = 2;

        const uint16_t buf_width = u8g2_GetUTF8Width(u8g2, buf);
        const uint16_t value_font_height = u8g2_GetMaxCharHeight(u8g2);

        const uint16_t frame_width = buf_width + (2 * border_padding);
        const uint16_t frame_height = value_font_height + (2 * border_padding);

        const uint16_t frame_x = item_right_edge - frame_width;
        const uint16_t frame_y = item_center_y - (frame_height / 2);

        const uint16_t text_x = frame_x + border_padding;
        const uint16_t text_y = frame_y + border_padding;

        u8g2_DrawStr(u8g2, text_x, text_y, buf);
        u8g2_DrawFrame(u8g2, frame_x, frame_y, frame_width, frame_height);

      } break;

      case SettingType::STRING: {
        const StringSetting &val = root[i].getString();
        const char *buf = val.str;

        u8g2_SetFont(u8g2, u8g2_font_4x6_tr);

        if (buf[0] == '\0') { // empty string
          if (val.placeholder != nullptr) {
            buf = val.placeholder;
          } else {
            static char empty_placeholder[] = "--";
            buf = empty_placeholder;
          }
        }

        constexpr size_t border_padding = 2;

        const uint8_t buf_width = u8g2_GetUTF8Width(u8g2, buf);
        const uint8_t value_font_height = u8g2_GetMaxCharHeight(u8g2);

        const uint16_t frame_width = buf_width + (2 * border_padding);
        const uint16_t frame_height = value_font_height + (2 * border_padding);

        const uint16_t frame_x = item_right_edge - frame_width;
        const uint16_t frame_y = item_center_y - (frame_height / 2);

        const uint16_t text_x = frame_x + border_padding;
        const uint16_t text_y = frame_y + border_padding;

        u8g2_DrawStr(u8g2, text_x, text_y, buf);
        u8g2_DrawFrame(u8g2, frame_x, frame_y, frame_width, frame_height);
      } break;

      case SettingType::ENUM: {
        char buf[64];
        const EnumSetting &val = root[i].getEnum();

        if (*(val.selected) >= val.n_items) {
          return;
        }

        snprintf(buf, sizeof(buf), "%c %s %c", *(val.selected) == 0 ? ' ' : '<',
                 val.items[*(val.selected)], *(val.selected) == val.n_items - 1 ? ' ' : '>');

        u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
        const uint16_t buf_width = u8g2_GetUTF8Width(u8g2, buf);
        const uint16_t text_x = item_right_edge - buf_width;

        const uint8_t font_height = u8g2_GetMaxCharHeight(u8g2);
        const uint16_t text_y = item_center_y - (font_height / 2);

        u8g2_DrawStr(u8g2, text_x, text_y, buf);
      } break;

      default:
        break;
    }
  }

  // Draw scrollbar if needed
  if (show_scrollbar) {
    // Draw dotted line track down the center of the scrollbar area
    uint16_t center_x = scrollbar_x + (scrollbar_width / 2);

    // Draw dotted line - alternate pixels every 2 pixels
    for (uint16_t y = scrollbar_top; y < scrollbar_bottom; y += 3) {
      u8g2_DrawPixel(u8g2, center_x, y);
    }

    // Calculate scrollbar thumb position and size
    float scroll_ratio = (float)viewport_start / (float)(n_root - visible_items);
    float thumb_size_ratio = (float)visible_items / (float)n_root;

    uint16_t thumb_height = (uint8_t)(scrollbar_height * thumb_size_ratio);
    if (thumb_height < 4)
      thumb_height = 4; // Minimum thumb height for visibility

    uint16_t thumb_y = scrollbar_top + (uint8_t)((scrollbar_height - thumb_height) * scroll_ratio);

    // Draw scrollbar thumb as a rounded rectangle
    u8g2_DrawRBox(u8g2, scrollbar_x, thumb_y, scrollbar_width, thumb_height, 1);
  }

  clearRect(0, height - title_height - 3, width, height - title_height - 3);

  u8g2_SetFont(u8g2, u8g2_font_helvB08_tr);
  u8g2_DrawLine(u8g2, 0, height - title_height - 2, width, height - title_height - 2);
  u8g2_DrawStr(u8g2, (width - header_width) / 2, height - title_height, header);
}
