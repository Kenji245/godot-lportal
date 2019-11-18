#include "larea.h"

void LArea::Create(String szName)
{
	m_iFirstRoom = 0;
	m_iNumRooms = 0;

	m_iFirstLight = 0;
	m_iNumLights = 0;

	m_szName = szName;
}
