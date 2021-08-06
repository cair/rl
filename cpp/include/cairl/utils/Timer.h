//
// Created by per on 5/15/21.
//

#ifndef FAST_TRADER_TIMER_H
#define FAST_TRADER_TIMER_H
//
// Created by hara on 2017/10/12.
//

#include <chrono>

class TimerClock {
    using SystemClock = std::chrono::system_clock;

public:
    TimerClock()
    {
        start();
    }

    void clear() { start_ = SystemClock::time_point::min(); }
    void start() {
        start_ = SystemClock::now();
    }
    [[nodiscard]] bool is_started() const
    {
        return (start_.time_since_epoch() != SystemClock::duration(0));
    }

    uint64_t get_ms()
    {
        if (is_started()) {
            SystemClock::duration diff = SystemClock::now() - start_;
            return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
        }
        return 0;
    }

    uint64_t get_us()
    {
        if (is_started()) {
            SystemClock::duration diff = SystemClock::now() - start_;
            return std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
        }
        return 0;
    }

    uint64_t get_ns()
    {
        if (is_started()) {
            SystemClock::duration diff = SystemClock::now() - start_;
            return std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count();
        }
        return 0;
    }

private:
    SystemClock::time_point start_ = SystemClock::time_point::min();
};
#endif //FAST_TRADER_TIMER_H
