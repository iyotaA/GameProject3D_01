
#include "main.h"

// �L�[�{�[�h
BYTE CInput::m_OldKeyState[256];
BYTE CInput::m_KeyState[256];

// �Q�[���p�b�h
XINPUT_STATE CInput::m_GamepadState;
XINPUT_STATE CInput::m_OldGamepadState;
bool CInput::m_IsInputLeftStick;
bool CInput::m_IsInputRightStick;

void CInput::Init()
{

	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );

	m_IsInputLeftStick = false;
}

void CInput::Uninit()
{


}

void CInput::Update()
{

	memcpy( m_OldKeyState, m_KeyState, 256 );
	memcpy(&m_OldGamepadState, &m_GamepadState, sizeof(XINPUT_STATE) );

	// �L�[�{�[�h�̏�Ԏ擾
	GetKeyboardState( m_KeyState );

	// �Q�[���p�b�h�̏�Ԏ擾
	XInputGetState(0, &m_GamepadState);

}

///////////////////////////////////////////////////
// �L�[�{�[�h
///////////////////////////////////////////////////
bool CInput::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool CInput::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}


///////////////////////////////////////////////////
// �Q�[���p�b�h
///////////////////////////////////////////////////
bool CInput::GetGamepadPress(int state)
{
	return (
		(m_GamepadState.Gamepad.wButtons & state) &
		(m_OldGamepadState.Gamepad.wButtons & state)
		);
}

bool CInput::GetGamepadTrigger(int state)
{
	return (
		(m_GamepadState.Gamepad.wButtons & state) &
		!(m_OldGamepadState.Gamepad.wButtons & state)
		);
}

XMFLOAT2 CInput::GetGamepadLeftStick()
{
	// ���͒l���f�b�h�]�[����菬�������0������
	if ((m_GamepadState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_GamepadState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(m_GamepadState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			m_GamepadState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		m_GamepadState.Gamepad.sThumbLX = 0;
		m_GamepadState.Gamepad.sThumbLY = 0;
		m_IsInputLeftStick = false;

		return XMFLOAT2(m_GamepadState.Gamepad.sThumbLX, m_GamepadState.Gamepad.sThumbLY);
	}

	m_IsInputLeftStick = true;
	return XMFLOAT2(m_GamepadState.Gamepad.sThumbLX, m_GamepadState.Gamepad.sThumbLY);
}

XMFLOAT2 CInput::GetGamepadRightStick()
{
	// ���͒l���f�b�h�]�[����菬�������0������
	if ((m_GamepadState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		m_GamepadState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(m_GamepadState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			m_GamepadState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		m_GamepadState.Gamepad.sThumbRX = 0;
		m_GamepadState.Gamepad.sThumbRY = 0;
		m_IsInputRightStick = false;

		return XMFLOAT2(m_GamepadState.Gamepad.sThumbRX, m_GamepadState.Gamepad.sThumbRY);
	}

	m_IsInputRightStick = true;
	return XMFLOAT2(m_GamepadState.Gamepad.sThumbRX, m_GamepadState.Gamepad.sThumbRY);
}

bool CInput::GetIsInputStick(int stick)
{
	assert(stick >= LEFT_STICK);
	assert(stick <= RIGHT_STICK);

	if (stick == LEFT_STICK) {
		return m_IsInputLeftStick;
	}
	else if (stick == RIGHT_STICK) {
		return m_IsInputRightStick;
	}

	return false;
}
