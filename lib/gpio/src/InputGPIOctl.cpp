#include "open_greenery/gpio/InputGPIOctl.hpp"
#include <wiringPi.h>
#include <iostream>

namespace open_greenery
{
namespace gpio
{

InputGPIOctl::InputGPIOctl(const PinId _pin, const Pull _pull)
    :GPIOctl(_pin)
{
    pullUpDnControl(pin(), static_cast<int>(_pull));
}

InputGPIOctl::~InputGPIOctl()
{
    pullUpDnControl(pin(), static_cast<int>(Pull::OFF));
}

LogicLevel InputGPIOctl::read() const
{
    return static_cast<LogicLevel>(digitalRead(pin()));
}

}
}
