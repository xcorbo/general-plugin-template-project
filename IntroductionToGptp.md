# What is GPTP? #

The General Plugin Template Project (GPTP) is a sample Visual C++ project for creating StarCraft: Brood War mods. GPTP gives mod makers control over numerous aspects of the StarCraft game engine.

GPTP is built as a plugin file (a DLL with a `*.qdp` extension) that can be loaded with MPQDraft or FireGraft for creating a mod executable file. When the mod executable is run, it calls StarCraft and injects the plugin(s) into the game, at which point the plugin takes over.

GPTP uses hooks to inject and run custom code into `StarCraft.exe`'s memory. From then on, the game will use the injected code for processing units, commands, and events.

_Note: If you have previous experience with GPTP v2.0 or v2.1, check out MigratingFromOlderVersionsOfGptp._

# How to Create Your Own Code #

The core of your plugin logic should go in `hooks/game_hooks.cpp`. This file contains three functions:

  * `onGameStart()`: Called when the game starts, right before loading the map and unit data.
  * `onGameEnd()`: Called when the game ends.
  * `nextFrame()`: Called every time the game is updated, which is typically once per frame.

Nearly all of your plugin's logic should go in `nextFrame()`. You can control and modify the units to achieve your desired effects.

# GPTP Reference #

GPTP provides numerous functions and data structures that allow you to control units, modify graphics, and utilize many aspects of the game itself. See the following reference pages for more information.

  * StructuresAndClasses
  * ApiFunctions
  * DataValues
  * HookModules
  * DrawingShapes
  * [Logging](Logging.md)

# Plugin Examples #

There are some plugin examples available on this wiki.

  * HowToMakeTheHardenedShieldsEffect
  * Tutorial\_EnergyConsumingAttacks