#include <SDL.h>

#include <audio.h>

std::map<Audio::Music, Mix_Music*> Audio::_music;
std::map<Audio::Sound, Mix_Chunk*> Audio::_sound;
std::map<Audio::Sound, int>        Audio::_channel;

//------------------------------------------------------------------------------
Audio::Audio(const std::string& path)
    :
    _volume(128),
    _paused(false),
    _muted(false)
{
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096);

    loadMusic(TITLE, path + "title.mod");
    loadMusic(OVER,  path + "over.mod");
    loadMusic(SCORE, path + "score.mod");
    loadMusic(GAME,  path + "game.mod");

    loadSound(LEFT,  path + "left.wav");
    loadSound(RIGHT, path + "right.wav");
    loadSound(BOOM,  path + "boom.wav");
    loadSound(HURRY, path + "hurry.wav");
    loadSound(ERASE, path + "erase.wav");
    loadSound(BOMB,  path + "bomb.wav");
    loadSound(LEVEL, path + "level.wav");
    loadSound(MISS,  path + "miss.wav");
    loadSound(CHEAT, path + "scratch.wav");

    Mix_ChannelFinished(freeChannel);
}

//------------------------------------------------------------------------------
Audio::~Audio()
{
    Mix_CloseAudio();

    Mix_FreeMusic(_music[TITLE]);
    Mix_FreeMusic(_music[OVER]);
    Mix_FreeMusic(_music[SCORE]);
    Mix_FreeMusic(_music[GAME]);

    Mix_FreeChunk(_sound[LEFT]);
    Mix_FreeChunk(_sound[RIGHT]);
    Mix_FreeChunk(_sound[BOOM]);
    Mix_FreeChunk(_sound[HURRY]);
    Mix_FreeChunk(_sound[ERASE]);
    Mix_FreeChunk(_sound[BOMB]);
    Mix_FreeChunk(_sound[LEVEL]);
    Mix_FreeChunk(_sound[MISS]);
    Mix_FreeChunk(_sound[CHEAT]);
}

//------------------------------------------------------------------------------
void
Audio::playMusic(Audio::Music music)
{
    //Mix_FadeInMusic(_music[music], -1, 200);
}

//------------------------------------------------------------------------------
void
Audio::musicVolume(int volume)
{
    _volume = volume;
    if (!_muted)
    {
        Mix_VolumeMusic(volume);
    }
}

//------------------------------------------------------------------------------
void
Audio::stopMusic()
{
    //Mix_FadeOutMusic(200);
}

//------------------------------------------------------------------------------
void
Audio::playSound(Audio::Sound sound)
{
    if (_muted)
    {
        Mix_Volume(-1, 0);
    }
    _channel[sound] = Mix_PlayChannel(-1, _sound[sound], 0);
}

//------------------------------------------------------------------------------
void
Audio::stopSound(Audio::Sound sound)
{
    if (_channel[sound] == -1) return;
    Mix_HaltChannel(_channel[sound]); 
}

//------------------------------------------------------------------------------
void
Audio::mute()
{
    _muted = !_muted;
    if (_muted)
    {
        Mix_Volume(-1, 0);
        Mix_VolumeMusic(0);
    }
    else
    {
        Mix_Volume(-1, 128);
        Mix_VolumeMusic(_volume);
    }
}

//------------------------------------------------------------------------------
void
Audio::pause()
{
    _paused = !_paused;
    if (_paused)
    {
        Mix_PauseMusic();
        Mix_Pause(-1);
    }
    else
    {
        Mix_ResumeMusic();
        Mix_Resume(-1);
    }
}

//------------------------------------------------------------------------------
void
Audio::loadMusic(Audio::Music music, const std::string& filename)
{
    Mix_Music* data = Mix_LoadMUS(filename.c_str());
    std::pair<Music, Mix_Music*> element(music, data);
    _music.insert(element);
}

//------------------------------------------------------------------------------
void
Audio::loadSound(Audio::Sound sound, const std::string& filename)
{
    Mix_Chunk* data = Mix_LoadWAV(filename.c_str());
    std::pair<Sound, Mix_Chunk*> element(sound, data);
    _sound.insert(element);
    _channel[sound] = -1;
}

//------------------------------------------------------------------------------
void
Audio::freeChannel(int channel)
{
    if (_channel[LEFT]  == channel) _channel[LEFT]  = -1;    
    if (_channel[RIGHT] == channel) _channel[RIGHT] = -1;    
    if (_channel[BOOM]  == channel) _channel[BOOM]  = -1;    
    if (_channel[HURRY] == channel) _channel[HURRY] = -1;    
    if (_channel[ERASE] == channel) _channel[ERASE] = -1;    
    if (_channel[BOMB]  == channel) _channel[BOMB]  = -1;    
    if (_channel[LEVEL] == channel) _channel[LEVEL] = -1;    
    if (_channel[MISS]  == channel) _channel[MISS]  = -1;    
    if (_channel[CHEAT] == channel) _channel[CHEAT] = -1;    
    Mix_Volume(channel, 128);
}
