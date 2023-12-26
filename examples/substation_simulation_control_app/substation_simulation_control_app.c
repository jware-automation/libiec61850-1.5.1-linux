/*
 *  server_example1.c
 *
 *  Copyright 2013 Michael Zillgith
 *
 *	This file is part of libIEC61850.
 *
 *	libIEC61850 is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	libIEC61850 is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	See COPYING file for the complete license text.
 */

#include "iec61850_client.h"
#include "iec61850_common.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *concat(char *str1, char *str2)
{
    // Calculate the length of the concatenated string
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int totalLen = len1 + len2 + 1; // +1 for the null terminator

    // Allocate memory for the concatenated string
    char *result = (char *)malloc(totalLen);

    // Check if memory allocation was successful
    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return "failed"; // Exit with an error code
    }

    // Copy the strings and concatenate them
    strcpy(result, str1);
    strcat(result, str2);

    // printf("Concatenated string: %s\n", result);

    // Free the dynamically allocated memory
    // free(result);

    return result;
}

void setValue(char *varName, int val, IedConnection con, IedClientError error, bool isBool)
{
    if (val != 999)
    {
        char *address = concat("simpleIOGenericIO/GGIO1.NamPlt.", varName);

        // simpleIOGenericIO/GGIO1.SPCSO1
        // IEC61850_FC_CO
        //  code for commands
        printf("Writing %s: %d\n", varName, val);
        if (varName == "IED1S" || varName == "IED4S" || varName == "IED7S" || varName == "IED13S" || varName == "IED16S")
        {
            char *commandObjectReference = "simpleIOGenericIO/GGIO1.SPCSO1";
            bool boolVal = false;
            if (val == 1)
            {
                boolVal = true;
            }

            ControlObjectClient control = ControlObjectClient_create(commandObjectReference, con);
            MmsValue *ctlVal = MmsValue_newBoolean(boolVal);
            ControlObjectClient_operate(control, ctlVal, 0);

            // reading the effect back
            MmsValue *value2 = IedConnection_readObject(con, &error, address, IEC61850_FC_DC);
            if (value2 != NULL)
            {
                int fValue = MmsValue_getBoolean(value2);
                printf("%s value: %d\n", varName, fValue);
                MmsValue_delete(value2);
            }
        }
        else
        {
            if (!isBool)
            {
                MmsValue *valueWrite = MmsValue_newFloat(val);
                IedConnection_writeObject(con, &error, address, IEC61850_FC_DC, valueWrite);

                MmsValue_delete(valueWrite);
                MmsValue *value2 = IedConnection_readObject(con, &error, address, IEC61850_FC_DC);
                if (value2 != NULL)
                {
                    float fValue = MmsValue_toFloat(value2);
                    printf("%s value: %f\n", varName, fValue);
                    MmsValue_delete(value2);
                }
            }
            else
            {
                bool boolVal = false;
                if (val == 1)
                {
                    boolVal = true;
                }
                MmsValue *valueWrite = MmsValue_newBoolean(boolVal);
                IedConnection_writeObject(con, &error, address, IEC61850_FC_DC, valueWrite);
                MmsValue_delete(valueWrite);
                MmsValue *value2 = IedConnection_readObject(con, &error, address, IEC61850_FC_DC);
                if (value2 != NULL)
                {
                    int fValue = MmsValue_getBoolean(value2);
                    printf("%s value: %d\n", varName, fValue);
                    MmsValue_delete(value2);
                }
            }
        }
    }
}

void setValues(int IED20T, int IED1V, int IED1I, int IED1T, int IED1S, int IED4S,
               int IED7T, int IED7S, int IED13S, int IED16V, int IED16I, int IED16T, int IED16S)
{
    IedClientError errors[6];
    char *hostname;
    int tcpPort = 102;
    char *IPs[6] = {getenv("SERVER_IED1"), getenv("SERVER_IED4"), getenv("SERVER_IED7"), getenv("SERVER_IED13"), getenv("SERVER_IED16"), getenv("SERVER_IED20")};
    int tcpPorts[6] = {102, 102, 102, 102, 102, 102};
    IedConnection cons[6];

    int i = 0;
    for (int i = 0; i < sizeof(tcpPorts) / sizeof(tcpPorts[0]); i++)
    {
        hostname = "localhost";
        hostname = IPs[i];
        tcpPort = tcpPorts[i];
        // IedClientError error = errors[i];
        cons[i] = IedConnection_create();
        IedConnection_connect(cons[i], &errors[i], hostname, tcpPort);
        if (errors[i] != IED_ERROR_OK)
        {
            printf("Error connecting to IED at IP %s.\n", hostname);
            sleep(2);
            // break;
        }
        else
        {
            printf("Success connecting to IED at IP %s.\n", hostname);
        }
    }

    int iedIndex = 0;
    for (int i = 0; i < 6; i++)
    {
        hostname = "localhost";
        hostname = IPs[iedIndex];
        int port = tcpPorts[iedIndex];
        tcpPort = port;
        // IedConnection con = IedConnection_create();
        // IedConnection_connect(con, &error, hostname, tcpPort);
        IedConnection con = cons[iedIndex];
        // IedClientError error = errors[iedIndex];

        if (errors[iedIndex] == IED_ERROR_OK)
        {
            // printf("Connected Successfully to port: %d\n", iedIndex);

            if (iedIndex == 0)
            {
                setValue("IED1V", IED1V, con, errors[iedIndex], false);
                // setValue("IED1I", IED1I, con, errors[iedIndex], false);
                setValue("IED1T", IED1T, con, errors[iedIndex], false);
            }
            else if (iedIndex == 2)
            {
                setValue("IED7T", IED7T, con, errors[iedIndex], false);
            }
            else if (iedIndex == 4)
            {
                // setValue("IED16V", IED16V, con, errors[iedIndex], false);
                // setValue("IED16I", IED16I, con, errors[iedIndex], false);
                setValue("IED16T", IED16T, con, errors[iedIndex], false);
            }
            else if (iedIndex == 5)
            {
                setValue("IED20T", IED20T, con, errors[iedIndex], false);
            }
            
            // IedConnection_close(con);
        }
        else
        {
            printf("Connected error at %d: %d\n", i, errors[iedIndex]);
        }
        iedIndex++;
        // IedConnection_destroy(con);
    }

    // sleep(2);

    //for status updates
    iedIndex = 0;
    for (int i = 0; i < 6; i++)
    {
        hostname = "localhost";
        hostname = IPs[iedIndex];
        int port = tcpPorts[iedIndex];
        tcpPort = port;
        // IedConnection con = IedConnection_create();
        // IedConnection_connect(con, &error, hostname, tcpPort);
        IedConnection con = cons[iedIndex];
        // IedClientError error = errors[iedIndex];

        if (errors[iedIndex] == IED_ERROR_OK)
        {
            // printf("Connected Successfully to port: %d\n", iedIndex);

            //status update
            if (iedIndex == 0)
            {
                setValue("IED1S", IED1S, con, errors[iedIndex], true);
            }
            else if (iedIndex == 1)
            {
                setValue("IED4S", IED4S, con, errors[iedIndex], true);
            }
            else if (iedIndex == 2)
            {
                setValue("IED7S", IED7S, con, errors[iedIndex], true);
            }
            else if (iedIndex == 3)
            {
                setValue("IED13S", IED13S, con, errors[iedIndex], true);
            }
            else if (iedIndex == 4)
            {
                setValue("IED16S", IED16S, con, errors[iedIndex], true);
            }
            
            IedConnection_close(con);
        }
        else
        {
            printf("Connected error at %d: %d\n", i, errors[iedIndex]);
        }
        iedIndex++;
        IedConnection_destroy(con);
    }
}

int main(int argc, char **argv)
{
    IedClientError error;
    printf("Welcome to Substation Simulation!\n\n");

    char *mode = "1";
    char input[100];

    if (argc > 1)
    {
        mode = argv[1];
        if (strcmp(mode, "1") == 0)
        {
            printf("Starting Nominal Mode...\n");
            setValues(20, 33000, 1000, 19, 1, 1, 18, 1, 1, 230, 20, 17, 1);
        }
        else if (strcmp(mode, "2") == 0)
        {
            printf("Starting Transfomer Overheat Mode...\n");
            // setValues(33000, 0, 18, 1, 1, 0, 25, 1, 0, 0, 17, 1, 100);
            setValues(100, 33000, 0, 18, 999, 999, 25, 999, 999, 0, 0, 17, 999);
            // sleep(2000);
            // setValues(33000, 0, 18, 1, 0, 0, 25, 1, 0, 0, 17, 1, 100);
        }
        else if (strcmp(mode, "3") == 0)
        {
            printf("Starting Abnormal Input Voltage...\n");
            // setValues(35000, 0, 20, 1, 1, 0, 24, 1, 0, 0, 17, 0, 21);
            setValues(21, 35000, 0, 20, 999, 999, 24, 999, 999, 0, 0, 17, 999);
            // sleep(2000);
            // setValues(35000, 0, 20, 1, 0, 0, 24, 0, 0, 0, 17, 0, 21);
        }
        else
        {
            printf("Invalid Input!\n");
            printf("Enter simulation mode: \n (1-nominal, 2-Transfomer overheat, 3-Abnormal Input Voltage)\n");
        }
    }
    else
    {
        while (true)
        {
            printf("Enter simulation mode: \n (1-nominal, 2-Transfomer overheat, 3-Abnormal Input Voltage)\n");
            scanf("%s", input);

            if (strcmp(input, "1") == 0)
            {
                printf("Starting Nominal Mode...\n");
                setValues(20, 33000, 1000, 19, 1, 1, 18, 1, 1, 230, 20, 17, 1);
            }
            else if (strcmp(input, "2") == 0)
            {
                printf("Starting Transfomer Overheat Mode...\n");
                setValues(100, 33000, 0, 18, 999, 999, 25, 999, 999, 0, 0, 17, 999);
                // setValues(33000, 0, 18, 1, 1, 0, 25, 1, 0, 0, 17, 1, 100);
                // sleep(2000);
                // setValues(33000, 0, 18, 1, 0, 0, 25, 1, 0, 0, 17, 1, 100);
            }
            else if (strcmp(input, "3") == 0)
            {
                printf("Starting Abnormal Input Voltage...\n");
                setValues(21, 35000, 0, 20, 999, 999, 24, 999, 999, 0, 0, 17, 999);
                // setValues(35000, 0, 20, 1, 1, 0, 24, 1, 0, 0, 17, 0, 21);
                // sleep(2000);
                // setValues(35000, 0, 20, 1, 0, 0, 24, 0, 0, 0, 17, 0, 21);
            }
            else
            {
                printf("Invalid Input!\n");
            }
        }
    }

    return 0;
} /* main() */
