#include <font.h>

//------------------------------------------------------------------------------
Font::Font(const std::string& path)
    :
    _charset(0)
{
	std::string file(path + "font.bmp");
    _charset = SDL_LoadBMP(file.c_str());

    if (!_charset)
    {
        return;
    }

    SDL_SetColorKey(_charset, SDL_SRCCOLORKEY | SDL_RLEACCEL,
                    SDL_MapRGB(_charset->format, 0, 0, 0));

    SDL_SetAlpha(_charset,SDL_SRCALPHA|SDL_RLEACCEL,192);
}

//------------------------------------------------------------------------------
Font::~Font()
{
    SDL_FreeSurface(_charset);
}

//------------------------------------------------------------------------------
Uint8
Font::slash()
{
    return _map('/');
}

//------------------------------------------------------------------------------
Uint8
Font::back()
{
    return _map('\\');
}

//------------------------------------------------------------------------------
Uint8
Font::orb()
{
    return _map('#');
}

//------------------------------------------------------------------------------
bool
Font::valid(Uint8 character)
{
    return _map(character) != 255;
}

//------------------------------------------------------------------------------
void
Font::print(Grid<Uint8>& symbol, const std::string& message, int x, int y)
{
    for(unsigned int i = 0; i < message.size(); ++i)
    {
        if(x >= symbol.getWidth())
        {
            x =  0;
            y += 1;
        }
        if(y >= symbol.getHeight())
        {
            y = 0;
        }
        symbol(x, y) = _map(message[i]);
        x += 1;
    }
}

//------------------------------------------------------------------------------
void
Font::draw(SDL_Surface* surface, unsigned x, unsigned y, Uint8 character)
{
    SDL_Rect srcchar;
    SDL_Rect dstchar;

    srcchar.w = 16;
    srcchar.h = 16;
    dstchar.w = 16;
    dstchar.h = 16;
    dstchar.x = x;
    dstchar.y = y;

    srcchar.x = ((character - 1) % 26) * 21 + 1;
    srcchar.y = ((character - 1) / 26) * 16;

    if (_charset)
    {
        SDL_BlitSurface(_charset, &srcchar, surface, &dstchar);
    }
    else
    {
        SDL_FillRect(surface,&dstchar,SDL_MapRGB(surface->format,128,128,128));
    }
}

//------------------------------------------------------------------------------
void
Font::draw(SDL_Surface* surface, unsigned x, unsigned y, const std::string& m)
{
    for(unsigned i = 0; i < m.size(); ++i)
    {
        Uint8 character = _map(static_cast<Uint8>(m[i]));
        draw(surface, x + i * 16, y, character);
    }
}

//------------------------------------------------------------------------------
void
Font::update(SDL_Surface* surface, Grid<Uint8>& symbol)
{
    for(int x = 0; x < symbol.getWidth(); ++x)
    {
        for(int y = 0; y < symbol.getHeight(); ++y)
        {
            bool flicker=false;
            if (symbol(x, y) == 255 || symbol(x, y) == 0)
            {
                continue;
            }
            if (symbol(x, y) == _map('#'))
            {
                flicker = true;
                symbol(x, y) += rand()%3+1;
            }
            draw(surface, x * 16 + 4, y * 16 + 4, symbol(x, y));
            if (flicker)
            {
                symbol(x, y) = _map('#');
            }
        }
    }
}
//------------------------------------------------------------------------------
Uint8
Font::_map(Uint8 character)
{
    if (character >= 'a' && character <= 'z')
    {
        return character - 'a' + 1;
    }
    if (character >= 'A' && character <= 'Z')
    {
        return character - 'A' + 27;
    }
    if (character >= '1' && character <= '9')
    {
        return character - '1' + 53;
    }
    if (character == ' ')
    {
        return 0;
    }
    if (character == '0')
    {
        return _map('O');
    }
    if (character == '.')
    {
        return _map('9')+1;
    }
    if (character == ':')
    {
        return _map('9')+2;
    }
    if (character == ',')
    {
        return _map('9')+3;
    }
    if (character == ';')
    {
        return _map('9')+4;
    }
    if (character == '(')
    {
        return _map('9')+5;
    }
    if (character == '*')
    {
        return _map('9')+7;
    }
    if (character == '!')
    {
        return _map('9')+8;
    }
    if (character == '?')
    {
        return _map('9')+9;
    }
    if (character == '\'')
    {
        return _map('9')+10;
    }
    if (character == ')')
    {
        return _map('9')+11;
    }
    if (character == '\\')
    {
        return _map('9')+12;
    }
    if (character == '/')
    {
        return _map('9')+13;
    }
    if (character == '#')
    {
        return _map('9')+14;
    }

    return 255;
}
