#include <highscore.h>
#include <font.h>

#include <sstream>
#include <cstdio>

//------------------------------------------------------------------------------
HighScore::HighScore(const std::string& path)
    :
    _table(),
    _path(path)
{
    load();
}

//------------------------------------------------------------------------------
HighScore::~HighScore()
{
    save();
}

//------------------------------------------------------------------------------
void
HighScore::print(Grid<Uint8>& symbol, Font& font)
{
    font.print(symbol, "     KINGS AND QUEENS OF REFRAXION     ", 0,8);
    font.print(symbol, "                                       ", 0,9);

	int i;
    for (i=0; i<5; ++i)
    {
        std::stringstream tmp;
        tmp << "       (" << i+1 << ") " << _table[TEAM][i].getInitials();
        int pad = 21 - _table[TEAM][i].getInitials().size();
        for (int k=0;k<pad;++k)
        {
            tmp << " ";
        }
        font.print(symbol, tmp.str(), 0,i+10);
    }

    font.print(symbol, "                                       ", 0,15);
    font.print(symbol, "    (1)    (2)    (3)    (4)    (5)    ", 0,16);

    Class foo[] = { P1, P2, P3, P4 };

    for (i=0; i<4; ++i)
    {
        std::stringstream tmp;
        for (int j=0; j<5; ++j)
        {
            tmp << "    " << _table[foo[i]][j].getInitials();
            int pad = 3 - _table[foo[i]][j].getInitials().size();
            for (int k=0;k<pad;++k)
            {
                tmp << " ";
            }
        }
        tmp << "    ";
        font.print(symbol, tmp.str(), 0,i+17);
    }
}

//------------------------------------------------------------------------------
void
HighScore::draw(SDL_Surface* surface)
{
    SDL_Rect rect;

    rect.x = 167;
    rect.h = 3;

    double ratio = 359 / static_cast<double>(_table[TEAM][0].getScore());

    rect.w = (Uint16)(static_cast<double>(_table[TEAM][0].getScore())*ratio+1);
    rect.y = 170;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 192, 192, 0));
    rect.w = (Uint16)(static_cast<double>(_table[TEAM][1].getScore())*ratio+1);
    rect.y = 186;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 192, 192, 0));
    rect.w = (Uint16)(static_cast<double>(_table[TEAM][2].getScore())*ratio+1);
    rect.y = 202;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 192, 192, 0));
    rect.w = (Uint16)(static_cast<double>(_table[TEAM][3].getScore())*ratio+1);
    rect.y = 218;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 192, 192, 0));
    rect.w = (Uint16)(static_cast<double>(_table[TEAM][4].getScore())*ratio+1);
    rect.y = 234;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 192, 192, 0));

    rect.w = 100;

    Class foo[] = { P1, P2, P3, P4 };

    int max = 0;
    for (int i = 0; i < 4 ; ++i )
    {
        if (_table[foo[i]][0].getScore()>=max)
        {
            max = _table[foo[i]][0].getScore();
        }
    }

    ratio = 99 / static_cast<double>(max);

    rect.y = 282;

    rect.w = (Uint16)(static_cast<double>(_table[P1][0].getScore())*ratio+1);
    rect.x = 41;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 192, 0, 0));
    rect.w = (Uint16)(static_cast<double>(_table[P1][1].getScore())*ratio+1);
    rect.x = 153;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 192, 0, 0));
    rect.w = (Uint16)(static_cast<double>(_table[P1][2].getScore())*ratio+1);
    rect.x = 265;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 192, 0, 0));
    rect.w = (Uint16)(static_cast<double>(_table[P1][3].getScore())*ratio+1);
    rect.x = 377;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 192, 0, 0));
    rect.w = (Uint16)(static_cast<double>(_table[P1][4].getScore())*ratio+1);
    rect.x = 489;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 192, 0, 0));

    rect.y = 298;

    rect.w = (Uint16)(static_cast<double>(_table[P2][0].getScore())*ratio+1);
    rect.x = 41;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 192, 0));
    rect.w = (Uint16)(static_cast<double>(_table[P2][1].getScore())*ratio+1);
    rect.x = 153;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 192, 0));
    rect.w = (Uint16)(static_cast<double>(_table[P2][2].getScore())*ratio+1);
    rect.x = 265;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 192, 0));
    rect.w = (Uint16)(static_cast<double>(_table[P2][3].getScore())*ratio+1);
    rect.x = 377;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 192, 0));
    rect.w = (Uint16)(static_cast<double>(_table[P2][4].getScore())*ratio+1);
    rect.x = 489;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 192, 0));

    rect.y = 314;

    rect.w = (Uint16)(static_cast<double>(_table[P3][0].getScore())*ratio+1);
    rect.x = 41;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 192));
    rect.w = (Uint16)(static_cast<double>(_table[P3][1].getScore())*ratio+1);
    rect.x = 153;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 192));
    rect.w = (Uint16)(static_cast<double>(_table[P3][2].getScore())*ratio+1);
    rect.x = 265;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 192));
    rect.w = (Uint16)(static_cast<double>(_table[P3][3].getScore())*ratio+1);
    rect.x = 377;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 192));
    rect.w = (Uint16)(static_cast<double>(_table[P3][4].getScore())*ratio+1);
    rect.x = 489;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 192));

    rect.y = 330;

    rect.w = (Uint16)(static_cast<double>(_table[P4][0].getScore())*ratio+1);
    rect.x = 41;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 96, 96, 96));
    rect.w = (Uint16)(static_cast<double>(_table[P4][1].getScore())*ratio+1);
    rect.x = 153;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 96, 96, 96));
    rect.w = (Uint16)(static_cast<double>(_table[P4][2].getScore())*ratio+1);
    rect.x = 265;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 96, 96, 96));
    rect.w = (Uint16)(static_cast<double>(_table[P4][3].getScore())*ratio+1);
    rect.x = 377;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 96, 96, 96));
    rect.w = (Uint16)(static_cast<double>(_table[P4][4].getScore())*ratio+1);
    rect.x = 489;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 96, 96, 96));
}

//------------------------------------------------------------------------------
void
HighScore::add(Class type, const std::string& initials, int score)
{
    Score myscore(initials, score);
    unsigned i;
    for (i=0; i < _table[type].size(); ++i)
    {
        if (myscore.getScore() >= _table[type][i].getScore())
        {
            break;
        }
    }
    _table[type].insert(_table[type].begin()+i, myscore);
    while (_table[type].size() > 5)
    {
        _table[type].pop_back();
    }
}

//------------------------------------------------------------------------------
void
HighScore::load()
{
    Class foo[] = { P1, P2, P3, P4, TEAM };
    FILE* file = fopen((_path + "scores.dat").c_str(), "rb");
    if (file == NULL)
    {
        add(TEAM, "", 1);
        add(TEAM, "", 1);
        add(TEAM, "", 1);
        add(TEAM, "", 1);
        add(TEAM, "", 1);

        add(P1, "", 1);
        add(P1, "", 1);
        add(P1, "", 1);
        add(P1, "", 1);
        add(P1, "", 1);

        add(P2, "", 1);
        add(P2, "", 1);
        add(P2, "", 1);
        add(P2, "", 1);
        add(P2, "", 1);

        add(P3, "", 1);
        add(P3, "", 1);
        add(P3, "", 1);
        add(P3, "", 1);
        add(P3, "", 1);

        add(P4, "", 1);
        add(P4, "", 1);
        add(P4, "", 1);
        add(P4, "", 1);
        add(P4, "", 1);

        return;
    }
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            int score;
            char initials[32];
            char sz;
            fread(&score, sizeof(int), 1, file);
            fread(&sz, sizeof(char), 1, file);
            fread(initials, sizeof(char), sz, file);
            std::string tmp(initials, sz);
            add(foo[i], tmp, score);
        }
    }
    fclose(file);
}

//------------------------------------------------------------------------------
void
HighScore::save()
{
    Class foo[] = { P1, P2, P3, P4, TEAM };
    FILE* file = fopen((_path + "scores.dat").c_str(), "wb");
    if (file == NULL)
    {
        return;
    }
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            int score = _table[foo[i]][j].getScore();
            const char* initials = _table[foo[i]][j].getInitials().c_str();
            char sz = static_cast<char>(_table[foo[i]][j].getInitials().size());
            fwrite(&score, sizeof(int), 1, file);
            fwrite(&sz, sizeof(char), 1, file);
            fwrite(initials, sizeof(char), sz, file);
        }
    }
    fclose(file);
}

//------------------------------------------------------------------------------
HighScore::Score::Score(const std::string& initials, int score)
    :
    _initials(initials),
    _score(score)
{
}

//------------------------------------------------------------------------------
HighScore::Score::~Score()
{
}

//------------------------------------------------------------------------------
int
HighScore::Score::getScore() const
{
    return _score;
}

//------------------------------------------------------------------------------
const std::string& 
HighScore::Score::getInitials() const
{
    return _initials;
}
