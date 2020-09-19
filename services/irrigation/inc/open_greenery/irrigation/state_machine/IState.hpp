#ifndef I_STATE_HPP
#define I_STATE_HPP

#include <cstdint>
#include "StateName.hpp"

namespace open_greenery::irrigation
{

class IState
{
public:
    virtual StateName handleSoilMoisture(std::int16_t _moisture) = 0;
    virtual StateName state() = 0;

    IState() = default;
    virtual ~IState() = default;
};

}

#endif //I_STATE_HPP
