﻿#include "update_status_effects.h"
#include "../SCBW/api.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/scbwdata.h"
#include "irradiate.h"

namespace {
//Helper functions that should be used only in this file
u8 getAcidSporeOverlayAdjustment(const CUnit* const unit);
} //unnamed namespace

namespace hooks {

//Detour for UpdateStatusEffects() (AKA RestoreAllUnitStats())
//Original function address: 0x00492F70 (SCBW 1.16.1)
//Note: this function is called every 8 ticks (when unit->cycleCounter reaches 8 == 0)
void updateStatusEffectsHook(CUnit *unit) {
  //Default StarCraft logic

  if (unit->stasisTimer) {
    unit->stasisTimer--;
    if (unit->stasisTimer == 0)
      unit->removeStasisField();
  }

  //익스트랙터는 스팀팩 타이머를 따로 쓰니까 제외
  if (unit->id != UnitId::extractor) {
    if (unit->stimTimer) {
      unit->stimTimer--;
      if (unit->stimTimer == 0)
        unit->updateSpeed();
    }
  }

  if (unit->ensnareTimer) {
    unit->ensnareTimer--;
    if (unit->ensnareTimer == 0) {
      unit->removeOverlay(ImageId::EnsnareOverlay_Small, ImageId::EnsnareOverlay_Large);
      unit->updateSpeed();
    }
  }

  if (unit->defensiveMatrixTimer) {
    unit->defensiveMatrixTimer--;
    if (unit->defensiveMatrixTimer == 0) {
      unit->reduceDefensiveMatrixHp(unit->defensiveMatrixHp);
    }
  }

  if (unit->irradiateTimer) {
    unit->irradiateTimer--;
    doIrradiateDamage(unit);
    if (unit->irradiateTimer == 0) {
      unit->removeOverlay(ImageId::Irradiate_Small, ImageId::Irradiate_Large);
      unit->irradiatedBy = NULL;
      unit->irradiatePlayerId = 8;
    }
  }

  if (unit->lockdownTimer) {
    unit->lockdownTimer--;
    if (unit->lockdownTimer == 0)
      unit->removeLockdown();
  }

  if (unit->maelstromTimer) {
    unit->maelstromTimer--;
    if (unit->maelstromTimer == 0)
      unit->removeMaelstrom();
  }

  if (unit->plagueTimer) {
    unit->plagueTimer--;
    if (!(unit->status & UnitStatus::Invincible)) {
      s32 damage = (Weapon::DamageAmount[WeaponId::Plague] << 8) / 76;
      if (unit->hitPoints > damage)
        unit->damageHp(damage);
    }
    if (unit->plagueTimer == 0)
      unit->removeOverlay(ImageId::PlagueOverlay_Small, ImageId::PlagueOverlay_Large);
  }

  if (unit->isUnderStorm)
    unit->isUnderStorm--;

  u8 previousAcidSporeCount = unit->acidSporeCount;
  for (int i = 0; i <= 8; ++i) {
    if (unit->acidSporeTime[i]) {
      unit->acidSporeTime[i]--;
      if (unit->acidSporeTime[i] == 0)
        unit->acidSporeCount--;
    }
  }
  if (unit->acidSporeCount) {
    u32 acidOverlayId = getAcidSporeOverlayAdjustment(unit) + ImageId::AcidSpores_1_Overlay_Small;
    if (!unit->getOverlay(acidOverlayId)) {
      unit->removeOverlay(ImageId::AcidSpores_1_Overlay_Small, ImageId::AcidSpores_6_9_Overlay_Large);
      if (unit->subunit)
        unit = unit->subunit;
      unit->sprite->createTopOverlay(acidOverlayId);
    }
  }
  else if (previousAcidSporeCount) {
    unit->removeOverlay(ImageId::AcidSpores_1_Overlay_Small, ImageId::AcidSpores_6_9_Overlay_Large);
  }
}

} //hooks

namespace {
/**** Helper function definitions. Do not change anything below this! ****/

u8 getAcidSporeOverlayAdjustment(const CUnit* const unit) {
  u8 adjustment = unit->acidSporeCount >> 1;
  return (adjustment < 3 ? adjustment : 3)
          + 4 * scbw::getUnitOverlayAdjustment(unit);
}

} //unnamed namespace
