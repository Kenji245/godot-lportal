#pragma once

#include "core/ustring.h"

class LArea
{
public:
	void Create(String szName);

	String m_szName;

//	int m_iFirstRoom;
//	int m_iNumRooms;

	// each area contains a list of global lights that affect this area
	int m_iFirstLight;
	int m_iNumLights;
};
