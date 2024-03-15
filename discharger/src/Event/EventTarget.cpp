#include "../Event/EventTarget.h"

#include <type_traits>

#include "../Event/Dispatcher.h"
#include "../Event/Event.h"
#include "../Event/Keyboard.h"
#include "../Event/Gamepad.h"
#include "../Event/Mouse.h"
#include "../Event/State.h"

namespace Ironhead
{
    namespace Event
    {
        EventTarget::EventTarget(Dispatcher* eventDispatcher) : _eventDispatcher(eventDispatcher)
        {
        }

        EventTarget::~EventTarget()
        {
            // notify Event Dispatcher that this target was deleted, it should not process any further events for this object
            _eventDispatcher->blockEventHandlers(this);
        }

        template<typename T>
        void EventTarget::emitEvent(std::unique_ptr<T> event, const Base::Delegate<T*>& handler)
        {
            static_assert(std::is_base_of<Event, T>::value, "T should be derived from Event::Event.");
            if (handler)
            {
                event->setTarget(this);
                _eventDispatcher->scheduleEvent<T>(this, std::move(event), handler); // handler copy is necessary here
            }
        }

        template void EventTarget::emitEvent<Event>(std::unique_ptr<Event>, const Base::Delegate<Event*>&);
        template void EventTarget::emitEvent<Mouse>(std::unique_ptr<Mouse>, const Base::Delegate<Mouse*>&);
        template void EventTarget::emitEvent<Keyboard>(std::unique_ptr<Keyboard>, const Base::Delegate<Keyboard*>&);
        template void EventTarget::emitEvent<Gamepad>(std::unique_ptr<Gamepad>, const Base::Delegate<Gamepad*>&);
        template void EventTarget::emitEvent<State>(std::unique_ptr<State>, const Base::Delegate<State*>&);
    }
}
