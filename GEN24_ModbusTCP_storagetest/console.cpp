#include "console.h"

Console::Console() {
    commandline.given_ip = false;
    commandline.given_activatedischargelimit = false;
    commandline.given_activatechargelimit = false;
    commandline.given_maxdischargevalue = false;
    commandline.given_maxchargevalue = false;
    commandline.given_minsocvalue = false;
    commandline.ip = "";
    commandline.activatedischargelimit = false;
    commandline.activatechargelimit = false;
    commandline.maxdischargevalue = 0;
    commandline.maxchargevalue = 0;
    commandline.minsocvalue = 0;

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
    timer->start(200);
}

Console::~Console() {
    GEN24.disconnect();
}

void Console::setargs(QCommandLineParser *args) {
    if(args->positionalArguments().count() > 0) {
        commandline.ip = args->positionalArguments().at(0);
        if(commandline.ip != "") commandline.given_ip = true;
    }

    if(args->isSet("ad")) {
        commandline.given_activatedischargelimit = true;
        if(args->value("ad").toInt() == 0) {
            commandline.activatedischargelimit = false;
        } else {
            commandline.activatedischargelimit = true;
        }
    }
    if(args->isSet("ac")) {
        commandline.given_activatechargelimit = true;
        if(args->value("ac").toInt() == 0) {
            commandline.activatechargelimit = false;
        } else {
            commandline.activatechargelimit = true;
        }
    }
    if(args->isSet("wd")) {
        commandline.given_maxdischargevalue = true;
        commandline.maxdischargevalue = args->value("wd").toFloat();
    }
    if(args->isSet("wc")) {
        commandline.given_maxchargevalue = true;
        commandline.maxchargevalue = args->value("wc").toFloat();
    }
    if(args->isSet("ms")) {
        commandline.given_minsocvalue = true;
        commandline.minsocvalue = args->value("ms").toFloat();
    }
}

void Console::timer_timeout() {
    if(commandline.given_ip == true) {
        GEN24.gen24_connect(commandline.ip,502,3000,200);
        commandline.given_ip = false; //don't connect again
        return;
    }

    if(GEN24.get_connected() == true) {
        if(commandline.given_activatedischargelimit == true) {
            GEN24.set_dischargelimit_enable(commandline.activatedischargelimit);
            commandline.given_activatedischargelimit = false;  //don't set again
            return;
        } else if(commandline.given_activatechargelimit == true) {
            GEN24.set_chargelimit_enable(commandline.activatechargelimit);
            commandline.given_activatechargelimit = false;  //don't set again
            return;
        } else if(commandline.given_maxdischargevalue == true) {
            GEN24.set_W_discharge(commandline.maxdischargevalue);
            commandline.given_maxdischargevalue = false;  //don't set again
            return;
        } else if(commandline.given_maxchargevalue == true) {
            GEN24.set_W_charge(commandline.maxchargevalue);
            commandline.given_maxchargevalue = false;  //don't set again
            return;
        } else if(commandline.given_minsocvalue == true) {
            GEN24.set_minstorage_level(commandline.minsocvalue);
            commandline.given_minsocvalue = false;  //don't set again
            return;
        } else {
            QCoreApplication::quit();
        }
    }
}
