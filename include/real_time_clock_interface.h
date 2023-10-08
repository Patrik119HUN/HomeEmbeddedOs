#pragma once
class IRTC {
   public:
    IRTC() {}
    virtual ~IRTC() {}
    virtual bool begin(TwoWire*) = 0;
    virtual void set_time(const DateTime&) = 0;
    virtual uint32_t get_time() = 0;
    virtual bool lost_power(void) = 0;
    virtual void disable_alarm(uint8_t) = 0;
    virtual void clear_alarm(uint8_t) = 0;
    virtual bool alarm_fired(uint8_t) = 0;
};
