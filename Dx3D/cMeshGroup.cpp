#include "stdafx.h"
#include "cMeshGroup.h"
#include "cMtlTex.h"

cMeshGroup::cMeshGroup()
	:m_pMesh(NULL)
	, m_pMtlTex(NULL)
{
}

cMeshGroup::~cMeshGroup()
{
	SAFE_RELEASE(m_pMtlTex);
	SAFE_RELEASE(m_pMesh);
}

void cMeshGroup::Setup(int size, std::vector<ST_PNT_VERTEX>& vecVertex, cMtlTex* mtltex)
{
	mtltex->AddRef();
	m_pMtlTex = mtltex;

	HRESULT hr = D3DXCreateMeshFVF(size / 3, size, D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF, g_pD3DDevice, &m_pMesh);
	assert(hr == D3D_OK && "mesh error");

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	{
		memcpy(pV, &vecVertex[0], sizeof(ST_PNT_VERTEX)*size);
	}
	m_pMesh->UnlockVertexBuffer();

	WORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	{
		for (int i = 0; i < size; i++)
		{
			pI[i] = i;
		}
	}
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	{
		for (int i = 0; i < size / 3; i++)
		{
			pA[i] = 0;
		}
	}
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdjBuffer(size);
	m_pMesh->GenerateAdjacency(0.0f, &vecAdjBuffer[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdjBuffer[0], 0, 0, 0);
}
void cMeshGroup::Setup(LPD3DXMESH mesh, cMtlTex* mtltex)
{
	mtltex->AddRef();
	m_pMtlTex = mtltex;

	mesh->AddRef();
	m_pMesh = mesh;

	
}

void cMeshGroup::Render()
{
	//g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetTexture(0, m_pMtlTex->pTex);
	g_pD3DDevice->SetMaterial(&m_pMtlTex->stMtl);
	m_pMesh->DrawSubset(0);
}
