#include <iostream>
#include <chrono>
#include <thread>
#include "open_greenery/adc/ADCFactory.hpp"

namespace oga = open_greenery::adc;
namespace ogd = open_greenery::driver;
namespace ogdf = open_greenery::dataflow;
namespace chr = std::chrono;
using clk = chr::system_clock;

int main()
{
    using ADC_MUX = ogd::ADS1115::MUX;
    // ADC reading sample
    constexpr ADC_MUX adc_ports [4] = {ADC_MUX::SINGLE_0,
                                      ADC_MUX::SINGLE_1,
                                      ADC_MUX::SINGLE_2,
                                      ADC_MUX::SINGLE_3};

    oga::ADCFactory & adc_factory = oga::ADCFactory::getInstance();
    std::map<ADC_MUX, std::shared_ptr<ogdf::ISensorReadProvider>> adc_readers;
    for (const auto port : adc_ports)
    {
        adc_readers[port] = adc_factory.getReader(ogd::ADS1115::Address::GND, port);
    }

    clk::time_point end = clk::now() + chr::seconds(30);
    try
    {
        while (clk::now() < end)
        {
            std::cout << "A0: " << adc_readers[ADC_MUX::SINGLE_0]->read() <<
                " | A1: " << adc_readers[ADC_MUX::SINGLE_1]->read() <<
                " | A2: " << adc_readers[ADC_MUX::SINGLE_2]->read() <<
                " | A3: " << adc_readers[ADC_MUX::SINGLE_3]->read() << std::endl;

            std::this_thread::sleep_for(chr::milliseconds(500));
        }
    }
    catch(const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
