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

#ifndef SE1_INJECTOR_INCL_INJECTOR_H
#define SE1_INJECTOR_INCL_INJECTOR_H

/* Null pointer */
#define INJECTOR_NULL (0)

/* Library interface */
#ifdef SE1_INJECTOR_EXPORTS
  #define INJECTOR_API __declspec(dllexport)
#else
  #define INJECTOR_API __declspec(dllimport)
#endif

/* Vorbis file callbacks structure */
typedef struct {
  void *read_func;
  void *seek_func;
  void *close_func;
  void *tell_func;
} ov_callbacks;


/* Use C interface in C++ */
#ifdef __cplusplus
extern "C" {
#endif


/* Path to the original library for hooking original methods */
INJECTOR_API extern char INJ_strOriginalLib[256];

/* Load original library */
INJECTOR_API void INJ_LoadLibrary(void);

/* Free original library */
INJECTOR_API int INJ_FreeLibrary(void);

/* Hook methods from the original library */
INJECTOR_API void INJ_HookOriginalMethods(void);


/* Declare methods per library */

/* BOOL Imm_CreateDevice(HINSTANCE &, HWND &) */
INJECTOR_API int Imm_CreateDevice(void *, void *);

INJECTOR_API void Imm_DeleteDevice(void);
INJECTOR_API int Imm_GetProductName(char *, int);
INJECTOR_API int Imm_LoadFile(const char *);
INJECTOR_API void Imm_UnLoadFile(void);
INJECTOR_API void immUnloadFile(void); /* Wrong symbol for "Imm_UnLoadFile" */
INJECTOR_API void Imm_PlayEffect(const char *);
INJECTOR_API void Imm_StopEffect(const char *);
INJECTOR_API void Imm_ChangeGain(const float);


/* e3_API *InitExploration3D(e3_INIT *) */
INJECTOR_API void *InitExploration3D(void *);


INJECTOR_API void alInitLibrary(void);
INJECTOR_API void alEndLibrary(void);

/* ALhandle alOpenInputFile(const char *) */
INJECTOR_API int alOpenInputFile(const char *);

/* ALhandle alOpenDecoder(ALhandle) */
INJECTOR_API int alOpenDecoder(int hFile);

/* ALhandle alOpenSubFile(ALhandle, ALsize, ALsize) */
INJECTOR_API int alOpenSubFile(int hFile, int sOffset, int sSize);

/* ALbool alGetMPXHeader(ALhandle, ALsint32 *, ALsint32 *, ALsint32 *, ALbool *, ALsint32 *) */
INJECTOR_API int alGetMPXHeader(int hFile, int *, int *, int *, int *, int *);

/* void alClose(ALhandle) */
INJECTOR_API void alClose(int);

/* ALsize alRead(ALhandle, void *, ALsize) */
INJECTOR_API int alRead(int, void *, int);

/* void alDecSeekAbs(ALhandle, ALfloat) */
INJECTOR_API void alDecSeekAbs(int, float);

/* ALfloat alDecGetLen(ALhandle) */
INJECTOR_API float alDecGetLen(int);


/* int ov_clear(OggVorbis_File *) */
INJECTOR_API int ov_clear(void *);

/* int ov_open(FILE *, OggVorbis_File *, char *, long) */
INJECTOR_API int ov_open(void *f, void *, char *, long);

/* int ov_open_callbacks(void *, OggVorbis_File *, char *, long, ov_callbacks) */
INJECTOR_API int ov_open_callbacks(void *, void *, char *, long, ov_callbacks);

/* long ov_read(OggVorbis_File *, char *, int, int, int, int, int *) */
INJECTOR_API long ov_read(void *, char *, int, int, int, int, int *);

/* vorbis_info *ov_info(OggVorbis_File *, int) */
INJECTOR_API void *ov_info(void *, int);

/* int ov_time_seek(OggVorbis_File *, double) */
INJECTOR_API int ov_time_seek(void *, double);


/* CGame *GAME_Create(void) */
INJECTOR_API void *GAME_Create(void);


/* GameGUI_interface *GAMEGUI_Create(void) */
INJECTOR_API void *GAMEGUI_Create(void);


#ifdef __cplusplus
};
#endif

#endif /* SE1_INJECTOR_INCL_INJECTOR_H */
