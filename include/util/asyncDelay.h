#include <Arduino.h>
#define ISMILI 0
#define ISMICROS 1

class AsyncDelay_c
{
protected:
    uint64_t _expires = 0;
    uint64_t _delay = 0;

public:
    AsyncDelay_c(const uint64_t &delay,uint8_t isMili);
    void restart(const uint64_t &delay,uint8_t isMili);
    bool isExpired(void);
    void repeat(void);
};

inline AsyncDelay_c::AsyncDelay_c(const uint64_t &delay,uint8_t isMili)
{
    restart(delay,isMili);
}

inline void AsyncDelay_c::restart(const uint64_t &delay,uint8_t isMili)
{
    if(isMili==ISMILI) _delay = delay * 1000;
    else _delay = delay;
    _expires = esp_timer_get_time();
}

inline bool AsyncDelay_c::isExpired(void)
{
    return( (uint64_t) esp_timer_get_time() - _expires >= _delay);
}

inline void AsyncDelay_c::repeat(void)
{
    _expires += _delay;
}