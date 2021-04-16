#include "mainwindow.h"
#include "console.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("GEN24 ModbusTCP storagetest");
    QCoreApplication::setApplicationVersion(VERSION);

    QCommandLineParser parser;
    parser.setApplicationDescription("GEN24 ModbusTCP storagetest");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("IP", "IP address of GEN24");

    QCommandLineOption enable_discharge("ad", "activate discharge limit (0 not activated, 1 activated)", "value");
    parser.addOption(enable_discharge);
    QCommandLineOption enable_charge("ac", "activate charge limit (0 not activated, 1 activated)", "value");
    parser.addOption(enable_charge);
    QCommandLineOption maxdischargeW_option(QStringList() << "wd", "max W to discharge", "value");
    parser.addOption(maxdischargeW_option);
    QCommandLineOption maxchargeW_option(QStringList() << "wc", "max W to charge", "value");
    parser.addOption(maxchargeW_option);
    QCommandLineOption minSoC_option(QStringList() << "ms", "min SoC of storage", "value");
    parser.addOption(minSoC_option);

    parser.process(a);

    MainWindow w;
    Console c;

    if(parser.positionalArguments().count() > 0) {
        c.setargs(&parser);
    } else {
        w.show();
    }

    return a.exec();
}
