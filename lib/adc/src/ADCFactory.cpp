#include "open_greenery/adc/ADCFactory.hpp"
#include "open_greenery/adc/ADS1115Reader.hpp"

namespace open_greenery
{
namespace adc
{

ADCFactory & ADCFactory::getInstance()
{
    static ADCFactory instance;
    return instance;
}

std::shared_ptr<IADCReader> ADCFactory::getReader(const ogd::ADS1115::Address _adr,
                                                  const ogd::ADS1115::MUX _mux)
{
    if (m_ads1115.find(_adr) == m_ads1115.end())
    {
        m_ads1115[_adr] = ADS1115Storage();
    }
    ADS1115Storage & storage = m_ads1115[_adr];
    std::unique_ptr<ogd::ADS1115> & adc = storage.adc;
    std::map<ogd::ADS1115::MUX, std::shared_ptr<IADCReader>> & readers = storage.readers;

    if (!adc)
    {
        adc = std::make_unique<ogd::ADS1115>(_adr);
    }

    if (readers.find(_mux) == readers.end())
    {
        readers[_mux] = std::make_shared<ADS1115Reader>(*adc, _mux);
    }

    return readers[_mux];
}

}
}
