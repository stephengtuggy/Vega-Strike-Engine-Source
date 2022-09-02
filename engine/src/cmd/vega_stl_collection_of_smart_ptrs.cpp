/*
 * vega_stl_collection_of_smart_ptrs.cpp
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
 * along with Vega Strike. If not, see <https://www.gnu.org/licenses/>.
 */

#include "vs_logging.h"
#include "vega_stl_collection_of_smart_ptrs.hpp"
#include "damageable.h"

bool UnitCollection::isKilled(UnitBaseClassPtr unit_base_class_ptr) {
    boost::shared_ptr<Damageable> as_damageable{};
    try {
        as_damageable = boost::dynamic_pointer_cast<Damageable>(unit_base_class_ptr);
    } catch (std::bad_cast& e) {
        VS_LOG(warning, "UnitCollection::isKilled(): Encountered instance of UnitBaseClass that wasn't Damageable");
        return false;
    }
    return as_damageable->Killed();
}

UnitCollection::UnitCollection() {
    activeIters.reserve(kInitialCapacity);
}

UnitCollection::UnitCollection(const UnitCollection & uc) {
    map::const_iterator in = uc.cbegin();
    while (in != uc.cend()) {
        append(in->second);
        ++in;
    }
}

void UnitCollection::insert_unique(UnitBaseClassPtr unit_base_class_ptr) {
    append(unit_base_class_ptr);
}

void UnitCollection::prepend(UnitBaseClassPtr unit_base_class_ptr) {
    if (unit_base_class_ptr) {
        map::emplace_hint(map::cbegin(), std::make_pair(unit_base_class_ptr->getFgID(), unit_base_class_ptr));
    }
}

void UnitCollection::prepend(UnitCollection::UnitIterator * it) {
    UnitBaseClassPtr tmp{nullptr};
    if (!it) {
        return;
    }
    map::iterator tmpI = begin();
    while ((tmp = **it)) {
        insert(tmpI, tmp);
        ++tmpI;
        it->advance();
    }
}

void UnitCollection::append(UnitBaseClassPtr unit_base_class_ptr) {
    if (unit_base_class_ptr) {
        map::emplace_hint(map::cend(), std::make_pair(unit_base_class_ptr->getFgID(), unit_base_class_ptr));
    }
}

void UnitCollection::append(UnitCollection::UnitIterator * it) {
    if (!it) {
        return;
    }
    UnitBaseClassPtr tmp{nullptr};
    while ((tmp = **it)) {
        map::emplace_hint(map::cend(), std::make_pair(tmp->getFgID(), tmp));
        it->advance();
    }
}

void UnitCollection::insert(map::iterator & iter,
        UnitBaseClassPtr unit_base_class_ptr) {
    if (unit_base_class_ptr) {
        map::emplace_hint(iter, std::make_pair(unit_base_class_ptr->getFgID(), unit_base_class_ptr));
    }
}

const UnitCollection::const_iterator &UnitCollection::emplace_hint(UnitCollection::const_iterator & hint, std::pair<std::string, UnitBaseClassPtr> unit_pair) {
    return *(new UnitCollection::const_iterator(map::emplace_hint(hint, unit_pair)));
}

UnitCollection::UnitIterator::UnitIterator(const UnitCollection::UnitIterator & orig) : UnitCollection::iterator(orig), col(orig.col) {
    if (col) {
        col->reg(this);
    }
}

UnitCollection::UnitIterator::UnitIterator(UnitCollection *col1) : UnitCollection::iterator(col1->begin()), col(col1) {
    col->reg(this);

    while (*this != col->end()) {
        if (!(**this)) {
            ++(*this);
        } else {
            if (isKilled(**this)) {
                col->erase(*this);
            } else {
                break;
            }
        }
    }
}

UnitCollection::UnitIterator::~UnitIterator() {
    if (col) {
        col->unreg(this);
    }
}

void UnitCollection::UnitIterator::remove() {
    if (col && *this != col->end()) {
        col->erase(*this);
    }
}

void UnitCollection::UnitIterator::moveBefore(UnitCollection & other_list) {
    if (col && *this != col->end()) {
        other_list.prepend(**this);
        col->erase(*this);
    }
}

void UnitCollection::UnitIterator::preinsert(UnitBaseClassPtr unit_base_class_ptr) {
    if (col && unit_base_class_ptr) {
        col->insert(*this, unit_base_class_ptr);
    }
}

void UnitCollection::UnitIterator::postinsert(UnitBaseClassPtr unit_base_class_ptr) {
    UnitCollection::iterator tmp = *this;
    if (col && unit_base_class_ptr && *this != col->end()) {
        ++tmp;
        col->insert(tmp, unit_base_class_ptr);
    }
}

void UnitCollection::UnitIterator::advance() {
    if (!col || *this == col->end()) {
        return;
    }
    ++(*this);
    while (*this != col->end()) {
        if (!(**this)) {
            ++(*this);
        } else {
            if (isKilled(**this)) {
                col->erase(*this);
            } else {
                break;
            }
        }
    }
}

UnitBaseClassPtr UnitCollection::UnitIterator::next() {
    advance();
    return **this;
}

UnitCollection::UnitIterator &UnitCollection::UnitIterator::operator=(const UnitCollection::UnitIterator &orig) {
    if (col != orig.col) {
        if (col) {
            col->unreg(this);
        }
        col = orig.col;
        if (col) {
            col->reg(this);
        }
    }
    (UnitCollection::iterator)*this = orig;
    return *this;
}

const UnitCollection::UnitIterator UnitCollection::UnitIterator::operator++(int) {
    UnitCollection::UnitIterator tmp(*this);
    advance();
    return tmp;
}

const UnitCollection::UnitIterator &UnitCollection::UnitIterator::operator++() {
    advance();
    return *this;
}

UnitCollection::ConstIterator::ConstIterator(const UnitCollection::ConstIterator & orig) : UnitCollection::const_iterator(orig), col(orig.col) {
//    if (col) {
//        col->reg(this);
//    }
}

UnitCollection::ConstIterator::ConstIterator(const UnitCollection *col1) : col(col1) {
    for ((UnitCollection::const_iterator)*this = col1->cbegin(); *this != col->cend(); ++(*this)) {
        if ((**this) && !isKilled(**this)) {
            break;
        }
    }
}

UnitCollection::ConstIterator::~ConstIterator() {
//    if (col) {
//        col->unreg(this);
//    }
}

UnitCollection::ConstIterator &UnitCollection::ConstIterator::operator=(const UnitCollection::ConstIterator &orig) {
    (UnitCollection::const_iterator)*this = orig;
    col = orig.col;
    return *this;
}

UnitBaseClassPtr UnitCollection::ConstIterator::next() {
    advance();
    if (col && *this != col->cend()) {
        return **this;
    }
    return nullptr;
}

void UnitCollection::ConstIterator::advance() {
    if (!col || *this == col->cend()) {
        return;
    }
    ++(*this);
    while (*this != col->cend()) {
        if (!(**this)) {
            ++(*this);
        } else {
            if (isKilled(**this)) {
                ++(*this);
            } else {
                break;
            }
        }
    }
}

const UnitCollection::ConstIterator &UnitCollection::ConstIterator::operator++() {
    advance();
    return *this;
}

const UnitCollection::ConstIterator UnitCollection::ConstIterator::operator++(int) {
    UnitCollection::ConstIterator tmp(*this);
    advance();
    return tmp;
}
