#include <Arduino.h>
#include <HardwareSerial.h>
#include <ESPTelnetBase.h>

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