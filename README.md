# Serious Engine 1 Injector

This is a simple library that can inject its own code into games on Serious Engine 1 via replacing optional modules of the engine that are dynamically loaded upon engine initialization.

### Supported injections
- `ImmWrapper.dll` - Obsolete IFeel support
- `eview3d.dll` - Exploration 3D module
- `amp11lib.dll` - MP3 playback support
- `vorbisfile.dll`/`libvorbisfile.dll` - OGG playback support
- `Game.dll`/`GameMP.dll` - Main Game module
- `GameGUI.dll`/`GameGUIMP.dll` - Extra GUI module

## How it works

When the engine starts, it dynamically loads specific libraries and hooks their methods via C API. What this injector does is substitute those methods with its own, which in turn call methods from the original library under a different file name.

This injector also exports its own custom symbols that you may utilize:
- `char INJ_strOriginalLib[256]` - Null-terminated ASCII string that contains a path to the original library for hooking its methods.
- `void INJ_LoadLibrary(void)` - Method for loading a new library if the path to it has been changed.
- `int INJ_FreeLibrary(void)` - Method for freeing the last loaded library. Return value is the same as from Windows' `FreeLibrary()`.
- `void INJ_HookOriginalMethods(void)` - Method for "rehooking" methods from the library. Needs to be called after loading a new one.

### File structure
- `Injector.c` - Main injector code. Defines methods that call original methods.
- `Injector.h` - Main injector interface. Can be used for linking the injector library.
- `Main.c` - Defines injector's entry point and hooks original methods upon loading the injector library.

## Building

This project is written in ANSI C using **Visual Studio 2010** but can be compiled virtually by any C compiler.

Batch script for building using **MSVC**:
```bat
cl.exe Main.c /LD /DINJECTOR_LIB_PATH="\"GameMP.dll\""
```

### Notes
- Don't forget to specify the path to the original library in the `INJ_strOriginalLib` variable using the `INJECTOR_LIB_PATH` macro. It can either be absolute or relative to the executable that loaded the injector.
- If you wish to build the entire working library, you can simply compile `Main.c` source file, since it includes `Injector.c` inline.
- If you wish to just link the injector instead, you need to include the `Injector.h` file and link the static library manually.

## License

To avoid potential issues with linking Serious Engine 1 to this project, **Serious Engine 1 Injector** is licensed under GNU GPL v2 (see LICENSE file).
