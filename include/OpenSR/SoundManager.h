/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_SOUNDMANAGER_H
#define RANGERS_SOUNDMANAGER_H

#include "config.h"
#include <boost/shared_ptr.hpp>
#include <map>
#include <SDL_mixer.h>

namespace Rangers
{
class Sound;
class RANGERS_ENGINE_API SoundManager
{
public:
    static SoundManager& instance();

    boost::shared_ptr<Sound> loadSound(const std::wstring& path);
    void playMusic(const std::wstring& path, bool loop);

private:
    SoundManager();
    SoundManager(const SoundManager& other);
    virtual ~SoundManager();

    std::map<std::wstring, boost::shared_ptr<Sound> > m_soundCache;
    Mix_Music *m_currentMusic;
};
}

#endif // RANGERS_SOUNDMANAGER_H
