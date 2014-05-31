#ifndef QGC_CONFIGURATION_H
#define QGC_CONFIGURATION_H

#include <QString>

/** @brief Polling interval in ms */
#define SERIAL_POLL_INTERVAL 9

/** @brief Heartbeat emission rate, in Hertz (times per second) */
#define MAVLINK_HEARTBEAT_DEFAULT_RATE 1

#define QGC_APPLICATION_NAME "QGroundControl"
#define QGC_APPLICATION_VERSION "v. 1.0.2 (beta)"

namespace QGC

{
const QString APPNAME = "QGROUNDCONTROL";
const QString COMPANYNAME = "QGROUNDCONTROL";
const int APPLICATIONVERSION = 102; // 1.0.1
}

namespace QGCAUTOQUAD {
    const QString APP_NAME = "QGroundControl for AutoQuad";
    const QString APP_ORG = "AutoQuad";
    const QString APP_VERSION_TXT = "1.4.0";
    const float APP_VERSION = 140.03f; // 1.4.0.03
}

#endif // QGC_CONFIGURATION_H
