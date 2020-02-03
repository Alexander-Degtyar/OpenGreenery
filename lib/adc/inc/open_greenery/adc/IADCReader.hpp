#ifndef I_ADC_READER_HPP
#define I_ADC_READER_HPP

#include <cstdint>

namespace open_greenery
{
namespace adc
{

class IADCReader
{
public:
    virtual std::int16_t read() const = 0;
    virtual ~IADCReader() = default;
};

}
}

#endif //I_ADC_READER_HPP
