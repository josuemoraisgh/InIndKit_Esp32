#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)

// On an ESP32: SDA (GPIO 21), SCL (GPIO 22) the pins for I2C are defined by the Wire-library.
Adafruit_SSD1306 display_oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class Display_t
{
private:
  char ca_line1[40] = "Inicializando...";
  char ca_line2[18] = ""; //
  char ca_line3[18] = ""; //
  int16_t i16_line1_width, i16_line1_minWidth;

public:
  bool start(void);
  void update(void);
  void setText(uint8_t line, const char txt[]);
};

bool Display_t::start(void)
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display_oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    return false;

  display_oled.setTextSize(2);
  display_oled.setTextColor(WHITE);
  display_oled.setTextWrap(false);
  i16_line1_width = display_oled.width();
  i16_line1_minWidth = -12 * strlen(ca_line1); // 12 = 6 pixels/character * text size 2
  return true;
}

void Display_t::update(void)
{
  //+--- Scroll IP ---+
  display_oled.clearDisplay();
  display_oled.setTextWrap(false);
  display_oled.setTextSize(2);              // Normal 1:1 pixel scale
  display_oled.setTextColor(SSD1306_WHITE); // Draw white text
  display_oled.setCursor(i16_line1_width, 0);
  display_oled.cp437(true); // Use full 256 char 'Code Page 437' font
  display_oled.print(ca_line1);
  if (--i16_line1_width < i16_line1_minWidth)
  {
    i16_line1_width = display_oled.width();
  }
  //+--- Print OLED Line2 ---+
  display_oled.setCursor(0, 20);
  display_oled.println(ca_line2);

  //+--- Print OLED Line3 ---+
  display_oled.setCursor(0, 40);
  display_oled.println(ca_line3);

  //+--- Atualiza o Display ---+
  display_oled.display();
}

void Display_t::setText(uint8_t line, const char txt[])
{
  switch (line)
  {
  case 1:
    strcpy(ca_line1, txt);
    i16_line1_width = display_oled.width();
    i16_line1_minWidth = -12 * strlen(ca_line1); // 12 = 6 pixels/character * text size 2
    break;
  case 2:
    strcpy(ca_line2, txt);
    break;
  default:
    strcpy(ca_line3, txt);
    break;
  }
}