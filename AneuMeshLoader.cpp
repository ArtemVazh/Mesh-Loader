#include "stdafx.h"
#include "AneuMeshLoader.h"
#include "Exception.h"

AneuMeshLoader::AneuMeshLoader(const std::string &p_filename) : filename(p_filename) {};

void AneuMeshLoader::LoadMesh() {
	std::ifstream file(filename);
	if (!file)
		throw ErrorFile(filename);
	int count, dimension;
	file >> count >> dimension;
	Nodes.resize(count);
	for (int i = 0; i < count; ++i) {
		Nodes[i].coordinates.resize(dimension);
		for (int j = 0; j < dimension; ++j) {
			file >> Nodes[i].coordinates[j];
		}
		Nodes[i].ID = i + 1;
	}
	int quantity;
	file >> count >> quantity;
	Elements.resize(count);
	for (int i = 0; i < count; ++i) {
		file >> Elements[i].MaterialType;
		for (int j = 0; j < quantity; ++j) {
			Elements[i].KnotsID.resize(quantity);
			file >> Elements[i].KnotsID[j];
		}
		Elements[i].ID = i + 1;
	}
	file >> count >> quantity;
	Surfaces.resize(count);
	for (int i = 0; i < count; ++i) {
		file >> Surfaces[i].IDSurfaceBC;
		for (int j = 0; j < quantity; ++j) {
			Surfaces[i].KnotsID.resize(quantity);
			file >> Surfaces[i].KnotsID[j];
		}
		Surfaces[i].ID = i + 1;
	}
}

void AneuMeshLoader::Output() {
	if (Nodes.empty() && Elements.empty() && Surfaces.empty())
		LoadMesh();
	int size = Nodes.size();
	int dimension = Nodes[0].coordinates.size();
	std::cout << "Количество узлов: " << size << " Размерность пространства: " << dimension << std::endl;
	for (int i = 0; i < size; ++i) {
		std::cout << "ID узла: " << Nodes[i].ID << "; Координаты узла: ";
		if (dimension == 3) {
			std::cout << "x = " << Nodes[i].coordinates[0] << ", y = " << Nodes[i].coordinates[1] << ", z = " << Nodes[i].coordinates[2];
		}
		else
			for (int j = 0; j < dimension; ++j)
				std::cout << Nodes[i].coordinates[j];
		std::cout << std::endl;
	}
	std::cout << std::endl;
	size = Elements.size();
	dimension = Elements[0].KnotsID.size();
	std::cout << "Количество КЭ: " << size << "; Количество узлов в одном КЭ: " << dimension << std::endl;
	for (int i = 0; i < size; ++i) {
		std::cout << "ID материала: " << Elements[i].MaterialType << "; ID КЭ: " << Elements[i].ID << "; ID узлов: ";
		for (int j = 0; j < dimension; ++j)
			std::cout << Elements[i].KnotsID[j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	size = Surfaces.size();
	dimension = Surfaces[0].KnotsID.size();
	std::cout << "Количество пов. КЭ: " << size << "; Количество узлов в одном пов. КЭ: " << dimension << std::endl;
	for (int i = 0; i < size; ++i) {
		std::cout << "ID поверхности: " << Surfaces[i].ID << "; ID пов-ти с данными гр.усл: " << Surfaces[i].IDSurfaceBC << "; ID узлов: ";
		for (int j = 0; j < dimension; ++j)
			std::cout << Surfaces[i].KnotsID[j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

std::vector<Node> AneuMeshLoader::GetNodes() const{
	return Nodes;
}

std::vector<Element> AneuMeshLoader::GetElemets() const{
	return Elements;
}

std::vector<Surface> AneuMeshLoader::GetSurfaces() const{
	return Surfaces;
}

std::list<Element> AneuMeshLoader::FindFE(int KnotID1, int KnotID2, int KnotID3) {
	std::vector<int> IDs = { KnotID1, KnotID2, KnotID3 };
	std::vector<int> Sorted_IDs = IDs;
	std::sort(Sorted_IDs.begin(), Sorted_IDs.end());
	std::list<Element> found;
	auto found_element = std::find_if(Elements.begin(), Elements.end(), [&](Element elem) {
		std::vector<int> &Sorted_Elems = elem.KnotsID;
		std::sort(Sorted_Elems.begin(), Sorted_Elems.end());
		return includes(Sorted_Elems.begin(), Sorted_Elems.end(), Sorted_IDs.begin(), Sorted_IDs.end());
	});
	if (found_element != Elements.end())
		found.push_back(*found_element);
	while (found_element != Elements.end()) {
		found_element = std::find_if(++found_element, Elements.end(), [&](Element elem) {
			std::vector<int> &Sorted_Elems = elem.KnotsID;
			std::sort(Sorted_Elems.begin(), Sorted_Elems.end());
			return includes(Sorted_Elems.begin(), Sorted_Elems.end(), Sorted_IDs.begin(), Sorted_IDs.end());
		});
		if (found_element != Elements.end())
			found.push_back(*found_element);
	}
	return found;
}

std::list<Element> AneuMeshLoader::FindFE(int KnotID1, int KnotID2) {
	std::vector<int> IDs = { KnotID1, KnotID2 };
	std::vector<int> Sorted_IDs = IDs;
	std::sort(Sorted_IDs.begin(), Sorted_IDs.end());
	std::list<Element> found;
	auto found_element = std::find_if(Elements.begin(), Elements.end(), [&](Element elem) {
		std::vector<int> &Sorted_Elems = elem.KnotsID;
		std::sort(Sorted_Elems.begin(), Sorted_Elems.end());
		return includes(Sorted_Elems.begin(), Sorted_Elems.end(), Sorted_IDs.begin(), Sorted_IDs.end());
	});
	if (found_element != Elements.end())
		found.push_back(*found_element);
	while (found_element != Elements.end()) {
		found_element = std::find_if(++found_element, Elements.end(), [&](Element elem) {
			std::vector<int> &Sorted_Elems = elem.KnotsID;
			std::sort(Sorted_Elems.begin(), Sorted_Elems.end());
			return includes(Sorted_Elems.begin(), Sorted_Elems.end(), Sorted_IDs.begin(), Sorted_IDs.end());
		});
		if (found_element != Elements.end())
			found.push_back(*found_element);
	}
	return found;
}

std::set<int> AneuMeshLoader::GetSurfacesID(int IDSurfaceBC) {
	std::set<int> SurfacesID;
	std::for_each(Surfaces.begin(), Surfaces.end(), [&](Surface surface) {
		if (surface.IDSurfaceBC == IDSurfaceBC) {
			int size = surface.KnotsID.size();
			for (int i = 0; i < size; ++i)
				SurfacesID.insert(surface.KnotsID[i]);
		}
	});
	return SurfacesID;
}

std::vector<Element> AneuMeshLoader::GetElementsByMaterialType(int MaterialType) {
	std::list<Element> ElementsByMaterialType;
	std::for_each(Elements.begin(), Elements.end(), [&](Element elem) {
		if (elem.MaterialType == MaterialType) {
			ElementsByMaterialType.push_back(elem);
		}
	});
	return std::vector<Element>(ElementsByMaterialType.begin(), ElementsByMaterialType.end());
}

std::vector<Surface> AneuMeshLoader::GetSurfacesBySurfaceID(int SurfaceID) {
	std::list<Surface> SurfacesBySurfaceID;
	std::for_each(Surfaces.begin(), Surfaces.end(), [&](Surface surface) {
		if (surface.IDSurfaceBC == SurfaceID) {
			SurfacesBySurfaceID.push_back(surface);
		}
	});
	return std::vector<Surface>(SurfacesBySurfaceID.begin(), SurfacesBySurfaceID.end());
}

std::vector<std::set<int>> AneuMeshLoader::GetNodeNeighbors() {
	int size = Elements.size();
	std::vector<std::set<int>> NodeNeighbors(Nodes.size());
	for (int i = 0; i < size; ++i) {
		int size2 = Elements[i].KnotsID.size();
		for (int j = 0; j < size2; ++j) {
			for (int k = 0; k < size2; ++k) {
				if (j != k)
					NodeNeighbors[Elements[i].KnotsID[j] - 1].insert(Elements[i].KnotsID[k]);
			}
		}
	}
	return NodeNeighbors;
}

std::vector<int> AneuMeshLoader::GetNewCombination(const std::vector<int> &IDs, int iteration) {
	std::vector<int> NewCombination(2);
	if (IDs.size() == 4) {
		for (int i = 0; i < 2; ++i) {
			if (iteration <= 2)
				NewCombination[i] = i + iteration;
			if (iteration == 3 && i == 0)
				NewCombination[i] = iteration;
			if (iteration == 3 && i == 1)
				NewCombination[i] = 0;
			if (iteration == 4 && i == 0)
				NewCombination[i] = i;
			if (iteration == 4 && i == 1)
				NewCombination[i] = 2;
			if (iteration == 5 && i == 0)
				NewCombination[i] = 1;
			if (iteration == 5 && i == 1)
				NewCombination[i] = 3;
		}
	}
	if (IDs.size() == 3) {
		for (int i = 0; i < 2; ++i) {
			if (iteration < 2)
				NewCombination[i] = i + iteration;
			if (iteration == 2 && i == 0)
				NewCombination[i] = iteration;
			if (iteration == 2 && i == 1)
				NewCombination[i] = 0;
		}
	}
	return NewCombination;
}

std::vector<float> AneuMeshLoader::GetNewCoordinates(const std::vector<int> &Combination, const std::vector<int> &KnotsID) {
	std::vector<int> IDs{ KnotsID [Combination[0]], KnotsID[Combination[1]] };
	int dimension = 3;
	std::vector<float> NewCoordinates(dimension);
	for (int i = 0; i < dimension; ++i) {
		NewCoordinates[i] = fabs(Nodes[IDs[1] - 1].coordinates[i] + Nodes[IDs[0] - 1].coordinates[i])/2;
	}
	return NewCoordinates;
}

void AneuMeshLoader::TransformationToQuadraticFE() {
	if (Elements[0].KnotsID.size() == 4) {
		int count = Nodes.size();
		std::list<Node> NewNodes;
		std::vector<int> Combination(2);
		Node NewNode;
		int size = Elements.size();
		for (int i = 0; i < size; ++i) {
			std::vector<int> AddIds(6);
			for (int j = 0; j < 6; ++j) {
				Combination = GetNewCombination(Elements[i].KnotsID, j);
				NewNode.coordinates = GetNewCoordinates(Combination, Elements[i].KnotsID);
				auto Find = std::find(NewNodes.begin(), NewNodes.end(), NewNode);
				if (Find == NewNodes.end()) {
					NewNode.ID = 1 + count++;
					NewNodes.push_back(Node(NewNode));
					AddIds[j] = NewNode.ID;
				}
				else {
					AddIds[j] = (*Find).ID;
				}
			}
			for (int k = 0; k < 6; ++k) {
				Elements[i].KnotsID.resize(10);
				Elements[i].KnotsID[k + 4] = AddIds[k];
			}
		}
		auto iter = NewNodes.begin();
		int size2 = Nodes.size();
		Nodes.resize(count);
		for (; iter != NewNodes.end(); ++iter)
			Nodes[size2++] = (*iter);
		std::list<Node> NewNodes2;
		int size3 = Surfaces.size();
		for (int i = 0; i < size3; ++i) {
			std::vector<int> AddIds(3);
			for (int j = 0; j < 3; ++j) {
				Combination = GetNewCombination(Surfaces[i].KnotsID, j);
				NewNode.coordinates = GetNewCoordinates(Combination, Surfaces[i].KnotsID);
				auto Find = std::find(NewNodes.begin(), NewNodes.end(), NewNode);
				if (Find == NewNodes.end()) {
					NewNode.ID = 1 + count++;
					NewNodes.push_back(Node(NewNode));
					AddIds[j] = NewNode.ID;
				}
				else {
					AddIds[j] = (*Find).ID;
				}
			}
			for (int k = 0; k < 3; ++k) {
				Surfaces[i].KnotsID.resize(6);
				Surfaces[i].KnotsID[k + 3] = AddIds[k];
			}
		}
	}
}