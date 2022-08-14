/*
 * collection.h
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
 * the Free Software Foundation, either version 2 of the License, or
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


/* unitCollection.h
 *
 *****/

#ifndef _UNITCOLLECTION_H_
#define _UNITCOLLECTION_H_

//Collection type:
//#define USE_OLD_COLLECTION
#define USE_STL_COLLECTION

#if defined (USE_OLD_COLLECTION)
#include "oldcollection.h"
#elif defined (USE_STL_COLLECTION)

#include <cstddef>
#include <list>
#include <vector>
#include "unit_fwd_decl.hpp"

/*
 * UnitCollection is the list container for units.
 * It's used to contain subunits, units in a system, etc.
 * Currently, you dont assign one collection to another.
 * You're not supposed to hold references to the list across physics frames
 * UnitCollection is designed to be robust to at least 20,000 units.
 */
class UnitCollection {
public:
    /*
     * UnitIterator is the "node" class for UnitCollection.
     * It's meant to mimic std::iterator's for the most part, but
     * Python code doesn't use them that way so we had to keep some
     * functions for backwards compatibility.
     * Operators supported : ++(), ()++, =, *
     */
    class UnitIterator {
    public:
        UnitIterator() : col(NULL) {
        }

        UnitIterator(const UnitIterator &);
        UnitIterator(UnitCollection *);
        virtual ~UnitIterator();

        inline bool isDone() {
            if (col && it != col->u.end()) {
                return false;
            }
            return true;
        }

        /*   Request the current unit to be removed */
        void remove();

        /*  Move argument list to the beginning of current list */
        void moveBefore(UnitCollection &);

        /* Insert unit before current UnitPtr/
        void preinsert(UnitPtr);

        /* Insert unit after current UnitPtr/
        void postinsert(UnitPtr unit);

        /* increment to next valid unit (may iterate many times) */
        void advance();

        /* same as advance, only it returns the unit at the same time */
        UnitPtr next();

        int size() const {
            return (col->size());
        }

        UnitIterator &operator=(const UnitIterator &);

        inline const UnitIterator operator++(int) {
            UnitCollection::UnitIterator tmp(*this);
            advance();
            return tmp;
        }

        inline const UnitIterator &operator++() {
            advance();
            return *this;
        }

        inline UnitPtr operator*() {
            if (col && it != col->u.end()) {
                return *it;
            }
            return NULL;
        }

    protected:
        friend class UnitCollection;
        //Pointer back to the collection we were spawned from
        UnitCollection *col;

        //Current position in the list
        std::list<UnitPtr>::iterator it;
    };

    /* This class is to be used when no changes to the list are made
     * and the iterator doesn't persist across physics frames.
     * that is to say, these should only be used as temporary iterators
     * in loops where the list is not modified.
     */
    class ConstIterator {
    public:
        ConstIterator() : col(NULL) {
        }

        ConstIterator(const ConstIterator &);
        ConstIterator(const UnitCollection *);
        ~ConstIterator();
        ConstIterator &operator=(const ConstIterator &orig);
        UnitPtr next();

        int size() const {
            return (col->size());
        }

        inline bool isDone() {
            if (col && it != col->u.end()) {
                return false;
            }
            return true;
        }

        void advance();
        const ConstIterator &operator++();
        const ConstIterator operator++(int);

        inline UnitPtr operator*() const {
            if (it != col->u.end() && !col->empty()) {
                return *it;
            }
            return NULL;
        }

    protected:
        friend class UnitCollection;
        const UnitCollection *col;
        std::list<UnitPtr>::const_iterator it;
    };

    /* backwards compatibility only.  Typedefs suck. dont use them. */
    typedef ConstIterator ConstFastIterator;
    typedef UnitIterator FastIterator;

    UnitCollection();
    UnitCollection(const UnitCollection &);

    inline ~UnitCollection() {
        destr();
    }

    /* Iterator creation functions. We use this to set the col pointer */
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

    /* Traverses entire list and only inserts if no matches are found
     * Do not use in any fast-code paths */
    void insert_unique(UnitPtr);

    inline bool empty() const {
        if (u.empty()) {
            return true;
        } else if (removedIters.empty() || u.size() > removedIters.size()) {
            return false;
        } else {
            return true;
        }
    }

    // Add a unit or iterator to the front of the list. */
    void prepend(UnitPtr);
    void prepend(UnitIterator *);

    /* Add a unit or iterator to the back of the list. */
    void append(UnitPtr);
    void append(UnitIterator *);

    /* This is how iterators insert units. Always inserts before iterator */
    void insert(std::list<UnitPtr>::iterator &, UnitPtr);

    /* Whipes out entire list only if no iterators are being held.
     * No code uses this function as of 0.5 release */
    void clear();

    bool contains(const UnitPtr) const;

    /* We only erase the unit from the list under the following conditions:
     * 1. if we have less than 4 iterators being held
     * 2. if none of those iterators are referencing the requested UnitPtr Otherwise the Unit pointer is removed from the list and set to NULL,
     * and the iterator is referenced on another list to be deleted
     * the delete list is processed when the number of iterators hits 1 or 0.
     * The reason for this is so we can be scalable to 20,000+ units and
     * modifications to the list by multiple held iterators dont bog us down
     */
    void erase(std::list<UnitPtr>::iterator &);

    /* traverse list and remove first (only) matching Unit.
     * Do not use in fast-path code */
    bool remove(const UnitPtr);

    /* Returns number of non-null units in list */
    inline const int size() const {
        return u.size() - removedIters.size();
    }

    /* Returns last non-null unit in list. May be Killed() */
    inline UnitPtr back() {
        for (std::list<UnitPtr>::reverse_iterator it = u.rbegin(); it != u.rend(); ++it) {
            if (*it) {
                return *it;
            }
        }
        return NULL;
    }

    /* Returns first non-null unit in list. May be Killed() */
    inline UnitPtr front() {
        for (std::list<UnitPtr>::iterator it = u.begin(); it != u.end(); ++it) {
            if (*it) {
                return *it;
            }
        }
        return NULL;
    }

private:
    friend class UnitIterator;
    friend class ConstIterator;

    /* Does not clear list.  It sets all the Unit pointers to null
     * And sets all the current iterator's collection pointers to NULL.
     * Effectively shutting the list down so it can be destroyed safely. */
    void destr();

    /* Nothing uses this operator as of 0.5, but maybe someday */
    const UnitCollection &operator=(const UnitCollection &);

    /* An iterator "registers" with a collection when it is created
     * This is how a collection tracks how many iterators are being held
     * and what position they are all holding
     */
    void reg(UnitCollection::UnitIterator *);

    /* Unregistering has the added function of clearing the list of
     * Null unit pointers to be removed from the collection when
     * we are down to our last active iterator */
    void unreg(UnitCollection::UnitIterator *);

    /* This is a list of the current iterators being held */
    std::vector<class UnitCollection::UnitIterator *> activeIters;

    /* This is a list of positions in the collection that are pointing to
     * NULL units, positions that should be removed from the collection
     * but couldn't because another iterator was referencing it. */
    std::vector<std::list<UnitPtr>::iterator> removedIters;

    /* Main collection */
    std::list<UnitPtr> u;
};

/* Typedefs.   We really should not use them but we're lazy */
typedef UnitCollection::UnitIterator un_iter;
typedef UnitCollection::ConstIterator un_kiter;
typedef UnitCollection::UnitIterator un_fiter;
typedef UnitCollection::ConstIterator un_fkiter;

#else
#error "No collection type chosen in collection.h:10"
#endif //USE_STL_COLLECTION

#endif

