#include <Arduino.h>
#include <functional>
#define DEBOUNCETIME 50

class DIn_c
{
//private:
//    static uint8_t index;
//    static DIn_c* sDIn[8];

protected:
    uint8_t pin;
    volatile unsigned long reading_time = 0;
    volatile uint8_t status_DIn = LOW;
    volatile uint8_t last_status_DIn = LOW;
    volatile uint16_t pressed_DIn_time = 0;    
    std::function<void(uint8_t status)> onChanged = NULL; // void (*onChanged)(uint8_t status) = NULL;    
    std::function<void()> onPressed = NULL; //void (*onPressed)(void) = NULL;
    bool debounce();
    void update();     

public:
    DIn_c() { DIn_c(0);}
    DIn_c(uint8_t pinDIn);
    void onValueChanged(std::function<void(uint8_t status)> f/*void (*f)(uint8_t status)*/);
    void onPressedWithTime(std::function<void()> f/*void (*f)(void)*/);
    void setPin(uint8_t pinDIn);
    uint8_t getPin(void);    
    uint8_t getStatus();
    void setTimeOnPressed(uint8_t time); 
    friend inline void updateDIn(DIn_c *din);          
};

//uint8_t DIn_c::index = 0;

DIn_c::DIn_c(uint8_t pinDIn) {
    //sDIn[index] = this;
    //index++;
    setPin(pinDIn);
    //attachInterrupt(pinDIn, DIn_c::updateISR, CHANGE);  
}

inline void updateDIn(DIn_c *din) {din->update();}
void DIn_c::update() {
    if (debounce()) onChanged(status_DIn);
}

uint8_t DIn_c::getPin(void) {
    return pin;
}

void DIn_c::setPin(uint8_t pinDIn)
{
    pin = pinDIn;
    pinMode(pin,INPUT_PULLDOWN);    
}

bool DIn_c::debounce()
{
    uint8_t leitura = digitalRead(pin); // A variável leitura recebe a leitura do pino do botão: HIGH (pressionado) ou LOW (Desacionado)
    if (pressed_DIn_time > 0 && leitura == HIGH && status_DIn == HIGH && (xTaskGetTickCount() - reading_time) > pressed_DIn_time)
    {
        reading_time = xTaskGetTickCount(); //versão do millis () que funciona a partir da interrupção
        onPressed();
    }
    if (leitura != last_status_DIn) // Se a leitura atual for diferente da leitura anterior
    {
        reading_time = xTaskGetTickCount(); // Reseta a variável btnData.reading_time atribuindo o tempo atual para uma nova contagem
        last_status_DIn = leitura; // Atualiza a variável ultimoStatusBotao para o que foi lido na variável leitura
    }
    if ((xTaskGetTickCount() - reading_time) > DEBOUNCETIME) // Se o resultado de (tempo atual - tempoUltimoDebounce) for maior que o tempo que determinamos (tempoDebounce), ou seja, já passou os 50 milissegundos que é o tempo que o botão precisa ficar pressionado para ter a certeza de que ele realmente foi pressionado? Se sim faça:
    {
        if (leitura != status_DIn)
        {                         // Verifica se a leitura do botão mudou, ou seja, se é diferente do status que o botão tinha da última vez. Se sim, faça:
            status_DIn = leitura; // statusBotao recebe o que foi lido na variável leitura (pressionado = 1 e não pressionado = 0)
            return (true);        // Se valor do status foi alterado
        }
    }
    return (false);
}

void DIn_c::setTimeOnPressed(uint8_t time) // Tempo em segundos
{
    pressed_DIn_time = ((uint16_t) time)*1000;
}

uint8_t DIn_c::getStatus()
{
    return status_DIn;
}

void DIn_c::onValueChanged(std::function<void(uint8_t status)> f)
{
    onChanged = f;
}

void DIn_c::onPressedWithTime(std::function<void()> f)
{
    onPressed = f;
}
