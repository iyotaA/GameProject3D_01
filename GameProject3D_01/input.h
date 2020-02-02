#ifndef INPUT_H_
#define INPUT_H_

#define LEFT_STICK    (0)
#define RIGHT_STICK   (1)
#define LEFT_TRRIGER  (3)
#define RIGHT_TRRIGER (4)

class CInput
{
public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress(BYTE KeyCode);
	static bool GetKeyTrigger(BYTE KeyCode);

	static bool GetGamepadPress(int state);
	static bool GetGamepadTrigger(int state);
	static bool GetGamepadRelease(int state);
	static bool GetIsInputStick(int stick);
	static XMFLOAT2 GetGamepadLeftStick();
	static XMFLOAT2 GetGamepadRightStick();

private:
	static bool InputLeftThumb();
	static bool InputRightThumb();

private:
	// キーボード
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

	// ゲームパッド
	static XINPUT_STATE m_GamepadState;
	static XINPUT_STATE m_OldGamepadState;
};

#endif
