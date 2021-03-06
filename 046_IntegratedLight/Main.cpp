#include "stdafx.h"
#include "./Systems/Window.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	D3DDesc desc;
	desc.AppName = L"D3D Game";
	desc.Instance = instance;	//instance 프로그램의 시작주소를 나타냄
	desc.bFullScreen = false;
	desc.bVsync = false;
	desc.Handle = NULL;
	desc.Width = 1280;
	desc.Height = 720;
	D3D::SetDesc(desc);

	Window * window = new Window();
	WPARAM wParam = window->Run();	//wParam 프로그램의 종료상태
	SAFE_DELETE(window);

	return wParam;
}