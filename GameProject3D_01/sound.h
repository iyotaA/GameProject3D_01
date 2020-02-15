//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include "xaudio2.h"				// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_GAME,				// 戦闘時BGM
	SOUND_LABEL_BGM_ENVIRONMENT,	// 環境音BGM
	SOUND_LABEL_BGM_TITLE,				// タイトルBGM
	SOUND_LABEL_BGM_VICTORY,			// 勝利BGM
	SOUND_LABEL_SE_TITLE_RETURN,	// タイトルシーンの入力時
	SOUND_LABEL_SE_DON,					// ドンッ！
	SOUND_LABEL_SE_VICTORY,				// 戦闘勝利
	SOUND_LABEL_SE_ATTACK_SMALL,	// 攻撃（小）
	SOUND_LABEL_SE_ATTACK_LARGE,	// 攻撃（大）
	SOUND_LABEL_SE_DRAW_A_SWORD,	// 抜刀
	SOUND_LABEL_SE_GARD,					// ガード
	SOUND_LABEL_SE_SWING,				// 空振り
	SOUND_LABEL_SE_FOOTSTEP,			// 足音
	SOUND_LABEL_SE_WING,					// 羽ばたく音
	SOUND_LABEL_SE_DAMAGE,				// ダメージ音
	SOUND_LABEL_SE_LANDING,				// 着地音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// サウンド種別
//*****************************************************************************
enum
{
	LOOP = XAUDIO2_LOOP_INFINITE,
	ONCE = 0,
};

//*****************************************************************************
// パラメータ構造体定義
//*****************************************************************************
typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント
	float fVolume;		// ボリューム
} SOUNDPARAM;


class CSound
{
public:

	static bool InitSound(HWND hWnd);
	static void UninitSound(void);
	static void Play(SOUND_LABEL label);
	static void StopSound(SOUND_LABEL label);
	static void StopSound(void);
	static void SetVolume(SOUND_LABEL label, float volume);
	static void StopSoundExpect(SOUND_LABEL label);

private:
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	static IXAudio2* m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice* m_pMasteringVoice;				// マスターボイス
	static IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// オーディオデータサイズ
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];					// 各音素材のパラメータ

};
#endif
