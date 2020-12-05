#include <gtest/gtest.h>
#include <memory>
#include <SQLiteCpp/SQLiteCpp.h>
#include <open_greenery/database/SensorWriter.hpp>
#include <open_greenery/database/SensorReader.hpp>
#include <set>
#include <unordered_set>
#include "QDebug"

namespace ogdb = open_greenery::database;
namespace ogdf = open_greenery::dataflow;

class SensorDataTest : public ::testing::Test
{
protected:
    struct SensorRecordHash
    {
        std::size_t operator()(const open_greenery::dataflow::SensorRecord & sr) const
        {
            const std::string str = std::to_string(sr.value)+
                                    sr.timestamp
                                            .toTimeSpec(Qt::UTC)
                                            .toString("yyyy-MM-dd hh:mm:ss")
                                            .toStdString();
            return std::hash<std::string>()(str);
        };
    };

    SensorDataTest()
    {
        db = std::make_shared<SQLite::Database>(DATABASE_PATH, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    }

    void TearDown() override
    {
        std::remove(DATABASE_PATH);
    }

    static QDateTime RoundToSeconds(QDateTime _dt)
    {
        constexpr auto MS_IN_SEC{1000u};
        auto ms_accuracy = _dt.toMSecsSinceEpoch();
        auto s_accuracy = (ms_accuracy / MS_IN_SEC) * MS_IN_SEC;
        _dt.setMSecsSinceEpoch(s_accuracy);
        return _dt;
    };

    static QDateTime strftimeToQDateTime(const char * timestamp)
    {
        // Convert SQL stftime to QDateTime
        auto utc_dt = QDateTime::fromString(QString(timestamp), "yyyy-MM-dd hh:mm:ss.zzz");
        utc_dt.setTimeSpec(Qt::UTC);// SQLite stores datetime in UTC
        auto local_dt = utc_dt.toLocalTime();// Convert UTC to local timezone
        return local_dt;
    }

    static bool equal(const QDateTime & lhs, const QDateTime & rhs)
    {
        return RoundToSeconds(lhs) == RoundToSeconds(rhs);
    }

    template<typename T>
    static bool SetsAreEqual(const T &s1, const T &s2)
    {
        if (s1.size() != s2.size())
        {
            return false;
        }
        for (const auto & element : s1)
        {

            if (s2.find(element) == s2.end())
            {
                return false;
            }
        }
        return true;
    }

    static std::int16_t SingleSensorValue()
    {
        return INT16_MIN;
    }

    static std::set<std::int16_t> MultipleSensorValues(std::uint16_t n)
    {
        const auto step {static_cast<size_t>((INT16_MAX - INT16_MIN) / n)};

        std::set<std::int16_t> expected_values;
        for (std::int64_t i {INT16_MIN}; i < INT16_MAX; i+=step)
        {
            expected_values.insert(i);
        }
        return expected_values;
    }

    static ogdf::SensorRecord SingleSensorRecord()
    {
        QDateTime dt;
        dt.setMSecsSinceEpoch(0);
        return {dt, INT16_MIN};
    }

    static std::unordered_set<ogdf::SensorRecord, SensorRecordHash> MultipleSensorRecords(std::uint16_t n)
    {
        const qint64 DATETIME_MS_MAX = QDateTime({2100, 12, 31}, {23, 59, 59}).toMSecsSinceEpoch();
        const auto value_step {(INT16_MAX - INT16_MIN) / n};
        qint64 DATETIME_MS_STEP {DATETIME_MS_MAX / n};

        std::unordered_set<ogdf::SensorRecord, SensorRecordHash> expected_values;

        std::int64_t val {INT16_MIN};
        QDateTime dt; dt.toTimeSpec(Qt::UTC); dt.setMSecsSinceEpoch(0);
        for (;val <= INT16_MAX && dt.toMSecsSinceEpoch() < DATETIME_MS_MAX;
              val+=value_step, dt = dt.addMSecs(DATETIME_MS_STEP))
        {
            ogdf::SensorRecord rec {RoundToSeconds(dt), std::int16_t(val)};
            expected_values.insert(rec);
        }
        return expected_values;
    }

    static std::pair<QDateTime, std::int16_t> SingleSensorDatetimeValue()
    {
        QDateTime dt;
        dt.setMSecsSinceEpoch(0);
        return {dt, INT16_MIN};
    }

    static std::set<std::pair<QDateTime, std::int16_t>> MultipleSensorDatetimeValues(std::uint16_t n)
    {
        const qint64 DATETIME_MS_MAX = QDateTime({2100, 12, 31}, {23, 59, 59}).toMSecsSinceEpoch();
        const auto value_step {(INT16_MAX - INT16_MIN) / n};
        qint64 DATETIME_MS_STEP {DATETIME_MS_MAX / n};

        std::set<std::pair<QDateTime, std::int16_t>> expected_values;

        std::int64_t val {INT16_MIN};
        QDateTime dt; dt.toTimeSpec(Qt::UTC); dt.setMSecsSinceEpoch(0);
        for (;val <= INT16_MAX && dt.toMSecsSinceEpoch() < DATETIME_MS_MAX;
              val+=value_step, dt = dt.addMSecs(DATETIME_MS_STEP))
        {
            expected_values.insert({RoundToSeconds(dt), val});
        }

        return expected_values;
    };

    static constexpr char DATABASE_PATH[]{"test.db3"};
    static constexpr char TABLE_NAME[]{"TEST_TABLE"};

    std::shared_ptr<SQLite::Database> db;
};


class SensorWriterTest : public SensorDataTest
{
protected:
    SensorWriterTest()
    {
        writer = std::make_unique<ogdb::SensorWriter>(ogdb::Table(db, TABLE_NAME));
    }

    std::unique_ptr<open_greenery::dataflow::ISensorDataReceiver> writer;
};

TEST_F(SensorWriterTest, TableExists)
{
    EXPECT_TRUE(db->tableExists(TABLE_NAME));
}

TEST_F(SensorWriterTest, ValueWriteCreatesRow)
{
    writer->write(SingleSensorValue());

    int rows_count = db->exec(std::string("SELECT COUNT(*) FROM ")+TABLE_NAME);
    EXPECT_EQ(rows_count, 1);
}

TEST_F(SensorWriterTest, ValueWrite)
{
    auto expected_value = SingleSensorValue();
    writer->write(expected_value);

    SQLite::Statement query (*db, std::string("SELECT * FROM ") + TABLE_NAME);
    std::vector<std::int16_t> received_data;
    while (query.executeStep())
    {
        const char * s_value = query.getColumn("value");
        const std::int16_t i_value = std::strtoul(s_value, nullptr, 10);
        received_data.push_back(i_value);
    }

    ASSERT_EQ(received_data.size(), 1);
    EXPECT_EQ(received_data.back(), expected_value);
}

TEST_F(SensorWriterTest, ValueWriteAutoTime)
{
    auto sensor_value = SingleSensorValue();
    auto expected_datetime = QDateTime::currentDateTime();// Now
    writer->write(sensor_value);

    SQLite::Statement query(*db, std::string("SELECT * FROM ") + TABLE_NAME);
    std::vector<QDateTime> actual_datetime;
    while (query.executeStep())
    {
        const char * timestamp = query.getColumn("time");
        actual_datetime.push_back(strftimeToQDateTime(timestamp));
    }

    ASSERT_EQ(actual_datetime.size(), 1);
    EXPECT_EQ(RoundToSeconds(actual_datetime.back()), RoundToSeconds(expected_datetime));
}

TEST_F(SensorWriterTest, MultipleInt16Values)
{
    // Write data to DB
    const auto expected_values = MultipleSensorValues(100u);
    for (const auto & value : expected_values)
    {
        writer->write(value);
    }

    // Get data from DB
    SQLite::Statement query (*db, std::string("SELECT * FROM ") + TABLE_NAME);
    std::set<std::int16_t> actual_values;
    while (query.executeStep())
    {
        const char * s_value = query.getColumn("value");
        const std::int16_t i_value = std::strtoul(s_value, nullptr, 10);
        actual_values.insert(i_value);
    }

    // Verification
    std::vector<std::int16_t> diff;
    std::set_symmetric_difference(expected_values.cbegin(), expected_values.cend(),
                        actual_values.cbegin(), actual_values.cend(),
                        std::back_inserter(diff));
    EXPECT_EQ(diff.size(), 0u);// Actual set equals to expected one
}

TEST_F(SensorWriterTest, RecordWriteCreatesRow)
{
    writer->write(SingleSensorRecord());

    int rows_count = db->exec(std::string("SELECT COUNT(*) FROM ")+TABLE_NAME);
    EXPECT_EQ(rows_count, 1);
}

TEST_F(SensorWriterTest, RecordWrite)
{
    const auto expected_record = SingleSensorRecord();
    writer->write(expected_record);

    SQLite::Statement query (*db, std::string("SELECT * FROM ") + TABLE_NAME);
    std::vector<open_greenery::dataflow::SensorRecord> received_data;
    while (query.executeStep())
    {
        const char * timestamp = query.getColumn("time");
        const auto dt = strftimeToQDateTime(timestamp);
        const char * s_value = query.getColumn("value");
        const std::int16_t i_value = std::strtoul(s_value, nullptr, 10);
        received_data.push_back({dt, i_value});
    }

    ASSERT_EQ(received_data.size(), 1);
    EXPECT_EQ(RoundToSeconds(received_data.back().timestamp), RoundToSeconds(expected_record.timestamp));
    EXPECT_EQ(received_data.back().value, expected_record.value);
}

TEST_F(SensorWriterTest, MultipleRecords)
{
    const auto expected_records = MultipleSensorRecords(100u);
    for (const auto & rec : expected_records)
    {
        writer->write(rec);
    }

    SQLite::Statement query (*db, std::string("SELECT * FROM ") + TABLE_NAME);
    std::unordered_set<open_greenery::dataflow::SensorRecord, SensorRecordHash> actual_values;
    while (query.executeStep())
    {
        const char * timestamp = query.getColumn("time");
        const auto dt = strftimeToQDateTime(timestamp);
        const char * s_value = query.getColumn("value");
        const std::int16_t i_value = std::strtoul(s_value, nullptr, 10);
        actual_values.insert({dt, i_value});
    }

    EXPECT_EQ(actual_values.size(), expected_records.size());
    EXPECT_TRUE(SetsAreEqual(expected_records, actual_values));// Actual set equals to expected one
}

TEST_F(SensorWriterTest, DateTimeValueWriteCreatesRow)
{
    const auto expected_pair = SingleSensorDatetimeValue();

    writer->write(expected_pair.first, expected_pair.second);

    int rows_count = db->exec(std::string("SELECT COUNT(*) FROM ")+TABLE_NAME);
    EXPECT_EQ(rows_count, 1);
}

TEST_F(SensorWriterTest, DateTimeValueWrite)
{
    const auto expected_pair = SingleSensorDatetimeValue();

    writer->write(expected_pair.first, expected_pair.second);
    using DateTimeValueRecord = std::pair<QDateTime, std::int16_t>;
    SQLite::Statement query (*db, std::string("SELECT * FROM ") + TABLE_NAME);
    std::vector<DateTimeValueRecord> received_data;
    while (query.executeStep())
    {
        const char * timestamp = query.getColumn("time");
        const auto dt = strftimeToQDateTime(timestamp);
        const char * s_value = query.getColumn("value");
        const std::int16_t i_value = std::strtoul(s_value, nullptr, 10);
        received_data.push_back({dt, i_value});
    }

    ASSERT_EQ(received_data.size(), 1);
    EXPECT_EQ(RoundToSeconds(received_data.back().first), RoundToSeconds(expected_pair.first));
    EXPECT_EQ(received_data.back().second, expected_pair.second);
}

TEST_F(SensorWriterTest, MultipleDateTimeValues)
{
    const auto expected_pairs = MultipleSensorDatetimeValues(100u);

    for (const auto & pair : expected_pairs)
    {
        writer->write(pair.first, pair.second);
    }

    SQLite::Statement query (*db, std::string("SELECT * FROM ") + TABLE_NAME);
    std::set<std::pair<QDateTime, std::int16_t>> actual_values;
    while (query.executeStep())
    {
        const char * timestamp = query.getColumn("time");
        const auto dt = strftimeToQDateTime(timestamp);
        const char * s_value = query.getColumn("value");
        const std::int16_t i_value = std::strtoul(s_value, nullptr, 10);
        actual_values.insert({dt, i_value});
    }
    EXPECT_EQ(actual_values.size(), expected_pairs.size());

    std::vector<std::pair<QDateTime, std::int16_t>> diff;
    std::set_symmetric_difference(expected_pairs.cbegin(), expected_pairs.cend(),
                                  actual_values.cbegin(), actual_values.cend(),
                                  std::back_inserter(diff));


    EXPECT_EQ(diff.size(), 0u);// Actual set equals to expected one
}

class SensorDataFunctionalTest : public SensorDataTest
{
protected:
    SensorDataFunctionalTest()
    {
        auto table = ogdb::Table(db, TABLE_NAME);
        reader = std::make_unique<ogdb::SensorReader>(table);
        writer = std::make_unique<ogdb::SensorWriter>(table);
    }

    std::function<QDateTime()> now = QDateTime::currentDateTime;
    constexpr static std::uint8_t SEC_IN_MIN {60u};
    std::unique_ptr<open_greenery::dataflow::ISensorDataReceiver> writer;
    std::unique_ptr<open_greenery::dataflow::ISensorDataProvider> reader;
};

TEST_F(SensorDataFunctionalTest, SingleValue)
{
    const auto expected_value = SingleSensorValue();

    writer->write(expected_value);

    const auto actual_data = reader->read(now().addSecs(-SEC_IN_MIN), now().addSecs(SEC_IN_MIN));

    ASSERT_EQ(actual_data.size(), 1u);
    EXPECT_EQ(actual_data.back().value, expected_value);
}

TEST_F(SensorDataFunctionalTest, SingleValueAutoTime)
{
    auto expected_datetime = QDateTime::currentDateTime();// Now

    writer->write(SingleSensorValue());

    const auto actual_data = reader->read(expected_datetime.addSecs(-SEC_IN_MIN), expected_datetime.addSecs(SEC_IN_MIN));

    ASSERT_EQ(actual_data.size(), 1);
    EXPECT_EQ(RoundToSeconds(actual_data.back().timestamp), RoundToSeconds(expected_datetime));
}

TEST_F(SensorDataFunctionalTest, MultipleValues)
{
    // Write data to DB
    const auto expected_values = MultipleSensorValues(100u);
    const auto start = now();
    for (const auto &value : expected_values)
    {
        writer->write(value);
    }

    // Get data from DB
    const auto received_data = reader->read(start.addSecs(-SEC_IN_MIN), start.addSecs(SEC_IN_MIN));
    std::set<std::int16_t> actual_values;
    std::for_each(received_data.cbegin(), received_data.cend(),
                  [&actual_values](const ogdf::SensorRecord & rec){actual_values.insert(rec.value);});

    // Verification
    std::vector<std::int16_t> diff;
    std::set_symmetric_difference(expected_values.cbegin(), expected_values.cend(),
                                  actual_values.cbegin(), actual_values.cend(),
                                  std::back_inserter(diff));
    EXPECT_EQ(diff.size(), 0u);// Actual set equals to expected one
}

TEST_F(SensorDataFunctionalTest, SingleRecord)
{
    const auto expected_record = SingleSensorRecord();

    writer->write(expected_record);

    const auto received_data = reader->read(expected_record.timestamp.addSecs(-SEC_IN_MIN),
                                            expected_record.timestamp.addSecs(SEC_IN_MIN));

    ASSERT_EQ(received_data.size(), 1);
    EXPECT_EQ(RoundToSeconds(received_data.back().timestamp), RoundToSeconds(expected_record.timestamp));
    EXPECT_EQ(received_data.back().value, expected_record.value);
}

TEST_F(SensorDataFunctionalTest, MultipleRecords)
{
    const auto expected_records = MultipleSensorRecords(100u);
    std::pair<QDateTime, QDateTime> time_limits {expected_records.cbegin()->timestamp,
                                                 expected_records.cbegin()->timestamp};
    for (const auto & rec : expected_records)
    {
        writer->write(rec);
        if (rec.timestamp < time_limits.first) {time_limits.first = rec.timestamp;}
        if (rec.timestamp > time_limits.second) {time_limits.second = rec.timestamp;}
    }

    const auto received_data = reader->read(time_limits.first, time_limits.second);
    std::unordered_set<ogdf::SensorRecord, SensorRecordHash> actual_records;
    std::for_each(received_data.cbegin(), received_data.cend(),
                  [&actual_records](const ogdf::SensorRecord & rec){actual_records.insert(rec);});

    EXPECT_EQ(actual_records.size(), expected_records.size());
    EXPECT_TRUE(SetsAreEqual(expected_records, actual_records));// Actual set equals to expected one
}

TEST_F(SensorDataFunctionalTest, SingleDateTimeValue)
{
    const auto expected_pair = SingleSensorDatetimeValue();

    writer->write(expected_pair.first, expected_pair.second);

    const auto received_data = reader->read(expected_pair.first.addSecs(-SEC_IN_MIN),
                                            expected_pair.first.addSecs(SEC_IN_MIN));

    ASSERT_EQ(received_data.size(), 1);
    EXPECT_EQ(RoundToSeconds(received_data.back().timestamp), RoundToSeconds(expected_pair.first));
    EXPECT_EQ(received_data.back().value, expected_pair.second);
}

TEST_F(SensorDataFunctionalTest, MultipleDateTimeValues)
{
    const auto expected_pairs = MultipleSensorDatetimeValues(100u);
    std::pair<QDateTime, QDateTime> time_limits {expected_pairs.cbegin()->first,
                                                 expected_pairs.cbegin()->first};
    for (const auto & pair : expected_pairs)
    {
        writer->write(pair.first, pair.second);
        if (pair.first < time_limits.first) {time_limits.first = pair.first;}
        if (pair.first > time_limits.second) {time_limits.second = pair.first;}
    }

    const auto received_data = reader->read(time_limits.first, time_limits.second);
    std::set<std::pair<QDateTime, std::int16_t>> actual_pairs;
    std::for_each(received_data.cbegin(), received_data.cend(),
                  [&actual_pairs](const ogdf::SensorRecord & rec){actual_pairs.insert({rec.timestamp, rec.value});});

    std::vector<std::pair<QDateTime, std::int16_t>> diff;
    std::set_symmetric_difference(expected_pairs.cbegin(), expected_pairs.cend(),
                                  actual_pairs.cbegin(), actual_pairs.cend(),
                                  std::back_inserter(diff));

    EXPECT_EQ(diff.size(), 0u);// Actual set equals to expected one
}
