//	Copyright (c) 2019 Lawnjelly

//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:

//	The above copyright notice and this permission notice shall be included in all
//	copies or substantial portions of the Software.

//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//	SOFTWARE.

/* room_manager.h */
#ifndef LROOM_MANAGER_H
#define LROOM_MANAGER_H

/**
	@author lawnjelly <lawnjelly@gmail.com>
*/

#include "scene/3d/spatial.h"
#include "CoBitField_Dynamic.h"
#include "lplanes_pool.h"

class LRoomManager : public Spatial {
	GDCLASS(LRoomManager, Spatial);

	friend class LRoom;

	// a quick list of object IDs of child rooms
	Vector<ObjectID> m_room_IDs;

	ObjectID m_room_curr;
	ObjectID m_cameraID;

	// keep track of which rooms are visible, so we can hide ones that aren't hit that were previously on
	Core::CoBitField_Dynamic m_BF_visible_rooms;
	Vector<int> m_VisibleRoomList[2];
	int m_CurrentVisibleRoomList;

public:
	LRoomManager();

	// convert empties and meshes to rooms and portals
	void convert();

	// choose which camera you want to use to determine visibility.
	// normally this will be your main camera, but you can choose another for debugging
	void set_camera(Node * pCam);

protected:
	static void _bind_methods();
	void _notification(int p_what);

	LPlanesPool m_Pool;
private:
	void Convert_Rooms();
	bool Convert_Room(Spatial * pNode);
	void Convert_Portals();
	void Find_Rooms();

	void FrameUpdate();
};

#endif
