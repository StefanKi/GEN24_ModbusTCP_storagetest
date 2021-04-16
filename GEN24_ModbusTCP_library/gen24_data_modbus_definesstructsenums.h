#ifndef GEN24_DATA_MODBUS_DEFINESSTRUCTSENUMS_H
#define GEN24_DATA_MODBUS_DEFINESSTRUCTSENUMS_H

    #define SUNSPEC_STARTADDRESS 40000
    #define SUNSPEC_START_LENGTH 2
    #define SUNSPEC_MODEL_START_LENGTH 2
    #define SUNSPEC_MODEL_COMMON 1
    #define SUNSPEC_MODEL_FLOAT_INVERTER_SINGLEPHASE 111
    #define SUNSPEC_MODEL_FLOAT_INVERTER_THREEPHASE 113
    #define SUNSPEC_MODEL_INTFS_INVERTER_SINGLEPHASE 111
    #define SUNSPEC_MODEL_INTFS_INVERTER_THREEPHASE 113
    #define SUNSPEC_MODEL_NAMEPLATE 120
    #define SUNSPEC_MODEL_SETTINGS 121
    #define SUNSPEC_MODEL_STATUS 122
    #define SUNSPEC_MODEL_CONTROLS 123
    #define SUNSPEC_MODEL_MPPT 160
        #define SUNSPEC_MODEL_MPPT_ID 0
        #define SUNSPEC_MODEL_MPPT_L 1
        #define SUNSPEC_MODEL_MPPT_DCA_SF 2
        #define SUNSPEC_MODEL_MPPT_DCV_SF 3
        #define SUNSPEC_MODEL_MPPT_DCW_SF 4
        #define SUNSPEC_MODEL_MPPT_DCWH_SF 5
        #define SUNSPEC_MODEL_MPPT_Evt 7
        #define SUNSPEC_MODEL_MPPT_N 8
        #define SUNSPEC_MODEL_MPPT_TmsPer 9
        #define SUNSPEC_MODEL_MPPT_1_ID 10
        #define SUNSPEC_MODEL_MPPT_1_IDStr 18
        #define SUNSPEC_MODEL_MPPT_1_DCA 19
        #define SUNSPEC_MODEL_MPPT_1_DCV 20
        #define SUNSPEC_MODEL_MPPT_1_DCW 21
        #define SUNSPEC_MODEL_MPPT_1_DCWH 23
        #define SUNSPEC_MODEL_MPPT_1_Tms 25
        #define SUNSPEC_MODEL_MPPT_1_Tmp 26
        #define SUNSPEC_MODEL_MPPT_1_DCSt 27
        #define SUNSPEC_MODEL_MPPT_1_DCEvt 29
        #define SUNSPEC_MODEL_MPPT_2_ID 30
        #define SUNSPEC_MODEL_MPPT_2_IDStr 38
        #define SUNSPEC_MODEL_MPPT_2_DCA 39
        #define SUNSPEC_MODEL_MPPT_2_DCV 40
        #define SUNSPEC_MODEL_MPPT_2_DCW 41
        #define SUNSPEC_MODEL_MPPT_2_DCWH 43
        #define SUNSPEC_MODEL_MPPT_2_Tms 45
        #define SUNSPEC_MODEL_MPPT_2_Tmp 46
        #define SUNSPEC_MODEL_MPPT_2_DCSt 47
        #define SUNSPEC_MODEL_MPPT_2_DCEvt 49
        #define SUNSPEC_MODEL_MPPT_3_ID 50
        #define SUNSPEC_MODEL_MPPT_3_IDStr 58
        #define SUNSPEC_MODEL_MPPT_3_DCA 59
        #define SUNSPEC_MODEL_MPPT_3_DCV 60
        #define SUNSPEC_MODEL_MPPT_3_DCW 61
        #define SUNSPEC_MODEL_MPPT_3_DCWH 63
        #define SUNSPEC_MODEL_MPPT_3_Tms 65
        #define SUNSPEC_MODEL_MPPT_3_Tmp 66
        #define SUNSPEC_MODEL_MPPT_3_DCSt 67
        #define SUNSPEC_MODEL_MPPT_3_DCEvt 69
        #define SUNSPEC_MODEL_MPPT_4_ID 70
        #define SUNSPEC_MODEL_MPPT_4_IDStr 78
        #define SUNSPEC_MODEL_MPPT_4_DCA 79
        #define SUNSPEC_MODEL_MPPT_4_DCV 80
        #define SUNSPEC_MODEL_MPPT_4_DCW 81
        #define SUNSPEC_MODEL_MPPT_4_DCWH 83
        #define SUNSPEC_MODEL_MPPT_4_Tms 85
        #define SUNSPEC_MODEL_MPPT_4_Tmp 86
        #define SUNSPEC_MODEL_MPPT_4_DCSt 87
        #define SUNSPEC_MODEL_MPPT_4_DCEvt 89
    #define SUNSPEC_MODEL_STORAGE 124
        #define SUNSPEC_MODEL_STORAGE_OFFSET_ID 0
        #define SUNSPEC_MODEL_STORAGE_OFFSET_L 1
        #define SUNSPEC_MODEL_STORAGE_OFFSET_WChaMax 2
        #define SUNSPEC_MODEL_STORAGE_OFFSET_WChaGra 3
        #define SUNSPEC_MODEL_STORAGE_OFFSET_WDisChaGra 4
        #define SUNSPEC_MODEL_STORAGE_OFFSET_StorCtl_Mod 5
        #define SUNSPEC_MODEL_STORAGE_OFFSET_VAChaMax 6
        #define SUNSPEC_MODEL_STORAGE_OFFSET_MinRsvPct 7
        #define SUNSPEC_MODEL_STORAGE_OFFSET_ChaState 8
        #define SUNSPEC_MODEL_STORAGE_OFFSET_StorAval 9
        #define SUNSPEC_MODEL_STORAGE_OFFSET_InBatV 10
        #define SUNSPEC_MODEL_STORAGE_OFFSET_ChaSt 11
        #define SUNSPEC_MODEL_STORAGE_OFFSET_OutWRte 12
        #define SUNSPEC_MODEL_STORAGE_OFFSET_InWRte 13
        #define SUNSPEC_MODEL_STORAGE_OFFSET_InOutWRte_WinTms 14
        #define SUNSPEC_MODEL_STORAGE_OFFSET_InOutWRte_RvrtTms 15
        #define SUNSPEC_MODEL_STORAGE_OFFSET_InOutWRte_RmpTms 16
        #define SUNSPEC_MODEL_STORAGE_OFFSET_ChaGriSet 17
        #define SUNSPEC_MODEL_STORAGE_OFFSET_WChaMax_SF 18
        #define SUNSPEC_MODEL_STORAGE_OFFSET_WChaDisChaGra_SF 19
        #define SUNSPEC_MODEL_STORAGE_OFFSET_VAChaMax_SF 20
        #define SUNSPEC_MODEL_STORAGE_OFFSET_MinRsvPct_SF 21
        #define SUNSPEC_MODEL_STORAGE_OFFSET_ChaState_SF 22
        #define SUNSPEC_MODEL_STORAGE_OFFSET_StorAval_SF 23
        #define SUNSPEC_MODEL_STORAGE_OFFSET_InBatV_SF 24
        #define SUNSPEC_MODEL_STORAGE_OFFSET_InOutWRte_SF 25
    #define SUNSPEC_MODEL_FLOAT_METER_SINGLEPHASE 211
    #define SUNSPEC_MODEL_FLOAT_METER_DUALPHASE 212
    #define SUNSPEC_MODEL_FLOAT_METER_THREEPHASE 213
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_ID 0
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_L 1
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_A 2
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_AphA 4
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_AphB 6
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_AphC 8
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_PhV 10
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_PhVphA 12
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_PhVphB 14
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_PhVphC 16
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_PPV 18
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_PPVphAB 20
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_PPVphBC 22
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_PPVphCA 24
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_Hz 26
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_W 28
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_WphA 30
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_WphB 32
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_WphC 34
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_VA 36
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_VAphA 38
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_VAphB 40
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_VAphC 42
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_VAR 44
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_VARphA 46
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_VARphB 48
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_VARphC 50
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_PF 52
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_PFphA 54
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_PFphB 56
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_PFphC 58
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotWhExp 60
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotWhExpPhA 62
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotWhExpPhB 64
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotWhExpPhC 66
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotWhImp 68
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotWhImpPhA 70
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotWhImpPhB 72
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotWhImpPhC 74
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVAhExp 76
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVAhExpPhA 78
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVAhExpPhB 80
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVAhExpPhC 82
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVAhImp 84
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVAhImpPhA 86
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVAhImpPhB 88
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVAhImpPhC 90
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhImpQ1 92
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhImpQ1phA 94
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhImpQ1phB 96
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhImpQ1phC 98
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhImpQ2 100
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhImpQ2phA 102
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhImpQ2phB 104
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhImpQ2phC 106
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhExpQ3 108
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhExpQ3phA 110
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhExpQ3phB 112
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhExpQ3phC 114
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhExpQ4 116
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhExpQ4phA 118
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhExpQ4phB 120
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_TotVArhExpQ4phC 122
        #define SUNSPEC_MODEL_FLOAT_METER_OFFSET_Evt 124
    #define SUNSPEC_MODEL_INTSF_METER_SINGLEPHASE 201
    #define SUNSPEC_MODEL_INTSF_METER_DUALPHASE 202
    #define SUNSPEC_MODEL_INTSF_METER_THREEPHASE 203
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_ID 0
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_L 1
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_A 2
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_AphA 3
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_AphB 4
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_AphC 5
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_A_SF 6
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PhV 7
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PhVphA 8
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PhVphB 9
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PhVphC 10
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PPV 11
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PPVphAB 12
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PPVphBC 13
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PPVphCA 14
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_V_SF 15
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_Hz 16
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_Hz_SF 17
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_W 18
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_WphA 19
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_WphB 20
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_WphC 21
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_W_SF 22
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_VA 23
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_VAphA 24
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_VAphB 25
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_VAphC 26
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_VA_SF 27
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_VAR 28
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_VARphA 29
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_VARphB 30
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_VARphC 31
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_VAR_SF 32
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PF 33
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PFphA 34
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PFphB 35
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PFphC 36
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_PF_SF 37
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotWhExp 38
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotWhExpPhA 40
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotWhExpPhB 42
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotWhExpPhC 44
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotWhImp 46
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotWhImpPhA 48
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotWhImpPhB 50
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotWhImpPhC 52
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotWh_SF 54
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVAhExp 55
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVAhExpPhA 57
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVAhExpPhB 59
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVAhExpPhC 61
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVAhImp 63
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVAhImpPhA 65
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVAhImpPhB 67
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVAhImpPhC 69
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVAh_SF 71
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhImpQ1 72
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhImpQ1phA 74
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhImpQ1phB 76
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhImpQ1phC 78
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhImpQ2 80
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhImpQ2phA 82
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhImpQ2phB 84
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhImpQ2phC 86
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhExpQ3 88
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhExpQ3phA 90
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhExpQ3phB 92
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhExpQ3phC 94
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhExpQ4 96
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhExpQ4phA 98
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhExpQ4phB 100
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArhExpQ4phC 102
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_TotVArh_SF 104
        #define SUNSPEC_MODEL_INTSF_METER_OFFSET_Evt 105
    #define SUNSPEC_MODEL_END 0xFFFF


    enum state_handle_enum {
        STARTUP,
        CHECK_SUNSPEC_REQUEST,
        CHECK_SUNSPEC_WAIT_AND_CHECK,
        GET_SUNSPEC_MODEL_ITEMS_REQUEST,
        GET_SUNSPEC_MODEL_ITEMS_WAIT_AND_CHECK,
        FIND_STARTADDRESS_STORAGE_START,
        FIND_STARTADDRESS_METER_START,
        FIND_STARTADDRESS_MPPT_START,
        GET_CURRENT_STORAGEDATA_REQUEST,
        GET_CURRENT_STORAGEDATA_WAIT_AND_CHECK,
        GET_CURRENT_METERDATA_REQUEST,
        GET_CURRENT_METERDATA_WAIT_AND_CHECK,
        GET_CURRENT_MPPTDATA_REQUEST,
        GET_CURRENT_MPPTDATA_WAIT_AND_CHECK,
        WAIT,
        ERROR
    };

    enum handle_calledfrom_enum {
        TIMER = 0,
        STATECHANGED = 1,
        READYREAD = 2
    };

    enum storage_status_enum {
        OFF = 1,
        EMPTY = 2,
        DISCHARGING = 3,
        CHARGING = 4,
        FULL = 5,
        HOLDING = 6,
        TESTING = 7
    };

    enum storage_chargefrom_enum {
        PV = 0,   // (Charging from grid disabled)
        GRID = 1, // (Charging from grid enabled)
    };

    enum sunspec_model_type_enum {
        UNKNOWN = 0,
        INTSF = 1,
        FLOAT = 2,
    };

    struct sunspec_model_item_struct {
        uint16_t startaddress;
        uint16_t length;
        uint16_t id;
    };

    struct datastruct {
        uint16_t address;
        QString description;
        uint16_t data;
    };

    struct configstruct {
        QString ip;
        uint16_t port;
        uint64_t interval_ms;
        uint8_t inverter_id;    //the id from the inverter model
        uint8_t meter_id;       //the id from the fronius smartmeter model
    };

    struct statusstruct {
        state_handle_enum state_handle;
        bool connected;
        bool disconnected_is_wanted;                //if we have a disconnect, then we see in this variable if it is a wanted or an unwanted disconnect - we should reconnect if it isn't wanted
        bool sunspec_inverter_models_loading;
        bool sunspec_meter_models_loading;
        bool sunspec_inverter_models_loaded;
        bool sunspec_meter_models_loaded;
        bool sunspec_inverter_ok;
        bool sunspec_inverter_storage_ok;
        bool sunspec_meter_ok;
        bool sunspec_meter_found;
        sunspec_model_type_enum sunspec_inverter_model_type;
        sunspec_model_type_enum sunspec_meter_model_type;
        uint16_t storage_module_listentrynum;
        uint16_t meter_module_listentrynum;
        uint16_t mppt_module_listentrynum;
        uint16_t *data_storage_raw;
        uint16_t *data_meter_raw;
        uint16_t *data_mppt_raw;
    };

#endif // GEN24_DATA_MODBUS_DEFINESSTRUCTSENUMS_H
