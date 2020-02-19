
// インクルード ////////////////////////////////////
#include "game_objects_all.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "shader_all.h"


// グローバル変数 ////////////////////////////////////
CScene*	CManager::m_Scene;
bool CManager::m_GameClear;
bool CManager::m_GameFailed;

void CManager::Init()
{
	// レンダラーの初期化
	CRenderer::Init();

	// インプットの初期化
	CInput::Init();

	// サウンドの初期化
	CSound::InitSound(GetWindow());

	// シェーダーの初期化
	ShaderManager::Init();

	// コリジョングリッド初期化
	CDebugPrimitive::DebugPrimitive_Init();

	// フェードマネージャーの初期化
	CFadeManager::Init();

	// シーンの初期化
	SetScene<CGame>();

	//カメラマネージャーの設定
	CCameraManager::Init();

	// imgui初期化
	CImgui::Init();

	m_GameClear = false;
	m_GameFailed = false;
}

void CManager::Uninit()
{
	// imgui終了処理
	CImgui::Uninit();

	// シーンの終了処理
	m_Scene->Uninit();
	delete m_Scene;

	// フェードマネージャーの終了処理
	CFadeManager::Uninit();

	// コリジョングリッド終了処理
	CDebugPrimitive::DebugPrimitive_Uninit();

	// シェーダーの終了処理
	ShaderManager::Uninit();

	// UI破壊
	CUserInterfaceManager::DeleteAllUI();

	// カメラ破壊
	CCameraManager::DeleteCamera();

	// サウンドの終了処理
	CSound::UninitSound();

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

	// フェードマネージャー更新
	CFadeManager::Update();

	// UI更新
	CUserInterfaceManager::Update();

	// シーンの更新
	m_Scene->Update();
}

void CManager::Draw()
{
	// imgui描画準備
	CImgui::NewFrame();

	//// Zバッファ書き込み
	//CRenderer::BeginDepth();
	//m_Scene->Draw();

	// 描画の開始
	CRenderer::Begin();

	// カメラ投影
	CCameraManager::Project();

	// シーンの描画
	m_Scene->Draw();
	//m_Scene->DrawGUI();

	// UI描画
	CUserInterfaceManager::Draw();

	// imgui描画
	CImgui::Draw();

	CRenderer::End();
	// 描画の終了

}

CScene* CManager::GetScene()
{
	return m_Scene;
}
