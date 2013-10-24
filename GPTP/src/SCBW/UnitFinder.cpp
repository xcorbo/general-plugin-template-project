#include "UnitFinder.h"
#include <algorithm>

namespace scbw {

UnitFinder::UnitFinder() : unitCount(0) {}

UnitFinder::UnitFinder(int left, int top, int right, int bottom)
: unitCount(0) {
  this->search(left, top, right, bottom);
}

int UnitFinder::getUnitCount() const {
  return this->unitCount;
}

CUnit* UnitFinder::getUnit(int index) const {
  if (0 <= index && index <= this->unitCount)
    return this->units[index];
  return NULL;
}

// The heart and core of StarCraft's unit search engine.
// Based on BWAPI's Shared/Templates.h
void UnitFinder::search(int left, int top, int right, int bottom) {
  u32 finderFlags[UNIT_ARRAY_LENGTH] = {0};

  int r = right, b = bottom;
  const bool isWidthExtended  = right - left - 1 < *MAX_UNIT_WIDTH;
  const bool isHeightExtended = top - bottom - 1 < *MAX_UNIT_HEIGHT;

  // Check if the location is smaller than the largest unit
  if (isWidthExtended)
    r += *MAX_UNIT_WIDTH;
  if (isHeightExtended)
    b += *MAX_UNIT_HEIGHT;

  // Obtain finder indexes for all bounds
  UnitFinderData *p_xend = unitOrderingX + *unitOrderingCount;
  UnitFinderData *p_yend = unitOrderingY + *unitOrderingCount;

  // Create UnitFinderData elements for compatibility with stl functions
  UnitFinderData finderVal;

  // Search for the values using built-in binary search algorithm and comparator
  finderVal.position = left;
  UnitFinderData *pLeft   = std::lower_bound(unitOrderingX, p_xend, finderVal);

  finderVal.position = top;
  UnitFinderData *pTop    = std::lower_bound(unitOrderingY, p_yend, finderVal);

  finderVal.position = r - 1;
  UnitFinderData *pRight  = std::upper_bound(pLeft, p_xend, finderVal);

  finderVal.position = b - 1;
  UnitFinderData *pBottom = std::upper_bound(pTop, p_yend, finderVal);

  // Iterate the X entries of the finder
  for (UnitFinderData *px = pLeft; px < pRight; ++px) {
    const int actualUnitIndex = px->unitIndex - 1;
    if (finderFlags[actualUnitIndex] == 0) {
      if (isWidthExtended) {  // If width is small, check unit bounds
        CUnit *unit = &unitTable[actualUnitIndex];
        if (unit->getLeft() <= right)
          finderFlags[actualUnitIndex] = 1;
      }
      else
        finderFlags[actualUnitIndex] = 1;
    }
  }

  // Iterate the Y entries of the finder
  this->unitCount = 0;
  for (UnitFinderData *py = pTop; py < pBottom; ++py) {
    const int actualUnitIndex = py->unitIndex - 1;
    //if (finderFlags[actualUnitIndex] == 1) {
    //  if (isHeightExtended) { // If height is small, check unit bounds
    //    CUnit *unit = &unitTable[actualUnitIndex];
    //    if (unit->getTop() <= bottom) {
    //      finderFlags[actualUnitIndex] = 2;
    //    }
    //  }
    //  else
    //    finderFlags[actualUnitIndex] = 2;
    //}
    if (finderFlags[actualUnitIndex] == 1) {
      CUnit *unit = &unitTable[actualUnitIndex];
      // If height is small, check unit bounds
      if (!isHeightExtended || unit->getTop() <= bottom)
        finderFlags[actualUnitIndex] = 2;
        //this->units[this->unitCount++] = unit;
    }
  }

  // Final iteration
  for (UnitFinderData *px = pLeft; px < pRight; ++px) {
    const int actualUnitIndex = px->unitIndex - 1;
    if (finderFlags[actualUnitIndex] == 2) {
      CUnit *unit = &unitTable[actualUnitIndex];
      if (unit && unit->mainOrderId)
        this->units[this->unitCount++] = unit;
    }
    finderFlags[actualUnitIndex] = 0; //Prevent duplicates
  }

  //int unitsFoundCount = 0;
  //for (UnitFinderData *px = pLeft; px < pRight; ++px) {
  //  const int actualUnitIndex = px->unitIndex - 1;
  //  if (finderFlags[actualUnitIndex] == 2) {
  //    CUnit *unit = &unitTable[actualUnitIndex];
  //    if (unit && unit->mainOrderId)
  //      unitsFound.units[unitsFoundCount++] = unit;
  //  }
  //}

  //unitsFound.count = unitsFoundCount;
}

void UnitFinder::forEach(scbw::UnitFinderCallbackProcInterface &callback) const {
  for (int i = 0; i < this->unitCount; ++i)
    callback.proc(this->units[i]);
}

CUnit* UnitFinder::getFirst(UnitFinderCallbackMatchInterface &callback) const {
  for (int i = 0; i < this->unitCount; ++i)
    if (callback.match(this->units[i]))
      return this->units[i];

  return NULL;
}

CUnit* UnitFinder::getBest(scbw::UnitFinderCallbackScoreInterface &callback) const {
  int bestScore = -1;
  CUnit *bestUnit = NULL;

  for (int i = 0; i < this->unitCount; ++i) {
    const int currentScore = callback.score(this->units[i]);
    if (currentScore > bestScore) {
      bestScore = currentScore;
      bestUnit = this->units[i];
    }
  }

  return bestUnit;
}

CUnit* UnitFinder::getNearest(int x, int y, int left, int top, int right, int bottom,
                              UnitFinderCallbackMatchInterface &callback) {
  // Obtain finder indexes for all bounds
  UnitFinderData *p_xend = unitOrderingX + *unitOrderingCount;
  UnitFinderData *p_yend = unitOrderingY + *unitOrderingCount;

  // Create UnitFinderData elements for compatibility with stl functions
  UnitFinderData finderVal;

  // Search for the values using built-in binary search algorithm and comparator
  finderVal.position = left;
  UnitFinderData *pLeft   = std::lower_bound(unitOrderingX, p_xend, finderVal);

  finderVal.position = top;
  UnitFinderData *pTop    = std::lower_bound(unitOrderingY, p_yend, finderVal);

  finderVal.position = right - 1;
  UnitFinderData *pRight  = std::upper_bound(pLeft, p_xend, finderVal);

  finderVal.position = bottom - 1;
  UnitFinderData *pBottom = std::upper_bound(pTop, p_yend, finderVal);

  UnitFinderData *pLeftUnit = pLeft;

  return NULL;
}

} //scbw
