#include "../Event/Mouse.h"

namespace Ironhead
{
    namespace Event
    {
        Mouse::Mouse(Type type) : Event(typeToString(type))
        {
            _type = type;
        }

        Mouse::Mouse(const Mouse& event, const std::string& newName) : Event(newName)
        {
            _button = event._button;
            _position = event._position;
            _offset = event._offset;
            _shiftPressed = event._shiftPressed;
            _controlPressed = event._controlPressed;
            _altPressed = event._altPressed;
        }

        Mouse::Mouse(const Mouse& event) : Mouse(event, event._name)
        {
        }

        Mouse::~Mouse()
        {
        }

        const char* Mouse::typeToString(Mouse::Type type)
        {
            switch (type)
            {
                case Type::BUTTON_DOWN: return "mousedown";
                case Type::BUTTON_UP:   return "mouseup";
                case Type::MOVE:        return "mousemove";
                default: return "mouse";
            }
        }

        Mouse::Type Mouse::originalType() const
        {
            return _type;
        }

        bool Mouse::leftButton() const
        {
            return _button == Button::LEFT;
        }

        bool Mouse::rightButton() const
        {
            return _button == Button::RIGHT;
        }

        Mouse::Button Mouse::button() const
        {
            return _button;
        }

        void Mouse::setButton(Mouse::Button button)
        {
            _button = button;
        }

        void Mouse::setControlPressed(bool value)
        {
            this->_controlPressed = value;
        }

        bool Mouse::controlPressed() const
        {
            return _controlPressed;
        }

        void Mouse::setShiftPressed(bool value)
        {
            this->_shiftPressed = value;
        }

        bool Mouse::shiftPressed() const
        {
            return _shiftPressed;
        }

        const Point& Mouse::position() const
        {
            return _position;
        }

        void Mouse::setPosition(const Point& position)
        {
            _position = position;
        }

        const Point& Mouse::offset() const
        {
            return _offset;
        }

        void Mouse::setOffset(const Point& offset)
        {
            _offset = offset;
        }

        bool Mouse::obstacle() const
        {
            return _obstacle;
        }

        void Mouse::setObstacle(bool obstacle)
        {
            _obstacle = obstacle;
        }

        bool Mouse::altPressed() const
        {
            return _altPressed;
        }

        void Mouse::setAltPressed(bool altPressed)
        {
            _altPressed = altPressed;
        }
    }
}
