#ifndef CURSOR_H
#define CURSOR_H

#include <grid.h>
#include <input.h>

#include <SDL.h>

#include <string>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class Cursor
{
  public:
    Cursor();
    ~Cursor();

    //--------------------------------------------------------------------------
    int getX();

    int getY();

    bool isVisible();

    void show();

    void hide();

    void update(const Input& input);

    const std::string& getString();

    bool enter();

    void reset();

  private:
    char upper(char key);
    bool        _enter;
    std::string _string;
    bool        _visible;
    int         _x;
    int         _y;
};

#endif
