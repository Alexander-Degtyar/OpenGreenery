#include "open_greenery/pump/Pump.hpp"
#include <iostream>

namespace og = open_greenery;

int main()
{
    std::cout << "Create pump instance." << std::endl;
    og::pump::Pump pump ({7u, og::gpio::Pinout::WIRING_PI});
    std::cout << "Enter volume of watering in ml:" << std::endl;
    unsigned int volume {};
    std::cin >> volume;
    pump.water(volume);
    std::cout << "Disable pump." << std::endl;
    return 0;
}
