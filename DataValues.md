GPTP exposes many of StarCraft's internal data values, structures, and arrays in <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/scbwdata.h'>SCBW/scbwdata.h</a></tt> in the form of pointers. Some of these are constants and cannot be modified; others can be modified to achieve various effects.

Note that many data values not documented here are usually reserved for use in hook modules, and should not be used unless you know what you are doing.




---


# Linked Lists #

GPTP exposes pointers to linked lists in StarCraft, allowing you to search for specific units, sprites, and bullet objects.

Using linked lists has two advantages over the (conventional) method of looping over structure tables:
  * Structure tables usually have unused entries, making it necessary to check whether the current entry is a valid (used) entry.
  * Linked lists allow you to loop through objects much more quickly.

## `firstVisibleUnit` ##

Points to the first <tt><a href='CUnit.md'>CUnit</a></tt> object in a linked list of visible units (units that have associated sprite objects).

Looping example:
```
for (CUnit *unit = *firstVisibleUnit; unit; unit = unit->link.next) {
  //Write your own code here
}
```

## `firstHiddenUnit` ##

Points to the first <tt><a href='CUnit.md'>CUnit</a></tt> object in a linked list of hidden units (units that don't have associated sprite objects). This includes units in production, units in transports, units in bunkers, workers inside gas buildings, nuclear missiles in silos, and Queens infesting Command Centers.

Looping example:
```
for (CUnit *unit = *firstHiddenUnit; unit; unit = unit->link.next) {
  //Write your own code here
}
```

## `firstPlayerUnit` ##

Points to a collection of first <tt><a href='CUnit.md'>CUnit</a></tt> objects in a linked list of all units owned by each player.

This example loops over all units owned by `playerId`:
```
for (CUnit *unit = firstPlayerUnit->unit[playerId]; unit; unit = unit->player_link.next) {
  //Write your own code here
}
```

## `firstScannerSweep` ##

Points to the first Scanner Sweep unit in a linked list.

_Note: This data variable needs more research._

## `firstBullet` ##

Points to the first `CBullet` object in a linked list of all `CBullet` objects on the map.

Looping example:
```
for (CBullet *bullet = *firstBullet; bullet; bullet = bullet->next) {
  //Write your own code here
}
```


---


# Structure Tables #

Structure tables are predefined arrays of objects in StarCraft, which provide the conventional way of searching for units, bullets, and sprites. In most cases, linked lists are faster and easier to use, but you can always use structure tables if you wish.

## `unitTable` ##

Points to the array of all units in the game. The size of this table is `UNIT_ARRAY_LENGTH`.

Looping example:
```
for (int i = 0; i < UNIT_ARRAY_LENGTH; ++i) {
  CUnit *unit = &unitTable[i];
  //Check if the unit is not unusable.
  if (unit->mainOrderId != OrderId::Die) {
    //Write your own code here
  }
}
```

## `bulletTable` ##

Points to the array of all bullets in the game. The size of this table is `BULLET_ARRAY_LENGTH`.

Looping example:
```
for (int i = 0; i < BULLET_ARRAY_LENGTH; ++i) {
  CBullet *bullet = &bulletTable[i];
  
  //Write your own code here
}
```

## `spriteTable` ##

Points to the array of all sprites in the game. The size of this table is `SPRITE_ARRAY_LENGTH`.

Looping example:
```
for (int i = 0; i < SPRITE_ARRAY_LENGTH; ++i) {
  CSprite *sprite = &spriteTable[i];
  
  //Write your own code here
}
```


---


# Data Variables #

Data variables can be directly modified in your code without using functions.

## `resources` ##

Points to a structure containing the following data:
  * Amount of minerals and gas owned by each player
  * Amount of total minerals and gas that each player has gained through the current game

This sample code gives 50 bonus minerals to `playerId`, as well as increasing the player's mineral gain score:
```
resources->minerals[playerId] += 50;
resources->cumulativeMinerals[playerId] += 50;
```


---


# Data Constants #

Data constants contain information that should not be modified in a hook. You can always read and use their values, but attempting to change them will usually cause compiler errors, or worse, in-game errors.

## `statTxtTbl` ##

Used to retrieve string entries in `stat_txt.tbl`. Note that string indices in TBL files start at 1.

This example prints string number 251 from `stat_txt.tbl` to the screen:
```
const char *pStr = (*statTxtTbl)->getString(251);
scbw::printText(pStr);
```

## `clientSelectionCount` ##

Stores the number of units currently selected by the player running the current client.

See <tt><a href='#clientSelectionGroup.md'>clientSelectionGroup</a></tt> for example usage.

## `clientSelectionGroup` ##

Stores pointers to the units currently selected by the player running the current client.

Example usage:
```
//Iterate through each selected unit
for (int i = 0; i < *clientSelectionCount; ++i) {
  CUnit *selectedUnit = clientSelectionGroup->units[i];
  //Write your own code here
}
```

## `mouse` ##

Stores the position of the mouse relative to the top left corner of the screen.

This example checks if the mouse is within a 40 &times; 30 rectangle area on the top left corner of the screen:
```
if (mouse->x <= 40 && mouse->y <= 30) {
  /* ... */
}
```

## `screenX, screenY` ##

Stores the position of the top left corner of the screen relative to the map.

This example displays the position of the screen relative to the map:
```
char buffer[100];
sprintf(buffer, "Screen positions: (%d, %d)", *screenX, *screenY);
graphics::drawText(buffer, 10, 10);
```

This example uses `screenX`, `screenY`, and `mouse` to compute the position of the mouse on the map:
```
s32 mouseMapX = mouse->x + *screenX;
s32 mouseMapY = mouse->y + *screenY;
```

## `playerTable` ##

Contains various information (names, player types, race types, and associated forces) of each player in the current game.

This example checks whether player 7 (player ID is 6) is a computer AI Protoss player:
```
if (playerTable[6].type == PlayerType::Computer && playerTable[6].race == RaceId::Protoss) {
  /* ... */
}
```

## `supplyData` ##

Points to an array of structures that contain the race-specific supply usage information for each player. The array must be indexed by `RaceId::Terran`, `RaceId::Zerg`, or `RaceId::Protoss`.

Note that supply counts are 2 times the amount shown in-game! (ex. Marines are actually 2 supply, Zerglings are 1 supply, and Zealots are 4 supply.)

By default, the maximum supply amount for all races and players is 400.

This example retrieves the supply usage information of player 4 (player ID is 3) (for Zerg):
```
u32 zergSupplyUsed      = raceSupply[RaceId::Zerg].used[3];
u32 zergSupplyProvided  = raceSupply[RaceId::Zerg].provided[3];
u32 zergSupplyMax       = raceSupply[RaceId::Zerg].max[3];      //This is always 400 by default
```

## `elapsedTimeSeconds` ##

Contains the elapsed time since starting the current game, measured in seconds (1 second == 15 frames).

Example usage:
```
//Check if at least 5 minutes have passed
if (*elapsedTimeSeconds >= 300) {
  /* ... */
}
```

## `elapsedTimeFrames` ##

Contains the elapsed time since starting the current game, measured in frames.

Example usage:
```
//Check if the game has just been started
if (*elapsedTimeFrames == 0) {
  /* ... */
}
```

## `GAME_TYPE` ##

Contains the flag value for the type of the current game (Melee, Free For All, Use Map Settings, etc.). Should be compared with `GameType::Enum`.

Example usage:
```
if (*GAME_TYPE == GameType::UseMapSettings) {
  /* ... */
} 
```

## `LOCAL_HUMAN_ID` ##

Contains the player ID (actually the player number minus 1) of the player who is running the current StarCraft client. Use this to find the current player ID in multiplayer games.

This example prints the name of the current player:
```
std::string buffer = playerTable[*LOCAL_HUMAN_ID].name;
buffer += " is using GPTP";
scbw::printText(buffer);
```

# DAT File Data #

In addition to data constants and variables, GPTP allows you to read various information from the DAT files used by StarCraft. This data is exposed as arrays indexed by the entry ID in each DAT file. For example, to find the attack range of the Scout's ground weapon:

```
//Find the Scout's ground weapon ID
u8 scoutGndWeapon = Unit::GroundWeapon[UnitId::scout];
//Get the weapon's range
u32 scoutGndRange = Weapon::MaxRange[scoutGndWeapon];

//The above can be combined into a single line of code.
u32 scoutGndRange = Weapon::MaxRange[Unit::GroundWeapon[UnitId::scout]];
```

## `units.dat` ##

The following arrays are available in the `units_dat` namespace, and can be indexed with the `CUnit::id` member variable or the `UnitId::Enum` enumeration (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/enumerations/UnitId.h'>SCBW/enumerations/UnitId.h</a></tt>).

| **Array** | **Type** | **Name in DatEdit** | **Stored Value** |
|:----------|:---------|:--------------------|:-----------------|
| `Graphic` | `u8` | Graphics | <tt><a href='#flingy.dat.md'>flingy.dat</a></tt> entry ID |
| `SubUnit` | `u16` | Subunit 1 | <tt><a href='#units.dat.md'>units.dat</a></tt> entry ID |
| `ConstructionGraphic` | `u32` | Construction | <tt><a href='#images.dat.md'>images.dat</a></tt> entry ID |
| `ShieldsEnabled` | `u8` | Shields Enable | Unchecked = 0, checked otherwise |
| `MaxShieldPoints` | `u16` | Shields | Same as the shield amount displayed in-game |
| `MaxHitPoints` | `s32` | Hit Points | 256 times the HP amount displayed in-game |
| `Elevation` | `u8` | Elevation Level | Number between 0-19 |
| `MovementFlags` | `u8` | (Flags in Other Properties section) | Hovering units = 0xC1 |
| `Rank` | `u8` | Rank/Sublabel | Number between 0-22. For Terran units, the index of the sublabel string in `stat_txt.tbl` is set to `(this value) + 549`. If the value is 0, no sublabel is displayed |
| `ComputerIdleOrder` | `u8` | Computer Idle | <tt><a href='#orders.dat.md'>orders.dat</a></tt> entry ID |
| `HumanIdleOrder` | `u8` | Human Idle | <tt><a href='#orders.dat.md'>orders.dat</a></tt> entry ID |
| `ReturnToIdleOrder` | `u8` | Return to Idle | <tt><a href='#orders.dat.md'>orders.dat</a></tt> entry ID |
| `AttackUnitOrder` | `u8` | Attack Unit | <tt><a href='#orders.dat.md'>orders.dat</a></tt> entry ID |
| `AttackMoveOrder` | `u8` | Attack Move | <tt><a href='#orders.dat.md'>orders.dat</a></tt> entry ID |
| `GroundWeapon` | `u8` | Ground Weapon | <tt><a href='#weapons.dat.md'>weapons.dat</a></tt> entry ID |
| `MaxGroundHits` | `u8` | Ground Max Hits |  |
| `AirWeapon` | `u8` | Air Weapon | <tt><a href='#weapons.dat.md'>weapons.dat</a></tt> entry ID |
| `MaxAirHits` | `u8` | Air Max Hits |  |
| `BaseProperty` | `u32` | (Flags in Advance Properties section) | Use the `UnitProperty::Enum` enumeration to check this flags |
| `SeekRange` | `u8` | Target Acquisition Range | Measured in matrices |
| `SightRange` | `u8` | Sight | Measured in matrices. Values larger than 11 may crash the game. |
| `ArmorUpgrade` | `u8` | Armor Upgrade | <tt><a href='#upgrades.dat.md'>upgrades.dat</a></tt> entry ID |
| `SizeType` | `u8` | Unit Size | 0 = Independent, 1 = Small, 2 = Medium, 3 = Large |
| `ArmorAmount` | `u8` | Vital Statistics > Armor |  |
| `UnitBounds` | `Box16` | Unit Dimensions |  |
| `MineralCost` | `u16` | Build Cost > Minerals |  |
| `GasCost` | `u16` | Build Cost > Gas |  |
| `TimeCost` | `u16` | Build Cost > Time | Measured in frames. |
| `GroupFlags` | `GroupFlag` | StarEdit Availability Flags | Use the `GroupFlag` structure as a bitfield |
| `SupplyProvided` | `u8` | Supply > Provided | 2 times the supply amount displayed in-game |
| `SupplyRequired` | `u8` | Supply > Required | 2 times the supply amount displayed in-game |
| `SpaceRequired` | `u8` | Space > Required | Number between 1-8, or 255 if untransportable |
| `SpaceProvided` | `u8` | Space > Provided | Number between 0-8 |
| `BuildScore` | `u16` | Score > Build |  |
| `DestroyScore` | `u16` | Score > Destroy |  |
| `MapStringId` | `u16` | Map String | `stat_txt.tbl` index of the unit's name |
| `BroodwarOnly` | `u8` | Build Cost > Broodwar | Unchecked = 0, checked otherwise |

## `weapons.dat` ##

The following arrays are available in the `weapons_dat` namespace, and can be indexed with the `WeaponId::Enum` enumeration (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/enumerations/WeaponId.h'>SCBW/enumerations/WeaponId.h</a></tt>).

| **Array** | **Type** | **Name in DatEdit** | **Stored Value** |
|:----------|:---------|:--------------------|:-----------------|
| `Label` | `u16` | Label | `stat_txt.tbl` index of the weapon's name (0 for no label) |
| `FlingyId` | `u32` | Graphics | <tt><a href='#flingy.dat.md'>flingy.dat</a></tt> entry ID |
| `TargetFlags` | `TargetFlag` | (Flags in Target Flags section) | Use the `TargetFlag` structure as a bitfield |
| `MinRange` | `u32` | Weapon Range > Min | Measured in pixels |
| `MaxRange` | `u32` | Weapon Range > Max | Measured in pixels |
| `DamageUpgrade` | `u8` | Damage Properties > Upgrade | <tt><a href='#upgrades.dat.md'>upgrades.dat</a></tt> entry ID |
| `DamageType` | `u8` | Damage Properties > Type | 0 = Independent, 1 = Explosive, 2 = Concussive, 3 = Normal, 4 = Ignore Armor |
| `Behavior` | `u8` | Graphical Properties > Behaviour | Compare with `WeaponBehavior::Enum` enumeration (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/enumerations.h'>SCBW/enumerations.h</a></tt>) |
| `RemoveAfter` | `u8` | Remove After | Measured in frames |
| `ExplosionType` | `u8` | Damage Properties > Effect | Compare with `WeaponEffect::Enum` enumeration (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/enumerations.h'>SCBW/enumerations.h</a></tt>) |
| `InnerSplashRadius` | `u16` | Splash Radii > Inner | Measured in pixels |
| `MediumSplashRadius` | `u16` | Splash Radii > Medium | Measured in pixels |
| `OuterSplashRadius` | `u16` | Splash Radii > Outer | Measured in pixels |
| `DamageAmount` | `16` | Damage Properties > Amount |  |
| `DamageBonus` | `16` | Damage Properties > Bonus |  |
| `Cooldown` | `u8` | Cooldown | Measured in frames |
| `DamageFactor` | `u8` | Damage Properties > Factor |  |
| `AttackDirection` | `u8` | Attack Angle | Measured in brads |
| `LaunchSpin` | `u8` | Launch Spin | Measured in brads |
| `ForwardOffset` | `u8` | Graphical Properties > X Offset | Measured in pixels |
| `VerticalOffset` | `u8` | Graphical Properties > Y Offset | Measured in pixels |

## `flingy.dat` ##

The following arrays are available in the `flingy_dat` namespace.

| **Array** | **Type** | **Name in DatEdit** | **Stored Value** |
|:----------|:---------|:--------------------|:-----------------|
| `SpriteID` | `u16` | Sprite File | <tt><a href='#sprites.dat.md'>sprites.dat</a></tt> entry ID |
| `TopSpeed` | `u32` | Top Speed | 256 times the actual speed (pixels per frame) |
| `Acceleration` | `u16` | Acceleration |  |
| `HaltDistance` | `u32` | Halt Distance |  |
| `TurnSpeed` | `u8` | Turn Radius | Measured in brads per frame |
| `MovementControl` | `u8` | Move Control | 0 = Flingy.dat control<br />1 = Partially mobile, weapon<br />2 = Iscript.bin control |

## `sprites.dat` ##

The following arrays are available in the `sprites_dat` namespace.

| **Array** | **Type** | **Name in DatEdit** | **Stored Value** |
|:----------|:---------|:--------------------|:-----------------|
| `ImageId` | `u16` | Image File | <tt><a href='#images.dat.md'>images.dat</a></tt> entry ID |
| `HpBarSize` | `s8` | Health Bar | Measured in pixels |
| `IsVisible` | `u8` | Is Visible | Unchecked = 0, checked otherwise |

## `images.dat` ##

The following arrays are available in the `images_dat` namespace, and can be indexed with the `ImageId::Enum` enumeration (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/enumerations/ImageId.h'>SCBW/enumerations/ImageId.h</a></tt>).

| **Array** | **Type** | **Name in DatEdit** | **Stored Value** |
|:----------|:---------|:--------------------|:-----------------|
| `IsTurnable` | `u8` | Graphics Turns | Unchecked = 0, checked otherwise |
| `IsClickable` | `u8` | Clickable | Unchecked = 0, checked otherwise |
| `RLE_Function` | `u8` | Drawing Properties > Function |  |
| `Remapping` | `u8` | Drawing Properties > Remapping |  |

## `upgrades.dat` ##

The following arrays are available in the `upgrades_dat` namespace, and can be indexed with the `UpgradeId::Enum` enumeration (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/enumerations/UpgradeId.h'>SCBW/enumerations/UpgradeId.h</a></tt>).

| **Array** | **Type** | **Name in DatEdit** | **Stored Value** |
|:----------|:---------|:--------------------|:-----------------|
| `MineralCostBase` | `u16` | Base Cost > Minerals |  |
| `MineralCostFactor` | `u16` | Factor Cost > Minerals |  |
| `GasCostBase` | `u16` | Base Cost > Gas |  |
| `GasCostFactor` | `u16` | Factor Cost > Gas |  |
| `TimeCostBase` | `u16` | Base Cost > Time | Measured in frames |
| `TimeCostFactor` | `u16` | Factor Cost > Time | Measured in frames |
| `Label` | `u16` | Label | `stat_txt.tbl` index of the upgrade's name (0 for no label) |
| `Race` | `u8` | Race | 0 = Zerg, 1 = Terran, 2 = Protoss, 3 = All |
| `MaxRepeats` | `u8` | Max Repeats |  |

## `techdata.dat` ##

The following arrays are available in the `techdata_dat` namespace, and can be indexed with the `TechId::Enum` enumeration (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/enumerations/TechId.h'>SCBW/enumerations/TechId.h</a></tt>).

| **Array** | **Type** | **Name in DatEdit** | **Stored Value** |
|:----------|:---------|:--------------------|:-----------------|
| `MineralCost` | `u16` | Technology Cost > Minerals |  |
| `GasCost` | `u16` | Technology Cost > Vespene |  |
| `TimeCost` | `u16` | Technology Cost > Time | Measured in frames |
| `EnergyCost` | `u16` | Technology Cost > Energy | Same as energy amount displayed in-game |
| `Label` | `u16` | Label | `stat_txt.tbl` index of the technology's name (0 for no label) |

## `orders.dat` ##

The following arrays are available in the 'orders\_dat' namespace, and can be indexed with the `CUnit::mainOrderId`, `CUnit::secondaryOrderId` member variables or the `OrderId::Enum` enumeration (<tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/enumerations/OrderId.h'>SCBW/enumerations/OrderId.h</a></tt>).

| **Array** | **Type** | **Name in DatEdit** | **Stored Value** |
|:----------|:---------|:--------------------|:-----------------|
| `Label` | `u16` | Order Properties > Label | `stat_txt.tbl` index of the order's name (0 for no label) |
| `UseWeaponTargeting` | `u8` | Use Weapon Targeting | Unchecked = 0, checked otherwise |
| `TechUsed` | `u8` | Order Properties > Energy | <tt><a href='#techdata.dat.md'>techdata.dat</a></tt> entry ID |