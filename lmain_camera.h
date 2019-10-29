#pragma once

// we get the main camera clipping planes and derive the points from 3 plane equations
// in order to cull shadow casters to the camera frustum
class LMainCamera
{
public:
	// same order as godot
	enum ePlane {
		P_NEAR,
		P_FAR,
		P_LEFT,
		P_TOP,
		P_RIGHT,
		P_BOTTOM,
		P_TOTAL,
	};

	// same order as godot
	enum ePoint {
		PT_FAR_LEFT_TOP,
		PT_FAR_LEFT_BOTTOM,
		PT_FAR_RIGHT_TOP,
		PT_FAR_RIGHT_BOTTOM,
		PT_NEAR_LEFT_TOP,
		PT_NEAR_LEFT_BOTTOM,
		PT_NEAR_RIGHT_TOP,
		PT_NEAR_RIGHT_BOTTOM,
	};

	static const char * m_szPlanes[];
	static const char * m_szPoints[];

	// 6 bits, 6 planes
	enum {LUT_SIZE = 64,};

	// create the LUT
	LMainCamera();

	bool Prepare(LRoomManager &manager, Camera * pCam);

	// main use of this object, we can create a clipping volume that is a mix of the light frustum and the camera volume
	bool AddCameraLightPlanes(LRoomManager &manager, const LCamera &lcam, LVector<Plane> &planes) const;

	LVector<Plane> m_Planes;
	LVector<Vector3> m_Points;

private:
	void GetNeighbours(ePlane p, ePlane neigh_planes[4]) const;
	void GetCornersOfPlanes( ePlane _fpPlane0, ePlane _fpPlane1, ePoint _fpRet[2] ) const;
	void AddLUT(int p0, int p1, ePoint pts[2]);
	void AddLUT_Entry(unsigned int n, ePoint pts[2]);
	void CompactLUT_Entry(int n);
	void DebugPrintLUT();
	String DebugStringLUT_Entry(const LVector<uint8_t> &entry);
	String String_PlaneBF(unsigned int BF);
	String String_LUTEntry(const LVector<uint8_t> &entry);

	// contains a list of points for each combination of plane facing directions
	LVector<uint8_t> m_LUT[LUT_SIZE];
};

