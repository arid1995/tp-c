/*
	�����(�): ��������, ���������, ������ � ����������
	�������� �������: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#include "header.h"
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")






// ����� ����� � ����������
int WINAPI  _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	
	::CoInitialize(NULL);   // ��� IDirectSound(COM)

	xGame game(hInstance);
	game.Create(_T("������� TM"), 800, 600, true);
	game.Run();

	::CoUninitialize();
	return 0;
}
