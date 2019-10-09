class Unit;
bool DistanceWarrantsWarpTo(std::shared_ptr<Unit> parent, float sigdistance, bool following); // following == not worried about toodamnclose
bool DistanceWarrantsTravelTo(std::shared_ptr<Unit> parent, float sigdistance, bool following);
bool TargetWorthPursuing(std::shared_ptr<Unit> parent, std::shared_ptr<Unit> target);
void WarpToP(std::shared_ptr<Unit> parent, std::shared_ptr<Unit> target, bool following);
void WarpToP(std::shared_ptr<Unit> parent, const QVector &target, float radius, bool following);
