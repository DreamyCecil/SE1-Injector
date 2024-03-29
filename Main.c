/* Copyright (c) 2023 Dreamy Cecil
This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

/* NOTE: This source file is for compiling the working library as a whole */

/* Compile injector inline */
#include "Injector.c"

/* Library entry point */
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD ulReason, LPVOID pReserved) {
  if (ulReason == DLL_PROCESS_ATTACH) {
    /* Load original library and hook its methods */
    INJ_LoadLibrary();
    INJ_HookOriginalMethods();
  }

  return TRUE;
};
