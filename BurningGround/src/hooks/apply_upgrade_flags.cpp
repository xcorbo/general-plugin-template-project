#include "apply_upgrade_flags.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"

/// This function is called when creating a new unit.
void applyUpgradeFlagsToNewUnitHook(CUnit* const unit) {
  //Default StarCraft behavior
  using scbw::getUpgradeLevel;

  u8 speedUpgradeLevel = 0, cooldownUpgradeLevel = 0;

  switch (unit->id) {
    case UnitId::vulture:
    case UnitId::jim_raynor_vulture:
      speedUpgradeLevel = getUpgradeLevel(unit->playerId, UpgradeId::IonThrusters);
      break;
    case UnitId::overlord:
      speedUpgradeLevel = getUpgradeLevel(unit->playerId, UpgradeId::PneumatizedCarapace);
      break;
    case UnitId::zergling:
      speedUpgradeLevel = getUpgradeLevel(unit->playerId, UpgradeId::MetabolicBoost);
      cooldownUpgradeLevel = getUpgradeLevel(unit->playerId, UpgradeId::AdrenalGlands);
      break;
    case UnitId::hydralisk:
      speedUpgradeLevel = getUpgradeLevel(unit->playerId, UpgradeId::MuscularAugments);
      break;
    case UnitId::ultralisk:
      speedUpgradeLevel = getUpgradeLevel(unit->playerId, UpgradeId::AnabolicSynthesis);
      break;
    case UnitId::zealot:
      speedUpgradeLevel = getUpgradeLevel(unit->playerId, UpgradeId::LegEnhancements);
      break;
    case UnitId::scout:
      speedUpgradeLevel = getUpgradeLevel(unit->playerId, UpgradeId::GraviticThrusters);
      break;
    case UnitId::shuttle:
      speedUpgradeLevel = getUpgradeLevel(unit->playerId, UpgradeId::GraviticDrive);
      break;
    case UnitId::observer:
      speedUpgradeLevel = getUpgradeLevel(unit->playerId, UpgradeId::GraviticBoosters);
      break;
    case UnitId::Hero_DevouringOne:
      cooldownUpgradeLevel = 1;
      speedUpgradeLevel = 1;
      break;
    case UnitId::Hero_HunterKiller:
    case UnitId::Hero_Yggdrasill:
    case UnitId::Hero_FenixZealot:
    case UnitId::Hero_Mojo:
    case UnitId::Hero_Artanis:
    case UnitId::lurker:
      speedUpgradeLevel = 1;
      break;
    case UnitId::Hero_Torrasque:
      if (scbw::isBroodWarMode())
        speedUpgradeLevel = 1;
      break;
  }

  if (speedUpgradeLevel) {
    unit->status |= UnitStatus::SpeedUpgrade;
    unit->updateSpeed();
  }
  if (cooldownUpgradeLevel) {
    unit->status |= UnitStatus::CooldownUpgrade;
  }
}

/// This function is called when an upgrade is finished, or when transferring a
/// unit's ownership from one player to another (via triggers or Mind Control).
void applyUpgradeFlagsToExistingUnitsHook(const u8 playerId, const u8 upgradeId) {
  //Default StarCraft logic
  bool isSpeedUpgrade = true, isCooldownUpgrade = false;
  u16 validUnitId1 = -1, validUnitId2 = -1;

  switch (upgradeId) {
    case UpgradeId::IonThrusters:
      validUnitId1 = UnitId::vulture;
      validUnitId2 = UnitId::jim_raynor_vulture;
      break;
    case UpgradeId::PneumatizedCarapace:
      validUnitId1 = UnitId::overlord;
      break;
    case UpgradeId::MetabolicBoost:
      validUnitId1 = UnitId::zergling;
      break;
    case UpgradeId::MuscularAugments:
      validUnitId1 = UnitId::hydralisk;
      break;
    case UpgradeId::LegEnhancements:
      validUnitId1 = UnitId::zealot;
      break;
    case UpgradeId::GraviticThrusters:
      validUnitId1 = UnitId::scout;
      break;
    case UpgradeId::GraviticDrive:
      validUnitId1 = UnitId::shuttle;
      break;
    case UpgradeId::GraviticBoosters:
      validUnitId1 = UnitId::observer;
      break;
    case UpgradeId::AnabolicSynthesis:
      validUnitId1 = UnitId::ultralisk;
      validUnitId2 = UnitId::torrasque;
      break;
    case UpgradeId::AdrenalGlands:
      validUnitId1 = UnitId::zergling;
      isSpeedUpgrade = false;
      isCooldownUpgrade = true;
      break;
    default:
      return;
  }

  for (CUnit *unit = firstPlayerUnit[playerId]; unit; unit = unit->playerNext) {
    if (unit->id == validUnitId1 || unit->id == validUnitId2) {
      if (isSpeedUpgrade) {
        unit->status |= UnitStatus::SpeedUpgrade;
        unit->updateSpeed();
      }
      if (isCooldownUpgrade)
        unit->status |= UnitStatus::CooldownUpgrade;
    }
  }
}
