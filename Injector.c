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
static void *AMP11_pInit;
static void *AMP11_pEnd;
static void *AMP11_pOpenInput;
static void *AMP11_pOpenInputFile;
static void *AMP11_pOpenSubFile;
static void *AMP11_pGetMPXHeader;
static void *AMP11_pClose;
static void *AMP11_pRead;
static void *AMP11_pDecSeekAbs;
static void *AMP11_pDecGetLen;
static void *Vorbis_pClear;
static void *Vorbis_pOpen;
static void *Vorbis_pOpenCallbacks;
static void *Vorbis_pRead;
static void *Vorbis_pInfo;
static void *Vorbis_pTimeSeek;
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

  AMP11_pInit          = GetSymbol("alInitLibrary");
  AMP11_pEnd           = GetSymbol("alEndLibrary");
  AMP11_pOpenInput     = GetSymbol("alOpenInputFile");
  AMP11_pOpenInputFile = GetSymbol("alOpenDecoder");
  AMP11_pOpenSubFile   = GetSymbol("alOpenSubFile");
  AMP11_pGetMPXHeader  = GetSymbol("alGetMPXHeader");
  AMP11_pClose         = GetSymbol("alClose");
  AMP11_pRead          = GetSymbol("alRead");
  AMP11_pDecSeekAbs    = GetSymbol("alDecSeekAbs");
  AMP11_pDecGetLen     = GetSymbol("alDecGetLen");

  Vorbis_pClear         = GetSymbol("ov_clear");
  Vorbis_pOpen          = GetSymbol("ov_open");
  Vorbis_pOpenCallbacks = GetSymbol("ov_open_callbacks");
  Vorbis_pRead          = GetSymbol("ov_read");
  Vorbis_pInfo          = GetSymbol("ov_info");
  Vorbis_pTimeSeek      = GetSymbol("ov_time_seek");

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

/* AMP11Lib methods */
void alInitLibrary(void) {
  ((void (__stdcall *)(void))AMP11_pInit)();
};

void alEndLibrary(void) {
  ((void (__stdcall *)(void))AMP11_pEnd)();
};

int alOpenInputFile(const char *strFileName) {
  return ((int (__stdcall *)(const char *))AMP11_pOpenInput)(strFileName);
};

int alOpenDecoder(int hFile) {
  return ((int (__stdcall *)(int))AMP11_pOpenInputFile)(hFile);
};

int alOpenSubFile(int hFile, int sOffset, int sSize) {
  return ((int (__stdcall *)(int, int, int))AMP11_pOpenSubFile)(hFile, sOffset, sSize);
};

int alGetMPXHeader(int hFile, int *piLayer, int *piVersion, int *piFrequency, int *pbStereo, int *piRate) {
  return ((int (__stdcall *)(int, int *, int *, int *, int *, int *))AMP11_pGetMPXHeader)(hFile, piLayer, piVersion, piFrequency, pbStereo, piRate);
};

void alClose(int hStream) {
  ((void (__stdcall *)(int))AMP11_pClose)(hStream);
};

int alRead(int hStream, void *pvBuffer, int size) {
  return ((int (__stdcall *)(int, void *, int))AMP11_pRead)(hStream, pvBuffer, size);
};

void alDecSeekAbs(int hDecoder, float fSeconds) {
  ((void (__stdcall *)(int, float))AMP11_pDecSeekAbs)(hDecoder, fSeconds);
};

float alDecGetLen(int hDecoder) {
  return ((float (__stdcall *)(int))AMP11_pDecGetLen)(hDecoder);
};

/* Vorbis file methods */
int ov_clear(void *vf) {
  return ((int (__cdecl *)(void *))Vorbis_pClear)(vf);
};

int ov_open(void *f, void *vf, char *initial, long ibytes) {
  return ((int (__cdecl *)(void *, void *, char *, long))Vorbis_pOpen)(f, vf, initial, ibytes);
};

int ov_open_callbacks(void *datasource, void *vf, char *initial, long ibytes, ov_callbacks callbacks) {
  return ((int (__cdecl *)(void *, void *, char *, long, ov_callbacks))Vorbis_pOpenCallbacks)(datasource, vf, initial, ibytes, callbacks);
};

long ov_read(void *vf, char *buffer, int length, int bigendianp, int word, int sgned, int *bitstream) {
  return ((long (__cdecl *)(void *, char *, int, int, int, int, int *))Vorbis_pRead)(vf, buffer, length, bigendianp, word, sgned, bitstream);
};

void *ov_info(void *vf, int link) {
  return ((void *(__cdecl *)(void *, int))Vorbis_pInfo)(vf, link);
};

int ov_time_seek(void *vf, double pos) {
  return ((int (__cdecl *)(void *, double))Vorbis_pTimeSeek)(vf, pos);
};

/* Game methods */
void *GAME_Create(void) {
  return ((void *(*)(void))Game_pCreate)();
};

/* GameGUI methods */
void *GAMEGUI_Create(void) {
  return ((void *(*)(void))GameGUI_pCreate)();
};
