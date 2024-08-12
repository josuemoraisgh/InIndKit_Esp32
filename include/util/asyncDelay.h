#include <Arduino.h>

class AsyncDelay_c
{
protected:
    unsigned long _expires = 0;
    unsigned long _delay = 0;

public:
    AsyncDelay_c(const unsigned long &delay);
    void restart(const unsigned long &delay);
    bool isExpired(void);
    void repeat(void);
};

inline AsyncDelay_c::AsyncDelay_c(const unsigned long &delay)
{
    restart(delay);
}

inline void AsyncDelay_c::restart(const unsigned long &delay)
{
    _delay = delay;
    _expires = xTaskGetTickCount() + _delay;
}

inline bool AsyncDelay_c::isExpired(void)
{
    return (TickType_t(xTaskGetTickCount() - _expires) >= 0);
}

inline void AsyncDelay_c::repeat(void)
{
    _expires += _delay;
}