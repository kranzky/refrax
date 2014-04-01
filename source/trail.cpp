#include <trail.h>
#include <config.h>

//------------------------------------------------------------------------------
Trail::Trail()
    :
    _delay(0),
    _speed(0),
    _type(BLAST),
    _heading(N),
    _valid(true)
{
    setPos(255, 255);
    _initColours();
}

//------------------------------------------------------------------------------
Trail::~Trail()
{
}

//------------------------------------------------------------------------------
void
Trail::init()
{
    _delay = 0;
    _valid = true;
}

//------------------------------------------------------------------------------
bool
Trail::update(void)
{
    if (!_valid)
    {
        return false;
    }
    if (_delay > 0)
    {
        --_delay;
        return false;
    }
    for (unsigned i = 7; i > 0; --i)
    {
        _x[i] = _x[i - 1];
        _y[i] = _y[i - 1];
    }
    if (_x[0] == 255 || _y[0] == 255)
    {
        return false;
    }
    if (_heading == E || _heading == NE || _heading == SE)
    {
        if (_x[0] == WIDTH - 1)
        {
            _x[0] = (_type == BLAST) ? 255 : 0;
        }
        else
        {
            ++_x[0];
        }
    }
    if (_heading == W || _heading == NW || _heading == SW)
    {
        if (_x[0] == 0)
        {
            _x[0] = (_type == BLAST) ? 255 : WIDTH - 1;
        }
        else
        {
            --_x[0];
        }
    }
    if (_heading == S || _heading == SE || _heading == SW)
    {
        if (_y[0] == HEIGHT - 1)
        {
            _y[0] = (_type == BLAST) ? 255 : 0;
        }
        else
        {
            ++_y[0];
        }
    }
    if (_heading == N || _heading == NE || _heading == NW)
    {
        if (_y[0] == 0)
        {
            _y[0] = (_type == BLAST) ? 255 : HEIGHT - 1;
        }
        else
        {
            --_y[0];
        }
    }
    if (_speed>0)
    {
        _delay = _speed;
    }
    return true;
}

//------------------------------------------------------------------------------
void
Trail::draw(Grid<Photon>& grid)
{
    if (!_valid)
    {
        return;
    }
    _valid = false;
    for (unsigned i = 0; i < 8; ++i)
    {
        if (_x[i] != 255 && _y[i] != 255)
        {
            _valid = true;
            grid(_x[i], _y[i]).brighten(_r[i], _g[i], _b[i]);
        }
    }
}

//------------------------------------------------------------------------------
bool
Trail::valid(void)
{
    return _valid;
}

//------------------------------------------------------------------------------
bool
Trail::active()
{
    return _delay==0;
}

//------------------------------------------------------------------------------
Trail::Type
Trail::getType()
{
    return _type;
}

//------------------------------------------------------------------------------
void
Trail::setType(Trail::Type type)
{
    _type = type;
    _initColours();
}

//------------------------------------------------------------------------------
Uint8
Trail::getX()
{
    return _x[0];
}

//------------------------------------------------------------------------------
Uint8
Trail::getY()
{
    return _y[0];
}

//------------------------------------------------------------------------------
void
Trail::setPos(Uint8 x, Uint8 y)
{
    _x[0] = x;
    _y[0] = y;

    for (unsigned i = 1; i < 8; ++i)
    {
        _x[i] = 255;
        _y[i] = 255;
    }
}

//------------------------------------------------------------------------------
void
Trail::setDelay(int delay)
{
    _delay = delay;
}

//------------------------------------------------------------------------------
int
Trail::getSpeed()
{
    return _speed;
}

//------------------------------------------------------------------------------
void
Trail::setSpeed(int speed)
{
    _speed = speed;
}

//------------------------------------------------------------------------------
Trail::Heading
Trail::getHeading()
{
    return _heading;
}

//------------------------------------------------------------------------------
void
Trail::setHeading(Trail::Heading heading)
{
    _heading = heading;
}

//------------------------------------------------------------------------------
void
Trail::turnLeft()
{
    if (_heading == N)
    {
        _heading = W;
    }
    else if (_heading == W)
    {
        _heading = S;
    }
    else if (_heading == S)
    {
        _heading = E;
    }
    else if (_heading == E)
    {
        _heading = N;
    }
}

//------------------------------------------------------------------------------
void
Trail::turnRight()
{
    if (_heading == N)
    {
        _heading = E;
    }
    else if (_heading == E)
    {
        _heading = S;
    }
    else if (_heading == S)
    {
        _heading = W;
    }
    else if (_heading == W)
    {
        _heading = N;
    }
}

//------------------------------------------------------------------------------
Uint32
Trail::getColour(SDL_Surface* surface)
{
    return SDL_MapRGB(surface->format, _r[0], _g[0], _b[0]);
}

//------------------------------------------------------------------------------
void
Trail::_setColour(unsigned i, Uint8 r, Uint8 g, Uint8 b)
{
    _r[i] = r;
    _g[i] = g;
    _b[i] = b;
}

//------------------------------------------------------------------------------
void
Trail::_initColours()
{
    switch(_type)
    {
        case P1:
        {
            for (unsigned i = 0; i < 8; ++i)
            {
                _setColour(i, 255 - 32 * i, 0, 0);
            }
            break;
        }
        case P2:
        {
            for (unsigned i = 0; i < 8; ++i)
            {
                _setColour(i, 0, 255 - 32 * i, 0);
            }
            break;
        }
        case P3:
        {
            for (unsigned i = 0; i < 8; ++i)
            {
                _setColour(i, 0, 0, 255 - 32 * i);
            }
            break;
        }
        case P4:
        {
            for (unsigned i = 0; i < 8; ++i)
            {
                _setColour(i, 128 - 16 * i, 128 - 16 * i, 128 - 16 * i);
            }
            break;
        }
        case BLAST:
        {
            _setColour(0, 128, 0,   0);
            _setColour(1, 128, 64,  0);
            _setColour(2, 128, 128, 0);
            _setColour(3, 64,  128, 0);
            _setColour(4, 0,   128, 0);
            _setColour(5, 0,   128, 64);
            _setColour(6, 0,   64,  128);
            _setColour(7, 0,   0,   128);
            break;
        }
        default:
        {
            break;
        }
    }
}
