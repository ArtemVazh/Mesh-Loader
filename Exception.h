#pragma once
#include "stdafx.h"
#include <string>
#include <exception>
#include <iostream>

class ErrorFileType : public std::exception {
public: 
	std::string errortype;
	ErrorFileType(std::string type) {
		errortype = type;
	}
	const char* what() const throw() {
		std::cout << "Неверный тип файла! (" << errortype << ")";
		return "";
	}
};

class ErrorFile : public std::exception {
public:
	std::string errorfile;
	ErrorFile(std::string filename) {
		errorfile = filename;
	}
	const char* what() const throw() {
		std::cout << "Файл не найден! (" << errorfile << ")";
		return "";
	}
};
