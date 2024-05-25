#include <Arduino.h>

#define DEBOUNCETIME 50

typedef struct
{
    uint8_t pin;
    unsigned long reading_time;
    bool status_btn;
    bool last_status_btn;
} btn_t;

bool debounceBtn(btn_t btn_i)
{
    bool rtn = false;
    bool leitura = digitalRead(btn_i.pin); // A variável leitura recebe a leitura do pino do botão: HIGH (pressionado) ou LOW (Desacionado)

    if (leitura != btn_i.last_status_btn) // Se a leitura atual for diferente da leitura anterior
    {
        btn_i.reading_time = millis(); // Reseta a variável btn_i.reading_time atribuindo o tempo atual para uma nova contagem
    }

    if ((millis() - btn_i.reading_time) > DEBOUNCETIME)
    { // Se o resultado de (tempo atual - tempoUltimoDebounce) for maior que o tempo que determinamos (tempoDebounce), ou seja, já passou os 50 milissegundos que é o tempo que o botão precisa ficar pressionado para ter a certeza de que ele realmente foi pressionado? Se sim faça:
        if (leitura != btn_i.status_btn)
        {                               // Verifica se a leitura do botão mudou, ou seja, se é diferente do status que o botão tinha da última vez. Se sim, faça:
            btn_i.status_btn = leitura; // statusBotao recebe o que foi lido na variável leitura (pressionado = 1 e não pressionado = 0)
            rtn = true;                 // Se valor do status foi alterado
        }
    }
    btn_i.last_status_btn = leitura; // Atualiza a variável ultimoStatusBotao para o que foi lido na variável leitura
    return (rtn);
}