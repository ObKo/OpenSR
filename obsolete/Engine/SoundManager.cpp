/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "OpenSR/SoundManager.h"
#include <SDL_mixer.h>

#include "OpenSR/Log.h"
#include "OpenSR/ResourceManager.h"
#include "OpenSR/Sound.h"


namespace Rangers
{
SoundManager::SoundManager(): m_currentMusic(0)
{
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024);
}


SoundManager::SoundManager(const SoundManager& other)
{
}

SoundManager::~SoundManager()
{
    if (m_currentMusic)
        Mix_FreeMusic(m_currentMusic);
    Mix_CloseAudio();
}

SoundManager& SoundManager::instance()
{
    static SoundManager manager;
    return manager;
}

boost::shared_ptr<Sound> SoundManager::loadSound(const std::string& path)
{
    std::map<std::string, boost::shared_ptr<Sound> >::const_iterator it = m_soundCache.find(path);
    if (it != m_soundCache.end())
        return it->second;

    boost::shared_ptr<Sound> sound = boost::shared_ptr<Sound>(new Sound(path));
    m_soundCache[path] = sound;
    return sound;
}

void SoundManager::playMusic(const std::string& path, bool loop)
{
    if (m_currentMusic)
        Mix_FreeMusic(m_currentMusic);

    SDL_RWops *m = ResourceManager::instance().getSDLRW(path);
    if (!m)
        return;

    m_currentMusic = Mix_LoadMUS_RW(m, 0);
    if (!m_currentMusic)
    {
        Log::error() << "Cannot play music: " << Mix_GetError();
        return;
    }
    Mix_PlayMusic(m_currentMusic, -1);
}
}
