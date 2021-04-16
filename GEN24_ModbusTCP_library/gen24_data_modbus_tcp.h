#ifndef GEN24_DATA_MODBUS_TCP_H
#define GEN24_DATA_MODBUS_TCP_H

#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QUrl>
#include <QTimer>
#include <QDateTime>
#include <QVariant>
#include <QDebug>
#include <QtMath>

#include "gen24_data_modbus_definesstructsenums.h"
#include "gen24_data_modbus_helperfuncs.h"

class QModbusClient;

class GEN24_data_modbus_tcp_class : public QObject
{
    Q_OBJECT

public:
    GEN24_data_modbus_tcp_class();

    void set_interval_ms(uint64_t interval_ms);
    void gen24_connect(QString ip, uint16_t port, uint64_t interval_ms, uint8_t meter_id);
    void gen24_disconnect();
    void gen24_reconnect(int wait_ms);
    double get_storage_level_percent();
    double get_min_storage_level_percent();
    double get_meter_W();
    double get_max_charge_W();
    double get_max_discharge_W();
    bool get_chargelimit_enable();
    bool get_dischargelimit_enable();

    bool get_connected();
    bool get_storage_ok();
    bool get_meter_ok();
    bool get_error();
    QString get_errorstring();

    void set_minstorage_level(double percent);
    void set_W_charge(double W);
    void set_W_discharge(double W);
    void set_W_load_into_storage(double W, bool force);
    void set_chargelimit_enable(bool enable);
    void set_dischargelimit_enable(bool enable);

signals:
    void newdata();

private:
    gen24_data_modbus_helperfuncs helperfuncs;
    QTimer *timer;
    QTimer *timer_reconnect;
    QModbusClient *modbusDevice;
    configstruct config;
    statusstruct status;
    QList<sunspec_model_item_struct> sunspec_inverter_model_items;
    QList<sunspec_model_item_struct> sunspec_meter_model_items;
    QString errorstring;


    void log(QString text);
    void handle(handle_calledfrom_enum calledfrom);
    void handle(handle_calledfrom_enum calledfrom, QModbusDataUnit modbusdatareceived);
    void modbus_request(uint16_t address, uint16_t len, uint8_t id);


private slots:
    void timer_timeout();
    void timer_reconnect_timeout();
    void modbusdevice_onStateChanged(int state);
    void modbusdevice_readyRead();

};

#endif // GEN24_DATA_MODBUS_TCP_H
