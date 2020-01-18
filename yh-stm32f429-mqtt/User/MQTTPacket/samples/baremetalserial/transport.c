/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - port to the bare metal environment and serial media specifics
 *******************************************************************************/

/** By the way, this is a nice bare bones example, easier to expand to whatever non-OS
media you might have */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "transport.h"

/**
This simple low-level implementation assumes a single connection for a single thread. Thus, single static
variables are used for that connection.
On other scenarios, you might want to put all these variables into a structure and index via the 'sock'
parameter, as some functions show in the comments
The blocking rx function is not supported.
If you plan on writing one, take into account that the current implementation of
MQTTPacket_read() has a function pointer for a function call to get the data to a buffer, but no provisions
to know the caller or other indicator (the socket id): int (*getfn)(unsigned char*, int)
*/
static transport_iofunctions_t *io = NULL;
static unsigned char *from = NULL;		// to keep track of data sending
static int howmany;				// ditto


void transport_sendPacketBuffernb_start(int sock, unsigned char* buf, int buflen)
{
	from = buf;			// from[sock] or mystruct[sock].from
	howmany = buflen;		// myhowmany[sock] or mystruct[sock].howmany
}

int transport_sendPacketBuffernb(int sock)
{
transport_iofunctions_t *myio = io;	// io[sock] or mystruct[sock].io
int len;

	/* you should have called open() with a valid pointer to a valid struct and 
	called sendPacketBuffernb_start with a valid buffer, before calling this */
	assert((myio != NULL) && (myio->send != NULL) && (from != NULL));
	if((len = myio->send(from, howmany)) > 0){
		from += len;
		if((howmany -= len) <= 0){
			return TRANSPORT_DONE;
		}
	} else if(len < 0){
		return TRANSPORT_ERROR;
	}
	return TRANSPORT_AGAIN;
}

int transport_sendPacketBuffer(const char*server_ip_and_port,unsigned char* buf, int buflen)
{
    u8 ret;
    char end_char[2];
    char connect_server_ip_port_cmd[56];

    first_index_to_read = 21;

    end_char[0] = 0x1A;//
    end_char[1] = '\0';

    ret = UART2_Send_AT_Command("AT+CIPSTATUS","CONNECT OK",3,1);//
    if(ret == 1)//
    {
        ret = UART2_Send_AT_Command("AT+CIPSEND",">",3,1);//
        if(ret == 0)
        {
            return AT_CIPSEND_ERROR;
        }

        UART2_SendU8Array(buf,buflen);
        delays(2);
        ret = UART2_Send_AT_Command_End(end_char,"SEND OK",4,1);
        if(ret == 0)
        {
            return END_CHAR_ERROR;
        }
        return 1;
    }

    ret = UART2_Send_AT_Command("AT+CGATT=1","OK",2,1);//

    if(ret == 0)
    {
        return AT_CGATT_ERROR;
    }

    ret = UART2_Send_AT_Command("AT+CGACT=1,1","OK",2,1);
    if(ret == 0)
    {
        return AT_CGATT1_ERROR;
    }

    memset(connect_server_ip_port_cmd,'\0',56);
    strcpy(connect_server_ip_port_cmd,"AT+CIPSTART=\"TCP\",");
    strcat(connect_server_ip_port_cmd,server_ip_and_port);

    ret = UART2_Send_AT_Command(connect_server_ip_port_cmd,"CONNECT OK",4,1);//
    if(ret == 0)
    {
        return AT_CIPSTART_ERROR;
    }

    ret = UART2_Send_AT_Command("AT+CIPSEND",">",3,1);//
    if(ret == 0)
    {
        return AT_CIPSEND_ERROR;
    }

    UART2_SendU8Array(buf,buflen);
    delays(2);
    ret = UART2_Send_AT_Command_End(end_char,"SEND OK",4,1);//
    if(ret == 0)
    {
         return END_CHAR_ERROR;
    }
    return 1;
}


int transport_getdata(unsigned char*buf, int count)
{
    memcpy(buf,&uart2_recv_buf[first_index_to_read],count);
    first_index_to_read += count;
    return count;
}

int transport_getdatanb(void *sck, unsigned char* buf, int count)
{
//int sock = *((int *)sck); 		/* sck: pointer to whatever the system may use to identify the transport */
transport_iofunctions_t *myio = io;	// io[sock] or mystruct[sock].io
int len;
	
	/* you should have called open() with a valid pointer to a valid struct before calling this */
	assert((myio != NULL) && (myio->recv != NULL));
	/* this call will return immediately if no bytes, or return whatever outstanding bytes we have,
	 upto count */
	if((len = myio->recv(buf, count)) >= 0)
		return len;
	return TRANSPORT_ERROR;
}

/**
return >=0 for a connection descriptor, <0 for an error code
*/
int transport_open(transport_iofunctions_t *thisio)
{
int idx=0;	// for multiple connections, you might, basically turn myio into myio[MAX_CONNECTIONS],

	//if((idx=assignidx()) >= MAX_CONNECTIONS)	// somehow assign an index,
	//	return TRANSPORT_ERROR;
	io = thisio;					// store myio[idx] = thisio, or mystruct[idx].io = thisio, 
	return idx;					// and return the index used
}

int transport_close(int sock)
{
int rc=TRANSPORT_DONE;

	return rc;
}
