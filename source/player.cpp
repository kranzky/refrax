#include <player.h>

//------------------------------------------------------------------------------
Player::Player()
    :
    _trail(),
    _score(0),
    _total(0),
    _refresh(true),
    _turned(false),
    _speed(false),
    _x(-1),
    _y(-1),
    _slash_key(Input::SLASH1),
    _back_key(Input::BACK1),
    _buffer(Input::CHEAT)
{
}

//------------------------------------------------------------------------------
Player::~Player()
{
}

//------------------------------------------------------------------------------
void
Player::init(int type)
{
    _speed = false;
    _trail.init();
    _trail.setSpeed(4);
    switch(type)
    {
        case 1: _trail.setType(Trail::P1); break;
        case 2: _trail.setType(Trail::P2); break;
        case 3: _trail.setType(Trail::P3); break;
        case 4: _trail.setType(Trail::P4); break;
        default: break;
    }
    _score=0;
    _total=0;
    switch(_trail.getType())
    {
        case Trail::P1:
//          _trail.setPos(WIDTH/2,0);
            _trail.setPos(WIDTH/2,HEIGHT/2);
            _trail.setHeading(Trail::E);
            _slash_key=Input::SLASH1;
            _back_key=Input::BACK1;
            break;
        case Trail::P2:
//          _trail.setPos(WIDTH-1,HEIGHT/2);
            _trail.setPos(WIDTH/2,HEIGHT/2);
            _trail.setHeading(Trail::S);
            _slash_key=Input::SLASH2;
            _back_key=Input::BACK2;
            break;
        case Trail::P3:
//          _trail.setPos(WIDTH/2,HEIGHT-1);
            _trail.setPos(WIDTH/2,HEIGHT/2);
            _trail.setHeading(Trail::W);
            _slash_key=Input::SLASH3;
            _back_key=Input::BACK3;
            break;
        case Trail::P4:
//          _trail.setPos(0,HEIGHT/2);
            _trail.setPos(WIDTH/2,HEIGHT/2);
            _trail.setHeading(Trail::N);
            _slash_key=Input::SLASH4;
            _back_key=Input::BACK4;
            break;
        default:
            break;
    }
}

//------------------------------------------------------------------------------
void
Player::update(Input& input,Grid<Uint8>& symbol,Font& font, Audio* audio)
{
    if(input.key(_slash_key))
    {
        _turned = _slash(symbol,font);
        if (!_turned)
        {
            _buffer = _slash_key;
        }
    }
    else if(input.key(_back_key))
    {
        _turned = _back(symbol,font);
        if (!_turned)
        {
            _buffer = _back_key;
        }
    }
    else if (!_turned && _buffer != Input::CHEAT)
    {
        if (_buffer == _slash_key)
        {
            _turned = _slash(symbol,font);
        }
        else
        {
            _turned = _back(symbol,font);
        }
        if (_turned) _buffer = Input::CHEAT;
    }
    _turn(symbol,font,audio);
    if (_trail.update())
    {
        _refresh = true;
    }
}

//------------------------------------------------------------------------------
void
Player::draw(Grid<Photon>& grid)
{
    _trail.draw(grid);
}

//------------------------------------------------------------------------------
bool
Player::active()
{
    return _trail.active();
}

//------------------------------------------------------------------------------
Uint8
Player::getX()
{
    return _trail.getX();
}

//------------------------------------------------------------------------------
Uint8
Player::getY()
{
    return _trail.getY();
}

//------------------------------------------------------------------------------
int
Player::getScore()
{
    return _score;
}

//------------------------------------------------------------------------------
int
Player::getTotal()
{
    return _total;
}

//------------------------------------------------------------------------------
void
Player::clearScore(bool speed)
{
    _score = 0;
    _speed = speed;
    if (!_speed) _trail.setSpeed(4);
    else _trail.setSpeed(0);
}


//------------------------------------------------------------------------------
void
Player::incScore()
{
    ++_score;
    ++_total;
    if (_speed) return;
    if(_score==1) _trail.setSpeed(3);
    if(_score==2) _trail.setSpeed(2);
    if(_score==4) _trail.setSpeed(1);
    if(_score==8) _trail.setSpeed(0);
}

//------------------------------------------------------------------------------
void
Player::addScore(int amount)
{
    _score += amount;
    _total += amount;
}

//------------------------------------------------------------------------------
Uint32
Player::getColour(SDL_Surface* surface)
{
    return _trail.getColour(surface);
}

//------------------------------------------------------------------------------
bool
Player::_slash(Grid<Uint8>& symbol,Font& font)
{
    if(symbol(_trail.getX(),_trail.getY())!=0) return false;
    Uint8 barrier = font.slash();
    symbol(_trail.getX(),_trail.getY()) = barrier;
    return true;
}

//------------------------------------------------------------------------------
bool
Player::_back(Grid<Uint8>& symbol,Font& font)
{
    if(symbol(_trail.getX(),_trail.getY())!=0) return false;
    Uint8 barrier = font.back();
    symbol(_trail.getX(),_trail.getY()) = barrier;
    return true;
}

//------------------------------------------------------------------------------
void
Player::_turn(Grid<Uint8>& symbol,Font& font, Audio* audio)
{
    if(!_refresh) return;
        if (_x != -1)
        {
            if (symbol(_x,_y)==font.slash())
            {
                symbol(_x,_y)=font.back();
            }
            else if (symbol(_x,_y)==font.back())
            {
                symbol(_x,_y)=font.slash();
            }
            _x = -1;
            _y = -1;
        }
    if(symbol(_trail.getX(),_trail.getY())==font.slash())
    {
        audio->stopSound(Audio::BOOM);
        if(_trail.getHeading()==Trail::N || _trail.getHeading()==Trail::S)
        {
            _trail.turnRight();
            audio->playSound(Audio::RIGHT);
        }
        else
        {
            _trail.turnLeft();
            audio->playSound(Audio::LEFT);
        }
        if (!_turned)
        {
        _x = _trail.getX();
        _y = _trail.getY();
        }
        _turned = false;
        _refresh = false;
    }
    else if(symbol(_trail.getX(),_trail.getY())==font.back())
    {
        audio->stopSound(Audio::BOOM);
        if(_trail.getHeading()==Trail::N || _trail.getHeading()==Trail::S)
        {
            _trail.turnLeft();
            audio->playSound(Audio::LEFT);
        }
        else
        {
            _trail.turnRight();
            audio->playSound(Audio::RIGHT);
        }
        if (!_turned)
        {
        _x = _trail.getX();
        _y = _trail.getY();
        }
        _turned = false;
        _refresh = false;
    }
}

//------------------------------------------------------------------------------
void
Player::_getHeading(int& dx, int& dy)
{
    dx = dy = 0;
    switch (_trail.getHeading())
    {
        case Trail::N: dy = -1; break;
        case Trail::E: dx =  1; break;
        case Trail::W: dx = -1; break;
        case Trail::S: dy =  1; break;
        default: break;
    }
}
