#include <Arduino.h>
#include "AsyncUDP.h"

class UDP_c : public AsyncUDP
{
protected:
    uint16_t server_port = 47269;
    void (*on_input)(String data) = NULL;

public:
    bool start(uint16_t port);
    bool start() { return (start(server_port)); }
    UDP_c(uint16_t port) : AsyncUDP() { this->server_port = server_port; }
    template <typename T>
    void plot(const char *varName, T x, T y, const char *unit = NULL);
    template <typename T>
    void plot(const char *varName, T y, const char *unit = NULL);
    template <typename T>
    void print(const T &data);
    template <typename T>
    void print(const T &data, int base);
    void println();
    template <typename T>
    void println(const T &data);
    template <typename T>
    void println(const T &data, int base);
    uint16_t serverPort() { return (server_port); }
    void onInputReceived(void (*f)(String data));
};

bool UDP_c::start(uint16_t port)
{
    server_port = port;
    if (((AsyncUDP *)this)->connect(IPAddress(200, 19, 148, 219), port))
    {
        Serial.print("UDP Listening on port: ");
        Serial.println(server_port);
        ((AsyncUDP *)this)->onPacket([this](AsyncUDPPacket packet)
                                     {this->on_input(packet.readStringUntil('\n'));});
        return true;
    }
    return false;
}

template <typename T>
void UDP_c::plot(const char *varName, T y, const char *unit)
{
    plot(varName, (T)millis(), y, unit);
}
template <typename T>
void UDP_c::plot(const char *varName, T x, T y, const char *unit)
{
    print(">"); // Inicio de envio de dados para um gráfico.
    print(varName);
    print(":");
    print(x);
    print(":");
    print(y);
    if (unit != NULL)
    {
        print("§"); // Unidade na sequência
        print(unit);
    }
    println("|g"); // Modo Grafico
}

template <typename T>
void UDP_c::print(const T &data)
{
    if (((AsyncUDP *)this)->connected())
        Serial.print(data);
    else
        ((AsyncUDP *)this)->print(data);
}

template <typename T>
void UDP_c::print(const T &data, int base)
{
    if (((AsyncUDP *)this)->connected())
        Serial.print(data, base);
    else
        ((AsyncUDP *)this)->print(data, base);
}

template <typename T>
void UDP_c::println(const T &data)
{
    if (((AsyncUDP *)this)->connected())
        Serial.println(data);
    else
        ((AsyncUDP *)this)->println(data);
}

template <typename T>
void UDP_c::println(const T &data, int base)
{
    if (((AsyncUDP *)this)->connected())
        Serial.println(data, base);
    else
        ((AsyncUDP *)this)->println(data, base);
}

void UDP_c::println()
{
    if (((AsyncUDP *)this)->connected())
        Serial.println();
    else
        ((AsyncUDP *)this)->println();
}

void UDP_c::onInputReceived(void (*f)(String data))
{
    on_input = f;
}