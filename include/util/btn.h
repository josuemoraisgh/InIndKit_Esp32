#include <Arduino.h>
#include <functional>
#define DEBOUNCETIME 50
class Btn_c
{
protected:
    volatile unsigned long reading_time = 0;
    volatile uint8_t status_btn = LOW;
    volatile uint8_t last_status_btn = LOW;
    std::function<void(uint8_t status)> onChanged = NULL; // void (*onChanged)(uint8_t status) = NULL;    
    uint16_t pressed_button_time = 0;
    std::function<void()> onPressed = NULL; //void (*onPressed)(void) = NULL;

public:
    uint8_t pin;
    Btn_c() { pin = 0; }
    Btn_c(uint8_t pinBtn) { pin = pinBtn; }
    void onValueChanged(std::function<void(uint8_t status)> f/*void (*f)(uint8_t status)*/);
    void onPressedWithTime(std::function<void()> f/*void (*f)(void)*/);
    bool debounceBtn();
    void setPin(uint8_t pinBtn);
    void IRAM_ATTR update();
    uint8_t getStatusBtn();
    void setTimePressedButton(uint8_t time);     
};

void Btn_c::setPin(uint8_t pinBtn)
{
    pin = pinBtn;
}

void IRAM_ATTR Btn_c::update()
{
    if (debounceBtn())
        onChanged(status_btn);
}

bool Btn_c::debounceBtn()
{
    uint8_t leitura = digitalRead(pin); // A variável leitura recebe a leitura do pino do botão: HIGH (pressionado) ou LOW (Desacionado)
    if (pressed_button_time > 0 && leitura == HIGH && status_btn == HIGH && (xTaskGetTickCount() - reading_time) > pressed_button_time)
    {
        reading_time = xTaskGetTickCount(); //versão do millis () que funciona a partir da interrupção
        onPressed();
    }
    if (leitura != last_status_btn) // Se a leitura atual for diferente da leitura anterior
    {
        reading_time = xTaskGetTickCount(); // Reseta a variável btnData.reading_time atribuindo o tempo atual para uma nova contagem
        last_status_btn = leitura; // Atualiza a variável ultimoStatusBotao para o que foi lido na variável leitura
    }
    if ((xTaskGetTickCount() - reading_time) > DEBOUNCETIME) // Se o resultado de (tempo atual - tempoUltimoDebounce) for maior que o tempo que determinamos (tempoDebounce), ou seja, já passou os 50 milissegundos que é o tempo que o botão precisa ficar pressionado para ter a certeza de que ele realmente foi pressionado? Se sim faça:
    {
        if (leitura != status_btn)
        {                         // Verifica se a leitura do botão mudou, ou seja, se é diferente do status que o botão tinha da última vez. Se sim, faça:
            status_btn = leitura; // statusBotao recebe o que foi lido na variável leitura (pressionado = 1 e não pressionado = 0)
            return (true);        // Se valor do status foi alterado
        }
    }
    return (false);
}

void Btn_c::setTimePressedButton(uint8_t time) // Tempo em segundos
{
    pressed_button_time = ((uint16_t) time)*1000;
}

uint8_t Btn_c::getStatusBtn()
{
    return status_btn;
}

void Btn_c::onValueChanged(std::function<void(uint8_t status)> f)
{
    onChanged = f;
}

void Btn_c::onPressedWithTime(std::function<void()> f)
{
    onPressed = f;
}