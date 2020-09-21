#ifndef I_PUMP_HPP
#define I_PUMP_HPP

#include <chrono>

namespace open_greenery::pump
{

class IPump
{
public:
    virtual void water(const std::chrono::milliseconds _dur) = 0;
    virtual void water(const std::uint16_t _vol_ml) = 0;

    IPump() = default;
    virtual ~IPump() = default;
};

}

#endif //I_PUMP_HPPP
