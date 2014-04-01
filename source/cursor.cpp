#include <cursor.h>

#include <cctype>

//------------------------------------------------------------------------------
Cursor::Cursor()
    :
    _enter(false),
    _string(""),
    _visible(false),
    _x(18),
    _y(22)
{
}

//------------------------------------------------------------------------------
Cursor::~Cursor()
{
}

//------------------------------------------------------------------------------
int
Cursor::getX()
{
    return _x;
}

//------------------------------------------------------------------------------
int
Cursor::getY()
{
    return _y;
}

//------------------------------------------------------------------------------
bool
Cursor::isVisible()
{
    static int count = 0;
    ++count;
    if (count % 6 == 0)
    {
        count = 0;
    }
    return _visible && count < 3;
}

//------------------------------------------------------------------------------
void
Cursor::show()
{
    _visible = true;
}

//------------------------------------------------------------------------------
void
Cursor::hide()
{
    _visible = false;
}

//------------------------------------------------------------------------------
void
Cursor::update(const Input& input)
{
    SDLKey key = input.getSymbol();
    SDLMod mod = input.getModifier();

    if ((key >= SDLK_a && key <= SDLK_z) ||
        (key >= SDLK_0 && key <= SDLK_9) ||
        key == SDLK_PERIOD ||
        key == SDLK_COMMA ||
        key == SDLK_COLON ||
        key == SDLK_SEMICOLON ||
        key == SDLK_LEFTPAREN ||
        key == SDLK_SLASH ||
        key == SDLK_BACKSLASH ||
        key == SDLK_QUESTION ||
        key == SDLK_EXCLAIM ||
        key == SDLK_RIGHTPAREN)
    {
        if (_string.size() < 3)
        {
            if (mod == KMOD_LSHIFT || mod == KMOD_RSHIFT || mod == KMOD_CAPS)
            {
                _string += upper(static_cast<char>(SDL_GetKeyName(key)[0]));
            }
            else
            {
                _string += static_cast<char>(SDL_GetKeyName(key)[0]);
            }
            ++_x;
        }
    }

    if (key == SDLK_SPACE)
    {
        if (_string.size() < 3)
        {
            _string += ' ';
            ++_x;
        }
    }

    if (key == SDLK_BACKSPACE || key == SDLK_DELETE)
    {
        if (_string.size() > 0)
        {
            _string.erase(_string.begin()+_string.size()-1,_string.end());
            --_x;
        }
    }

    if (key == SDLK_RETURN)
    {
        _enter = true;
    }
}

//------------------------------------------------------------------------------
const std::string&
Cursor::getString()
{
    static std::string tmp = "";
    tmp = _string;
    for (unsigned i = 0; i < 3-_string.size(); ++i)
    {
        tmp += " ";
    }
    return tmp;
}

//------------------------------------------------------------------------------
bool
Cursor::enter()
{
    return _enter;
}

//------------------------------------------------------------------------------
void
Cursor::reset()
{
    _enter = false;
    _string = "";
    _x = 18;
    _y = 22;
}

//------------------------------------------------------------------------------
char
Cursor::upper(char key)
{
    key = static_cast<char>(toupper(static_cast<int>(key)));
    if (key == '9') key = '(';
    if (key == '0') key = ')';
    if (key == '1') key = '!';
    if (key == '/') key = '?';
    if (key == ';') key = ':';
    return key;
}
