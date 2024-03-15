#ifndef EVENT_EMITTER_H
#define EVENT_EMITTER_H

#include <memory>

#include "../Event/Handler.h"

namespace Ironhead
{
    namespace Event
    {
        class Event;
        class Dispatcher;

        class EventTarget
        {
            public:
                EventTarget(Dispatcher* eventDispatcher);
                virtual ~EventTarget();

                /**
                 * Emit given event to Event Dispatcher for delayed processing.
                 */
                template <typename T>
                void emitEvent(std::unique_ptr<T> event, const Base::Delegate<T*>& handler);

            private:
                Dispatcher* _eventDispatcher;
        };
    }
}
#endif //EVENT_EMITTER_H
