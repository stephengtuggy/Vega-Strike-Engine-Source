#include <stdlib.h>
#include "container.h"
#include "unit_generic.h"
UnitContainer::UnitContainer()
{
    unit = NULL;
    VSCONSTRUCT1( 'U' )
}
UnitContainer::UnitContainer( Unit *un ) : unit( NULL )
{
    SetUnit( un );
    VSCONSTRUCT1( 'U' );
}
UnitContainer::~UnitContainer()
{
    VSDESTRUCT1
    if (unit)
        unit->UnRef();
    //bad idea...arrgh!
}
void UnitContainer::SetUnit( Unit *un )
{
    if (unit) {
        unit->UnRef();
    }
    // // if the unit is null then go here otherwise if the unit is killed then go here
    if (un == NULL /*  ? un->Killed() == true : true */) {
        unit = NULL;
        return;
    } else {
        unit = un;
        unit->Ref();
    }
}
