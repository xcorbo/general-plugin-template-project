# How GPTP Works #

The General Plugin Template Project is a sample MPQDraft plugin coupled with a system of hook modules.

## MPQDraft Plugin ##

The MPQDraft plugin source code lives in the root directory. The `Plugin` class, which implements the IMPQDraftPlugin interface, is spread across `Plugin.h`, `Plugin.cpp`, `configure.cpp`, and `initialize.cpp`.

`initialize.cpp` contains the `Plugin::InitializePlugin()` function, which handles all hook injections. If you want to make additional memory patches or injections, they must be done here.

`configure.cpp` contains the `Plugin::Configure()` function, which is called when the user (usually a modder) clicks the 'Configure' button in MPQDraft or FireGraft. By default, GPTP displays a simple informative message box.

GPTP does not utilize MPQDraft's plugin module feature, which allows plugins to read files within the SEMPQ. For more information, see `MPQDraftPlugin.h`.

## Hooks ##

A hook is a piece of code that is injected into another process, i.e. `StarCraft.exe`. This is also known as a [code cave](http://www.codeproject.com/Articles/20240/The-Beginners-Guide-to-Codecaves).

In GPTP, hooking functions are grouped into hook modules. Each hook module consists of three types of functions: hook functions, wrapper functions, and injector functions.

A hook function contains the logical part of the code that is executed by the game. A hook module can have multiple hook functions that are closely related.

The wrapper function serves as a bridge between a hook function and StarCraft. It parses function parameters and return values so that the hook would not disrupt the game flow. A hook function may have multiple wrappers if there are many memory positions that need to be hooked.

The injector function is responsible for actually hooking the wrapper function(s) to StarCraft. It overwrites certain parts of the game memory with assembly instructions, causing the game to call the wrapper function(s) instead of running its own code. Each hook module always has exactly one injector function.

The program flow in an unhooked code segment may look like this:

![http://gptp.googlecode.com/svn/wiki/normal_game_flow.png](http://gptp.googlecode.com/svn/wiki/normal_game_flow.png)

After hooking, the program flow will be changed to:

![http://gptp.googlecode.com/svn/wiki/injected_game_flow.png](http://gptp.googlecode.com/svn/wiki/injected_game_flow.png)

### Technical details ###

Wrapper functions are naked functions (`void __declspec(naked)`) with special prolog/epilog code written in inline assembly. Wrappers use `PUSHAD/POPAD` and `PUSHFD/POPFD` instructions to preserve register state between hook function calls. Wrappers also use static local variables for copying data so that the call stack is unaffected.

## Hook Files ##

Source and header files for hooks reside in the `hooks/` subdirectory. Each hook module consists of three files, which follow a specific naming scheme:

  * `[hook module name].cpp`: Hook function source file, meant to be edited by modders.
  * `[hook module name].h`: Contains hook function/injector function declarations. This header file should be included in `initialize.cpp`.
  * `[hook module name]_inject.cpp`: Contains wrapper/injector function definitions. In general, this file should be left as is.

A few hook modules (e.g. `psi_field.cpp`) have more than three files. These files use the `[hook_module_name]_[purpose].cpp` naming scheme. Editing these files is discouraged, unless noted otherwise.

Most hook modules are self-contained; to remove a hook module, simply delete the three files, and remove the call to the injector function in `initialize.cpp`. However, some hook modules depend on other hook modules, and removing one may cause compiler errors.