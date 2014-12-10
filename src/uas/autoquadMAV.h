#ifndef AUTOQUADMAV_H
#define AUTOQUADMAV_H

#include <stdint.h>

namespace AUTOQUADMAV {

    enum mavlinkCustomDataSets {
        AQMAV_DATASET_LEGACY1 = 0,	// legacy sets can eventually be phased out
        AQMAV_DATASET_LEGACY2,
        AQMAV_DATASET_LEGACY3,
        AQMAV_DATASET_ALL,		// use this to toggle all datasets at once
        AQMAV_DATASET_GPS_XTRA,
        AQMAV_DATASET_UKF_XTRA,
        AQMAV_DATASET_SUPERVISOR,
        AQMAV_DATASET_STACKSFREE,
        AQMAV_DATASET_GIMBAL,
        AQMAV_DATASET_ESC_TELEMETRY,
        AQMAV_DATASET_ENUM_END
    };

#ifdef _MSC_VER
    #pragma pack(push,1)
    typedef struct {
        unsigned __int64 state :    3;
        unsigned __int64 vin :	    12;	// x 100
        unsigned __int64 amps :	    14;	// x 100
        unsigned __int64 rpm :	    15;
        unsigned __int64 duty :	    8;	// x (255/100)
        unsigned __int64 temp :     9;  // (Deg C + 32) * 4
        unsigned __int64 errCode :  3;
    } esc32CanStatus_t;
    #pragma pack(pop)
#else
    struct esc32CanStatus {
        unsigned int state :    3;
        unsigned int vin :	    12;	// x 100
        unsigned int amps :	    14;	// x 100
        unsigned int rpm :	    15;
        unsigned int duty :	    8;	// x (255/100)
        unsigned int temp :     9;  // (Deg C + 32) * 4
        unsigned int errCode :  3;
    } __attribute__((__packed__));
    typedef esc32CanStatus esc32CanStatus_t;
#endif

}

#endif // AUTOQUADMAV_H
