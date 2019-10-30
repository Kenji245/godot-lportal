#pragma once

#include "lvector.h"

class LSource;
class LRoomManager;
class LRoom;
namespace Lawn {class LBitField_Dynamic;}

class LTrace
{
public:
	enum eTraceFlags
	{
		CULL_SOBS = 1 << 0,
		CULL_DOBS = 1 << 1,
		TOUCH_ROOMS = 1 << 2,
	};


	void Trace_Prepare(LRoomManager &manager, const LSource &cam, Lawn::LBitField_Dynamic &BF_SOBs, Lawn::LBitField_Dynamic &BF_Rooms, LVector<int> &visible_SOBs, LVector<int> &visible_Rooms);
//	void Trace_Prepare(LRoomManager &manager, const LCamera &cam, Lawn::LBitField_Dynamic &BF_SOBs, Lawn::LBitField_Dynamic &BF_DOBs, Lawn::LBitField_Dynamic &BF_Rooms, LVector<int> &visible_SOBs, LVector<int> &visible_DOBs, LVector<int> &visible_Rooms);

	void Trace_SetFlags(unsigned int flags) {m_TraceFlags = flags;}

	void Trace_Begin(LRoom &room, LVector<Plane> &planes);

private:
	void Trace_Recursive(int depth, LRoom &room, const LVector<Plane> &planes, int first_portal_plane);

	void CullSOBs(LRoom &room, const LVector<Plane> &planes);
	void CullDOBs(LRoom &room, const LVector<Plane> &planes);
	void FirstTouch(LRoom &room);
	void DetectFirstTouch(LRoom &room);


	LRoomManager * m_pManager;
	const LSource * m_pCamera;

	Lawn::LBitField_Dynamic * m_pBF_SOBs;
	//Lawn::LBitField_Dynamic * m_pBF_DOBs;
	Lawn::LBitField_Dynamic * m_pBF_Rooms;

	LVector<int> * m_pVisible_SOBs;
	//LVector<int> * m_pVisible_DOBs;
	LVector<int> * m_pVisible_Rooms;

	unsigned int m_TraceFlags;
};
