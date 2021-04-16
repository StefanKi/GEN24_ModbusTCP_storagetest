#ifndef GEN24_DATA_MODBUS_HELPERFUNCS_H
#define GEN24_DATA_MODBUS_HELPERFUNCS_H

#include <QString>
#include <QVector>

#include "gen24_data_modbus_definesstructsenums.h"

class gen24_data_modbus_helperfuncs {
    public:
    gen24_data_modbus_helperfuncs();

    QString gen24_modbus_to_string(QVector<quint16> data);

    void zero_status(statusstruct *status);
    bool data_ready_to_use(statusstruct *status);
};

#endif // GEN24_DATA_MODBUS_HELPERFUNCS_H
