#include "stdafx.h"
#include <clocale>
#include "RunApplication.h"

int main()
{
	setlocale(0, "russian");
	try {
		Run();
	}
	catch (ErrorFileType error) {
		std::cerr << error.what() << std::endl;
	}
	catch (ErrorFile error) {
		std::cerr << error.what() << std::endl;
	}
	system("pause");
	return 0;
}

