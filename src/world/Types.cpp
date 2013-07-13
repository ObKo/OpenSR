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

#include "Types.h"
#include <libRanger.h>
#include "WorldHelper.h"

namespace
{
const uint32_t PLANET_STYLE_SIGNATURE = *((uint32_t*)"SRPS");
const uint32_t SYSTEM_STYLE_SIGNATURE = *((uint32_t*)"SRSS");
}

namespace Rangers
{
namespace World
{
Point::Point(float _x, float _y)
{
    x = _x;
    y = _y;
}

Point::Point()
{
    x = 0.0f;
    y = 0.0f;
}

PlanetStyle::PlanetStyle()
{
    hasCloud = false;
    speed = 0.0f;
    cloudSpeed = 0.0f;
    hasRing = false;
    hasRingBackground = false;
    ringOffsetX = 0.0f;
    ringOffsetY = 0.0f;
    ringBgOffsetX = 0.0f;
    ringBgOffsetY = 0.0f;
}

bool PlanetStyle::serialize(std::ostream& stream) const
{
    stream.write((const char *)&PLANET_STYLE_SIGNATURE, 4);

    if (!WorldHelper::serializeString(id, stream))
        return false;
    if (!WorldHelper::serializeString(texture, stream))
        return false;
    if (!WorldHelper::serializeString(cloud, stream))
        return false;

    stream.write((const char *)&hasCloud, sizeof(bool));
    stream.write((const char *)&speed, sizeof(float));
    stream.write((const char *)&cloudSpeed, sizeof(float));

    if (!WorldHelper::serializeString(ring, stream))
        return false;
    if (!WorldHelper::serializeString(ringBackground, stream))
        return false;

    stream.write((const char *)&hasRing, sizeof(bool));
    stream.write((const char *)&hasRingBackground, sizeof(bool));
    stream.write((const char *)&ambientColor, sizeof(int));
    stream.write((const char *)&ringOffsetX, sizeof(float));
    stream.write((const char *)&ringOffsetY, sizeof(float));
    stream.write((const char *)&ringBgOffsetX, sizeof(float));
    stream.write((const char *)&ringBgOffsetY, sizeof(float));

    if (!stream.good())
        return false;

    return true;
}

bool PlanetStyle::deserialize(std::istream& stream)
{
    uint32_t sig;
    stream.read((char *)&sig, 4);

    if (sig != PLANET_STYLE_SIGNATURE)
        return false;

    if (!WorldHelper::deserializeString(id, stream))
        return false;
    if (!WorldHelper::deserializeString(texture, stream))
        return false;
    if (!WorldHelper::deserializeString(cloud, stream))
        return false;

    stream.read((char *)&hasCloud, sizeof(bool));
    stream.read((char *)&speed, sizeof(float));
    stream.read((char *)&cloudSpeed, sizeof(float));

    if (!WorldHelper::deserializeString(ring, stream))
        return false;
    if (!WorldHelper::deserializeString(ringBackground, stream))
        return false;

    stream.read((char *)&hasRing, sizeof(bool));
    stream.read((char *)&hasRingBackground, sizeof(bool));
    stream.read((char *)&ambientColor, sizeof(int));
    stream.read((char *)&ringOffsetX, sizeof(float));
    stream.read((char *)&ringOffsetY, sizeof(float));
    stream.read((char *)&ringBgOffsetX, sizeof(float));
    stream.read((char *)&ringBgOffsetY, sizeof(float));

    if (!stream.good())
        return false;

    return true;
}

SystemStyle::SystemStyle()
{
    animated = false;
}

bool SystemStyle::serialize(std::ostream& stream) const
{
    stream.write((const char *)&SYSTEM_STYLE_SIGNATURE, 4);

    if (!WorldHelper::serializeString(id, stream))
        return false;
    if (!WorldHelper::serializeString(star, stream))
        return false;

    stream.write((const char *)&animated, sizeof(bool));
    stream.write((const char *)&color, sizeof(uint32_t));

    if (!WorldHelper::serializeString(background, stream))
        return false;

    if (!stream.good())
        return false;

    return true;
}

bool SystemStyle::deserialize(std::istream& stream)
{
    uint32_t sig;
    stream.read((char *)&sig, 4);

    if (sig != SYSTEM_STYLE_SIGNATURE)
        return false;

    if (!WorldHelper::deserializeString(id, stream))
        return false;
    if (!WorldHelper::deserializeString(star, stream))
        return false;

    stream.read((char *)&animated, sizeof(bool));
    stream.read((char *)&color, sizeof(uint32_t));

    if (!WorldHelper::deserializeString(background, stream))
        return false;

    if (!stream.good())
        return false;

    return true;
}
}
}
