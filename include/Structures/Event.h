//
// Created by Roger on 10/11/2024.
//

#ifndef EVENT_H
#define EVENT_
#include <functional>
#include "Structures/EventTypes.h"

// Patro observer per a gestió d'events
// Codi tret de https://www.theimpossiblecode.com/blog/c11-generic-observer-pattern/
template <typename... Args>
class Event
{
public:
    typedef std::function<void(Args...)> CallbackType;

    class EventID
    {
    public:
        EventID() : valid(false) {}
    private:
        friend class Event<Args...>;
        explicit EventID(typename std::list<CallbackType>::iterator i)
        : iter(i), valid(true)
        {}

        typename std::list<CallbackType>::iterator iter;
        bool valid;
    };

    // register to be notified
    EventID addCB(CallbackType cb)
    {
        if (cb)
        {
            cbs.push_back(cb);
            return EventID(--cbs.end());
        }
        return EventID();
    }

    // unregister to be notified
    void delCB(EventID &id)
    {
        if (id.valid)
        {
            cbs.erase(id.iter);
        }
    }

    void broadcast(Args... args)
    {
        for (auto &cb : cbs)
        {
            cb(args...);
        }
    }

private:
    std::list<CallbackType> cbs;
};

typedef Event<int> IntEvent;
typedef Event<void> SimpleEvent;


#endif //EVENT_H
