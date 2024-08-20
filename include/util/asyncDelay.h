#include <Arduino.h>

class AsyncDelay_c
{
protected:
    bool _isMili = true;
    int64_t _expires = 0;
    int64_t _delay = 0;

public:
    AsyncDelay_c(const unsigned long &delay,bool isMili = true);
    void restart(const unsigned long &delay,bool isMili = true);
    bool isExpired(void);
    void repeat(void);
};

inline AsyncDelay_c::AsyncDelay_c(const unsigned long &delay,bool isMili)
{
    restart(delay,isMili);
}

inline void AsyncDelay_c::restart(const unsigned long &delay,bool isMili)
{
    _isMili = isMili;
    _delay = delay;
    _expires = int64_t(esp_timer_get_time() / (_isMili ? 1000 : 1)) + _delay;
}

inline bool AsyncDelay_c::isExpired(void)
{
    return (TickType_t( int64_t(esp_timer_get_time() / (_isMili ? 1000 : 1)) - _expires) >= 0);
}

inline void AsyncDelay_c::repeat(void)
{
    _expires += _delay;
}