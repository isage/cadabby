#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <memory>
#include <string>
#include <vector>

#include "../Base/Singleton.h"
//#include "../Game/Time.h"

#include <SDL.h>

namespace Ironhead
{

namespace State
{
    class State;
}

namespace Event
{
    class Event;
    class Dispatcher;
}

namespace Game
{

const float FPS = 60.;
const float msPerFrame = 1000. / FPS;
#define PX_PER_DELTA() (float)(delta)/Ironhead::Game::msPerFrame
#define INC_PER_DELTA(x) x+=PX_PER_DELTA()
#define DEC_PER_DELTA(x) x-=PX_PER_DELTA()
#define INC_TIMES_PER_DELTA(x, times) x += PX_PER_DELTA() * times
#define DEC_TIMES_PER_DELTA(x, times) x -= PX_PER_DELTA() * times

enum class Action {
    ACCEPT,
    DECLINE,
    FIRE,
    INVALID
};

static const std::string actionNames[4] = {
  "accept",
  "decline",
  "fire",
  "invalid"
};

class Game
{
public:
    static Game* getInstance();

    void shutdown();
    /**
     * Returns pointer to a state at the top of stack.
     * @param offset optional offset (1 means second from the top, and so on)
     */
    State::State* topState(unsigned offset = 0) const;
    void pushState(State::State* state);
    void setState(State::State* state);
    void popState(bool doDelete = true);

    static Action getActionFromName(std::string& name) {
        for(size_t i = 0; i != static_cast<size_t>(Action::INVALID); ++i) {
            if (name == actionNames[i]) {
                return static_cast<Action>(i);
            }
        }
        return Action::INVALID;
    }

    void run();
    void quit();
    void init();

    /**
     * @brief Handle all incoming events from OS (mouse, keyboard, etc.).
     */
    void handle();
    /**
     * @brief Process real-time logic.
     */
    void think(uint32_t delta);
    /**
     * @brief Render the game.
     */
    void render();

// TODO?    Input::Mouse* mouse() const;
    Event::Dispatcher* eventDispatcher();

protected:
    std::vector<std::unique_ptr<State::State>> _states;
    std::vector<std::unique_ptr<State::State>> _statesForDelete;
    std::unique_ptr<Event::Dispatcher> _eventDispatcher;

    bool _quit = false;
    bool _initialized = false;

    SDL_Event _event;

    std::vector<State::State*> _getVisibleStates();
    std::vector<State::State*> _getActiveStates();

private:
    friend class Base::Singleton<Game>;
    std::unique_ptr<Event::Event> _createEventFromSDL(const SDL_Event& sdlEvent);

    Game();
    ~Game();
    Game(Game const&) = delete;
    void operator=(Game const&) = delete;
    void _updateFps();

    uint32_t _lastTick;
    uint32_t _frames = 0;
    uint32_t _fps = 0;
    uint32_t _fpstimer = 0;

};


}
}


#endif // GAME_GAME_H
