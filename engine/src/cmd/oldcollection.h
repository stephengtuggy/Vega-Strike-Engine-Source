/*
 * oldcollection.h
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

#ifndef _OLDCOLLECTION_H_
#define _OLDCOLLECTION_H_
#include <stdlib.h>

class Unit;

class UnitCollection {
private:
///units is the beginning of the list. It isn't a pointer to reduce indirection
    class UnitListNode {
#ifdef _TEST_
        friend void Iterate( UnitCollection &c );
#endif
    public:
        UnitPtr unit;
        UnitListNode *next;

        UnitListNode(UnitPtr unit);
///These are functions for iterator use only
        void PostInsert(UnitPtr un);
/// iterator use only
        void Remove();
        UnitListNode(UnitPtr unit, UnitListNode *next);
        ~UnitListNode();

    private:
        UnitListNode();
        UnitListNode(const UnitListNode &);
        UnitListNode &operator=(const UnitListNode &);
    }
            *u;
///Destroys the list until init is called. Functions will segfault.
    void destr();

///Initializes the list so that there are 2 empty nodes (u and u->next)  NULL unit terminates this list.
    void init() {
        u = new UnitListNode(NULL);
        u->next = new UnitListNode(NULL, new UnitListNode(NULL));
    }

public:
///Initislizes the first unit and then calls init;
    UnitCollection() {
        init();
    }

///destroys the list permanently
    ~UnitCollection() {
        destr();
    }

    class UnitIterator {
#ifdef _TEST_
        friend void Iterate( UnitCollection &c );
#endif
    private:
///the position in the list
        UnitListNode *pos;
///Finds the next unit (or NULL) that isn't Killed()
        void GetNextValidUnit();
    public:
        UnitIterator() : pos(NULL) {
        }

///Creates this unit iterator
        UnitIterator(UnitListNode *start) : pos(start) {
            GetNextValidUnit();
        }

        UnitIterator(const UnitIterator &orig) : pos(orig.pos) {
        }

        UnitIterator &operator=(const UnitIterator &orig) {
            pos = orig.pos;
            return *this;
        }

        ~UnitIterator() {
            pos = NULL;
        }

        bool isDone() const {
            return pos->next->unit == NULL;
        }

///removes something after pos.  eg the first valid unit. or current()
        void remove();
        void moveBefore(UnitCollection &otherList);

///inserts in front of current
        void preinsert(UnitPtr unit) {
            pos->next = new UnitListNode(unit, pos->next);
        }

/// inserts after current
        void postinsert(UnitPtr unit);

///returns the unit pos is pointing at or NULL if all dead or end of list.
        UnitPtr current() {
            return pos->next->unit;
        }

///advances the counter
        UnitPtr next() {
            advance();
            return current();
        }

        void advance() {
            pos = pos->next;
            GetNextValidUnit();
        }

        inline UnitPtr operator++(int) {
            UnitPtr un = current();
            advance();
            return un;
        }

        inline UnitPtr operator++() {
            advance();
            return current();
        }

        inline UnitPtr operator*() {
            return current();
        }
    };
    class ConstIterator {
    private:
        const UnitListNode *pos;
        void GetNextValidUnit();
    public:
        ConstIterator() : pos(NULL) {
        }

        ConstIterator(const ConstIterator &orig) : pos(orig.pos) {
        }

        ConstIterator(const UnitListNode *start) : pos(start) {
            GetNextValidUnit();
        }

        ConstIterator &operator=(const ConstIterator &orig) {
            pos = orig.pos;
            return *this;
        }

        ~ConstIterator() {
            pos = NULL;
        }

        const UnitPtr next() {
            advance();
            return current();
        }

        const UnitPtr current() const {
            return pos->next->unit;
        }

        bool isDone() const {
            return current() == NULL;
        }

        void advance() {
            pos = pos->next;
            GetNextValidUnit();
        }

        inline const UnitPtr operator++() {
            advance();
            return current();
        }

        inline const UnitPtr operator++(int) {
            const UnitPtr un = current();
            advance();
            return un;
        }

        inline const UnitPtr operator*() const {
            return current();
        }
    };

    class ConstFastIterator {
    private:
        const UnitListNode *pos;
    public:
        ConstFastIterator() : pos(NULL) {
        }

        ConstFastIterator(const ConstFastIterator &orig) : pos(orig.pos) {
        }

        ConstFastIterator(const UnitListNode *start) : pos(start) {
        }

        ~ConstFastIterator() {
            pos = NULL;
        }

        const UnitPtr current() const {
            return pos->next->unit;
        }

        void advance() {
            pos = pos->next;
        }

        inline const UnitPtr operator++() {
            advance();
            return current();
        }

        inline const UnitPtr operator++(int) {
            const UnitPtr un = current();
            advance();
            return un;
        }

        inline const UnitPtr operator*() const {
            return current();
        }

        const UnitPtr next() {
            advance();
            return current();
        }

        bool isDone() const {
            return pos->next->unit == NULL;
        }

    private:
        ConstFastIterator &operator=(const ConstFastIterator &);
    };

    class FastIterator {
    private:
        UnitListNode *pos;
    public:

///removes something after pos.  eg the first valid unit. or current()
        void remove();

///inserts in front of current
        void preinsert(UnitPtr unit) {
            pos->next = new UnitListNode(unit, pos->next);
        }

/// inserts after current
        void postinsert(UnitPtr unit);

        FastIterator() : pos(NULL) {
        }

        FastIterator(const FastIterator &orig) : pos(orig.pos) {
        }

        FastIterator(UnitListNode *start) : pos(start) {
        }

        ~FastIterator() {
            pos = NULL;
        }

        UnitPtr current() {
            return pos->next->unit;
        }

        void advance() {
            pos = pos->next;
        }

        inline UnitPtr operator++(int) {
            UnitPtr un = current();
            advance();
            return un;
        }

        inline UnitPtr operator++() {
            advance();
            return current();
        }

        inline UnitPtr operator*() {
            return current();
        }

        UnitPtr next() {
            advance();
            return current();
        }

        bool isDone() const {
            return pos->next->unit == NULL;
        }

    private:
        FastIterator &operator=(const FastIterator &);
    };
    static void FreeUnusedNodes(); //not allowed to happen if any lists are traversing
    static void *PushUnusedNode(UnitListNode *node);
#ifdef _TEST_
    friend void Iterate( UnitCollection &c );
#endif

//could be empty and this returns false...but usually correct...never has units when it returns true
    bool empty() const {
        return u->next->unit == NULL;
    }

    UnitIterator createIterator() {
        return UnitIterator(u);
    }

    ConstIterator constIterator() const {
        return ConstIterator(u);
    }

    FastIterator fastIterator() {
        return FastIterator(u);
    }

    ConstFastIterator constFastIterator() const {
        return ConstFastIterator(u);
    }

    void insert_unique(UnitPtr un) {
        for (UnitListNode *i = u->next; i != NULL; i = i->next) {
            if (i->unit == un) {
                return;
            }
        }
        prepend(un);
    }

    void prepend(UnitPtr unit) {
        u->next = new UnitListNode(unit, u->next);
    }

    void prepend(UnitListNode *unitlistnode) {
        unitlistnode->next = u->next;
        u->next = unitlistnode;
    }

    void prepend(UnitIterator *iter);
    void append(UnitPtr unit);
    void append(UnitIterator *iter);

    void clear() {
        destr();
        init();
    }

    bool contains(const UnitPtr unit) const;
    bool remove(const UnitPtr unit);
    void cleanup();

    UnitPtr front() {
        return *createIterator();
    }

    const UnitPtr front() const {
        return *constIterator();
    }

    UnitCollection(const UnitCollection &c);
    const UnitCollection &operator=(const UnitCollection &c);
};
typedef UnitCollection::UnitIterator un_iter;
typedef UnitCollection::ConstIterator un_kiter;
typedef UnitCollection::ConstFastIterator un_fkiter;
typedef UnitCollection::FastIterator un_fiter;

#endif

