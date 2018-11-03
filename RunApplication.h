#pragma once
#include "stdafx.h"
#include "Exception.h"
#include "AneuMeshLoader.h"

int Menu() {
	int choose;
	std::cout << "Меню:" << std::endl;
	std::cout << "1 - загрузить сетку" << std::endl;
	std::cout << "2 - вывести сетку" << std::endl;
	std::cout << "3 - найти КЭ по ID трех его вершинных узлов" << std::endl;
	std::cout << "4 - найти КЭ по ребру" << std::endl;
	std::cout << "5 - создать контейнер поверхностных узлов по ID поверхности" << std::endl;
	std::cout << "6 - создать контейнер КЭ с заданным ID материала" << std::endl;
	std::cout << "7 - создать контейнер поверхностных КЭ с заданным ID поверхности" << std::endl;
	std::cout << "8 - преобразовать симплексные тетраэдральные КЭ в квадратичные путем внесения новых узлов в загруженную КЭ сетку" << std::endl;
	std::cout << "9 - создать контейнер, n-ый элемент которого хранит контейнер всех «соседей» для узла n" << std::endl;
	std::cout << "10 - выход" << std::endl;
	std::cin >> choose;
	return choose;
}

void Run() {
	setlocale(0, "russian");
	std::cout << "Введите название файла" << std::endl;
	std::string filename;
	std::cin >> filename;
	std::string filetype = filename.substr(filename.find("."), (filename.size() - filename.find(".")));
	if (filetype != ".aneu")
		throw ErrorFileType(filetype);
	int choose = Menu();
	MeshLoader *Mesh = new AneuMeshLoader(filename);
	AneuMeshLoader *AnueMesh = dynamic_cast<AneuMeshLoader*> (Mesh);
	while (choose != 10) {
		switch (choose) {
		case 1: {
			AnueMesh->LoadMesh();
			break;
		}
		case 2: {
			AnueMesh->Output();
			break;
		}
		case 3: {
			std::cout << "Введите ID трёх вершин" << std::endl;
			int ID1, ID2, ID3;
			std::cin >> ID1 >> ID2 >> ID3;
			std::list<Element> FEs = AnueMesh->FindFE(ID1, ID2, ID3);
			std::cout << "Найденные КЭ" << std::endl;
			if (FEs.size() != 0)
				std::for_each(FEs.begin(), FEs.end(), [=](Element elem) {std::cout << elem.ID << " "; });
			else
				std::cout << "КЭ не найдены!" << std::endl;
			break;
		}
		case 4: {
			std::cout << "Введите ID двух вершин" << std::endl;
			int ID1, ID2;
			std::cin >> ID1 >> ID2;
			std::list<Element> FEs = AnueMesh->FindFE(ID1, ID2);
			std::cout << "Найденные КЭ: " << std::endl;
			if (FEs.size() != 0)
				std::for_each(FEs.begin(), FEs.end(), [=](Element elem) {std::cout << elem.ID << " "; });
			else
				std::cout << "КЭ не найдены!" << std::endl;
			break;
		}
		case 5: {
			std::cout << "Введите ID поверхности" << std::endl;
			int ID;
			std::cin >> ID;
			std::set<int> IDs = AnueMesh->GetSurfacesID(ID);
			std::cout << "Найденные поверхностные узлы: " << std::endl;
			if (IDs.size() != 0)
				std::for_each(IDs.begin(), IDs.end(), [=](int ID) {std::cout << ID << " "; });
			else
				std::cout << "Поверхности не найдены!" << std::endl;
			break;
		}
		case 6: {
			std::cout << "Введите ID материала" << std::endl;
			int ID;
			std::cin >> ID;
			std::vector<Element> FEs = AnueMesh->GetElementsByMaterialType(ID);
			std::cout << "Найденные КЭ: " << std::endl;
			if (FEs.size() != 0)
				std::for_each(FEs.begin(), FEs.end(), [=](Element elem) {std::cout << elem.ID << " "; });
			else
				std::cout << "КЭ не найдены!" << std::endl;
			break;
		}
		case 7: {
			std::cout << "Введите ID поверхности с данными гр.усл" << std::endl;
			int ID;
			std::cin >> ID;
			std::vector<Surface> FEs = AnueMesh->GetSurfacesBySurfaceID(ID);
			std::cout << "Найденные поверхностные КЭ с данными гр.усл: " << std::endl;
			if (FEs.size() != 0)
				std::for_each(FEs.begin(), FEs.end(), [=](Surface elem) {std::cout << elem.ID << " "; });
			else
				std::cout << "КЭ не найдены!" << std::endl;
			break;
		}
		case 8: {
			AnueMesh->TransformationToQuadraticFE();
			break;
		}
		case 9: {
			std::vector<std::set<int>> Neighbors = AnueMesh->GetNodeNeighbors();
			std::cout << "Найденные соседи каждого узла: " << std::endl;
			int size = Neighbors.size();
			for (int i = 0; i < size; ++i) {
				std::cout << std::endl << "Для узла: " << i + 1 << std::endl;
				std::for_each(Neighbors[i].begin(), Neighbors[i].end(), [=](int id) {std::cout << id << " "; });
			}
			break;
		}
		}
		std::cout << std::endl;
		choose = Menu();
	}
}