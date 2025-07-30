#include "settings.h"
#include "u8g2.h"

struct MenuConfig {
  struct {
    uint16_t left, right, bottom, top;
  } margin;

  struct {
    uint16_t width; // Width of the slider
    bool round;     // Makes the slider round yaaya
    bool dotted;    // Have a dotted line instead of a solid line in the bg.
  } scrollbar_config;

  const uint8_t *header_font;
  const uint8_t *item_font;
};

void Menu::render() {
  if (n_root == 0)
    return; // No items, we can just skip rendering

  if (viewport_size == 0) {
    updateViewportSize(); // Try updating

    if (viewport_size == 0)
      return;
  }

  const uint16_t &height = u8g2->height;
  const uint16_t &width = u8g2_GetDisplayWidth(u8g2);

  size_t visible_items = getViewportEnd() - viewport_start;
  const bool show_scroll_bar = n_root > visible_items;

  // Draw the title of the menu attached to the bottom
  centeredText(height - u8g2_GetMaxCharHeight(u8g2), header);
}
