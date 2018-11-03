#pragma once
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <list>
#include <algorithm>
#include "MeshLoader.h"
#include "DataType.h"
#include "fstream"

class AneuMeshLoader: public MeshLoader {
	std::vector<Node> Nodes;
	std::vector<Element> Elements;
	std::vector<Surface> Surfaces;
	std::string filename;
public:
	AneuMeshLoader(const std::string &p_filename);
	void LoadMesh();
	void Output();
	std::vector<Node> GetNodes() const;
	std::vector<Element> GetElemets() const;
	std::vector<Surface> GetSurfaces() const;
	std::list<Element> FindFE(int KnotID1, int KnotID2, int KnotID3);
	std::list<Element> FindFE(int KnotID1, int KnotID2);
	std::set<int> GetSurfacesID(int IDSurfaceBC);
	std::vector<Element> GetElementsByMaterialType(int MaterialType);
	std::vector<Surface> GetSurfacesBySurfaceID(int SurfaceID);
	std::vector<std::set<int>> GetNodeNeighbors();
	void TransformationToQuadraticFE();
	std::vector<int> GetNewCombination(const std::vector<int> &IDs, int iteration);
	std::vector<float> GetNewCoordinates(const std::vector<int> &Combination, const std::vector<int> &KnotsID);
};