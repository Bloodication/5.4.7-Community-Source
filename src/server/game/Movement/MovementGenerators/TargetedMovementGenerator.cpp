/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ByteBuffer.h"
#include "TargetedMovementGenerator.h"
#include "Errors.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "World.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "Player.h"
#include "VehicleDefines.h"
#include "MMapFactory.h"
#include "VMapFactory.h"

#include <cmath>

template<class T, typename D>
void TargetedMovementGeneratorMedium<T, D>::_setTargetLocation(T &owner, bool updateDestination)
{
	if (!i_target.isValid() || !i_target->IsInWorld())
		return;

	if (owner.HasUnitState(UNIT_STATE_NOT_MOVE) || owner.HasUnitState(UNIT_STATE_LOST_CONTROL))
		return;

	if (owner.GetTypeId() == TYPEID_UNIT)
	{
		if (owner.HasUnitState(UNIT_STATE_CASTING))
			return;

		if (!i_target->isInAccessiblePlaceFor(owner.ToCreature()))
			return;
	}

    switch((&owner)->GetEntry())
    {
        case 52498: // Beth'tilac
            if ((&owner)->GetMapId() == 720)
            {
                if ((&owner)->GetAI()->GetData(2) == 0 && (i_target->GetPositionZ() < 100.0f || i_target->IsPetGuardianStuff()))
                    return;
            }
            break;
        case 52581: // Cinderweb Drone
        case 52447: // Cinderweb Spiderling
        case 53745: // Engorged Broodling
            if ((&owner)->GetMapId() == 720)
                if (i_target->GetPositionZ() > 100.0f)
                    return;
            break;
        case 56923: // Twilight Sapper
            if ((&owner)->GetMotionMaster()->GetMotionSlot(MOTION_SLOT_CONTROLLED))
                return;
            break;
        case 45870:
        case 45871:
        case 45872:
        case 45812:
            if (Creature* pCreature = (&owner)->ToCreature())
            if (pCreature->GetHomePosition().GetExactDist2d(i_target->GetPositionX(), i_target->GetPositionY()) > 60.0f)
                return;
            break;
    }
    //! Following block of code deleted by MrSmite in issue 4891
    //! Code kept for learning and diagnostical purposes
//
//     if (i_offset && i_target->IsWithinDistInMap(&owner,2*i_offset))
//     {
//         if (!owner.movespline->Finalized())
//             return;
//
//         owner.GetPosition(x, y, z);
//     }
//     else
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	if (updateDestination || !i_path)
	{
		float dist = 0.0f;
		float size = 0.0f;

		if (!i_offset)
		{
			dist = i_target->GetCombatReach();
			size = i_target->GetCombatReach() - i_target->GetObjectSize();
		}
		else
		{
			// Pets need special handling.
			// We need to subtract GetObjectSize() because it gets added back further down the chain, and that makes pets too far away.
			// Subtracting it allows pets to properly be (GetCombatReach() + i_offset) away.
			// Only applies when i_target is pet's owner otherwise pets and mobs end up doing a "dance" while fighting.
			if (owner.isPet() && i_target->GetTypeId() == TYPEID_PLAYER)
			{
				dist = i_target->GetCombatReach();
				size = i_target->GetCombatReach() - i_target->GetObjectSize();
			}
			else
			{
				size = owner.GetObjectSize();
				dist = i_offset;
			}
		}

		G3D::Vector3 dest = !owner.movespline->Finalized() ? owner.movespline->FinalDestination() : G3D::Vector3(owner.GetPositionX(), owner.GetPositionY(), owner.GetPositionZ());

		if (i_target->GetExactDist2d(dest.x, dest.y) <= (dist + size) && i_target->IsWithinLOS(dest.x, dest.y, dest.z))
		{
			i_recalculateTravel = false;
			return;
		}

		if (owner.GetOwner() && owner.GetOwner()->ToPlayer())
		{
			i_target->GetClosePoint(x, y, z, size, (dist - 0.1f), i_angle); // To at i_offset distance from target and i_angle from target facing
		}
		else
		{
			dist = owner.GetCombatReach() - i_target->GetCombatReach();
			i_target->GetRandomContactPoint(&owner, x, y, z, 1.0f, i_target->GetCombatReach()); // to at i_offset distance from target and i_angle from target facing
		}


		if (!i_target->IsWithinLOS(x, y, z))
			i_target->GetPosition(x, y, z);
		else if (fabs(i_target->GetPositionZ() - z) >= 2.0f || fabs(z - i_target->GetPositionZ()) >= 2.0f)
			i_target->GetPosition(x, y, z);
	}
	else
	{
		// The destination has not changed, we just need to refresh the path (usually speed change)
		G3D::Vector3 end = i_path->GetEndPosition();
		x = end.x;
		y = end.y;
		z = end.z;
	}

	float _z = z;
	owner.UpdateAllowedPositionZ(x, y, z);

	VMAP::IVMapManager* vmgr = VMAP::VMapFactory::createOrGetVMapManager();
	GridMap* gmap = const_cast<Map*>(owner.GetMap())->GetGrid(x, y);
	if (vmgr && vmgr->isHeightCalcEnabled() && gmap)
	{
		float targetFloor = vmgr->getHeight(owner.GetMap()->GetId(), x, y, _z + 2.0f, 50.0f);
		float targetground = VMAP_INVALID_HEIGHT_VALUE;
		float targetgridHeight = gmap->getHeight(x, y);

		if (z + 4.0f > targetgridHeight)
			targetground = targetgridHeight;

		targetFloor = targetFloor != VMAP_INVALID_HEIGHT_VALUE ? targetFloor : targetground;

		if (_z - z >= 4.0f || _z - targetFloor >= 4.0f)
		{
			float _ownerZ = owner.GetPositionZ();
			owner.UpdateAllowedPositionZ(owner.GetPositionX(), owner.GetPositionY(), _ownerZ);

			float ownerFloor = vmgr->getHeight(owner.GetMap()->GetId(), owner.GetPositionX(), owner.GetPositionY(), owner.GetPositionZ() + 2.0f, 50.0f);
			float ownerground = VMAP_INVALID_HEIGHT_VALUE;
			float ownergridHeight = gmap->getHeight(x, y);

			if (_ownerZ + 4.0f > ownergridHeight)
				ownerground = ownergridHeight;

			ownerFloor = ownerFloor != VMAP_INVALID_HEIGHT_VALUE ? ownerFloor : ownerground;

			if (owner.GetPositionZ() - _ownerZ >= 4.0f || owner.GetPositionZ() - ownerFloor >= 4.0f)
			{
				Movement::MoveSplineInit init(owner);
				init.MoveTo(x, y, _z, false, true);
				init.Launch();
			}
			return;
		}
	}

	if (owner.GetMap()->IsInWater(x, y, z))
	{
		Movement::MoveSplineInit init(owner);
		init.MoveTo(x, y, z);
		init.Launch();
	}
	else
	{
		if (!i_path)
			i_path = new PathGenerator(&owner);
		else i_path->Clear();

		bool result = i_path->CalculatePath(x, y, z);
		if (!result || (i_path->GetPathType() & PATHFIND_NOPATH))
		{
			if (owner.HasUnitState(UNIT_STATE_FOLLOW))
			{
				Movement::MoveSplineInit init(owner);
				init.MoveTo(x, y, z, false, true);
				init.Launch();
			}
			else
				i_recalculateTravel = true;

			return;
		}

		if (!owner.movespline->Finalized())
		{
			owner.UpdateSplinePosition();
			owner.movespline->_Finalize();
		}

		Movement::MoveSplineInit init(owner);
		init.MovebyPath(i_path->GetPath());
		init.Launch();
	}

	D::_addUnitStateMove(owner);
	i_targetReached = false;
	i_recalculateTravel = false;
	owner.AddUnitState(UNIT_STATE_CHASE);
}

template<class T, typename D>
bool TargetedMovementGeneratorMedium<T,D>::Update(T &owner, const uint32 & time_diff)
{
    if (!i_target.isValid() || !i_target->IsInWorld())
        return false;

    if (!owner.isAlive())
        return true;

	if (owner.HasUnitState(UNIT_STATE_LOST_CONTROL) || owner.HasUnitState(UNIT_STATE_NOT_MOVE))
	{
		D::_clearUnitStateMove(owner);
		return true;
	}

    // prevent movement while casting spells with cast time or channel time
    if (owner.HasUnitState(UNIT_STATE_CASTING))
    {
        if (!owner.IsStopped())
            owner.StopMoving();
        return true;
    }

	// Prevent movement while casting spells with cast time or channel time. Some creatures need adding here.
	if (owner.HasUnitState(UNIT_STATE_CASTING))
	{
		bool glyphwaterelemental = false;

		if (owner.GetOwner() && owner.GetOwner()->HasAura(63090) && owner.GetCharmInfo() && owner.GetCharmInfo()->HasCommandState(COMMAND_FOLLOW) && owner.ToPet() && owner.ToPet()->HasReactState(REACT_HELPER))
			glyphwaterelemental = true;

		if (!glyphwaterelemental)
		{
			if (!owner.IsStopped())
				owner.StopMoving();

			return true;
		}
	}

	if (owner.HasUnitState(UNIT_STATE_POSSESSED))
		return false;

    // prevent crash after creature killed pet
    if (static_cast<D*>(this)->_lostTarget(owner))
    {
        D::_clearUnitStateMove(owner);
        return true;
    }

	bool targetMoved = false;
	i_recheckDistance.Update(time_diff);
	if (i_recheckDistance.Passed())
	{
		i_recheckDistance.Reset(250);

		float allowed_dist = owner.GetCombatReach();

		if (owner.HasUnitState(UNIT_STATE_FOLLOW))
			allowed_dist = i_target->GetCombatReach();

		G3D::Vector3 dest = owner.movespline->FinalDestination();
		if (!owner.movespline->Finalized())
			targetMoved = !i_target->IsWithinDist3d(dest.x, dest.y, dest.z, allowed_dist) || !i_target->IsWithinLOS(dest.x, dest.y, dest.z);
		else
			targetMoved = !i_target->IsWithinDist3d(owner.GetPositionX(), owner.GetPositionY(), owner.GetPositionZ(), allowed_dist) || !i_target->IsWithinLOS(owner.GetPositionX(), owner.GetPositionY(), owner.GetPositionZ());
	}

	if ((i_recalculateTravel || targetMoved) && !(!owner.movespline->Finalized() && owner.movespline->isParabolic()))
		_setTargetLocation(owner, true);

	if (owner.movespline->Finalized())
	{
		static_cast<D*>(this)->MovementInform(owner);
		if (i_angle == 0.0f && !owner.HasInArc(0.01f, i_target.getTarget()))
			owner.SetInFront(i_target.getTarget());

		if (!i_targetReached)
		{
			i_targetReached = true;
			static_cast<D*>(this)->_reachTarget(owner);
			//owner->SetFacingTo(i_target->GetOrientation());
		}
		if (i_path)
			i_path->Clear();
	}

	return true;
}

//-----------------------------------------------//
template<class T>
void ChaseMovementGenerator<T>::_reachTarget(T &owner)
{
    if (owner.IsWithinMeleeRange(this->i_target.getTarget()))
        owner.Attack(this->i_target.getTarget(),true);
}

template<>
void ChaseMovementGenerator<Player>::Initialize(Player &owner)
{
    owner.AddUnitState(UNIT_STATE_CHASE|UNIT_STATE_CHASE_MOVE);
    _setTargetLocation(owner, true);
}

template<>
void ChaseMovementGenerator<Creature>::Initialize(Creature &owner)
{
    owner.SetWalk(false);
    owner.AddUnitState(UNIT_STATE_CHASE|UNIT_STATE_CHASE_MOVE);
    _setTargetLocation(owner, true);
}

template<class T>
void ChaseMovementGenerator<T>::Finalize(T &owner)
{
    owner.ClearUnitState(UNIT_STATE_CHASE|UNIT_STATE_CHASE_MOVE);
}

template<class T>
void ChaseMovementGenerator<T>::Reset(T &owner)
{
    Initialize(owner);
}

template<class T>
void ChaseMovementGenerator<T>::MovementInform(T & /*unit*/)
{
}

template<>
void ChaseMovementGenerator<Creature>::MovementInform(Creature &unit)
{
    // Pass back the GUIDLow of the target. If it is pet's owner then PetAI will handle
    if (unit.AI())
        unit.AddMovementInform(CHASE_MOTION_TYPE, i_target.getTarget()->GetGUIDLow());
}

//-----------------------------------------------//
template<>
bool FollowMovementGenerator<Creature>::EnableWalking(Creature & owner) const
{
	return i_target.isValid() && i_target->IsWalking() &&
		i_target->IsWithinMeleeRange(&owner, MELEE_RANGE + PET_FOLLOW_DIST);
}

template<>
bool FollowMovementGenerator<Player>::EnableWalking(Player & /*owner*/) const
{
    return false;
}

template<>
void FollowMovementGenerator<Player>::_updateSpeed(Player &/*u*/)
{
    // nothing to do for Player
}

template<>
void FollowMovementGenerator<Creature>::_updateSpeed(Creature &u)
{
    // pet only sync speed with owner
    if (!((Creature&)u).isPet() || !i_target.isValid() || i_target->GetGUID() != u.GetOwnerGUID())
        return;

    u.UpdateSpeed(MOVE_RUN,true);
    u.UpdateSpeed(MOVE_WALK,true);
    u.UpdateSpeed(MOVE_SWIM,true);
}

template<>
void FollowMovementGenerator<Player>::Initialize(Player &owner)
{
    owner.AddUnitState(UNIT_STATE_FOLLOW|UNIT_STATE_FOLLOW_MOVE);
    _updateSpeed(owner);
    _setTargetLocation(owner, true);
}

template<>
void FollowMovementGenerator<Creature>::Initialize(Creature &owner)
{
    owner.AddUnitState(UNIT_STATE_FOLLOW|UNIT_STATE_FOLLOW_MOVE);
    _updateSpeed(owner);
    _setTargetLocation(owner, true);
}

template<class T>
void FollowMovementGenerator<T>::Finalize(T &owner)
{
    owner.ClearUnitState(UNIT_STATE_FOLLOW|UNIT_STATE_FOLLOW_MOVE);
    _updateSpeed(owner);
}

template<class T>
void FollowMovementGenerator<T>::Reset(T &owner)
{
    Initialize(owner);
}

template<class T>
void FollowMovementGenerator<T>::MovementInform(T & /*unit*/)
{
}

template<>
void FollowMovementGenerator<Creature>::MovementInform(Creature &unit)
{
    // Pass back the GUIDLow of the target. If it is pet's owner then PetAI will handle
    if (unit.AI())
        unit.AI()->MovementInform(FOLLOW_MOTION_TYPE, i_target.getTarget()->GetGUIDLow());
}

//-----------------------------------------------//
template void TargetedMovementGeneratorMedium<Player,ChaseMovementGenerator<Player> >::_setTargetLocation(Player &, bool);
template void TargetedMovementGeneratorMedium<Player,FollowMovementGenerator<Player> >::_setTargetLocation(Player &, bool);
template void TargetedMovementGeneratorMedium<Creature,ChaseMovementGenerator<Creature> >::_setTargetLocation(Creature &, bool);
template void TargetedMovementGeneratorMedium<Creature,FollowMovementGenerator<Creature> >::_setTargetLocation(Creature &, bool);
template bool TargetedMovementGeneratorMedium<Player,ChaseMovementGenerator<Player> >::Update(Player &, const uint32 &);
template bool TargetedMovementGeneratorMedium<Player,FollowMovementGenerator<Player> >::Update(Player &, const uint32 &);
template bool TargetedMovementGeneratorMedium<Creature,ChaseMovementGenerator<Creature> >::Update(Creature &, const uint32 &);
template bool TargetedMovementGeneratorMedium<Creature,FollowMovementGenerator<Creature> >::Update(Creature &, const uint32 &);

template void ChaseMovementGenerator<Player>::_reachTarget(Player &);
template void ChaseMovementGenerator<Creature>::_reachTarget(Creature &);
template void ChaseMovementGenerator<Player>::Finalize(Player &);
template void ChaseMovementGenerator<Creature>::Finalize(Creature &);
template void ChaseMovementGenerator<Player>::Reset(Player &);
template void ChaseMovementGenerator<Creature>::Reset(Creature &);
template void ChaseMovementGenerator<Player>::MovementInform(Player &unit);

template void FollowMovementGenerator<Player>::Finalize(Player &);
template void FollowMovementGenerator<Creature>::Finalize(Creature &);
template void FollowMovementGenerator<Player>::Reset(Player &);
template void FollowMovementGenerator<Creature>::Reset(Creature &);
template void FollowMovementGenerator<Player>::MovementInform(Player &unit);
