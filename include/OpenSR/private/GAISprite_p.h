/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_GAI_SPRITE_P_H
#define RANGERS_GAI_SPRITE_P_H

#include "AnimatedSprite_p.h"

#include <vector>
#include <boost/shared_array.hpp>
#include <libRanger.h>

namespace Rangers
{
class GAISpritePrivate: public AnimatedSpritePrivate
{
    RANGERS_DECLARE_PUBLIC(GAISprite)
public:
    std::vector< boost::shared_array<char> > m_gaiFrames;
    bool m_needNextFrame;
    GAIHeader m_gaiHeader;
    GLuint m_textureBuffer;

    int m_baseFrameWidth;
    int m_baseFrameHeight;
    boost::shared_array<unsigned char> m_baseFrame;
};
}

#endif