#ifndef LOGIC_LEVEL_HPP
#define LOGIC_LEVEL_HPP

#include <wiringPi.h>

#undef LOW
#undef HIGH

namespace open_greenery
{
namespace gpio
{

enum class LogicLevel : bool
{
    LOW = 0,
    HIGH = 1
};

}
}

#endif //LOGIC_LEVEL_HPP
