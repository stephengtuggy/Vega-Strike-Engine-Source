#ifndef _GFX_CLICK_LIST_H_
#define _GFX_CLICK_LIST_H_

//#include "vegastrike.h"
#include "gfx/matrix.h"
#include "collection.h"
#include "star_system.h"
Vector MouseCoordinate(int x, int y); // FIXME

class ClickList
{
  private:
    UnitCollection *      parentIter;
    StarSystem *          parentSystem;
    UnitCollection *      lastCollection;
    std::shared_ptr<Unit> lastSelected;

  public:
    // gets passed in unnormalized mouse values btw 0 and g_game.x_resolution&& g_game.y_resolution
    bool queryShip(int mouseX, int mouseY, std::shared_ptr<Unit>); // returns if the ship's in iterator utilizes
    ClickList(StarSystem *parSystem, UnitCollection *parentIter);
    ~ClickList()
    {
    }
    UnitCollection *      requestIterator(int mouseX, int mouseY);
    UnitCollection *      requestIterator(int minX, int minY, int maxX, int maxY);
    std::shared_ptr<Unit> requestShip(int mouseX, int mouseY);
};
#endif
