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

#include "Injector.h"

#include <windows.h>

char INJ_strOriginalLib[256] = "< please specify path to the original library >";

/* Original library handle */
static HINSTANCE _hLibrary = INJECTOR_NULL;

void INJ_LoadLibrary(void) {
  _hLibrary = LoadLibraryA(INJ_strOriginalLib);
};

int INJ_FreeLibrary(void) {
  BOOL bResult = FreeLibrary(_hLibrary);
  _hLibrary = INJECTOR_NULL;
  return bResult;
};

/* Retrieve a symbol from the original library */
static void *GetSymbol(const char *strSymbolName)
{
  if (_hLibrary != INJECTOR_NULL) {
    return GetProcAddress(_hLibrary, strSymbolName);
  }

  return INJECTOR_NULL;
};

/* Define methods per injector type */
#if SE1_INJECTOR_TYPE == SE1_INJECTOR_IFEEL

/* Hook original methods */
static void *_pCreate;
static void *_pDelete;
static void *_pName;
static void *_pLoad;
static void *_pUnload;
static void *_pPlay;
static void *_pStop;
static void *_pGain;

void INJ_HookOriginalMethods(void) {
  _pCreate = GetSymbol("Imm_CreateDevice");
  _pDelete = GetSymbol("Imm_DeleteDevice");
  _pName   = GetSymbol("Imm_GetProductName");
  _pLoad   = GetSymbol("Imm_LoadFile");
  _pUnload = GetSymbol("Imm_UnLoadFile"); /* "immUnloadFile" in SE1 */
  _pPlay   = GetSymbol("Imm_PlayEffect");
  _pStop   = GetSymbol("Imm_StopEffect");
  _pGain   = GetSymbol("Imm_ChangeGain");
};

/* Injector methods */
int Imm_CreateDevice(void *hInstance, void *hWnd) {
  return ((int (*)(void *, void *))_pCreate)(hInstance, hWnd);
};

void Imm_DeleteDevice(void) {
  ((void (*)(void))_pDelete)();
};

int Imm_GetProductName(char *strProduct, int iMaxCount) {
  return ((int (*)(char *, int))_pName)(strProduct, iMaxCount);
};

int Imm_LoadFile(const char *fnFile) {
  return ((int (*)(const char *))_pLoad)(fnFile);
};

void Imm_UnLoadFile(void) {
  ((void (*)(void))_pUnload)();
};

void immUnloadFile(void) {
  ((void (*)(void))_pUnload)();
};

void Imm_PlayEffect(const char *pstrEffectName) {
  ((void (*)(const char *))_pPlay)(pstrEffectName);
};

void Imm_StopEffect(const char *pstrEffectName) {
  ((void (*)(const char *))_pStop)(pstrEffectName);
};

void Imm_ChangeGain(const float fGain) {
  ((void (*)(const float))_pGain)(fGain);
};

#elif SE1_INJECTOR_TYPE == SE1_INJECTOR_EXP3D

/* Hook original methods */
static void *_pInit;

void INJ_HookOriginalMethods(void) {
  _pInit = GetSymbol("InitExploration3D");
};

/* Injector methods */
void *InitExploration3D(void *pInitStruct) {
  return ((void *(*)(void *))_pInit)(pInitStruct);
};

#elif SE1_INJECTOR_TYPE == SE1_INJECTOR_GAME

/* Hook original methods */
static void *_pCreate;

void INJ_HookOriginalMethods(void) {
  _pCreate = GetSymbol("GAME_Create");
};

/* Injector methods */
void *GAME_Create(void) {
  return ((void *(*)(void))_pCreate)();
};

#elif SE1_INJECTOR_TYPE == SE1_INJECTOR_GAMEGUI

/* Hook original methods */
static void *_pCreate;

void INJ_HookOriginalMethods(void) {
  _pCreate = GetSymbol("GAMEGUI_Create");
};

/* Injector methods */
void *GAMEGUI_Create(void) {
  return ((void *(*)(void))_pCreate)();
};

#endif /* SE1_INJECTOR_TYPE checks */
