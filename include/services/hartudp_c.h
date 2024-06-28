#include <Arduino.h>
#include <HardwareSerial.h>
#include <hal/uart_types.h>
#include "driver/uart.h"
#include "AsyncUDP.h"

class HartUdp_c : public HardwareSerial, protected AsyncUDP
{

protected:
    // int8_t ctsPin = -1;
    int8_t rtsPin = -1;
    uint16_t server_port = 4000;
    AsyncUDPPacket *packet = NULL;

public:
    HartUdp_c(uint16_t port) : HardwareSerial(UART_NUM_2), AsyncUDP()
    {
        server_port = port;
    }
    bool setup(int8_t rxPin, int8_t txPin, int8_t rtsPin) { return (setup(server_port, rxPin, txPin, rtsPin)); }
    bool setup(uint16_t port, int8_t rxPin, int8_t txPin, int8_t rtsPin);
    void hartToUdp();
    void udpToHart(AsyncUDPPacket packet);

protected:
    void handleInput() {}
};

bool HartUdp_c::setup(uint16_t port, int8_t rxPin, int8_t txPin, int8_t rtsPin)
{
    if (((AsyncUDP *)this)->listen(port))
    {
        // this->ctsPin = ctsPin;
        this->rtsPin = rtsPin;
        server_port = port;
        pinMode(this->rtsPin, OUTPUT);
        // pinMode(this->ctsPin, INPUT);
        digitalWrite(this->rtsPin, HIGH);
        ((HardwareSerial *)this)->begin(1200, SERIAL_8O1, rxPin, txPin, true);
        //((HardwareSerial *)this)->setPins(3, 1, -1, 22);
        //((HardwareSerial *)this)->setHwFlowCtrlMode(UART_HW_FLOWCTRL_RTS, UART_FIFO_LEN - 8);
        //((HardwareSerial *)this)->setMode(UART_MODE_UART);
        ((AsyncUDP *)this)->onPacket([this](AsyncUDPPacket packet){udpToHart(packet);});
        ((HardwareSerial *)this)->onReceive([this](){hartToUdp();});
        return true;
    }
    return false;
}

void HartUdp_c::udpToHart(AsyncUDPPacket packet)
{
    this->packet = &packet;
    digitalWrite(this->rtsPin, LOW);
    ((HardwareSerial *)this)->write(packet.data(), packet.length());
    digitalWrite(this->rtsPin, HIGH);
}

void HartUdp_c::hartToUdp()
{
    if (packet != NULL)
    {
        const size_t tam = ((HardwareSerial *)this)->available();
        if (tam > 0)
        {
            uint8_t data[tam];
            ((HardwareSerial *)this)->readBytes(data, tam);
            packet->write(data, tam);
        }
    }
}