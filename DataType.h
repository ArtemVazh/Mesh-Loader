#pragma once
#include <string>
#include <vector>

struct Node {
	int ID;
	std::vector<float> coordinates;
	bool operator==(const Node &NodeCheck) {
		int dimension = coordinates.size();
		int count = 0;
		for (int i = 0; i < dimension; ++i) {
			if (coordinates[i] == NodeCheck.coordinates[i])
				++count;
		}
		return (count == dimension);
	}
};

struct Element {
	int ID;
	int MaterialType;
	std::vector<int> KnotsID;
};

struct Surface {
	int ID;
	int IDSurfaceBC;
	std::vector<int> KnotsID;
};