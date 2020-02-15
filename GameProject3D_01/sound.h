//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include "xaudio2.h"				// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_GAME,				// �퓬��BGM
	SOUND_LABEL_BGM_ENVIRONMENT,	// ����BGM
	SOUND_LABEL_BGM_TITLE,				// �^�C�g��BGM
	SOUND_LABEL_BGM_VICTORY,			// ����BGM
	SOUND_LABEL_SE_TITLE_RETURN,	// �^�C�g���V�[���̓��͎�
	SOUND_LABEL_SE_DON,					// �h���b�I
	SOUND_LABEL_SE_VICTORY,				// �퓬����
	SOUND_LABEL_SE_ATTACK_SMALL,	// �U���i���j
	SOUND_LABEL_SE_ATTACK_LARGE,	// �U���i��j
	SOUND_LABEL_SE_DRAW_A_SWORD,	// ����
	SOUND_LABEL_SE_GARD,					// �K�[�h
	SOUND_LABEL_SE_SWING,				// ��U��
	SOUND_LABEL_SE_FOOTSTEP,			// ����
	SOUND_LABEL_SE_WING,					// �H�΂�����
	SOUND_LABEL_SE_DAMAGE,				// �_���[�W��
	SOUND_LABEL_SE_LANDING,				// ���n��
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �T�E���h���
//*****************************************************************************
enum
{
	LOOP = XAUDIO2_LOOP_INFINITE,
	ONCE = 0,
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

	static IXAudio2* m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	static IXAudio2MasteringVoice* m_pMasteringVoice;				// �}�X�^�[�{�C�X
	static IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	static BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// �I�[�f�B�I�f�[�^�T�C�Y
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];					// �e���f�ނ̃p�����[�^

};
#endif
