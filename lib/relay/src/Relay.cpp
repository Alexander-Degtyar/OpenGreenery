#include "open_greenery/relay/Relay.hpp"
#include "open_greenery/gpio/GPIOFactory.hpp"

namespace open_greenery
{
namespace relay
{

namespace ogio = open_greenery::gpio;

Relay::Relay(const ogio::PinId _pin)
    :m_enabled(false),
    m_gpio(ogio::GPIOFactory::getInstance().getOutputGPIOctl(_pin))
{}

Relay::~Relay()
{
    disable();
}

void Relay::enable()
{
    m_gpio->write(ogio::LogicLevel::HIGH);
    m_enabled = true;
}

void Relay::disable()
{
    m_gpio->write(ogio::LogicLevel::LOW);
    m_enabled = false;
}

bool Relay::enabled() const
{
    return m_enabled;
}

}
}
