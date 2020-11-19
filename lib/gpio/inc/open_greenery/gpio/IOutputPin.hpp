#ifndef I_OUTPUT_PIN_HPP
#define I_OUTPUT_PIN_HPP

#include "LogicLevel.hpp"

namespace open_greenery
{
namespace gpio
{

class IOutputPin
{
public:
    virtual void write(const LogicLevel _value) const = 0;

    virtual ~IOutputPin() = default;
};

}
}

#endif //I_OUTPUT_PIN_HPP
