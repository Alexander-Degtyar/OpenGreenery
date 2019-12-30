#include "open_greenery/gpio/GPIOctl.hpp"
#include <mutex>
#include <wiringPi.h>
#include "open_greenery/gpio/PinId.hpp"
#include "open_greenery/gpio/Pinout.hpp"

namespace open_greenery
{
namespace gpio
{

static std::once_flag s_wpi_setup; 

GPIOctl::GPIOctl(const PinId _pin_id)
{
    m_pin = _pin_id.cast_to(Pinout::WIRING_PI).pin;

    std::call_once(s_wpi_setup, wiringPiSetup);
}

PinNumber GPIOctl::pin() const
{
    return m_pin;
}

}
}
