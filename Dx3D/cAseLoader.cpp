#include "stdafx.h"
#include "cAseLoader.h"
#include "Asciitok.h"
#include "cMtlTex.h"
#include <regex>

cAseLoader::cAseLoader()
{
}

cAseLoader::~cAseLoader()
{
}

void cAseLoader::Load(std::vector<LPD3DXMESH>& meshs, std::string folder, std::string file)
{
	FILE* pFile;	
	std::vector<cMtlTex*>	vecMtlTex;
	
	errno_t open_error;	
	open_error = fopen_s(&pFile, (folder + file).c_str(), "r");
	{
		assert(open_error == 0);
		
		char read_line[1024] = { 0, };
		char token[1024] = { 0, };
		while (!feof(pFile))
		{
			fgets(read_line, 1024, pFile);
			sscanf(read_line, "%s", token);			
			
						
#pragma region ReadMaterialList
			if (strcmp(token, ID_MATERIAL_LIST) == 0)
			{
				int nNumMlt = 0;
				fgets(read_line, 1024, pFile);
				sscanf(read_line, "%*s %d", &nNumMlt);
				vecMtlTex.resize(nNumMlt);

				for (int i = 0; i < nNumMlt; i++)
				{
					fgets(read_line, 1024, pFile);
					sscanf(read_line, "%s", token);

					cMtlTex* newMtlTex = new cMtlTex;

					if (strcmp(token, ID_MATERIAL) == 0)
					{
						while (strcmp(token, "}") != 0)
						{
							fgets(read_line, 1024, pFile);
							sscanf(read_line, "%s", token);
							if (strcmp(token, ID_AMBIENT) == 0)
							{
								float r, g, b;
								sscanf(read_line, "%*s %f %f %f", &r, &g, &b);
								newMtlTex->stMtl.Ambient.r = r;
								newMtlTex->stMtl.Ambient.g = g;
								newMtlTex->stMtl.Ambient.b = b;
								newMtlTex->stMtl.Ambient.r = 1.0f;
							}
							else if (strcmp(token, ID_DIFFUSE) == 0)
							{
								float r, g, b;
								sscanf(read_line, "%*s %f %f %f", &r, &g, &b);
								newMtlTex->stMtl.Diffuse.r = r;
								newMtlTex->stMtl.Diffuse.g = g;
								newMtlTex->stMtl.Diffuse.b = b;
								newMtlTex->stMtl.Diffuse.r = 1.0f;
							}
							else if (strcmp(token, ID_SPECULAR) == 0)
							{
								float r, g, b;
								sscanf(read_line, "%*s %f %f %f", &r, &g, &b);
								newMtlTex->stMtl.Specular.r = r;
								newMtlTex->stMtl.Specular.g = g;
								newMtlTex->stMtl.Specular.b = b;
								newMtlTex->stMtl.Specular.r = 1.0f;
							}
							else if (strcmp(token, ID_MAP_DIFFUSE) == 0)
							{
								while (strcmp(token, "}") != 0)
								{
									fgets(read_line, 1024, pFile);
									sscanf(read_line, "%s", token);
									if (strcmp(token, ID_BITMAP) == 0)
									{
										std::string texturefile;
										std::regex r(R"(^.*\/(.+\..+)\"\n$)");
										std::smatch m;
										std::string strL = std::string(read_line);
										if (std::regex_match(strL, m, r))
										{
											texturefile = m[1].str();
										}
										newMtlTex->pTex = g_pTextureManager->GetTexture((folder + texturefile));
									}
									else
									{
										continue;
									}
								}
							}
							else
							{
								continue;
							}

						}
					}
					vecMtlTex[i] = newMtlTex;
				}

			}
#pragma endregion
#pragma region ReadGeometry
			else if (strcmp(token, ID_GEOMETRY) == 0)
			{	
				int nNumVertex = 0;
				int nNumFaces = 0;
				int nNumTVertex = 0;
				int nNumTVFaces = 0;

				std::vector<D3DXVECTOR3>		vecV;
				std::vector<D3DXVECTOR2>		vecVT;
				std::vector<D3DXVECTOR3>		vecVN;
				std::vector<std::vector<int>>	vecVF;
				std::vector<std::vector<int>>	vecTVF;
				std::vector<ST_PNT_VERTEX>		vecVertex;

				while (strcmp(token, "}") != 0)
				{
					fgets(read_line, 1024, pFile);
					sscanf(read_line, "%s", token);
					if (strcmp(token, ID_MESH) == 0)
					{
						while (strcmp(token, "}") != 0)
						{
							fgets(read_line, 1024, pFile);
							sscanf(read_line, "%s", token);
							if (strcmp(token, ID_MESH_NUMVERTEX) == 0)
							{
								sscanf(read_line, "%*s %d", &nNumVertex);
							}
							else if (strcmp(token, ID_MESH_NUMFACES) == 0)
							{
								sscanf(read_line, "%*s %d", &nNumFaces);						
							}
							else if (strcmp(token, ID_MESH_VERTEX_LIST) == 0)
							{
								float x = 0.0f, y = 0.0f, z = 0.0f;
								for (int i = 0; i < nNumVertex; i++)
								{
									fgets(read_line, 1024, pFile);
									sscanf(read_line, "%*s %*d %f %f %f", &x, &z, &y);
									vecV.push_back(D3DXVECTOR3(x, y, z));
								}
							}
							else if (strcmp(token, ID_MESH_FACE_LIST) == 0)
							{
								std::vector<int>	vecVertixIndex(3);
								int a = 0, b = 0, c = 0;
								for (int i = 0; i < nNumFaces; i++)
								{
									fgets(read_line, 1024, pFile);
									sscanf(read_line, "%*s %*s %*s %d %*s %d %*s %d", &a, &c, &b);
									vecVertixIndex[0] = a;
									vecVertixIndex[1] = b;
									vecVertixIndex[2] = c;
									vecVF.push_back(vecVertixIndex);
									vecVertixIndex.clear();									
								}
							}
							else if (strcmp(token, ID_MESH_NUMTVERTEX) == 0)
							{
								sscanf(read_line, "%*s %d", &nNumTVertex);
							}
							else if (strcmp(token, ID_MESH_VERTEX_LIST) == 0)
							{
								float u = 0.0f, v = 0.0f;
								for (int i = 0; i < nNumTVertex; i++)
								{
									fgets(read_line, 1024, pFile);
									sscanf(read_line, "%*s %*d %f %f", &u, &v);
									vecVT.push_back(D3DXVECTOR2(u, 1.0f - v));
								}
							}
							else if (strcmp(token, ID_MESH_NUMTVFACES) == 0)
							{
								sscanf(read_line, "%*s %d", &nNumTVFaces);
							}
							else if (strcmp(token, ID_MESH_TFACELIST) == 0)
							{
								std::vector<int>	vecTextureIndex(3);
								int a = 0, b = 0, c = 0;
								for (int i = 0; i < nNumTVFaces; i++)
								{
									fgets(read_line, 1024, pFile);
									sscanf(read_line, "%*s %*s %*s %d %*s %d %*s %d", &a, &c, &b);
									vecTextureIndex[0] = a;
									vecTextureIndex[1] = b;
									vecTextureIndex[2] = c;
									vecTVF.push_back(vecTextureIndex);
									vecTextureIndex.clear();
								}
							}
							else if (strcmp(token, ID_MESH_NORMALS) == 0)
							{
								ST_PNT_VERTEX v;
								float x = 0.0f, y = 0.0f, z = 0.0f;
								for (int i = 0; i < nNumFaces; i++)
								{
									fgets(read_line, 1024, pFile);
									sscanf(read_line, "%s", &token);
									if (strcmp(token, ID_MESH_FACENORMAL) == 0)
									{
										for (int j = 0; j < 3; j++)
										{
											fgets(read_line, 1024, pFile);
											sscanf(read_line, "%*s %*d %f %f %f", &x, &z, &y);
											v.p = vecV[vecVF[i][j]];
											v.t = vecVT[vecTVF[i][j]];
											v.n = D3DXVECTOR3(x, y, z);
										}
										vecVertex.push_back(v);
									}
								}
							}
							else
							{
								continue;
							}
						}

					}
					else if (strcmp(token, ID_MATERIAL_REF) == 0)
					{

					}
					else
					{
						continue;
					}
				}

			}
#pragma endregion

			else
			{
				continue;
			}
		}
	}
	fclose(pFile);
}
