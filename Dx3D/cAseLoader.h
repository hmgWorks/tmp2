#pragma once

class cMeshGroup;

class cAseLoader
{
public:
	cAseLoader();
	~cAseLoader();

	void Load(std::vector<cMeshGroup*>& meshs, std::string folder, std::string file);
};

