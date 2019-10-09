#include "navigation.h"
namespace Orders
{
class DockingOps : public MoveTo
{
    UnitContainer docking;
    void *        formerOwnerDoNotDereference;
    enum { GETCLEARENCE, DOCKING, DOCKED, UNDOCKING } state;
    Order *oldstate;
    float  timer;
    int    port;
    bool   physicallyDock;
    bool   facedtarget;
    bool   keeptrying;

  public:
    QVector Movement(std::shared_ptr<Unit> utdw);
    DockingOps(std::shared_ptr<Unit> unitToDockWith, Order *oldstate, bool physicallyDock, bool keeptrying);
    virtual void SetParent(std::shared_ptr<Unit> par);
    void         Execute();
    bool         RequestClearence(std::shared_ptr<Unit> );
    bool         DockToTarget(std::shared_ptr<Unit> );
    bool         PerformDockingOperations(std::shared_ptr<Unit> );
    bool         Undock(std::shared_ptr<Unit> );
    virtual void Destroy();
    void         RestoreOldAI();
};
} // namespace Orders
