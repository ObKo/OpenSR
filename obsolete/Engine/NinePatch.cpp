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

#include "OpenSR/NinePatch.h"
#include "OpenSR/private/NinePatch_p.h"

#include <vector>
#include <cfloat>
#include <algorithm>

#include <OpenSR/libRanger.h>

#include "OpenSR/Texture.h"
#include "OpenSR/ResourceManager.h"



namespace Rangers
{
void NinePatchPrivate::initFromDescriptor(boost::shared_ptr<NinePatchDescriptor> desc)
{
    if (!desc)
        return;

    rows = desc->rows;
    columns = desc->columns;
    sizeableColumns = desc->sizeableColumns;
    sizeableRows = desc->sizeableRows;
    tiledRegions = desc->tiledRegions;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            regions.push_back(TextureRegion(desc->regions[i * columns + j]));
        }
    }

    for (int i = 0; i < columns; i++)
        width += regions[i].texture->width() * (regions[i].u2 - regions[i].u1);
    for (int i = 0; i < rows; i++)
        height += regions[i * columns].texture->height() * (regions[i * columns].v2 - regions[i * columns].v1);

}

NinePatch::NinePatch(): Sprite(*(new NinePatchPrivate()))
{
    RANGERS_D(NinePatch);
    d->columns = 0;
    d->rows = 0;
}

NinePatch::NinePatch(boost::shared_ptr<NinePatchDescriptor> desc):
    Sprite(*(new NinePatchPrivate()))
{
    RANGERS_D(NinePatch);
    d->initFromDescriptor(desc);
    markToUpdate();
}

NinePatch::NinePatch(const std::string& name):
    Sprite(*(new NinePatchPrivate()))
{
    RANGERS_D(NinePatch);
    d->rows = 0;
    d->columns = 0;
    std::string s = suffix(name);
    if (s == "9.json")
    {
        boost::shared_ptr<NinePatchDescriptor> desc = boost::dynamic_pointer_cast<NinePatchDescriptor>(
                    ResourceManager::instance().objectManager().loadObject(name, "ninepatch"));
        if (desc)
            d->initFromDescriptor(desc);
    }
    else
    {
        d->region = TextureRegion(ResourceManager::instance().loadTexture(name));
        if (d->region.texture)
        {
            int width = d->region.texture->width();
            int height = d->region.texture->height();
            d->width = width - 2;
            d->height = height - 2;
            glBindTexture(GL_TEXTURE_2D, d->region.texture->openGLTexture());
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
                d->rows = 1;
            }
            else
            {
                if (rows[0] == 0.0f)
                {
                    firstRowSizeable = true;
                    rows.erase(rows.begin());
                }
                d->rows = rows.size() + 1;
            }

            if (!columns.size())
            {
                d->columns = 1;
            }
            else
            {
                if (columns[0] == 0.0f)
                {
                    firstColumnSizeable = true;
                    columns.erase(columns.begin());
                }
                d->columns = columns.size() + 1;
            }
            for (int i = 0; i < d->rows; i++)
            {
                if (((firstRowSizeable) && !(i & 0x1)) || ((!firstRowSizeable) && (i & 0x1)))
                {
                    d->sizeableRows.push_back(i);
                }
            }
            for (int i = 0; i < d->columns; i++)
            {
                if (((firstColumnSizeable) && !(i & 0x1)) || ((!firstColumnSizeable) && (i & 0x1)))
                {
                    d->sizeableColumns.push_back(i);
                }
            }
            for (int i = 0; i < d->rows; i++)
            {
                float vStart, vEnd;
                if (i == 0)
                    vStart = 1.0f / d->region.texture->height();
                else
                    vStart = rows[i - 1];
                if (i == (d->rows - 1))
                    vEnd = 1.0f - 1.0f / d->region.texture->height();
                else
                    vEnd = rows[i];

                for (int j = 0; j < d->columns; j++)
                {
                    float uStart, uEnd;
                    if (j == 0)
                        uStart = 1.0f / d->region.texture->width();
                    else
                        uStart = columns[j - 1];
                    if (j == (d->columns - 1))
                        uEnd = 1.0f - 1.0f / d->region.texture->width();
                    else
                        uEnd = columns[j];
                    TextureRegion r;
                    r.texture = d->region.texture;
                    r.u1 = uStart;
                    r.u2 = uEnd;
                    r.v1 = vStart;
                    r.v2 = vEnd;
                    d->regions.push_back(r);
                }
            }
        }
    }
    markToUpdate();
}

NinePatch::NinePatch(NinePatchPrivate &p): Sprite(p)
{
}

void NinePatch::processMain()
{
    RANGERS_D(NinePatch);
    Object::processMain();
    lock();

    if (!d->buffer)
    {
        glGenBuffers(1, &d->buffer);
    }

    float xOffset = 0, yOffset = 0;

    switch (d->xOrigin)
    {
    case POSITION_X_CENTER:
        xOffset = -d->width / 2.0f;
        break;
    case POSITION_X_LEFT:
        xOffset = 0;
        break;
    case POSITION_X_RIGHT:
        xOffset = -d->width;
        break;
    }

    switch (d->yOrigin)
    {
    case POSITION_Y_CENTER:
        yOffset = -d->height / 2.0f;
        break;
    case POSITION_Y_TOP:
        yOffset = 0;
        break;
    case POSITION_Y_BOTTOM:
        yOffset = -d->height;
        break;
    }

    std::vector<float> widths;
    std::vector<float> heights;

    float residue = d->width;

    for (int i = 0; i != d->columns; i++)
    {
        float width = (d->regions[i].u2 - d->regions[i].u1) * d->regions[i].texture->width();
        if (std::find(d->sizeableColumns.begin(), d->sizeableColumns.end(), i) == d->sizeableColumns.end())
            residue -= width;
        widths.push_back(width);

    }

    int size = d->sizeableColumns.size();
    if (!size)
    {
        size = d->columns;
        float total = 0;
        for (int i = 0; i < size; i++)
        {
            total += widths[i];
        }
        for (int i = 0; i < size; i++)
        {
            widths[i] = d->width * widths[i] / total;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            widths[d->sizeableColumns[i]] = residue / size;
        }
    }

    residue = d->height;

    for (int i = 0; i != d->rows; i++)
    {
        float height = (d->regions[i * d->columns].v2 - d->regions[i * d->columns].v1) * d->regions[i * d->columns].texture->height();
        if (std::find(d->sizeableRows.begin(), d->sizeableRows.end(), i) == d->sizeableRows.end())
            residue -= height;
        heights.push_back(height);
    }

    size = d->sizeableRows.size();
    if (!size)
    {
        size = d->rows;
        float total = 0;
        for (int i = 0; i < size; i++)
        {
            total += heights[i];
        }
        for (int i = 0; i < size; i++)
        {
            heights[i] = d->height * heights[i] / total;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            heights[d->sizeableRows[i]] = residue / size;
        }
    }

    d->vertexCount = d->rows * d->columns * 4;

    if (d->vertices)
        delete[] d->vertices;
    d->vertices = new Vertex[d->vertexCount];

    float y = 0.0f;
    for (int i = 0; i < d->rows; i++)
    {
        float x = 0.0f;
        for (int j = 0; j < d->columns; j++)
        {
            d->vertices[(i * d->columns + j) * 4].x = xOffset + x;
            d->vertices[(i * d->columns + j) * 4].u = d->regions[i * d->columns + j].u1;
            d->vertices[(i * d->columns + j) * 4].y = yOffset + y;
            d->vertices[(i * d->columns + j) * 4].v = d->regions[i * d->columns + j].v1;

            d->vertices[(i * d->columns + j) * 4 + 1].x = xOffset + x + widths[j];
            d->vertices[(i * d->columns + j) * 4 + 1].u = d->regions[i * d->columns + j].u2;
            d->vertices[(i * d->columns + j) * 4 + 1].y = yOffset + y;
            d->vertices[(i * d->columns + j) * 4 + 1].v = d->regions[i * d->columns + j].v1;

            d->vertices[(i * d->columns + j) * 4 + 2].x = xOffset + x + widths[j];
            d->vertices[(i * d->columns + j) * 4 + 2].u = d->regions[i * d->columns + j].u2;
            d->vertices[(i * d->columns + j) * 4 + 2].y = yOffset + y + heights[i];
            d->vertices[(i * d->columns + j) * 4 + 2].v = d->regions[i * d->columns + j].v2;

            d->vertices[(i * d->columns + j) * 4 + 3].x = xOffset + x;
            d->vertices[(i * d->columns + j) * 4 + 3].u = d->regions[i * d->columns + j].u1;
            d->vertices[(i * d->columns + j) * 4 + 3].y = yOffset + y + heights[i];
            d->vertices[(i * d->columns + j) * 4 + 3].v = d->regions[i * d->columns + j].v2;

            x += widths[j];
        }
        y += heights[i];
    }

    std::vector<int>::const_iterator end = d->tiledRegions.end();
    for (std::vector<int>::const_iterator i = d->tiledRegions.begin(); i != end; ++i)
    {
        //FIXME: Tiling works only on full texture.
        d->vertices[(*i) * 4].u = 0.0f;
        d->vertices[(*i) * 4].v = 0.0f;
        d->vertices[(*i) * 4 + 1].u = float(widths[(*i) % d->columns]) / d->regions.at(*i).texture->width();
        d->vertices[(*i) * 4 + 1].v = 0.0f;
        d->vertices[(*i) * 4 + 2].u = float(widths[(*i) % d->columns]) / d->regions.at(*i).texture->width();
        d->vertices[(*i) * 4 + 2].v = float(heights[(*i) / d->rows]) / d->regions.at(*i).texture->height();
        d->vertices[(*i) * 4 + 3].u = 0.0f;
        d->vertices[(*i) * 4 + 3].v = float(heights[(*i) / d->rows]) / d->regions.at(*i).texture->height();
    }

    glBindBuffer(GL_ARRAY_BUFFER, d->buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * d->vertexCount, d->vertices, GL_DYNAMIC_DRAW);

    delete[] d->vertices;
    d->vertices = 0;

    unlock();
}

void NinePatch::draw() const
{
    RANGERS_D(const NinePatch);
    //if (!texture)
    //    return;

    if (!d->buffer)
        return;
    if (!prepareDraw())
        return;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, d->buffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), OPENGL_BUFFER_OFFSET(sizeof(float) * 2));

    //TODO: Move tiling to fragment shader?
    std::vector<int>::const_iterator tend = d->tiledRegions.end();
    for (std::vector<int>::const_iterator ti = d->tiledRegions.begin(); ti != tend; ++ti)
    {
        glBindTexture(GL_TEXTURE_2D, d->regions[*ti].texture->openGLTexture());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    int vertexIndex = 0;
    std::vector<TextureRegion>::const_iterator end = d->regions.end();
    for (std::vector<TextureRegion>::const_iterator i = d->regions.begin(); i != end; ++i)
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

float NinePatch::normalHeight() const
{
    RANGERS_D(const NinePatch);
    float height = 0.0f;
    for (int i = 0; i < d->rows; i++)
    {
        const TextureRegion &r = d->regions.at(i * d->columns);
        if (r.texture)
            height += (r.v2 - r.v1) * r.texture->height();
    }
    return height;
}

float NinePatch::normalWidth() const
{
    RANGERS_D(const NinePatch);
    float width = 0.0f;
    for (int i = 0; i < d->columns; i++)
    {
        const TextureRegion &r = d->regions.at(i);
        if (r.texture)
            width += (r.u2 - r.u1) * r.texture->width();
    }
    return width;
}
}
