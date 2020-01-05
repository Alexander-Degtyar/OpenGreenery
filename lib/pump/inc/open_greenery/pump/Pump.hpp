#ifndef PUMP_HPP
#define PUMP_HPP

#include <chrono>
#include <cstdint>
#include "open_greenery/relay/Relay.hpp"

namespace open_greenery
{
namespace pump
{

class Pump
{
public:
    Pump(const open_greenery::gpio::PinId _pin);

    ~Pump();

    void water(const std::chrono::milliseconds _dur);

    void water(const std::uint16_t _vol_ml);

private:
    open_greenery::relay::Relay m_relay;
};

}
}

#endif //PUMP_HPP
