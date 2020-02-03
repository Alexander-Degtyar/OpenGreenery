#include "open_greenery/adc/ADS1115Reader.hpp"

namespace open_greenery
{
namespace adc
{

ADS1115Reader::ADS1115Reader(ogd::ADS1115 & _adc, ogd::ADS1115::MUX _mux)
    :m_adc(_adc),
    m_mux(_mux)
{}

std::int16_t ADS1115Reader::read() const
{
    return m_adc.read(m_mux);
}

}
}
