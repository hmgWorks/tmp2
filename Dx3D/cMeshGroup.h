#pragma once

class cMtlTex;

class cMeshGroup
{
private:
	LPD3DXMESH		m_pMesh;
	cMtlTex*		m_pMtlTex;
public:
	cMeshGroup();
	~cMeshGroup();

	void Setup(int size, std::vector<ST_PNT_VERTEX>& vecVertex, cMtlTex* mtltex);	
	void Setup(LPD3DXMESH mesh, cMtlTex* mtltex);
	void Render();
};

