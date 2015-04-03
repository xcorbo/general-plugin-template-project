Here are solutions to some of the problems you might face while making plugins with GPTP.




---


## Why is the .QDP file so big? ##

Most likely you are building the project in **Debug** mode. In Debug mode, the Visual C++ compiler adds lots of debugging-related code that inflates the size of the plugin. It also skips many speed and size optimizations, so that you can edit, re-compile and test changes more quickly. You can change the build mode to **Release**, which will make the plugin smaller and faster at the cost of longer build times.

Note: GPTP uses the C++ `assert()` macro in multiple places to help find potential game-crashing bugs. We recommend using the Debug mode for testing your plugin, and switching to Release mode _only_ when you are ready to release your mod.


---


## Why are my FireGraft EXE Edits not working? ##

This is because GPTP hooks into the same memory positions that FireGraft works on. If a hook function operates on a feature that can also be changed with FireGraft (e.g. speed upgrades), there is a good chance that the hook will conflict with the EXE edit. If you have no intentions of using a hook module, deleting their header/source/injector files will make the EXE edits work again.


---


## Is it okay to edit any source code? ##

For modding, you should usually only edit the source files in the project root directory, as well as those in the `hooks/` and `AI/` directories. The source and header files in `SCBW/` directly reference or emulate the data in StarCraft, so changing them can cause unexpected side effects.