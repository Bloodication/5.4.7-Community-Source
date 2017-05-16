/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "PathGenerator.h"
#include "Map.h"
#include "Creature.h"
#include "Log.h"

////////////////// PathGenerator //////////////////
PathGenerator::PathGenerator(const Unit* owner)
    : _type(PATHFIND_BLANK)
    , _endPosition(G3D::Vector3::zero())
    , _sourceUnit(owner)
{ }

bool PathGenerator::CalculatePath(float destX, float destY, float destZ, bool /*forceDest*/)
{
    float x, y, z;
    _sourceUnit->GetPosition(x, y, z);

    if (!JadeCore::IsValidMapCoord(destX, destY, destZ) || !JadeCore::IsValidMapCoord(x, y, z))
        return false;

    G3D::Vector3 dest(destX, destY, destZ);
    SetEndPosition(dest);
    SetActualEndPosition(GetEndPosition());

    G3D::Vector3 start(x, y, z);
    SetStartPosition(start);

    //TC_LOG_DEBUG("maps", "++ PathGenerator::CalculatePath() for %u\n", _sourceUnit->GetGUIDLow());

    BuildShortcut();
    _type = PathType(PATHFIND_NORMAL | PATHFIND_NOT_USING_PATH);
    return true;
}

void PathGenerator::NormalizePath()
{
    for (Movement::PointsArray::iterator i = _pathPoints.begin(); i != _pathPoints.end(); ++i)
        _sourceUnit->UpdateAllowedPositionZ(i->x, i->y, i->z);
}

void PathGenerator::BuildShortcut()
{
    //TC_LOG_DEBUG("maps", "++ BuildShortcut :: making shortcut\n");

    Clear();

    // We break line into multiple segments of 3 unit lengths each
    _pathPoints.reserve(2);
    _pathPoints.push_back(GetStartPosition());
    _pathPoints.push_back(GetActualEndPosition());

    NormalizePath();

    _type = PATHFIND_SHORTCUT;
}
