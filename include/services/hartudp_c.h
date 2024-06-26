#include <Arduino.h>
#include <HardwareSerial.h>
#include <hal/uart_types.h>
#include "driver/uart.h"
#include "AsyncUDP.h"

/*https://github.com/espressif/arduino-esp32/issues/8755
Set "USB CDC On Boot" to "Disable" and then Serial will use HardwareSerial API instead of HWCDC API.
board_build.extra_flags =
    -DARDUINO_USB_MODE=1
    -DARDUINO_USB_CDC_ON_BOOT=0
*/

class HartUdp_c : public HardwareSerial, protected AsyncUDP
{

protected:
    uint16_t server_port = 4000;
    IPAddress *remoteIP = NULL;

public:
    HartUdp_c(uint16_t port) : HardwareSerial(UART_NUM_2), AsyncUDP()
    {
        server_port = port;
    }
    bool setup(int8_t rxPin, int8_t txPin, int8_t ctsPin, int8_t rtsPin) { return (setup(server_port, rxPin, txPin, ctsPin, rtsPin)); }
    bool setup(uint16_t port, int8_t rxPin, int8_t txPin, int8_t ctsPin, int8_t rtsPin);
    void hartToUdp();
    void udpToHart(uint8_t *buffer,size_t size,IPAddress remoteIP);
    void loop(){}

protected:
    void handleInput() {}
};

bool HartUdp_c::setup(uint16_t port, int8_t rxPin, int8_t txPin, int8_t ctsPin, int8_t rtsPin)
{
    if (((AsyncUDP *)this)->listen(port))
    {
        server_port = port;
        pinMode(rtsPin, OUTPUT);
        pinMode(ctsPin, INPUT);
        ((HardwareSerial *)this)->begin(1200, SERIAL_8O1, rxPin, txPin);
        ((HardwareSerial *)this)->setPins(rxPin, txPin, ctsPin, rtsPin);
        ((HardwareSerial *)this)->setHwFlowCtrlMode(UART_HW_FLOWCTRL_CTS_RTS, UART_FIFO_LEN - 8);
        //((HardwareSerial *)this)->setMode(UART_MODE_UART);
        ((AsyncUDP *)this)->onPacket([this](AsyncUDPPacket packet)
                                     { udpToHart(packet.data(),packet.length(),packet.remoteIP()); });
        ((HardwareSerial *)this)->onReceive([this](){ hartToUdp(); });
        return true;
    }
    return false;
}

void HartUdp_c::udpToHart(uint8_t *buffer,size_t size,IPAddress remoteIP)
{
    if (size == 8 &&
          buffer[0] == 255 &&
          buffer[1] == 255 &&
          buffer[2] == 0 &&
          buffer[3] == 0 &&
          buffer[4] == 255 &&
          buffer[5] == 255 &&
          buffer[6] == 0 &&
          buffer[7] == 0)
    {
        const uint8_t okBuffer[8] = {255,255,0,0,255,255,0,0};//Solicita conexão
        this->remoteIP = new IPAddress(remoteIP);        
        ((AsyncUDP *)this)->writeTo(okBuffer, 8,remoteIP,this->server_port); 
        //this->packet->write(okBuffer, 8);
    }
    else
    {
        //digitalWrite(this->rtsPin, LOW);
        ((HardwareSerial *)this)->write(buffer, size);
        //digitalWrite(this->rtsPin, HIGH);
    }
}

void HartUdp_c::hartToUdp()
{
    //if (this->packet != NULL)
    //{
        const size_t tam = ((HardwareSerial *)this)->available();
        if (tam > 0)
        {
            uint8_t data[tam];
            ((HardwareSerial *)this)->readBytes(data, tam);
            if (this->remoteIP != NULL) ((AsyncUDP *)this)->writeTo(data, tam,*this->remoteIP,this->server_port);        
            //((HardwareSerial *)this)->write(data, tam);              
            //if (this->packet != NULL) this->packet->write(data, tam);
        }
    //}
}