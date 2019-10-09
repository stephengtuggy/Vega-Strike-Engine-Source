#ifndef _UNITCONTAINER_H_
#define _UNITCONTAINER_H_

#include "debug_vs.h"

class Unit;

class UnitContainer
{
  protected:
    std::shared_ptr<Unit> unit;

  public:
    UnitContainer();
    UnitContainer(std::shared_ptr<Unit>);
    UnitContainer(const UnitContainer &un)
    {
        VSCONSTRUCT1('U')
        unit = 0;
        SetUnit(un.unit);
    }
    const UnitContainer &operator=(const UnitContainer &a)
    {
        SetUnit(a.unit);
        return a;
    }
    bool operator==(const std::shared_ptr<Unit> oth) const
    {
        return unit == oth;
    }
    bool operator!=(const std::shared_ptr<Unit> oth) const
    {
        return unit != oth;
    }
    bool operator==(const UnitContainer &oth) const
    {
        return unit == oth.unit;
    }
    bool operator!=(const UnitContainer &oth) const
    {
        return unit != oth.unit;
    }
    ~UnitContainer();
    void                  SetUnit(std::shared_ptr<Unit>);
    std::shared_ptr<Unit> GetUnit();

    const std::shared_ptr<Unit> GetConstUnit() const
    {
        return unit;
    }
};

#endif
