#ifndef I_INPUT_PIN_HPP
#define I_INPUT_PIN_HPP

#include "LogicLevel.hpp"

namespace open_greenery
{
namespace gpio
{

class IInputPin
{
public:
    virtual LogicLevel read() const = 0;

    virtual ~IInputPin() = default;
};

}
}

#endif //I_INPUT_PIN_HPP
