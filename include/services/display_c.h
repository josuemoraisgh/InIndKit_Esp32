#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)

class Display_c : public Adafruit_SSD1306
{
private:
  char ca_line1[20] = "Inicializando...";
  char ca_line2[20] = ""; //
  char ca_line3[20] = ""; //

  uint8_t ui8_txtSize_l1 = 2, ui8_txtSize_l2 = 2, ui8_txtSize_l3 = 2;
  int16_t i16_line1_width, i16_line1_minWidth;

  bool scrollLeft = false;

public:
  // On an ESP32: SDA (GPIO 21), SCL (GPIO 22) the pins for I2C are defined by the Wire-library.
  Display_c(void) : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}
  bool displayStart(void);
  void displayUpdate(void);
  void setDisplayText(uint8_t line, const char txt[], uint8_t txtSize = 2);
};

bool Display_c::displayStart(void)
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    return false;

  clearDisplay();
  setTextWrap(false);
  setTextColor(SSD1306_WHITE); // Draw white text

  setTextSize(ui8_txtSize_l1); // Normal 1:1 pixel scale
  setCursor(0, 0);
  println(ca_line1);

  setTextSize(ui8_txtSize_l2); // Normal 1:1 pixel scale
  setCursor(0, 20);
  println(ca_line2);

  setTextSize(ui8_txtSize_l3); // Normal 1:1 pixel scale
  setCursor(0, 40);
  println(ca_line3);

  i16_line1_width = 36;                        // 12*3 - display_oled.width();
  i16_line1_minWidth = -12 * strlen(ca_line1); // -12 = 6 pixels/character * text size 2
  return true;
}

void Display_c::displayUpdate(void)
{
  //+--- Scroll IP ---+
  clearDisplay();
  setTextWrap(false);
  setTextColor(SSD1306_WHITE); // Draw white text
  cp437(true);                 // Use full 256 char 'Code Page 437' font

  setTextSize(ui8_txtSize_l1); // Normal 1:1 pixel scale
  setCursor(i16_line1_width, 0);
  print(ca_line1);
  if(scrollLeft) ++i16_line1_width;
  else --i16_line1_width;
  if (i16_line1_width < i16_line1_minWidth) scrollLeft = true;
  if (i16_line1_width > 24)  scrollLeft = false;
  //+--- Print OLED Line2 ---+
  setTextSize(ui8_txtSize_l2); // Normal 1:1 pixel scale
  setCursor(0, 20);
  println(ca_line2);

  //+--- Print OLED Line3 ---+
  setTextSize(ui8_txtSize_l3); // Normal 1:1 pixel scale
  setCursor(0, 40);
  println(ca_line3);

  //+--- Atualiza o Display ---+
  display();
}

void Display_c::setDisplayText(uint8_t line, const char txt[], uint8_t txtSize)
{
  switch (line)
  {
  case 1:
    strcpy(ca_line1, txt);
    i16_line1_minWidth = -12 * (strlen(ca_line1) - 9); // 12 = 6 pixels/character * text size 2
    ui8_txtSize_l1 = txtSize;
    break;
  case 2:
    strcpy(ca_line2, txt);
    ui8_txtSize_l2 = txtSize;
    break;
  default:
    strcpy(ca_line3, txt);
    ui8_txtSize_l3 = txtSize;
    break;
  }
}