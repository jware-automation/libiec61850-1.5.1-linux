/*
 *  server_example_basic_io.c
 *
 *  - How to use simple control models
 *  - How to serve analog measurement data
 *  - Using the IedServerConfig object to configure stack features
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "static_model.h"
#include "goose_receiver.h"
#include "goose_subscriber.h"
#include "mms_value.h"
#include "goose_publisher.h"

/* import IEC 61850 device model created from SCL-File */
extern IedModel iedModel;

static int running = 0;
static IedServer iedServer = NULL;
static char *gooseInterface = NULL;

void
sigint_handler(int signalId)
{
    running = 0;
}

static void
writeLog(const char *format, ...)
{
    FILE *fptr;
    fptr = fopen("gooseLog.txt", "a");
    va_list args;
    va_start(args, format);
    vfprintf(fptr, format, args);
    va_end(args);
    fclose(fptr);
}

static uint8_t
getReceiverId(const char *iedName)
{
    char *iedIPLastDigitString;
    const char *iedIPString = getenv(iedName);
    char *token = strtok(iedIPString, ".");
    while (token != NULL)
    {
        // printf(" %s\n", token); // printing each token
        iedIPLastDigitString = token;
        token = strtok(NULL, ".");
    }
    uint8_t iedLastDigit = atoi(iedIPLastDigitString);
    return iedLastDigit;
}

static void
publishGooseMsg (const char *iedName, int Ied, MmsValue* status)
{
    LinkedList dataSetValues = LinkedList_create();

    LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(Ied));
    LinkedList_add(dataSetValues, status);

    CommParameters gooseCommParameters;
    uint8_t receiverIed;

    gooseCommParameters.appId = 1000;

    uint8_t myMac[6];
    
    // Ethernet_getInterfaceMACAddress(gooseInterface, myMac);
    char *iedIPLastDigitString;
    const char *iedIPString = getenv(iedName);
    char *iedIPCopy = strdup(iedIPString);
    char *token = strtok(iedIPCopy, ".");
    uint8_t j = 2;
    myMac[0] = 0x02;
    myMac[1] = 0x42;
    while (token != NULL)
    {
        // printf(" %s\n", token); // printing each token
        iedIPLastDigitString = token;
        myMac[j] = atoi(iedIPLastDigitString);
        token = strtok(NULL, ".");
        j++;
    }
    free(iedIPCopy);

    writeLog("Publishimg goose message:\n  Ied:%d\n", Ied);

    writeLog("  dstMac: %02X:%02X:%02X:%02X:%02X:%02X\n", myMac[0],myMac[1],myMac[2],myMac[3],myMac[4],myMac[5]);
    writeLog("  gooseInterface: %s\n", gooseInterface);
    // writeLog("  MAC address: ");
    // for (int i = 0; i < 5; i++) {
    //     writeLog("%d", myMac[i]);
    //     gooseCommParameters.dstAddress[i] = myMac[i];
    //     if (i < 5) {
    //         writeLog(":"); // Print a colon between elements, except for the last one
    //     }
    // }
    // writeLog("\n");

    gooseCommParameters.dstAddress[0] = myMac[0];
    gooseCommParameters.dstAddress[1] = myMac[1];
    gooseCommParameters.dstAddress[2] = myMac[2];
    gooseCommParameters.dstAddress[3] = myMac[3];
    gooseCommParameters.dstAddress[4] = myMac[4];

    gooseCommParameters.dstAddress[5] = myMac[5];
    gooseCommParameters.vlanId = 0;
    gooseCommParameters.vlanPriority = 4;

    GoosePublisher publisher = GoosePublisher_create(&gooseCommParameters, gooseInterface);

    if (publisher) {
        GoosePublisher_setGoCbRef(publisher, "simpleIOGenericIO/LLN0$GO$gcbAnalogValues");
        GoosePublisher_setConfRev(publisher, 1);
        GoosePublisher_setDataSetRef(publisher, "simpleIOGenericIO/LLN0$AnalogValues");
        GoosePublisher_setTimeAllowedToLive(publisher, 500);

        int i = 0;

        for (i = 0; i < 4; i++) {
            Thread_sleep(1000);

            if (i == 3) {
                /* now change dataset to send an invalid GOOSE message */
                LinkedList_add(dataSetValues, MmsValue_newBoolean(true));
                int result = GoosePublisher_publish(publisher, dataSetValues);
                printf("published %d\n", result);
                writeLog("published %d\n", result);
            }
            else {
                if (GoosePublisher_publish(publisher, dataSetValues) == -1) {
                    printf("Error sending message!\n");
                    writeLog("Error sending message!\n");
                }
                else {
                    printf("published success\n");
                    writeLog("published success\n");
                }
            }
        }

        GoosePublisher_destroy(publisher);
    }
    else {
        writeLog("Failed to create GOOSE publisher. Reason can be that the Ethernet interface doesn't exist or root permission are required.\n");
        printf("Failed to create GOOSE publisher. Reason can be that the Ethernet interface doesn't exist or root permission are required.\n");
    }
}

void
gooseListener(GooseSubscriber subscriber, void* parameter)
{
    writeLog("GOOSE event:\n");
    writeLog("  vlanTag: %s\n", GooseSubscriber_isVlanSet(subscriber) ? "found" : "NOT found");
    if (GooseSubscriber_isVlanSet(subscriber))
    {
        writeLog("    vlanId: %u\n", GooseSubscriber_getVlanId(subscriber));
        writeLog("    vlanPrio: %u\n", GooseSubscriber_getVlanPrio(subscriber));
    }
    writeLog("  appId: %d\n", GooseSubscriber_getAppId(subscriber));
    uint8_t macBuf[6];
    GooseSubscriber_getSrcMac(subscriber,macBuf);
    writeLog("  srcMac: %02X:%02X:%02X:%02X:%02X:%02X\n", macBuf[0],macBuf[1],macBuf[2],macBuf[3],macBuf[4],macBuf[5]);
    GooseSubscriber_getDstMac(subscriber,macBuf);
    writeLog("  dstMac: %02X:%02X:%02X:%02X:%02X:%02X\n", macBuf[0],macBuf[1],macBuf[2],macBuf[3],macBuf[4],macBuf[5]);
    writeLog("  goId: %s\n", GooseSubscriber_getGoId(subscriber));
    writeLog("  goCbRef: %s\n", GooseSubscriber_getGoCbRef(subscriber));
    writeLog("  dataSet: %s\n", GooseSubscriber_getDataSet(subscriber));
    writeLog("  confRev: %u\n", GooseSubscriber_getConfRev(subscriber));
    writeLog("  ndsCom: %s\n", GooseSubscriber_needsCommission(subscriber) ? "true" : "false");
    writeLog("  simul: %s\n", GooseSubscriber_isTest(subscriber) ? "true" : "false");
    writeLog("  stNum: %u sqNum: %u\n", GooseSubscriber_getStNum(subscriber),
            GooseSubscriber_getSqNum(subscriber));
    writeLog("  timeToLive: %u\n", GooseSubscriber_getTimeAllowedToLive(subscriber));

    uint64_t timestamp = GooseSubscriber_getTimestamp(subscriber);

    writeLog("  timestamp: %u.%u\n", (uint32_t) (timestamp / 1000), (uint32_t) (timestamp % 1000));
    writeLog("  message is %s\n", GooseSubscriber_isValid(subscriber) ? "valid" : "INVALID");

    MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);

    MmsValue* iValue = MmsValue_getElement(values, 0);
    MmsValue* statusValue = MmsValue_getElement(values, 1);

    int32_t iedIndex = MmsValue_toInt32(iValue);
    bool status = MmsValue_getBoolean(statusValue);

    if (iedIndex == 10)
    {
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_NamPlt_IED7S, statusValue);
    }
    

    writeLog("  IED Index: %d\n", iedIndex);
    writeLog("  IED Status: %d\n", status);

    char buffer[1024];

    MmsValue_printToBuffer(values, buffer, 1024);

    writeLog("  AllData: %s\n", buffer);
}

static ControlHandlerResult
controlHandlerForBinaryOutput(ControlAction action, void* parameter, MmsValue* value, bool test)
{
    if (test)
        return CONTROL_RESULT_FAILED;

    if (MmsValue_getType(value) == MMS_BOOLEAN) {
        printf("received binary control command: ");

        if (MmsValue_getBoolean(value))
            printf("on\n");
        else
            printf("off\n");
    }
    else
        return CONTROL_RESULT_FAILED;

    uint64_t timeStamp = Hal_getTimeInMs();

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO1) {
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_NamPlt_IED7S, value);
        // publishGooseMsg(getReceiverId("SERVER_IED1"), 2, value);
        // publishGooseMsg(getReceiverId("SERVER_IED16"), 2, value);
        publishGooseMsg("SERVER_IED1", 2, value);
        publishGooseMsg("SERVER_IED16", 2, value);

        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1_t, timeStamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1_stVal, value);
    }

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO2) {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2_t, timeStamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2_stVal, value);
    }

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO3) {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3_t, timeStamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3_stVal, value);
    }

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO4) {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_t, timeStamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_stVal, value);
    }

    return CONTROL_RESULT_OK;
}


static void
connectionHandler (IedServer self, ClientConnection connection, bool connected, void* parameter)
{
    if (connected)
        printf("Connection opened\n");
    else
        printf("Connection closed\n");
}

static void
rcbEventHandler(void* parameter, ReportControlBlock* rcb, ClientConnection connection, IedServer_RCBEventType event, const char* parameterName, MmsDataAccessError serviceError)
{
    printf("RCB: %s event: %i\n", ReportControlBlock_getName(rcb), event);

    if ((event == RCB_EVENT_SET_PARAMETER) || (event == RCB_EVENT_GET_PARAMETER)) {
        printf("  param:  %s\n", parameterName);
        printf("  result: %i\n", serviceError);
    }

    if (event == RCB_EVENT_ENABLE) {
        char* rptId = ReportControlBlock_getRptID(rcb);
        printf("   rptID:  %s\n", rptId);
        char* dataSet = ReportControlBlock_getDataSet(rcb);
        printf("   datSet: %s\n", dataSet);

        free(rptId);
        free(dataSet);
    }
}

int
main(int argc, char** argv)
{
    int tcpPort = 102;

    if (argc > 1) {
        tcpPort = atoi(argv[1]);
    }

    if (argc > 2)
        gooseInterface = argv[2];
    else
        gooseInterface = getenv("ETH_INTERFACE");

    printf("Using interface %s for goose\n", gooseInterface);

    printf("Using libIEC61850 version %s\n", LibIEC61850_getVersionString());

    /* Create new server configuration object */
    IedServerConfig config = IedServerConfig_create();

    /* Set buffer size for buffered report control blocks to 200000 bytes */
    IedServerConfig_setReportBufferSize(config, 200000);

    /* Set stack compliance to a specific edition of the standard (WARNING: data model has also to be checked for compliance) */
    IedServerConfig_setEdition(config, IEC_61850_EDITION_2);

    /* Set the base path for the MMS file services */
    IedServerConfig_setFileServiceBasePath(config, "./vmd-filestore/");

    /* disable MMS file service */
    IedServerConfig_enableFileService(config, false);

    /* enable dynamic data set service */
    IedServerConfig_enableDynamicDataSetService(config, true);

    /* disable log service */
    IedServerConfig_enableLogService(config, false);

    /* set maximum number of clients */
    IedServerConfig_setMaxMmsConnections(config, 2);

    /* Create a new IEC 61850 server instance */
    iedServer = IedServer_createWithConfig(&iedModel, NULL, config);

    /* configuration object is no longer required */
    IedServerConfig_destroy(config);

    /* set the identity values for MMS identify service */
    IedServer_setServerIdentity(iedServer, "MZ", "basic io", "1.4.2");

    /* Install handler for operate command */
    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1,
            (ControlHandler) controlHandlerForBinaryOutput,
            IEDMODEL_GenericIO_GGIO1_SPCSO1);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2,
            (ControlHandler) controlHandlerForBinaryOutput,
            IEDMODEL_GenericIO_GGIO1_SPCSO2);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3,
            (ControlHandler) controlHandlerForBinaryOutput,
            IEDMODEL_GenericIO_GGIO1_SPCSO3);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4,
            (ControlHandler) controlHandlerForBinaryOutput,
            IEDMODEL_GenericIO_GGIO1_SPCSO4);

    IedServer_setConnectionIndicationHandler(iedServer, (IedConnectionIndicationHandler) connectionHandler, NULL);

    IedServer_setRCBEventHandler(iedServer, rcbEventHandler, NULL);

    /* By default access to variables with FC=DC and FC=CF is not allowed.
     * This allow to write to simpleIOGenericIO/GGIO1.NamPlt.vendor variable used
     * by iec61850_client_example1.
     */
    IedServer_setWriteAccessPolicy(iedServer, IEC61850_FC_DC, ACCESS_POLICY_ALLOW);

    /* MMS server will be instructed to start listening for client connections. */
    IedServer_start(iedServer, tcpPort);

    if (!IedServer_isRunning(iedServer)) {
        printf("Starting server failed (maybe need root permissions or another server is already using the port)! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    // goose subscriber-----------------------------------------------------------------------------
    GooseReceiver gooseReceiver = GooseReceiver_create();
    GooseReceiver_setInterfaceId(gooseReceiver, gooseInterface);

    GooseSubscriber subscriber = GooseSubscriber_create("", NULL);
    GooseSubscriber_setObserver(subscriber);
    GooseSubscriber_setListener(subscriber, gooseListener, NULL);

    GooseReceiver_addSubscriber(gooseReceiver, subscriber);
    GooseReceiver_start(gooseReceiver);

    if (!GooseReceiver_isRunning(gooseReceiver)) {
        printf("Failed to start GOOSE subscriber. Reason can be that the Ethernet interface doesn't exist or root permission are required.\n");

        GooseReceiver_stop(gooseReceiver);
        GooseReceiver_destroy(gooseReceiver);
    }
    // Goose subscriber ends------------------------------------------------------------------------

    running = 1;

    signal(SIGINT, sigint_handler);

    float t = 0.f;

    while (running) {
        uint64_t timestamp = Hal_getTimeInMs();

        t += 0.1f;

        float an1 = sinf(t);
        float an2 = sinf(t + 1.f);
        float an3 = sinf(t + 2.f);
        float an4 = sinf(t + 3.f);

        Timestamp iecTimestamp;

        Timestamp_clearFlags(&iecTimestamp);
        Timestamp_setTimeInMilliseconds(&iecTimestamp, timestamp);
        Timestamp_setLeapSecondKnown(&iecTimestamp, true);

        /* toggle clock-not-synchronized flag in timestamp */
        if (((int) t % 2) == 0)
            Timestamp_setClockNotSynchronized(&iecTimestamp, true);

        IedServer_lockDataModel(iedServer);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn1_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn1_mag_f, an1);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn2_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn2_mag_f, an2);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn3_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn3_mag_f, an3);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn4_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn4_mag_f, an4);

        IedServer_unlockDataModel(iedServer);

        Thread_sleep(100);
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);
    // stop goose receiver
    GooseReceiver_stop(gooseReceiver);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);
    GooseReceiver_destroy(gooseReceiver);

    return 0;
} /* main() */
