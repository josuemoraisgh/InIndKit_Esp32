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
  bool isFuncMode = false;
  bool isStart = true;

  char ca_lineTxt[3][20] = {"Inicializando...", "", ""}; //
  uint8_t ui8_lineSize[3] = {16, 0, 0};
  uint8_t ui8_txtSize[3] = {2, 2, 2};
  int16_t i16_lineWidth[3] = {36, 36, 36};
  int16_t i16_lineMinWidth[3];
  bool scrollLeft[3] = {false, false, false};

public:
  // On an ESP32: SDA (GPIO 21), SCL (GPIO 22) the pins for I2C are defined by the Wire-library.
  Display_c(void) : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}
  bool displayStart(void);
  void displayUpdate(void);
  void setDisplayText(uint8_t line, const char txt[], bool funcMode = false, uint8_t txtSize = 2);
  void setFuncMode(bool funcMode);
  void rotaty(uint8_t index);
};

bool Display_c::displayStart(void)
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    return false;
  setDisplayText(1, ca_lineTxt[0]);
  setDisplayText(2, ca_lineTxt[1]);
  setDisplayText(3, ca_lineTxt[2]);  

  return true;
}

void Display_c::displayUpdate(void)
{
  //+--- Scroll IP ---+
  if (ui8_lineSize[0] > 10 || ui8_lineSize[1] > 10 || ui8_lineSize[2] > 10 || isStart )
  {
    isStart = false;
    clearDisplay();
    setTextWrap(false);
    setTextColor(SSD1306_WHITE); // Draw white text
    cp437(true);                 // Use full 256 char 'Code Page 437' font
    //+--- Print OLED Line2 ---+
    rotaty(0);
    //+--- Print OLED Line2 ---+
    rotaty(1);
    //+--- Print OLED Line3 ---+
    rotaty(2);
    //+--- Atualiza o Display ---+
    display();
  }
}

void Display_c::rotaty(uint8_t index)
{
  if (ui8_lineSize[index] > 10)
  {
    setTextSize(ui8_txtSize[index]); // Normal 1:1 pixel scale
    setCursor(i16_lineWidth[index], 0);
    print(ca_lineTxt[index]);
    if (scrollLeft[index])
      ++i16_lineWidth[index];
    else
      --i16_lineWidth[index];
    if (i16_lineWidth[index] < i16_lineMinWidth[index])
      scrollLeft[index] = true;
    if (i16_lineWidth[index] > 24)
      scrollLeft[index] = false;
  }
  else
  {
    setTextSize(ui8_txtSize[index]); // Normal 1:1 pixel scale
    setCursor(0, index * 20);
    println(ca_lineTxt[index]); 
  }
}

void Display_c::setDisplayText(uint8_t line, const char txt[], bool funcMode, uint8_t txtSize)
{
  if (this->isFuncMode == funcMode)
  {
    strcpy(ca_lineTxt[line - 1], txt);
    ui8_lineSize[line - 1] = strlen(ca_lineTxt[line - 1]);
    i16_lineMinWidth[line - 1] = -12 * (ui8_lineSize[line - 1] - 9); // 12 = 6 pixels/character * text size 2
    ui8_txtSize[line - 1] = txtSize;
  }
  displayUpdate();
}

void Display_c::setFuncMode(bool funcMode)
{
  this->isFuncMode = funcMode;
}