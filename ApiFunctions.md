Header file: <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/api.h'>SCBW/api.h</a></tt>

GPTP provides many useful functions for modding in the `scbw` namespace. Many of these functions wrap StarCraft's internal functions.






---


# Output Functions #

## `playSound` ##
```
void playSound(u32 sfxId, const CUnit *sourceUnit = nullptr);
```

Plays the given sound from sfxdata.dat, optionally using a unit as the source of the sound.

## `printText` ##
```
void printText(const char* text, u32 color = GameTextColor::White);
```

Prints text to the screen, optionally using the given text color.

## `showErrorMessageWithSfx` ##
```
void showErrorMessageWithSfx(u32 playerId, u32 statTxtId, u32 sfxId);
```

Displays an error message and plays a sound for the player.

### Parameters ###
  * `playerId`: ID of the player who receives the message.
  * `statTxtId`: ID of the error message string in `stat_txt.tbl`.
  * `sfxId`: ID of the sound file in `sfxdata.dat`.



---


# Unit Checks #

## `canBeEnteredBy` ##
```
bool canBeEnteredBy(const CUnit* transport, const CUnit* unit);
```

Checks whether the `transport` (Dropship, Overlord, Shuttle, or Bunker) can load the `unit`. This checks:

  * Whether the `transport` has any status effects (Lockdown, Maelstrom, etc.)
  * If the `transport` and `unit` is owned by the same player
  * If the `transport` has enough cargo space for `unit`.

It does NOT check whether `transport` is a hallucination, or is an Overlord without the transport upgrade, or whether the `unit` itself has any status effects. Note that this function is affected by the following EXE edits in FireGraft:
  * Change bunker size check to organic flag check
  * Remove bunker size check

## `canWeaponTargetUnit` ##
```
bool canWeaponTargetUnit(u8 weaponId, const CUnit *target = nullptr, const CUnit *attacker = nullptr);
```

Checks `weapons.dat` targeting flags to see if the `target` can be hit by the `weaponId`.

### Parameters ###
  * `weaponId`: ID of the weapon in `weapons.dat`. If this is an invalid ID, returns false.
  * `target`: Target unit to test. If `nullptr` is passed, the function checks whether the weapon can target a position on the ground. If `target` is invincible, the function returns false.
  * `attacker`: The attacking unit to use for checking the "friendly units only" flag (e.g. Consume).

## `isUnderDarkSwarm` ##
```
bool isUnderDarkSwarm(const CUnit *unit);
```

Checks if the `unit` is under a Dark Swarm. This does NOT check whether the `unit` is a ground unit or a building.


---


# Graphics and Geometry #

## `getAngle` ##
```
s32 getAngle(s32 xHead, s32 yHead, s32 xTail, s32 yTail);
```

Calculates the angle (in binary radians) of the arrow that starts at (`xTail`, `yTail`) and ends at (`xHead`, `yHead`).

## `getDistanceFast` ##
```
u32 getDistanceFast(s32 x1, s32 y1, s32 x2, s32 y2);
```

StarCraft's internal function used to quickly calculate distances between two points (`x1`, `y1`) and (`x2`, `y2`).

_Warning: This function is inaccurate for long distances._

## `getPolarX, getPolarY` ##
```
s32 getPolarX(s32 distance, u8 angle);
s32 getPolarY(s32 distance, u8 angle);
```

Calculates the X and Y Cartesian coordinates from the given polar coordinates (`distance`, `angle`), using StarCraft's internal data.

Note: `angle` is measured in binary radians, clockwise, starting at the top direction.


---


# Game Information #

## `isBroodWarMode` ##
```
bool isBroodWarMode();
```

Checks whether the game is in Brood War mode (instead of vanilla! StarCraft).

## `isCheatEnabled` ##
```
bool isCheatEnabled(u32 cheatFlag);
```

Checks whether the given cheat flag is enabled.

## `isGamePaused` ##
```
bool isGamePaused();
```
Checks whether the game is paused.

## `isInReplay` ##
```
bool isInReplay();
```

Checks whether the game is in replay mode.


---


# Player Information #

## `isAlliedTo` ##
```
bool isAlliedTo(u8 playerA, u8 playerB);
```

Checks whether `playerB` is recognized by `playerA` as an ally.

_Warning: The opposite may not necessarily be true!_

## `getSupplyRemaining` ##
```
s32 getSupplyRemaining(u8 playerId, u8 raceId);
```

Returns the amount of remaining supply (total available - total used) for the `playerId`, using `raceId` to determine the appropriate race to use. This is affected by the "Food For Thought" cheat flag.

## `getUpgradeLevel` ##
```
u8 getUpgradeLevel(8 playerId, u8 upgradeId);
```

Returns the player's upgrade level of `upgradeId`. Note that this uses `UpgradeId::Enum`, instead of `ScUpgrades::Enum` and `BwUpgrades::Enum`.

## `setUpgradeLevel` ##
```
void setUpgradeLevel(u8 playerId, u8 upgradeId, u8 level);
```

Sets the player's upgrade level of `upgradeId`. Note that this uses `UpgradeId::Enum`, instead of `ScUpgrades::Enum` and `BwUpgrades::Enum`.

## `hasTechResearched` ##
```
bool hasTechResearched(u8 playerId, u16 techId);
```

Checks whether the `playerId` has the `techId` researched.

Note: This uses `TechId::Enum`, instead of `ScTech::Enum` and `BwTech::Enum`.

## `setTechResearchState` ##
```
void setTechResearchState(u8 playerId, u16 techId, bool isResearched);
```

Sets the player's tech research state for `techId` to `isResearched`.

Note: This uses `TechId::Enum`, instead of `ScTech::Enum` and `BwTech::Enum`.


---


# Map Information #

## `getActiveTileAt` ##
```
ActiveTile& getActiveTileAt(s32 x, s32 y);
```

Returns the pointer to the active tile data at (x, y).

## `getGroundHeightAtPos` ##
```
u32 getGroundHeightAtPos(s32 x, s32 y);
```

Returns the elevation of the tile at (x, y). 0 for low terrain, 1 for medium, and 2 for high terrain.


---


# Utility Functions #

## `createUnitAtPos` ##
```
CUnit* createUnitAtPos(u16 unitType, u16 playerId, u32 x, u32 y);
```

Attempts to create a unit at the specified coordinates and return a pointer to it. If the unit cannot be created (e.g. there is no space), this function displays an error message and returns `nullptr` instead. This function is the same one used for creating pre-placed units in UMS maps.

## `getUnitOverlayAdjustment` ##
```
u32 getUnitOverlayAdjustment(const CUnit* const unit);
```

Calculates the images.dat overlay ID adjustment for the given unit. This piece of code is used throughout StarCraft.exe for various status effects, such as Ensnare, Plague, and Defensive Matrix.

## `refreshScreen` ##
```
void refreshScreen(int left, int top, int right, int bottom);
void refreshScreen();
```

Refreshes the screen position within the given rectangle area in pixels. If no arguments are given, the function refreshes the entire screen.

## `randBetween` ##
```
u32 randBetween(u32 min, u32 max);
```

Generates a pseudorandom number between `min` and `max`, inclusive. This is identical to the function used internally by StarCraft, and is guaranteed to generate the same sequence in a replay.

_NOTE: The RNG has a maximum range of 0 - 32767._

## `random` ##
```
u16 random();
```

Generates a pseudorandom number between 0 and 32767, inclusive. This is identical to the function used internally by StarCraft, and is guaranteed to generate the same sequence in a replay.


---


# Needs Research #

## `refreshConsole` ##
```
void refreshConsole();
```

Tells StarCraft to refresh the console (current button set, unit portrait, and status bar). not completely understood.

## `playFrame` ##
```
void playFrame(CImage* image, u16 frameset);
```

Functionally identical to the `playfram` Iscript opcode.

## `setAnywhereLocation` ##
```
void setAnywhereLocation();
```

Sets the data of the Anywhere location.

## `setLocation` ##
```
void setLocation(int locNumber, int left, int top, int right, int bottom, int flags);
```

Sets the data of location `locNumber`.