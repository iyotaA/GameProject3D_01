#ifndef MAIN_H_
#define MAIN_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <Windows.h>
#include <assert.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;


#define SCREEN_WIDTH		(1366)			// �E�C���h�E�̕�
#define SCREEN_HEIGHT		(768) 			// �E�C���h�E�̍���
#define PI					(3.1415926535f)	// �~����
#define FAILD_NUM			(-99)


#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "xaudio2.lib")

// �����x�N�g��
typedef struct Dir3Vector {
	XMVECTOR front;
	XMVECTOR right;
	XMVECTOR up;
}Dir3Vector;

HWND GetWindow();
void DrawFPS();

#endif
