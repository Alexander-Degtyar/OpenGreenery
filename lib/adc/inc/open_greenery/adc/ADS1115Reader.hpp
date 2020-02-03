#ifndef ADS1115_READER_HPP
#define ADS1115_READER_HPP

#include "IADCReader.hpp"
#include "open_greenery/driver/ads1115/ADS1115.hpp"

namespace open_greenery
{
namespace adc
{

namespace ogd = open_greenery::driver;

class ADS1115Reader final : public IADCReader
{
public:
    ADS1115Reader(ogd::ADS1115 & _adc, ogd::ADS1115::MUX _mux);
    ~ADS1115Reader() override final = default;

    std::int16_t read() const override final;

private:
    ogd::ADS1115 & m_adc;
    ogd::ADS1115::MUX m_mux;
};

}
}

#endif //ADS1115_READER_HPP
