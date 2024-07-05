#include <Arduino.h>
#include <HardwareSerial.h>
#include <hal/uart_types.h>
#include "driver/uart.h"
#include "AsyncUDP.h"
// https://github.com/espressif/esp-idf/blob/master/examples/peripherals/uart/uart_events/main/uart_events_example_main.c
// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/uart.html
// https://controllerstech.com/how-to-use-uart-in-esp32-esp-idf/
/* https://github.com/espressif/arduino-esp32/issues/8755
Set "USB CDC On Boot" to "Disable" and then Serial will use HardwareSerial API instead of HWCDC API.
board_build.extra_flags =
    -D ARDUINO_USB_CDC_ON_BOOT=1
    -D ARDUINO_USB_MODE=1
ou
build_flags =
    -D ARDUINO_USB_CDC_ON_BOOT=1
    -D ARDUINO_USB_MODE=1
*/
const uart_port_t uart_num = UART_NUM_2;
static const int RX_BUF_SIZE = 1024;
class HartUdp_c : public HardwareSerial, protected AsyncUDP
{

protected:
    uint16_t server_port = 4000;
    IPAddress *remoteIP = NULL;
    int8_t ctsPin = -1;
    int8_t rtsPin = -1;

public:
    HartUdp_c(uint16_t port) : HardwareSerial(uart_num), AsyncUDP()
    {
        server_port = port;
    }
    bool setup(int8_t rxPin, int8_t txPin, int8_t ctsPin, int8_t rtsPin) { return (setup(server_port, rxPin, txPin, ctsPin, rtsPin)); }
    bool setup(uint16_t port, int8_t rxPin, int8_t txPin, int8_t ctsPin, int8_t rtsPin);
    void hartToUdp();
    void udpToHart(uint8_t *buffer, size_t size, IPAddress remoteIP);
    void loop() {}

protected:
    void handleInput() {}
};

bool HartUdp_c::setup(uint16_t port, int8_t rxPin, int8_t txPin, int8_t ctsPin, int8_t rtsPin)
{
    if (((AsyncUDP *)this)->listen(port))
    {
        server_port = port;
        this->ctsPin = ctsPin;
        this->rtsPin = rtsPin;
        uart_config_t uart_config = {
            .baud_rate = 1200,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_ODD,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
            .rx_flow_ctrl_thresh = UART_FIFO_LEN - 8,
            .source_clk = UART_SCLK_APB,
        };
        // Configure UART parameters
        ESP_ERROR_CHECK(uart_driver_install(uart_num, RX_BUF_SIZE * 2, 0, 0, NULL, 0));
        ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
        ESP_ERROR_CHECK(uart_set_mode(uart_num, UART_MODE_UART));
        ESP_ERROR_CHECK(uart_set_pin(uart_num, txPin, rxPin, rtsPin, ctsPin));
        //((HardwareSerial *)this)->begin(1200, SERIAL_8O1, rxPin, txPin);
        //((HardwareSerial *)this)->setPins(rxPin, txPin, ctsPin, rtsPin);
        //((HardwareSerial *)this)->setHwFlowCtrlMode(UART_HW_FLOWCTRL_CTS_RTS, UART_FIFO_LEN - 8);
        //((HardwareSerial *)this)->setMode(UART_MODE_UART);
        ((AsyncUDP *)this)->onPacket([this](AsyncUDPPacket packet)
                                     { udpToHart(packet.data(), packet.length(), packet.remoteIP()); });
        ((HardwareSerial *)this)->onReceive([this]()
                                            { hartToUdp(); });
        return true;
    }
    return false;
}

void HartUdp_c::udpToHart(uint8_t *buffer, size_t size, IPAddress remoteIP)
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
        const uint8_t okBuffer[8] = {255, 255, 0, 0, 255, 255, 0, 0}; // Solicita conexão
        this->remoteIP = new IPAddress(remoteIP);
        ((AsyncUDP *)this)->writeTo(okBuffer, 8, remoteIP, this->server_port);
    }
    else
    {
        // digitalWrite(this->rtsPin, HIGH);              // Indicate we have data to transmit
        // while (digitalRead(this->ctsPin) != HIGH);     // Block waiting for other side.
        ((HardwareSerial *)this)->write(buffer, size); // Send the data.
        //((HardwareSerial *)this)->flush();             // Wait for transmit to finish.
        // digitalWrite(this->rtsPin, LOW);
    }
}

void HartUdp_c::hartToUdp()
{
    const size_t tam = ((HardwareSerial *)this)->available();
    if (tam > 0)
    {
        // digitalWrite(this->rtsPin, LOW);
        uint8_t data[tam];
        ((HardwareSerial *)this)->readBytes(data, tam);
        if (this->remoteIP != NULL)
            ((AsyncUDP *)this)->writeTo(data, tam, *this->remoteIP, this->server_port);
    }
}