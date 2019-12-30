#include "open_greenery/gpio/GPIOFactory.hpp"

namespace open_greenery
{
namespace gpio
{

GPIOFactory & GPIOFactory::getInstance()
{
    static GPIOFactory instance;
    return instance;
}

std::shared_ptr<InputGPIOctl> GPIOFactory::getInputGPIOctl(const PinId _pin, const Pull _pull)
{
    const PinId key_pin_id = _pin.cast_to(Pinout::WIRING_PI);
    if (m_ctls.find(key_pin_id.pin) != m_ctls.end())
    {
        throw std::logic_error("Requested pin already used.");
    }

    const auto ictl = std::make_shared<InputGPIOctl>(key_pin_id, _pull);
    m_ctls[key_pin_id.pin] = ictl;
    return ictl;
}

std::shared_ptr<OutputGPIOctl> GPIOFactory::getOutputGPIOctl(const PinId _pin)
{
    const PinId key_pin_id = _pin.cast_to(Pinout::WIRING_PI);
    if (m_ctls.find(key_pin_id.pin) != m_ctls.end())
    {
        throw std::logic_error("Requested pin already used.");
    }

    const auto octl = std::make_shared<OutputGPIOctl>(key_pin_id);
    m_ctls[key_pin_id.pin] = octl;
    return octl;
}

}
}
