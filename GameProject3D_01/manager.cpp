
// インクルード ////////////////////////////////////
#include "game_objects_all.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"

// グローバル変数 ////////////////////////////////////
CScene*	CManager::m_Scene;
int CManager::m_Score;


void CManager::Init()
{
	// レンダラーの初期化
	CRenderer::Init();

	// インプットの初期化
	CInput::Init();

	// コリジョングリッド初期化
	CDebugPrimitive::DebugPrimitive_Init();

	//カメラの生成
	CCameraManager::CreateCamera();
	CCameraManager::CreateCamera();

	// シーンの初期化
	SetScene<CGame>();

	//カメラマネージャーの設定
	CCameraManager::SetCurrentCameraId(0);
	CCameraManager::Init();

	// imgui初期化
	CImgui::Init();

	m_Score = 0;
}

void CManager::Uninit()
{
	// imgui終了処理
	CImgui::Uninit();

	// シーンの終了処理
	m_Scene->Uninit();
	delete m_Scene;

	// カメラ破壊
	CCameraManager::DeleteCamera();

	// コリジョングリッド終了処理
	CDebugPrimitive::DebugPrimitive_Uninit();

	// インプットの終了処理
	CInput::Uninit();

	// レンダラーの終了処理
	CRenderer::Uninit();
}

void CManager::Update()
{
	// インプットの更新
	CInput::Update();

	// カメラ更新
	CCameraManager::Update();

	// シーンの更新
	m_Scene->Update();
}

void CManager::Draw()
{
	// 描画の開始
	CRenderer::Begin();

	// imgui描画準備
	CImgui::NewFrame();

	// GUIの描画
	DrawGUI();

	// カメラ投影
	CCameraManager::Project();

	// シーンの描画
	m_Scene->Draw();

	// imgui描画
	CImgui::Draw();

	CRenderer::End();
	// 描画の終了

}

void CManager::DrawGUI()
{
	DrawFPS();
}

CScene* CManager::GetScene()
{
	return m_Scene;
}
