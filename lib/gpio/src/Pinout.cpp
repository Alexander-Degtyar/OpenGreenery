#include "open_greenery/gpio/Pinout.hpp"
#include <stdexcept>

namespace open_greenery
{
namespace gpio
{

std::string toString(const Pinout _pinout)
{
    switch (_pinout)
    {
        case Pinout::BROADCOM:
        {
            return "BROADCOM";
        }
        case Pinout::PHYSICAL:
        {
            return "PHYSICAL";
        }
        case Pinout::WIRING_PI:
        {
            return "WIRING_PI";
        }
        default:
        {
            throw std::logic_error("Unhandled pinout.");
        }
    }
}

}
}
