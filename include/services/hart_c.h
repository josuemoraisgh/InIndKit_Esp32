#include <Arduino.h>
#include <HardwareSerial.h>
#include <ESPTelnetBase.h>

// Enables or disables Hardware Flow Control using RTS and/or CTS pins (must use setAllPins() before)
#define UART_HW_FLOWCTRL_DISABLE 0x0   //disable hardware flow control
#define UART_HW_FLOWCTRL_RTS     0x1   //enable RX hardware flow control (rts)
#define UART_HW_FLOWCTRL_CTS     0x2   //enable TX hardware flow control (cts)
#define UART_HW_FLOWCTRL_CTS_RTS 0x3   //enable hardware flow control
/////////////////////////////////////////////////////////////////////////
// Used to set RS485 modes such as UART_MODE_RS485_HALF_DUPLEX for Auto RTS function on ESP32
#define UART_MODE_UART                   0x00    //mode: regular UART mode
#define UART_MODE_RS485_HALF_DUPLEX      0x01    //mode: half duplex RS485 UART mode control by RTS pin
#define UART_MODE_IRDA                   0x02    //mode: IRDA UART mode
#define UART_MODE_RS485_COLLISION_DETECT 0x03    //mode: RS485 collision detection UART mode (used for test purposes)
#define UART_MODE_RS485_APP_CTRL         0x04    //mode: application control RS485 UART mode (used for test purposes)
/////////////////////////////////////////////////////////////////
class Hart_c : public HardwareSerial, public ESPTelnetBase
{

public:
    using ESPTelnetBase::ESPTelnetBase;
    Hart_c(uint16_t port) : HardwareSerial(1)
    {
        server_port = port;
    }
    bool setup() { return (setup(server_port)); }
    bool setup(uint16_t port);
    void loop();
    void hartToTelnet();
    void telnetToHart();

protected:
    void handleInput() {}
};

bool Hart_c::setup(uint16_t port)
{
    if (isConnected())
        stop();
    server_port = port;
    ((HardwareSerial *)this)->begin(1200);
    ((HardwareSerial *)this)->setHwFlowCtrlMode(UART_HW_FLOWCTRL_CTS_RTS);
    ((HardwareSerial *)this)->setMode(UART_MODE_RS485_HALF_DUPLEX); // For Auto RTS function on ESP32
    return ((ESPTelnetBase *)this)->begin(server_port);
}

void Hart_c::loop()
{
    ((ESPTelnetBase *)this)->loop();
    telnetToHart();
    hartToTelnet();
}

void Hart_c::telnetToHart()
{
    if (client)
    {
        const size_t tam = client.available();
        if (tam > 0)
        {
            uint8_t data[tam];
            client.readBytes(data, tam);
            ((HardwareSerial *)this)->write(data, tam);
        }
    }
}

void Hart_c::hartToTelnet()
{
    if (client)
    {
        const size_t tam = ((HardwareSerial *)this)->available();
        if (tam > 0)
        {
            uint8_t data[tam];
            ((HardwareSerial *)this)->readBytes(data, tam);
            ((ESPTelnet *)this)->write(data, tam);
        }
    }
}