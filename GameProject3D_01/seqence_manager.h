#ifndef SEQENCE_MANAGER_H_
#define SEQENCE_MANAGER_H_

#include "camera_manager.h"
#include "camera.h"
#include "Vector3.h"

struct SequenceParam
{
	SequenceParam(Vector3 offset, Vector3 rotateRad_r_p_y, float length)
		: Offset(offset)
		, RotateRadRPY(rotateRad_r_p_y)
		, Length(length)
	{}
	Vector3 Offset;			// m_StandardPosition����̃I�t�Z�b�g
	Vector3 RotateRadRPY;	// ��]�p�x�i���W�A���jRoll, Pitch, Yaw
	float Length;
};

struct SeqenceTrack {
	SeqenceTrack(SequenceParam start, SequenceParam end, float sec)
		: Start(start), End(end), CompleteSec(sec)
	{}
	SequenceParam Start, End;	 // �J�n�E�I��
	float CompleteSec;  // �����܂ł̕b��
};

class CSeqenceManager
{
public:
	typedef enum
	{
		SEQUENCE_CLEAR,
		SEQUENCE_DIED,
		SEQUENCE_MAX
	}SEQUENCE_ID;

	static void Init();
	static void Uninit();
	static void Update();
	static void PlaySeqence(SEQUENCE_ID sequnce_id, Vector3 standard_pos);

private:
	static bool PlayCurrentSequenceTrack();

private:
	static bool m_PlaySequence;
	static int m_CurrentSequenceId;
	static int m_CurrentSequenceTrack;
	static int m_FrameCounter;
	static Vector3 m_StandardPosition;
	static std::vector<SeqenceTrack*>	m_Sequences[SEQUENCE_MAX];
	static CCamera* m_Camera;

};

#endif // !SEQENCE_MANAGER_H_
