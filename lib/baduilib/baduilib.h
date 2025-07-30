#pragma once
#include <cstdint>
#include <string.h>
#include <u8g2.h>

enum class ActionType { LEFT, RIGHT, UP, DOWN, SEL, NONE };

// Put -1 into w, h to specify full size
struct Rect {
  int x, y, w, h;

  static Rect full() { return {-1, -1, -1, -1}; };
};

class SceneManager; // Forward decl.

class Scene {
protected:
  u8g2_t *u8g2 = nullptr;
  SceneManager *scene_manager;
  Rect render_region = Rect::full();

  void clearRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  void centeredText(uint16_t y, const char *str);
  void wordWrap(int x, int y, const char *text, bool maintain_x);

public:
  char ID[64];
  Scene() {}

  void setRenderRegion(Rect r) { render_region = r; };

  void setID(const char *i) { strcpy(ID, i); }
  void setDisplay(u8g2_t *u8g2) { this->u8g2 = u8g2; }
  void setManager(SceneManager *m) { scene_manager = m; }

  virtual void render() = 0;
  virtual void handleInput(ActionType t) = 0;
};

class SceneManager : public Scene {
  // NOTE:
  // stack for every scene we enter
  // Example:
  //   | Keyboard
  //   | SSID
  //   | Network
  //   | Settings
  //   | Main Menu
  //   |--------
  //   | Header
  static constexpr size_t MAX_DEPTH = 5;
  struct SceneStack {
    Scene *scene;
  };

  SceneStack scenes[MAX_DEPTH] = {};
  int8_t scenes_top;
  bool dirty;

  u8g2_t *u8g2;

public:
  SceneManager(u8g2_t *u8g2) :
      scenes_top(-1),
      dirty(true),
      u8g2(u8g2) {}

  void markDirty() { dirty = true; };

  bool push(Scene *scene);
  bool pop();
  bool isEmpty();
  bool isFull();

  void render() override;
  void handleInput(ActionType t) override;

  friend class StackViewer;
};
