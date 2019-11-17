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
#include "Vector3.h"
using namespace DirectX;


#define SCREEN_WIDTH		(1366)			// ウインドウの幅
#define SCREEN_HEIGHT		(768) 			// ウインドウの高さ
#define PI					(3.1415926535f)	// 円周率
#define FAILD_NUM			(-99)
#define DELTA_TIME			(1.0 / 60.0)

#define SAFE_RELEASE(p) if(p){ p->Release(); p=NULL; }
#define SAFE_DELETE(p) if(p){ delete[] p; p=NULL; }


#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "xaudio2.lib")

// 方向ベクトル
struct Dir3Vector
{
	Vector3 front;
	Vector3 right;
	Vector3 up;
};

HWND GetWindow();
void DrawFPS();

#endif
