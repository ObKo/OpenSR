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

namespace Rangers
{

class GAISprite: public AnimatedSprite
{
public:
    GAISprite(const char *data, int size, GIFrame baseFrame, Object *parent = 0);
    ~GAISprite();

    void draw();
    void processLogic(int dt);
    void setFrame(int f);
    void processMain();
    void reset();

private:
    void drawFrame(int i);
    void loadGIFrame5(const char *data, unsigned char *background, int startX, int startY, int finishX, int finishY);
    char **m_gaiFrames;
    int m_dataSize;
    bool m_needNextFrame;
    GAIHeader m_gaiHeader;
    GIFrame m_baseFrame;
    //unsigned char *m_currentData;
    GLuint m_textureBuffer;
};

}

#endif /* GAISPRITE_H_ */
