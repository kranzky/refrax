#include <scrolltext.h>
#include <font.h>

//------------------------------------------------------------------------------
Scrolltext::Scrolltext()
    :
    _message(""),
    _offset(640)
{
}

//------------------------------------------------------------------------------
Scrolltext::~Scrolltext()
{
}

//------------------------------------------------------------------------------
void
Scrolltext::print(const std::string& message, bool scroll)
{
    _scroll = scroll;
    _message += message;
}

//------------------------------------------------------------------------------
void
Scrolltext::clear()
{
    _message = "";
    _offset  = 640;
    _scroll  = true;
}

//------------------------------------------------------------------------------
void
Scrolltext::update(SDL_Surface* surface, Font& font)
{
    if (!_scroll)
    {
        _offset = (640 - static_cast<int>(_message.size() * 16))/2;
    }

    font.draw(surface, _offset, 459, _message);

    if (!_scroll) return;

    _offset -= 5;

    if (_offset < -static_cast<int>(_message.size() * 16))
    {
        _offset = 640;
    }
}
