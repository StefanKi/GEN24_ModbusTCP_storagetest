#ifndef CONSOLE_H
#define CONSOLE_H

#include <QSettings>
#include <QCommandLineParser>
#include "global.h"
#include "../GEN24_ModbusTCP_library/gen24_data_modbus_tcp.h"

struct commandline_struct {
    bool given_ip;
    bool given_activatedischargelimit;
    bool given_activatechargelimit;
    bool given_maxdischargevalue;
    bool given_maxchargevalue;
    bool given_minsocvalue;

    QString ip;
    bool activatedischargelimit;
    bool activatechargelimit;
    float maxdischargevalue;
    float maxchargevalue;
    float minsocvalue;
};

class Console : public QObject {
    Q_OBJECT

    public:
    explicit Console();
    ~Console();

    void setargs(QCommandLineParser *args);

    private:
    GEN24_data_modbus_tcp_class GEN24;
    QTimer *timer;
    commandline_struct commandline;

    private slots:
    void timer_timeout();
};

#endif // MAINWINDOW_H
