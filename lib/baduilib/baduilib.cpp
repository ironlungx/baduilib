#include "baduilib.h"
#include "u8g2.h"

void Scene::clearRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  u8g2_SetDrawColor(u8g2, 0); // black
  u8g2_DrawBox(u8g2, x, y, w, h);
  u8g2_SetDrawColor(u8g2, 1); // foreground
}

// display a string on multiple text lines, keeping words intact where possible,
// and accepting \n to force a new line
void Scene::wordWrap(int x, int y, const char *text, bool maintain_x) const {
  unsigned int dspwidth = u8g2_GetDisplayWidth(u8g2); // display width in pixels
  unsigned int strwidth = 0;                          // string width in pixels
  unsigned int orignal_x = x;

  char glyph[2];
  glyph[1] = 0;
  for (const char *ptr = text, *lastblank = NULL; *ptr; ++ptr) {
    while (x == 0 && (*text == ' ' || *text == '\n'))
      if (ptr == text++)
        ++ptr; // skip blanks and newlines at the left edge

    glyph[0] = *ptr;
    strwidth += u8g2_GetStrWidth(u8g2, glyph); // accumulate the pixel width

    if (*ptr == ' ')
      lastblank = ptr; // remember where the last blank was

    else
      ++strwidth; // non-blanks will be separated by one additional pixel

    if (*ptr == '\n' ||            // if we found a newline character,
        x + strwidth > dspwidth) { // or if we ran past the right edge of the display
      int starting_x = x;

      // print to just before the last blank, or to just before where we got to
      while (text < (lastblank ? lastblank : ptr)) {
        glyph[0] = *text++;
        x += u8g2_DrawStr(u8g2, x, y, glyph);
      }

      strwidth -= x - starting_x;       // account for what we printed
      y += u8g2_GetMaxCharHeight(u8g2); // advance to the next line
      x = maintain_x ? orignal_x : 0;
      lastblank = NULL;
    }
  }
  while (*text) { // print any characters left over
    glyph[0] = *text++;
    x += u8g2_DrawStr(u8g2, x, y, glyph);
  }
}
void Scene::centeredText(uint16_t y, const char *str) const {
  // x => (width - str_width) / 2
  uint16_t width = u8g2_GetUTF8Width(u8g2, str);
  u8g2_DrawStr(u8g2, (u8g2->width - width) / 2, y, str);
}

bool SceneManager::push(Scene *scene) {
  if (scenes_top == MAX_DEPTH - 1) {
    return false;
  }

  scene->setDisplay(u8g2);

  scene->setManager(this);
  scenes_top++;
  scenes[scenes_top].scene = scene;
  markDirty();
  return true;
}

bool SceneManager::pop() {
  if (isEmpty())
    return false;
  scenes_top--;
  markDirty();
  return true;
}

bool SceneManager::isFull() { return (scenes_top == MAX_DEPTH - 1); }

bool SceneManager::isEmpty() { return (scenes_top == -1); }

void SceneManager::render() {
  if (!dirty || isEmpty())
    return;

  u8g2_ClearBuffer(u8g2);
  scenes[scenes_top].scene->render();
  u8g2_SendBuffer(u8g2);

  dirty = false;
}

void SceneManager::handleInput(ActionType t) {
  if (!isEmpty())
    scenes[scenes_top].scene->handleInput(t);
}
