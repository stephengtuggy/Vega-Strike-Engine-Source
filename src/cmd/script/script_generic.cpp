#include "xml_support.h"
#include "cmd/unit_generic.h"
#include "mission.h"

void Mission::call_unit_toxml(missionNode *node, int mode, varInst *ovi)
{
    std::shared_ptr<Unit> my_object = getUnitObject(node, mode, ovi);
    if (my_object) {
        string fgid = my_object->getFgID();
    }
}

std::shared_ptr<Unit> Mission::getUnitObject(missionNode *node, int mode, varInst *ovi)
{
    std::shared_ptr<Unit> my_object = NULL;
    if (mode == SCRIPT_RUN)
        my_object = (std::shared_ptr<Unit>)ovi->object;
    return my_object;
}
