#include "open_greenery/driver/ads1115/ADS1115.hpp"
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>

namespace open_greenery
{
namespace driver
{

ADS1115::Config::Config(const Address _adr)
    :adr(_adr)
{}

ADS1115::ADS1115(const Config _cfg)
    :m_cfg(_cfg)
{
    m_i2c_dev = open("/dev/i2c-1", O_RDWR);
    if (m_i2c_dev < 0)
    {
        throw std::runtime_error("Failed to open the I2C bus.");
    }
    ioctl(m_i2c_dev, I2C_SLAVE, std::uint8_t(m_cfg.adr));
}

ADS1115::ADS1115(const Address _adr)
    :ADS1115(Config{_adr})
{}

std::int16_t ADS1115::read(const Channel _ch)
{
    const std::uint16_t adc_cfg = cfgRegs()
        | channelMask(_ch)
        | std::uint16_t(OS::SINGLE);

    std::uint8_t reg_cfg [3] {};
    reg_cfg[0] = 0x01;// Set Pointer Register Byte to the Config Register
    reg_cfg[1] = (adc_cfg >> 8) & 0x00FF;
    reg_cfg[2] = adc_cfg & 0x00FF;
        
    ::write(m_i2c_dev, reg_cfg, 3);
    std::this_thread::sleep_for(std::chrono::milliseconds(8));// Conversion delay
    
    reg_cfg[0] = 0x00;// Set Pointer Register Byte to the Conversion Register
    ::write(m_i2c_dev, reg_cfg, 1);

    std::uint8_t data [2] {};
    if (::read(m_i2c_dev, &data, 2) != 2)
    {
        throw std::runtime_error("Failed to read the I2C bus.");
    }

    std::int16_t raw_adc = (data[0] * 256 + data[1]);
    if (raw_adc > 32767)
    {
        raw_adc -= 65535;
    }
    
    return raw_adc;
}

ADS1115::Config ADS1115::cfg() const
{
    return m_cfg;
}

std::uint16_t ADS1115::cfgRegs() const
{
    return  std::uint16_t(m_cfg.mux)   |
            std::uint16_t(m_cfg.pga)   |
            std::uint16_t(m_cfg.mode)  |
            std::uint16_t(m_cfg.dr)    |
            std::uint16_t(m_cfg.cm)    |
            std::uint16_t(m_cfg.cp)    |
            std::uint16_t(m_cfg.cl)    |
            std::uint16_t(m_cfg.cq);
}

std::uint16_t ADS1115::channelMask(const Channel _ch) const
{
    switch (_ch)
    {
        case Channel::A0:
            return std::uint16_t(MUX::SINGLE_0);
        case Channel::A1:
            return std::uint16_t(MUX::SINGLE_1);
        case Channel::A2:
            return std::uint16_t(MUX::SINGLE_2);
        case Channel::A3:
            return std::uint16_t(MUX::SINGLE_3);
        default:
            throw std::logic_error("Invalid channel.");
    }
}

}
}
