#ifndef EVENT_EVENT_H
#define EVENT_EVENT_H

#include <string>

namespace Ironhead
{
    namespace Event
    {
        class EventTarget;

        /**
         * @brief Base event class
         *
         * This class is base for all other event classes
         */
        class Event
        {
            public:
                Event(const std::string& name);
                virtual ~Event();

                std::string name() const;
                void setName(const std::string& name);

                EventTarget* target() const;
                void setTarget(EventTarget* value);

                bool handled() const;
                /**
                 * @brief Sets that event is handled or not.
                 * If called from within handle(Event*) function, affects event capturing process by preventing OS Event to "fall down" to other elements.
                 * If called from within the event handler function, prevents other handlers of the same event to be called.
                 * @param value the handled flag.
                 */
                void setHandled(bool value = true);

            protected:
                /// Is event handled or not
                bool _handled = false;
                /// Event name
                std::string _name;
                /// Event target
                EventTarget* _target = nullptr;
        };
    }
}
#endif //EVENT_EVENT_H
