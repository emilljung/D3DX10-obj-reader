//#include "readOBJ.h"
#include "WorkingObject.h"
#include <windows.h>

//Project -> DrawLine properties -> Linker -> System -> Subsystem -> Console/Windows

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	WorkingObject wObj(hInstance);
	wObj.initiateAndRun();

	return 0;
}