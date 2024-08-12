#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class Display_c 
{
protected:
  void rotaty(uint8_t index);
  bool start(const uint8_t &SDA = 0, const uint8_t &SCL = 0); 
  void update(void);   
  bool isFuncMode = false;
  bool isChanged = true;
  bool scrollLeft[3] = {false, false, false};
  char ca_lineTxt[3][20] = {"Inicializando...", "", ""}; //
  uint8_t ui8_lineSize[3] = {16, 0, 0};
  uint8_t ui8_txtSize[3] = {2, 2, 2};
  int16_t i16_lineWidth[3] = {12, 12, 12};
  int16_t i16_lineMinWidth[3];    

public:
  // On an ESP32: SDA (GPIO 21), SCL (GPIO 22) the pins for I2C are defined by the Wire-library.
  void setText(uint8_t line, const char txt[], bool funcMode = false, uint8_t txtSize = 2);
  void setFuncMode(bool funcMode);

  friend inline bool startDisplay(Display_c *disp,const uint8_t &SDA = 0, const uint8_t &SCL = 0);
  friend inline void updateDisplay(Display_c *disp);  
};

inline bool startDisplay(Display_c *disp,const uint8_t &SDA, const uint8_t &SCL) {return disp->start(SDA,SCL);}
bool Display_c::start(const uint8_t &SDA, const uint8_t &SCL)
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(SDA != 0 && SCL != 0) Wire.setPins(SDA, SCL); // Set the I2C pins before begin
  if (!SSD1306.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    return false;
  setText(1, ca_lineTxt[0]);
  setText(2, ca_lineTxt[1]);
  setText(3, ca_lineTxt[2]);

  return true;
}

inline void updateDisplay(Display_c *disp) {disp->update();}
void Display_c::update(void)
{
  //+--- Scroll IP ---+
  if (ui8_lineSize[0] > 10 || ui8_lineSize[1] > 10 || ui8_lineSize[2] > 10 || isChanged)
  {
    isChanged = false;
    SSD1306.clearDisplay();
    SSD1306.setTextWrap(false);
    SSD1306.setTextColor(SSD1306_WHITE); // Draw white text
    SSD1306.cp437(true);                 // Use full 256 char 'Code Page 437' font
    //+--- Print OLED Line2 ---+
    rotaty(0);
    //+--- Print OLED Line2 ---+
    rotaty(1);
    //+--- Print OLED Line3 ---+
    rotaty(2);
    //+--- Atualiza o Display ---+
    SSD1306.display();
  }
}

void Display_c::rotaty(uint8_t index)
{
  if (ui8_lineSize[index] > 10)
  {
    SSD1306.setTextSize(ui8_txtSize[index]); // Normal 1:1 pixel scale
    SSD1306.setCursor(i16_lineWidth[index], index * 20);
    SSD1306.print(ca_lineTxt[index]);
    if (scrollLeft[index])
      ++i16_lineWidth[index];
    else
      --i16_lineWidth[index];
    if (i16_lineWidth[index] < i16_lineMinWidth[index])
      scrollLeft[index] = true;
    if (i16_lineWidth[index] > 12)
      scrollLeft[index] = false;
  }
  else
  {
    SSD1306.setTextSize(ui8_txtSize[index]); // Normal 1:1 pixel scale
    SSD1306.setCursor(0, index * 20);
    SSD1306.println(ca_lineTxt[index]);
  }
}

void Display_c::setText(uint8_t line, const char txt[], bool funcMode, uint8_t txtSize)
{
  if (this->isFuncMode == funcMode)
  {
    strcpy(ca_lineTxt[line - 1], txt);
    ui8_lineSize[line - 1] = strlen(ca_lineTxt[line - 1]);
    i16_lineMinWidth[line - 1] = -12 * (ui8_lineSize[line - 1] - 9); // (6 pixels) ~= (1 character)
    ui8_txtSize[line - 1] = txtSize;
    isChanged = true;
  }
  update();
}

void Display_c::setFuncMode(bool funcMode)
{
  this->isFuncMode = funcMode;
}