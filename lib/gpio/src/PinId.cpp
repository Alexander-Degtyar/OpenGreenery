#include "open_greenery/gpio/PinId.hpp"
#include <functional>
#include <map>
#include <stdexcept>
#include "open_greenery/gpio/Pinout.hpp"

namespace open_greenery
{
namespace gpio
{

using PinoutTable = std::map<PinNumber, PinNumber>;

static PinoutTable s_bcm_to_wiring {{
    {2, 8},
    {3, 9},
    {4, 7},

    {14, 15},
    {15, 16},
    {18, 1},

    {17, 0},
    {27, 2},
    {22, 3},

    {23, 4},
    {24, 5},

    {10, 12},
    {9, 13},
    {11, 14},

    {25, 6},
    {8, 10},
    {7, 11},
    {1, 31},

    {0, 30},
    {5, 21},
    {6, 22},
    {13, 23},
    {19, 24},
    {26, 25},

    {12, 26},
    {16, 27},
    {20, 28},
    {21, 29}
}};

static PinoutTable s_phys_to_wiring {{
    {3, 8},
    {5, 9},
    {7, 7},

    {8, 15},
    {10, 16},
    {12, 1},

    {11, 0},
    {13, 2},
    {15, 3},

    {16, 4},
    {18, 5},

    {19, 12},
    {21, 13},
    {23, 14},

    {22, 6},
    {24, 10},
    {26, 11},
    {28, 31},

    {27, 30},
    {29, 21},
    {31, 22},
    {33, 23},
    {35, 24},
    {37, 25},

    {32, 26},
    {36, 27},
    {38, 28},
    {40, 29},
}};

PinId PinId::cast_to(Pinout _po) const
{
    if (_po != Pinout::WIRING_PI)
    {
        throw std::logic_error("Unimplemented PinId cast.");
    }

    auto wiring_cast = [this](const PinoutTable & _table) -> PinId
    {
            const auto wiring_pin = _table.find(pin);
            if (wiring_pin == _table.end())
            {
                throw std::domain_error("Unusable pin: "+toString(pinout)+"."+std::to_string(pin));
            }
            return {wiring_pin->second, Pinout::WIRING_PI};
    };

    switch (pinout)
    {
        case Pinout::WIRING_PI:
        {
            return *this;
            break;
        }
        case Pinout::PHYSICAL:
        {
            return wiring_cast(s_phys_to_wiring);
            break;
        }
        case Pinout::BROADCOM:
        {
            return wiring_cast(s_bcm_to_wiring);
            break;
        }
        default:
        {
            throw std::logic_error("Unsupported pinout.");
        }
    }

    
}

}
}
