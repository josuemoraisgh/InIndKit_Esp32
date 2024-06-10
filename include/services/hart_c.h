#include <Arduino.h>
#include "ESPTelnetBase.h"

/**************** Hart Interface - UART1 **********/
#define def_pin_Hart_RXD 9  // Pino RX da ESP32 conectado ao pino RX do DS8500
#define def_pin_Hart_TXD 10 // Pino TX da ESP32 conectado ao pino TX do DS8500
#define def_pin_Hart_RTS 11 // Pino RTS da ESP32 conectado ao pino RTS do DS8500
#define def_pin_Hart_CTS 6  // Pino CD da ESP32 conectado ao pino CD do DS8500

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

protected:
    void handleInput();
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
    size_t tam = ((HardwareSerial *)this)->available();
    if (tam > 0)
    {
        uint8_t data[tam];
        ((HardwareSerial *)this)->readBytes(data, tam);
        ((ESPTelnet *)this)->write(data, tam);
    }
    ((ESPTelnetBase *)this)->loop();
}

void Hart_c::handleInput()
{
    size_t tam = client.available();
    if (tam > 0)
    {
        uint8_t data[tam];
        client.readBytes(data, tam);
        ((HardwareSerial *)this)->write(data, tam);
    }
}