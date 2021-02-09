#include <gtest/gtest.h>
#include <memory>
#include <SQLiteCpp/Database.h>

#include <open_greenery/database/light/ControlHandledWriter.hpp>
#include <open_greenery/database/light/ControlReader.hpp>
#include <open_greenery/database/light/StatusWriter.hpp>

namespace ogdfl = open_greenery::dataflow::light;
namespace ogdbl = open_greenery::database::light;

class LightDaoTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        db = std::make_shared<SQLite::Database>(DATABASE_PATH, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        status_receiver_ = std::make_unique<ogdbl::StatusWriter>(db);
        control_provider_ = std::make_unique<ogdbl::ControlReader>(db);
        control_handled_receiver_ = std::make_unique<ogdbl::ControlHandledWriter>(db);
    }

    void TearDown() override
    {
        std::remove(DATABASE_PATH);
    }

    static constexpr char DATABASE_PATH[]{"test.db3"};

    std::shared_ptr<SQLite::Database> db;
    std::unique_ptr<ogdfl::IStatusReceiver> status_receiver_;
    std::unique_ptr<ogdfl::IControlProvider> control_provider_;
    std::unique_ptr<ogdfl::IControlHandledReceiver> control_handled_receiver_;
};

TEST_F(LightDaoTest, TableCreated)
{
     EXPECT_TRUE(db->tableExists("light"));
}

TEST_F(LightDaoTest, DefaultConfigurationExists)
{
    const auto query_str = "SELECT count(*) AS count FROM light;";
    SQLite::Statement query(*db, query_str);
    query.executeStep();
    const int lines = query.getColumn("count");

    EXPECT_EQ(lines, 1);
}

TEST_F(LightDaoTest, DefaultConfigurationValues)
{
    const auto query_str = "SELECT * FROM light;";
    SQLite::Statement query(*db, query_str);
    query.executeStep();
    const int id = query.getColumn("id");
    const int user_id = query.getColumn("user_id");
    const char * mode = query.getColumn("mode");
    // TODO: time values
    const int is_enabled = query.getColumn("is_enabled");
    const char * control = query.getColumn("control");

    EXPECT_EQ(id, 1);
    EXPECT_EQ(user_id, 0); // Hardcoded user_is
    EXPECT_STREQ(mode, "auto");
    EXPECT_EQ(is_enabled, 0);
    EXPECT_STREQ(control, "handled");
}
