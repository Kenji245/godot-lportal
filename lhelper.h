#pragma once

#include "lroom_manager.h"

class LHelper
{
public:
	struct LFace
	{
		Vector3 m_Pos[3];
		Vector3 m_Norm[3];
		int m_index[3];

		String ToString() const;
	};

	// unique vert
	struct LVert
	{
		Vector3 m_Pos;
		Vector3 m_Norm;
		Vector2 m_UV;
		Vector2 m_UV2;

		bool ApproxEqual(const LVert &o) const;
	};

	// one function to do the whole internal workflow
	MeshInstance * CreateLightmapProxy(LRoomManager &manager);

	// for lightmapping
	bool MergeSOBs(LRoomManager &manager, MeshInstance * pMerged);

	// take the UV2 coords from the merged mesh and attach these to the SOB meshes
	bool UnMergeSOBs(LRoomManager &manager, MeshInstance * pMerged);

//	bool UnMergeSOBs(LRoomManager &manager, const PoolVector<Vector2> &uv2s);

	// main function for getting merged uv2 back to sobs
	bool TransferUV2(const MeshInstance &mi_from, MeshInstance &mi_to);

private:
	void Merge_MI(const MeshInstance &mi, PoolVector<Vector3> &verts, PoolVector<Vector3> &norms, PoolVector<int> &inds);

	bool UnMerge_SOB(MeshInstance &mi, const PoolVector<Vector3> merged_verts, const PoolVector<Vector3> merged_norms, const PoolVector<Vector2> &merged_uv2s, const PoolVector<int> &merged_inds, int &vert_count);

	unsigned int FindMatchingVertex(const PoolVector<Vector2> &uvs, const Vector2 &uv1) const;

	bool BakeLightmap(BakedLightmap &baked_lightmap, MeshInstance * pMerged);

	bool LightmapUnwrap(Ref<ArrayMesh> am, const Transform &trans);
//	bool LightmapUnwrap(const PoolVector<Vector3> &p_verts, const PoolVector<Vector3> &p_normals, const PoolVector<int> &p_inds, PoolVector<Vector2> &r_uvs);
	//Error lightmap_unwrap(ArrayMesh &am, const Transform &p_base_transform, float p_texel_size);
	int DebugCountUVs(MeshInstance &mi);

	void Transform_Verts(const PoolVector<Vector3> &ptsLocal, PoolVector<Vector3> &ptsWorld, const Transform &tr) const;
	void Transform_Norms(const PoolVector<Vector3> &normsLocal, PoolVector<Vector3> &normsWorld, const Transform &tr) const;

	int DoFacesMatch(const LFace& sob_f, const LFace &m_face) const;
	bool DoFaceVertsApproxMatch(const LFace& sob_f, const LFace &m_face, int c0, int c1) const;
	bool DoPosNormsApproxMatch(const Vector3 &a_pos, const Vector3 &a_norm, const Vector3 &b_pos, const Vector3 &b_norm) const;

	int FindOrAddVert(LVector<LVert> &uni_verts, const LVert &vert) const;

	void SetOwnerRecursive(Node * pNode, Node * pOwner);

//	bool xatlas_mesh_lightmap_unwrap(float p_texel_size, const float *p_vertices, const float *p_normals, int p_vertex_count, const int *p_indices, const int *p_face_materials, int p_index_count, float **r_uv, int **r_vertex, int *r_vertex_count, int **r_index, int *r_index_count, int *r_size_hint_x, int *r_size_hint_y);
};
