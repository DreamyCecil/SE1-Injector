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


/* Hook original methods */
static void *IFeel_pCreate;
static void *IFeel_pDelete;
static void *IFeel_pName;
static void *IFeel_pLoad;
static void *IFeel_pUnload;
static void *IFeel_pPlay;
static void *IFeel_pStop;
static void *IFeel_pGain;
static void *Exp3D_pInit;
static void *Game_pCreate;
static void *GameGUI_pCreate;

void INJ_HookOriginalMethods(void) {
  IFeel_pCreate = GetSymbol("Imm_CreateDevice");
  IFeel_pDelete = GetSymbol("Imm_DeleteDevice");
  IFeel_pName   = GetSymbol("Imm_GetProductName");
  IFeel_pLoad   = GetSymbol("Imm_LoadFile");
  IFeel_pUnload = GetSymbol("Imm_UnLoadFile"); /* "immUnloadFile" in SE1 */
  IFeel_pPlay   = GetSymbol("Imm_PlayEffect");
  IFeel_pStop   = GetSymbol("Imm_StopEffect");
  IFeel_pGain   = GetSymbol("Imm_ChangeGain");

  Exp3D_pInit = GetSymbol("InitExploration3D");

  Game_pCreate = GetSymbol("GAME_Create");

  GameGUI_pCreate = GetSymbol("GAMEGUI_Create");
};


/* Define methods per library */

/* IFeel methods */
int Imm_CreateDevice(void *hInstance, void *hWnd) {
  return ((int (*)(void *, void *))IFeel_pCreate)(hInstance, hWnd);
};

void Imm_DeleteDevice(void) {
  ((void (*)(void))IFeel_pDelete)();
};

int Imm_GetProductName(char *strProduct, int iMaxCount) {
  return ((int (*)(char *, int))IFeel_pName)(strProduct, iMaxCount);
};

int Imm_LoadFile(const char *fnFile) {
  return ((int (*)(const char *))IFeel_pLoad)(fnFile);
};

void Imm_UnLoadFile(void) {
  ((void (*)(void))IFeel_pUnload)();
};

void immUnloadFile(void) {
  ((void (*)(void))IFeel_pUnload)();
};

void Imm_PlayEffect(const char *pstrEffectName) {
  ((void (*)(const char *))IFeel_pPlay)(pstrEffectName);
};

void Imm_StopEffect(const char *pstrEffectName) {
  ((void (*)(const char *))IFeel_pStop)(pstrEffectName);
};

void Imm_ChangeGain(const float fGain) {
  ((void (*)(const float))IFeel_pGain)(fGain);
};

/* Exploration 3D methods */
void *InitExploration3D(void *pInitStruct) {
  return ((void *(*)(void *))Exp3D_pInit)(pInitStruct);
};

/* Game methods */
void *GAME_Create(void) {
  return ((void *(*)(void))Game_pCreate)();
};

/* GameGUI methods */
void *GAMEGUI_Create(void) {
  return ((void *(*)(void))GameGUI_pCreate)();
};
