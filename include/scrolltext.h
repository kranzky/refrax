#ifndef SCROLLTEXT_H
#define SCROLLTEXT_H

#include <SDL.h>

#include <string>

class Font;

class Scrolltext
{
  public:
    Scrolltext();
    ~Scrolltext();

    void print(const std::string& message, bool scroll = true);

    void clear();

    void update(SDL_Surface* surface, Font& font);

  private:
	std::string _message;
    int			_offset;
    bool		_scroll;
};

#endif
