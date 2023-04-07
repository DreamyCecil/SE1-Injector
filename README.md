# Serious Engine 1 Injector

This is a simple library that can inject its own code into games on Serious Engine 1 via replacing optional modules of the engine that are dynamically loaded upon engine initialization.

### Supported injections
- `ImmWrapper.dll` - Obsolete IFeel support
- `eview3d.dll` - Exploration 3D module
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
- `Injector.c` - Main injector code. Defines methods for the selected module that call original methods.
- `Injector.h` - Main injector interface. Can be used for linking the injector library.
- `InjectorTypes.h` - Defines types of supported injections.
- `Main.c` - Defines injector's entry point and hooks original methods upon loading the injector library.

## Building

This project is written in ANSI C using **Visual Studio 2010** but can be compiled virtually by any C compiler.

By default, the injector is setup to compile for the IFeel module but that can be changed by redefining the `SE1_INJECTOR_TYPE` macro with one of the supported types:
```cpp
#define SE1_INJECTOR_IFEEL   0 /* IFeel (ImmWrapper.dll) */
#define SE1_INJECTOR_EXP3D   1 /* Exploration 3D (eview3d.dll) */
#define SE1_INJECTOR_GAME    2 /* Game library (Game.dll / GameMP.dll) */
#define SE1_INJECTOR_GAMEGUI 3 /* GameGUI library (GameGUI.dll / GameGUIMP.dll) */
```

### Notes
- Don't forget to specify the path to the original library in the `INJ_strOriginalLib` variable that's defined in `Injector.c`. It can either be absolute or relative to the executable that loaded the injector.
- If you're building the injector itself, don't forget to define the `SE1_INJECTOR_EXPORTS` macro in project settings to make it export all the necessary symbols.
- If you're just linking the injector instead, you need to include the `Injector.h` file and link the static library.

## License

To avoid potential issues with linking Serious Engine 1 to this project, **Serious Engine 1 Injector** is licensed under GNU GPL v2 (see LICENSE file).
