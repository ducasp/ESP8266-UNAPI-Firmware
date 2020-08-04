/*
--
-- UNAPI8266.h
--   ESP8266 UNAPI Implementation.
--   Revision 1.20
--
-- Requires Arduino IDE and ESP8266 libraries
-- Copyright (c) 2019 - 2020 Oduvaldo Pavan Junior ( ducasp@ gmail.com )
-- All rights reserved.
--
-- Redistribution and use of this source code or any derivative works, are
-- permitted provided that the following conditions are met:
--
-- 1. Redistributions of source code must retain the above copyright notice,
--    this list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright
--    notice, this list of conditions and the following disclaimer in the
--    documentation and/or other materials provided with the distribution.
-- 3. Redistributions may not be sold, nor may they be used in a commercial
--    product or activity without specific prior written permission.
-- 4. Source code of derivative works MUST be published to the public.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
-- "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
-- TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
-- PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
-- CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
-- EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
-- PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
-- OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
-- WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
-- OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
-- ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--
*/

#ifndef _UNAPI8266_H
#define _UNAPI8266_H

#define MAX_CMD_DATA_LEN 2148
#define ALLOW_OTA_UPDATE
#define ALLOW_TLS

struct ESPConfig {
  unsigned char ucConfigFileName[8];
  unsigned char ucStructVersion;
  unsigned char ucNagle;
  unsigned char ucAlwaysOn;
  unsigned int uiRadioOffTimer;
  unsigned char ucAutoClock;
  int iGMT;
};

enum CommandTypes {
  NO_COMMAND = 0,
  QUICK_COMMAND = 1,
  REGULAR_COMMAND = 2
};

enum RxDataParserStates {
	RX_PARSER_IDLE = 0,
	RX_PARSER_WAIT_DATA_SIZE,
	RX_PARSER_GET_DATA,
	RX_PARSER_PROCCESS_CMD
};

enum ConnectionStates {
  CONN_CLOSED = 0,
  CONN_UDP = 1,
  CONN_TCP_ACTIVE = 2,
  CONN_TCP_PASSIVE = 3,
  CONN_TCP_TLS_A = 4
};

enum CustomFunctions {
	CUSTOM_F_RESET = 'R',
  CUSTOM_F_RETRY_TX = 'r',
  CUSTOM_F_SCAN = 'S',
  CUSTOM_F_SCAN_R = 's',
  CUSTOM_F_CONNECT_AP = 'A',
  CUSTOM_F_UPDATE_FW = 'U',
  CUSTOM_F_UPDATE_CERTS = 'u',
  CUSTOM_F_GET_VER = 'V',
  CUSTOM_F_START_RS232_UPDATE = 'Z',
  CUSTOM_F_START_RS232_CERT_UPDATE = 'Y',
  CUSTOM_F_BLOCK_RS232_UPDATE = 'z',
  CUSTOM_F_END_RS232_UPDATE = 'E',
  CUSTOM_F_NO_DELAY = 'N',
  CUSTOM_F_DELAY = 'D',
  CUSTOM_F_INITCERTS = 'I',
  CUSTOM_F_WIFI_ON_TIMER_SET = 'T',
  CUSTOM_F_TURN_WIFI_OFF = 'O',
  CUSTOM_F_QUERY_SETTINGS = 'Q',
  CUSTOM_F_QUERY_AUTOCLOCK = 'c',
  CUSTOM_F_SET_AUTOCLOCK = 'C',
  CUSTOM_F_GET_DATETIME = 'G',
  CUSTOM_F_WARMBOOT = 'W',
  CUSTOM_F_HOLDCONNECTION = 'H',
  CUSTOM_F_RELEASECONNECTION = 'h',
  CUSTOM_F_GETAPSTS = 'g',
  CUSTOM_F_QUERY = '?'
};

enum TcpipUnapiFunctions {  
    TCPIP_GET_CAPAB = 1,
    TCPIP_GET_IPINFO = 2,
    TCPIP_NET_STATE = 3,
	TCPIP_SEND_ECHO = 4,
	TCPIP_RCV_ECHO = 5,
	TCPIP_DNS_Q = 6,
  TCPIP_DNS_Q_NEW = 206,
	TCPIP_DNS_S = 7,
	TCPIP_UDP_OPEN = 8,
    TCPIP_UDP_CLOSE = 9,
	TCPIP_UDP_STATE = 10,
    TCPIP_UDP_SEND = 11,
	TCPIP_UDP_RCV = 12,
    TCPIP_TCP_OPEN = 13,
    TCPIP_TCP_CLOSE = 14,
    TCPIP_TCP_ABORT = 15,
    TCPIP_TCP_STATE = 16,
    TCPIP_TCP_SEND = 17,
    TCPIP_TCP_RCV = 18,
	TCPIP_TCP_FLUSH = 19,
	TCPIP_RAW_OPEN = 20,
	TCPIP_RAW_CLOSE = 21,
	TCPIP_RAW_STATE = 22,
	TCPIP_RAW_SEND = 23,
	TCPIP_RAW_RCV = 24,
	TCPIP_CONFIG_AUTOIP = 25,
	TCPIP_CONFIG_IP = 26,
	TCPIP_CONFIG_TTL = 27,
	TCPIP_CONFIG_PING = 28,
    TCPIP_WAIT = 29
};

enum TcpipUnapiGetCapabParam1
{
	TCPIP_GET_CAPAB_FLAGS = 1,
	TCPIP_GET_CAPAB_CONN = 2,
	TCPIP_GET_CAPAB_DGRAM = 3,
  TCPIP_GET_SECONDARY_CAPAB_FLAGS = 4
};

enum TcpipUnapiGetIpInfoParam1
{
  TCPIP_GET_IPINFO_LOCALIP = 1,
  TCPIP_GET_IPINFO_PEERIP = 2,
  TCPIP_GET_IPINFO_SUBNETMASK = 3,
  TCPIP_GET_IPINFO_GATEWAY = 4,
  TCPIP_GET_IPINFO_PRIMDNS = 5,
  TCPIP_GET_IPINFO_SECDNS = 6
};

enum TcpipErrorCodes {
    UNAPI_ERR_OK = 0,			    
    UNAPI_ERR_NOT_IMP,		
    UNAPI_ERR_NO_NETWORK,		
    UNAPI_ERR_NO_DATA,		
    UNAPI_ERR_INV_PARAM,		
    UNAPI_ERR_QUERY_EXISTS,	
    UNAPI_ERR_INV_IP,		    
    UNAPI_ERR_NO_DNS,		    
    UNAPI_ERR_DNS,		    
    UNAPI_ERR_NO_FREE_CONN,	
    UNAPI_ERR_CONN_EXISTS,	
    UNAPI_ERR_NO_CONN,		
    UNAPI_ERR_CONN_STATE,		
    UNAPI_ERR_BUFFER,		    
    UNAPI_ERR_LARGE_DGRAM,	
    UNAPI_ERR_INV_OPER
};

#endif
