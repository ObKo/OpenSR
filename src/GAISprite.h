/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_GAISPRITE_H
#define RANGERS_GAISPRITE_H

#include "AnimatedSprite.h"
#include <vector>
#include <boost/shared_array.hpp>

namespace Rangers
{

class GAISprite: public AnimatedSprite
{
public:
    GAISprite(Object *parent = 0);
    GAISprite(const char *data, int size, GIFrame baseFrame, Object *parent = 0);
    GAISprite(const std::wstring& name, Object *parent = 0);
    GAISprite(const GAISprite &other);
    ~GAISprite();

    void draw();
    void processLogic(int dt);
    void setFrame(int f);
    void processMain();
    void reset();

    GAISprite& operator=(const GAISprite& other);

private:
    void drawFrame(int i);
    void loadGIFrame5(const char *data, unsigned char *background, int startX, int startY, int finishX, int finishY);
    void loadGAI(const char *data, int size, GIFrame baseFrame);

    std::vector< boost::shared_array<char> > m_gaiFrames;
    bool m_needNextFrame;
    GAIHeader m_gaiHeader;
    GLuint m_textureBuffer;

    int m_baseFrameWidth;
    int m_baseFrameHeight;
    boost::shared_array<unsigned char> m_baseFrame;
};

}

#endif /* GAISPRITE_H_ */
