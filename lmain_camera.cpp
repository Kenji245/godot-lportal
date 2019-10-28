#include "lmain_camera.h"

#include "scene/3d/camera.h"
#include "core/math/camera_matrix.h"

void LMainCamera::GetNeighbours(ePlane p, ePlane neigh_planes[4]) const
{
	// table of neighbouring planes to each
	static const ePlane NeighTable[P_TOTAL][4] = {
	{	// LSM_FP_NEAR
	P_LEFT,
	P_RIGHT,
	P_TOP,
	P_BOTTOM
	},
	{	// LSM_FP_FAR
	P_LEFT,
	P_RIGHT,
	P_TOP,
	P_BOTTOM
	},
	{	// LSM_FP_LEFT
	P_TOP,
	P_BOTTOM,
	P_NEAR,
	P_FAR
	},
	{	// LSM_FP_TOP
	P_LEFT,
	P_RIGHT,
	P_NEAR,
	P_FAR
	},
	{	// LSM_FP_RIGHT
	P_TOP,
	P_BOTTOM,
	P_NEAR,
	P_FAR
	},
	{	// LSM_FP_BOTTOM
	P_LEFT,
	P_RIGHT,
	P_NEAR,
	P_FAR
	},
	};

	for (int n=0; n<4; n++)
	{
		neigh_planes[n] = NeighTable[p][n];
	}
}


/**
* Given two planes, returns the two points shared by those planes.  The points are always
*	returned in counter-clockwise order, assuming the first input plane is facing towards
*	the viewer.
*
* \param _fpPlane0 The plane facing towards the viewer.
* \param _fpPlane1 A plane neighboring _fpPlane0.
* \param _fpRet An array of exactly two elements to be filled with the indices of the points
*	on return.
*/
void LMainCamera::GetCornersOfPlanes( ePlane _fpPlane0, ePlane _fpPlane1,
ePoint _fpRet[2] ) const
{
	static const ePoint fpTable[P_TOTAL][P_TOTAL][2] = {
	{	// LSM_FP_NEAR
	{	// LSM_FP_NEAR
	PT_NEAR_LEFT_TOP, PT_NEAR_RIGHT_TOP,		// Invalid combination.
	},
	{	// LSM_FP_FAR
	PT_FAR_RIGHT_TOP, PT_FAR_LEFT_TOP,		// Invalid combination.
	},
	{	// LSM_FP_LEFT
	PT_NEAR_LEFT_TOP, PT_NEAR_LEFT_BOTTOM,
	},
	{	// LSM_FP_TOP
	PT_NEAR_RIGHT_TOP, PT_NEAR_LEFT_TOP,
	},
	{	// LSM_FP_RIGHT
	PT_NEAR_RIGHT_BOTTOM, PT_NEAR_RIGHT_TOP,
	},
	{	// LSM_FP_BOTTOM
	PT_NEAR_LEFT_BOTTOM, PT_NEAR_RIGHT_BOTTOM,
	},
	},

	//	P_NEAR,
	//	P_FAR,
	//	P_LEFT,
	//	P_TOP,
	//	P_RIGHT,
	//	P_BOTTOM,

	{	// LSM_FP_LEFT
	{	// LSM_FP_LEFT
	PT_FAR_LEFT_BOTTOM, PT_FAR_LEFT_BOTTOM,		// Invalid combination.
	},
	{	// LSM_FP_RIGHT
	PT_FAR_LEFT_BOTTOM, PT_FAR_LEFT_BOTTOM,		// Invalid combination.
	},
	{	// LSM_FP_TOP
	PT_NEAR_LEFT_TOP, PT_FAR_LEFT_TOP,
	},
	{	// LSM_FP_BOTTOM
	PT_FAR_LEFT_BOTTOM, PT_NEAR_LEFT_BOTTOM,
	},
	{	// LSM_FP_NEAR
	PT_NEAR_LEFT_BOTTOM, PT_NEAR_LEFT_TOP,
	},
	{	// LSM_FP_FAR
	PT_FAR_LEFT_TOP, PT_FAR_LEFT_BOTTOM,
	},
	},
	{	// LSM_FP_RIGHT
	{	// LSM_FP_LEFT
	PT_FAR_RIGHT_BOTTOM, PT_FAR_RIGHT_BOTTOM,	// Invalid combination.
	},
	{	// LSM_FP_RIGHT
	PT_FAR_RIGHT_BOTTOM, PT_FAR_RIGHT_BOTTOM,	// Invalid combination.
	},
	{	// LSM_FP_TOP
	PT_FAR_RIGHT_TOP, PT_NEAR_RIGHT_TOP,
	},
	{	// LSM_FP_BOTTOM
	PT_NEAR_RIGHT_BOTTOM, PT_FAR_RIGHT_BOTTOM,
	},
	{	// LSM_FP_NEAR
	PT_NEAR_RIGHT_TOP, PT_NEAR_RIGHT_BOTTOM,
	},
	{	// LSM_FP_FAR
	PT_FAR_RIGHT_BOTTOM, PT_FAR_RIGHT_TOP,
	},
	},

	// ==

	{	// LSM_FP_TOP
	{	// LSM_FP_LEFT
	PT_FAR_LEFT_TOP, PT_NEAR_LEFT_TOP,
	},
	{	// LSM_FP_RIGHT
	PT_NEAR_RIGHT_TOP, PT_FAR_RIGHT_TOP,
	},
	{	// LSM_FP_TOP
	PT_NEAR_LEFT_TOP, PT_FAR_LEFT_TOP,		// Invalid combination.
	},
	{	// LSM_FP_BOTTOM
	PT_FAR_LEFT_BOTTOM, PT_NEAR_LEFT_BOTTOM,	// Invalid combination.
	},
	{	// LSM_FP_NEAR
	PT_NEAR_LEFT_TOP, PT_NEAR_RIGHT_TOP,
	},
	{	// LSM_FP_FAR
	PT_FAR_RIGHT_TOP, PT_FAR_LEFT_TOP,
	},
	},
	{	// LSM_FP_BOTTOM
	{	// LSM_FP_LEFT
	PT_NEAR_LEFT_BOTTOM, PT_FAR_LEFT_BOTTOM,
	},
	{	// LSM_FP_RIGHT
	PT_FAR_RIGHT_BOTTOM, PT_NEAR_RIGHT_BOTTOM,
	},
	{	// LSM_FP_TOP
	PT_NEAR_LEFT_BOTTOM, PT_FAR_LEFT_BOTTOM,	// Invalid combination.
	},
	{	// LSM_FP_BOTTOM
	PT_FAR_LEFT_BOTTOM, PT_NEAR_LEFT_BOTTOM,	// Invalid combination.
	},
	{	// LSM_FP_NEAR
	PT_NEAR_RIGHT_BOTTOM, PT_NEAR_LEFT_BOTTOM,
	},
	{	// LSM_FP_FAR
	PT_FAR_LEFT_BOTTOM, PT_FAR_RIGHT_BOTTOM,
	},
	},

	// ==

	{	// LSM_FP_FAR
	{	// LSM_FP_LEFT
	PT_FAR_LEFT_BOTTOM, PT_FAR_LEFT_TOP,
	},
	{	// LSM_FP_RIGHT
	PT_FAR_RIGHT_TOP, PT_FAR_RIGHT_BOTTOM,
	},
	{	// LSM_FP_TOP
	PT_FAR_LEFT_TOP, PT_FAR_RIGHT_TOP,
	},
	{	// LSM_FP_BOTTOM
	PT_FAR_RIGHT_BOTTOM, PT_FAR_LEFT_BOTTOM,
	},
	{	// LSM_FP_NEAR
	PT_FAR_LEFT_TOP, PT_FAR_RIGHT_TOP,		// Invalid combination.
	},
	{	// LSM_FP_FAR
	PT_FAR_RIGHT_TOP, PT_FAR_LEFT_TOP,		// Invalid combination.
	},
	},
	};
	_fpRet[0] = fpTable[_fpPlane0][_fpPlane1][0];
	_fpRet[1] = fpTable[_fpPlane0][_fpPlane1][1];
}

LMainCamera::LMainCamera()
{
	// create the LUT


	// each pair of planes that are opposite can have an edge
	for (int p0 = 0; p0<P_TOTAL; p0++)
	{
		// for each neighbour of the plane
		ePlane neighs[4];
		GetNeighbours((ePlane) p0, neighs);

		for (int n=0; n<4; n++)
		{
			int p1 = neighs[n];

			//if these are opposite we need to add the 2 points they share


		}

	}
}


bool LMainCamera::AddCameraLightPlanes(const LCamera &lcam, LVector<Plane> &planes) const
{
	// find which of the camera planes are facing away from the light
	assert (m_Planes.size() == 8);

	uint32_t lookup = 0;

	for (int n=0; n<8; n++)
	{
		float dot = m_Planes[n].normal.dot(lcam.m_ptDir);
		if (dot > 0.0f)
		{
			lookup |= 1 >> n;
		}
	}


	return true;
}


bool LMainCamera::Prepare(Camera * pCam)
{
	m_Planes.copy_from(pCam->get_frustum());

	if (m_Points.size() != 8)
		m_Points.resize(8);

	//	Vector<Plane> planes = get_projection_planes(Transform());
	const CameraMatrix::Planes intersections[8][3] = {
	{ CameraMatrix::PLANE_FAR, CameraMatrix::PLANE_LEFT, CameraMatrix::PLANE_TOP },
	{ CameraMatrix::PLANE_FAR, CameraMatrix::PLANE_LEFT, CameraMatrix::PLANE_BOTTOM },
	{ CameraMatrix::PLANE_FAR, CameraMatrix::PLANE_RIGHT, CameraMatrix::PLANE_TOP },
	{ CameraMatrix::PLANE_FAR, CameraMatrix::PLANE_RIGHT, CameraMatrix::PLANE_BOTTOM },
	{ CameraMatrix::PLANE_NEAR, CameraMatrix::PLANE_LEFT, CameraMatrix::PLANE_TOP },
	{ CameraMatrix::PLANE_NEAR, CameraMatrix::PLANE_LEFT, CameraMatrix::PLANE_BOTTOM },
	{ CameraMatrix::PLANE_NEAR, CameraMatrix::PLANE_RIGHT, CameraMatrix::PLANE_TOP },
	{ CameraMatrix::PLANE_NEAR, CameraMatrix::PLANE_RIGHT, CameraMatrix::PLANE_BOTTOM },
	};

	for (int i = 0; i < 8; i++) {

		Vector3 point;
		bool res = m_Planes[intersections[i][0]].intersect_3(m_Planes[intersections[i][1]], m_Planes[intersections[i][2]], &point);
		ERR_FAIL_COND_V(!res, false);
		//m_Points[i] = p_transform.xform(point);
		m_Points[i] = point;
	}

	return true;
}

