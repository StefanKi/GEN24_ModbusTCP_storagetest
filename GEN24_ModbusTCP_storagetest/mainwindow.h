#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "global.h"
#include "../GEN24_ModbusTCP_library/gen24_data_modbus_tcp.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    private:
    Ui::MainWindow *ui;

    GEN24_data_modbus_tcp_class GEN24;
    QTimer *timer;

    void update_ui();

    private slots:
    void timer_timeout();
    void newdataslot();
    void on_pushButton_charge_send_clicked();
    void on_pushButton_discharge_send_clicked();
    void on_pushButton_min_percent_send_clicked();
    void on_checkBox_chargelimit_clicked();
    void on_checkBox_dischargelimit_clicked();
    void on_pushButton_connect_clicked();
    void on_pushButton_disconnect_clicked();
    void on_lineEdit_ip_editingFinished();
};

#endif // MAINWINDOW_H
