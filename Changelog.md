# v2.41 #

## General ##
  * GPTP now supports a basic text file logging mechanism. See [Logging](Logging.md) for details.
  * A new compile-time macro, `countof()`, has been added to definitions.h. This retrieves the full length of an array.

## API Functions ##
  * New functions
    * `CUnit::giveTo(), CUnit::getFirstLoadedUnit(), CUnit::getAirWeapon(), CUnit::getCurrentShieldsInGame(), CUnit::isSubunit(), CUnit::canAttackTarget(), CUnit::isTargetEnemy(), CUnit::isUnitEnemy(), CUnit::isDead()`
    * API functions: `scbw::moveUnit(), scbw::prepareUnitMove(), scbw::checkUnitCollisionPos(), scbw::setUnitPosition(), scbw::refreshUnitAfterMove(), scbw::setInGameLoopState()`
  * Renamed functions
    * `CUnit::getActiveGroundWeapon() -> CUnit::getGroundWeapon()`
    * `AI::unitCanAttack() -> CUnit::hasWeapon()`
    * `AI::getCurrentHpInGame() -> CUnit::getCurrentHpInGame()`
    * `AI::getMaxHpInGame() -> CUnit::getMaxHpInGame()`
    * `AI::getCurrentLifeInGame() -> CUnit::getCurrentLifeInGame()`
  * `CUnit::getLastOwnerId()` has been deprecated. It will be removed in v2.42.

## Data Variables ##
  * `scbwdata.h` has been reorganized and (mostly) properly documented.
  * The syntax for using `statTxtTbl` and `mapStringTbl` has become a bit cleaner. You can use `statTxtTbl->getString()` instead of `(*statTxtTbl)->getString()`.
  * Renamed `weapons_dat::AttackDirection[]` to `weapons_dat::AttackAngle[]`.

## Hook modules ##
  * Added `hooks/attack_priority.cpp`. This hook module controls how units find and acquire attackable targets.

## Bug fixes ##
  * Fixed an off-by-1 bug in `CUnit::getIndex()`.
  * `UnitFinder::search()` now also returns units that are dying (i.e. running the `OrderId::Die` order).
  * Fix a bug in `hooks/cloak_tech_inject.cpp` that caused game crash when the AI attempted to cloak its units.
  * Fixed a bug in `AI/spellcasting.cpp` that caused Queens to become obsessed with Parasite in UMS maps.
  * `scbw::random()` and `scbw::randBetween()` now works properly in `nextFrame()`, thanks to `scbw::setInGameLoopState()`.

# v2.4 #
  * General
    * GPTP has moved to Visual C++ 2010, and now uses several C++11 features, such as lambdas and `static_assert()`. GPTP is now incompatible with Visual C++ 2005/2008. The default project file is created in Visual C++ 2010 Express.
    * GPTP now checks the version number of `StarCraft.exe` and displays a warning message if the player attempts to load the plugin with a version of StarCraft other than 1.16.1.
  * Hooks
    * Several hook modules have been moved into subdirectories for better classification.
      * `hooks\unit_stats\`: `armor_bonus.cpp`, `max_energy.cpp`, `sight_range.cpp`, `weapon_range.cpp`
      * `hooks\interface\`: `weapon_armor_tooltip.cpp`
    * `irradiate.cpp` has been merged into `update_status_effects.cpp`.
    * `energy_regeneration.cpp` and `update_unit_timers.cpp` has been merged into `update_unit_state.cpp`.
    * The default form of the `nextFrame()` function has been cleaned up a little bit. It now uses `elapsedTimeFrames` instead of `firstRun` to print the "Hello, world!" message on game start.
    * `cloak_nearby_units.cpp` now uses a lambda function instead of manually looping through multiple units.
  * SCBW Library
    * <tt><a href='UnitFinder.md'>UnitFinder</a></tt>
      * Once again, <tt><a href='UnitFinder.md'>UnitFinder</a></tt> member functions now accept function pointers, functors, and lambdas as callbacks.
      * `UnitFinder::getNearest()` has been replaced by `UnitFinder::getNearestTarget()`, which emulates SCBW behavior more closely.
    * API functions
      * The <tt><a href='CUnit.md'>CUnit</a></tt> structure header file has been split into two files--`CUnit.h` for member function declarations and `CUnitLayout.h` for structure layout. To use the <tt><a href='CUnit.md'>CUnit</a></tt> structure, just `#include` the `CUnit.h` header file as before.
      * Added <tt><a href='CUnit#isRemorphingBuilding.md'>CUnit::isRemorphingBuilding()</a></tt>.
      * Added <tt><a href='ApiFunctions#getPolarX,_getPolarY.md'>scbw::getPolarX(), scbw::getPolarY()</a></tt>.
      * `CUnit::canBuild()` has been renamed to `CUnit::canMakeUnit()`.
      * `scbw::getRaceId()` has been replaced with `CUnit::getRace()`.
      * `scbw::getStatTxtTblString()` has been removed. You can use `(*statTxtTbl)->getString()` to retrieve strings.
    * Data Variables
      * The namespaces for DAT data arrays were renamed to distinguish them better from class/structure names.
        * `Unit -> units_dat`
        * `Weapon -> weapons_dat`
        * `Flingy -> flingy_dat`
        * `Sprite -> sprites_dat`
        * `Image -> images_dat`
        * `Tech -> techdata_dat`
        * `Upgrade -> upgrades_dat`
        * `Order -> orders_dat`
      * New data variables: <tt><a href='DataValues#GAME_TYPE.md'>GAME_TYPE</a></tt>, <tt><a href='DataValues#elapsedTimeFrames.md'>elapsedTimeFrames</a></tt>.
      * Added `statTxtTbl` and `mapStringTbl` for retrieving TBL strings.
      * The `elapsedTime` data constant has been renamed to <tt><a href='DataValues#elapsedTimeSeconds.md'>elapsedTimeSeconds</a></tt>.
    * Enumerations
      * `GameType::Enum` has been expanded with more game types.
      * `PlayerRace::Enum` has been renamed to `RaceId::Enum`.
    * The `STR()` macro has been moved to <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/definitions.h'>definitions.h</a></tt>
  * Bug fixes
    * Player IDs are now stored as unsigned (as they should be).
    * `CUnit::getName()` now returns the custom unit name (if it exists) in UMS maps.
    * Fixed an integer overflow bug in the Consume hook module.

# v2.39 #
  * Added <tt><a href='CUnit#isValidCaster.md'>CUnit::isValidCaster()</a></tt>.
  * Fixed a bug that prevented casting Feedback on spellcasting units.

# v2.38 #

  * New hook modules
    * Tech Target Check: Determine whether a tech spell can be used on a unit, and if not, what error messages should be used.
    * Fire Weapon: Control how weapons are fired. This affects the behavior of `CUnit::fireWeapon()`.
    * Transfer Tech & Upgrades: Determine which technologies and upgrades are transferred when a unit's ownership is passed to another player via triggers or Mind Control.
  * Library changes
    * Added `scbw::getAngle(), scbw::isInReplay(), scbw::setUpgradeLevel(), scbw::hasTechResearched(), scbw::setTechResearchState(), CImage::setRemapping()`.
    * `scbw::fireUnitWeapon()` has been replaced with `CUnit::fireWeapon()`.
    * Added more enumeration members to `UnitId::Enum` (`SCBW/enumerations/UnitId.h`)
  * Bug fixes
    * Fixed a bug that caused the Spider Mine hooks to crash the game.
    * Fixed a bug that caused Zerg buildings to be completed when attempting to cancel them.

# v2.37 #

  * Library changes
    * Added `CUnit::getSeekRange(), CUnit::orderToIdle()`
    * `CUnit::hasOverlay()` and `CSprite::hasOverlay()` has been replaced with `CUnit::getOverlay()` and `CSprite::getOverlay()`.
    * Added structures and data constants for changing image color remappings.
  * Hook changes
    * Recharge Shields hook now supports modifying the recharge distance.
  * Bug fixes
    * AI: AI High Templars will no longer waste energy on Hallucinations.
    * Psi Field hooks: Fixed a bug that caused a crash when saving a game.
    * Bunker hooks: The weapon firing effect for Marines and Ghosts in bunkers are now displayed properly.
    * Unit Morph hook: Added some missing hook functions.
    * Building Morph hook: Zerg melee AI no longer freezes.

# v2.36 #

  * New hooks
    * Unit spellcasting AI hook: Because this module is quite large, it is placed in a separate directory (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/AI/spellcasting.cpp'>AI/spellcasting.cpp</a></tt>).
    * <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/unit_tooltip.cpp'>hooks/unit_tooltip.cpp</a></tt>: Customize the tooltip text of the weapon, armor, and shield icons in the unit console.
    * <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/bunker_hooks.cpp'>hooks/bunker_hooks.cpp</a></tt>: Control bunker attack overlays for specific unit types.
    * Added hooks for fine-tuning unit/building morph conditions (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/unit_morph.cpp'>hooks/unit_morph.cpp</a></tt> and <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/building_morph.cpp'>hooks/building_morph.cpp</a></tt>).
  * Hook changes
    * The "Damage With" hook module has been renamed to "Weapon Damage" hook module. (File renamed from `damage_with.cpp` to `weapon_damage.cpp`.
  * Library changes
    * [CUnit](CUnit.md)
      * Added: `CUnit::getLastOwnerId(), CUnit::canDetect(), CUnit::getActiveGroundWeapon(), CUnit::isVisibleTo(), CUnit::order(), CUnit::getLoadedUnit(), CUnit::hasLoadedUnit(), CUnit::canBuild(), CUnit::isFrozen(), CUnit::getName()`.
      * `CUnit::canUseTech()` has been slightly tweaked.
    * API functions:
      * `scbw::hasOverlay()` has been replaced with `CUnit::hasOverlay()` and `CSprite::hasOverlay()`.
      * Added `scbw::canWeaponTargetUnit(), scbw::random(), scbw::isUnderDarkSwarm()`.
    * Added: `CSprite::isVisibleTo()`.
    * The UnitFinder class now uses callback interface classes instead of templates for receiving callbacks. This should reduce executable code bloat by a little bit.
  * Bug fix: The Sight Range Limit Extender will no longer crash games.

# v2.35 #

  * New features
    * Hooks
      * Irradiate proc hooks (`hooks/irradiate.cpp`)
      * Cloaking tech check hooks (`hooks/cloak_tech.cpp`)
    * Added member functions: `CUnit::canUseTech(), CUnit::setSecondaryOrder(), CUnit::getFromIndex(), CUnit::getIndex()`
  * Bug fixes
    * Fixed bug in Unit Timers Hook that prevented updating the status effects of units loaded inside transports.
    * Fixed critical bug in UnitFinder that created duplicate search entries

# v2.34 #

  * Dropped official support for Code::Blocks. You can still import GPTP into a new project, though.
  * The hook system has been overhauled from the header-source format to the header-source-injector format.
  * MPQDraftPlugin overhaul:
    * `qdp.cpp` has been replaced with `Plugin.h`, `Plugin.cpp` and `plugin_main.cpp`. This should be unnoticeable for most modders.
    * `qdp.def` has been renamed to `plugin.def`. This should be reflected in the tutorials.
    * `initialize.h` and `configure.h` have been renamed to `initialize.cpp` and `configure.cpp`, respectively.
  * `offsets.h` has been removed and all hardcoded offsets moved to function definitions. This should make writing hooks easier.
  * Graphics changes (`graphics/graphics.h`)
    * Drawing functions have been merged into one function per shape (ex: `drawDotOnMap() / drawDotOnScreen() => drawDot()`).
    * Drawing functions now take an extra parameter of `graphics::CoordType` type. Allowed values are `graphics::ON_SCREEN` (default), `graphics::ON_MAP`, and `graphics::ON_MOUSE`.
  * `SCBW/unit_finder.h` and `SCBW/unit_finder.cpp` have been replaced by the UnitFinder class. See documentation for more info.
  * The following functions have been replaced with:
    * `scbw::createOverlay() => CSprite::createOverlay(), CSprite::createTopOverlay()`
    * `scbw::removeOverlay() => CUnit::removeOverlay(), CSprite::removeOverlay()`
    * `scbw::doWeaponDamage() => CUnit::damageWith()`
  * New functions: `scbw::showErrorMessageWithSfx(), scbw::refreshButtonSet(), CUnit::getMaxWeaponRange(), CUnit::getMaxEnergy(), CUnit::getArmor(), CUnit::getArmorBonus(), CUnit::getSightRange()`
  * New hooks
    * `hooks/harvest.cpp`: Change how much resources are harvested/returned per each worker trip.
  * Bug fixes
    * Psi field sprites become automatically invisible when a selected psi field provider stops providing power.
    * Shapes drawn on the map no longer "lag" when the viewport is scrolled across the map.
  * To avoid confusion, `hooks::updatePsiFieldProvider()` no longer requires a `CUnit*` parameter, and should be called _once_ per frame. It has also been renamed to `hooks::updatePsiFieldProviders()`.

# v2.33 #

  * GPTP now supports drawing dots, lines, boxes, and text directly on the screen and the map. See graphics\graphics.h for details.
  * New hooks:
    * Psi field - make (most) units generate psi fields
    * Arbiter cloaking - configure which units are (not) cloaked by Arbiters
  * Unit finder functions have been moved to a separate file (unit\_finder.h).
  * Unit finder functions now use a separate structure (`UnitFinderResult`) to store search results.
  * Some API functions have been moved to structure members.
  * Several structure members have been renamed/modified.
  * The `memoryPatchX()` functions in tools.h have been merged into a template function.
  * Bug fixes:
    * Fixed a critical bug in Unit::getDistanceToTarget().
    * Fixed small bugs in unit finder functions.

# v2.32 #

  * GPTP now supports Visual C++ out of the box (again). Tested with VC++ Express 2005 and the Platform SDK.
  * Added new hook for controlling which units can fire from inside bunkers.
  * New API functions: `scbw::isAlliedTo(), scbw::isGamePaused()`
  * GPTP now supports StarCraft: Brood War v1.16.1 only.
  * Bug fixes:
    * Fixed a bug that caused Ensnared units to attack faster.

# v2.31 #

  * Fixed a critical bug with doWeaponDamageHook().

# v2.3 #

  * Hooks have been modified:
    * Function definitions of hooks now reside in the hooks\ directory.
To modify any hooks, edit the `*.CPP` files.
    * Plague damage hooks have been replaced by the more powerful `updateUnitStatus()` hook.
  * Fixed missing hooks for setting rally points
  * Changed how the VERSION macro works--see definitions.h for details.
  * Large enumerations are now spread between multiple header files. Don't worry; you can still include enumerations.h to use all of them.
  * Added new enumeration: `ImageId` (`SCBW\enumerations\ImageId.h`)
  * `UNIT::healthPoints` has been renamed to `UNIT::hitPoints`.
  * Other numerous changes that should make modding easier

# v2.23 #
  * Turns out `UNIT::healthPoints` is a signed 32-bit integer. Fixed.
  * Fixed typos:
    * UNIT struct: `statisTime` -> `stasisTime`
    * UnitStatus enum: `GoundedBuilding` -> `GroundedBuilding`
  * The `UNIT::unitId` member has been renamed to UNIT::id`.
  * Added new API functions: `hasPathToPos(), hasPathToTarget(), isTargetWithinRange(), getDistanceFast(), removeUnit()`
  * Added new hooks:
    * Control what actually happens when you set a rally point
    * Control upgrade effects for weapon range and sight range
    * Control which units can detect.

# v2.221 #
  * Hotfixed a problem that caused rally hook to crash.

# v2.22 #
  * JmpCallPatch() has been replaced by jmpPatch() and callPatch(), reflecting the latest set of functions used in BWAPI. See tools.h for a thorough documentation.
  * Many existing hooks have been rewritten to use jmpPatch() and callPatch(). This reduces quite a few lines of unnecessary code.
  * Fixed (well, deleted) a potential bug in stimPacksHpCostToEax2().
  * Fixed a potential bug in energyRegenAmountHook() that could make a unit's energy overflow in some weird cases.
  * Added a new pointer constant, CONSUME\_ENERGY\_GAIN. You can use this pointer to access the energy gain amount set in FireGraft. consumeEnergyGainHook() has been modified to not destroy this value.

# v2.21 #

  * Many enumerations were renamed. I promise I won't rename them again.
  * Added some new API functions and one hook function for controlling unit rally orders.
  * You can now loop through units and bullets using linked lists (using tables is still supported).
  * Some namespaces were renamed.
  * The UNIT structure has been slightly modified.
  * memoryPatch() has been split to memoryPatch1(), memoryPatch2(), and memoryPatch4() to avoid ambiguities.
  * sightLimitExtender should work properly now.

# v2.2 #

  * Massively reorganized header and source files.
  * Added many missing DAT offsets, copied from BWAPI's Offsets.h
  * Added some SCBW's internal structures, also copied from BWAPI
  * Added many enumerations, also copied from BWAPI
  * Added several hook functions discovered by pastelmind (special thanks to Heinermann)
  * You can now generate custom plugin IDs with MakeID.hta. It supports random IDs as well as checksum-based IDs. Just double-click the MakeID.hta file to run the app.