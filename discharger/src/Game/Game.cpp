#include "../Game/Game.h"

#include <algorithm>
#include <sstream>
#include <ctime>
#include <memory>

#include "../Utils/Logger.h"
#include "../Graphics/Renderer.h"
#include "../Settings.h"
#include "../State/State.h"
#include "../Event/Dispatcher.h"
#include "../Event/State.h"
#include "../Sound/SoundManager.h"

namespace Ironhead
{
namespace Game
{

Game *Game::getInstance()
{
  return Base::Singleton<Game>::get();
}


Game::Game()
{
}

void Game::init()
{
    if (_initialized) return;
    (void)Settings::getInstance();

    Graphics::Renderer::getInstance()->init(Settings::getInstance()->displayMode());
    Graphics::Renderer::getInstance()->showLoadingScreen();

    _eventDispatcher = std::make_unique<Event::Dispatcher>();

    srand(time(0)); /// randomization

    // Preload assets
    Sound::SoundManager::getInstance()->init();

    _initialized = true;
}

Game::~Game()
{
//    shutdown();
}

void Game::shutdown()
{
    while (!_states.empty()) popState();
    Sound::SoundManager::getInstance()->shutdown();
    Graphics::Renderer::getInstance()->close();
#if defined(__SWITCH__)
    romfsExit();
#endif
    SDL_Quit();
}

void Game::pushState(State::State* state)
{
    _states.push_back(std::unique_ptr<State::State>(state));
    if (!state->initialized()) state->init();
    state->emitEvent(std::make_unique<Event::State>("push"), state->pushHandler());
    state->setActive(true);
    state->emitEvent(std::make_unique<Event::State>("activate"), state->activateHandler());
}

void Game::popState(bool doDelete)
{
    if (_states.size() == 0) return;

    State::State* state = _states.back().get();
    if (doDelete)
    {
        _statesForDelete.emplace_back(std::move(_states.back()));
    }
    else
    {
        _states.back().release();
    }
    _states.pop_back();
    state->setActive(false);
    state->emitEvent(std::make_unique<Event::State>("deactivate"), state->deactivateHandler());
    state->emitEvent(std::make_unique<Event::State>("pop"), state->popHandler());
}

void Game::setState(State::State* state)
{
    while (!_states.empty()) popState();
    pushState(state);
}

void Game::_updateFps()
{
  _frames++;

  if ((SDL_GetTicks() - _fpstimer) >= 500)
  {
    _fpstimer   = SDL_GetTicks();
    _fps        = _frames * 2;
    _frames = 0;
  }
}


void Game::run()
{
    LOG_INFO("Starting main loop");

    _lastTick = SDL_GetTicks();

    while (!_quit)
    {
        uint32_t delta = SDL_GetTicks() - _lastTick;
        _lastTick = SDL_GetTicks();

        handle();
        think(delta);
        render();

        _statesForDelete.clear();
        SDL_Delay(1);
    }
    LOG_INFO("Stopping main loop");
}

void Game::quit()
{
    _quit = true;
}


State::State* Game::topState(unsigned offset) const
{
    return (_states.rbegin() + offset)->get();
}

std::vector<State::State*> Game::_getVisibleStates()
{
    std::vector<State::State*> subset;
    if (_states.size() == 0)
    {
        return subset;
    }
    // we must render all states from last fullscreen state to the top of stack
    auto it = _states.end();
    do
    {
        --it;
    }
    while (it != _states.begin() && !(*it)->fullscreen());

    for (; it != _states.end(); ++it)
    {
        subset.push_back((*it).get());
    }
    return subset;
}

std::vector<State::State*> Game::_getActiveStates()
{
    // we must handle all states from top to bottom of stack
    std::vector<State::State*> subset;

    auto it = _states.rbegin();
    // active states
    for (; it != _states.rend(); ++it)
    {
        auto state = it->get();
        if (!state->active())
        {
            state->emitEvent(std::make_unique<Event::State>("activate"), state->activateHandler());
            state->setActive(true);
        }
        subset.push_back(state);
        if (state->modal() || state->fullscreen())
        {
            ++it;
            break;
        }
    }
    // not active states
    for (; it != _states.rend(); ++it)
    {
        auto state = it->get();
        if (state->active())
        {
            state->emitEvent(std::make_unique<Event::State>("deactivate"), state->activateHandler());
            state->setActive(false);
        }
    }
    return subset;
}

std::unique_ptr<Event::Event> Game::_createEventFromSDL(const SDL_Event& sdlEvent)
{
    using Mouse = Event::Mouse;
    using Keyboard = Event::Keyboard;
    using Gamepad = Event::Gamepad;
    switch (sdlEvent.type)
    {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            SDL_Keymod mods = SDL_GetModState();
            auto mouseEvent = std::make_unique<Mouse>((sdlEvent.type == SDL_MOUSEBUTTONDOWN) ? Mouse::Type::BUTTON_DOWN : Mouse::Type::BUTTON_UP);
            mouseEvent->setPosition({sdlEvent.button.x, sdlEvent.button.y});
            switch (sdlEvent.button.button)
            {
                case SDL_BUTTON_LEFT:
                    mouseEvent->setButton(Mouse::Button::LEFT);
                    break;
                case SDL_BUTTON_RIGHT:
                    mouseEvent->setButton(Mouse::Button::RIGHT);
                    break;
                case SDL_BUTTON_MIDDLE:
                    mouseEvent->setButton(Mouse::Button::MIDDLE);
                    break;
                case SDL_BUTTON_X1:
                    mouseEvent->setButton(Mouse::Button::X1);
                    break;
                case SDL_BUTTON_X2:
                    mouseEvent->setButton(Mouse::Button::X2);
                    break;
            }
            mouseEvent->setShiftPressed(mods & KMOD_SHIFT);
            mouseEvent->setControlPressed(mods & KMOD_CTRL);
            mouseEvent->setAltPressed(mods & KMOD_ALT);
            return std::move(mouseEvent);
        }
        case SDL_MOUSEMOTION:
        {
            auto mouseEvent = std::make_unique<Event::Mouse>(Mouse::Type::MOVE);
            mouseEvent->setPosition({sdlEvent.motion.x, sdlEvent.motion.y});
            mouseEvent->setOffset({sdlEvent.motion.xrel,sdlEvent.motion.yrel});
            return std::move(mouseEvent);
        }
        case SDL_KEYDOWN:
        {
            auto keyboardEvent = std::make_unique<Event::Keyboard>(Keyboard::Type::KEY_DOWN);
            keyboardEvent->setKeyCode(sdlEvent.key.keysym.sym);
            keyboardEvent->setAltPressed(sdlEvent.key.keysym.mod & KMOD_ALT);
            keyboardEvent->setShiftPressed(sdlEvent.key.keysym.mod & KMOD_SHIFT);
            keyboardEvent->setControlPressed(sdlEvent.key.keysym.mod & KMOD_CTRL);
            return std::move(keyboardEvent);
        }
        case SDL_KEYUP:
        {
            auto keyboardEvent = std::make_unique<Event::Keyboard>(Keyboard::Type::KEY_UP);
            keyboardEvent->setKeyCode(sdlEvent.key.keysym.sym);
            keyboardEvent->setAltPressed(sdlEvent.key.keysym.mod & KMOD_ALT);
            keyboardEvent->setShiftPressed(sdlEvent.key.keysym.mod & KMOD_SHIFT);
            keyboardEvent->setControlPressed(sdlEvent.key.keysym.mod & KMOD_CTRL);;

            if (keyboardEvent->keyCode() == SDLK_F12)
            {
//                renderer()->screenshot();
            }
            return std::move(keyboardEvent);
        }
        case SDL_CONTROLLERBUTTONDOWN:
        {
            auto gamepadEvent = std::make_unique<Event::Gamepad>(Gamepad::Type::BUTTON_DOWN);
            gamepadEvent->setGamepad(sdlEvent.cbutton.which);
            gamepadEvent->setButton(sdlEvent.cbutton.button);
            return std::move(gamepadEvent);
        }
        case SDL_CONTROLLERBUTTONUP:
        {
            auto gamepadEvent = std::make_unique<Event::Gamepad>(Gamepad::Type::BUTTON_UP);
            gamepadEvent->setGamepad(sdlEvent.cbutton.which);
            gamepadEvent->setButton(sdlEvent.cbutton.button);
            return std::move(gamepadEvent);
        }
        case SDL_CONTROLLERAXISMOTION:
        {
            auto gamepadEvent = std::make_unique<Event::Gamepad>(Gamepad::Type::AXIS);
            gamepadEvent->setGamepad(sdlEvent.caxis.which);
            gamepadEvent->setAxis(sdlEvent.caxis.axis);
            gamepadEvent->setAxisValue(sdlEvent.caxis.value);
            return std::move(gamepadEvent);
        }
    }
    return std::unique_ptr<Event::Event>();
}


void Game::handle()
{
//    if (_renderer->fading()) return;

    while (SDL_PollEvent(&_event))
    {
        if (_event.type == SDL_QUIT)
        {
            _quit = true;
        }
        else
        {
            auto event = _createEventFromSDL(_event);
            if (event)
            {
                for (auto state : _getActiveStates()) state->handle(event.get());
            }
        }
        // process events generate during handle()
        _eventDispatcher->processScheduledEvents();
    }

}

void Game::think(uint32_t delta)
{
    for (auto state : _getVisibleStates())
    {
        state->think(delta);
    }
    // process custom events
    _eventDispatcher->processScheduledEvents();
}

void Game::render()
{
    for (auto state : _getVisibleStates())
    {
        state->render();
    }

    if (Settings::getInstance()->displayFps())
    {
        _updateFps();

        std::string fpstext = fmt::format("{} fps", _fps);

        int x = (Graphics::Renderer::getInstance()->screenWidth - 4) - Graphics::Renderer::getInstance()->font.getWidth(fpstext);
        Graphics::Renderer::getInstance()->font.draw(x, 4, fpstext, 0xFF1F1F, true);

    }

    Graphics::Renderer::getInstance()->flip();
}

Event::Dispatcher* Game::eventDispatcher()
{
    return _eventDispatcher.get();
}


}
}
