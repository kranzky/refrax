#ifndef PHOTON_H
#define PHOTON_H

#include <SDL.h>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class Photon
{
  public:
    Photon();
    ~Photon();

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    void clear();

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    void brighten(Uint8 r, Uint8 g, Uint8 b);

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    Uint32 colour(SDL_Surface* surface);

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    bool empty();

  private:
    Uint8  _n;
    Uint16 _r;
    Uint16 _g;
    Uint16 _b;
};

#endif
