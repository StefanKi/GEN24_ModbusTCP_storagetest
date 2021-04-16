#include "gen24_data_modbus_helperfuncs.h"

gen24_data_modbus_helperfuncs::gen24_data_modbus_helperfuncs() {

}

QString gen24_data_modbus_helperfuncs::gen24_modbus_to_string(QVector<quint16> data) {
    QChar byte;
    QString string;

    string = "";
    for(int i=0 ; i<data.length() ; i++) {
        byte = data.at(i) >> 8;
        string.append(byte);
        byte = data.at(i) & 0xFF;
        string.append(byte);
    }
    return string;
}

void gen24_data_modbus_helperfuncs::zero_status(statusstruct *status) {
    status->state_handle = state_handle_enum::STARTUP;
    status->connected = false;
    status->disconnected_is_wanted = false;
    status->sunspec_inverter_models_loading = false;
    status->sunspec_meter_models_loading = false;
    status->sunspec_inverter_models_loaded = false;
    status->sunspec_meter_models_loaded = false;
    status->sunspec_inverter_ok = false;
    status->sunspec_inverter_storage_ok = false;
    status->sunspec_meter_ok = false;
    status->sunspec_meter_found = false;
    status->sunspec_inverter_model_type = sunspec_model_type_enum::UNKNOWN;
    status->sunspec_meter_model_type = sunspec_model_type_enum::UNKNOWN;
    status->storage_module_listentrynum = 0;
    status->meter_module_listentrynum = 0;
    if(status->data_storage_raw != NULL) {
        delete status->data_storage_raw;
        status->data_storage_raw = NULL;
    }
    if(status->data_meter_raw != NULL) {
        delete status->data_meter_raw;
        status->data_meter_raw = NULL;
    }
}

bool gen24_data_modbus_helperfuncs::data_ready_to_use(statusstruct *status) {
    if((status->connected == true) &&
            (status->data_meter_raw != NULL) &&
            (status->data_storage_raw != NULL) &&
            (status->sunspec_inverter_models_loaded == true) &&
            (status->sunspec_inverter_models_loading == false) &&
            (status->sunspec_inverter_ok == true) &&
            (status->sunspec_inverter_storage_ok == true) &&
            (status->sunspec_meter_found == true) &&
            (status->sunspec_meter_models_loaded == true) &&
            (status->sunspec_meter_models_loading == false) &&
            (status->sunspec_meter_ok == true)) {
        return true;
    } else {
        return false;
    }
}
