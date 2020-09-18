#ifndef PUMP_HPP
#define PUMP_HPP

#include <chrono>
#include <cstdint>
#include "open_greenery/relay/Relay.hpp"
#include "IPump.hpp"

namespace open_greenery::pump
{

class Pump : public IPump
{
public:
    Pump(const open_greenery::gpio::PinId _pin);

    ~Pump() override;

    void water(const std::chrono::milliseconds _dur) override;

    void water(const std::uint16_t _vol_ml) override;

private:
    open_greenery::relay::Relay m_relay;
};

}

#endif //PUMP_HPP
