#include <iostream>
#include "open_greenery/driver/ads1115/ADS1115.hpp"

namespace ogd = open_greenery::driver;

int main()
{
    // ADC reading sample
    try
    {
        ogd::ADS1115 adc(ogd::ADS1115::Address::GND);
        std::cout << "GND:             " << adc.read(ogd::ADS1115::Channel::A0) << std::endl;
        std::cout << "3.3V after 1kOm: " << adc.read(ogd::ADS1115::Channel::A1) << std::endl;
        std::cout << "3.3V after 2kOm: " << adc.read(ogd::ADS1115::Channel::A2) << std::endl;
    }
    catch(const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
