#include "gen24_data_modbus_tcp.h"

//TODO: error string anpassen wenn status.state_handle = state_handle_enum::ERROR gemacht wird


//PUBLIC
GEN24_data_modbus_tcp_class::GEN24_data_modbus_tcp_class() {
    errorstring = "no error";

    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &GEN24_data_modbus_tcp_class::timer_timeout);
    timer->start(5000);

    timer_reconnect = new QTimer;
    connect(timer_reconnect, &QTimer::timeout, this, &GEN24_data_modbus_tcp_class::timer_reconnect_timeout);

    modbusDevice = new QModbusTcpClient(this);
    connect(modbusDevice, &QModbusClient::stateChanged, this, &GEN24_data_modbus_tcp_class::modbusdevice_onStateChanged);

    status.data_storage_raw = NULL; //first we must set it to NULL because we didn't request memory for it
    status.data_meter_raw = NULL; //first we must set it to NULL because we didn't request memory for it
    status.data_mppt_raw = NULL; //first we must set it to NULL because we didn't request memory for it
    helperfuncs.zero_status(&status);
}

//PUBLIC
//function gen24_connect
//ip: the ip of the gen24 to connect to
//port: the modbus port of the gen24 to connect to (default 502)
//interval_ms: the interval in which the data will be requested from the gen24
//meter_id: the id of the fronius smartmeter which is connected to the gen24 (default 200)
void GEN24_data_modbus_tcp_class::gen24_connect(QString ip, uint16_t port, uint64_t interval_ms, uint8_t meter_id) {
    if(status.connected == false) {
        config.ip = ip;
        config.port = port;
        config.interval_ms = interval_ms;
        config.meter_id = meter_id;
        config.inverter_id = 1;

        const QUrl url = QUrl::fromUserInput(QString("%1:%2").arg(ip).arg(port));
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
        modbusDevice->setTimeout(1000);
        modbusDevice->setNumberOfRetries(5);
        if (!modbusDevice->connectDevice()) {
            log(QString("connection failed: %1").arg(modbusDevice->errorString()));
        }

        timer->setInterval(interval_ms);
    } else {
        log("already connected. disconnect first");
    }
}

//PUBLIC
void GEN24_data_modbus_tcp_class::gen24_disconnect() {
    status.disconnected_is_wanted = true;
    modbusDevice->disconnectDevice();
    helperfuncs.zero_status(&status);
    log("disconnect");
}

//PUBLIC
void GEN24_data_modbus_tcp_class::gen24_reconnect(int wait_ms=0) {
    timer_reconnect->setSingleShot(true);
    timer_reconnect->start(wait_ms);
}

//PUBLIC
void GEN24_data_modbus_tcp_class::set_interval_ms(uint64_t interval_ms) {
    timer->setInterval(interval_ms);
}

//PUBLIC
double GEN24_data_modbus_tcp_class::get_storage_level_percent() {
    uint16_t ChaState;
    int16_t ChaState_SF;
    double percent;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return 0;
    }

    ChaState = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_ChaState];
    ChaState_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_ChaState_SF];
    percent = (double)ChaState * (qPow(10,ChaState_SF));

    return percent;
}

//PUBLIC
double GEN24_data_modbus_tcp_class::get_min_storage_level_percent() {
    uint16_t MinRsvPct;
    int16_t MinRsvPct_SF;
    double percent;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return 0;
    }

    MinRsvPct = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_MinRsvPct];
    MinRsvPct_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_MinRsvPct_SF];
    percent = (double)MinRsvPct * (qPow(10,MinRsvPct_SF));

    return percent;
}

//PUBLIC
double GEN24_data_modbus_tcp_class::get_meter_W() {
    int16_t W;
    int16_t W_SF;
    uint32_t W_32;
    double meter_W;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return 0;
    }

    if(status.sunspec_meter_model_type == sunspec_model_type_enum::INTSF) {
        W = status.data_meter_raw[SUNSPEC_MODEL_INTSF_METER_OFFSET_W];
        W_SF = status.data_meter_raw[SUNSPEC_MODEL_INTSF_METER_OFFSET_W_SF];
        meter_W = (double)W * (qPow(10,W_SF));
    } else if(status.sunspec_meter_model_type == sunspec_model_type_enum::FLOAT) {
        W_32 = (((uint32_t)status.data_meter_raw[SUNSPEC_MODEL_FLOAT_METER_OFFSET_W]) << 16) | ((uint32_t)status.data_meter_raw[SUNSPEC_MODEL_FLOAT_METER_OFFSET_W+1]);
        meter_W = *(float*)&W_32;
    } else {
        meter_W = 0;
    }

    return meter_W;
}

//PUBLIC
double GEN24_data_modbus_tcp_class::get_max_charge_W() {
    int16_t InWRte;
    int16_t InOutWRte_SF;
    uint16_t WChaMax;
    int16_t WChaMax_SF;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return 0;
    }

    //unfortunately we can't get the W directly. we have to get percentages of maximum available charge/discharge
    //therefore we have to do some calculations to convert the percent into W...
    double maxwatt;
    double percent;
    double watt;

    InWRte = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_InWRte];
    InOutWRte_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_InOutWRte_SF];
    WChaMax = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_WChaMax];
    WChaMax_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_WChaMax_SF];

    maxwatt = (double)WChaMax * (qPow(10,WChaMax_SF));
    percent = (double)InWRte * (qPow(10,InOutWRte_SF));
    watt = maxwatt * ((double)percent) / 100;

    return watt;
}

//PUBLIC
double GEN24_data_modbus_tcp_class::get_max_discharge_W() {
    int16_t OutWRte;
    int16_t InOutWRte_SF;
    uint16_t WChaMax;
    int16_t WChaMax_SF;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return 0;
    }

    //unfortunately we can't get the W directly. we have to get percentages of maximum available charge/discharge
    //therefore we have to do some calculations to convert the percent into W...
    double maxwatt;
    double percent;
    double watt;

    OutWRte = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_OutWRte];
    InOutWRte_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_InOutWRte_SF];
    WChaMax = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_WChaMax];
    WChaMax_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_WChaMax_SF];

    maxwatt = (double)WChaMax * (qPow(10,WChaMax_SF));
    percent = (double)OutWRte * (qPow(10,InOutWRte_SF));
    watt = maxwatt * ((double)percent) / 100;

    return watt;
}

//PUBLIC
bool GEN24_data_modbus_tcp_class::get_chargelimit_enable() {
    uint16_t StorCtl_Mod;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return false;
    }

    StorCtl_Mod = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_StorCtl_Mod];

    if(StorCtl_Mod & 0x01) {
        return true;
    } else {
        return false;
    }
}

//PUBLIC
bool GEN24_data_modbus_tcp_class::get_dischargelimit_enable() {
    uint16_t StorCtl_Mod;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return false;
    }

    StorCtl_Mod = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_StorCtl_Mod];

    if(StorCtl_Mod & 0x02) {
        return true;
    } else {
        return false;
    }
}

//PUBLIC
bool GEN24_data_modbus_tcp_class::get_connected() {
    return status.connected;
}

//PUBLIC
bool GEN24_data_modbus_tcp_class::get_storage_ok() {
    return status.sunspec_inverter_storage_ok;
}

//PUBLIC
bool GEN24_data_modbus_tcp_class::get_meter_ok() {
    return status.sunspec_meter_ok;
}

//PUBLIC
bool GEN24_data_modbus_tcp_class::get_error() {
    if(status.state_handle != state_handle_enum::ERROR) {
        return false;
    } else {
        return true;
    }
}

//PUBLIC
QString GEN24_data_modbus_tcp_class::get_errorstring() {
    return errorstring;
}

//PUBLIC
void GEN24_data_modbus_tcp_class::set_minstorage_level(double percent) {
    uint16_t MinRsvPct;
    int16_t MinRsvPct_SF;
    uint16_t address;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return;
    }

    MinRsvPct_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_MinRsvPct_SF];
    MinRsvPct = (double)percent / (qPow(10,MinRsvPct_SF));


    address = sunspec_inverter_model_items.at(status.storage_module_listentrynum).startaddress + SUNSPEC_MODEL_STORAGE_OFFSET_MinRsvPct;

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, address, config.inverter_id);
    writeUnit.setValue(0, MinRsvPct);

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, config.inverter_id)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    log(QString("Write response error: %1 (Modbus exception: 0x%2)").arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                } else if (reply->error() != QModbusDevice::NoError) {
                    log(QString("Write response error: %1 (code: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        log(QString("Write error: %1").arg(modbusDevice->errorString()));
    }
    log("min storage set");
}

//PUBLIC
void GEN24_data_modbus_tcp_class::set_W_charge(double W) {
    int16_t InWRte;
    int16_t InOutWRte_SF;
    uint16_t WChaMax;
    int16_t WChaMax_SF;
    uint16_t address;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return;
    }

    //unfortunately we can't set the W directly. we have to set percentages of maximum available charge/discharge
    //therefore we have to do some calculations to convert the W into percent...
    double maxwatt;
    double watt_percent;


    WChaMax = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_WChaMax];
    InOutWRte_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_InOutWRte_SF];
    WChaMax_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_WChaMax_SF];

    //we tell the inverter with how many W the storage should be charged or discharged
    maxwatt = (double)WChaMax * (qPow(10,WChaMax_SF));
    watt_percent = W / maxwatt;
    watt_percent *= 100;

    InWRte = (double)watt_percent / (qPow(10,InOutWRte_SF));

    address = sunspec_inverter_model_items.at(status.storage_module_listentrynum).startaddress + SUNSPEC_MODEL_STORAGE_OFFSET_InWRte;
    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, address, 1);
    writeUnit.setValue(0, InWRte);

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, config.inverter_id)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    log(QString("Write response error: %1 (Modbus exception: 0x%2)").arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                } else if (reply->error() != QModbusDevice::NoError) {
                    log(QString("Write response error: %1 (code: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        log(QString("Write error: %1").arg(modbusDevice->errorString()));
    }

    log("set max charge");
}

//PUBLIC
void GEN24_data_modbus_tcp_class::set_W_discharge(double W) {
    int16_t OutWRte;
    int16_t InOutWRte_SF;
    uint16_t WChaMax;
    int16_t WChaMax_SF;
    uint16_t address;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return;
    }

    //unfortunately we can't set the W directly. we have to set percentages of maximum available charge/discharge
    //therefore we have to do some calculations to convert the W into percent...
    double maxwatt;
    double watt_percent;


    WChaMax = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_WChaMax];
    InOutWRte_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_InOutWRte_SF];
    WChaMax_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_WChaMax_SF];

    //we tell the inverter with how many W the storage should be charged or discharged
    maxwatt = (double)WChaMax * (qPow(10,WChaMax_SF));
    watt_percent = W / maxwatt;
    watt_percent *= 100;

    OutWRte = (double)watt_percent / (qPow(10,InOutWRte_SF));

    address = sunspec_inverter_model_items.at(status.storage_module_listentrynum).startaddress + SUNSPEC_MODEL_STORAGE_OFFSET_OutWRte;
    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, address, 1);
    writeUnit.setValue(0, OutWRte);

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, config.inverter_id)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    log(QString("Write response error: %1 (Modbus exception: 0x%2)").arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                } else if (reply->error() != QModbusDevice::NoError) {
                    log(QString("Write response error: %1 (code: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        log(QString("Write error: %1").arg(modbusDevice->errorString()));
    }

    log("set max discharge");
}

//PUBLIC
//set the power (in W) to put into the storage
//when W is negative, the the power will be pulled out of the storage
//force enables (true) or disables (true) the values
//if force is false, then the inverter himselve decides the power to pull in or out of the storage
//if force is true, then the value W will be forced into or out the storage
//if force is true and the storage is in energy saving mode, then the storage will be woken up
//to wake up the storage, the (MinRsvPct) is set to (ChaState - TODO %)
void GEN24_data_modbus_tcp_class::set_W_load_into_storage(double W, bool force) {
    //TODO: wake up the storage
    //TODO: dont wake up the storage for discharging if the targetted minimum storage is reached

    int16_t InWRte;
    int16_t OutWRte;
    int16_t InOutWRte_SF;
    uint16_t StorCtl_Mod;
    uint16_t WChaMax;
    int16_t WChaMax_SF;
    uint16_t address;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return;
    }

    //unfortunately we can't set the W directly. we have to set percentages of maximum available charge/discharge
    //therefore we have to do some calculations to convert the W into percent...
    double maxwatt;
    double watt_percent;


    WChaMax = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_WChaMax];
    InOutWRte_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_InOutWRte_SF];
    WChaMax_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_WChaMax_SF];

    if(force == true) {
        //we tell the inverter with how many W the storage should be charged or discharged
        maxwatt = (double)WChaMax * (qPow(10,WChaMax_SF));
        watt_percent = W / maxwatt;
        watt_percent *= 100;

        InWRte = (double)watt_percent / (qPow(10,InOutWRte_SF));
        OutWRte = (double)-watt_percent / (qPow(10,InOutWRte_SF));

        StorCtl_Mod = 3; //charge and discharge bits are set
    } else {
        //let the inverter decide how many W to be charged or discharged
        //InWRte and OutWRte are set to default values
        InOutWRte_SF = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_InOutWRte_SF];
        InWRte = (double)(100) / (qPow(10,InOutWRte_SF));
        OutWRte = (double)(100) / (qPow(10,InOutWRte_SF));
        StorCtl_Mod = 0; //charge and discharge bits are not set;
    }

    address = sunspec_inverter_model_items.at(status.storage_module_listentrynum).startaddress + SUNSPEC_MODEL_STORAGE_OFFSET_StorCtl_Mod;
    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, address, 1);
    writeUnit.setValue(0, StorCtl_Mod);

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, config.inverter_id)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    log(QString("Write response error: %1 (Modbus exception: 0x%2)").arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                } else if (reply->error() != QModbusDevice::NoError) {
                    log(QString("Write response error: %1 (code: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        log(QString("Write error: %1").arg(modbusDevice->errorString()));
    }

    address = sunspec_inverter_model_items.at(status.storage_module_listentrynum).startaddress + SUNSPEC_MODEL_STORAGE_OFFSET_OutWRte;
    writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, address, 2);
    writeUnit.setValue(0, OutWRte);
    writeUnit.setValue(1, InWRte);

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, config.inverter_id)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    log(QString("Write response error: %1 (Modbus exception: 0x%2)").arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                } else if (reply->error() != QModbusDevice::NoError) {
                    log(QString("Write response error: %1 (code: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        log(QString("Write error: %1").arg(modbusDevice->errorString()));
    }

    log("set W to load");
}

//PUBLIC
void GEN24_data_modbus_tcp_class::set_chargelimit_enable(bool enable) {
    uint16_t StorCtl_Mod;
    uint16_t address;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return;
    }

    StorCtl_Mod = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_StorCtl_Mod];

    if(enable == true) {
        StorCtl_Mod = StorCtl_Mod | 0x01; //set charge bit
    } else {
        StorCtl_Mod = StorCtl_Mod & ~0x01; //clear charge bit
    }

    address = sunspec_inverter_model_items.at(status.storage_module_listentrynum).startaddress + SUNSPEC_MODEL_STORAGE_OFFSET_StorCtl_Mod;
    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, address, 1);
    writeUnit.setValue(0, StorCtl_Mod);

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, config.inverter_id)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    log(QString("Write response error: %1 (Modbus exception: 0x%2)").arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                } else if (reply->error() != QModbusDevice::NoError) {
                    log(QString("Write response error: %1 (code: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        log(QString("Write error: %1").arg(modbusDevice->errorString()));
    }

    log("charglimit enable");
}

//PUBLIC
void GEN24_data_modbus_tcp_class::set_dischargelimit_enable(bool enable) {
    uint16_t StorCtl_Mod;
    uint16_t address;

    if(!helperfuncs.data_ready_to_use(&status)) {
        return;
    }

    StorCtl_Mod = status.data_storage_raw[SUNSPEC_MODEL_STORAGE_OFFSET_StorCtl_Mod];

    if(enable == true) {
        StorCtl_Mod = StorCtl_Mod | 0x02; //set discharge bit
    } else {
        StorCtl_Mod = StorCtl_Mod & ~0x02; //clear discharge bit
    }

    address = sunspec_inverter_model_items.at(status.storage_module_listentrynum).startaddress + SUNSPEC_MODEL_STORAGE_OFFSET_StorCtl_Mod;
    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, address, 1);
    writeUnit.setValue(0, StorCtl_Mod);

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, config.inverter_id)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    log(QString("Write response error: %1 (Modbus exception: 0x%2)").arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                } else if (reply->error() != QModbusDevice::NoError) {
                    log(QString("Write response error: %1 (code: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        log(QString("Write error: %1").arg(modbusDevice->errorString()));
    }

    log("discharge limit enable");
}



//public functions
//####################################################
//private functions


//PRIVATE
void GEN24_data_modbus_tcp_class::timer_timeout() {
    handle(handle_calledfrom_enum::TIMER);
}

//PRIVATE
void GEN24_data_modbus_tcp_class::timer_reconnect_timeout() {
    helperfuncs.zero_status(&status);
    modbusDevice->disconnectDevice();
    status.disconnected_is_wanted = false; //if this is set to false, then it will be automatically reconnected
}

//PRIVATE
void GEN24_data_modbus_tcp_class::handle(handle_calledfrom_enum calledfrom) {
    QModbusDataUnit modbusdatareceived;
    handle(calledfrom, modbusdatareceived);
}

//PRIVATE
//function handle
//this function is called from different other functions which also give the information from which function it has been called
//important info for the state handle:
//  since the inverter info and the meter info are seperate sunspec models on different IDs, some of the first states are used twice - once for the inverter id and once for the meter id
//  following states will be ran twice:
//    CHECK_SUNSPEC_REQUEST
//    CHECK_SUNSPEC_WAIT_AND_CHECK
//    GET_SUNSPEC_MODEL_ITEMS_REQUEST
//    GET_SUNSPEC_MODEL_ITEMS_WAIT_AND_CHECK
void GEN24_data_modbus_tcp_class::handle(handle_calledfrom_enum calledfrom, QModbusDataUnit modbusdatareceived) {
    bool do_another_run = true;

    do {
        do_another_run = false;     //will be set to true, if the next case should be run immediately after that one without waiting for the timer
        switch(status.state_handle) {
            case state_handle_enum::STARTUP:
                if(status.connected == true) {
                    status.state_handle = state_handle_enum::CHECK_SUNSPEC_REQUEST;
                    //first do the inverter model
                    status.sunspec_inverter_models_loading = true;
                    status.sunspec_meter_models_loading = false;
                    status.sunspec_inverter_models_loaded = false;
                    status.sunspec_meter_models_loaded = false;
                    do_another_run = true;
                } else {
                    //not yet connected - is ok, but should change soon
                }
            break;
            case state_handle_enum::CHECK_SUNSPEC_REQUEST:
                if(status.connected == true) {
                    if(status.sunspec_inverter_models_loading == true) {
                        modbus_request(SUNSPEC_STARTADDRESS, SUNSPEC_START_LENGTH, config.inverter_id);
                    } else if(status.sunspec_meter_models_loading == true) {
                        modbus_request(SUNSPEC_STARTADDRESS, SUNSPEC_START_LENGTH, config.meter_id);
                    }
                    status.state_handle = state_handle_enum::CHECK_SUNSPEC_WAIT_AND_CHECK;
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::CHECK_SUNSPEC_WAIT_AND_CHECK:
                if(status.connected == true) {
                    if(calledfrom == handle_calledfrom_enum::READYREAD) {
                        if(modbusdatareceived.valueCount() == SUNSPEC_START_LENGTH) {
                            if(modbusdatareceived.startAddress() == SUNSPEC_STARTADDRESS) {
                                QString string = helperfuncs.gen24_modbus_to_string(modbusdatareceived.values());
                                if(string == "SunS") {
                                    if(status.sunspec_inverter_models_loading == true) {
                                        status.sunspec_inverter_ok = true;
                                        sunspec_inverter_model_items.clear();
                                    } else if(status.sunspec_meter_models_loading == true) {
                                        status.sunspec_meter_ok = false;    //we found the SunS on the meter address, but not the meter himself
                                        sunspec_meter_model_items.clear();
                                    }
                                    status.state_handle = state_handle_enum::GET_SUNSPEC_MODEL_ITEMS_REQUEST;
                                    do_another_run = true;
                                } else {
                                    if(status.sunspec_inverter_models_loading == true) {
                                        errorstring = QString("ERROR: no 0x53756e53 ('SunS') at address %1 for inverter").arg(SUNSPEC_STARTADDRESS);
                                        log(errorstring);
                                    } else if(status.sunspec_meter_models_loading == true) {
                                        errorstring = QString("ERROR: no 0x53756e53 ('SunS') at address %1 for meter").arg(SUNSPEC_STARTADDRESS);
                                        log(errorstring);
                                    }
                                    status.state_handle = state_handle_enum::ERROR;
                                }
                            } else {
                                //modbusdata not as expected - perhaps a wrong ip address is given
                                log("modbusdata not as expected - reconnecting in 5s");
                                status.state_handle = state_handle_enum::ERROR;
                                gen24_reconnect(5000); //wait 5 seconds before reconnect
                            }
                        } else {
                            //modbusdata not as expected - perhaps a wrong ip address is given
                            log("modbusdata not as expected - reconnecting in 5s");
                            status.state_handle = state_handle_enum::ERROR;
                            gen24_reconnect(5000); //wait 5 seconds before reconnect
                        }
                    } else {
                        //not called from the expected function - ignore
                    }
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::GET_SUNSPEC_MODEL_ITEMS_REQUEST:
                if(status.connected == true) {
                    if(status.sunspec_inverter_models_loading == true) {
                        if(sunspec_inverter_model_items.length() == 0) {
                            //first request
                            modbus_request(SUNSPEC_STARTADDRESS + SUNSPEC_START_LENGTH, SUNSPEC_MODEL_START_LENGTH, config.inverter_id);
                        } else {
                            //further requests
                            modbus_request(sunspec_inverter_model_items.last().startaddress + sunspec_inverter_model_items.last().length + SUNSPEC_START_LENGTH, SUNSPEC_MODEL_START_LENGTH, config.inverter_id);
                        }
                    } else if(status.sunspec_meter_models_loading == true) {
                        if(sunspec_meter_model_items.length() == 0) {
                            //first request
                            modbus_request(SUNSPEC_STARTADDRESS + SUNSPEC_START_LENGTH, SUNSPEC_MODEL_START_LENGTH, config.meter_id);
                        } else {
                            //further requests
                            modbus_request(sunspec_meter_model_items.last().startaddress + sunspec_meter_model_items.last().length + SUNSPEC_START_LENGTH, SUNSPEC_MODEL_START_LENGTH, config.meter_id);
                        }
                    }
                    status.state_handle = state_handle_enum::GET_SUNSPEC_MODEL_ITEMS_WAIT_AND_CHECK;
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::GET_SUNSPEC_MODEL_ITEMS_WAIT_AND_CHECK:
                if(status.connected == true) {
                    if(calledfrom == handle_calledfrom_enum::READYREAD) {
                        if(modbusdatareceived.valueCount() == SUNSPEC_MODEL_START_LENGTH) {
                            if(status.sunspec_inverter_models_loading == true) {
                                sunspec_inverter_model_items.append(sunspec_model_item_struct {.startaddress = (uint16_t)modbusdatareceived.startAddress(),
                                                                                               .length = modbusdatareceived.value(1),
                                                                                               .id = modbusdatareceived.value(0)
                                                                                              });
                                //check if this is the end - if not resume...
                                if(sunspec_inverter_model_items.last().id == SUNSPEC_MODEL_END) {
                                    status.sunspec_inverter_models_loaded = true;
                                    status.sunspec_inverter_models_loading = false;
                                    status.sunspec_meter_models_loaded = false;
                                    status.sunspec_meter_models_loading = true;
                                    status.state_handle = state_handle_enum::CHECK_SUNSPEC_REQUEST;
                                    do_another_run = true;
                                } else {
                                    status.state_handle = state_handle_enum::GET_SUNSPEC_MODEL_ITEMS_REQUEST;
                                    do_another_run = true;
                                }
                            } else if(status.sunspec_meter_models_loading == true) {
                                sunspec_meter_model_items.append(sunspec_model_item_struct {.startaddress = (uint16_t)modbusdatareceived.startAddress(),
                                                                                            .length = modbusdatareceived.value(1),
                                                                                            .id = modbusdatareceived.value(0)
                                                                                           });
                                //check if this is the end - if not resume...
                                if(sunspec_meter_model_items.last().id == SUNSPEC_MODEL_END) {
                                    status.sunspec_inverter_models_loaded = true;
                                    status.sunspec_inverter_models_loading = false;
                                    status.sunspec_meter_models_loaded = true;
                                    status.sunspec_meter_models_loading = false;
                                    status.state_handle = state_handle_enum::FIND_STARTADDRESS_STORAGE_START;
                                    do_another_run = true;
                                } else {
                                    status.state_handle = state_handle_enum::GET_SUNSPEC_MODEL_ITEMS_REQUEST;
                                    do_another_run = true;
                                }
                            }
                            //TODO: perhaps check vor valid sunspec model items
                        } else {
                            //modbusdata not as expected - perhaps a wrong ip address is given
                            log("modbusdata not as expected - reconnecting in 5s");
                            status.state_handle = state_handle_enum::ERROR;
                            gen24_reconnect(5000); //wait 5 seconds before reconnect
                        }
                    } else {
                        //not called from the expected function - ignore
                    }
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::FIND_STARTADDRESS_STORAGE_START:
                status.sunspec_inverter_storage_ok = false;
                if(status.connected == true) {
                    for(int i=0 ; i<sunspec_inverter_model_items.length() ; i++) {
                        if(sunspec_inverter_model_items.at(i).id == SUNSPEC_MODEL_STORAGE) {
                            status.sunspec_inverter_storage_ok = true;
                            status.storage_module_listentrynum = i;
                        }
                    }

                    if(status.sunspec_inverter_storage_ok == true) {
                        status.state_handle = state_handle_enum::FIND_STARTADDRESS_METER_START;
                        do_another_run = true;
                    } else {
                        log(QString("no storage module found in sunspec registers"));
                        status.state_handle = state_handle_enum::WAIT;
                        do_another_run = true;
                    }
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::FIND_STARTADDRESS_METER_START:
                status.sunspec_meter_found = false;
                if(status.connected == true) {
                    for(int i=0 ; i<sunspec_meter_model_items.length() ; i++) {
                        if(sunspec_meter_model_items.at(i).id == SUNSPEC_MODEL_INTSF_METER_THREEPHASE) {
                            status.sunspec_meter_model_type = sunspec_model_type_enum::INTSF;
                            status.sunspec_meter_found = true;
                            status.sunspec_meter_ok = true;         //we really found the meter
                            status.meter_module_listentrynum = i;
                        } else if(sunspec_meter_model_items.at(i).id == SUNSPEC_MODEL_FLOAT_METER_THREEPHASE) {
                            status.sunspec_meter_model_type = sunspec_model_type_enum::FLOAT;
                            status.sunspec_meter_found = true;
                            status.sunspec_meter_ok = true;         //we really found the meter
                            status.meter_module_listentrynum = i;
                        }
                    }

                    if(status.sunspec_meter_found == true) {
                        status.state_handle = state_handle_enum::FIND_STARTADDRESS_MPPT_START;  //after storagedata request also the meter data is automatically requested
                        do_another_run = true;
                    } else {
                        log(QString("no meter module found in sunspec registers"));
                        status.state_handle = state_handle_enum::FIND_STARTADDRESS_MPPT_START;
                        do_another_run = true;
                    }
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::FIND_STARTADDRESS_MPPT_START:
                if(status.connected == true) {
                    for(int i=0 ; i<sunspec_inverter_model_items.length() ; i++) {
                        if(sunspec_inverter_model_items.at(i).id == SUNSPEC_MODEL_MPPT) {
                            status.mppt_module_listentrynum = i;
                        }
                    }

                    status.state_handle = state_handle_enum::GET_CURRENT_STORAGEDATA_REQUEST;
                    do_another_run = true;

                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::GET_CURRENT_STORAGEDATA_REQUEST:
                if(status.connected == true) {
                    if(status.sunspec_inverter_storage_ok == true) {
                        modbus_request(sunspec_inverter_model_items.at(status.storage_module_listentrynum).startaddress, sunspec_inverter_model_items.at(status.storage_module_listentrynum).length + SUNSPEC_START_LENGTH, config.inverter_id);
                        status.state_handle = state_handle_enum::GET_CURRENT_STORAGEDATA_WAIT_AND_CHECK;
                    } else {
                        //we shouldn't get here because if no storage or no meter is found, then it should rest in WAIT
                    }
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::GET_CURRENT_STORAGEDATA_WAIT_AND_CHECK:
                if(status.connected == true) {
                    if(modbusdatareceived.valueCount() == (uint)sunspec_inverter_model_items.at(status.storage_module_listentrynum).length + SUNSPEC_START_LENGTH) {
                        if(modbusdatareceived.startAddress() == sunspec_inverter_model_items.at(status.storage_module_listentrynum).startaddress) {
                            if(status.data_storage_raw == NULL) {
                                status.data_storage_raw = new uint16_t[sunspec_inverter_model_items.at(status.storage_module_listentrynum).length + SUNSPEC_START_LENGTH];
                            }
                            for(uint i=0 ; i<(uint)sunspec_inverter_model_items.at(status.storage_module_listentrynum).length + SUNSPEC_START_LENGTH ; i++) {
                                status.data_storage_raw[i] = modbusdatareceived.value(i);
                            }
                            if(status.sunspec_meter_found == false) {
                                status.state_handle = state_handle_enum::GET_CURRENT_MPPTDATA_REQUEST;
                            } else {
                                status.state_handle = state_handle_enum::GET_CURRENT_METERDATA_REQUEST;
                            }
                            do_another_run = true;
                        } else {
                            //modbusdata not as expected
                            log("modbusdata not as expected - reconnecting in 5s");
                            status.state_handle = state_handle_enum::ERROR;
                            gen24_reconnect(5000); //wait 5 seconds before reconnect
                        }
                    } else {
                        //modbusdata not as expected
                        log("modbusdata not as expected - reconnecting in 5s");
                        status.state_handle = state_handle_enum::ERROR;
                        gen24_reconnect(5000); //wait 5 seconds before reconnect
                    }
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::GET_CURRENT_METERDATA_REQUEST:
                if(status.connected == true) {
                    if(status.sunspec_meter_found == true) {
                        //modbus_request(sunspec_meter_model_items.at(status.meter_module_listentrynum).startaddress, sunspec_meter_model_items.at(status.meter_module_listentrynum).length + SUNSPEC_START_LENGTH, config.meter_id);
                        modbus_request(sunspec_meter_model_items.at(status.meter_module_listentrynum).startaddress, sunspec_meter_model_items.at(status.meter_module_listentrynum).length, config.meter_id); //eigentlich wär die obere Zeile richtig - aber irgendwas is da faul wenn man das in der float einstellung beim gen24 abfragen will - da kommt ein fehler
                        status.state_handle = state_handle_enum::GET_CURRENT_METERDATA_WAIT_AND_CHECK;
                    } else {
                        //we shouldn't get here because if no storage or no meter is found, then it should rest in WAIT
                    }
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::GET_CURRENT_METERDATA_WAIT_AND_CHECK:
                if(status.connected == true) {
                    //if(modbusdatareceived.valueCount() == (uint)sunspec_meter_model_items.at(status.meter_module_listentrynum).length + SUNSPEC_START_LENGTH) {
                    if(modbusdatareceived.valueCount() == (uint)sunspec_meter_model_items.at(status.meter_module_listentrynum).length) {  //eigentlich wär die obere Zeile richtig - aber irgendwas is da faul wenn man das in der float einstellung beim gen24 abfragen will - da kommt ein fehler
                        if(modbusdatareceived.startAddress() == sunspec_meter_model_items.at(status.meter_module_listentrynum).startaddress) {
                            if(status.data_meter_raw == NULL) {
                                status.data_meter_raw = new uint16_t[sunspec_meter_model_items.at(status.meter_module_listentrynum).length + SUNSPEC_START_LENGTH];
                            }
                            for(uint i=0 ; i<(uint)sunspec_meter_model_items.at(status.meter_module_listentrynum).length + SUNSPEC_START_LENGTH ; i++) {
                                status.data_meter_raw[i] = modbusdatareceived.value(i);
                            }
                            status.state_handle = state_handle_enum::GET_CURRENT_MPPTDATA_REQUEST;
                            do_another_run = true;
                            emit newdata();
                        } else {
                            //modbusdata not as expected
                            log("modbusdata not as expected - reconnecting in 5s");
                            status.state_handle = state_handle_enum::ERROR;
                            gen24_reconnect(5000); //wait 5 seconds before reconnect
                        }
                    } else {
                        //modbusdata not as expected
                        log("modbusdata not as expected - reconnecting in 5s");
                        status.state_handle = state_handle_enum::ERROR;
                        gen24_reconnect(5000); //wait 5 seconds before reconnect
                    }
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::GET_CURRENT_MPPTDATA_REQUEST:
                if(status.connected == true) {
                    modbus_request(sunspec_inverter_model_items.at(status.mppt_module_listentrynum).startaddress, sunspec_inverter_model_items.at(status.mppt_module_listentrynum).length + SUNSPEC_START_LENGTH, config.inverter_id);
                    status.state_handle = state_handle_enum::GET_CURRENT_MPPTDATA_WAIT_AND_CHECK;
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::GET_CURRENT_MPPTDATA_WAIT_AND_CHECK:
                if(status.connected == true) {
                    if(modbusdatareceived.valueCount() == (uint)sunspec_inverter_model_items.at(status.mppt_module_listentrynum).length + SUNSPEC_START_LENGTH) {
                    if(modbusdatareceived.startAddress() == sunspec_inverter_model_items.at(status.mppt_module_listentrynum).startaddress) {
                            if(status.data_mppt_raw == NULL) {
                                status.data_mppt_raw = new uint16_t[sunspec_inverter_model_items.at(status.mppt_module_listentrynum).length + SUNSPEC_START_LENGTH];
                            }
                            for(uint i=0 ; i<(uint)sunspec_inverter_model_items.at(status.mppt_module_listentrynum).length + SUNSPEC_START_LENGTH ; i++) {
                                status.data_mppt_raw[i] = modbusdatareceived.value(i);
                            }
                            status.state_handle = state_handle_enum::WAIT;
                            do_another_run = true;
                            emit newdata();
                        } else {
                            //modbusdata not as expected
                            log("modbusdata not as expected - reconnecting in 5s");
                            status.state_handle = state_handle_enum::ERROR;
                            gen24_reconnect(5000); //wait 5 seconds before reconnect
                        }
                    } else {
                        //modbusdata not as expected
                        log("modbusdata not as expected - reconnecting in 5s");
                        status.state_handle = state_handle_enum::ERROR;
                        gen24_reconnect(5000); //wait 5 seconds before reconnect
                    }
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::WAIT:
                if(status.connected == true) {
                    if(calledfrom == handle_calledfrom_enum::TIMER) {
                        //called from timer --> get new storage data and do another run
                        status.state_handle = state_handle_enum::GET_CURRENT_STORAGEDATA_REQUEST;   //after storagedata request also the meter data is automatically requested
                        do_another_run = true;
                    } else {
                        //not called from the expected function - ignore
                    }
                } else {
                    //not connected, but should be connected - should be automatically reconnected
                }
            break;
            case state_handle_enum::ERROR:
                //stay here - something serious happend
            break;
            default:

            break;
        }
    } while(do_another_run);
}

//PRIVATE
void GEN24_data_modbus_tcp_class::modbusdevice_onStateChanged(int state) {
    if (state == QModbusDevice::UnconnectedState) {
        status.connected = false;
        log("disconnected");
        if(status.disconnected_is_wanted == false) {
            gen24_connect(config.ip, config.port, config.interval_ms, config.meter_id);
        }
    } else if (state == QModbusDevice::ConnectedState) {
        log("connected");
        if(status.connected == false) {
            status.connected = true;
            status.state_handle = state_handle_enum::STARTUP;
        }
    }
    handle(handle_calledfrom_enum::STATECHANGED);
}

//PRIVATE
void GEN24_data_modbus_tcp_class::log(QString text) {
    qDebug() << text;
}

//PRIVATE
void GEN24_data_modbus_tcp_class::modbusdevice_readyRead() {
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        //log("Data received");

        const QModbusDataUnit unit = reply->result();
        handle(handle_calledfrom_enum::READYREAD, unit);

    } else if (reply->error() == QModbusDevice::ProtocolError) {
        log(QString("Read response error: %1 (Modbus exception: 0x%2)").arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
    } else {
        log(QString("Read response error: %1 (code: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16));
    }

    reply->deleteLater();
}

//PRIVATE
void GEN24_data_modbus_tcp_class::modbus_request(uint16_t address, uint16_t len, uint8_t id) {
    QModbusDataUnit data = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, address, len);
    if(auto *reply = modbusDevice->sendReadRequest(data, id)) {
        if(!reply->isFinished()) {
            connect(reply, SIGNAL(finished()), this, SLOT(modbusdevice_readyRead()));
        } else {
            delete reply; //broadcast replies return immediately
        }
        //log(QString("Modbus request sent"));
    } else {
        log(QString("Modbus read error: %1").arg(modbusDevice->errorString()));
    }
}
