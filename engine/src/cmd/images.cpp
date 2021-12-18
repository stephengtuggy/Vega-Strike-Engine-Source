#include "cmd/images.h"


template < typename BOGUS > //added by chuck starchaser, to try to break dependency to VSSprite in vegaserver
UnitImages<BOGUS>::~UnitImages()
{
    if (pExplosion != nullptr) {
        delete pExplosion;
        pExplosion = nullptr;
    }
    if (pHudImage != nullptr) {
        delete pHudImage;
        pHudImage = nullptr;
    }
    VSDESTRUCT1
}

//explicit instantiation needed
template struct UnitImages<void>;

static UnitImages<void> bleh;
