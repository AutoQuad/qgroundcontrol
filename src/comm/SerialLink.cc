/*=====================================================================
======================================================================*/
/**
 * @file
 *   @brief Cross-platform support for serial ports
 *
 *   @author Lorenz Meier <mavteam@student.ethz.ch>
 *   @author Maxim Paperno (max@paperno.us)
 *
 */

#include <QTimer>
#include <QDebug>
#include <QSettings>
#include <QMutexLocker>
//#include <QSerialPortInfo>

#include "SerialLink.h"
#include "LinkManager.h"
#include "QGC.h"
#include <MG.h>
//#include <iostream>

//#ifdef _WIN32
//#include "windows.h"
//#include <qextserialenumerator.h>
//#endif

/* old Mac init code
//#if defined (__APPLE__) && defined (__MACH__)
//#include <stdio.h>
//#include <string.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <sys/ioctl.h>
//#include <errno.h>
//#include <paths.h>
//#include <termios.h>
//#include <sysexits.h>
//#include <sys/param.h>
//#include <sys/select.h>
//#include <sys/time.h>
//#include <time.h>
//#include <AvailabilityMacros.h>

//#ifdef __MWERKS__
//#define __CF_USE_FRAMEWORK_INCLUDES__
//#endif


//#include <CoreFoundation/CoreFoundation.h>

//#include <IOKit/IOKitLib.h>
//#include <IOKit/serial/IOSerialKeys.h>
//#if defined(MAC_OS_X_VERSION_10_3) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_3)
//#include <IOKit/serial/ioss.h>
//#endif
//#include <IOKit/IOBSD.h>

//// Apple internal modems default to local echo being on. If your modem has local echo disabled,
//// undefine the following macro.
//#define LOCAL_ECHO

//#define kATCommandString  "AT\r"

//#ifdef LOCAL_ECHO
//#define kOKResponseString  "AT\r\r\nOK\r\n"
//#else
//#define kOKResponseString  "\r\nOK\r\n"
//#endif
//#endif


//// Some helper functions for serial port enumeration
//#if defined (__APPLE__) && defined (__MACH__)

//enum {
//    kNumRetries = 3
//};

//// Function prototypes
//static kern_return_t FindModems(io_iterator_t *matchingServices);
//static kern_return_t GetModemPath(io_iterator_t serialPortIterator, char *bsdPath, CFIndex maxPathSize);

//// Returns an iterator across all known modems. Caller is responsible for
//// releasing the iterator when iteration is complete.
//static kern_return_t FindModems(io_iterator_t *matchingServices)
//{
//    kern_return_t      kernResult;
//    CFMutableDictionaryRef  classesToMatch;

//    //! @function IOServiceMatching
//    //@abstract Create a matching dictionary that specifies an IOService class match.
//    //@discussion A very common matching criteria for IOService is based on its class. IOServiceMatching will create a matching dictionary that specifies any IOService of a class, or its subclasses. The class is specified by C-string name.
//    //@param name The class name, as a const C-string. Class matching is successful on IOService's of this class or any subclass.
//    //@result The matching dictionary created, is returned on success, or zero on failure. The dictionary is commonly passed to IOServiceGetMatchingServices or IOServiceAddNotification which will consume a reference, otherwise it should be released with CFRelease by the caller.

//    // Serial devices are instances of class IOSerialBSDClient
//    classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue);
//    if (classesToMatch == NULL) {
//        printf("IOServiceMatching returned a NULL dictionary.\n");
//    } else {
//        //!
//        //  @function CFDictionarySetValue
//        //  Sets the value of the key in the dictionary.
//        //  @param theDict The dictionary to which the value is to be set. If this
//        //    parameter is not a valid mutable CFDictionary, the behavior is
//        //    undefined. If the dictionary is a fixed-capacity dictionary and
//        //    it is full before this operation, and the key does not exist in
//        //    the dictionary, the behavior is undefined.
//        //  @param key The key of the value to set into the dictionary. If a key
//        //    which matches this key is already present in the dictionary, only
//        //    the value is changed ("add if absent, replace if present"). If
//        //    no key matches the given key, the key-value pair is added to the
//        //    dictionary. If added, the key is retained by the dictionary,
//        //    using the retain callback provided
//        //    when the dictionary was created. If the key is not of the sort
//        //    expected by the key retain callback, the behavior is undefined.
//        //  @param value The value to add to or replace into the dictionary. The value
//        //    is retained by the dictionary using the retain callback provided
//        //    when the dictionary was created, and the previous value if any is
//        //    released. If the value is not of the sort expected by the
//        //    retain or release callbacks, the behavior is undefined.
//        //
//        CFDictionarySetValue(classesToMatch,
//                             CFSTR(kIOSerialBSDTypeKey),
//                             CFSTR(kIOSerialBSDModemType));

//        // Each serial device object has a property with key
//        // kIOSerialBSDTypeKey and a value that is one of kIOSerialBSDAllTypes,
//        // kIOSerialBSDModemType, or kIOSerialBSDRS232Type. You can experiment with the
//        // matching by changing the last parameter in the above call to CFDictionarySetValue.

//        // As shipped, this sample is only interested in modems,
//        // so add this property to the CFDictionary we're matching on.
//        // This will find devices that advertise themselves as modems,
//        // such as built-in and USB modems. However, this match won't find serial modems.
//    }

//    //! @function IOServiceGetMatchingServices
//    //    @abstract Look up registered IOService objects that match a matching dictionary.
//    //    @discussion This is the preferred method of finding IOService objects currently registered by IOKit. IOServiceAddNotification can also supply this information and install a notification of new IOServices. The matching information used in the matching dictionary may vary depending on the class of service being looked up.
//    //    @param masterPort The master port obtained from IOMasterPort().
//    //    @param matching A CF dictionary containing matching information, of which one reference is consumed by this function. IOKitLib can contruct matching dictionaries for common criteria with helper functions such as IOServiceMatching, IOOpenFirmwarePathMatching.
//    //    @param existing An iterator handle is returned on success, and should be released by the caller when the iteration is finished.
//    //    @result A kern_return_t error code.

//    kernResult = IOServiceGetMatchingServices(kIOMasterPortDefault, classesToMatch, matchingServices);
//    if (KERN_SUCCESS != kernResult) {
//        printf("IOServiceGetMatchingServices returned %d\n", kernResult);
//        goto exit;
//    }

//exit:
//    return kernResult;
//}

//// Given an iterator across a set of modems, return the BSD path to the first one.
////  If no modems are found the path name is set to an empty string.
//
//static kern_return_t GetModemPath(io_iterator_t serialPortIterator, char *bsdPath, CFIndex maxPathSize)
//{
//    io_object_t    modemService;
//    kern_return_t  kernResult = KERN_FAILURE;
//    Boolean      modemFound = false;

//    // Initialize the returned path
//    *bsdPath = '\0';

//    // Iterate across all modems found. In this example, we bail after finding the first modem.

//    while ((modemService = IOIteratorNext(serialPortIterator)) && !modemFound) {
//        CFTypeRef  bsdPathAsCFString;

//        // Get the callout device's path (/dev/cu.xxxxx). The callout device should almost always be
//        // used: the dialin device (/dev/tty.xxxxx) would be used when monitoring a serial port for
//        // incoming calls, e.g. a fax listener.

//        bsdPathAsCFString = IORegistryEntryCreateCFProperty(modemService,
//                                                            CFSTR(kIOCalloutDeviceKey),
//                                                            kCFAllocatorDefault,
//                                                            0);
//        if (bsdPathAsCFString) {
//            Boolean result;

//            // Convert the path from a CFString to a C (NUL-terminated) string for use
//            // with the POSIX open() call.

//            result = CFStringGetCString((CFStringRef)bsdPathAsCFString,
//                                        bsdPath,
//                                        maxPathSize,
//                                        kCFStringEncodingUTF8);
//            CFRelease(bsdPathAsCFString);

//            if (result) {
//                //printf("Modem found with BSD path: %s", bsdPath);
//                modemFound = true;
//                kernResult = KERN_SUCCESS;
//            }
//        }

//        printf("\n");

//        // Release the io_service_t now that we are done with it.

//        (void) IOObjectRelease(modemService);
//    }

//    return kernResult;
//}
//#endif
*/
//using namespace TNX;


SerialLink::SerialLink(QString portname, int baudRate, bool hardwareFlowControl, bool parity,
                       int dataBits, int stopBits) :
    port(0),
    portSettings(PortSettings()),
    portOpenMode(QIODevice::ReadWrite),
    bitsSentTotal(0),
    bitsSentShortTerm(0),
    bitsSentCurrent(0),
    bitsSentMax(0),
    bitsReceivedTotal(0),
    bitsReceivedShortTerm(0),
    bitsReceivedCurrent(0),
    bitsReceivedMax(0),
    connectionStartTime(0),
    ports(new QVector<QString>()),
    waitingToReconnect(0),
    m_reconnectDelayMs(0),
    m_linkLossExpected(false),
    m_stopp(false),
    mode_port(false),
    countRetry(0),
    maxLength(0),
    rows(0),
    cols(0),
    firstRead(0)
{
    portEnumerator = new QextSerialEnumerator();
    portEnumerator->setUpNotifications();
    QObject::connect(portEnumerator, SIGNAL(deviceDiscovered(const QextPortInfo &)), this, SLOT(deviceDiscovered(const QextPortInfo &)));
    QObject::connect(portEnumerator, SIGNAL(deviceRemoved(const QextPortInfo &)), this, SLOT(deviceRemoved(const QextPortInfo &)));

    getCurrentPorts();

    // Setup settings
    this->porthandle = portname.trimmed();
    if (!ports->contains(porthandle))
        porthandle = "";
    if (this->porthandle == "" && ports->size() > 0)
        this->porthandle = ports->first();

/*
//#ifdef _WIN32
    // Port names above 20 need the network path format - if the port name is not already in this format
    // catch this special case
//    if (this->porthandle.size() > 0 && !this->porthandle.startsWith("\\")) {
//        // Append \\.\ before the port handle. Additional backslashes are used for escaping.
//        this->porthandle = "\\\\.\\" + this->porthandle;
//    }
//#endif
*/

    // Set unique ID and add link to the list of links
    this->id = getNextLinkId();

    int par = parity ? (int)PAR_EVEN : (int)PAR_NONE;
    int fc = hardwareFlowControl ? (int)FLOW_HARDWARE : (int)FLOW_OFF;

    setBaudRate(baudRate);
    setFlowType(fc);
    setParityType(par);
    setDataBitsType(dataBits);
    setStopBitsType(stopBits);
    setTimeoutMillis(-1);  // -1 means do not block on serial read/write. Do not use zero.
    setReconnectDelayMs(10);  // default 10ms before reconnecting to M4, after detecting that COM port is back

    // Set the port name
    name = this->porthandle.length() ? this->porthandle : tr("Serial Link ") + QString::number(getId());

    if (name == this->porthandle || name == "")
        loadSettings();

    QObject::connect(this, SIGNAL(portError()), this, SLOT(disconnect()));

}

SerialLink::~SerialLink()
{
    this->disconnect();
    if(port)
        delete port;
    if (ports)
        delete ports;
    port = NULL;
    ports = NULL;
}

QVector<QString>* SerialLink::getCurrentPorts()
{
    ports->clear();
    foreach (const QextPortInfo &p, portEnumerator->getPorts()) {
        if (p.portName.length())
            ports->append(p.portName);//  + " - " + p.friendName);
//      qDebug() << __FILE__ << __LINE__ << p.portName  << p.friendName << p.physName << p.enumName << p.vendorID << p.productID;
    }
/* old Linux and Mac code
//#ifdef __linux

//    // TODO Linux has no standard way of enumerating serial ports
//    // However the device files are only present when the physical
//    // device is connected, therefore listing the files should be
//    // sufficient.

//    QString devdir = "/dev";
//    QDir dir(devdir);
//    dir.setFilter(QDir::System);

//    QFileInfoList list = dir.entryInfoList();
//    for (int i = 0; i < list.size(); ++i) {
//        QFileInfo fileInfo = list.at(i);
//        if (fileInfo.fileName().contains(QString("ttyUSB")) || fileInfo.fileName().contains(QString("ttyS")) || fileInfo.fileName().contains(QString("ttyACM")))
//        {
//            ports->append(fileInfo.canonicalFilePath());
//        }
//    }
//#endif

//#if defined (__APPLE__) && defined (__MACH__)

//    // Enumerate serial ports
//    kern_return_t    kernResult; // on PowerPC this is an int (4 bytes)
//    io_iterator_t    serialPortIterator;
//    char        bsdPath[MAXPATHLEN];
//    kernResult = FindModems(&serialPortIterator);
//    kernResult = GetModemPath(serialPortIterator, bsdPath, sizeof(bsdPath));
//    IOObjectRelease(serialPortIterator);    // Release the iterator.

//    // Add found modems
//    if (bsdPath[0])
//    {
//        ports->append(QString(bsdPath));
//    }

//    // Add USB serial port adapters
//    // TODO Strangely usb serial port adapters are not enumerated, even when connected
//    QString devdir = "/dev";
//    QDir dir(devdir);
//    dir.setFilter(QDir::System);

//    QFileInfoList list = dir.entryInfoList();
//    for (int i = list.size() - 1; i >= 0; i--) {
//        QFileInfo fileInfo = list.at(i);
//        if (fileInfo.fileName().contains(QString("ttyUSB")) ||
//                fileInfo.fileName().contains(QString("tty.")) ||
//                fileInfo.fileName().contains(QString("ttyS")) ||
//                fileInfo.fileName().contains(QString("ttyACM")))
//        {
//            ports->append(fileInfo.canonicalFilePath());
//        }
//    }
//#endif
*/

    return this->ports;
}

void SerialLink::loadSettings()
{
    // Load defaults from settings
    QSettings settings;
    settings.sync();
    if (settings.contains("SERIALLINK_COMM_PORT"))
    {
        setPortName(settings.value("SERIALLINK_COMM_PORT").toString());
        setBaudRate(settings.value("SERIALLINK_COMM_BAUD").toInt());
        setParityType(settings.value("SERIALLINK_COMM_PARITY").toInt());
        setStopBitsType(settings.value("SERIALLINK_COMM_STOPBITS").toInt());
        setDataBitsType(settings.value("SERIALLINK_COMM_DATABITS").toInt());
        setFlowType(settings.value("SERIALLINK_COMM_FLOW_CONTROL").toInt());
        setTimeoutMillis(settings.value("SERIALLINK_COMM_TIMEOUT", (qint32)portSettings.Timeout_Millisec).toInt());
        setReconnectDelayMs(settings.value("SERIALLINK_COMM_RECONDELAY", m_reconnectDelayMs).toInt());
    }
}

void SerialLink::writeSettings()
{
    // Store settings
    QSettings settings;
    settings.setValue("SERIALLINK_COMM_PORT", getPortName());
    settings.setValue("SERIALLINK_COMM_BAUD", getBaudRateType());
    settings.setValue("SERIALLINK_COMM_PARITY", getParityType());
    settings.setValue("SERIALLINK_COMM_STOPBITS", getStopBitsType());
    settings.setValue("SERIALLINK_COMM_DATABITS", getDataBitsType());
    settings.setValue("SERIALLINK_COMM_FLOW_CONTROL", getFlowType());
    settings.setValue("SERIALLINK_COMM_TIMEOUT", (qint32)getTimeoutMillis());
    settings.setValue("SERIALLINK_COMM_RECONDELAY", reconnectDelayMs());
    settings.sync();
}


/**
 * @brief Runs the thread
 *
 **/
void SerialLink::run()
{
    // Initialize the connection
    if (!hardwareConnect())
        return;

    while (!m_stopp)
    {
        this->readBytes();

        // Serial data isn't arriving that fast normally, this saves the thread
        // from consuming too much processing time
        msleep(SerialLink::poll_interval);
    }
}

bool SerialLink::validateConnection() {
    bool ok = this->isConnected() && !port->lastError(); // && (!port->error() || port->error() == QSerialPort::TimeoutError)
    if (ok && (portOpenMode & QIODevice::ReadOnly) && !port->isReadable())
        ok = false;
    if (ok && (portOpenMode & QIODevice::WriteOnly) && !port->isWritable())
        ok = false;
    if(!ok) {
        emit portError();
        if (!m_linkLossExpected)
            emit communicationError(this->getName(), tr("Link %1 unexpectedly disconnected!").arg(this->porthandle));
        //qWarning() << __FILE__ << __LINE__ << ok << port->lastError() << port->errorString();
        //this->disconnect();
        return false;
    }
    return true;
}

void SerialLink::deviceRemoved(const QextPortInfo &pi)
{
    bool isValid = isPortHandleValid();

    if (!isValid && pi.vendorID == 1155 && pi.productID == 22352)
        waitingToReconnect = MG::TIME::getGroundTimeNow();

//    qDebug() << __FILE__ << __LINE__ << pi.portName  << pi.friendName << pi.physName << pi.enumName << pi.vendorID << pi.productID << getPortName() << waitingToReconnect;

    if (!port || !port->isOpen() || isValid)
        return;

    emit portError();
    if (!m_linkLossExpected)
        emit communicationError(this->getName(), tr("Link %1 unexpectedly disconnected!").arg(this->porthandle));
    //qWarning() << __FILE__ << __LINE__ << "device removed" << port->lastError() << port->errorString();
}

void SerialLink::deviceDiscovered(const QextPortInfo &pi)
{
//    qDebug() << __FILE__ << __LINE__ << pi.portName  << pi.friendName << pi.physName << pi.enumName << pi.vendorID << pi.productID << getPortName()
//             << waitingToReconnect << MG::TIME::getGroundTimeNow() << MG::TIME::getGroundTimeNow() - waitingToReconnect;
    Q_UNUSED(pi);
    if (waitingToReconnect && !port) {
        if (MG::TIME::getGroundTimeNow() - waitingToReconnect > reconnect_wait_timeout) {
            waitingToReconnect = 0;
            return;
        }
        if (isPortHandleValid()) {
            QTimer::singleShot(m_reconnectDelayMs, this, SLOT(connect()));
            waitingToReconnect = 0;
            //this->connect();
        }
    }
}

void SerialLink::writeBytes(const char* data, qint64 size)
{
    if (!validateConnection())
        return;
    int b = port->write(data, size);

    if (b > 0) {
        // Increase write counter
        bitsSentTotal += b * 8;
        //qDebug() << "Serial link " << this->getName() << "transmitted" << b << "bytes:";
    } else if (b == -1) {
        emit portError();
        if (!m_linkLossExpected)
            emit communicationError(this->getName(), tr("Could not send data - error on link %1").arg(this->porthandle));
    }
}

/**
 * @brief Read a number of bytes from the interface.
 *
 * @param data Pointer to the data byte array to write the bytes to
 * @param maxLength The maximum number of bytes to write
 **/
void SerialLink::readBytes()
{
    if (!validateConnection())
        return;

    if (mode_port)  // ESC32 special data read mode
        return readEsc32Tele();

    const qint64 maxLength = 2048;
    char data[maxLength];
    qint64 numBytes = 0, rBytes = 0; //port->bytesAvailable();

    dataMutex.lock();
    while(numBytes = port->bytesAvailable()) {
    //if(rBytes) {
        //qDebug() << "numBytes: " << numBytes;
        /* Read as much data in buffer as possible without overflow */
        rBytes = numBytes;
        if(maxLength < rBytes) rBytes = maxLength;

        if (port->read(data, rBytes) == -1) { // -1 result means error
            emit portError();
            if (!m_linkLossExpected)
                emit communicationError(this->getName(), tr("Could not read data - link %1 is disconnected!").arg(this->getName()));
            return;
        }

        QByteArray b(data, rBytes);
        emit bytesReceived(this, b);
        bitsReceivedTotal += rBytes * 8;

//        for (int i=0; i<rBytes; i++){
//            unsigned int v=data[i];
//            fprintf(stderr,"%02x ", v);
//        } fprintf(stderr,"\n");
    }
    dataMutex.unlock();
}

void SerialLink::readEsc32Tele(){
    //dataMutex.lock();
    qint64 numBytes = 0;

    if (!validateConnection())
        return;

    // Clear up the input Buffer
    if ( this->firstRead == 1) {
        while(true) {
            port->read(data,1);
            if (data[0] == 'A') {
                break;
            }
        }
        this->firstRead = 2;
    }
    retryA:
    if (!port->isOpen() || !mode_port)
        return;

    while( true) {
        numBytes = port->bytesAvailable();
        if ( numBytes > 0)
            break;
        msleep(1);
        if (!port->isOpen() || !mode_port)
            break;
    }
    if ( this->firstRead == 0) {
        numBytes = port->read(data,1);
        if ( numBytes  >= 1 ) {
            if ( data[0] != 'A') {
                goto retryA;
            }
        }
        else if (numBytes <= 0){
            msleep(5);
            goto retryA;
        }
    }
    else {
         this->firstRead = 0;
    }

    while( true) {
        numBytes = port->bytesAvailable();
        if ( numBytes > 0)
            break;
        msleep(1);
        if (!port->isOpen() || !mode_port)
            break;
    }
    numBytes = port->read(data,1);
    if ( numBytes  == 1 ) {
        if ( data[0] != 'q') {
            goto retryA;
        }
    }
    else if (numBytes <= 0){
        msleep(5);
        goto retryA;
    }

    while( true) {
        numBytes = port->bytesAvailable();
        if ( numBytes > 0)
            break;
        msleep(1);
        if (!port->isOpen() || !mode_port)
            break;
    }
    numBytes = port->read(data,1);
    if ( numBytes  == 1 ) {
        if ( data[0] != 'T') {
            msleep(5);
            goto retryA;
        }
    }
    else if (numBytes <= 0){
        msleep(5);
        goto retryA;
    }

    while( true) {
        numBytes = port->bytesAvailable();
        if ( numBytes >= 2)
            break;
        msleep(1);
        if (!port->isOpen() || !mode_port)
            break;
    }
    port->read(data,2);
    rows = 0;
    cols = 0;
    rows = data[0];
    cols = data[1];
    int length_array = (((cols*rows)*sizeof(float))+2);
    if ( length_array > 300) {
        qDebug() << "bad col " << cols << " rows " << rows;
        goto retryA;
    }
    while( true) {
        numBytes = port->bytesAvailable();
        if ( numBytes >= length_array)
            break;
        msleep(1);
        if (!port->isOpen() || !mode_port)
            break;
    }
    //qDebug() << "avalible" << numBytes;
    numBytes = port->read(data,length_array);
    if (numBytes == length_array ){
        QByteArray b(data, numBytes);
        emit teleReceived(b, rows, cols);
    }

    goto retryA;

    //dataMutex.unlock();
}

/**
 * @brief Disconnect the connection.
 *
 * @return True if connection has been disconnected, false if connection couldn't be disconnected.
 **/
bool SerialLink::disconnect()
{
    if(this->isRunning() && !m_stopp) {
        m_stoppMutex.lock();
        this->m_stopp = true;
        //m_waitCond.wakeOne();
        m_stoppMutex.unlock();
        // w/out the following if(), when calling disconnect() from run() we can get a warning about thread waiting on itself
        //if (currentThreadId() != QThread::currentThreadId())
        this->wait();
    }

    if (this->isConnected())
        port->close();

    // delete (invalid) port. not sure this is necessary.
    if (port) { //  && !isPortHandleValid()
        port->deleteLater();
        port = NULL;
    }

    emit disconnected();
    emit connected(false);
    return !this->isConnected();
}

/**
 * @brief Connect the connection.
 *
 * @return True if connection has been established, false if connection couldn't be established.
 **/
bool SerialLink::connect()
{
    if (this->isRunning()) {
        this->disconnect();
        this->wait();
    }

    // reset the run stop flag
    m_stoppMutex.lock();
    m_stopp = false;
    m_stoppMutex.unlock();

    waitingToReconnect = 0;
    m_linkLossExpected = false;

    this->start(QThread::LowPriority);
    return this->isRunning();
}

/**
 * @brief This function is called indirectly by the connect() call.
 *
 * The connect() function starts the thread and indirectly calls this method.
 *
 * @return True if the connection could be established, false otherwise
 * @see connect() For the right function to establish the connection.
 **/
bool SerialLink::hardwareConnect()
{
    if (!isPortHandleValid()) {
        emit communicationError(this->getName(), tr("Failed to open serial port %1 because it no longer exists in the system.").arg(porthandle));
        return false;
    }

    if (!port) {
        port = new QextSerialPort(QextSerialPort::Polling);
        if (!port) {
            emit communicationError(this->getName(), tr("Could not create serial port object."));
            return false;
        }
        QObject::connect(port, SIGNAL(aboutToClose()), this, SIGNAL(disconnected()));
        //QObject::connect(port, SIGNAL(readyRead()), this, SLOT(readBytes()), Qt::DirectConnection);
    }

    if (port->isOpen())
        port->close();

    QString err;

    port->setPortName(porthandle);
    port->setBaudRate(portSettings.BaudRate);
    port->setDataBits(portSettings.DataBits);
    port->setParity(portSettings.Parity);
    port->setStopBits(portSettings.StopBits);
    port->setFlowControl(portSettings.FlowControl);
    port->setTimeout(portSettings.Timeout_Millisec);

    if (!port->open(portOpenMode))
        err = tr("Failed to open serial port %1 with error: %2 (%3)").arg(this->porthandle).arg(port->errorString()).arg(port->lastError());

    if (err.length()) {
        emit communicationError(this->getName(), err);
        qWarning() << __FILE__ << __LINE__ << err << port->portName() << port->baudRate() << "db:" << port->dataBits() \
                   << "p:" << port->parity() << "sb:" << port->stopBits() << "fc:" << port->flowControl();

        if (port->isOpen())
            port->close();
        port->deleteLater();
        port = NULL;

        return false;
    }

    connectionStartTime = MG::TIME::getGroundTimeNow();

    if(isConnected()) {
        emit connected();
        emit connected(true);
        qDebug() << __FILE__ << __LINE__  << "Connected Serial" << porthandle  << "with settings" \
                 << port->portName() << port->baudRate() << "db:" << port->dataBits() \
                 << "p:" << port->parity() << "sb:" << port->stopBits() << "fc:" << port->flowControl();
    } else
        return false;

    writeSettings();

    return true;
}


// verify that a port still exists on the system
bool SerialLink::isPortValid(const QString &pname) {
//    QSerialPortInfo pi(pname);
//    return pi.isValid();
    return getCurrentPorts()->contains(pname);
}

// verify that the current port still exists on the system
bool SerialLink::isPortHandleValid() {
    return isPortValid(porthandle);
}

bool SerialLink::isConnected()
{
    if (port)
        return port->isOpen(); //isPortHandleValid() &&
    else
        return false;
}


// currently unused
qint64 SerialLink::bytesAvailable()
{
    if (port) {
        return port->bytesAvailable();
    } else {
        return 0;
    }
}

// not used at the moment, doesn't detect disconnect with "native" USB connection (eg. AQ M4)
//void SerialLink::handleError(QSerialPort::SerialPortError error)
//{
//    //qDebug() << __FILE__ << __LINE__ << error << port->error() << port->errorString();
//    if (error == QSerialPort::ResourceError) {
//        emit communicationError(this->getName(), tr("Link %1 unexpectedly disconnected with error: %2").arg(this->getName()).arg(port->errorString()));
//        this->disconnect();
//    }
//}


//
// Setter methods
//

bool SerialLink::setPortName(QString portName)
{
    portName = portName.trimmed();
    if(this->porthandle != portName && this->isPortValid(portName)) {
        bool reconnect = isConnected();
        if (reconnect)
            this->disconnect();
        if (isRunning())
            this->wait();

        this->porthandle = portName;
        setName(tr("serial port ") + portName);

//#ifdef _WIN32
        // Port names above 20 need the network path format - if the port name is not already in this format
        // catch this special case
//        if (!this->porthandle.startsWith("\\")) {
//            // Append \\.\ before the port handle. Additional backslashes are used for escaping.
//            this->porthandle = "\\\\.\\" + this->porthandle;
//        }
//#endif

        // do not auto-reconnect
        //if(reconnect)
        //    this->connect();
    }
    return true;
}


void SerialLink::setName(QString name)
{
    if (this->name != name) {
        this->name = name;
        emit nameChanged(this->name);
    }
}

// doesn't seem to be used anywhere
bool SerialLink::setBaudRateString(const QString& rate)
{
    bool ok;
    int intrate = rate.toInt(&ok);
    if (!ok) return false;
    return setBaudRate(intrate);
}

bool SerialLink::setBaudRate(int rate)
{
    bool accepted = false;
    BaudRateType br = (BaudRateType)(rate);
    if (br) {
        portSettings.BaudRate = br;
        if (port)
            port->setBaudRate(portSettings.BaudRate);
        accepted = true;
    }
    return accepted;
}

bool SerialLink::setTimeoutMillis(const long &ms)
{
    portSettings.Timeout_Millisec = ms;
    if (port)
        port->setTimeout(portSettings.Timeout_Millisec);
    return true;
}

bool SerialLink::setFlowType(int flow)
{
    bool accepted = false;
    if (flow >= (int)FLOW_OFF && flow <= (int)FLOW_XONXOFF) {
        portSettings.FlowControl = (FlowType)flow;
        if (port)
            port->setFlowControl(portSettings.FlowControl);
        accepted = true;
    }
    return accepted;
}

bool SerialLink::setParityType(int parity)
{
    bool accepted = false;
    if (parity >= (int)PAR_NONE && parity <= (int)PAR_SPACE) {
        portSettings.Parity = (ParityType)parity;
        if (port)
            port->setParity(portSettings.Parity);
        accepted = true;
    }
    return accepted;
}


bool SerialLink::setDataBitsType(int dataBits)
{
    bool accepted = false;
    if (dataBits >= (int)DATA_5 && dataBits <= (int)DATA_8) {
        portSettings.DataBits = (DataBitsType)dataBits;
        if (port)
            port->setDataBits(portSettings.DataBits);
        accepted = true;
    }
    return accepted;
}

bool SerialLink::setStopBitsType(int stopBits)
{
    bool accepted = false;
    if (stopBits == 1 || stopBits == 2) {
        portSettings.StopBits = stopBits == 1 ? STOP_1 : STOP_2;
        if (port)
            port->setStopBits(portSettings.StopBits);
        accepted = true;
    }
    return accepted;
}

void SerialLink::setEsc32Mode(bool mode) {
    mode_port = mode;
    if ( mode_port ) {
        this->rows = 0;
        this->cols = 0;
        firstRead = 1;
    }
}

void SerialLink::setReconnectDelayMs(const quint16 &ms)
{
    m_reconnectDelayMs = ms;
}

void SerialLink::linkLossExpected(const bool yes)
{
    m_linkLossExpected = yes;
}

//
// Misc. getters
//

QextSerialPort * SerialLink::getPort() {
    return port;
}

int SerialLink::getId()
{
    return id;
}

QString SerialLink::getName()
{
    return name;
}

bool SerialLink::getEsc32Mode() {
    return mode_port;
}

qint64 SerialLink::getNominalDataRate()
{
    return (qint64)portSettings.BaudRate;
}

qint64 SerialLink::getTotalUpstream()
{
    QMutexLocker locker(&statisticsMutex);
    return bitsSentTotal / ((MG::TIME::getGroundTimeNow() - connectionStartTime) / 1000);
}

qint64 SerialLink::getCurrentUpstream()
{
    return 0; // TODO
}

qint64 SerialLink::getMaxUpstream()
{
    return 0; // TODO
}

qint64 SerialLink::getBitsSent()
{
    return bitsSentTotal;
}

qint64 SerialLink::getBitsReceived()
{
    return bitsReceivedTotal;
}

qint64 SerialLink::getTotalDownstream()
{
    QMutexLocker locker(&statisticsMutex);
    return bitsReceivedTotal / ((MG::TIME::getGroundTimeNow() - connectionStartTime) / 1000);
}

qint64 SerialLink::getCurrentDownstream()
{
    return 0; // TODO
}

qint64 SerialLink::getMaxDownstream()
{
    return 0; // TODO
}

bool SerialLink::isFullDuplex()
{
    /* Serial connections are always half duplex */
    return false;
}

int SerialLink::getLinkQuality()
{
    /* This feature is not supported with this interface */
    return -1;
}

QString SerialLink::getPortName()
{
    return porthandle;
}

int SerialLink::getBaudRate()
{
    return getNominalDataRate();
}

int SerialLink::getBaudRateType()
{
    return getNominalDataRate();
}

int SerialLink::getFlowType()
{
    return (int)portSettings.FlowControl;
}

int SerialLink::getParityType()
{
    return (int)portSettings.Parity;
}

int SerialLink::getDataBitsType()
{
    return (int)portSettings.DataBits;
}

int SerialLink::getStopBitsType()
{
    switch (portSettings.StopBits) {
    case STOP_1 :
        return 1;
    case STOP_2 :
        return 2;
    default :
        return -1;
    }
}

//int SerialLink::getDataBits()
//{
//    return (int)portSettings.DataBits;
//}

//int SerialLink::getStopBits()
//{
//    switch (portSettings.StopBits) {
//    case STOP_1 :
//    case STOP_1_5 :
//        return 1;
//    case STOP_2 :
//        return 2;
//    }
//}
