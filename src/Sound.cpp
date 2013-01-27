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

#include "Sound.h"
#include "ResourceManager.h"
#include <SDL_rwops.h>
#include <SDL_mixer.h>

namespace Rangers
{
Sound::Sound(const std::wstring& path): m_chunk(0)
{
    size_t s;
    char *wav = ResourceManager::instance().loadData(path, s);
    if (wav)
    {
        m_chunk = Mix_LoadWAV_RW(SDL_RWFromMem(wav, s), 1);
        delete[] wav;
    }
}

Sound::~Sound()
{
    if (m_chunk)
        Mix_FreeChunk(m_chunk);
}

void Sound::play()
{
    Mix_PlayChannel(-1, m_chunk, 0);
}
}
