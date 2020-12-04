#include "open_greenery/pump/Pump.hpp"
#include <thread>
#include <open_greenery/gpio/GPIOFactory.hpp>

namespace open_greenery
{
namespace pump
{

namespace ogio = open_greenery::gpio;

Pump::Pump(ogio::PinId _pin)
// TODO: Use dependency injection. Get pointer to Pump interface.
        : m_relay(ogio::GPIOFactory::getInstance().getOutputGPIOctl({_pin.pin, _pin.pinout}))
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
    std::chrono::duration<float, std::ratio<60, 1>> watering_dur (float(_vol_ml)/ML_MIN);
    water(std::chrono::duration_cast<std::chrono::milliseconds>(watering_dur));
}


}
}
