//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_SHOOT,			// 発射音
	SOUND_LABEL_SE_HIT,				// 発射音
	SOUND_LABEL_SE_ATTACK,			// 攻撃音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// サウンド種別
//*****************************************************************************
enum
{
	LOOP = -1,
	ONCE,
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
private:
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	static IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// オーディオデータサイズ																
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];					// 各音素材のパラメータ

public:

	static bool InitSound(HWND hWnd);
	static void UninitSound(void);
	static void Play(SOUND_LABEL label);
	static void StopSound(SOUND_LABEL label);
	static void StopSound(void);
	static void SetVolume(SOUND_LABEL label, float volume);
	static void StopSoundExpect(SOUND_LABEL label);
};
#endif
