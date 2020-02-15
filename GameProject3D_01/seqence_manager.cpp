#include "main.h"
#include "game_objects_all.h"
#include "scene.h"
#include "seqence_manager.h"
#include "input.h"

bool CSeqenceManager::m_PlaySequence;
int CSeqenceManager::m_CurrentSequenceId;
int CSeqenceManager::m_CurrentSequenceTrack;
int CSeqenceManager::m_FrameCounter;
Vector3 CSeqenceManager::m_StandardPosition;
std::vector<SeqenceTrack*>	CSeqenceManager::m_Sequences[SEQUENCE_MAX];
CCamera* CSeqenceManager::m_Camera;


void CSeqenceManager::Init()
{
	//===============================================
	// クリア時のシーケンスセット
	//===============================================
	m_Sequences[SEQUENCE_CLEAR].push_back(
		new SeqenceTrack(
			SequenceParam(Vector3(5.0f, 2.0, -10.0f), Vector3(0.0f, 0.0f, 0.0f), 5.0f),
			SequenceParam(Vector3(-5.0f, 2.0, -10.0f), Vector3(0.0f, 45.0f, 0.0f), 5.0f),
			5.0f));
	m_Sequences[SEQUENCE_CLEAR].push_back(
		new SeqenceTrack(
			SequenceParam(Vector3(0.0f, 8.0f, 7.0f), Vector3(90.0f, 0.0f, 0.0f), 5.0f),
			SequenceParam(Vector3(0.0f, 8.0f, -7.0f), Vector3(90.0f, 0.0f, 0.0f), 5.0f),
			5.0f));
	m_Sequences[SEQUENCE_CLEAR].push_back(
		new SeqenceTrack(
			SequenceParam(Vector3(0.0f, 10.0f, -10.0f),Vector3(0.0f, 0.0f, 0.0f), 5.0f),
			SequenceParam(Vector3(0.0f, 3.0f, -10.0f), Vector3(0.0f, 45.0f, 0.0f), 5.0f),
			5.0f));


	//===============================================
	// 死亡時のシーケンスセット
	//===============================================
	m_Sequences[SEQUENCE_DIED].push_back(
		new SeqenceTrack(
			SequenceParam(Vector3(0.0f, 0.0f, 0.0f),Vector3(60.0f, 0.0f, 0.0f), 5.0f),
			SequenceParam(Vector3(0.0f, 0.0f, 0.0f), Vector3(20.0f, 90.0f, 0.0f), 2.0f),
			13.0f));

	m_PlaySequence = false;
	m_CurrentSequenceId = 0;
	m_CurrentSequenceTrack = 0;
	m_FrameCounter = 0;
	m_StandardPosition = Vector3();

	m_Camera = NULL;
}


void CSeqenceManager::Uninit()
{
	for (int i = 0; i < SEQUENCE_MAX; i++) {

		if(!m_Sequences[i].empty())
			m_Sequences[i].clear();
	}
}


void CSeqenceManager::Update()
{
	if (!m_PlaySequence)return;

	// 現在のシーケンスの現トラックを再生
	if (PlayCurrentSequenceTrack()) {

		// 現在のトラック再生後に次のトラックのIDをセット
		if (m_CurrentSequenceTrack < m_Sequences[m_CurrentSequenceId].size() - 1){
			m_CurrentSequenceTrack++;
			m_FrameCounter = 0;
			return;
		}

		// 現在のトラックが最後だったらシーケンス終了
		CCameraManager::DeleteCamera(m_Camera->GetId());
		m_Camera = NULL;
		m_PlaySequence = false;
		m_FrameCounter = 0;
		// 入力を有効に
		CInput::InputEnable(true);
	}
}


void CSeqenceManager::PlaySeqence(SEQUENCE_ID sequnce_id, Vector3 standard_pos)
{
	// シーケンス再生中は再生を実行できない
	if (m_PlaySequence)return;

	CCameraManager::CreateCamera();
	m_Camera = CCameraManager::GetCamera();
	m_StandardPosition = standard_pos;
	m_CurrentSequenceId = sequnce_id;
	m_PlaySequence = true;
	m_CurrentSequenceTrack = 0;

	// 入力を禁止
	CInput::InputEnable(false);
}


bool CSeqenceManager::PlayCurrentSequenceTrack()
{
	SeqenceTrack* track = m_Sequences[m_CurrentSequenceId][m_CurrentSequenceTrack];

	float lerp_deg = m_FrameCounter * DELTA_TIME / track->CompleteSec * 90.0f;
	if (lerp_deg >= 90.0f) { lerp_deg = 90.0f; }
	float t = sinf(lerp_deg * DEGREE_TO_RADIAN);

	Vector3 pos = Lerp(m_StandardPosition + track->Start.Offset, m_StandardPosition + track->End.Offset, t);
	Vector3 rot  = Lerp(track->Start.RotateRadRPY, track->End.RotateRadRPY, t);
	float length  = Lerp(track->Start.Length, track->End.Length, t);
	m_Camera->SetAtPos(&pos);
	m_Camera->SetRotation(&(rot * DEGREE_TO_RADIAN));
	m_Camera->SetLengthToAt(length);

	m_FrameCounter++;

	// 終了でtrueを返す
	return (t >= 1.0f) ? true: false;
}
