#include <input.h>
#include <photon.h>
#include <grid.h>
#include <trail.h>
#include <player.h>
#include <font.h>
#include <audio.h>
#include <scrolltext.h>
#include <highscore.h>
#include <cursor.h>

#include <SDL.h>

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
enum Mode
{
    ATTRACT,
    PLAY,
    PAUSE,
    QUIT,
    GAMEOVER,
    HIGHSCORE,
    LEVEL
};

Mode                mode;
Grid<Photon>        grid;
Input               input;
std::vector<Player> player;
std::vector<Trail>  trail;
Grid<Uint8>         symbol;
Font*               font;
int                 timer;
Audio*              audio;
int                 level;
int                 bomb;
Scrolltext          scrolltext;
bool                notime;
bool                clocked;
bool                skip;
int                 winner;
HighScore*          highscore;
bool                showhigh;
Cursor              cursor;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int plot(SDL_Surface* surface, int x, int y, Uint32 colour);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void draw(SDL_Surface* surface);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void initMode();

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void updateMode();

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void blast(Uint8 x,Uint8 y,int delay=0, int speed=0);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void drawMode(SDL_Surface* surface);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void drawStatus(SDL_Surface* surface);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void clearSymbol(Uint8 condition = 0);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void clearGrid();

bool verifyScore(int offset=0);

//------------------------------------------------------------------------------
int
main(int argc, char** argv)
{
	std::string path(".");

    srand(time(NULL));

    if (argc > 2 )
    {
        std::cerr << "Usage: " << argv[0] << " [resource_path]" << std::endl;
        return 3;
    }

    if (argc == 2)
    {
        path = argv[1];
    }

    path +="/resource/";

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Unable to initialise SDL" << std::endl;
        return 1;
    }

    SDL_WM_SetCaption("R E F R A X", "Refrax");

    SDL_ShowCursor(SDL_DISABLE);

    const SDL_VideoInfo* video_info = SDL_GetVideoInfo();

    SDL_PixelFormat* pixel_format = video_info->vfmt;

    SDL_Surface* surface = SDL_SetVideoMode(640,480,pixel_format->BitsPerPixel,
                                            SDL_FULLSCREEN |
                                            SDL_SWSURFACE |
                                            SDL_HWPALETTE);

    SDL_EnableKeyRepeat(0,0);

    if (!surface)
    {
        std::cerr << "Unable to set video mode" << std::endl;
        return 2;
    }

    audio = new Audio(path);
    font  = new Font(path);
    highscore  = new HighScore(path);

    clearSymbol();

    mode = ATTRACT;

    level = 1;
    bomb  = 1;
    notime= false;
    clocked= false;
    skip  = false;

    initMode();

    Uint32 now  = SDL_GetTicks();
	Uint32 then = now;

    do
    {
        input.clear();
        input.poll();
        if (input.key(Input::QUIT) && mode != PAUSE && mode != HIGHSCORE)
        {
            if (mode == ATTRACT)
            {
                mode = QUIT;
            }
            else
            {
                mode = HIGHSCORE;
            }
            initMode();
        }
        if (input.key(Input::PAUSE))
        {
            if (mode == PLAY)
            {
                scrolltext.print("*** PAUSED ***", false);
                audio->pause(); 
                mode = PAUSE;
            }
            else if (mode == PAUSE)
            {
                scrolltext.clear();
                audio->pause(); 
                mode = PLAY;
            }
        }
        if (input.key(Input::WINDOW))
        {
            SDL_WM_ToggleFullScreen(surface);
        }
        if (input.key(Input::AUDIO))
        {
            audio->mute();
        }
        if (input.key(Input::LEVEL) && mode == PLAY)
        {
            mode = LEVEL;
            skip = true;
            initMode();
        }
        if (input.key(Input::CHEAT) && mode == PLAY)
        {
            audio->playSound(Audio::CHEAT);
            notime = !notime;
            if(notime) audio->stopSound(Audio::HURRY);
        }
        if (input.key(Input::START) && mode == ATTRACT)
        {
            mode = PLAY;
            initMode();
            input.clear();
        }
        updateMode();
        drawMode(surface);
		now = SDL_GetTicks();
        int delay = 72 - static_cast<int>(now - then);
        if (delay > 0) SDL_Delay(delay);
		then = now;
        SDL_Flip(surface);
    }
    while (mode != QUIT);

    delete audio;
    delete font;
    delete highscore;

    SDL_FreeSurface(surface);

    SDL_Quit();

    return 0;
}

//------------------------------------------------------------------------------
int
plot(SDL_Surface* surface, int x, int y, Uint32 colour)
{
    SDL_Rect rect;

    rect.x = x * 16 + 4;
    rect.y = y * 16 + 4;
    rect.w = 16;
    rect.h = 16;

    return SDL_FillRect(surface, &rect, colour);
}

//------------------------------------------------------------------------------
void
draw(SDL_Surface* surface)
{
    for (unsigned x = 0; x < symbol.getWidth(); ++x)
    {
        for (unsigned y = 0; y < symbol.getHeight(); ++y)
        {
            plot(surface, x, y, grid(x, y).colour(surface));
        }
    }
    
    if (cursor.isVisible())
    {
		Uint32 color = SDL_MapRGB(surface->format, 128, 128, 255);
        plot(surface, cursor.getX(), cursor.getY(), color);
    }

    if (showhigh)
    {
        highscore->draw(surface);
    }

    font->update(surface, symbol);
    scrolltext.update(surface, *font);
}

//------------------------------------------------------------------------------
void
initMode()
{
    static Mode last_mode = PAUSE;
    static bool first = true;

    if (player.size() == 0)
    {
        for (unsigned i = 0; i < 4; ++ i)
        {
            Player tmp;
            player.push_back(tmp);
        }
    }
    if (mode != PAUSE && mode != LEVEL && (last_mode != LEVEL))
    {
        trail.erase(trail.begin(), trail.end());
        if (mode != GAMEOVER)
        {
            clearSymbol();
        }
    }
    audio->musicVolume(128);
    audio->stopSound(Audio::HURRY);
    scrolltext.clear();
    showhigh = false;
    switch(mode)
    {
        case ATTRACT:
        {
            font->print(symbol, "     ###  #### #### ###   ##  #  #     ", 0,1);
            font->print(symbol, "     #  # #    #    #  # #  # #  #     ", 0,2);
            font->print(symbol, "     ###  ###  ###  ###  ####  ##      ", 0,3);
            font->print(symbol, "     #  # #    #    #  # #  # #  #     ", 0,4);
            font->print(symbol, "     #  # #### #    #  # #  # #  #     ", 0,5);

            if (first)
            {
                first = false;
                scrolltext.print("Refrax v1.0 : Monday June 23, 2003", false);
            }

            Trail tmp;
            tmp.setHeading(Trail::E);

            for (unsigned i = 0; i < 4; ++i)
            {
                tmp.setType(static_cast<Trail::Type>(Trail::P1 + i));
                tmp.setSpeed(i + 2);
                tmp.setPos(0, 17 + i);
                trail.push_back(tmp);
            }

            if (last_mode != ATTRACT)
            {
                audio->playMusic(Audio::TITLE);
                for (int i = 0 ; i < 40; ++ i)
                {
                    int x = 0;
                    int y = 0;
                    x = rand() % WIDTH;
                    y = rand() % HEIGHT;
                    blast(x, y, 0, 0);
                }
            }

            break;
        }
        case PLAY:
        {
            audio->musicVolume(64);
            if (last_mode != LEVEL)
            {
                audio->playMusic(Audio::GAME);
            }
            else
            {
                audio->stopSound(Audio::BOOM);
                audio->stopSound(Audio::BOMB);
                audio->playSound(Audio::LEVEL);
            }
            timer = TIME;

            if (last_mode != LEVEL)
            {
                for (unsigned i = 0; i < player.size(); ++i)
                {
                    player[i].init(i + 1);
                }
                level = 1;
                bomb  = 1;
                notime= false;
                clocked= false;
                skip  = false;
            }
            else
            {
                ++bomb;
                level *= 2;
                if (level > 128)
                {
                    level = 1;
                    bomb  = 1;
                    clocked = true;
                    audio->playSound(Audio::MISS);
                }
                for (unsigned i = 0; i < player.size(); ++i)
                {
                    player[i].clearScore(clocked);
                }
            }

            for (int i=0; i < bomb; ++i)
            {
            int x;
            int y; 
            do
            {
                x = rand()%symbol.getWidth();
                y = rand()%symbol.getHeight();
            }
            while (symbol(x,y) == font->orb());
            symbol(x,y) = font->orb();
            }

            break;
        }
        case LEVEL:
        {
            clearSymbol(font->orb());
            audio->musicVolume(64);
            break;
        }
        case PAUSE:
        {
            break;
        }
        case QUIT:
        {
            break;
        }
        case GAMEOVER:
        {
            audio->playMusic(Audio::OVER);
            audio->playSound(Audio::MISS);
            clearSymbol(font->orb());

           font->print(symbol, "           ###  ##   # #  ####         " ,0,8);
           font->print(symbol, "          #    #  # # # # #            " ,0,9);
           font->print(symbol, "          # ## #### # # # ###          " ,0,10);
           font->print(symbol, "          #  # #  # #   # #            " ,0,11);
           font->print(symbol, "           ### #  # #   # ####         " ,0,12);
           font->print(symbol, "           ###  #  # #### ###          " ,0,14);
           font->print(symbol, "          #   # #  # #    #  #         " ,0,15);
           font->print(symbol, "          #   # #  # ###  ###          " ,0,16);
           font->print(symbol, "          #   # #  # #    #  #         " ,0,17);
           font->print(symbol, "           ###   ##  #### #  #         " ,0,18);

            scrolltext.print("                                               ");
            scrolltext.print("        ");
            scrolltext.print("And you thought scrolly text was so eighties...");
            scrolltext.print("                                        ");
            scrolltext.print("Music kudos to Moby, aka El Mobilo, of the old ");
            scrolltext.print("Amiga demo scene: Sanity Arte forever!");
            scrolltext.print("                                          ");
            scrolltext.print("YAK, big ta for Iridis Alpha: ace pause mode!");
            scrolltext.print("                                              ");
            scrolltext.print("Onya PD for your reckless enthusiasm...");
            scrolltext.print("                                              ");
            scrolltext.print("Hi to JB and all the crew at BWS: best of luck!");
            scrolltext.print("                                               ");
            scrolltext.print("     Thanks to SDL ... without you none of this");
            scrolltext.print(" would have been possible. ");
            scrolltext.print("                                               ");
            scrolltext.print("G'day to Wil, Dave, Drew, Brewski, Tsung, Tee!");
            scrolltext.print("                                           ");
            scrolltext.print("20 GOTO 10                            ");

            Trail tmp;
            for (unsigned i = 0; i < 4; ++i)
            {
                tmp.setHeading(static_cast<Trail::Heading>(Trail::N + i * 2));
                tmp.setType(static_cast<Trail::Type>(Trail::P1 + i));
                tmp.setSpeed((i==1||i==2)?1:2);
                tmp.setPos((i<2)?4:34,(i%2==0)?4:22);
                trail.push_back(tmp);
            }

            break;
        }
        case HIGHSCORE:
        {
            audio->playMusic(Audio::SCORE);
           font->print(symbol, "           #  # ###  ### #  #          ", 0,1);
           font->print(symbol, "           #  #  #  #    #  #          ", 0,2);
           font->print(symbol, "           ####  #  # ## ####          ", 0,3);
           font->print(symbol, "           #  #  #  #  # #  #          ", 0,4);
           font->print(symbol, "           #  # ###  ### #  #          ", 0,5);
           font->print(symbol, "      ###  ###  ##  ###  ####  ###     ", 0,7);
           font->print(symbol, "     #    #    #  # #  # #    #        ", 0,8);
           font->print(symbol, "      ##  #    #  # ###  ###   ##      ", 0,9);
           font->print(symbol, "        # #    #  # #  # #       #     ", 0,10);
           font->print(symbol, "     ###   ###  ##  #  # #### ###      ", 0,11);
           font->print(symbol, "    WELL DONE! MARK YOUR TERRITORY!    ", 0,17);
            break;
        }
        default:
        {
            break;
        }
    }

    last_mode = mode;
}

//------------------------------------------------------------------------------
void
updateMode()
{
    static Mode last_mode = PAUSE;

    if (mode != PAUSE)
    {
        for (unsigned i = 0; i < trail.size(); ++i)
        {
            trail[i].update();
        }
    }
    for (int i = trail.size() - 1; i >= 0; --i)
    {
        if (!trail[i].valid())
        {
            trail.erase(trail.begin() + i, trail.begin() + i + 1);
        }
    }

    Mode remember = mode;

    switch(mode)
    {
        case ATTRACT:
        {
            static int count = 0;
            if (last_mode != mode)
            {
                count = 0;
            }
            ++count;
            if (count == 1)
            {
            showhigh = false;
           font->print(symbol, "               Written by              ", 0,8);
           font->print(symbol, "             Jason Hutchens            ", 0,9);
           font->print(symbol, "                  with                 ", 0,10);
           font->print(symbol, "             Philip Dunstan            ", 0,11);
           font->print(symbol, "                                       ", 0,12);
           font->print(symbol, "                                       ", 0,13);
           font->print(symbol, "                                       ", 0,14);
           font->print(symbol, "                CONTROLS               ", 0,15);
           font->print(symbol, "                                       ", 0,16);
           font->print(symbol, " P1 : Crazy insane light guy (Q and W) ", 0,17);
           font->print(symbol, " P2 : Welsh hippy guru coder (X and C) ", 0,18);
           font->print(symbol, " P3 : Mutant zapping beastie (N and M) ", 0,19);
           font->print(symbol, " P4 : Fluffy bleating sheepy (O and P) ", 0,20);
           font->print(symbol, "                                       ", 0,21);
           font->print(symbol, "                                       ", 0,22);
           font->print(symbol, "         Hit your key to play.         ", 0,23);
           font->print(symbol, "                                       ", 0,24);
           font->print(symbol, "                                       ", 0,25);
           font->print(symbol, "       Inspired by YAK's Deflex!       ", 0,26);
            }
            else if (count == 66)
            {
                scrolltext.clear();
            }
            else if (count == 142)
            {
                showhigh = true;
                highscore->print(symbol, *font);
           font->print(symbol, "    email: lloyd at kranzky dot com    ", 0,23);
           font->print(symbol, "       Inspired by YAK's Deflex!       ", 0,26);
            }
            else if (count == 396)
            {
                count = 0;
            }
            if (count % 13 == 0)
            {
                int x = 0;
                int y = 0;
                while (symbol(x, y) != Font::orb())
                {
                    x = 5 + rand() % 29;
                    y = 1 + rand() % 5;
                }
                blast(x, y);
            }
            break;
        }
        case PLAY:
        {
            if (!notime) --timer;
            if (timer == 0)
            {
                mode = GAMEOVER;
                initMode();
                break;
            }
            else if (timer == (TIME*23)/100 && !notime)
            {
                audio->playSound(Audio::HURRY);
            }
			unsigned i;
            for (i = 0; i < trail.size(); ++i)
            {
                if (symbol(trail[i].getX(), trail[i].getY()) == font->slash() ||
                    symbol(trail[i].getX(), trail[i].getY()) == font->back())
                {
                    symbol(trail[i].getX(), trail[i].getY()) = 0;
                    audio->playSound(Audio::ERASE);
                }
            }
            for (i = 0; i < player.size(); ++i)
            {
                player[i].update(input, symbol, *font, audio);
                if (symbol(player[i].getX(), player[i].getY()) == font->orb())
                {
                    if (!verifyScore(bomb)) do
                    {
                        Uint8 x = rand() % WIDTH;
                        Uint8 y = rand() % HEIGHT;
                        if (symbol(x, y) != 0)
                        {
                            continue;
                        }
                        symbol(x, y) = font->orb();
                        break;
                    }
                    while (true);
                    blast(player[i].getX(), player[i].getY());
                    audio->playSound(Audio::BOOM);
                    if (!notime) timer=TIME;
                    audio->stopSound(Audio::HURRY);
                    if (!notime) player[i].incScore();
                    symbol(player[i].getX(), player[i].getY()) = 0;
                    if (verifyScore())
                    {
                        winner = i;
                        mode = LEVEL;
                        initMode();
                        break;
                    }
                }
            }
            break;
        }
        case LEVEL:
        {
            int      max = WIDTH * HEIGHT;
            unsigned x   = rand() % WIDTH;
            unsigned y   = rand() % HEIGHT;
            while (max > 0)
            {
                if (symbol(x, y)==font->slash() || symbol(x, y)==font->back())
                {
                    blast(x, y);
                    symbol(x, y)=0;
                    audio->playSound(Audio::BOMB);
                    break;
                }
                --max;
                ++x;
                if (x == WIDTH)
                {
                    x = 0;
                    ++y;
                    if (y == HEIGHT)
                    {
                        y = 0;
                    }
                }
            }
            for (unsigned i = 0; i < player.size(); ++i)
            {
                player[i].update(input, symbol, *font, audio);
            }
            static int count = 0;
            if (max == 0)
            {
                ++count;
            }
            if (count == 20 || skip)
            {
                count = 0;
                mode = PLAY;
                if (!skip && !notime) player[winner].addScore(level);
                skip = false;
                initMode();
            }
            break;
        }
        case PAUSE:
        {
            break;
        }
        case QUIT:
        {
            break;
        }
        case GAMEOVER:
        {
            int      max = WIDTH * HEIGHT;
            unsigned x   = rand() % WIDTH;
            unsigned y   = rand() % HEIGHT;
            while (max > 0)
            {
                if (symbol(x, y)==font->slash() || symbol(x, y)==font->back())
                {
                    blast(x, y);
                    symbol(x, y)=0;
                    audio->playSound(Audio::BOMB);
                    break;
                }
                --max;
                ++x;
                if (x == WIDTH)
                {
                    x = 0;
                    ++y;
                    if (y == HEIGHT)
                    {
                        y = 0;
                    }
                }
            }
            static int count = 0;
            ++count;
            if (max == 0 && count % 17 == 0)
            {
                count = 0;
                int x = 0;
                int y = 0;
                while (symbol(x, y) != Font::orb())
                {
                    x = 8 + rand() % 23;
                    y = 8 + rand() % 10;
                }
                blast(x, y);
            }
            break;
        }
        case HIGHSCORE:
        {
            static bool quitting = false;
            static int fella = 0;
            static bool show = true;
            static std::string team_initials = "";
            static int total = 0;
            Trail::Type foo[] = { Trail::P1, Trail::P2, Trail::P3, Trail::P4 };
            HighScore::Class bar[] = { HighScore::P1, HighScore::P2,
                                       HighScore::P3, HighScore::P4 };
            if (show)
            {
                quitting = false;
                cursor.reset();
                cursor.show();
                input.clearSymbol();
                if (fella == 0) team_initials = "";
                show = false;
                total = 0;
                Trail tmp;
                tmp.setHeading(Trail::E);
                tmp.setType(foo[fella]);
                tmp.setPos(0,17);
                tmp.setSpeed(0);
                trail.push_back(tmp);
            }
            else if (input.key(Input::QUIT))
            {
                quitting = true;
            }
            cursor.update(input);
            input.clearSymbol();
            font->print(symbol, cursor.getString(), 18, 22);
            if (cursor.enter() || quitting)
            {
                total += player[fella].getTotal();
                if (fella > 0)
                {
                    team_initials += " ; ";
                }
                team_initials += cursor.getString();
                highscore->add(bar[fella], cursor.getString(),
                               player[fella].getTotal());
                ++fella;
                if (fella == 4)
                {
                    highscore->add(HighScore::TEAM, team_initials, total);
                    show = true;
                    fella = 0;
                    cursor.hide();
                    mode = ATTRACT;
                    initMode();
                    break;
                }
                else
                {
                    for (unsigned i = 0; i < trail.size(); ++i)
                    {
                        if (trail[i].getType() != Trail::BLAST)
                        {
                            trail[i].setType(foo[fella]);
                        }
                    }
                }
                cursor.reset();
            }
            static int count = 0;
            ++count;
            if (count % 15 == 0)
            {
            count = 0;
            int x = 0;
            int y = 0;
            while (symbol(x, y) != Font::orb())
            {
                x = 5 + rand() % 28;
                y = 0 + rand() % 11;
            }
            blast(x, y);
            }
            break;
        }
        default:
        {
            break;
        }
    }

    last_mode = remember;
}

//------------------------------------------------------------------------------
void
blast(Uint8 x, Uint8 y, int delay, int speed)
{
    if (x > WIDTH-1 || y > HEIGHT-1)
    {
        return;
    }

    Trail tmp;
    tmp.setDelay(delay);
    tmp.setSpeed(speed);
    tmp.setType(Trail::BLAST);
    tmp.setPos(x,y);

    for (unsigned i = 0; i < 8; ++i)
    {
        tmp.setHeading(static_cast<Trail::Heading>(Trail::N+i));
        trail.push_back(tmp);
    }
}

//------------------------------------------------------------------------------
void
drawMode(SDL_Surface* surface)
{
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 32, 64));

    clearGrid();

    switch(mode)
    {
        case ATTRACT:
        {
            break;
        }
        case PLAY: case PAUSE: case LEVEL:
        {
            for (unsigned i = 0; i < player.size(); ++i)
            {
                player[i].draw(grid);
            }
            drawStatus(surface);
            break;
        }
        case QUIT:
        {
            break;
        }
        case GAMEOVER:
        {
            break;
        }
        case HIGHSCORE:
        {
            break;
        }
        default:
        {
            break;
        }
    }
    for (unsigned i = 0; i < trail.size(); ++i)
    {
        trail[i].draw(grid);
    }
    draw(surface);
}

//------------------------------------------------------------------------------
void
drawStatus(SDL_Surface* surface)
{
    SDL_Rect rect;

    rect.x = 4;
    rect.y = 456;
    rect.w = 632;
    rect.h = 20;

    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 0));

    rect.w = 3;
    rect.x = 633;

    int total = 0;

    int max = 0;

	unsigned i;

    for (i = 0; i < player.size(); ++i)
    {
        total += player[i].getScore();
        if (player[i].getScore() > max)
        {
            max = player[i].getScore();
        }
    }

    rect.x -= (total - max) * (640 / level);

    if (rect.x < 633) rect.x += 7;

    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 255, 255));

    for (i = 0; i < player.size(); ++i)
    {
        rect.x = 4;
        rect.y = 457 + i * 5;
        rect.w = player[i].getScore() * (640 / level);
        if (rect.w > rect.x) rect.w -= rect.x;
        if (rect.w > 629) rect.w = 629;
        rect.h = 2;

        SDL_FillRect(surface, &rect, player[i].getColour(surface));
    }

    rect.x = 633;
    rect.y = 4;
    rect.w = 3;
    rect.h = 448;

    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 0));

    rect.h = timer * 448 / TIME;
    rect.y = 452 - rect.h;

    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 255, 0));
}

//------------------------------------------------------------------------------
void
clearSymbol(Uint8 condition)
{
    for (Uint8 x = 0; x < symbol.getWidth(); ++x)
    {
        for (Uint8 y = 0; y < symbol.getHeight(); ++y)
        {
            if (condition == 0 || symbol(x, y) == condition)
            {
                symbol(x, y) = 0;
            }
        }
    }
}

//------------------------------------------------------------------------------
void
clearGrid()
{
    for (Uint8 x = 0; x < grid.getWidth(); ++x)
    {
        for (Uint8 y = 0; y < grid.getHeight(); ++y)
        {
            grid(x,y).clear();
        }
    }
}

//------------------------------------------------------------------------------
bool
verifyScore(int offset)
{
    int total = 0;
    for (unsigned i = 0; i < player.size(); ++i)
    {
        total += player[i].getScore();
    }
    return total+offset >= level;
}
