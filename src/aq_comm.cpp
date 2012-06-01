#include "aq_comm.h"
#include <cmath>
#include <QDebug>
#include <QMap>


AQLogParser::AQLogParser()
{
    xValues.clear();
    yValues.clear();
    if ( !xValues.contains("xvalue"))
        xValues.insert("xvalue", new QVector<double>());
}

AQLogParser::~AQLogParser()
{
}

void AQLogParser::ResetLog()
{
    xValues.clear();
    yValues.clear();
}

int AQLogParser::ParseLogHeader(QString fileName)
{
FILE *lf;
fileName = QDir::toNativeSeparators(fileName);
FileName = fileName;
#ifdef Q_OS_WIN
    lf = fopen(fileName.toLocal8Bit().constData(),"rb");
#else
    lf = fopen(fileName.toLocal8Bit().constData(),"rb");
#endif
    oldLog = false;
    xAxisCount = 0;
    if (lf) {
        LogChannelsStruct.clear();
        if (loggerReadHeader(lf) == 0 ) {
            return 0;
            fclose(lf);
        }
    }
    fclose(lf);
    return -1;
}

int AQLogParser::loggerReadHeader(FILE *fp)
{
    char ckA, ckB = 0;
    char ckA_Calculate, ckB_Calculate = 0;
    int i;
    int c = 0;
    uint8_t count_channels = 0;
    loggerFieldsAndActive_t fieldsInfo;
    LogChannelsStruct.clear();
    loggerTop:

    if (c != EOF) {
        if ((c = fgetc(fp)) != 'A')
            goto loggerTop;
        if ((c = fgetc(fp)) != 'q')
            goto loggerTop;
        if ((c = fgetc(fp)) != 'H') {
            if (c != 'L')
                goto loggerTop;
            else {
                oldLog = true;
                createHeaderL();
                return 0;
            }

        }


        count_channels = fgetc(fp);
        logHeader = (loggerFields_t*) calloc(count_channels , sizeof(loggerFields_t));
        fread(logHeader, sizeof(loggerFields_t), count_channels, fp);
        ckA_Calculate = 0;
        ckB_Calculate = 0;
        LoggerFrameSize = 0;
        ckA = fgetc(fp);
        ckB = fgetc(fp);
        ckA_Calculate += count_channels;
        ckB_Calculate += ckA_Calculate;
        for (i = 0; i<count_channels; i++) {
            ckA_Calculate += logHeader[i].fieldId;
            ckB_Calculate += ckA_Calculate;
            ckA_Calculate += logHeader[i].fieldType;
            ckB_Calculate += ckA_Calculate;
            fieldsInfo.fieldActive = false;
            fieldsInfo.fieldId = logHeader[i].fieldId;
            fieldsInfo.fieldType = logHeader[i].fieldType;
            fieldsInfo.fieldName = GetChannelsName(logHeader[i].fieldId);
            LogChannelsStruct.append(qMakePair(GetChannelsName(fieldsInfo.fieldId), fieldsInfo));
            //qDebug() << "FieldID=" << logHeader[i].fieldId << " Name" << GetChannelsName(logHeader[i].fieldId) << " Type" << logHeader[i].fieldType << " list" << LogChannelsStruct.count();

            switch (logHeader[i].fieldType) {
                case LOG_TYPE_DOUBLE:
                    LoggerFrameSize += 8;
                    break;
                case LOG_TYPE_FLOAT:
                case LOG_TYPE_U32:
                case LOG_TYPE_S32:
                    LoggerFrameSize += 4;
                    break;
                case LOG_TYPE_U16:
                case LOG_TYPE_S16:
                    LoggerFrameSize += 2;
                    break;
                case LOG_TYPE_U8:
                case LOG_TYPE_S8:
                    LoggerFrameSize += 1;
                    break;
            }
        }

        if (ckA_Calculate == ckA && ckB_Calculate == ckB) {
            free(logHeader);
            return 0;
        }
        else {
            LogChannelsStruct.clear();
            free(logHeader);
            qDebug() << "logger: checksum error\n";
            goto loggerTop;
        }
    }

    if (logHeader)
        free(logHeader);
    return -1;
}

QString AQLogParser::GetChannelsName(uint8_t fieldId)
{
    switch (fieldId) {
        case LOG_LASTUPDATE:
            return "LOG_LASTUPDATE";
        break;
        case LOG_VOLTAGE0:
            return "LOG_VOLTAGE0";
        break;
        case LOG_VOLTAGE1:
            return "LOG_VOLTAGE1";
        break;
        case LOG_VOLTAGE2:
            return "LOG_VOLTAGE2";
        break;
        case LOG_VOLTAGE3:
            return "LOG_VOLTAGE3";
        break;
        case LOG_VOLTAGE4:
            return "LOG_VOLTAGE4";
        break;
        case LOG_VOLTAGE5:
            return "LOG_VOLTAGE5";
        break;
        case LOG_VOLTAGE6:
            return "LOG_VOLTAGE6";
        break;
        case LOG_VOLTAGE7:
            return "LOG_VOLTAGE7";
        break;
        case LOG_VOLTAGE8:
            return "LOG_VOLTAGE8";
        break;
        case LOG_VOLTAGE9:
            return "LOG_VOLTAGE9";
        break;
        case LOG_VOLTAGE10:
            return "LOG_VOLTAGE10";
        break;
        case LOG_VOLTAGE11:
            return "LOG_VOLTAGE11";
        break;
        case LOG_VOLTAGE12:
            return "LOG_VOLTAGE12";
        break;
        case LOG_VOLTAGE13:
            return "LOG_VOLTAGE13";
        break;
        case LOG_VOLTAGE14:
            return "LOG_VOLTAGE14";
        break;
        case LOG_IMU_RATEX:
            return "LOG_IMU_RATEX";
        break;
        case LOG_IMU_RATEY:
            return "LOG_IMU_RATEY";
        break;
        case LOG_IMU_RATEZ:
            return "LOG_IMU_RATEZ";
        break;
        case LOG_IMU_ACCX:
            return "LOG_IMU_ACCX";
        break;
        case LOG_IMU_ACCY:
            return "LOG_IMU_ACCY";
        break;
        case LOG_IMU_ACCZ:
            return "LOG_IMU_ACCZ";
        break;
        case LOG_IMU_MAGX:
            return "LOG_IMU_MAGX";
        break;
        case LOG_IMU_MAGY:
            return "LOG_IMU_MAGY";
        break;
        case LOG_IMU_MAGZ:
            return "LOG_IMU_MAGZ";
        break;
        case LOG_GPS_PDOP:
            return "LOG_GPS_PDOP";
        break;
        case LOG_GPS_HDOP:
            return "LOG_GPS_HDOP";
        break;
        case LOG_GPS_VDOP:
            return "LOG_GPS_VDOP";
        break;
        case LOG_GPS_TDOP:
            return "LOG_GPS_TDOP";
        break;
        case LOG_GPS_NDOP:
            return "LOG_GPS_NDOP";
        break;
        case LOG_GPS_EDOP:
            return "LOG_GPS_EDOP";
        break;
        case LOG_GPS_ITOW:
            return "LOG_GPS_ITOW";
        break;
        case LOG_GPS_POS_UPDATE:
            return "LOG_GPS_POS_UPDATE";
        break;
        case LOG_GPS_LAT:
            return "LOG_GPS_LAT";
        break;
        case LOG_GPS_LON:
            return "LOG_GPS_LON";
        break;
        case LOG_GPS_HEIGHT:
            return "LOG_GPS_HEIGHT";
        break;
        case LOG_GPS_HACC:
            return "LOG_GPS_HACC";
        break;
        case LOG_GPS_VACC:
            return "LOG_GPS_VACC";
        break;
        case LOG_GPS_VEL_UPDATE:
            return "LOG_GPS_VEL_UPDATE";
        break;
        case LOG_GPS_VELN:
            return "LOG_GPS_VELN";
        break;
        case LOG_GPS_VELE:
            return "LOG_GPS_VELE";
        break;
        case LOG_GPS_VELD:
            return "LOG_GPS_VELD";
        break;
        case LOG_GPS_SACC:
            return "LOG_GPS_SACC";
        break;

        case LOG_ADC_PRESSURE1:
                return "LOG_ADC_PRESSURE1";
            break;
        case LOG_ADC_PRESSURE2:
                return "LOG_ADC_PRESSURE2";
            break;
        case LOG_ADC_TEMP0:
                return "LOG_ADC_TEMP0";
            break;
        case LOG_ADC_TEMP1:
                return "LOG_ADC_TEMP1";
            break;
        case LOG_ADC_TEMP2:
                return "LOG_ADC_TEMP2";
            break;
        case LOG_ADC_VIN:
                return "LOG_ADC_VIN";
            break;
        case LOG_ADC_MAG_SIGN:
                return "LOG_ADC_MAG_SIGN";
            break;
        case LOG_UKF_Q1:
                return "LOG_UKF_Q1";
            break;
        case LOG_UKF_Q2:
                return "LOG_UKF_Q2";
            break;
        case LOG_UKF_Q3:
                return "LOG_UKF_Q3";
            break;
        case LOG_UKF_Q4:
                return "LOG_UKF_Q4";
            break;
        case LOG_UKF_POSN:
                return "LOG_UKF_POSN";
            break;
        case LOG_UKF_POSE:
                return "LOG_UKF_POSE";
            break;
        case LOG_UKF_POSD:
                return "LOG_UKF_POSD";
            break;
        case LOG_UKF_PRES_ALT:
                return "LOG_UKF_PRES_ALT";
            break;
        case LOG_UKF_ALT:
                return "LOG_UKF_ALT";
            break;
        case LOG_UKF_VELN:
                return "LOG_UKF_VELN";
            break;
        case LOG_UKF_VELE:
                return "LOG_UKF_VELE";
            break;
        case LOG_UKF_VELD:
                return "LOG_UKF_VELD";
            break;
        case LOG_MOT_MOTOR0:
                return "LOG_MOT_MOTOR0";
            break;
        case LOG_MOT_MOTOR1:
                return "LOG_MOT_MOTOR1";
            break;
        case LOG_MOT_MOTOR2:
                return "LOG_MOT_MOTOR2";
            break;
        case LOG_MOT_MOTOR3:
                return "LOG_MOT_MOTOR3";
            break;
        case LOG_MOT_MOTOR4:
                return "LOG_MOT_MOTOR4";
            break;
        case LOG_MOT_MOTOR5:
                return "LOG_MOT_MOTOR5";
            break;
        case LOG_MOT_MOTOR6:
                return "LOG_MOT_MOTOR6";
            break;
        case LOG_MOT_MOTOR7:
                return "LOG_MOT_MOTOR7";
            break;
        case LOG_MOT_MOTOR8:
                return "LOG_MOT_MOTOR8";
            break;
        case LOG_MOT_MOTOR9:
                return "LOG_MOT_MOTOR9";
            break;
        case LOG_MOT_MOTOR10:
                return "LOG_MOT_MOTOR10";
            break;
        case LOG_MOT_MOTOR11:
                return "LOG_MOT_MOTOR11";
            break;
        case LOG_MOT_MOTOR12:
                return "LOG_MOT_MOTOR12";
            break;
        case LOG_MOT_MOTOR13:
                return "LOG_MOT_MOTOR13";
            break;

        case LOG_MOT_THROTTLE:
                return "LOG_MOT_THROTTLE";
            break;
        case LOG_MOT_PITCH:
                return "LOG_MOT_PITCH";
            break;
        case LOG_MOT_ROLL:
                return "LOG_MOT_ROLL";
            break;
        case LOG_MOT_YAW:
                return "LOG_MOT_YAW";
            break;
        case LOG_RADIO_QUALITY:
                return "LOG_RADIO_QUALITY";
            break;
        case LOG_RADIO_CHANNEL0:
                return "LOG_RADIO_CHANNEL0";
            break;
        case LOG_RADIO_CHANNEL1:
                return "LOG_RADIO_CHANNEL1";
            break;
        case LOG_RADIO_CHANNEL2:
                return "LOG_RADIO_CHANNEL2";
            break;
        case LOG_RADIO_CHANNEL3:
                return "LOG_RADIO_CHANNEL3";
            break;
        case LOG_RADIO_CHANNEL4:
                return "LOG_RADIO_CHANNEL4";
            break;
        case LOG_RADIO_CHANNEL5:
                return "LOG_RADIO_CHANNEL5";
            break;
        case LOG_RADIO_CHANNEL6:
                return "LOG_RADIO_CHANNEL6";
            break;
        case LOG_RADIO_CHANNEL7:
                return "LOG_RADIO_CHANNEL7";
            break;
        case LOG_RADIO_CHANNEL8:
                return "LOG_RADIO_CHANNEL8";
            break;
        case LOG_RADIO_CHANNEL9:
                return "LOG_RADIO_CHANNEL9";
            break;
        case LOG_RADIO_CHANNEL10:
                return "LOG_RADIO_CHANNEL10";
            break;
        case LOG_RADIO_CHANNEL11:
                return "LOG_RADIO_CHANNEL11";
            break;
        case LOG_RADIO_CHANNEL12:
                return "LOG_RADIO_CHANNEL12";
            break;
        case LOG_RADIO_CHANNEL13:
                return "LOG_RADIO_CHANNEL13";
            break;
        case LOG_RADIO_CHANNEL14:
                return "LOG_RADIO_CHANNEL14";
            break;
        case LOG_RADIO_CHANNEL15:
                return "LOG_RADIO_CHANNEL15";
            break;
        case LOG_RADIO_CHANNEL16:
                return "LOG_RADIO_CHANNEL16";
            break;
        case LOG_RADIO_CHANNEL17:
                return "LOG_RADIO_CHANNEL17";
            break;
        case LOG_NUM_IDS:
                return "LOG_NUM_IDS";
            break;
        default:
            return "Not connected" + QString::number(fieldId);
        break;

    }

}

void AQLogParser::GenerateChannelsCurve(bool isOld) {

    if ( !xValues.contains("XVALUES") )
        xValues.insert("XVALUES", new QVector<double>());

    if ( !isOld ) {
        for ( int i=0; i<LogChannelsStruct.count(); i++ ) {
            QPair<QString,loggerFieldsAndActive_t> val_pair = LogChannelsStruct.at(i);
            loggerFieldsAndActive_t val  = val_pair.second;
            if ( val.fieldActive == 1 ) {

                switch (val.fieldId) {
                    case LOG_LASTUPDATE:
                        yValues.insert("LOG_LASTUPDATE", new QVector<double>());
                    break;
                    case LOG_VOLTAGE0:
                        yValues.insert("LOG_VOLTAGE0", new QVector<double>());
                    break;
                    case LOG_VOLTAGE1:
                        yValues.insert("LOG_VOLTAGE1", new QVector<double>());
                    break;
                    case LOG_VOLTAGE2:
                        yValues.insert("LOG_VOLTAGE2", new QVector<double>());
                    break;
                    case LOG_VOLTAGE3:
                        yValues.insert("LOG_VOLTAGE3", new QVector<double>());
                    break;
                    case LOG_VOLTAGE4:
                        yValues.insert("LOG_VOLTAGE4", new QVector<double>());
                    break;
                    case LOG_VOLTAGE5:
                        yValues.insert("LOG_VOLTAGE5", new QVector<double>());
                    break;
                    case LOG_VOLTAGE6:
                        yValues.insert("LOG_VOLTAGE6", new QVector<double>());
                    break;
                    case LOG_VOLTAGE7:
                        yValues.insert("LOG_VOLTAGE7", new QVector<double>());
                    break;
                    case LOG_VOLTAGE8:
                        yValues.insert("LOG_VOLTAGE8", new QVector<double>());
                    break;
                    case LOG_VOLTAGE9:
                        yValues.insert("LOG_VOLTAGE9", new QVector<double>());
                    break;
                    case LOG_VOLTAGE10:
                        yValues.insert("LOG_VOLTAGE10", new QVector<double>());
                    break;
                    case LOG_VOLTAGE11:
                        yValues.insert("LOG_VOLTAGE11", new QVector<double>());
                    break;
                    case LOG_VOLTAGE12:
                        yValues.insert("LOG_VOLTAGE12", new QVector<double>());
                    break;
                    case LOG_VOLTAGE13:
                        yValues.insert("LOG_VOLTAGE13", new QVector<double>());
                    break;
                    case LOG_VOLTAGE14:
                        yValues.insert("LOG_VOLTAGE14", new QVector<double>());
                    break;
                    case LOG_IMU_RATEX:
                        yValues.insert("LOG_IMU_RATEX", new QVector<double>());
                    break;
                    case LOG_IMU_RATEY:
                        yValues.insert("LOG_IMU_RATEY", new QVector<double>());
                    break;
                    case LOG_IMU_RATEZ:
                        yValues.insert("LOG_IMU_RATEZ", new QVector<double>());
                    break;
                    case LOG_IMU_ACCX:
                        yValues.insert("LOG_IMU_ACCX", new QVector<double>());
                    break;
                    case LOG_IMU_ACCY:
                        yValues.insert("LOG_IMU_ACCY", new QVector<double>());
                    break;
                    case LOG_IMU_ACCZ:
                        yValues.insert("LOG_IMU_ACCZ", new QVector<double>());
                    break;
                    case LOG_IMU_MAGX:
                        yValues.insert("LOG_IMU_MAGX", new QVector<double>());
                    break;
                    case LOG_IMU_MAGY:
                        yValues.insert("LOG_IMU_MAGY", new QVector<double>());
                    break;
                    case LOG_IMU_MAGZ:
                        yValues.insert("LOG_IMU_MAGZ", new QVector<double>());
                    break;
                    case LOG_GPS_PDOP:
                        yValues.insert("LOG_GPS_PDOP", new QVector<double>());
                    break;
                    case LOG_GPS_HDOP:
                        yValues.insert("LOG_GPS_HDOP", new QVector<double>());
                    break;
                    case LOG_GPS_VDOP:
                        yValues.insert("LOG_GPS_VDOP", new QVector<double>());
                    break;
                    case LOG_GPS_TDOP:
                        yValues.insert("LOG_GPS_TDOP", new QVector<double>());
                    break;
                    case LOG_GPS_NDOP:
                        yValues.insert("LOG_GPS_NDOP", new QVector<double>());
                    break;
                    case LOG_GPS_EDOP:
                        yValues.insert("LOG_GPS_EDOP", new QVector<double>());
                    break;
                    case LOG_GPS_ITOW:
                        yValues.insert("LOG_GPS_ITOW", new QVector<double>());
                    break;
                    case LOG_GPS_POS_UPDATE:
                        yValues.insert("LOG_GPS_POS_UPDATE", new QVector<double>());
                    break;
                    case LOG_GPS_LAT:
                        yValues.insert("LOG_GPS_LAT", new QVector<double>());
                    break;
                    case LOG_GPS_LON:
                        yValues.insert("LOG_GPS_LON", new QVector<double>());
                    break;
                    case LOG_GPS_HEIGHT:
                        yValues.insert("LOG_GPS_HEIGHT", new QVector<double>());
                    break;
                    case LOG_GPS_HACC:
                        yValues.insert("LOG_GPS_HACC", new QVector<double>());
                    break;
                    case LOG_GPS_VACC:
                        yValues.insert("LOG_GPS_VACC", new QVector<double>());
                    break;
                    case LOG_GPS_VEL_UPDATE:
                        yValues.insert("LOG_GPS_VEL_UPDATE", new QVector<double>());
                    break;
                    case LOG_GPS_VELN:
                        yValues.insert("LOG_GPS_VELN", new QVector<double>());
                    break;
                    case LOG_GPS_VELE:
                        yValues.insert("LOG_GPS_VELE", new QVector<double>());
                    break;
                    case LOG_GPS_VELD:
                        yValues.insert("LOG_GPS_VELD", new QVector<double>());
                    break;
                    case LOG_GPS_SACC:
                        yValues.insert("LOG_GPS_SACC", new QVector<double>());
                    break;

                    case LOG_ADC_PRESSURE1:
                        yValues.insert("LOG_ADC_PRESSURE1", new QVector<double>());
                        break;
                    case LOG_ADC_PRESSURE2:
                        yValues.insert("LOG_ADC_PRESSURE2", new QVector<double>());
                        break;
                    case LOG_ADC_TEMP0:
                        yValues.insert("LOG_ADC_TEMP0", new QVector<double>());
                        break;
                    case LOG_ADC_TEMP1:
                        yValues.insert("LOG_ADC_TEMP1", new QVector<double>());
                        break;
                    case LOG_ADC_TEMP2:
                        yValues.insert("LOG_ADC_TEMP2", new QVector<double>());
                        break;


                    case LOG_ADC_VIN:
                        yValues.insert("LOG_ADC_VIN", new QVector<double>());
                        break;
                    case LOG_ADC_MAG_SIGN:
                        yValues.insert("LOG_ADC_MAG_SIGN", new QVector<double>());
                        break;
                    case LOG_UKF_Q1:
                        yValues.insert("LOG_UKF_Q1", new QVector<double>());
                        break;
                    case LOG_UKF_Q2:
                        yValues.insert("LOG_UKF_Q2", new QVector<double>());
                        break;
                    case LOG_UKF_Q3:
                        yValues.insert("LOG_UKF_Q3", new QVector<double>());
                        break;
                    case LOG_UKF_Q4:
                        yValues.insert("LOG_UKF_Q4", new QVector<double>());
                        break;
                    case LOG_UKF_POSN:
                        yValues.insert("LOG_UKF_POSN", new QVector<double>());
                        break;
                    case LOG_UKF_POSE:
                        yValues.insert("LOG_UKF_POSE", new QVector<double>());
                        break;
                    case LOG_UKF_POSD:
                        yValues.insert("LOG_UKF_POSD", new QVector<double>());
                        break;
                    case LOG_UKF_PRES_ALT:
                        yValues.insert("LOG_UKF_PRES_ALT", new QVector<double>());
                        break;
                    case LOG_UKF_ALT:
                        yValues.insert("LOG_UKF_ALT", new QVector<double>());
                        break;
                    case LOG_UKF_VELN:
                        yValues.insert("LOG_UKF_VELN", new QVector<double>());
                        break;
                    case LOG_UKF_VELE:
                        yValues.insert("LOG_UKF_VELE", new QVector<double>());
                        break;
                    case LOG_UKF_VELD:
                        yValues.insert("LOG_UKF_VELD", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR0:
                        yValues.insert("LOG_MOT_MOTOR0", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR1:
                        yValues.insert("LOG_MOT_MOTOR1", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR2:
                        yValues.insert("LOG_MOT_MOTOR2", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR3:
                        yValues.insert("LOG_MOT_MOTOR3", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR4:
                        yValues.insert("LOG_MOT_MOTOR4", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR5:
                        yValues.insert("LOG_MOT_MOTOR5", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR6:
                        yValues.insert("LOG_MOT_MOTOR6", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR7:
                        yValues.insert("LOG_MOT_MOTOR7", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR8:
                        yValues.insert("LOG_MOT_MOTOR8", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR9:
                        yValues.insert("LOG_MOT_MOTOR9", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR10:
                        yValues.insert("LOG_MOT_MOTOR10", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR11:
                        yValues.insert("LOG_MOT_MOTOR11", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR12:
                        yValues.insert("LOG_MOT_MOTOR12", new QVector<double>());
                        break;
                    case LOG_MOT_MOTOR13:
                        yValues.insert("LOG_MOT_MOTOR13", new QVector<double>());
                        break;
                    case LOG_MOT_THROTTLE:
                        yValues.insert("LOG_MOT_THROTTLE", new QVector<double>());
                        break;
                    case LOG_MOT_PITCH:
                        yValues.insert("LOG_MOT_PITCH", new QVector<double>());
                        break;
                    case LOG_MOT_ROLL:
                        yValues.insert("LOG_MOT_ROLL", new QVector<double>());
                        break;
                    case LOG_MOT_YAW:
                        yValues.insert("LOG_MOT_YAW", new QVector<double>());
                        break;
                    case LOG_RADIO_QUALITY:
                        yValues.insert("LOG_RADIO_QUALITY", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL0:
                        yValues.insert("LOG_RADIO_CHANNEL0", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL1:
                        yValues.insert("LOG_RADIO_CHANNEL1", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL2:
                        yValues.insert("LOG_RADIO_CHANNEL2", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL3:
                        yValues.insert("LOG_RADIO_CHANNEL3", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL4:
                        yValues.insert("LOG_RADIO_CHANNEL4", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL5:
                        yValues.insert("LOG_RADIO_CHANNEL5", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL6:
                        yValues.insert("LOG_RADIO_CHANNEL6", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL7:
                        yValues.insert("LOG_RADIO_CHANNEL7", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL8:
                        yValues.insert("LOG_RADIO_CHANNEL8", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL9:
                        yValues.insert("LOG_RADIO_CHANNEL9", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL10:
                        yValues.insert("LOG_RADIO_CHANNEL10", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL11:
                        yValues.insert("LOG_RADIO_CHANNEL11", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL12:
                        yValues.insert("LOG_RADIO_CHANNEL12", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL13:
                        yValues.insert("LOG_RADIO_CHANNEL13", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL14:
                        yValues.insert("LOG_RADIO_CHANNEL14", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL15:
                        yValues.insert("LOG_RADIO_CHANNEL15", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL16:
                        yValues.insert("LOG_RADIO_CHANNEL16", new QVector<double>());
                        break;
                    case LOG_RADIO_CHANNEL17:
                        yValues.insert("LOG_RADIO_CHANNEL17", new QVector<double>());
                        break;
                    case LOG_NUM_IDS:
                        yValues.insert("LOG_NUM_IDS", new QVector<double>());
                        break;
                    default:
                    break;

                }
        }
    }
    }
    else {
        for ( int i=0; i<LogChannelsStruct.count(); i++ ) {
            QPair<QString,loggerFieldsAndActive_t> val_pair = LogChannelsStruct.at(i);
            loggerFieldsAndActive_t val  = val_pair.second;
            if ( val.fieldActive == 1 ) {

                switch (val.fieldId) {
                    case MICROS:
                        yValues.insert("LOG_LASTUPDATE", new QVector<double>());
                    break;
                    case VOLTAGE1:
                        yValues.insert("VOLTAGE1", new QVector<double>());
                    break;
                    case VOLTAGE2:
                        yValues.insert("VOLTAGE2", new QVector<double>());
                    break;
                    case VOLTAGE3:
                        yValues.insert("VOLTAGE3", new QVector<double>());
                    break;
                    case VOLTAGE4:
                        yValues.insert("VOLTAGE4", new QVector<double>());
                    break;
                    case VOLTAGE5:
                        yValues.insert("VOLTAGE5", new QVector<double>());
                    break;
                    case VOLTAGE6:
                        yValues.insert("VOLTAGE6", new QVector<double>());
                    break;
                    case VOLTAGE7:
                        yValues.insert("VOLTAGE7", new QVector<double>());
                    break;
                    case VOLTAGE8:
                        yValues.insert("VOLTAGE8", new QVector<double>());
                    break;
                    case VOLTAGE9:
                        yValues.insert("VOLTAGE9", new QVector<double>());
                    break;
                    case VOLTAGE10:
                        yValues.insert("VOLTAGE10", new QVector<double>());
                    break;
                    case VOLTAGE11:
                        yValues.insert("VOLTAGE11", new QVector<double>());
                    break;
                    case VOLTAGE12:
                        yValues.insert("VOLTAGE12", new QVector<double>());
                    break;
                    case VOLTAGE13:
                        yValues.insert("VOLTAGE13", new QVector<double>());
                    break;
                    case VOLTAGE14:
                        yValues.insert("VOLTAGE14", new QVector<double>());
                    break;
                    case VOLTAGE15:
                        yValues.insert("VOLTAGE14", new QVector<double>());
                    break;
                    case RATEX:
                        yValues.insert("RATEX", new QVector<double>());
                    break;
                    case RATEY:
                        yValues.insert("RATEY", new QVector<double>());
                    break;
                    case RATEZ:
                        yValues.insert("RATEZ", new QVector<double>());
                    break;
                    case ACCX:
                        yValues.insert("ACCX", new QVector<double>());
                    break;
                    case ACCY:
                        yValues.insert("ACCY", new QVector<double>());
                    break;
                    case ACCZ:
                        yValues.insert("ACCZ", new QVector<double>());
                    break;
                    case MAGX:
                        yValues.insert("MAGX", new QVector<double>());
                    break;
                    case MAGY:
                        yValues.insert("MAGY", new QVector<double>());
                    break;
                    case MAGZ:
                        yValues.insert("MAGZ", new QVector<double>());
                    break;
                    case PRESSURE1:
                        yValues.insert("PRESSURE1", new QVector<double>());
                    break;
                    case PRESSURE2:
                        yValues.insert("PRESSURE2", new QVector<double>());
                    break;
                    case TEMP1:
                        yValues.insert("TEMP1", new QVector<double>());
                    break;
                    case TEMP2:
                        yValues.insert("TEMP2", new QVector<double>());
                    break;
                    case TEMP3:
                        yValues.insert("TEMP3", new QVector<double>());
                    break;
                    case TEMP4:
                        yValues.insert("TEMP4", new QVector<double>());
                    break;
                    case AUX_RATEX:
                        yValues.insert("AUX_RATEX", new QVector<double>());
                    break;
                    case AUX_RATEY:
                        yValues.insert("AUX_RATEY", new QVector<double>());
                    break;
                    case AUX_RATEZ:
                        yValues.insert("AUX_RATEZ", new QVector<double>());
                    break;
                    case AUX_ACCX:
                        yValues.insert("AUX_ACCX", new QVector<double>());
                    break;
                    case AUX_ACCY:
                        yValues.insert("AUX_ACCY", new QVector<double>());
                    break;
                    case AUX_ACCZ:
                        yValues.insert("AUX_ACCZ", new QVector<double>());
                    break;
                    case AUX_MAGX:
                        yValues.insert("AUX_MAGX", new QVector<double>());
                    break;
                    case AUX_MAGY:
                        yValues.insert("AUX_MAGY", new QVector<double>());
                    break;
                    case AUX_MAGZ:
                        yValues.insert("AUX_MAGZ", new QVector<double>());
                    break;
                    case VIN:
                        yValues.insert("VIN", new QVector<double>());
                    break;
                    case GPS_POS_UPDATE:
                        yValues.insert("GPS_POS_UPDATE", new QVector<double>());
                    break;
                    case LAT:
                        yValues.insert("LAT", new QVector<double>());
                    break;

                    case LON:
                        yValues.insert("LON", new QVector<double>());
                        break;
                    case GPS_ALT:
                        yValues.insert("GPS_ALT", new QVector<double>());
                        break;
                    case GPS_POS_ACC:
                        yValues.insert("GPS_POS_ACC", new QVector<double>());
                        break;
                    case GPS_VEL_UPDATE:
                        yValues.insert("GPS_VEL_UPDATE", new QVector<double>());
                        break;
                    case GPS_VELN:
                        yValues.insert("GPS_VELN", new QVector<double>());
                        break;


                    case GPS_VELE:
                        yValues.insert("GPS_VELE", new QVector<double>());
                        break;
                    case GPS_VELD:
                        yValues.insert("GPS_VELD", new QVector<double>());
                        break;
                    case GPS_VEL_ACC:
                        yValues.insert("GPS_VEL_ACC", new QVector<double>());
                        break;
                    case POSN:
                        yValues.insert("POSN", new QVector<double>());
                        break;
                    case POSE:
                        yValues.insert("POSE", new QVector<double>());
                        break;
                    case POSD:
                        yValues.insert("POSD", new QVector<double>());
                        break;
                    case VELN:
                        yValues.insert("VELN", new QVector<double>());
                        break;
                    case VELE:
                        yValues.insert("VELE", new QVector<double>());
                        break;
                    case VELD:
                        yValues.insert("VELD", new QVector<double>());
                        break;
                    case QUAT0:
                        yValues.insert("QUAT0", new QVector<double>());
                        break;
                    case QUAT1:
                        yValues.insert("QUAT1", new QVector<double>());
                        break;
                    case QUAT2:
                        yValues.insert("QUAT2", new QVector<double>());
                        break;
                    case QUAT3:
                        yValues.insert("QUAT3", new QVector<double>());
                        break;
                    case MOTOR1:
                        yValues.insert("MOTOR1", new QVector<double>());
                        break;
                    case MOTOR2:
                        yValues.insert("MOTOR2", new QVector<double>());
                        break;
                    case MOTOR3:
                        yValues.insert("MOTOR3", new QVector<double>());
                        break;
                    case MOTOR4:
                        yValues.insert("MOTOR4", new QVector<double>());
                        break;
                    case MOTOR5:
                        yValues.insert("MOTOR5", new QVector<double>());
                        break;
                    case MOTOR6:
                        yValues.insert("MOTOR6", new QVector<double>());
                        break;
                    case MOTOR7:
                        yValues.insert("MOTOR7", new QVector<double>());
                        break;
                    case MOTOR8:
                        yValues.insert("MOTOR8", new QVector<double>());
                        break;
                    case MOTOR9:
                        yValues.insert("MOTOR9", new QVector<double>());
                        break;
                    case MOTOR10:
                        yValues.insert("MOTOR10", new QVector<double>());
                        break;
                    case MOTOR11:
                        yValues.insert("MOTOR11", new QVector<double>());
                        break;
                    case MOTOR12:
                        yValues.insert("MOTOR12", new QVector<double>());
                        break;
                    case MOTOR13:
                        yValues.insert("MOTOR13", new QVector<double>());
                        break;
                    case MOTOR14:
                        yValues.insert("MOTOR14", new QVector<double>());
                        break;
                    case THROTTLE:
                        yValues.insert("THROTTLE", new QVector<double>());
                        break;
                    case EXTRA1:
                        yValues.insert("EXTRA1", new QVector<double>());
                        break;
                    case EXTRA2:
                        yValues.insert("EXTRA2", new QVector<double>());
                        break;
                    case EXTRA3:
                        yValues.insert("EXTRA3", new QVector<double>());
                        break;
                    case EXTRA4:
                        yValues.insert("EXTRA4", new QVector<double>());
                        break;
                    case NUM_FIELDS:
                        yValues.insert("NUM_FIELDS", new QVector<double>());
                        break;
                    default:
                    break;

                }
        }
    }
    }

}

void AQLogParser::ShowCurves() {
    int n = 0;
    int count = 0;
    FILE *lf;
    QString fileName = QDir::toNativeSeparators(FileName);
    xValues.clear();
    yValues.clear();
    #ifdef Q_OS_WIN
        lf = fopen(fileName.toLocal8Bit().constData(),"rb");
    #else
        lf = fopen(fileName.toLocal8Bit().constData(),"rb");
    #endif

        if (lf) {

            if (!oldLog) {
                GenerateChannelsCurve(false);
                while (ParseLogM(lf) != EOF)
                    n++;
            } else {
                count = 0;
                GenerateChannelsCurve(true);
                bool appendXvalues = true;
                for ( int i=0; i<LogChannelsStruct.count(); i++ ) {
                    QPair<QString,loggerFieldsAndActive_t> val_pair = LogChannelsStruct.at(i);
                    if ( val_pair.second.fieldActive == 1 ) {
                        rewind(lf);
                        while (loggerReadEntry(lf, &logEntry) != EOF) {
                            if ( appendXvalues) {
                                count++;
                                xValues.value("XVALUES")->append(count);
                            }
                            double va = logDumpGetValue(&logEntry, val_pair.second.fieldId);
                            yValues.value(val_pair.first)->append(va);
                        }
                        appendXvalues = false;
                    }
                }
            }
        }
        fclose(lf);
}

int AQLogParser::ParseLogM(FILE *fp) {
    int c = 0;

    loggerTop:

    if (c != EOF) {
        if ((c = fgetc(fp)) != 'A')
            goto loggerTop;
        if ((c = fgetc(fp)) != 'q')
            goto loggerTop;

        c = fgetc(fp);
        if (c == 'M') {
            if (loggerReadEntryM(fp) == -1)
                goto loggerTop;
            else
                return 1;
        }
        else {
            goto loggerTop;
        }

    }

    return EOF;
}

int AQLogParser::loggerReadEntryM(FILE *fp) {
    char buffer[1024];
    char *buf = buffer;
    unsigned char ckA, ckB;
    int i;
    double tmp_double;
    float tmp_float;
    uint32_t tmp_uint32;
    int32_t tmp_int32;
    uint16_t tmp_uint16;
    int16_t tmp_int16;
    uint8_t tmp_uint8;
    int8_t tmp_int8;

    if (LoggerFrameSize > 0 && fread(buffer, LoggerFrameSize, 1, fp) == 1) {
        // calc checksum
        ckA = ckB = 0;
        for (i = 0; i < LoggerFrameSize; i++) {
            ckA += buf[i];
            ckB += ckA;
        }

        if (fgetc(fp) == ckA && fgetc(fp) == ckB) {
            xValues.value("XVALUES")->append(xAxisCount++);

            for ( int i=0; i<LogChannelsStruct.count(); i++ ) {
                QPair<QString,loggerFieldsAndActive_t> val_pair = LogChannelsStruct.at(i);
                loggerFieldsAndActive_t val  = val_pair.second;
                switch (val.fieldType) {
                    case LOG_TYPE_DOUBLE:
                        if ( val.fieldActive == 1 )
                        {
                            tmp_double = *(double *)buf;
                            yValues.value(val.fieldName)->append(tmp_double);
                        }
                        buf += 8;
                        break;
                    case LOG_TYPE_FLOAT:
                        if ( val.fieldActive == 1 )
                        {
                            tmp_float = *(float *)buf;
                            yValues.value(val.fieldName)->append(tmp_float);
                        }
                        buf += 4;
                        break;

                    case LOG_TYPE_U32:
                        if ( val.fieldActive == 1 )
                        {
                            tmp_uint32 = *(uint32_t *)buf;
                            yValues.value(val.fieldName)->append(tmp_uint32);
                        }
                        buf += 4;
                        break;

                    case LOG_TYPE_S32:
                        if ( val.fieldActive == 1 )
                        {
                            tmp_int32 = *(int32_t *)buf;
                            yValues.value(val.fieldName)->append(tmp_int32);
                        }
                        buf += 4;
                        break;

                    case LOG_TYPE_U16:
                        if ( val.fieldActive == 1 )
                        {
                            tmp_uint16 = *(uint16_t *)buf;
                            yValues.value(val.fieldName)->append(tmp_uint16);
                        }
                        buf += 2;
                        break;
                    case LOG_TYPE_S16:
                        if ( val.fieldActive == 1 )
                        {
                            tmp_int16 = *(int16_t *)buf;
                            yValues.value(val.fieldName)->append(tmp_int16);
                        }
                        buf += 2;
                        break;

                    case LOG_TYPE_U8:
                        if ( val.fieldActive == 1 )
                        {
                            tmp_uint8 = *(uint8_t *)buf;
                            yValues.value(val.fieldName)->append(tmp_uint8);
                        }
                        buf += 1;
                        break;
                        case LOG_TYPE_S8:
                        if ( val.fieldActive == 1 )
                        {
                            tmp_int8 = *(int8_t *)buf;
                            yValues.value(val.fieldName)->append(tmp_int8);
                        }
                        buf += 1;
                        break;
                }
            }
            return 1;
        }

        qDebug() << "logger: checksum error\n";

    }

    return -1;
}

void AQLogParser::createHeaderL() {
    LogChannelsStruct.clear();
    SetChannelsStruct();
}

void AQLogParser::SetChannelsStruct()
{
    loggerFieldsAndActive_t fieldsInfo;

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MICROS;
    fieldsInfo.fieldType = LOG_TYPE_U32;
    fieldsInfo.fieldName = "MICROS";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE1;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE1";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE2;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE2";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE3;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE3";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE4;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE4";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE5;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE5";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE6;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE6";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE7;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE7";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE8;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE8";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE9;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE9";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE10;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE10";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE11;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE11";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE12;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE12";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE13;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE13";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE14;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE14";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VOLTAGE15;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VOLTAGE15";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = RATEX;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "RATEX";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = RATEY;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "RATEY";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = RATEZ;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "RATEZ";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = ACCX;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "ACCX";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = ACCY;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "ACCY";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = ACCZ;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "ACCZ";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MAGX;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "MAGX";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MAGY;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "MAGY";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MAGY;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "MAGY";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = PRESSURE1;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "PRESSURE1";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = PRESSURE2;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "PRESSURE2";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = TEMP1;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "TEMP1";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = TEMP2;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "TEMP2";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = TEMP3;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "TEMP3";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = TEMP4;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "TEMP4";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = AUX_RATEX;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "AUX_RATEX";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = AUX_RATEY;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "AUX_RATEY";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = AUX_RATEZ;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "AUX_RATEZ";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = AUX_ACCX;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "AUX_ACCX";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = AUX_ACCY;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "AUX_ACCY";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = AUX_ACCZ;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "AUX_ACCZ";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));



    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = AUX_MAGX;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "AUX_MAGX";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = AUX_MAGY;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "AUX_MAGY";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = AUX_MAGZ;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "AUX_MAGZ";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = AUX_MAGZ;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "AUX_MAGZ";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VIN;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VIN";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = GPS_POS_UPDATE;
    fieldsInfo.fieldType = LOG_TYPE_U32;
    fieldsInfo.fieldName = "GPS_POS_UPDATE";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = LAT;
    fieldsInfo.fieldType = LOG_TYPE_DOUBLE;
    fieldsInfo.fieldName = "LAT";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = LON;
    fieldsInfo.fieldType = LOG_TYPE_DOUBLE;
    fieldsInfo.fieldName = "LON";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = GPS_ALT;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "GPS_ALT";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = GPS_POS_ACC;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "GPS_POS_ACC";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = GPS_VEL_UPDATE;
    fieldsInfo.fieldType = LOG_TYPE_U32;
    fieldsInfo.fieldName = "GPS_VEL_UPDATE";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));



    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = GPS_VELN;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "GPS_VELN";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = GPS_VELE;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "GPS_VELE";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = GPS_VELD;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "GPS_VELD";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = GPS_VEL_ACC;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "GPS_VEL_ACC";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));



    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = POSN;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "POSN";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = POSE;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "POSE";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = POSD;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "POSD";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VELN;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VELN";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VELE;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VELE";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = VELD;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "VELD";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = QUAT0;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "QUAT0";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = QUAT1;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "QUAT1";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = QUAT2;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "QUAT2";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = QUAT3;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "QUAT3";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));



    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR1;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR1";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR2;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR2";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR3;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR3";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR4;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR4";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR5;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR5";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR6;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR6";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR7;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR7";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR8;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR8";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR9;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR9";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR10;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR10";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR11;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR11";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR12;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR12";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR13;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR13";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = MOTOR14;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "MOTOR14";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = THROTTLE;
    fieldsInfo.fieldType = LOG_TYPE_U16;
    fieldsInfo.fieldName = "THROTTLE";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));


    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = EXTRA1;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "EXTRA1";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = EXTRA2;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "EXTRA2";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = EXTRA3;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "EXTRA3";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));

    fieldsInfo.fieldActive = false;
    fieldsInfo.fieldId = EXTRA4;
    fieldsInfo.fieldType = LOG_TYPE_FLOAT;
    fieldsInfo.fieldName = "EXTRA4";
    LogChannelsStruct.append(qMakePair(fieldsInfo.fieldName, fieldsInfo));
}

bool AQLogParser::getOldLog(){
    return oldLog;
}

int AQLogParser::loggerReadEntry(FILE *fp, loggerRecord_t *r)
{
    char *buf = (char *)r;
    char ckA, ckB;
    uint i;
    int c = 0;

    loggerTop:

    if (c != EOF) {
        if ((c = fgetc(fp)) != 'A')
            goto loggerTop;
        if ((c = fgetc(fp)) != 'q')
            goto loggerTop;
        if ((c = fgetc(fp)) != 'L')
            goto loggerTop;

        if (fread(buf, sizeof(loggerRecord_t), 1, fp) == 1) {
            // calc checksum
            ckA = ckB = 0;
            for (i = 0; i < sizeof(loggerRecord_t) - 2; i++) {
                ckA += buf[i];
                ckB += ckA;
            }

            if (r->ckA == ckA && r->ckB == ckB) {
                return 1;
            }
            else {
                fprintf(stderr, "logger: checksum error\n");
                goto loggerTop;
            }
        }
    }

    return EOF;
}

double AQLogParser::logDumpGetValue(loggerRecord_t *l, int field)
{
    double val;

    switch (field) {
    case MICROS:
        val =  l->lastUpdate;
        break;
    case VOLTAGE1:
        val = l->voltages[0];
        break;
    case VOLTAGE2:
        val = l->voltages[1];
        break;
    case VOLTAGE3:
        val = l->voltages[2];
        break;
    case VOLTAGE4:
        val = l->voltages[3];
        break;
    case VOLTAGE5:
        val = l->voltages[4];
        break;
    case VOLTAGE6:
        val = l->voltages[5];
        break;
    case VOLTAGE7:
        val = l->voltages[6];
        break;
    case VOLTAGE8:
        val = l->voltages[7];
        break;
    case VOLTAGE9:
        val = l->voltages[8];
        break;
    case VOLTAGE10:
        val = l->voltages[9];
        break;
    case VOLTAGE11:
        val = l->voltages[10];
        break;
    case VOLTAGE12:
        val = l->voltages[11];
        break;
    case VOLTAGE13:
        val = l->voltages[12];
        break;
    case VOLTAGE14:
        val = l->voltages[13];
        break;
    case VOLTAGE15:
        val = l->voltages[14];
        break;
    case RATEX:
        val = l->rate[0];
        break;
    case RATEY:
        val = l->rate[1];
        break;
    case RATEZ:
        val = l->rate[2];
        break;
    case ACCX:
        val = l->acc[0];
        break;
    case ACCY:
        val = l->acc[1];
        break;
    case ACCZ:
        val = l->acc[2];
        break;
    case MAGX:
        val = l->mag[0];
        break;
    case MAGY:
        val = l->mag[1];
        break;
    case MAGZ:
        val = l->mag[2];
        break;
    case PRESSURE1:
        val = l->pressure[0];
        break;
    case PRESSURE2:
        val = l->pressure[1];
        break;
    case TEMP1:
        val = l->temp[0];
        break;
    case TEMP2:
        val = l->temp[1];
        break;
    case TEMP3:
        val = l->temp[2];
        break;
    case TEMP4:
        val = l->temp[3];
        break;
    case AUX_RATEX:
        val = l->rateAux[0];
        break;
    case AUX_RATEY:
        val = l->rateAux[1];
        break;
    case AUX_RATEZ:
        val = l->rateAux[2];
        break;
    case AUX_ACCX:
        val = l->accAux[0];
        break;
    case AUX_ACCY:
        val = l->accAux[1];
        break;
    case AUX_ACCZ:
        val = l->accAux[2];
        break;
    case AUX_MAGX:
        val = l->magAux[0];
        break;
    case AUX_MAGY:
        val = l->magAux[1];
        break;
    case AUX_MAGZ:
        val = l->magAux[2];
        break;
    case VIN:
        val = l->vIn;
        break;
    case GPS_POS_UPDATE:
        val = l->gpsPosUpdate;
        break;
    case LAT:
        val = l->lat;
        break;
    case LON:
        val = l->lon;
        break;
    case GPS_ALT:
        val = l->gpsAlt;
        break;
    case GPS_POS_ACC:
        val = l->gpsPosAcc;
        break;
    case GPS_VEL_UPDATE:
        val = l->gpsVelUpdate;
        break;
    case GPS_VELN:
        val = l->gpsVel[0];
        break;
    case GPS_VELE:
        val = l->gpsVel[1];
        break;
    case GPS_VELD:
        val = l->gpsVel[2];
        break;
    case GPS_VEL_ACC:
        val = l->gpsVelAcc;
        break;
    case POSN:
        val = l->pos[0];
        break;
    case POSE:
        val = l->pos[1];
        break;
    case POSD:
        val = l->pos[2];
        break;
    case VELN:
        val = l->vel[0];
        break;
    case VELE:
        val = l->vel[1];
        break;
    case QUAT0:
        val = l->quat[0];
        break;
    case QUAT1:
        val = l->quat[1];
        break;
    case QUAT2:
        val = l->quat[2];
        break;
    case QUAT3:
        val = l->quat[3];
        break;
    case VELD:
        val = l->vel[2];
        break;
    case MOTOR1:
        val = l->motors[0];
        break;
    case MOTOR2:
        val = l->motors[1];
        break;
    case MOTOR3:
        val = l->motors[2];
        break;
    case MOTOR4:
        val = l->motors[3];
        break;
    case MOTOR5:
        val = l->motors[4];
        break;
    case MOTOR6:
        val = l->motors[5];
        break;
    case MOTOR7:
        val = l->motors[6];
        break;
    case MOTOR8:
        val = l->motors[7];
        break;
    case MOTOR9:
        val = l->motors[8];
        break;
    case MOTOR11:
        val = l->motors[9];
        break;
    case MOTOR12:
        val = l->motors[10];
        break;
    case MOTOR13:
        val = l->motors[11];
        break;
    case MOTOR14:
        val = l->motors[12];
        break;
    case THROTTLE:
        val = l->throttle;
        break;
    case EXTRA1:
        val = l->extra[0];
        break;
    case EXTRA2:
        val = l->extra[1];
        break;
    case EXTRA3:
        val = l->extra[2];
        break;
    case EXTRA4:
        val = l->extra[3];
        break;
    }

    return val;
}








AQEsc32::AQEsc32()
{
    esc32BinaryMode = 0;
    esc32DoCommand = 0;
    StepMessageFromEsc32 = 0;
    esc32state = 0;
    seriallinkEsc32 = NULL;
}

AQEsc32::~AQEsc32()
{
    if ( seriallinkEsc32 ) {
        if ( seriallinkEsc32->isConnected() )
            seriallinkEsc32->disconnect();
        seriallinkEsc32 = NULL;
    }

}

void AQEsc32::Connect(QString port)
{
    seriallinkEsc32 = new SerialLink(port,230400,false,false,8,1);
    seriallinkEsc32->setBaudRate(230400);
    seriallinkEsc32->setFlowType(0);
    connect(seriallinkEsc32, SIGNAL(connected()), this, SLOT(connectedEsc32()));
    connect(seriallinkEsc32, SIGNAL(disconnected()), this, SLOT(disconnectedEsc32()));
    connect(seriallinkEsc32, SIGNAL(destroyed()), this, SLOT(destroyedEsc32()));
    connect(seriallinkEsc32, SIGNAL(bytesReceived(LinkInterface*, QByteArray)), this, SLOT(BytesRceivedEsc32(LinkInterface*, QByteArray)));
    seriallinkEsc32->connect();
}

void AQEsc32::Disconnect()
{
    SwitchFromBinaryToAscii();
    seriallinkEsc32->disconnect();
    disconnect(seriallinkEsc32, SIGNAL(connected()), this, SLOT(connectedEsc32()));
    disconnect(seriallinkEsc32, SIGNAL(disconnected()), this, SLOT(disconnectedEsc32()));
    disconnect(seriallinkEsc32, SIGNAL(destroyed()), this, SLOT(destroyedEsc32()));
    disconnect(seriallinkEsc32, SIGNAL(bytesReceived(LinkInterface*, QByteArray)), this, SLOT(BytesRceivedEsc32(LinkInterface*, QByteArray)));
    seriallinkEsc32 = NULL;
}

void AQEsc32::SavePara(QString ParaName, QVariant ParaValue) {
    if ( esc32BinaryMode == 0)
    {
        SwitchFromAsciiToBinary();
    }
    StepMessageFromEsc32 = 4;
    ParaNameLastSend = ParaName;
    ParaLastSend = BINARY_COMMAND_SET;
    int paraToWrite =  getEnumByName(ParaName);
    float valueToWrite = ParaValue.toFloat();
    esc32SendCommand(BINARY_COMMAND_SET,paraToWrite,valueToWrite,2);

}

void AQEsc32::sendCommand(int command, float Value1, float Value2, int num ){
    if ( esc32BinaryMode == 0)
    {
        SwitchFromAsciiToBinary();
    }
    StepMessageFromEsc32 = 4;
    ParaNameLastSend = "";
    ParaLastSend = command;
    LastParaValueSend1 = Value1;
    LastParaValueSend2 = Value2;
    esc32SendCommand(command,Value1,Value2,num);

}

int AQEsc32::GetEsc32State() {
    return esc32state;
}

int AQEsc32::getEnumByName(QString Name)
{
    if ( Name == "CONFIG_VERSION")
    {
        return CONFIG_VERSION;
    }
    if ( Name == "STARTUP_MODE")
    {
        return STARTUP_MODE;
    }
    if ( Name == "BAUD_RATE")
    {
        return BAUD_RATE;
    }
    if ( Name == "PTERM")
    {
        return PTERM;
    }
    if ( Name == "ITERM")
    {
        return ITERM;
    }

    if ( Name == "FF1TERM")
    {
        return FF1TERM;
    }
    if ( Name == "FF2TERM")
    {
        return FF2TERM;
    }
    if ( Name == "SHUNT_RESISTANCE")
    {
        return SHUNT_RESISTANCE;
    }
    if ( Name == "MIN_PERIOD")
    {
        return MIN_PERIOD;
    }
    if ( Name == "MAX_PERIOD")
    {
        return MAX_PERIOD;
    }

    if ( Name == "BLANKING_MICROS")
    {
        return BLANKING_MICROS;
    }
    if ( Name == "ADVANCE")
    {
        return ADVANCE;
    }
    if ( Name == "START_VOLTAGE")
    {
        return START_VOLTAGE;
    }
    if ( Name == "DUTY_INCREASE_FACTOR")
    {
        return DUTY_INCREASE_FACTOR;
    }
    if ( Name == "GOOD_DETECTS_START")
    {
        return GOOD_DETECTS_START;
    }
    if ( Name == "BAD_DETECTS_DISARM")
    {
        return BAD_DETECTS_DISARM;
    }

    if ( Name == "MAX_CURRENT")
    {
        return MAX_CURRENT;
    }
    if ( Name == "SWITCH_FREQ")
    {
        return SWITCH_FREQ;
    }
    if ( Name == "MOTOR_POLES")
    {
        return MOTOR_POLES;
    }
    if ( Name == "PWM_MIN_PERIOD")
    {
        return PWM_MIN_PERIOD;
    }

    if ( Name == "PWM_MIN_VALUE")
    {
        return PWM_MIN_VALUE;
    }
    if ( Name == "PWM_LO_VALUE")
    {
        return PWM_LO_VALUE;
    }
    if ( Name == "PWM_HI_VALUE")
    {
        return PWM_HI_VALUE;
    }
    if ( Name == "PWM_MAX_VALUE")
    {
        return PWM_MAX_VALUE;
    }
    if ( Name == "PWM_MIN_START")
    {
        return PWM_MIN_START;
    }

    if ( Name == "PWM_RPM_SCALE")
    {
        return PWM_RPM_SCALE;
    }
    if ( Name == "FET_BRAKING")
    {
        return FET_BRAKING;
    }
    if ( Name == "CONFIG_NUM_PARAMS")
    {
        return CONFIG_NUM_PARAMS;
    }
    return 0;
}

void AQEsc32::connectedEsc32(){
    emit Esc32Connected();
    //read_config_esc32();
}

void AQEsc32::disconnectedEsc32(){
    emit ESc32Disconnected();
}

void AQEsc32::destroyedEsc32(){
}

void AQEsc32::BytesRceivedEsc32(LinkInterface* link, QByteArray bytes){
    // Only add data from current link
    if (link == seriallinkEsc32)
    {
        //unsigned char byte = bytes.at(j);
        switch (StepMessageFromEsc32)
        {
            case 0:
                // Parse all bytes
            break;

            //get ascii values for parameter
            case 1:
                LIST_MessageFromEsc32 += QString(bytes);
                if ( LIST_MessageFromEsc32.contains("Command not found")) {
                    LIST_MessageFromEsc32 = "";
                    qDebug() << LIST_MessageFromEsc32;
                    return;
                }

                if ( LIST_MessageFromEsc32.endsWith("> ")) {
                    //decodeParameterFromEsc32(LIST_MessageFromEsc32);
                    emit ShowConfig(LIST_MessageFromEsc32);
                    qDebug() << LIST_MessageFromEsc32;
                    LIST_MessageFromEsc32 = "";
                }
            break;

            // Waiting for a commit to ASCII mode
            case 2:
                ParaWriten_MessageFromEsc32 += QString(bytes);
                ResponseFromEsc32.append(bytes);
                indexOfAqC = ParaWriten_MessageFromEsc32.indexOf("AqC");
                if (indexOfAqC > -1) {
                    checkInA = checkInB = 0;
                    int in = indexOfAqC+3;
                    commandLengthBack = ResponseFromEsc32[in];
                    esc32InChecksum(commandLengthBack);
                    in++;
                    command_ACK_NACK = ResponseFromEsc32[in];
                    esc32InChecksum(command_ACK_NACK);
                    in++;
                    commandSeqIdBack = ResponseFromEsc32[in];
                    esc32InChecksum(commandSeqIdBack);
                    in++;
                    commandBack = ResponseFromEsc32[in];
                    esc32InChecksum(commandBack);
                    in++;
                    unsigned char tmp_A = ResponseFromEsc32[in];
                    unsigned char tmp_B = ResponseFromEsc32[in+1];
                    if ((checkInA == tmp_A ) && (checkInB == tmp_B)) {
                        if ( command_ACK_NACK == 250) {
                            esc32BinaryMode = 0;
                            esc32state = 0;
                        }
                    }
                }

            break;

            // Waiting for a commit to binary mode
            case 3:
                ParaWriten_MessageFromEsc32 += QString(bytes);
                if ( ParaWriten_MessageFromEsc32.contains("command mode...\r\n")) {
                    esc32BinaryMode = 1;
                    ParaWriten_MessageFromEsc32 = "";
                    esc32state = 1;
                }
            break;

            //Waiting for commit of send Parameter
            case 4:
                ParaWriten_MessageFromEsc32 += QString(bytes);
                ResponseFromEsc32.append(bytes);
                indexOfAqC = ParaWriten_MessageFromEsc32.indexOf("AqC");
                if (indexOfAqC > -1) {
                    checkInA = checkInB = 0;
                    int in = indexOfAqC+3;
                    commandLengthBack = ResponseFromEsc32[in];
                    esc32InChecksum(commandLengthBack);
                    in++;
                    command_ACK_NACK = ResponseFromEsc32[in];
                    esc32InChecksum(command_ACK_NACK);
                    in++;
                    commandSeqIdBack = ResponseFromEsc32[in];
                    esc32InChecksum(commandSeqIdBack);
                    in++;
                    commandBack = ResponseFromEsc32[in];
                    esc32InChecksum(commandBack);
                    in++;
                    unsigned char tmp_A = ResponseFromEsc32[in];
                    unsigned char tmp_B = ResponseFromEsc32[in+1];
                    if ((checkInA == tmp_A ) && (checkInB == tmp_B)) {
                        if ( ParaLastSend == BINARY_COMMAND_SET )
                            emit Esc32ParaWritten(ParaNameLastSend);
                        else
                            emit Esc32CommandWritten(ParaLastSend,LastParaValueSend1,LastParaValueSend2);

                        if ( ParaLastSend == BINARY_COMMAND_START)
                            esc32state = 2;
                        if ( ParaLastSend == BINARY_COMMAND_STOP )
                            esc32state = 3;
                        if ( ParaLastSend == BINARY_COMMAND_ARM )
                            esc32state = 4;
                        if ( ParaLastSend == BINARY_COMMAND_DISARM )
                            esc32state = 5;


                    }
                }

            break;

            default:
            break;
        }
    }
}

void AQEsc32::ReadConfigEsc32() {

    QByteArray transmit;
    StepMessageFromEsc32 = 1;
    transmit.append("set list\n");
    seriallinkEsc32->writeBytes(transmit,transmit.size());
}

void AQEsc32::SwitchFromAsciiToBinary()
{
    StepMessageFromEsc32 = 3;
    QByteArray transmit;
    transmit.append("binary\n");
    seriallinkEsc32->writeBytes(transmit,transmit.size());
    TimeOutWaiting = 0;
    while ( esc32BinaryMode == 0) {
        QCoreApplication::processEvents();
        TimeOutWaiting++;
        if (TimeOutWaiting > 100000 )
                break;
    }
}

void AQEsc32::SwitchFromBinaryToAscii()
{
    StepMessageFromEsc32 = 2;
    commandSeqIdBack = esc32SendCommand(BINARY_COMMAND_CLI, 0.0, 0.0, 0);
    TimeOutWaiting = 0;
    while ( esc32BinaryMode == 1) {
        QCoreApplication::processEvents();
        TimeOutWaiting++;
        if (TimeOutWaiting > 100000 )
                break;
    }
}

int AQEsc32::esc32SendCommand(unsigned char command, float param1, float param2, int n) {
    QByteArray transmit;
    ResponseFromEsc32.clear();
    ParaWriten_MessageFromEsc32 = "";
    transmit.append("A");
    transmit.append("q");
    //################################
    seriallinkEsc32->writeBytes(transmit,transmit.length());
    checkOutA = checkOutB = 0;

    esc32SendChar(1 + 2 + n*sizeof(float));
    esc32SendChar(command);
    esc32SendShort(commandSeqId++);
    if (n > 0)
        esc32SendFloat(param1);
    if (n > 1)
        esc32SendFloat(param2);
    transmit.clear();
    transmit.append(checkOutA);
    transmit.append(checkOutB);
    seriallinkEsc32->writeBytes(transmit,transmit.size());
    return (commandSeqId - 1);
}

void AQEsc32::esc32SendChar(unsigned char c) {
    QByteArray transmit;
    transmit.append(c);
    seriallinkEsc32->writeBytes(transmit,transmit.size());
    esc32OutChecksum(c);
}

void AQEsc32::esc32SendShort(unsigned short i) {
    unsigned char j;
    unsigned char *c = (unsigned char *)&i;

    for (j = 0; j < sizeof(short); j++)
        esc32SendChar(*c++);
}

void AQEsc32::esc32SendFloat(float f) {
    unsigned char j;
    unsigned char *c = (unsigned char *)&f;

    for (j = 0; j < sizeof(float); j++)
        esc32SendChar(*c++);
}

void AQEsc32::esc32OutChecksum(unsigned char c) {
    checkOutA += c;
    checkOutB += checkOutA;
}

void AQEsc32::esc32InChecksum(unsigned char c) {
    checkInA += c;
    checkInB += checkInA;
}



AQEsc32Calibration::AQEsc32Calibration() {

}

AQEsc32Calibration::~AQEsc32Calibration() {

}

void AQEsc32Calibration::run() {

    /*
    esc32SendReliably(BINARY_COMMAND_ARM, 0.0, 0.0, 0);
    esc32SendReliably(BINARY_COMMAND_STOP, 0.0, 0.0, 0);
    esc32SendReliably(BINARY_COMMAND_TELEM_RATE, 0.0, 0.0, 1);
    esc32SendReliably(BINARY_COMMAND_TELEM_VALUE, 0.0, BINARY_VALUE_RPM, 2);
    esc32SendReliably(BINARY_COMMAND_TELEM_VALUE, 1.0, BINARY_VALUE_VOLTS_MOTOR, 2);
    esc32SendReliably(BINARY_COMMAND_TELEM_VALUE, 2.0, BINARY_VALUE_AMPS, 2);
    esc32SendReliably(BINARY_COMMAND_SET, MAX_CURRENT, 0.0, 2);
    */

}
