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

#include "NinePatch.h"
#include "Texture.h"

namespace Rangers
{
NinePatch::NinePatch(Object *parent): Sprite(parent), m_embedded(false)
{
    m_centralPatch.u1 = 0;
    m_centralPatch.v1 = 0;
    m_centralPatch.u2 = 0;
    m_centralPatch.v2 = 0;
}

NinePatch::NinePatch(const std::wstring& resourceName, Object *parent): Sprite(resourceName, parent), m_embedded(true)
{
    m_centralPatch.u1 = -1.0f;
    m_centralPatch.v1 = -1.0f;
    m_centralPatch.u2 = -1.0f;
    m_centralPatch.v2 = -1.0f;
}

NinePatch::NinePatch(const std::wstring& resourceName, const TextureRegion& centralRegion, Object *parent):
    Sprite(resourceName, parent), m_embedded(false), m_centralPatch(centralRegion)
{

}

void NinePatch::processMain()
{
    Object::processMain();

    if (!m_texture)
        return;

    lock();

    if (m_embedded && ((m_centralPatch.u1 < 0) || (m_centralPatch.u2 < 0) || (m_centralPatch.v1 < 0) || (m_centralPatch.v2 < 0)))
    {
        int width = m_texture->width();
        int height = m_texture->height();
        int u1 = 0, v1 = 0, u2 = 0, v2 = 0;
        bool black = false;
        glBindTexture(GL_TEXTURE_2D, m_texture->openGLTexture());
        int realWidth = (width % 4) != 0 ? width + 4 - (width % 4) : width;
        uint8_t *grayData = new uint8_t[realWidth * height];
        glGetTexImage(GL_TEXTURE_2D, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, grayData);
        for (int i = 0; i < realWidth; i++)
        {
            if ((grayData[i] > 127) && !black)
            {
                u1 = i;
            }
            else
            {
                if (grayData[i] > 127)
                {
                    u2 = i;
                    break;
                }
                if (!black)
                    black = true;
            }

        }
        black = false;
        for (int i = 0; i < height; i++)
        {
            if ((grayData[i * realWidth] > 127) && !black)
            {
                v1 = i;
            }
            else
            {
                if (grayData[i * realWidth] > 127)
                {
                    v2 = i;
                    break;
                }
                if (!black)
                    black = true;
            }

        }
        m_centralPatch.u1 = float(u1) / width;
        m_centralPatch.u2 = float(u2) / width;
        m_centralPatch.v1 = float(v1) / height;
        m_centralPatch.v2 = float(v2) / height;
        delete[] grayData;
    }

    if (!m_buffer)
    {
        m_vertices = new Vertex[4 * 9];
        m_vertexCount = 4 * 9;
        memset(m_vertices, 0, m_vertexCount * sizeof(Vertex));

        glGenBuffers(1, &m_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertexCount, m_vertices, GL_DYNAMIC_DRAW);
        delete m_vertices;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    m_vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    float u1 = 0, u2 = 0, v1 = 0, v2 = 0;
    float xOffset = 0, yOffset = 0;

    switch (m_xOrigin)
    {
    case POSITION_X_CENTER:
        xOffset = -m_width / 2.0f;
        break;
    case POSITION_X_LEFT:
        xOffset = 0;
        break;
    case POSITION_X_RIGHT:
        xOffset = -m_width;
        break;
    }

    switch (m_yOrigin)
    {
    case POSITION_Y_CENTER:
        yOffset = -m_height / 2.0f;
        break;
    case POSITION_Y_TOP:
        yOffset = 0;
        break;
    case POSITION_Y_BOTTOM:
        yOffset = -m_height;
        break;
    }

    float width0, width1, width2;
    float height0, height1, height2;
    float uStart = 0, uEnd = 0, vStart = 0, vEnd = 0;

    if (m_embedded)
    {
        uStart = 1.0f / m_texture->width();
        uEnd = 1.0f - 1.0f / m_texture->width();
        vStart = 1.0f / m_texture->height();
        vEnd = 1.0f - 1.0f / m_texture->height();
    }

    width0 = m_texture->width() * m_centralPatch.u1;
    width2 = m_texture->width() * (1.0f - m_centralPatch.u2);
    width1 = m_width - width0 - width2;
    height0 = m_texture->height() * m_centralPatch.v1;
    height2 = m_texture->height() * (1.0f - m_centralPatch.v2);
    height1 = m_height - height0 - height2;

    m_vertices[0].x = xOffset;
    m_vertices[0].y = yOffset;
    m_vertices[0].u = uStart;
    m_vertices[0].v = vStart;
    m_vertices[1].x = xOffset + width0;
    m_vertices[1].y = yOffset;
    m_vertices[1].u = m_centralPatch.u1;
    m_vertices[1].v = vStart;
    m_vertices[2].x = xOffset + width0;
    m_vertices[2].y = yOffset + height0;
    m_vertices[2].u = m_centralPatch.u1;
    m_vertices[2].v = m_centralPatch.v1;
    m_vertices[3].x = xOffset;
    m_vertices[3].y = yOffset + height0;
    m_vertices[3].u = uStart;
    m_vertices[3].v = m_centralPatch.v1;

    m_vertices[4].x = xOffset + width0;
    m_vertices[4].y = yOffset;
    m_vertices[4].u = m_centralPatch.u1;
    m_vertices[4].v = vStart;
    m_vertices[5].x = xOffset + width0 + width1;
    m_vertices[5].y = yOffset;
    m_vertices[5].u = m_centralPatch.u2;
    m_vertices[5].v = vStart;
    m_vertices[6].x = xOffset + width0 + width1;
    m_vertices[6].y = yOffset + height0;
    m_vertices[6].u = m_centralPatch.u2;
    m_vertices[6].v = m_centralPatch.v1;
    m_vertices[7].x = xOffset + width0;
    m_vertices[7].y = yOffset + height0;
    m_vertices[7].u = m_centralPatch.u1;
    m_vertices[7].v = m_centralPatch.v1;

    m_vertices[8].x = xOffset + width0 + width1;
    m_vertices[8].y = yOffset;
    m_vertices[8].u = m_centralPatch.u2;
    m_vertices[8].v = vStart;
    m_vertices[9].x = xOffset + m_width;
    m_vertices[9].y = yOffset;
    m_vertices[9].u = uEnd;
    m_vertices[9].v = vStart;
    m_vertices[10].x = xOffset + m_width;
    m_vertices[10].y = yOffset + height0;
    m_vertices[10].u = uEnd;
    m_vertices[10].v = m_centralPatch.v1;
    m_vertices[11].x = xOffset + width0 + width1;
    m_vertices[11].y = yOffset + height0;
    m_vertices[11].u = m_centralPatch.u2;
    m_vertices[11].v = m_centralPatch.v1;

    m_vertices[12].x = xOffset;
    m_vertices[12].y = yOffset + height0;
    m_vertices[12].u = uStart;
    m_vertices[12].v = m_centralPatch.v1;
    m_vertices[13].x = xOffset + width0;
    m_vertices[13].y = yOffset + height0;
    m_vertices[13].u = m_centralPatch.u1;
    m_vertices[13].v = m_centralPatch.v1;
    m_vertices[14].x = xOffset + width0;
    m_vertices[14].y = yOffset + height0 + height1;
    m_vertices[14].u = m_centralPatch.u1;
    m_vertices[14].v = m_centralPatch.v2;
    m_vertices[15].x = xOffset;
    m_vertices[15].y = yOffset + height0 + height1;
    m_vertices[15].u = uStart;
    m_vertices[15].v = m_centralPatch.v2;

    m_vertices[16].x = xOffset + width0;
    m_vertices[16].y = yOffset + height0;
    m_vertices[16].u = m_centralPatch.u1;
    m_vertices[16].v = m_centralPatch.v1;
    m_vertices[17].x = xOffset + width0 + width1;
    m_vertices[17].y = yOffset + height0;
    m_vertices[17].u = m_centralPatch.u2;
    m_vertices[17].v = m_centralPatch.v1;
    m_vertices[18].x = xOffset + width0 + width1;
    m_vertices[18].y = yOffset + height0 + height1;
    m_vertices[18].u = m_centralPatch.u2;
    m_vertices[18].v = m_centralPatch.v2;
    m_vertices[19].x = xOffset + width0;
    m_vertices[19].y = yOffset + height0 + height1;
    m_vertices[19].u = m_centralPatch.u1;
    m_vertices[19].v = m_centralPatch.v2;

    m_vertices[20].x = xOffset + width0 + width1;
    m_vertices[20].y = yOffset + height0;
    m_vertices[20].u = m_centralPatch.u2;
    m_vertices[20].v = m_centralPatch.v1;
    m_vertices[21].x = xOffset + m_width;
    m_vertices[21].y = yOffset + height0;
    m_vertices[21].u = uEnd;
    m_vertices[21].v = m_centralPatch.v1;
    m_vertices[22].x = xOffset + m_width;
    m_vertices[22].y = yOffset + height0 + height1;
    m_vertices[22].u = uEnd;
    m_vertices[22].v = m_centralPatch.v2;
    m_vertices[23].x = xOffset + width0 + width1;
    m_vertices[23].y = yOffset + height0 + height1;
    m_vertices[23].u = m_centralPatch.u2;
    m_vertices[23].v = m_centralPatch.v2;

    m_vertices[24].x = xOffset;
    m_vertices[24].y = yOffset + height0 + height1;
    m_vertices[24].u = uStart;
    m_vertices[24].v = m_centralPatch.v2;
    m_vertices[25].x = xOffset + width0;
    m_vertices[25].y = yOffset + height0 + height1;
    m_vertices[25].u = m_centralPatch.u1;
    m_vertices[25].v = m_centralPatch.v2;
    m_vertices[26].x = xOffset + width0;
    m_vertices[26].y = yOffset + m_height;
    m_vertices[26].u = m_centralPatch.u1;
    m_vertices[26].v = vEnd;
    m_vertices[27].x = xOffset;
    m_vertices[27].y = yOffset + m_height;
    m_vertices[27].u = uStart;
    m_vertices[27].v = vEnd;

    m_vertices[28].x = xOffset + width0;
    m_vertices[28].y = yOffset + height0 + height1;
    m_vertices[28].u = m_centralPatch.u1;
    m_vertices[28].v = m_centralPatch.v2;
    m_vertices[29].x = xOffset + width0 + width1;
    m_vertices[29].y = yOffset + height0 + height1;
    m_vertices[29].u = m_centralPatch.u2;
    m_vertices[29].v = m_centralPatch.v2;
    m_vertices[30].x = xOffset + width0 + width1;
    m_vertices[30].y = yOffset + m_height;
    m_vertices[30].u = m_centralPatch.u2;
    m_vertices[30].v = vEnd;
    m_vertices[31].x = xOffset + width0;
    m_vertices[31].y = yOffset + m_height;
    m_vertices[31].u = m_centralPatch.u1;
    m_vertices[31].v = vEnd;

    m_vertices[32].x = xOffset + width0 + width1;
    m_vertices[32].y = yOffset + height0 + height1;
    m_vertices[32].u = m_centralPatch.u2;
    m_vertices[32].v = m_centralPatch.v2;
    m_vertices[33].x = xOffset + m_width;
    m_vertices[33].y = yOffset + height0 + height1;
    m_vertices[33].u = uEnd;
    m_vertices[33].v = m_centralPatch.v2;
    m_vertices[34].x = xOffset + m_width;
    m_vertices[34].y = yOffset + m_height;
    m_vertices[34].u = uEnd;
    m_vertices[34].v = vEnd;
    m_vertices[35].x = xOffset + width0 + width1;
    m_vertices[35].y = yOffset + m_height;
    m_vertices[35].u = m_centralPatch.u2;
    m_vertices[35].v = vEnd;

    glUnmapBuffer(GL_ARRAY_BUFFER);
    unlock();
}

void NinePatch::draw() const
{
    if (!m_texture)
        return;

    if (!prepareDraw())
        return;

    glBindTexture(GL_TEXTURE_2D, m_texture->openGLTexture());


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 2));

    glDrawArrays(GL_QUADS, 0, m_vertexCount);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
}
}
