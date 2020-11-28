#include "open_greenery/relay/Relay.hpp"

namespace open_greenery
{
namespace relay
{

namespace ogio = open_greenery::gpio;

Relay::Relay(std::shared_ptr<ogio::IOutputPin> _pin)
    :m_enabled(false)
{
    if (_pin == nullptr)
    {
        throw std::logic_error("Output pin pointer in null");
    }
    m_gpio = std::move(_pin);
    m_gpio->write(ogio::LogicLevel::LOW);
}

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

void Relay::toggle()
{
    if (enabled())
    {
        disable();
    }
    else
    {
        enable();
    }
}

}
}
