#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
    timer->start(200);

    connect(&GEN24, SIGNAL(newdata()), this, SLOT(newdataslot()));

    QString filename;
    filename = "settings.ini";
    QSettings settings(filename, QSettings::IniFormat);
    ui->lineEdit_ip->setText(settings.value("gen24ipaddress", "192.168.21.10").toString());

    ui->label_version->setText(VERSION);
}

MainWindow::~MainWindow() {
    GEN24.disconnect();
    delete ui;
}

void MainWindow::timer_timeout() {
    update_ui();
}

void MainWindow::newdataslot() {
    update_ui();
}

void MainWindow::update_ui() {
    double meter_W;
    double storage_percent;
    double min_storage_percent;
    double max_charge_W;
    double max_discharge_W;
    QString statusstring;
    QString string;

    if(GEN24.get_error() == true) {
        statusstring = QString("Fehler: %1").arg(GEN24.get_errorstring());
    } else {
        if(GEN24.get_connected() == true) {
            if(GEN24.get_meter_ok() == false) {
                statusstring = "Kein Smartmeter gefunden";
            } else if(GEN24.get_storage_ok() == false) {
                statusstring = "Kein Speicher gefunden";
            } else {
                statusstring = "Verbindung OK";
            }
        } else {
            statusstring = "Nicht verbunden";
        }
    }
    meter_W = GEN24.get_meter_W();
    storage_percent = GEN24.get_storage_level_percent();
    min_storage_percent = GEN24.get_min_storage_level_percent();
    max_charge_W = GEN24.get_max_charge_W();
    max_discharge_W = GEN24.get_max_discharge_W();

    string = "";
    string.append(QString("%1\n").arg(statusstring));
    string.append(QString("Netzbezug: %1 W\n").arg(meter_W));
    string.append(QString("Storage aktuell: %1 %\n").arg(storage_percent));
    ui->label_status->setText(string);

    ui->label_percent_min_current->setText(QString("(aktuell: %1 %)").arg(min_storage_percent));
    ui->label_max_charge_current->setText(QString("(aktuell: %1 W)").arg(max_charge_W));
    ui->label_max_discharge_current->setText(QString("(aktuell: %1 W)").arg(max_discharge_W));
    ui->checkBox_chargelimit->setChecked(GEN24.get_chargelimit_enable());
    ui->checkBox_dischargelimit->setChecked(GEN24.get_dischargelimit_enable());

    Q_UNUSED(storage_percent);
    Q_UNUSED(min_storage_percent);
}

void MainWindow::on_pushButton_charge_send_clicked() {
    QString string;
    double watt;

    string = ui->lineEdit_charge->text();
    string = string.replace(",",".");
    watt = string.toDouble();

    GEN24.set_W_charge(watt);
}

void MainWindow::on_pushButton_discharge_send_clicked() {
    QString string;
    double watt;

    string = ui->lineEdit_discharge->text();
    string = string.replace(",",".");
    watt = string.toDouble();

    GEN24.set_W_discharge(watt);
}

void MainWindow::on_pushButton_min_percent_send_clicked() {
    QString string;
    double percent;

    string = ui->lineEdit_percent_min->text();
    string = string.replace(",",".");
    percent = string.toDouble();

    GEN24.set_minstorage_level(percent);
}

void MainWindow::on_checkBox_chargelimit_clicked() {
    GEN24.set_chargelimit_enable(ui->checkBox_chargelimit->isChecked());
}

void MainWindow::on_checkBox_dischargelimit_clicked() {
    GEN24.set_dischargelimit_enable(ui->checkBox_dischargelimit->isChecked());
}

void MainWindow::on_pushButton_connect_clicked() {
    GEN24.gen24_connect(ui->lineEdit_ip->text(),502,3000,200);
}

void MainWindow::on_pushButton_disconnect_clicked() {
    GEN24.gen24_disconnect();
}

void MainWindow::on_lineEdit_ip_editingFinished() {
    QString filename;
    filename = "settings.ini";
    QSettings settings(filename, QSettings::IniFormat);
    settings.setValue("gen24ipaddress", ui->lineEdit_ip->text());
    settings.sync();
}
