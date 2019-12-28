#ifndef PULL_HPP
#define PULL_HPP

#include <cstdint>

namespace open_greenery
{
namespace gpio
{

enum class Pull : std::uint8_t
{
    OFF = 0,
    DOWN = 1,
    UP = 2
};

}
}

#endif //PULL_HPP
