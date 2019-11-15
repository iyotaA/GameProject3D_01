//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_SHOOT,			// ���ˉ�
	SOUND_LABEL_SE_HIT,				// ���ˉ�
	SOUND_LABEL_SE_ATTACK,			// �U����
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �T�E���h���
//*****************************************************************************
enum
{
	LOOP = -1,
	ONCE,
};

//*****************************************************************************
// �p�����[�^�\���̒�`
//*****************************************************************************
typedef struct
{
	char *pFilename;	// �t�@�C����
	int nCntLoop;		// ���[�v�J�E���g
	float fVolume;		// �{�����[��
} SOUNDPARAM;


class CSound
{
private:
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	static IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// �I�[�f�B�I�f�[�^�T�C�Y																
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];					// �e���f�ނ̃p�����[�^

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
