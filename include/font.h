#ifndef FONT_H
#define FONT_H

#include <grid.h>

#include <SDL.h>

#include <string>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class Font
{
  public:
    Font(const std::string& path);
    ~Font();

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    static Uint8 slash();

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    static Uint8 back();

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    static Uint8 orb();

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    bool valid(Uint8 character);

    //--------------------------------------------------------------------------
    //  Print the specified string of symbols at the given coordinates within
    //  the Grid class. This is achieved by iterating through the string,
    //  mapping each character in it to the relevant symbol index, and inserting
    //  the result into the appropriate element of the Grid class. Note that
    //  the string will wrap, both horizontally and vertically.
    //--------------------------------------------------------------------------
    void print(Grid<Uint8>& symbol, const std::string& message, int x, int y);

    void draw(SDL_Surface* surface, unsigned x, unsigned y, Uint8 character);

    void draw(SDL_Surface* surface, unsigned x,unsigned y,const std::string& m);

    //--------------------------------------------------------------------------
    //  Draw the contents of the Grid class onto the screen. All valid
    //  elements of the Grid class are mapped to coordinates within the font
    //  bitmap, and the block thus specified is blitted to the screen. In the
    //  special case of the symbol index specifying the Orb character, one of
    //  three possible Orbs is selected at "Random", resulting in cool flicker.
    //--------------------------------------------------------------------------
    void update(SDL_Surface* surface, Grid<Uint8>& symbol);

  private:
    //--------------------------------------------------------------------------
    //  Map an ASCII character code into a symbol index. This index is typically
    //  stored as an element of the Grid class. It is used to determine which
    //  section of the font bitmap should be used to represent the character.
    //--------------------------------------------------------------------------
    static Uint8 _map(Uint8 character);

    SDL_Surface* _charset;
};

#endif
