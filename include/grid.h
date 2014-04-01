#ifndef GRID_H
#define GRID_H

#include <photon.h>
#include <config.h>

#include <SDL.h>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template<class SymbolT> class Grid
{
  public:
    Grid(Uint8 width = WIDTH, Uint8 height = HEIGHT);
    ~Grid();

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    Uint8 getWidth();

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    Uint8 getHeight();

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    SymbolT operator()(Uint8 x, Uint8 y) const;

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    SymbolT& operator()(Uint8 x, Uint8 y);

  private:
    int      _width;
    int      _height;
    SymbolT* _grid;
};

//------------------------------------------------------------------------------
template<class SymbolT>
Grid<SymbolT>::Grid(Uint8 width, Uint8 height)
    :
    _width(width),
    _height(height),
    _grid(0)
{
    _grid = new SymbolT [width * height];
}

//------------------------------------------------------------------------------
template<class SymbolT>
Grid<SymbolT>::~Grid()
{
    delete [] _grid;
}

//------------------------------------------------------------------------------
template<class SymbolT>
Uint8
Grid<SymbolT>::getWidth()
{
    return _width;
}

//------------------------------------------------------------------------------
template<class SymbolT>
Uint8
Grid<SymbolT>::getHeight()
{
    return _height;
}

//------------------------------------------------------------------------------
template<class SymbolT>
SymbolT
Grid<SymbolT>::operator()(Uint8 x, Uint8 y) const
{
    return _grid[x + y * _width];
}

//------------------------------------------------------------------------------
template<class SymbolT>
SymbolT&
Grid<SymbolT>::operator()(Uint8 x, Uint8 y)
{
    return _grid[x + y * _width];
}

#endif
