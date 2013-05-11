/*==LICENSE==

This file is part of PlasmaImport.
Copyright (C) 2013 Christopher Schmidt

This plugin is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This plugin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Lemuria. If not, see <http://www.gnu.org/licenses/>.

*==LICENSE==*/

#include "PlasmaImport.h"

HINSTANCE hInstance = NULL;
 
BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) 
{
	hInstance = hinstDLL;
	switch(fdwReason) 
    {
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		}
	return(TRUE);
}

__declspec( dllexport ) const TCHAR* LibDescription()
{
   return "Plasma Max Importer";
}
 
__declspec( dllexport ) int LibNumberClasses()
{
  return 1;
}
 
__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
  switch(i)
  {
  case 0:
      return &ClassDescInst;
  default:
      return 0;
  }
}
 
__declspec( dllexport ) ULONG LibVersion()
{
  return Get3DSMAXVersion();
}

__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 1;
}