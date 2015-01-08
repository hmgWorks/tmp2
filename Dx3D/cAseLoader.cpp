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
				while (strcmp(token, "}") != 0)
				{
					fgets(read_line, 1024, pFile);
					sscanf(read_line, "%s", token);
					if (strcmp(token, ID_MESH) == 0)
					{

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
