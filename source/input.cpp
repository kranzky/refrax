#include <input.h>

#include <iostream>

//------------------------------------------------------------------------------
Input::Input()
    :
    _keymap(0),
    _symbol(SDLK_CLEAR),
    _modifier(KMOD_NONE)
{
}

//------------------------------------------------------------------------------
Input::~Input()
{
}

//------------------------------------------------------------------------------
void
Input::clearSymbol()
{
    _symbol = SDLK_CLEAR;
    _modifier = KMOD_NONE;
}

//------------------------------------------------------------------------------
void
Input::poll()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        {
            if (event.type == SDL_KEYDOWN)
            {
                _symbol   = event.key.keysym.sym;
                _modifier = event.key.keysym.mod;
            }
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                {
                    _bit(&event,QUIT);
                    break;
                }
                case SDLK_F1:
                {
                    _bit(&event,PAUSE);
                    break;
                }
                case SDLK_F2:
                {
                    _bit(&event,AUDIO);
                    break;
                }
                case SDLK_F3:
                {
                    _bit(&event,WINDOW);
                    break;
                }
                case SDLK_F11:
                {
                    _bit(&event,CHEAT);
                    break;
                }
                case SDLK_F12:
                {
                    _bit(&event,LEVEL);
                    break;
                }
                case SDLK_q:
                {
                    _bit(&event,BACK1);
                    break;
                }
                case SDLK_w:
                {
                    _bit(&event,SLASH1);
                    break;
                }
                case SDLK_x:
                {
                    _bit(&event,BACK2);
                    break;
                }
                case SDLK_c:
                {
                    _bit(&event,SLASH2);
                    break;
                }
                case SDLK_n:
                {
                    _bit(&event,BACK3);
                    break;
                }
                case SDLK_m:
                {
                    _bit(&event,SLASH3);
                    break;
                }
                case SDLK_o:
                {
                    _bit(&event,BACK4);
                    break;
                }
                case SDLK_p:
                {
                    _bit(&event,SLASH4);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        if (_keymap & 522240)
        {
            _keymap |= START;
        }
    }
}

//------------------------------------------------------------------------------
SDLKey
Input::getSymbol() const
{
    return _symbol;
}

//------------------------------------------------------------------------------
SDLMod
Input::getModifier() const
{
    return _modifier;
}

//------------------------------------------------------------------------------
bool
Input::key(Code code)
{
    return (_keymap & code) != 0;
}

//------------------------------------------------------------------------------
void
Input::clear(void)
{
    _keymap = 0;
}

//------------------------------------------------------------------------------
void
Input::_bit(SDL_Event* event, Code code)
{
    if (event->type == SDL_KEYDOWN)
    {
        _keymap |= code;
    }
}
