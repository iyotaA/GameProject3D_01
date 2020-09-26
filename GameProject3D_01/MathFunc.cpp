// インクルード ////////////////////////////////////
#include "main.h"
#include "game_objects_all.h"
#include "scene.h"
#include "camera_manager.h"
#include "MathFunc.h"

float Lerp(float start, float end, float t)
{
    return (1 - t) * start + t * end;
}

Vector3 Lerp(Vector3 start, Vector3 end, float t)
{
	return XMVectorLerp(start, end, t);
}

XMFLOAT2 GetScreenPos(Vector3 World_Pos)
{
	CCamera* camera = CCameraManager::GetCamera();

	XMMATRIX g_View = XMLoadFloat4x4(&camera->GetViewMatrix());
	XMMATRIX g_Projection = XMLoadFloat4x4(&camera->GetProjectionMatrix());

	float w = SCREEN_WIDTH / 2.0f;
	float h = SCREEN_HEIGHT / 2.0f;
	XMMATRIX viewport = {
	w, 0, 0, 0,
	0, -h, 0, 0,
	0, 0, 1, 0,
	w, h, 0, 1
	};

	// ビュー変換とプロジェクション変換
	World_Pos = XMVector3Transform(World_Pos, g_View);
	World_Pos = XMVector3Transform(World_Pos, g_Projection);

	XMFLOAT3 temp;
	XMStoreFloat3(&temp, World_Pos);
	// zで割って-1~1の範囲に収める
	// スクリーン変換
	Vector3  view_vec = XMVectorSet(temp.x / temp.z, temp.y / temp.z, 1.0f, 1.0f);
	view_vec = XMVector3Transform(view_vec, viewport);

	return XMFLOAT2(view_vec.x, view_vec.y);
}

float RotateDegree_Normalize(float deg)
{
	float fraction = deg - (int)deg;		// 端数（小数点以下）
	int degree = 360 % (int)deg;
	return fraction + degree;
}

float RotateRadian_Normalize(float rad)
{
	int deg_to_rad = (rad * RADIAN_TO_DEGREE);
	if (deg_to_rad == 0)return 0.0f;

	float radian = 360 % deg_to_rad;
	return radian * DEGREE_TO_RADIAN;
}

