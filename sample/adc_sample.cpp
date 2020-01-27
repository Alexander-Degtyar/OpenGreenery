#include <iostream>
#include <chrono>
#include <thread>
#include "open_greenery/driver/ads1115/ADS1115.hpp"

namespace ogd = open_greenery::driver;
namespace chr = std::chrono;
using clk = chr::system_clock;

int main()
{
    // ADC reading sample
    try
    {
        ogd::ADS1115 adc(ogd::ADS1115::Address::GND);

        clk::time_point end = clk::now() + chr::seconds(10);
        while (clk::now() < end)
        {
            std::cout << "A0: " << adc.read(ogd::ADS1115::Channel::A0) << std::endl;
            std::this_thread::sleep_for(chr::milliseconds(100));
        }
    }
    catch(const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
