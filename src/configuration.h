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
    const QString APP_VERSION_TXT = "1.6 BETA 1";
    const float APP_VERSION = 160.00f; // 1.6.0.00
}

#endif // QGC_CONFIGURATION_H
