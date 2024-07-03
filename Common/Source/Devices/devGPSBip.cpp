/*
 * LK8000 Tactical Flight Computer -  WWW.LK8000.IT
 * Released under GNU/GPL License v.2 or later
 * See CREDITS.TXT file for authors and copyrights
 *
 * File:   devGPSBip.cpp
 * Author: Bruno de Lacheisserie
 *
 * Created on 7 juin 2016
 */

#include "externs.h"
#include "devGPSBip.h"
#include <functional>
#include <array>
#include "DeviceRegister.h"
#include "devGeneric.h"

namespace {

  const TCHAR* DeviceNameList[] = {
    _T("LK8EX1"),
    _T("C-Probe"),
  };

  std::array<DeviceDescriptor_t, std::size(DeviceNameList)> DeviceDesciptorList;

  BOOL ParseNMEA(DeviceDescriptor_t *d, const char *String, NMEA_INFO *GPS_INFO) {
    for(auto& Dev : DeviceDesciptorList) {
      if(Dev.ParseNMEA && Dev.ParseNMEA(d, String, GPS_INFO)) {
        // this device send GPS data only when fix is valid.
        d->nmeaParser.connected = true;
        return TRUE;
      }
    }
    return FALSE;
  }

void Install(DeviceDescriptor_t* d, const TCHAR* szDeviceName) {
  genInstall(d); // install Generic driver callback first

  static_assert(DeviceDesciptorList.size() == std::size(DeviceNameList));

  auto ItOut = std::begin(DeviceDesciptorList);
  for(auto DevName : DeviceNameList) {
    auto& descriptor = *(ItOut++);
    descriptor.InitStruct(d->PortNumber);

    const DeviceRegister_t* pDev = GetRegisteredDevice(DevName);
    if (pDev) {
      pDev->Installer(&descriptor);
    }
  }

  _tcscpy(d->Name, szDeviceName);
  d->ParseNMEA = ParseNMEA;
}

} // namespace

namespace Stodeus {

void GPSBipInstall(DeviceDescriptor_t* d) {
  Install(d, GPSBip);
}

void UltraBipInstall(DeviceDescriptor_t* d) {
  Install(d, UltraBip);
}

void BlueBipInstall(DeviceDescriptor_t* d) {
  Install(d, BlueBip);
}

} // Stodeus