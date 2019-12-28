#include "open_greenery/gpio/OutputGPIOctl.hpp"
#include <wiringPi.h>

namespace open_greenery
{
namespace gpio
{

OutputGPIOctl::OutputGPIOctl(const PinId _pin)
    :GPIOctl(_pin)
{
    pinMode(pin(), OUTPUT);
}

OutputGPIOctl::~OutputGPIOctl()
{
    write(LogicLevel::LOW);
    pinMode(pin(), INPUT);
}

void OutputGPIOctl::write(const LogicLevel _value) const
{
    digitalWrite(pin(), static_cast<int>(_value));
}

}
}
