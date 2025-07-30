#include "settings.h"
#include <cmath>

Menu::Menu(const char *root_header, Setting *root, size_t n_root) :
    Scene(),
    root(root),
    n_root(n_root),
    header(root_header) {
  setID(root_header);
}

void Menu::setDisplay(u8g2_t *u8g2) {
  Scene::setDisplay(u8g2);
  updateViewportSize();
}

void Menu::updateViewportSize() {
  if (!u8g2) {
    viewport_size = 0; // Safe fallback
    return;
  }

  u8g2_SetFont(u8g2, u8g2_font_haxrcorp4089_tr);

  const uint8_t item_font_height = u8g2_GetMaxCharHeight(u8g2);
  const uint8_t item_total_height =
      item_font_height + (2 * 2); // padding_y = 2   TODO: make the UI constants configurable

  viewport_size = std::ceil(u8g2->height / item_total_height);
}

void Menu::handleInput(ActionType action) {
  switch (action) {
      /* case ActionType::UP: {
        if (highlight_idx > 0) {
          highlight_idx--;
          if (highlight_idx < viewport_start) {
            viewport_start = highlight_idx;
          }
          scene_manager->markDirty();
        }
      } break; */

    case ActionType::UP: {
      if (highlight_idx == 0) // We're already on the top item
        break;

      size_t i = highlight_idx - 1;

      while (true) {
        if (root[i].isFocusable()) {
          highlight_idx = i;

          if (highlight_idx < viewport_start) {
            viewport_start = highlight_idx;
          }

          scene_manager->markDirty();
          break;
        }

        if (i == 0)
          break;
        i--;
      }
    } break;

      /* case ActionType::DOWN: {
        if (highlight_idx + 1 < n_root) {
          highlight_idx++;
          if (highlight_idx >= getViewportEnd())
            viewport_start++;
        }
        scene_manager->markDirty();
      } break; */

    case ActionType::DOWN: {
      size_t i = highlight_idx + 1;

      while (i < n_root) {
        if (root[i].isFocusable()) {
          highlight_idx = i;
          if (highlight_idx >= viewport_start + viewport_size) {
            viewport_start = highlight_idx - (viewport_size - 1);
          }

          scene_manager->markDirty();
          break;
        }
        i++;
      }
    } break;

    case ActionType::LEFT: {
      Setting &selected = root[highlight_idx];

      if (selected.getType() == SettingType::ENUM) {
        EnumSetting &s = selected.getEnum();

        if (*(s.selected) > 0) { // Check if not at minimum
          (*(s.selected))--;     // Decrement for LEFT
          scene_manager->markDirty();
        }
      }
    } break;

    case ActionType::RIGHT: {
      Setting &selected = root[highlight_idx];

      if (selected.getType() == SettingType::ENUM) {
        EnumSetting &s = selected.getEnum();

        if (*(s.selected) < s.n_items - 1) {
          (*(s.selected))++;
          scene_manager->markDirty();
        }
      }
    } break;

    case ActionType::SEL: {
      Setting &selected = root[highlight_idx];

      selected.onClick(); // Execute item's callback

      if (selected.getType() == SettingType::SUBMENU) {
        Scene *s = selected.getSubmenu().submenu;

        scene_manager->push(s);
      } else if (selected.getType() == SettingType::BACK) {
        scene_manager->pop();
      }
      scene_manager->markDirty();
    } break;
    case ActionType::NONE:
      break;
  };
}
