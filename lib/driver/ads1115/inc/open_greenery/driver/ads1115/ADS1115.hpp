#ifndef ADS1115_HPP
#define ADS1115_HPP

#include <chrono>
#include <cstdint>
#include <mutex>

namespace open_greenery
{
namespace driver
{

class ADS1115
{
public:
    enum class Address
    {
        GND = 0x48,// The ADDR pin connected to the GND
        VDD = 0x49,// The ADDR pin connected to the VDD
        SDA = 0x4A,// The ADDR pin connected to the SDA
        SCL = 0x4B // The ADDR pin connected to the SCL
    };

    enum class OS_W : std::uint16_t// Operational status. Single-conversion control
    {
        DISABLE = 0x0000,// Write: Set to disable a single-conversion
        SINGLE  = 0x8000,// Write: Set to start a single-conversion
    };

    enum class OS_R : std::uint8_t// Operational status
    {
        BUSY    = 0x00,// Read: Bit = 0 when conversion is in progress
        NOTBUSY = 0x80,// Read: Bit = 1 when device is not performing a conversion
        MASK    = 0x80
    };

    enum class MUX : std::uint16_t// Input multiplexer configuration
    {
        DIFF_0_1 = 0x0000,// Differential P = AIN0, N = AIN1 (default)
        DIFF_0_3 = 0x1000,// Differential P = AIN0, N = AIN3
        DIFF_1_3 = 0x2000,// Differential P = AIN1, N = AIN3
        DIFF_2_3 = 0x3000,// Differential P = AIN2, N = AIN3
        SINGLE_0 = 0x4000,// Single-ended AIN0
        SINGLE_1 = 0x5000,// Single-ended AIN1
        SINGLE_2 = 0x6000,// Single-ended AIN2
        SINGLE_3 = 0x7000 // Single-ended AIN3
    };

    enum class PGA : std::uint16_t// Programmable gain amplifier configuration
    {
        V_6_114 = 0x0000,// +/-6.144V range = Gain 2/3
        V_4_096 = 0x0200,// +/-4.096V range = Gain 1
        V_2_048 = 0x0400,// +/-2.048V range = Gain 2 (default)
        V_1_024 = 0x0600,// +/-1.024V range = Gain 4
        V_0_512 = 0x0800,// +/-0.512V range = Gain 8
        V_0_256 = 0x0A00 // +/-0.256V range = Gain 16
    };

    enum class MODE : std::uint16_t// Device operating mode
    {
        CONTIN = 0x0000,// Continuous conversion mode
        SINGLE = 0x0100 // Power-down single-shot mode (default)
    };

    enum class DR : std::uint16_t// Data rate
    {    
        SPS_8   = 0x0000,// 8 samples per second
        SPS_16  = 0x0020,// 16 samples per second
        SPS_32  = 0x0040,// 32 samples per second
        SPS_64  = 0x0060,// 64 samples per second
        SPS_128 = 0x0080,// 128 samples per second (default)
        SPS_250 = 0x00A0,// 250 samples per second
        SPS_475 = 0x00C0,// 475 samples per second
        SPS_860 = 0x00E0 // 860 samples per second
    };

    enum class COMP_MODE : std::uint16_t// Comparator mode
    {
        TRAD   = 0x0000,// Traditional comparator with hysteresis (default)
        WINDOW = 0x0010 // Window comparator
    };

    enum class COMP_POL : std::uint16_t// Comparator polarity
    {
        ACTVLOW  = 0x0000,// ALERT/RDY pin is low when active (default)
        ACTVHIGH = 0x0008 // ALERT/RDY pin is high when active
    };

    enum class COMP_LAT : std::uint16_t// Latching comparator
    {
        NONLAT = 0x0000,// Non-latching comparator (default)
        LATCH  = 0x0004 // Latching comparator
    };

    enum class COMP_QUE : std::uint16_t// Comparator queue and disable
    {
        CONV_1 = 0x0000,// Assert ALERT/RDY after one conversions
        CONV_2 = 0x0001,// Assert ALERT/RDY after two conversions
        CONV_4 = 0x0002,// Assert ALERT/RDY after four conversions
        NONE   = 0x0003 // Disable the comparator and put ALERT/RDY in high state (default)
    };

    struct Config
    {
        Address adr;
        MUX mux         = MUX::DIFF_0_1;
        PGA pga         = PGA::V_6_114;
        MODE mode       = MODE::SINGLE;
        DR dr           = DR::SPS_8;
        COMP_MODE cm    = COMP_MODE::TRAD;
        COMP_POL cp     = COMP_POL::ACTVLOW;
        COMP_LAT cl     = COMP_LAT::NONLAT;
        COMP_QUE cq     = COMP_QUE::NONE;

        Config(const Address _adr);
        std::uint16_t bitmask() const;
    };

    enum class Register : std::uint8_t {CONFIG = 0x01, CONVERSION = 0x00};

    ADS1115(const Address _adr);

    ADS1115(const Config _cfg);

    std::int16_t read(const MUX _ch);

    Config config();

    void setConfig(const Config _cfg);

private:
    std::chrono::milliseconds conversionDuration(const DR _dr) const;

    int m_i2c_dev;
    std::mutex m_i2c_mtx;
    Config m_cfg;
    std::mutex m_cfg_mtx;
};

}
}

#endif //ADS1115_HPP
