#pragma once
#include "scbwdata.h"

namespace scbw {

/// The callback interface for UnitFinder::forEach().
class UnitFinderCallbackProcInterface {
  public:
    virtual void proc(CUnit *unit) = 0;
};

/// The callback interface for UnitFinder::getFirst() and UnitFinder::getNearest().
class UnitFinderCallbackMatchInterface {
  public:
    virtual bool match(const CUnit *unit) = 0;
};

/// The callback interface for UnitFinder::getBest().
class UnitFinderCallbackScoreInterface {
  public:
    virtual int score(const CUnit *unit) = 0;
};

/// The UnitFinder class is used to efficiently search for units in a certain
/// area using StarCraft's internal data structures.

class UnitFinder {
  public:
    /// Default constructor.
    UnitFinder();

    /// Constructs and searches for all units within the given bounds.
    UnitFinder(int left, int top, int right, int bottom);

    /// Searches for all units within the given bounds, using StarCraft's
    /// internal data structures. This is much more efficient than manually
    /// looping through the entire unit table or linked list.
    void search(int left, int top, int right, int bottom);

    /// Returns the number of units found by UnitFinder::search() call.
    /// If no searches have been conducted, returns 0.
    int getUnitCount() const;

    /// Returns the unit at the given index. Invalid index returns NULL instead.
    CUnit* getUnit(int index) const;

    /// Iterates through all units found, calling callback.proc() once for each
    /// unit.
    void forEach(UnitFinderCallbackProcInterface &callback) const;

    /// Returns the first unit for which callback.match() returns true.
    /// If there are no matches, returns NULL.
    CUnit* getFirst(UnitFinderCallbackMatchInterface &callback) const;

    /// Returns the unit for which callback.score() returns the highest
    /// nonnegative score. If there are no units, returns NULL.
    /// Note: If callback.score() returns a negative value, the unit is ignored.
    CUnit* getBest(UnitFinderCallbackScoreInterface &callback) const;

    /// Returns the unit nearest to (x, y) for which callback.match() returns
    /// true, using the given map bounds. If there are no matches, returns NULL.
    /// This does NOT take unit size into account when calculating distance.
    static CUnit* getNearest(int x, int y, int left, int top, int right, int bottom,
                             UnitFinderCallbackMatchInterface &callback);
    
    /// Returns the unit nearest to (x, y) for which callback.match() returns
    /// true, searching the entire map. If there are no matches, returns NULL.
    /// This does NOT take unit size into account when calculating distance.
    static CUnit* getNearest(int x, int y, UnitFinderCallbackMatchInterface &callback);

  private:
    int unitCount;
    CUnit* units[UNIT_ARRAY_LENGTH];
};

} //scbw
