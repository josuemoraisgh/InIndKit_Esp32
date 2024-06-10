#include <Arduino.h>
#include "ESPTelnet.h"

/**************** Hart Interface - UART1 **********/
#define def_pin_Hart_RXD 9  // Pino RX da ESP32 conectado ao pino RX do DS8500
#define def_pin_Hart_TXD 10 // Pino TX da ESP32 conectado ao pino TX do DS8500
#define def_pin_Hart_RTS 11 // Pino RTS da ESP32 conectado ao pino RTS do DS8500
#define def_pin_Hart_CTS 6  // Pino CD da ESP32 conectado ao pino CD do DS8500

class Hart_c : public HardwareSerial, private ESPTelnet
{
    // typedef void (*CallbackFunction)(String str);

public:
    // CallbackFunction on_input = NULL;
    Hart_c(uint16_t port) : HardwareSerial(1), ESPTelnet() { this->server_port = server_port; }
    bool setup() { return (setup(server_port)); }
    bool setup(uint16_t port)
    {
        ((ESPTelnet *)this)->stop();
        server_port = port;
        ((HardwareSerial *)this)->begin(1200);
        ((HardwareSerial *)this)->setHwFlowCtrlMode(UART_HW_FLOWCTRL_CTS_RTS);
        ((HardwareSerial *)this)->setMode(UART_MODE_RS485_HALF_DUPLEX); // For Auto RTS function on ESP32
        return (((ESPTelnet *)this)->begin(server_port));
    }
    void loop()
    {
        size_t tam = ((HardwareSerial *)this)->available();
        if (tam > 0)
        {
            uint8_t data[tam];
            ((HardwareSerial *)this)->readBytes(data, tam);
            ((ESPTelnet *)this)->write(data,tam);
        }
        ((ESPTelnet *)this)->loop();
    }
    // void onInputReceived(CallbackFunction f)
    //{
    //     on_input = f;
    // }
};