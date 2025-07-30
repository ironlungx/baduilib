#include <baduilib.h>
#include <settings.h>
#include <stack_viewer.h>
#include <u8g2.h>

#define LEN(x) (sizeof(x) / sizeof((x)[0]))

u8g2_t       u8g2;
SceneManager manager(&u8g2);

bool quit = false;

struct {
  char ssid[128];
  bool ble;
  int  port;
  bool webserver;
  bool darkMode;

  uint8_t wifi_mode   = 0;
  uint8_t debug_level = 0;
} config;

Setting wifiMenu[] = {
    Setting::Back(),
    Setting::Info("WiFi1"),
    Setting::Info("WiFi2"),
    Setting::Info("WiFi3"),
};

MenuConfig c;

Menu wifi("WiFi Config", wifiMenu, LEN(wifiMenu), &c);

Setting set[] = {
    Setting::String("SSID", config.ssid, LEN(config.ssid)),
    Setting::Bool("Enable Bluetooth", &config.ble),
    Setting::Int("Port", &config.port, 0, 65535),
    Setting::Separator(),
    Setting::Info("Webserver Config", true, true),
    Setting::Submenu("WiFi Config", &wifi),
    Setting::Int("Port again", &config.port, 0, 65535),
    Setting::Enum("WiFi Mode", &config.wifi_mode, 3, "AP", "STA", "AP+STA"),
    Setting::Enum("Log Level", &config.debug_level, 4, "DEBUG", "INFO", "WARN", "ERROR"),
    Setting::Button(
        "Quit", [](void *p) { quit = true; }, nullptr),
};

Menu mainMenu("Main Menu", set, LEN(set), &c);

int main(void) {
  u8g2_SetupBuffer_SDL_256x128(&u8g2, &u8g2_cb_r0);

  u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
  u8g2_SetFontPosTop(&u8g2);
  u8g2_SetFontDirection(&u8g2, 0);

  manager.push(&mainMenu);

  while (!quit) {
    manager.render();

    int k = u8g_sdl_get_key();

    switch (k) {
      case 'q': {
        quit = true;
      } break;
      case 'k': {
        manager.handleInput(ActionType::UP);
      } break;
      case 'j': {
        manager.handleInput(ActionType::DOWN);
      } break;
      case 'l': {
        manager.handleInput(ActionType::RIGHT);
      } break;
      case 'h': {
        manager.handleInput(ActionType::LEFT);
      } break;

      case 's': {
        manager.handleInput(ActionType::SEL);
      } break;

      case 'v': {
        StackViewer v;
        manager.push(&v);
      } break;
    }
  }

  return 0;
}
