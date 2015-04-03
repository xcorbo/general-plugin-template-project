Structure header files:
  * Functions: <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/structures/CUnit.h'>SCBW/structures/CUnit.h</a></tt>
  * Member variables: <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/structures/CUnitLayout.h'>SCBW/structures/CUnitLayout.h</a></tt>

The `CUnit` structure is used by StarCraft to store all information regarding a unit's state, such as its HP amount, controlling player, Stim Packs timer, and units loaded inside a transport. Because the base structure is quite large, and GPTP provides numerous methods for working with units, the structure is split into two files, `CUnit.h` (for function information) and `CUnitLayout.h` (for member variable information). To use `CUnit`, place the following statement at the top of your code:

```
#include <SCBW/structures/CUnit.h>
```




---


# Methods #

GPTP extends the CUnit structure with many useful methods. All methods are guaranteed to behave exactly as they do in StarCraft, or obey the modified behavior enforced by hook modules.

Note: The methods in GPTP may not necessarily be actual functions within StarCraft, but a custom method that merely emulates a certain behavior.


---


## Unit Stats and Properties ##

### `canDetect` ###
```
bool canDetect() const;
```

Checks if the unit is a clean detector (no Lockdown, Optical Flare, etc.).

_Note: This is affected by the Detector Hook module._ (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/detector.cpp'>hooks/detector.cpp</a></tt>)

### `getActiveGroundWeapon` ###
```
u8 getActiveGroundWeapon() const;
```

Returns the unit's ground weapon ID. If the unit is an unburrowed Lurker, returns `WeaponId::None` instead.

### `getArmor` ###
```
u8 getArmor() const;
```

Returns the total armor amount of this unit (with upgrades). To retrieve a unit's base armor amount, use `units_dat::ArmorAmount[]`.

### `getArmorBonus` ###
```
u8 getArmorBonus() const;
```

Returns the bonus armor amount of this unit (from upgrades). To retrieve a unit's base armor amount, use `units_dat::ArmorAmount[]`.

### `getMaxEnergy` ###
```
u16 getMaxEnergy() const;
```

Returns the maximum energy amount of this unit (with upgrades).

### `getMaxWeaponRange` ###
```
u32 getMaxWeaponRange(u8 weaponId) const;
```

Returns the maximum range of a weapon. The weapon is assumed to be attached to this unit for calculating upgrade effects.

_Note: This is affected by the Weapon Range hook module._

### `getName` ###
```
const char* getName() const;
static const char* getName(u16 unitId);
```

Retrieves the in-game name of this unit. In UMS maps, this returns custom names (if they exist).

The static version of this function retrieves the name of a custom unit ID.

### `getRace` ###
```
RaceId::Enum getRace() const;
static RaceId::Enum getRace(u16 unitId);
```

Returns the race of the current unit. The return value is always one of: `RaceId::Terran, RaceId::Zerg, RaceId::Protoss, RaceId::Neutral`.

The static version of this function retrieves the race of a custom unit ID.

### `getSeekRange` ###
```
u8 getSeekRange() const;
```

Returns the unit's seek range (AKA target acquisition range) in matrices.

_Note: This is affected by the Weapon Range hook module._

### `getSightRange` ###
```
u32 getSightRange(bool isforSpellCasting = false) const;
```

Returns the sight range of this unit (with upgrades). If `isforSpellCasting` is true, also factors in status effects.

### `isFrozen` ###
```
bool isFrozen() const;
```

Checks if the unit is unpowered / lockdowned / stasised / maelstromed.

### `isRemorphingBuilding` ###
```
bool isRemorphingBuilding() const;
```

Checks if the unit is a remorphing building (i.e. is a Lair, Hive, Greater Spire, Sunken or Spore Colony under construction).

### `isValidCaster` ###
```
bool isValidCaster() const;
```

Checks if the unit is a spellcaster (has energy) and not a hallucination.


---


## Positions and Dimensions ##

### `getX, getY` ###
```
u16 getX() const;
u16 getY() const;
```

Returns the X and Y positions of the unit's sprite on the map. Apparently, this is preferred by Blizzard over using `CUnit::position.x` and `CUnit::position.y`.

### `getTileX, getTileY` ###
```
u16 getTileX() const;
u16 getTileY() const;
```

Returns the X and Y positions of the tile that the unit's sprite is on.

### `getLeft, getRight, getTop, getBottom` ###
```
s16 getLeft() const;
s16 getRight() const;
s16 getTop() const;
s16 getBottom() const;
```

Returns the positions of the left, right, top, and bottom edges of the unit's collision box.


---


## Modifying Unit State ##

### `setHp` ###
```
void setHp(s32 hitPoints);
```

Sets the unit's HP, and also updates burning / bleeding graphics and unit strength data (used by the AI). HP is guaranteed not to overflow.

### `damageWith` ###
```
void damageWith(s32 damage, u8 weaponId, CUnit *attacker = nullptr,
  s8 attackingPlayer = -1, s8 direction = 0, u8 damageDivisor = 1);
```

Deals damage to this unit, using a specific weapons.dat ID.

_Note: This function is controlled by the Weapon Damage hook module._ (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/weapon_damage.cpp'>hooks/weapon_damage.cpp</a></tt>)

#### Parameters ####
  * damage: The amount of damage dealt to this unit.
  * weaponId: `weapons.dat` ID to use.
  * attacker: Attacking unit (for increasing kill count)
  * attackingPlayer: Attacking player ID (for increasing kill score)
  * direction: Attacking direction (for shield flicker overlays)
  * damageDivisor: Damage divisor (for calculating splash damage / glave wurm damage)

### `damageHp` ###
```
void damageHp(s32 damage, CUnit *attacker = nullptr, s32 attackingPlayer = -1, bool notify = true);
```

Deals damage directly to unit HP, killing it if possible.

### `reduceDefensiveMatrixHp` ###
```
void reduceDefensiveMatrixHp(s32 amount);
```

Reduces Defensive Matrix by `amount`, removing it if possible.

### `remove` ###
```
void remove();
```

Removes (kills) the unit from the game. To immediately remove a unit without killing it, use:
```
unit->userActionFlags |= 0x4;
unit->remove();
```

### `removeLockdown, removeMaelstrom, removeStasisField` ###
```
void removeLockdown();
void removeMaelstrom();
void removeStasisField();
```

Removes status effects from the unit.


---


## Unit Orders ##

### `orderTo` ###
```
void orderTo(u32 orderId, const CUnit *target = nullptr);
void orderTo(u32 orderId, u16 x, u16 y);
```

Issue a new order to the unit, using either a target unit (`target`) or position (`x, y`). Units ordered with this function are considered to be controlled by the user.

### `orderToIdle` ###
```
void orderToIdle();
```

Causes the unit to go idle.

### `order` ###
```
void order(u8 orderId, u16 x, u16 y, const CUnit *target, u16 targetUnitId, bool stopPreviousOrders);
```

Issues a new order to the unit. In general, <tt><a href='#orderTo.md'>orderTo()</a></tt> should be used instead.

### `setSecondaryOrder` ###
```
void setSecondaryOrder(u8 orderId);
```

Set the secondary order of this unit. Internally used by several hooks.


---


## Graphics and Animations ##

Note: See `CSprite::createOverlay()` and `CSprite::createTopOverlay()` for creating image overlays on units and sprites.

### `getOverlay` ###
```
CImage* getOverlay(u16 imageId) const;
```

Returns the overlay image of this unit's sprite (or its subunit's sprite) that has the given @p imageId. If the image cannot be found, returns `nullptr`.

### `playIscriptAnim` ###
```
void playIscriptAnim(IscriptAnimation::Enum animation);
```

Makes the unit's sprite play the specified Iscript animation entry.

### `removeOverlay` ###
```
void removeOverlay(u32 imageIdStart, u32 imageIdEnd);
void removeOverlay(u32 imageId);
```

Removes the first image overlay found that has an ID value between `imageIdStart` and `imageIdEnd.` The one-parameter version removes the first image overlay with the given `imageId`.


---


## Distances and Terrain ##

### `getDistanceToTarget` ###
```
u32 getDistanceToTarget(const CUnit *target) const;
```

Returns the distance between this unit and the `target`, taking unit collision size in `units.dat` into account.

Internally, this function uses `scbw::getDistanceFast()`.

### `hasPathToPos` ###
```
bool hasPathToPos(u32 x, u32 y) const;
```

Checks whether this unit can reach the target position. This checks only the terrain layout, and does not consider obstacles (units and buildings).

### `hasPathToUnit` ###
```
bool hasPathToUnit(const CUnit *target) const;
```

Checks whether this unit can reach the `target` unit. This checks only the terrain layout, and does not consider obstacles (units and buildings).


---


## DAT Requirements ##

### `canMakeUnit` ###
```
int canMakeUnit(u16 unitId, s8 playerId) const;
```

Checks if the unit can build / train / morph into `unitId`. This checks:
  * If the unit is actually owned by the commanding `playerId`
  * Whether the unit is stunned / disabled.

If the unit can build / train / morph, returns 1. If the tech needs to be researched, returns -1. If the unit cannot use the tech at all, returns 0.

### `canUseTech` ###
```
int canUseTech(u8 techId, s8 playerId) const;
```

Checks the following:
  * If the unit is actually owned by the commanding `playerId`
  * If the unit's owning player has the `techId` researched
  * If the unit has enough energy (or energy cheat is enabled)
  * If the unit is not stunned / is a hallucination / is being built
  * If the unit meets the Use Tech Requirements (editable in FireGraft)

If the unit can use the tech, returns 1. If the tech needs to be researched, returns -1. If the unit cannot use the tech at all, returns 0.


---


## Utility Methods ##

### `fireWeapon` ###
```
void fireWeapon(u8 weaponId) const;
```

Makes the unit use the specified weapon to attack its current target unit stored in the `CUnit::orderTarget.unit` member. This does not affect the unit's weapon cooldown. The spawned weapon sprite obeys the weapon behavior properties in `weapons.dat`.

Note: This function is affected by the Fire Weapon hook module (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/weapon_fire.cpp'>hooks/weapon_fire.cpp</a></tt>)

### `getFromIndex` ###
```
static CUnit* getFromIndex(u16 index);
```

Retrieves the unit pointer by `index` from the unit table (first unit is indexed at 1). If invalid, returns `nullptr` instead.

_Note: This is a static member function, and does NOT require an instance of the class._

### `getIndex` ###
```
u16 getIndex() const;
```

Returns the index of this unit in the unit table. First unit == index 1.

### `getLastOwnerId` ###
```
s8 getLastOwnerId() const;
```

Returns the ID of the last player to own this unit (i.e. the player color of the unit sprite). This is usually the same as `CUnit::playerId`, but if the unit belongs to a defeated player, this returns the correct player ID (instead of 11).

### `getLoadedUnit` ###
```
CUnit* getLoadedUnit(int index) const;
```

Returns the loaded unit at `index` (value between 0-7). If no unit is loaded at the slot, returns `nullptr` instead.

### `hasLoadedUnit` ###
```
bool hasLoadedUnit() const;
```

Checks if this unit has other units loaded inside.

### `isVisibleTo` ###
```
bool isVisibleTo(s8 playerId) const;
```

Checks whether this unit can be seen by `playerId` (i.e. not covered by the fog-of-war and is detectable).

### `updateSpeed` ###
```
void updateSpeed();
```

Updates the unit's actual speed. This function should be called after changing any properties and status effects that affect movement speed.