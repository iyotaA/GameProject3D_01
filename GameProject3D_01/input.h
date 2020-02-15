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
	static bool GetKeyRelease(BYTE KeyCode);

	static bool GetGamepadPress(int button);
	static bool GetGamepadTrigger(int button);
	static bool GetGamepadRelease(int button);
	static bool GetIsInputStick(int stick);
	static XMFLOAT2 GetGamepadLeftStick();
	static XMFLOAT2 GetGamepadRightStick();

	static void InputEnable(bool enable);
	static void EnableKeyCode(BYTE code);
	static void EnableGamepadButton(int button);
	static void EnableGamepadStick(bool enable);

private:
	static bool InputLeftThumb();
	static bool InputRightThumb();

private:
	static bool m_InputEnable;
	static bool m_StickEnable;
	static std::vector<BYTE> m_EnableKeyCode;
	static std::vector<int> m_EnableGamepadButton;

	// キーボード
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

	// ゲームパッド
	static XINPUT_STATE m_GamepadState;
	static XINPUT_STATE m_OldGamepadState;
};

#endif
