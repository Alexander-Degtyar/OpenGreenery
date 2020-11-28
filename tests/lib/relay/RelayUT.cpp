#include <gtest/gtest.h>
#include <vector>
#include <open_greenery/relay/Relay.hpp>

namespace ogio = open_greenery::gpio;
namespace ogr = open_greenery::relay;

class OutputPinMock : public ogio::IOutputPin
{
public:

    void write(const ogio::LogicLevel _value) const override
    {
        m_write_calls.push_back(_value);
    }

    const std::vector<ogio::LogicLevel> & writeCalls() const
    {
        return m_write_calls;
    }

private:
    mutable std::vector<ogio::LogicLevel> m_write_calls;
};

class RelayTest : public ::testing::Test
{
protected:
    RelayTest()
        :m_output_pin_mock(std::make_shared<OutputPinMock>()),
        m_relay(m_output_pin_mock)
    {}

    std::shared_ptr<OutputPinMock> m_output_pin_mock;
    ogr::Relay m_relay;
};

TEST(RelayConstructor, NullOutputPin)
{
    EXPECT_THROW(ogr::Relay(nullptr);, std::logic_error);
}

TEST_F(RelayTest, DisabledByDefault)
{
    EXPECT_FALSE(m_relay.enabled());
    ASSERT_EQ(m_output_pin_mock->writeCalls().size(), 1u);
    EXPECT_EQ(m_output_pin_mock->writeCalls().back(), ogio::LogicLevel::LOW);
}

TEST_F(RelayTest, Enable)
{
    m_relay.enable();

    EXPECT_TRUE(m_relay.enabled());
    ASSERT_EQ(m_output_pin_mock->writeCalls().size(), 2u);  // Disable on start, enable after call
    EXPECT_EQ(m_output_pin_mock->writeCalls().back(), ogio::LogicLevel::HIGH);
}

TEST_F(RelayTest, Disable)
{
    m_relay.disable();

    EXPECT_FALSE(m_relay.enabled());
    ASSERT_EQ(m_output_pin_mock->writeCalls().size(), 2u);  // Disable on start, disable after call
    EXPECT_EQ(m_output_pin_mock->writeCalls().back(), ogio::LogicLevel::LOW);
}

TEST_F(RelayTest, Toggle)
{
    const auto first {m_relay.enabled()};
    m_relay.toggle();
    const auto second {m_relay.enabled()};

    EXPECT_NE(first, second);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
