#include <iostream>
#include <chrono>
#include <thread>
#include "open_greenery/adc/ADCFactory.hpp"

namespace oga = open_greenery::adc;
namespace ogd = open_greenery::driver;
namespace chr = std::chrono;
using clk = chr::system_clock;

int main()
{
    // ADC reading sample
    try
    {
        auto a0 = oga::ADCFactory::getInstance().getReader(ogd::ADS1115::Address::GND, ogd::ADS1115::MUX::SINGLE_0);

        clk::time_point end = clk::now() + chr::seconds(10);
        while (clk::now() < end)
        {
            std::cout << "A0: " << a0->read() << std::endl;
            std::this_thread::sleep_for(chr::milliseconds(100));
        }
    }
    catch(const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
