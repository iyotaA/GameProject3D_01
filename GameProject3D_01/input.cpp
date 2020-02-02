
#include "main.h"

// キーボード
BYTE CInput::m_OldKeyState[256];
BYTE CInput::m_KeyState[256];

// ゲームパッド
XINPUT_STATE CInput::m_GamepadState;
XINPUT_STATE CInput::m_OldGamepadState;

void CInput::Init()
{
	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );
}

void CInput::Uninit()
{

}

void CInput::Update()
{
	memcpy( m_OldKeyState, m_KeyState, 256 );
	memcpy(&m_OldGamepadState, &m_GamepadState, sizeof(XINPUT_STATE) );

	// キーボードの状態取得
	GetKeyboardState( m_KeyState );

	// ゲームパッドの状態取得
	XInputGetState(0, &m_GamepadState);
}

///////////////////////////////////////////////////
// キーボード
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
// ゲームパッド
///////////////////////////////////////////////////
bool CInput::GetGamepadPress(int state)
{
	bool flag;
	flag = ((m_GamepadState.Gamepad.wButtons & state) &
		(m_OldGamepadState.Gamepad.wButtons & state));
	if (flag) return true;

	if (state == LEFT_TRRIGER) {
		return (m_GamepadState.Gamepad.bLeftTrigger &
			m_OldGamepadState.Gamepad.bLeftTrigger);
	}
	if (state == RIGHT_TRRIGER) {
		return (m_GamepadState.Gamepad.bRightTrigger &
			m_OldGamepadState.Gamepad.bRightTrigger);
	}

	return false;
}

bool CInput::GetGamepadTrigger(int state)
{
	bool flag;
	flag = ((m_GamepadState.Gamepad.wButtons & state) &&
		!(m_OldGamepadState.Gamepad.wButtons & state));
	if (flag) return true;

	if (state == LEFT_TRRIGER) {
		return (m_GamepadState.Gamepad.bLeftTrigger &&
			!m_OldGamepadState.Gamepad.bLeftTrigger);
	}
	if (state == RIGHT_TRRIGER) {
		return (m_GamepadState.Gamepad.bRightTrigger &&
			!m_OldGamepadState.Gamepad.bRightTrigger);
	}

	return false;
}

bool CInput::GetGamepadRelease(int state)
{
	bool flag;
	flag = (!(m_GamepadState.Gamepad.wButtons & state) &&
		(m_OldGamepadState.Gamepad.wButtons & state));
	if (flag) return true;

	if (state == LEFT_TRRIGER) {
		return (!m_GamepadState.Gamepad.bLeftTrigger &&
			m_OldGamepadState.Gamepad.bLeftTrigger);
	}
	if (state == RIGHT_TRRIGER) {
		return (!m_GamepadState.Gamepad.bRightTrigger &&
			m_OldGamepadState.Gamepad.bRightTrigger);
	}

	return false;
}

XMFLOAT2 CInput::GetGamepadLeftStick()
{
	// 入力値がデッドゾーンより小さければ0初期化
	if ((m_GamepadState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_GamepadState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(m_GamepadState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			m_GamepadState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		m_GamepadState.Gamepad.sThumbLX = 0;
		m_GamepadState.Gamepad.sThumbLY = 0;

		return XMFLOAT2(m_GamepadState.Gamepad.sThumbLX, m_GamepadState.Gamepad.sThumbLY);
	}

	return XMFLOAT2(m_GamepadState.Gamepad.sThumbLX, m_GamepadState.Gamepad.sThumbLY);
}

XMFLOAT2 CInput::GetGamepadRightStick()
{
	// 入力値がデッドゾーンより小さければ0初期化
	if ((m_GamepadState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		m_GamepadState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(m_GamepadState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			m_GamepadState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		m_GamepadState.Gamepad.sThumbRX = 0;
		m_GamepadState.Gamepad.sThumbRY = 0;

		return XMFLOAT2(m_GamepadState.Gamepad.sThumbRX, m_GamepadState.Gamepad.sThumbRY);
	}

	return XMFLOAT2(m_GamepadState.Gamepad.sThumbRX, m_GamepadState.Gamepad.sThumbRY);
}

bool CInput::GetIsInputStick(int stick)
{
	if (stick == LEFT_STICK) { return InputLeftThumb(); }
	else if (stick == RIGHT_STICK) { return InputRightThumb(); }

	return false;
}

bool CInput::InputLeftThumb()
{
	if ((m_GamepadState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_GamepadState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(m_GamepadState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			m_GamepadState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		return false;
	}

	return true;
}

bool CInput::InputRightThumb()
{
	if ((m_GamepadState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		m_GamepadState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(m_GamepadState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			m_GamepadState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		return false;
	}

	return true;
}
