#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <SDL.h>

#include <grid.h>
#include <trail.h>

#include <string>
#include <vector>
#include <map>

class Font;

class HighScore
{
  public:

    enum Class
    {
        P1 = Trail::P1,
        P2 = Trail::P2,
        P3 = Trail::P3,
        P4 = Trail::P4,
        TEAM
    };

    HighScore(const std::string& path);
    ~HighScore();

    void print(Grid<Uint8>& symbol, Font& font);

    void draw(SDL_Surface* surface);

    void add(Class type, const std::string& initials, int score);

  private:
    class Score
    {
      public:
        Score(const std::string& initials, int score);
        ~Score();
        int getScore() const;
        const std::string& getInitials() const;
      private:
        std::string _initials;
        int         _score;
    };

    void load();
    void save();

    std::map<Class, std::vector<Score> > _table;
    std::string                          _path;
};

#endif
