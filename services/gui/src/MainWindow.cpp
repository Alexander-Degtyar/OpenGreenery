#include "open_greenery/gui/MainWindow.hpp"
#include "open_greenery/gui/page/MultipleCharts.hpp"
#include <QDebug>

namespace open_greenery::gui
{

MainWindow::MainWindow(std::function<void()> _cb_quit_app)
    : m_window_main(),
    m_cb_quit_app(_cb_quit_app),
    m_topbar(static_cast<IAppQuit *>(this)),
    m_page()
{
    auto db = std::make_shared<SQLite::Database>("/home/pi/og/db/soil_moisture.db3", SQLite::OPEN_READONLY);
    qDebug() << "SQLite database file " << QString::fromStdString(db->getFilename()) << " opened successfully";

    open_greenery::database::Table db_tables [] {{db, "A0"},
                                                 {db, "A1"},
                                                 {db, "A2"},
                                                 {db, "A3"}};
    std::vector<open_greenery::gui::page::DateValueChart> soil_moisture_charts;
    for (const auto & db_table : db_tables)
    {
        if (db_table.valid())
        {
            soil_moisture_charts.emplace_back(
                    open_greenery::database::SensorReader(db_table),
                    QString::fromStdString(db_table.name+" soil moisture"));
        }
    }

    const QDateTime date_from = QDateTime::currentDateTime().addMonths(-1);// Month ago
    const QDateTime date_to = QDateTime::currentDateTime();
    for (auto & chart : soil_moisture_charts)
    {
        chart.update(date_from, date_to);
    }
    m_page = new open_greenery::gui::page::MultipleCharts(std::move(soil_moisture_charts));

    auto main_layout = new QVBoxLayout();
    main_layout->addLayout(m_topbar.layout());
    main_layout->addWidget(m_page->widget());

    auto main_wdg = new QWidget();
    main_wdg->setLayout(main_layout);
    m_window_main.setWindowTitle("OpenGreenery");
    m_window_main.setCentralWidget(main_wdg);
    m_window_main.show();
}

void MainWindow::quit()
{
    m_cb_quit_app();
}

}
