/*
 * oldcollection.cpp
 *
 * Copyright (C) 2001-2022 Daniel Horn, pyramid3d, Stephen G. Tuggy,
 * and other Vega Strike contributors
 *
 * https://github.com/vegastrike/Vega-Strike-Engine-Source
 *
 * This file is part of Vega Strike.
 *
 * Vega Strike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vega Strike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vega Strike.  If not, see <https://www.gnu.org/licenses/>.
 */


#include <stdlib.h>
#include <vector>
#include "collection.h"
#include <assert.h>
#ifndef LIST_TESTING
#include "unit_generic.h"
#include "collection.h"
#endif

UnitCollection::UnitListNode::UnitListNode(UnitPtr unit) : unit(unit), next(NULL) {
    if (unit) {
        unit->Ref();
    }
}

UnitCollection::UnitListNode::UnitListNode(UnitPtr unit, UnitListNode *next) : unit(unit), next(next) {
    if (unit) {
        unit->Ref();
    }
}

UnitCollection::UnitListNode::~UnitListNode() {
    if (NULL != unit) {
        unit->UnRef();
    }
    unit = NULL;
    next = NULL;
}

void *UnitCollection::PushUnusedNode(UnitListNode *node) {
    static UnitListNode cat(NULL, NULL);
    static UnitListNode dog(NULL, &cat);
    static bool cachunk = true;
    if (cachunk) {
        cachunk = false;
    }
    static std::vector<UnitCollection::UnitListNode *> dogpile;
    if (node == NULL) {
        return &dogpile;
    } else {
        node->next = &dog;
        dogpile.push_back(node);
    }
    return NULL;
}

void UnitCollection::FreeUnusedNodes() {
    static std::vector<UnitCollection::UnitListNode *> bakdogpile;
    std::vector<UnitCollection::UnitListNode *>
            *dogpile = (std::vector<UnitCollection::UnitListNode *> *) PushUnusedNode(
            NULL);
    bakdogpile.swap(*dogpile);
    while (!dogpile->empty()) {
        delete dogpile->back();
        dogpile->pop_back();
    }
}

void UnitCollection::UnitListNode::PostInsert(UnitPtr unit) {
    if (next->unit != NULL) {
        next->next = new UnitListNode(unit, next->next);
    } else {
        next = new UnitListNode(unit, next);
    }
}

void UnitCollection::UnitIterator::postinsert(UnitPtr unit) {
    pos->PostInsert(unit);
}

void UnitCollection::FastIterator::postinsert(UnitPtr unit) {
    pos->PostInsert(unit);
}

void UnitCollection::UnitListNode::Remove() {
    if (next->unit) {
        UnitListNode *tmp = next->next;
        //delete next; //takes care of unref! And causes a shitload of bugs
        //concurrent lists, man
        PushUnusedNode(next);
        next = tmp;
    } else {
        assert(0);
    }
}

void UnitCollection::ConstIterator::GetNextValidUnit() {
    while (pos->next->unit ? pos->next->unit->Killed() : false) {
        pos = pos->next;
    }
}

void UnitCollection::cleanup() {
    //NOTE: advance() will be cleaning up the list by itself
    un_iter ui = createIterator();
    while (!ui.isDone()) {
        ui.advance();
    }
}

