#ifndef PINOUT_HPP
#define PINOUT_HPP

#include <cstdint>
#include <string>

namespace open_greenery
{
namespace gpio
{

enum class Pinout : std::uint8_t
{
    BROADCOM = 0,
    PHYSICAL = 1,
    WIRING_PI = 2
};

std::string toString(const Pinout _pinout);

}
}

#endif //PINOUT_HPP
