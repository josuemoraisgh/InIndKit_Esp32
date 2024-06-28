#include <Arduino.h>
#include <HardwareSerial.h>
#include <ESPTelnetBase.h>
#include <hal/uart_types.h>
#include "driver/uart.h"

class Hart_c : public HardwareSerial, protected ESPTelnetBase
{

protected:
    // int8_t ctsPin = -1;
    int8_t rtsPin = -1;

public:
    using ESPTelnetBase::ESPTelnetBase;
    Hart_c(uint16_t port) : HardwareSerial(UART_NUM_2), ESPTelnetBase()
    {
        server_port = port;
    }
    bool setup(int8_t rxPin, int8_t txPin, int8_t rtsPin) { return (setup(server_port, rxPin, txPin, rtsPin)); }
    bool setup(uint16_t port, int8_t rxPin, int8_t txPin, int8_t rtsPin);
    void loop();
    void hartToTelnet();
    void telnetToHart();

protected:
    void handleInput() {}
};

bool Hart_c::setup(uint16_t port, int8_t rxPin, int8_t txPin, int8_t rtsPin)
{
    if (((ESPTelnetBase *)this)->begin(server_port))
    {
        // this->ctsPin = ctsPin;
        this->rtsPin = rtsPin;
        server_port = port;
        pinMode(this->rtsPin, OUTPUT);
        // pinMode(this->ctsPin, INPUT);
        digitalWrite(this->rtsPin, HIGH);
        ((HardwareSerial *)this)->begin(1200, SERIAL_8O1, 3, 1, true);
        //((HardwareSerial *)this)->setPins(3, 1, -1, 22);
        //((HardwareSerial *)this)->setHwFlowCtrlMode(UART_HW_FLOWCTRL_RTS, UART_FIFO_LEN - 8);
        //((HardwareSerial *)this)->setMode(UART_MODE_UART);
        ((HardwareSerial *)this)->onReceive([this](){ hartToTelnet(); });
        return true;
    }
    return false;
}

void Hart_c::loop()
{
    telnetToHart();
    ((ESPTelnetBase *)this)->loop();
}

void Hart_c::telnetToHart()
{
    if (((ESPTelnetBase *)this)->isConnected())
    {
        const size_t tam = client.available();
        if (tam > 0)
        {
            uint8_t data[tam];
            client.readBytes(data, tam);
            digitalWrite(this->rtsPin, LOW);
            ((HardwareSerial *)this)->write(data, tam);
            digitalWrite(this->rtsPin, HIGH);
        }
    }
}

void Hart_c::hartToTelnet()
{
    const size_t tam = ((HardwareSerial *)this)->available();
    if (tam > 0)
    {
        uint8_t data[tam];
        ((HardwareSerial *)this)->readBytes(data, tam);
        client.write(data, tam);
    }
}