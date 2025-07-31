#include "settings.h"
#include <cstdio>

void Menu::render() {
  // Implement render_region support for proper positioning
  if (n_root == 0) {
    return;
  }

  if (viewport_size == 0) {
    updateViewportSize(); // Try to update it
    if (viewport_size == 0) {
      return;
    }
  }

  const uint16_t &height   = render_region.h;
  const uint16_t &width    = render_region.w;
  const uint16_t &region_x = render_region.x;
  const uint16_t &region_y = render_region.y;

  // Use config values and apply render region offset
  const uint8_t padding_x = config->margin.left + region_x;
  const uint8_t padding_y = config->margin.top + region_y;

  u8g2_SetFont(u8g2, config->title_font);

  const uint16_t title_height = u8g2_GetMaxCharHeight(u8g2);
  const uint16_t title_width  = u8g2_GetUTF8Width(u8g2, title);

  // Scrollbar configuration using config with render region offset
  const uint8_t  scrollbar_width  = config->scrollbar.width;
  const uint16_t scrollbar_x      = region_x + width - scrollbar_width;
  const uint16_t scrollbar_top    = padding_y;
  const uint16_t scrollbar_bottom = region_y + height - title_height - 5;
  const uint8_t  scrollbar_height = scrollbar_bottom - scrollbar_top;

  // Calculate the content area boundaries for clipping
  const uint16_t content_top    = padding_y;
  const uint16_t content_bottom = region_y + height - title_height - 5;

  // Calculate visible items in viewport
  size_t visible_items = getViewportEnd() - viewport_start;

  // Only show scrollbar if there are more items than can fit in viewport
  bool show_scrollbar = n_root > visible_items;

  // Render items until we hit the display bottom
  for (size_t i = viewport_start; i < n_root; i++) {
    u8g2_SetFont(u8g2, config->item_font);

    const uint16_t item_font_height = u8g2_GetMaxCharHeight(u8g2);
    uint16_t       item_right_edge =
        show_scrollbar ? scrollbar_x - 2 : region_x + width - config->margin.right;
    size_t   display_index = i - viewport_start;
    uint16_t title_y       = padding_y + (config->item_padding * (display_index + 1)) +
                       (item_font_height * display_index);

    // Stop rendering if item would be completely below display
    if (title_y > u8g2->height) {
      break;
    }

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
          const char *item_text = root[i].getTitle();

          // Calculate the left and right boundaries for centering
          uint16_t left_boundary  = region_x + config->margin.left;
          uint16_t right_boundary = show_scrollbar
                                        ? (scrollbar_x - 2)
                                        : // scrollbar_x is already absolute, just leave gap
                                        (region_x + width - config->margin.right);

          uint16_t available_width = right_boundary - left_boundary;
          uint16_t text_width      = u8g2_GetUTF8Width(u8g2, item_text);

          // Center the text within the available space
          uint16_t x = left_boundary + (available_width - text_width) / 2;
          u8g2_DrawStr(u8g2, x, title_y, item_text);
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
      if (show_scrollbar && (padding_x + box_width + scrollbar_width + 2) > (region_x + width)) {
        box_width = (region_x + width) - padding_x - scrollbar_width - 4;
      }

      if (config->scrollbar.round) {
        u8g2_DrawRBox(u8g2, padding_x - 1, title_y - 1, box_width, item_font_height + 2, 1);
      } else {
        u8g2_DrawBox(u8g2, padding_x - 1, title_y - 1, box_width, item_font_height + 2);
      }

      u8g2_SetDrawColor(u8g2, 0); // black
    }

    u8g2_DrawStr(u8g2, padding_x, title_y, root[i].getTitle());

    if (highlight_idx == i) {
      u8g2_SetDrawColor(u8g2, 1);
    }

    // Adjust item rendering positions to account for scrollbar
    switch (root[i].getType()) {
      case SettingType::BOOL: {
        constexpr size_t   frame_len = 8; // 8x8 outer frame
        const BoolSetting &b         = root[i].getBool();

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
        uint16_t         x0, y0;
        uint16_t         x1, y1;
        uint16_t         x2, y2;

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
        u8g2_SetFont(u8g2, config->value_font);

        char buf[16];
        snprintf(buf, 16, "%d", *val.value);

        constexpr size_t border_padding = 2;

        const uint16_t buf_width         = u8g2_GetUTF8Width(u8g2, buf);
        const uint16_t value_font_height = u8g2_GetMaxCharHeight(u8g2);

        const uint16_t frame_width  = buf_width + (2 * border_padding);
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
        const char          *buf = val.str;

        u8g2_SetFont(u8g2, config->value_font);

        if (buf[0] == '\0') { // empty string
          if (val.placeholder != nullptr) {
            buf = val.placeholder;
          } else {
            static char empty_placeholder[] = "--";
            buf                             = empty_placeholder;
          }
        }

        constexpr size_t border_padding = 2;

        const uint8_t buf_width         = u8g2_GetUTF8Width(u8g2, buf);
        const uint8_t value_font_height = u8g2_GetMaxCharHeight(u8g2);

        const uint16_t frame_width  = buf_width + (2 * border_padding);
        const uint16_t frame_height = value_font_height + (2 * border_padding);

        const uint16_t frame_x = item_right_edge - frame_width;
        const uint16_t frame_y = item_center_y - (frame_height / 2);

        const uint16_t text_x = frame_x + border_padding;
        const uint16_t text_y = frame_y + border_padding;

        u8g2_DrawStr(u8g2, text_x, text_y, buf);
        u8g2_DrawFrame(u8g2, frame_x, frame_y, frame_width, frame_height);
      } break;

      case SettingType::ENUM: {
        char               buf[64];
        const EnumSetting &val = root[i].getEnum();

        if (*(val.selected) >= val.n_items) {
          return;
        }

        snprintf(buf, sizeof(buf), "%c %s %c", *(val.selected) == 0 ? ' ' : '<',
                 val.items[*(val.selected)], *(val.selected) == val.n_items - 1 ? ' ' : '>');

        u8g2_SetFont(u8g2, config->value_font);
        const uint16_t buf_width = u8g2_GetUTF8Width(u8g2, buf);
        const uint16_t text_x    = item_right_edge - buf_width;

        const uint8_t  font_height = u8g2_GetMaxCharHeight(u8g2);
        const uint16_t text_y      = item_center_y - (font_height / 2);

        u8g2_DrawStr(u8g2, text_x, text_y, buf);
      } break;

      default:
        break;
    }
  }

  // Clear/clip areas where content extends beyond viewport bounds
  // Clear above content area
  if (content_top > region_y) {
    clearRect(region_x, region_y, width, content_top - region_y);
  }

  // Clear below content area (above title section)
  if (content_bottom < (region_y + height - title_height - 3)) {
    clearRect(region_x, content_bottom, width,
              (region_y + height - title_height - 3) - content_bottom);
  }

  // Draw scrollbar if needed
  if (show_scrollbar) {
    uint16_t center_x = scrollbar_x + (scrollbar_width / 2);

    // Draw background track based on config
    if (config->scrollbar.dotted) {
      // Draw dotted line - alternate pixels every 2 pixels
      for (uint16_t y = scrollbar_top; y < scrollbar_bottom; y += 3) {
        u8g2_DrawPixel(u8g2, center_x, y);
      }
    } else {
      // Draw solid line track
      u8g2_DrawVLine(u8g2, center_x, scrollbar_top, scrollbar_height);
    }

    // Calculate scrollbar thumb position and size
    float scroll_ratio     = (float)viewport_start / (float)(n_root - visible_items);
    float thumb_size_ratio = (float)visible_items / (float)n_root;

    uint16_t thumb_height = (uint8_t)(scrollbar_height * thumb_size_ratio);
    if (thumb_height < 4)
      thumb_height = 4; // Minimum thumb height for visibility

    uint16_t thumb_y = scrollbar_top + (uint8_t)((scrollbar_height - thumb_height) * scroll_ratio);

    // Draw scrollbar thumb based on config
    if (config->scrollbar.round) {
      u8g2_DrawRBox(u8g2, scrollbar_x, thumb_y, scrollbar_width, thumb_height, 1);
    } else {
      u8g2_DrawBox(u8g2, scrollbar_x, thumb_y, scrollbar_width, thumb_height);
    }
  }

  clearRect(region_x, region_y + height - title_height - 3, width, title_height + 3);

  u8g2_SetFont(u8g2, config->title_font);
  u8g2_DrawLine(u8g2, region_x, region_y + height - title_height - 2, region_x + width,
                region_y + height - title_height - 2);
  u8g2_DrawStr(u8g2, region_x + (width - title_width) / 2, region_y + height - title_height, title);
}
