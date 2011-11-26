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
#include <vector>
#include <cfloat>
#include "Log.h"

namespace
{
struct Delta
{
    Delta()
    {
        t1 = 0.0f;
        t2 = 0.0f;
        size = 0.0f;
    }
    Delta(float t1_, float t2_, float size_)
    {
        t1 = t1_;
        t2 = t2_;
        size = size_;
    }
    float t1, t2;
    float size;
};
}

namespace Rangers
{
NinePatch::NinePatch(Object *parent): Sprite(parent), m_embedded(false)
{

}

NinePatch::NinePatch(const std::wstring& resourceName, Object *parent): Sprite(resourceName, parent), m_embedded(true)
{
}

NinePatch::NinePatch(const std::wstring& resourceName, const std::vector<float> &rows, const std::vector<float> &columns, Object *parent):
    Sprite(resourceName, parent), m_embedded(false), m_rows(rows), m_columns(columns)
{

}

void NinePatch::processMain()
{
    //TODO: Move some stuff to initialization only.
    Object::processMain();

    if (!m_texture)
        return;

    lock();

    if (m_embedded && (!m_columns.size() || !m_rows.size()))
    {
        int width = m_texture->width();
        int height = m_texture->height();
        glBindTexture(GL_TEXTURE_2D, m_texture->openGLTexture());
        int realWidth = (width % 4) != 0 ? width + 4 - (width % 4) : width;
        uint8_t *grayData = new uint8_t[realWidth * height];
        glGetTexImage(GL_TEXTURE_2D, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, grayData);

        bool black = false;

        for (int i = 1; i < width - 1; i++)
        {
            if ((grayData[i] > 127) && black)
            {
                m_columns.push_back(float(i) / width);
                black = false;
            }
            else if ((grayData[i] < 128) && !black)
            {
                m_columns.push_back(float(i) / width);
                black = true;
            }

        }
        black = false;
        for (int i = 1; i < height - 1; i++)
        {
            if ((grayData[i * realWidth] > 127) && black)
            {
                m_rows.push_back(float(i) / height);
                black = false;
            }
            else if ((grayData[i * realWidth] < 128) && !black)
            {
                m_rows.push_back(float(i) / height);
                black = true;
            }
        }
        delete[] grayData;
    }

    if (!m_buffer)
    {
        glGenBuffers(1, &m_buffer);
    }

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

    float uStart = 0, uEnd = 0, vStart = 0, vEnd = 0;

    if (m_embedded)
    {
        uStart = 1.0f / m_texture->width();
        uEnd = 1.0f - 1.0f / m_texture->width();
        vStart = 1.0f / m_texture->height();
        vEnd = 1.0f - 1.0f / m_texture->height();
    }

    std::vector<Delta> widths;
    std::vector<Delta> heights;
    std::vector<int> sizeable;

    bool currentSizeable = false;
    float residue = m_width;
    int size = m_columns.size();

    for (int i = 0; i != size; i++)
    {
        if (i != 0)
        {
            if (currentSizeable)
                sizeable.push_back(i);
            else
                residue -= (m_columns[i] - m_columns[i - 1]) * m_texture->width();
            widths.push_back(Delta(m_columns[i - 1], m_columns[i], (m_columns[i] - m_columns[i - 1]) * m_texture->width()));
        }
        else
        {
            if (m_columns[0] != 0.0f)
            {
                residue -= (m_columns[0] - uStart) * m_texture->width();
                widths.push_back(Delta(uStart, m_columns[0], (m_columns[0] - uStart) * m_texture->width()));
            }
        }
        currentSizeable = !currentSizeable;
    }
    if (size)
    {
        if (currentSizeable)
            sizeable.push_back(size);
        else
            residue -= (uEnd - m_columns[size - 1]) * m_texture->width();
        widths.push_back(Delta(m_columns[size - 1], uEnd, (uEnd - m_columns[size - 1]) * m_texture->width()));
    }
    else
    {
        widths.push_back(Delta(uStart, uEnd, m_texture->width()));
        residue = 0.0f;
    }

    size = sizeable.size();
    for (int i = 0; i < size; i++)
    {
        widths[sizeable[i]].size = residue / size;
    }

    sizeable.clear();
    currentSizeable = false;
    residue = m_height;
    size = m_rows.size();

    for (int i = 0; i != size; i++)
    {
        if (i != 0)
        {

            if (currentSizeable)
                sizeable.push_back(i);
            else
                residue -= (m_rows[i] - m_rows[i - 1]) * m_texture->height();
            heights.push_back(Delta(m_rows[i - 1], m_rows[i], (m_rows[i] - m_rows[i - 1]) * m_texture->height()));
        }
        else
        {
            if (m_rows[0] != 0.0f)
            {
                residue -= (m_rows[0] - vStart)  * m_texture->height();
                heights.push_back(Delta(vStart, m_rows[0], (m_rows[0] - vStart) * m_texture->height()));
            }
        }
        currentSizeable = !currentSizeable;
    }
    if (size)
    {
        if (currentSizeable)
            sizeable.push_back(size);
        else
            residue -= (vEnd - m_rows[size - 1]) * m_texture->height();
        heights.push_back(Delta(m_rows[size - 1], vEnd, (vEnd - m_rows[size - 1]) * m_texture->height()));
    }
    else
    {
        heights.push_back(Delta(vStart, vEnd, m_texture->height()));
        residue = 0.0f;
    }

    size = sizeable.size();
    for (int i = 0; i < size; i++)
    {
        heights[sizeable[i]].size = residue / size;
    }

    m_vertexCount = heights.size() * widths.size() * 4;

    if (m_vertices)
        delete[] m_vertices;
    m_vertices = new Vertex[m_vertexCount];

    float x = 0.0f, y = 0.0f;
    for (int i = 0; i < heights.size(); i++)
    {
        x = 0.0f;
        for (int j = 0; j < widths.size(); j++)
        {
            m_vertices[(i * widths.size() + j) * 4].x = xOffset + x;
            m_vertices[(i * widths.size() + j) * 4].u = widths[j].t1;
            m_vertices[(i * widths.size() + j) * 4].y = yOffset + y;
            m_vertices[(i * widths.size() + j) * 4].v = heights[i].t1;

            m_vertices[(i * widths.size() + j) * 4 + 1].x = xOffset + x + widths[j].size;
            m_vertices[(i * widths.size() + j) * 4 + 1].u = widths[j].t2;
            m_vertices[(i * widths.size() + j) * 4 + 1].y = yOffset + y;
            m_vertices[(i * widths.size() + j) * 4 + 1].v = heights[i].t1;

            m_vertices[(i * widths.size() + j) * 4 + 2].x = xOffset + x + widths[j].size;
            m_vertices[(i * widths.size() + j) * 4 + 2].u = widths[j].t2;
            m_vertices[(i * widths.size() + j) * 4 + 2].y = yOffset + y + heights[i].size;
            m_vertices[(i * widths.size() + j) * 4 + 2].v = heights[i].t2;

            m_vertices[(i * widths.size() + j) * 4 + 3].x = xOffset + x;
            m_vertices[(i * widths.size() + j) * 4 + 3].u = widths[j].t1;
            m_vertices[(i * widths.size() + j) * 4 + 3].y = yOffset + y + heights[i].size;
            m_vertices[(i * widths.size() + j) * 4 + 3].v = heights[i].t2;

            x += widths[j].size;
        }
        y += heights[i].size;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertexCount, m_vertices, GL_DYNAMIC_DRAW);

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
