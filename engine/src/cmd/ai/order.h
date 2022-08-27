/*
 * order.h
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
 * along with Vega Strike.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef _CMD_ORDER_H

#define _CMD_ORDER_H

#include "gfx/vec.h"
#include "cmd/container.h"
#include <list>
#include <vector>
#include <string>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "vega_intrusive_ptr.hpp"

/**
 * Order is the base class for all orders.  All orders inherit from Order
 * Each fighter may have an order they are currently executing. Orders naturally
 * can queue other orders, and decide which orders may be executed in parallel by those orders' bit codes.
 * Orders affect their "parent" and may call any function within that parent
 * Limitation (perhaps resolvable) Orders do not "know" what their parent
 * is until Execute() time.  Perhaps "SetParent" Should be made virtual so that
 * various variables may be set earlier than at every execution time
 */

class Order;
using OrderRawPtr = Order *;
using OrderConstRawPtr = Order const *;
using OrderIntrusivePtr = boost::intrusive_ptr<Order>;
using OrderSharedPtr = boost::shared_ptr<Order>;
using OrderWeakPtr = boost::weak_ptr<Order>;

using OrderPtr = OrderSharedPtr;
using OrderParentPtr = OrderWeakPtr;
using OrderPtrForPy = OrderSharedPtr; //OrderRawPtr;

using OrderRef = Order &;

class Animation;
typedef std::vector<class varInst *> olist_t;
class Order : public boost::intrusive_ref_counter<Order, boost::thread_safe_counter> {
private:

public:
    virtual ~Order();
protected:
///The unit this order is attached to
    UnitParentPtr parent;
///The bit code (from ORDERTYPES) that this order is (for parallel execution)
    unsigned int type;

    unsigned int subtype;
///Whether or not this order is done
    bool done;
///If this order applies to a group of units (as in form up with this group)
    UnitContainer group;
///If this order applies to a physical location in world space
    QVector targetlocation;
///The queue of suborders that will be executed in parallel according to bit code
    std::vector<OrderPtr> suborders;
///a bunch of communications that have not been answered CommunicationMessages are actually containing reference to a nice Finite State Machine that can allow a player to have a reasonable conversation with an AI
    std::list<class CommunicationMessage *> messagequeue;
///changes the local relation of this unit to another...may inform superiors about "good" or bad! behavior depending on the AI
    virtual void Destructor();
protected:
/// this function calls the destructor (needs to be overridden for python;

public:

    virtual void ChooseTarget() {
        /*not implemented see fire.cpp*/
    }

    virtual bool PursueTarget(UnitPtr, bool isleader) {
        return false;
    }

///clears the messasges of this order
    void ClearMessages();
///The varieties of order types  MOVEMENT,FACING, and WEAPON orders may not be mutually executed (lest one engine goes left, the other right)
    enum ORDERTYPES { MOVEMENT = 1, FACING = 2, WEAPON = 4, CLOAKING = 8, ALLTYPES = (1 | 2 | 4 | 8) };
    enum SUBORDERTYPES { SLOCATION = 1, STARGET = 2, SSELF = 4 };

///The default constructor setting everything to NULL and no dependency on order
    Order()
            : parent(),
            type(0),
            subtype(0),
            done(false),
            targetlocation(0, 0, 0) {
        VSCONSTRUCT1('O')
    }

///The constructor that specifies what order dependencies this order has
    Order(int type, int subtype)
            : parent(),
            type(type),
            subtype(subtype),
            done(false),
            targetlocation(0, 0, 0) {
        VSCONSTRUCT1('O')
    }

    // TODO: Refactor this away! stephengtuggy 2022-08-23
///The virtual function that unrefs all memory then calls Destruct () which takes care of unreffing this or calling delete on this
    virtual void Destroy();

///The function that gets called and executes all queued suborders
    virtual void Execute();
///returns a pointer to the first order that may be bitwised ored with that type
    OrderPtr queryType(unsigned int type);
///returns a pointer to the first order that may be bitwise ored with any type
    OrderPtr queryAny(unsigned int type);
///Erases all orders that bitwise OR with that type
    void eraseType(unsigned int type);
///Attaches a group of targets to this order (used for strategery-type games)
    bool AttachOrder(UnitPtr targets);
///Attaches a navigation point to this order
    bool AttachOrder(QVector target);
///Attaches a group (form up) to this order
    bool AttachSelfOrder(UnitPtr targets);
///Enqueues another order that will be executed (in parallel perhaps) when next void Execute() is called
    OrderPtr EnqueueOrder(OrderPtr ord);
///Replaces the first order of that type in the order queue
    OrderPtr ReplaceOrder(OrderPtr ord);

    bool Done() {
        return done;
    }

    int getType() {
        return type;
    }

    int getSubType() {
        return subtype;
    }

///Sets the parent of this Unit.  Any virtual functions must call this one
    virtual void SetParent(UnitParentPtr parent1) {
        parent = parent1;
    }

    UnitParentPtr GetParent() const {
        return parent;
    }

///Sends a communication message from the Unit (encapulated in c) to this unit
    virtual void Communicate(const class CommunicationMessage &c);
///processes a single message...generally called by the Messages() func
    virtual void ProcessCommMessage(class CommunicationMessage &c);
///responds (or does not) to certain messages in the message queue
    virtual void ProcessCommunicationMessages(float CommRepsonseTime, bool RemoveMessageProcessed);
/// return pointer to order or NULL if not found
    OrderPtr findOrder(OrderPtr ord);
/// erase that order from the list
    void eraseOrder(OrderPtr ord);
/// enqueue order as first order
    OrderPtr EnqueueOrderFirst(OrderPtr ord);

/// returns the orderlist (NULL for orders that haven't got any)
    virtual olist_t *getOrderList() {
        return NULL;
    }

    virtual void AdjustRelationTo(UnitPtr un, float factor);

    virtual std::string getOrderDescription() {
        return "empty";
    }

///searches the suborders recursively for the first order that has an orderlist
    Order *findOrderList();
    std::string createFullOrderDescription(int level = 0);

    void setActionString(std::string astring) {
        actionstring = astring;
    }

    std::string getActionString() {
        return actionstring;
    }

    virtual float getMood() {
        return 0;
    }

protected:
    std::string actionstring;
};
///Convenience order factory for "clicking to create an order"
class OrderFactory {
public:
    virtual int type() {
        return 0;
    }

    OrderFactory() {
    }

    virtual Order *newOrder() {
        return new Order;
    }
};

namespace Orders {

///Executes another order for a number of seconds
class ExecuteFor : public Order {
private:

///The child order to execute
    OrderPtr child;
///the time it has executed the child order for
    float time;
///the total time it can execute child order
    float maxtime;
public:
    virtual ~ExecuteFor() {
    }

public:
    ExecuteFor(OrderPtr chld, float seconds) : Order(chld->getType(), chld->getSubType()),
            child(chld),
            time(0),
            maxtime(seconds) {
    }

///Executes child order and then any suborders that may be pertinent
    void Execute();

///Removes this order
    virtual void Destroy() {
        child->Destroy();
        Order::Destroy();
    }
};

// Execute two orders simultaneously and wait until both has finished.
class Join : public Order {
public:
    Join(UnitPtr parent,
            Order *firstOrder,
            Order *secondOrder);
    void Execute();

private:
    Order *first;
    Order *second;
};

// Execute one order and prevent other orders with excludeTypes from executing at the same time.
class Sequence : public Order {
public:
    Sequence(UnitPtr parent,
            Order *order,
            unsigned int excludeTypes);
    void Execute();

private:
    Order *order;
};

} // namespace Orders

#endif

