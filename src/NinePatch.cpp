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
#include <algorithm>
#include <libRanger.h>
#include "JSONHelper.h"
#include "ResourceManager.h"


namespace Rangers
{
NinePatch::NinePatch(Object *parent): Sprite(parent)
{
    m_descriptor.columns = 0;
    m_descriptor.rows = 0;
}

NinePatch::NinePatch(const NinePatchDescriptor &desc, Object *parent):
    Sprite(parent), m_descriptor(desc)
{
    for (int i = 0; i < m_descriptor.columns; i++)
        m_width += m_descriptor.regions[i].texture->width() * (m_descriptor.regions[i].u2 - m_descriptor.regions[i].u1);
    for (int i = 0; i < m_descriptor.rows; i++)
        m_height += m_descriptor.regions[i * m_descriptor.columns].texture->height() * (m_descriptor.regions[i * m_descriptor.columns].v2 - m_descriptor.regions[i * m_descriptor.columns].v1);

    markToUpdate();
}

NinePatch::NinePatch(const std::wstring& name, Object *parent): Sprite(parent)
{
    m_descriptor.rows = 0;
    m_descriptor.columns = 0;
    std::wstring s = suffix(name);
    if (s == L"9.json")
    {
        size_t dataSize;
        char *jsonData = ResourceManager::instance()->loadData(name, dataSize);
        if (jsonData)
        {
            bool parseError = false;
            m_descriptor = JSONHelper::parseNinePatch(std::string(jsonData, dataSize), parseError);
            for (int i = 0; i < m_descriptor.columns; i++)
                m_width += m_descriptor.regions[i].texture->width() * (m_descriptor.regions[i].u2 - m_descriptor.regions[i].u1);
            for (int i = 0; i < m_descriptor.rows; i++)
                m_height += m_descriptor.regions[i * m_descriptor.columns].texture->height() * (m_descriptor.regions[i * m_descriptor.columns].v2 - m_descriptor.regions[i * m_descriptor.columns].v1);
        }
    }
    else
    {
        m_region = TextureRegion(ResourceManager::instance()->loadTexture(name));
        if (m_region.texture)
        {
            int width = m_region.texture->width();
            int height = m_region.texture->height();
            m_width = width - 2;
            m_height = height - 2;
            glBindTexture(GL_TEXTURE_2D, m_region.texture->openGLTexture());
            int realWidth = (width % 4) != 0 ? width + 4 - (width % 4) : width;
            uint8_t *grayData = new uint8_t[realWidth * height];
            glGetTexImage(GL_TEXTURE_2D, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, grayData);

            bool black = false;

            std::vector<float> columns;
            std::vector<float> rows;

            for (int i = 1; i < width - 1; i++)
            {
                if ((grayData[i] > 127) && black)
                {
                    columns.push_back(float(i) / width);
                    black = false;
                }
                else if ((grayData[i] < 128) && !black)
                {
                    columns.push_back(float(i) / width);
                    black = true;
                }

            }
            black = false;
            for (int i = 1; i < height - 1; i++)
            {
                if ((grayData[i * realWidth] > 127) && black)
                {
                    rows.push_back(float(i) / height);
                    black = false;
                }
                else if ((grayData[i * realWidth] < 128) && !black)
                {
                    rows.push_back(float(i) / height);
                    black = true;
                }
            }
            delete[] grayData;

            bool firstRowSizeable = false, firstColumnSizeable = false;

            if (!rows.size())
            {
                m_descriptor.rows = 1;
            }
            else
            {
                if (rows[0] == 0.0f)
                {
                    firstRowSizeable = true;
                    rows.erase(rows.begin());
                }
                m_descriptor.rows = rows.size() + 1;
            }

            if (!columns.size())
            {
                m_descriptor.columns = 1;
            }
            else
            {
                if (columns[0] == 0.0f)
                {
                    firstColumnSizeable = true;
                    columns.erase(columns.begin());
                }
                m_descriptor.columns = columns.size() + 1;
            }
            for (int i = 0; i < m_descriptor.rows; i++)
            {
                if (((firstRowSizeable) && !(i & 0x1)) || ((!firstRowSizeable) && (i & 0x1)))
                {
                    m_descriptor.sizeableRows.push_back(i);
                }
            }
            for (int i = 0; i < m_descriptor.columns; i++)
            {
                if (((firstColumnSizeable) && !(i & 0x1)) || ((!firstColumnSizeable) && (i & 0x1)))
                {
                    m_descriptor.sizeableColumns.push_back(i);
                }
            }
            for (int i = 0; i < m_descriptor.rows; i++)
            {
                float vStart, vEnd;
                if (i == 0)
                    vStart = 1.0f / m_region.texture->height();
                else
                    vStart = rows[i - 1];
                if (i == (m_descriptor.rows - 1))
                    vEnd = 1.0f - 1.0f / m_region.texture->height();
                else
                    vEnd = rows[i];

                for (int j = 0; j < m_descriptor.columns; j++)
                {
                    float uStart, uEnd;
                    if (j == 0)
                        uStart = 1.0f / m_region.texture->width();
                    else
                        uStart = columns[j - 1];
                    if (j == (m_descriptor.columns - 1))
                        uEnd = 1.0f - 1.0f / m_region.texture->width();
                    else
                        uEnd = columns[j];
                    TextureRegion r;
                    r.texture = m_region.texture;
                    r.u1 = uStart;
                    r.u2 = uEnd;
                    r.v1 = vStart;
                    r.v2 = vEnd;
                    m_descriptor.regions.push_back(r);
                }
            }
        }
    }
    markToUpdate();
}

void NinePatch::processMain()
{
    Object::processMain();
    lock();

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

    std::vector<float> widths;
    std::vector<float> heights;

    float residue = m_width;

    for (int i = 0; i != m_descriptor.columns; i++)
    {
        float width = (m_descriptor.regions[i].u2 - m_descriptor.regions[i].u1) * m_descriptor.regions[i].texture->width();
        if (std::find(m_descriptor.sizeableColumns.begin(), m_descriptor.sizeableColumns.end(), i) == m_descriptor.sizeableColumns.end())
            residue -= width;
        widths.push_back(width);

    }

    int size = m_descriptor.sizeableColumns.size();
    if (!size)
    {
        size = m_descriptor.columns;
        float total = 0;
        for (int i = 0; i < size; i++)
        {
            total += widths[i];
        }
        for (int i = 0; i < size; i++)
        {
            widths[i] = m_width * widths[i] / total;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            widths[m_descriptor.sizeableColumns[i]] = residue / size;
        }
    }

    residue = m_height;

    for (int i = 0; i != m_descriptor.rows; i++)
    {
        float height = (m_descriptor.regions[i * m_descriptor.columns].v2 - m_descriptor.regions[i * m_descriptor.columns].v1) * m_descriptor.regions[i * m_descriptor.columns].texture->height();
        if (std::find(m_descriptor.sizeableRows.begin(), m_descriptor.sizeableRows.end(), i) == m_descriptor.sizeableRows.end())
            residue -= height;
        heights.push_back(height);
    }

    size = m_descriptor.sizeableRows.size();
    if (!size)
    {
        size = m_descriptor.rows;
        float total = 0;
        for (int i = 0; i < size; i++)
        {
            total += heights[i];
        }
        for (int i = 0; i < size; i++)
        {
            heights[i] = m_height * heights[i] / total;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            heights[m_descriptor.sizeableRows[i]] = residue / size;
        }
    }

    m_vertexCount = m_descriptor.rows * m_descriptor.columns * 4;

    if (m_vertices)
        delete[] m_vertices;
    m_vertices = new Vertex[m_vertexCount];

    float x = 0.0f, y = 0.0f;
    for (int i = 0; i < m_descriptor.rows; i++)
    {
        x = 0.0f;
        for (int j = 0; j < m_descriptor.columns; j++)
        {
            m_vertices[(i * m_descriptor.columns + j) * 4].x = xOffset + x;
            m_vertices[(i * m_descriptor.columns + j) * 4].u = m_descriptor.regions[i * m_descriptor.columns + j].u1;
            m_vertices[(i * m_descriptor.columns + j) * 4].y = yOffset + y;
            m_vertices[(i * m_descriptor.columns + j) * 4].v = m_descriptor.regions[i * m_descriptor.columns + j].v1;

            m_vertices[(i * m_descriptor.columns + j) * 4 + 1].x = xOffset + x + widths[j];
            m_vertices[(i * m_descriptor.columns + j) * 4 + 1].u = m_descriptor.regions[i * m_descriptor.columns + j].u2;
            m_vertices[(i * m_descriptor.columns + j) * 4 + 1].y = yOffset + y;
            m_vertices[(i * m_descriptor.columns + j) * 4 + 1].v = m_descriptor.regions[i * m_descriptor.columns + j].v1;

            m_vertices[(i * m_descriptor.columns + j) * 4 + 2].x = xOffset + x + widths[j];
            m_vertices[(i * m_descriptor.columns + j) * 4 + 2].u = m_descriptor.regions[i * m_descriptor.columns + j].u2;
            m_vertices[(i * m_descriptor.columns + j) * 4 + 2].y = yOffset + y + heights[i];
            m_vertices[(i * m_descriptor.columns + j) * 4 + 2].v = m_descriptor.regions[i * m_descriptor.columns + j].v2;

            m_vertices[(i * m_descriptor.columns + j) * 4 + 3].x = xOffset + x;
            m_vertices[(i * m_descriptor.columns + j) * 4 + 3].u = m_descriptor.regions[i * m_descriptor.columns + j].u1;
            m_vertices[(i * m_descriptor.columns + j) * 4 + 3].y = yOffset + y + heights[i];
            m_vertices[(i * m_descriptor.columns + j) * 4 + 3].v = m_descriptor.regions[i * m_descriptor.columns + j].v2;

            x += widths[j];
        }
        y += heights[i];
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertexCount, m_vertices, GL_DYNAMIC_DRAW);

    unlock();
}

void NinePatch::draw() const
{
    //if (!m_texture)
    //    return;

    if (!m_buffer)
        return;
    if (!prepareDraw())
        return;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 2));

    int vertexIndex = 0;
    std::vector<TextureRegion>::const_iterator end = m_descriptor.regions.end();
    for (std::vector<TextureRegion>::const_iterator i = m_descriptor.regions.begin(); i != end; ++i)
    {
        if (!(*i).texture)
            continue;

        glBindTexture(GL_TEXTURE_2D, (*i).texture->openGLTexture());
        glDrawArrays(GL_QUADS, vertexIndex, 4);
        vertexIndex += 4;
    }

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
}
}
