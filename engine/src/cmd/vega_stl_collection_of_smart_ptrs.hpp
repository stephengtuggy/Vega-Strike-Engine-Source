/*
 * vega_stl_collection_of_smart_ptrs.hpp
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

#ifndef VEGA_STRIKE_SRC_CMD_VEGA_STL_COLLECTION_OF_SMART_PTRS_HPP_
#define VEGA_STRIKE_SRC_CMD_VEGA_STL_COLLECTION_OF_SMART_PTRS_HPP_

#include <utility>
#include <string>
#include <cstdint>
#include <map>
#include <vector>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include "vs_umap.hpp"
#include "unit_base_class.hpp"
//#include "unit_generic.h"

static const int kInitialCapacity = 20;
/**
 * UnitCollection is the collection type for holding units. It's used to contain subunits,
 * units in a system, etc. Currently, you don't assign one collection to another. You're
 * not supposed to hold references to the list across physics frames.
 *
 * UnitCollection is intended to be robust to at least 20,000 units.
 */
class UnitCollection : public std::map<std::string, boost::shared_ptr<UnitBaseClass>> {
public:
    /**
     * UnitIterator is the "node" class for UnitCollection.
     * It inherits from a std::iterator type, but with additional functions for Python
     * code compatibility.
     */
    class UnitIterator : public UnitCollection::iterator {
    protected:
        friend class UnitCollection;
        /**
         * Pointer back to the collection we were spawned from
         */
        UnitCollection * col;

    public:
        UnitIterator() : col() {}
        UnitIterator(const UnitIterator &);
        UnitIterator(UnitCollection * col1);
        virtual ~UnitIterator();

        inline bool isDone() {
            if (col && *this != col->end()) {
                return false;
            }
            return true;
        }

        inline UnitBaseClassPtr get() {
            return **this;
        }

        /**
         * Request that the current unit be removed
         */
        void remove();

        /**
         * Move unit pointed to, to other_list
         */
        void moveBefore(UnitCollection & other_list);

        /**
         * Insert unit before current Unit
         */
        void preinsert(UnitBaseClassPtr unit_base_class_ptr);

        /**
         * Insert unit after current Unit
         */
        void postinsert(UnitBaseClassPtr unit_base_class_ptr);

        /**
         * Increment to next valid unit. (May iterate many times.)
         */
        void advance();

        /**
         * Same as advance, only it returns the unit at the same time
         *
         * @return the unit pointed to
         */
        UnitBaseClassPtr next();

        inline size_t size() const {
            return col->size();
        }

        UnitIterator & operator=(const UnitIterator &orig);

        const UnitIterator operator++(int);

        const UnitIterator & operator++();

        inline UnitBaseClassPtr operator*() {
            if (col && *this != col->end()) {
                return iterator::operator->()->second;
            }
            return nullptr;
        }
    };

    /**
     * This class is to be used when no changes to the list are made
     * and the iterator doesn't persist across physics frames.
     * That is to say, these should only be used as temporary iterators
     * in loops where the list is not modified.
     */
    class ConstIterator : public UnitCollection::const_iterator {
    protected:
        friend class UnitCollection;
        UnitCollection const * col;

    public:
        ConstIterator() : col() {}
        ConstIterator(const ConstIterator &);
        ConstIterator(UnitCollection const * col1);
        virtual ~ConstIterator();
        ConstIterator & operator=(const ConstIterator & orig);
        UnitBaseClassPtr next();

        inline size_t size() const {
            return col->size();
        }

        inline bool isDone() {
            if (col && *this != col->end()) {
                return false;
            }
            return true;
        }

        void advance();
        const ConstIterator & operator++();
        const ConstIterator operator++(int);

        inline UnitBaseClassPtr operator*() const {
            if (col && *this != col->end()) {
                return const_iterator::operator->()->second;
            }
            return nullptr;
        }
    };

    /**
     * For backwards compatibility
     */
    using ConstFastIterator = ConstIterator;
    using FastIterator = UnitIterator;

    UnitCollection();
    UnitCollection(const UnitCollection &);

    inline ~UnitCollection() {
        destr();
    }

    /**
     * Iterator creation functions. We use these to set the col pointer.
     */
     inline UnitIterator createIterator() {
         return UnitIterator(this);
     }

     inline FastIterator fastIterator() {
         return FastIterator(this);
     }

     inline ConstIterator constIterator() const {
         return ConstIterator(this);
     }

     inline ConstFastIterator constFastIterator() const {
         return ConstFastIterator(this);
     }

     /**
      * Insert a unit into the collection without providing a hint as to its
      * proper position
      */
     void insert_unique(UnitBaseClassPtr);

     inline bool empty() const {
         if (map::empty()) {
             return true;
         } else if (removedIters.empty() || map::size() > removedIters.size()) {
             return false;
         } else {
             return true;
         }
     }

     /**
      * Insert a unit into the collection with hint to put it at the front
      */
     void prepend(UnitBaseClassPtr);

     /**
      * Add an iterator to the front of the iterator list
      */
     void prepend(UnitIterator *);

     /**
      * Insert a unit into the collection with hint to put it at the back
      */
     void append(UnitBaseClassPtr);

     /**
      * Add an iterator to the back of the iterator list
      */
     void append(UnitIterator *);

     /**
      * Insert a unit into the collection with hint to put it right before iterator
      */
     void insert(map::iterator &, UnitBaseClassPtr);

     /**
      * Add a unit into the collection with hint regarding where to put it
      */
     const UnitCollection::const_iterator & emplace_hint(UnitCollection::const_iterator &, std::pair<std::string, UnitBaseClassPtr>);

     /**
      * Wipes out entire collection, only if no iterators are being held.
      *
      * Is this presently being used anywhere? Previous comment says that it was
      * unused as of 0.5
      */
     void clear();

     bool contains(boost::shared_ptr<Unit const>) const;

     /**
      * Note: Adding return value SGT 2022-08-28
      *
      * @return the position in the collection from which the unit was removed
      */
     map::iterator erase(map::iterator &);

     bool remove(boost::shared_ptr<Unit const>);

     /**
      * Returns number of non-null units in collection
      *
      * @return number of non-null units in collection
      */
     inline const size_t size() const {
         return map::size() - removedIters.size();
     }

     /**
      * Returns last non-null unit in collection. May be Killed()
      *
      * @return pointer to last non-null unit in collection
      */
     inline UnitBaseClassPtr back() {
         for (map::reverse_iterator it = map::rbegin(); it != map::rend(); ++it) {
             if (it->second) {
                 return it->second;
             }
         }
         return nullptr;
     }

     /**
      * Returns first non-null unit in collection. May be Killed()
      *
      * @return pointer to first non-null unit in collection
      */
    inline UnitBaseClassPtr front() {
        for (map::iterator it = map::begin(); it != map::end(); ++it) {
            if (it->second) {
                return it->second;
            }
        }
        return nullptr;
    }

    static bool isKilled(UnitBaseClassPtr unit_base_class_ptr);

private:
    friend class UnitIterator;
    friend class ConstIterator;

    /**
     * Does not actually clear out collection. It sets all the Unit pointers to null
     * and sets all the current iterators' collection pointers to null, effectively
     * shutting down the collection so that it can be destroyed safely.
     */
    void destr();

    /**
     * Does anything use this operator yet? A previous comment says that it was unused
     * as of 0.5
     *
     * @return
     */
    const UnitCollection & operator=(const UnitCollection &);

    /**
     * An iterator "registers" with a collection when it is created. This is how
     * a collection tracks how many iterators are being held and what position
     * each one is pointing to.
     */
    void reg(UnitCollection::UnitIterator *);

    /**
     * Unregistering has the added function of clearing the list of Null unit
     * pointers to be removed from the collection when we are down to our
     * last active iterator.
     */
    void unreg(UnitCollection::UnitIterator *);

    /**
     * This is a list of the current iterators being held
     */
    std::vector<UnitCollection::UnitIterator *> activeIters;

    /**
     * This is a list of positions in the collection that are pointing to
     * Null units, positions that should be removed from the collection
     * but couldn't because another iterator was referencing it
     */
    std::vector<map::iterator *> removedIters;
};

using VegaUnitCollection = UnitCollection;
using un_iter = UnitCollection::UnitIterator;
using un_kiter = UnitCollection::ConstIterator;
using un_fiter = UnitCollection::UnitIterator;
using un_fkiter = UnitCollection::ConstIterator;

inline UnitCollection::const_iterator setUnitFullName(UnitCollection& unit_collection, UnitCollection::iterator unit_in_question, std::string new_name) {
    UnitBaseClassPtr the_unit = unit_in_question->second;
    UnitCollection::iterator old_location = unit_collection.erase(unit_in_question);
    the_unit->setFullname(new_name);
    return unit_collection.emplace_hint(old_location, std::make_pair(new_name, the_unit));
}

inline UnitCollection::const_iterator setUnitFullName(UnitCollection& unit_collection, un_iter unit_in_question, std::string new_name) {
    UnitBaseClassPtr the_unit = unit_in_question->second;
    UnitCollection::iterator old_location = unit_collection.erase(unit_in_question);
    the_unit->setFullname(new_name);
    return unit_collection.emplace_hint(old_location, std::make_pair(new_name, the_unit));
}

//#include <set>
//#include <boost/shared_ptr.hpp>
//#include "unit_base_class.hpp"
//
//struct UnitCompare {
//    bool operator()(boost::shared_ptr<Unit> unit_a, boost::shared_ptr<Unit> unit_b) {
//        return unit_a->getFgID() < unit_b->getFgID();
//    }
//};
//
//class UnitCollection : public std::set<boost::shared_ptr<Unit>, UnitCompare> {
//
//};

#endif //VEGA_STRIKE_SRC_CMD_VEGA_STL_COLLECTION_OF_SMART_PTRS_HPP_
