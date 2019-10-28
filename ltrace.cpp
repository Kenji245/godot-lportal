#include "ltrace.h"
#include "ldebug.h"
#include "ldob.h"
#include "lportal.h"
#include "lbitfield_dynamic.h"
#include "lroom_manager.h"

#define LMAN m_pManager



//void LTrace::Trace_Prepare(LRoomManager &manager, const LCamera &cam, Lawn::LBitField_Dynamic &BF_SOBs, Lawn::LBitField_Dynamic &BF_DOBs, Lawn::LBitField_Dynamic &BF_Rooms, LVector<int> &visible_SOBs, LVector<int> &visible_DOBs, LVector<int> &visible_Rooms)
void LTrace::Trace_Prepare(LRoomManager &manager, const LCamera &cam, Lawn::LBitField_Dynamic &BF_SOBs, Lawn::LBitField_Dynamic &BF_Rooms, LVector<int> &visible_SOBs, LVector<int> &visible_Rooms)
{
	m_pManager = &manager;
	m_pCamera = &cam;

	m_pBF_SOBs = &BF_SOBs;
//	m_pBF_DOBs = &BF_DOBs;
	m_pBF_Rooms = &BF_Rooms;
	m_pVisible_SOBs = &visible_SOBs;
//	m_pVisible_DOBs = &visible_DOBs;
	m_pVisible_Rooms = &visible_Rooms;
}

void LTrace::CullSOBs(LRoom &room, const LVector<Plane> &planes)
{
	// clip all objects in this room to the clipping planes
	int last_sob = room.m_iFirstSOB + room.m_iNumSOBs;
	for (int n=room.m_iFirstSOB; n<last_sob; n++)
	{
		LSob &sob = LMAN->m_SOBs[n];

		//LPRINT_RUN(2, "sob " + itos(n) + " " + sob.GetSpatial()->get_name());

		// already determined to be visible through another portal
		if (m_pBF_SOBs->GetBit(n))
		{
			//LPRINT_RUN(2, "\talready visible");
			continue;
		}

		bool bShow = true;


		// estimate the radius .. for now
		const AABB &bb = sob.m_aabb;

//		print("\t\t\tculling object " + pObj->get_name());

		for (int p=0; p<planes.size(); p++)
		{
//				float dist = planes[p].distance_to(pt);
//				print("\t\t\t\t" + itos(p) + " : dist " + String(Variant(dist)));

			float r_min, r_max;
			bb.project_range_in_plane(planes[p], r_min, r_max);

	//		print("\t\t\t\t" + itos(p) + " : r_min " + String(Variant(r_min)) + ", r_max " + String(Variant(r_max)));


			if (r_min > 0.0f)
			{
				//LPRINT_RUN(2, "\tout of view");
				bShow = false;
				break;
			}
		}

		if (bShow)
		{
			// sob is renderable and visible (not shadow only)
			//LPRINT_RUN(2, "\tin view");
			m_pBF_SOBs->SetBit(n, true);
			m_pVisible_SOBs->push_back(n);
		}

	} // for through sobs

}

void LTrace::CullDOBs(LRoom &room, const LVector<Plane> &planes)
{
	// NYI this isn't efficient, there may be more than 1 portal to the same room

	// cull DOBs
	int nDOBs = room.m_DOBs.size();

	for (int n=0; n<nDOBs; n++)
	{
		LDob &dob = room.m_DOBs[n];

		Spatial * pObj = dob.GetSpatial();

		if (pObj)
		{
			bool bShow = true;
			const Vector3 &pt = pObj->get_global_transform().origin;

			float radius = dob.m_fRadius;

			for (int p=0; p<planes.size(); p++)
			{
				float dist = planes[p].distance_to(pt);
				//print("\t\t\t\t" + itos(p) + " : dist " + String(Variant(dist)));

				if (dist > radius)
				{
					bShow = false;
					break;
				}
			}

			if (bShow)
			{
				LPRINT_RUN(1, "\tDOB " + pObj->get_name() + " visible");
				dob.m_bVisible = true;
			}
			else
			{
				LPRINT_RUN(1, "\tDOB " + pObj->get_name() + " culled");
			}
		}
	} // for through dobs

}


void LTrace::Trace_Recursive(int depth, LRoom &room, const LVector<Plane> &planes, int first_portal_plane)
{
	// prevent too much depth
	if (depth > 8)
	{
		LPRINT_RUN(2, "\t\t\tDEPTH LIMIT REACHED");
		WARN_PRINT_ONCE("LPortal Depth Limit reached (seeing through > 8 portals)");
		return;
	}

	// for debugging
	Lawn::LDebug::m_iTabDepth = depth;
	LPRINT_RUN(2, "");
	LPRINT_RUN(2, "ROOM '" + room.get_name() + "' planes " + itos(planes.size()) + " portals " + itos(room.m_iNumPortals) );

	// only handle one touch per frame so far (one portal into room)
	//assert (manager.m_uiFrameCounter > m_uiFrameTouched);

	// first touch
	if (room.m_uiFrameTouched < LMAN->m_uiFrameCounter)
		FirstTouch(room);


	CullSOBs(room, planes);
	CullDOBs(room, planes);

	// portals





	// look through portals
	int nPortals = room.m_iNumPortals;

	for (int port_num=0; port_num<nPortals; port_num++)
	{
		int port_id = room.m_iFirstPortal + port_num;

		const LPortal &port = LMAN->m_Portals[port_id];

		// have we already handled the room on this frame?
		// get the room pointed to by the portal
		LRoom * pLinkedRoom = &LMAN->Portal_GetLinkedRoom(port);

		// cull by portal angle to camera.
		// Note we need to deal with 'side on' portals, and the camera has a spreading view, so we cannot simply dot
		// the portal normal with camera direction, we need to take into account angle to the portal itself.
		const Vector3 &portal_normal = port.m_Plane.normal;
		LPRINT_RUN(2, "\tPORTAL " + itos (port_num) + " (" + itos(port_id) + ") " + port.get_name() + " normal " + portal_normal);

		// we will dot the portal angle with a ray from the camera to the portal centre
		// (there might be an even better ray direction but this will do for now)
		Vector3 dir_portal = port.m_ptCentre - m_pCamera->m_ptPos;

		// doesn't actually need to be normalized?
		float dot = dir_portal.dot(portal_normal);

		if (dot <= -0.0f) // 0.0
		{
			//LPRINT_RUN(2, "\t\tCULLED (wrong direction) dot is " + String(Variant(dot)) + ", dir_portal is " + dir_portal);
			LPRINT_RUN(2, "\t\tCULLED (wrong direction)");
			continue;
		}

		// is it culled by the planes?
		LPortal::eClipResult overall_res = LPortal::eClipResult::CLIP_INSIDE;

		// while clipping to the planes we maintain a list of partial planes, so we can add them to the
		// recursive next iteration of planes to check
		static LVector<int> partial_planes;
		partial_planes.clear();

		// for portals, we want to ignore the near clipping plane, as we might be right on the edge of a doorway
		// and still want to look through the portal.
		// So we are starting this loop from 1, ASSUMING that plane zero is the near clipping plane.
		// If it isn't we would need a different strategy
		// Note that now this only occurs for the first portal out of the current room. After that,
		// 0 is passed as first_portal_plane, because the near plane will probably be irrelevant,
		// and we are now not necessarily copying the camera planes.
		for (int l=first_portal_plane; l<planes.size(); l++)
		{
			LPortal::eClipResult res = port.ClipWithPlane(planes[l]);

			switch (res)
			{
			case LPortal::eClipResult::CLIP_OUTSIDE:
				overall_res = res;
				break;
			case LPortal::eClipResult::CLIP_PARTIAL:
				overall_res = res;
				partial_planes.push_back(l);
				break;
			default: // suppress warning
				break;
			}

			if (overall_res == LPortal::eClipResult::CLIP_OUTSIDE)
				break;
		}

		// this portal is culled
		if (overall_res == LPortal::eClipResult::CLIP_OUTSIDE)
		{
			LPRINT_RUN(2, "\t\tCULLED (outside planes)");
			continue;
		}

		// else recurse into that portal
		unsigned int uiPoolMem = LMAN->m_Pool.Request();
		if (uiPoolMem != -1)
		{
			// get a vector of planes from the pool
			LVector<Plane> &new_planes = LMAN->m_Pool.Get(uiPoolMem);
			new_planes.clear();

			// NEW!! if portal is totally inside the planes, don't copy the old planes
			if (overall_res != LPortal::eClipResult::CLIP_INSIDE)
			{
				// copy the existing planes
				//new_planes.copy_from(planes);

				// new .. only copy the partial planes that the portal cuts through
				for (int n=0; n<partial_planes.size(); n++)
					new_planes.push_back(planes[partial_planes[n]]);
			}

			// add the planes for the portal
			// NOTE that we can also optimize by not adding portal planes for edges that
			// were behind a partial plane. NYI
			port.AddPlanes(*LMAN, m_pCamera->m_ptPos, new_planes);


			if (pLinkedRoom)
			{
				Trace_Recursive(depth+1, *pLinkedRoom, new_planes, 0);
				//pLinkedRoom->DetermineVisibility_Recursive(manager, depth + 1, cam, new_planes, 0);
				// for debugging need to reset tab depth
				Lawn::LDebug::m_iTabDepth = depth;
			}

			// we no longer need these planes
			LMAN->m_Pool.Free(uiPoolMem);
		}
		else
		{
			// planes pool is empty!
			// This will happen if the view goes through shedloads of portals
			// The solution is either to increase the plane pool size, or build levels
			// with views through multiple portals. Looking through multiple portals is likely to be
			// slow anyway because of the number of planes to test.
			WARN_PRINT_ONCE("Planes pool is empty");
		}

	} // for p through portals

}

void LTrace::FirstTouch(LRoom &room)
{
	// set the frame counter
	room.m_uiFrameTouched = LMAN->m_uiFrameCounter;

	// show this room and add to visible list of rooms
	room.Room_MakeVisible(true);

	m_pBF_Rooms->SetBit(room.m_RoomID, true);

	// keep track of which rooms are shown this frame
	m_pVisible_Rooms->push_back(room.m_RoomID);

	// hide all dobs
	for (int n=0; n<room.m_DOBs.size(); n++)
		room.m_DOBs[n].m_bVisible = false;
}
