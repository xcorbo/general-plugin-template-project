//Source file for the Weapon Fire hook module.
//This hook controls how weapons are fired.
#include "weapon_fire.h"
#include <SCBW/scbwdata.h>
#include <SCBW/enumerations.h>
#include <SCBW/api.h>


//-------- Helper function declarations. Do NOT modify! ---------//

namespace {
typedef void (__stdcall *GetWeaponHitPosFunc)(const CUnit *unit, s32 *x, s32 *y);
GetWeaponHitPosFunc const getWeaponHitPos = (GetWeaponHitPosFunc) 0x004762C0;
void createBullet(u8 weaponId, const CUnit *source, s16 x, s16 y, u8 attackingPlayer, u8 direction);
} //unnamed namespace


//-------- Actual hook functions --------//

namespace hooks {

//Fires a weapon from the @p unit.
//This hook affects the following iscript opcodes: attackwith, attack, castspell
//This also affects CUnit::fireWeapon().
void fireWeaponHook(CUnit *unit, u8 weaponId) {

  //Retrieve the spawning position for the bullet.
  s32 x, y;

  if (weapons_dat::Behavior[weaponId] == WeaponBehavior::AppearOnTargetUnit) {
    if (!unit->orderTarget.unit)
      return;

    getWeaponHitPos(unit, &x, &y);
  }

  else if (weapons_dat::Behavior[weaponId] == WeaponBehavior::AppearOnTargetSite) {
    x = unit->orderTarget.pt.x;
    y = unit->orderTarget.pt.y;
  }

  else {
    s32 forwardOffset = weapons_dat::ForwardOffset[weaponId];

    x = unit->getX()
        + (forwardOffset * angleDistance[unit->currentDirection1].x / 256);
    y = unit->getY()
        + (forwardOffset * angleDistance[unit->currentDirection1].y / 256)
        - weapons_dat::VerticalOffset[weaponId];
  }

  if (weapons_dat::FlingyId[weaponId]) {
    u8 bulletDirection = unit->currentDirection1;
    //Make Vultures and Lurkers always fire at the target direction
    if (weapons_dat::Behavior[weaponId] == WeaponBehavior::GoToMaxRange
        && unit->orderTarget.unit)
      bulletDirection = scbw::getAngle(
        unit->getX(), unit->getY(),
        unit->orderTarget.unit->getX(), unit->orderTarget.unit->getY());

    createBullet(weaponId, unit, x, y, unit->playerId, bulletDirection);
  }
}

} //hooks


//-------- Helper function definitions. Do NOT modify! --------//

namespace {

const u32 Helper_CreateBullet = 0x0048C260;
void createBullet(u8 weaponId, const CUnit *source, s16 x, s16 y, u8 attackingPlayer, u8 direction) {
  u32 attackingPlayer_ = attackingPlayer, direction_ = direction;
  s32 x_ = x, y_ = y;

  __asm {
    PUSHAD
    PUSH direction_
    PUSH attackingPlayer_
    PUSH y_
    PUSH x_
    MOV EAX, source
    MOVZX ECX, weaponId
    CALL Helper_CreateBullet
    POPAD
  }
}

} //unnamed namespace
