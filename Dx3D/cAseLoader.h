#pragma once
class cAseLoader
{
public:
	cAseLoader();
	~cAseLoader();

	void Load(std::vector<LPD3DXMESH>& meshs, std::string folder, std::string file);
};

