#pragma once

#include "lvector.h"

class LCamera;
class LRoomManager;
class LRoom;
namespace Lawn {class LBitField_Dynamic;}

class LTrace
{
public:
	void Trace_Prepare(LRoomManager &manager, const LCamera &cam, Lawn::LBitField_Dynamic &BF_SOBs, Lawn::LBitField_Dynamic &BF_Rooms, LVector<int> &visible_SOBs, LVector<int> &visible_Rooms);
//	void Trace_Prepare(LRoomManager &manager, const LCamera &cam, Lawn::LBitField_Dynamic &BF_SOBs, Lawn::LBitField_Dynamic &BF_DOBs, Lawn::LBitField_Dynamic &BF_Rooms, LVector<int> &visible_SOBs, LVector<int> &visible_DOBs, LVector<int> &visible_Rooms);

	void Trace_Recursive(int depth, LRoom &room, const LVector<Plane> &planes, int first_portal_plane = 1);

private:
	void CullSOBs(LRoom &room, const LVector<Plane> &planes);
	void CullDOBs(LRoom &room, const LVector<Plane> &planes);
	void FirstTouch(LRoom &room);


	LRoomManager * m_pManager;
	const LCamera * m_pCamera;

	Lawn::LBitField_Dynamic * m_pBF_SOBs;
	//Lawn::LBitField_Dynamic * m_pBF_DOBs;
	Lawn::LBitField_Dynamic * m_pBF_Rooms;

	LVector<int> * m_pVisible_SOBs;
	//LVector<int> * m_pVisible_DOBs;
	LVector<int> * m_pVisible_Rooms;
};
