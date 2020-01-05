#include "open_greenery/pump/Pump.hpp"
#include <thread>

namespace open_greenery
{
namespace pump
{

Pump::Pump(open_greenery::gpio::PinId _pin)
        : m_relay(_pin)
{}

Pump::~Pump()
{
    m_relay.disable();
}

void Pump::water(const std::chrono::milliseconds _dur)
{
    m_relay.enable();
    std::this_thread::sleep_for(_dur);
    m_relay.disable();
}

void Pump::water(const std::uint16_t _vol_ml)
{
    constexpr float ML_MIN {100.};
    std::chrono::duration<float, std::ratio<60, 1>> watering_dur (_vol_ml/ML_MIN);
    water(std::chrono::duration_cast<std::chrono::milliseconds>(watering_dur));
}


}
}
