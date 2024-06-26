/*
 * LK8000 Tactical Flight Computer -  WWW.LK8000.IT
 * Released under GNU/GPL License v.2 or later
 * See CREDITS.TXT file for authors and copyrights
 *
 * File:   bthapi.h
 * Author: Bruno de Lacheisserie
 *
 */

#ifndef CE_BTHAPI_H
#define	CE_BTHAPI_H

#ifdef _WIN32

typedef ULONGLONG BT_ADDR;

#define NAP_MASK                ((ULONGLONG) 0xFFFF00000000LL)
#define SAP_MASK                ((ULONGLONG) 0x0000FFFFFFFFLL)

#define NAP_BIT_OFFSET          (8 * 4)
#define SAP_BIT_OFFSET          (0)

#define GET_NAP(_bt_addr)       ((USHORT) ((_bt_addr & NAP_MASK) >> NAP_BIT_OFFSET))
#define GET_SAP(_bt_addr)       ((ULONG)  ((_bt_addr & SAP_MASK) >> SAP_BIT_OFFSET))

#define SET_NAP(_nap) (((ULONGLONG) ((USHORT) (_nap))) << NAP_BIT_OFFSET)
#define SET_SAP(_sap) (((ULONGLONG) ((ULONG)  (_sap))) << SAP_BIT_OFFSET)

#define SET_NAP_SAP(_nap, _sap) (SET_NAP(_nap) | SET_SAP(_sap))

#ifndef BTHPROTO_RFCOMM
#define 	BTHPROTO_RFCOMM   0x0003
#endif
#ifndef AF_BTH
#define AF_BTH  32
#endif
#ifndef NS_BTH
#define NS_BTH  16
#endif

struct SOCKADDR_BTH {
    USHORT addressFamily; // Always AF_BTH
    BT_ADDR btAddr; // Bluetooth device address
    GUID serviceClassId; // [OPTIONAL] system will query SDP for port
    ULONG port; // RFCOMM channel or L2CAP PSM
};

#define HCI_HARDWARE_UNKNOWN							0
#define HCI_HARDWARE_NOT_PRESENT						1
#define HCI_HARDWARE_INITIALIZING						2
#define HCI_HARDWARE_RUNNING							3
#define HCI_HARDWARE_SHUTDOWN							4
#define HCI_HARDWARE_ERROR								5

const GUID SerialPortServiceClass_UUID = {0x00001101, 0x0000, 0x1000, {0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB}};

enum BTH_RADIO_MODE {
  BTH_POWER_OFF,
  BTH_CONNECTABLE,
  BTH_DISCOVERABLE
};

#endif
#endif	/* CE_BTHAPI_H */
