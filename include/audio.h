#ifndef AUDIO_H
#define AUDIO_H

#include <SDL_mixer.h>

#include <string>
#include <map>
#include <vector>

class Audio
{
  public:
    enum Music
    {
        TITLE,
        OVER,
        SCORE,
        GAME
    };
    enum Sound
    {
        LEFT,
        RIGHT,
        BOOM,
        HURRY,
        ERASE,
        BOMB,
        LEVEL,
        MISS,
        CHEAT
    };

    Audio(const std::string& path);
    ~Audio();

    void playMusic(Music music);
    void musicVolume(int volume);
    void stopMusic();
    void playSound(Sound sound);
    void stopSound(Sound sound);
    void mute();
    void pause();

  private:
    void loadMusic(Music music, const std::string& filename);
    void loadSound(Sound sound, const std::string& filename);
    static void freeChannel(int channel);

    static std::map<Music, Mix_Music*> _music; 
    static std::map<Sound, Mix_Chunk*> _sound; 
    static std::map<Sound, int>        _channel;
    int                                _volume;
    bool                               _paused;
    bool                               _muted;
};

#endif
