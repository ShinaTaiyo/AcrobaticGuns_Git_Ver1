//=========================================================
//
//５月２２日：DirectXクラス化、キー入力クラス化、実装
//Author:ShinaTaiyo
//
//=========================================================

//=========================
//インクルード
//=========================
#include "input.h"
#include "main.h"
#include "debugproc.h"
#include "manager.h"
//=========================================================

//=========================
//静的メンバ初期化
//=========================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//=========================
//コンストラクタ
//=========================
CInput::CInput() : m_pDevice()
{

}
//=========================================================

//=========================
//デストラクタ
//=========================
CInput::~CInput()
{

}

//=========================
//初期化処理
//=========================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	
	//====================================
	//DirectInputオブジェクトの生成
	//====================================
	if (m_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	//=================================================================
	return S_OK;
}
//===============================================================

//=========================
//終了処理
//=========================
void CInput::Uninit()
{
	//=====================================
	//DirectInputオブジェクトの破棄
	//=====================================
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
	//======================================================
}

//=====================================
//コンストラクタ（キーボードクラス）
//=====================================
CInputKeyboard::CInputKeyboard() : m_aKeyState(),m_aKeyStateTrigger(),m_nRepeatCnt(0)
{
	memset(&m_aKeyState[0], NULL, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger[0], NULL, sizeof(m_aKeyStateTrigger));
}
//===============================================================

//=====================================
//デストラクタ（キーボードクラス）
//=====================================
CInputKeyboard::~CInputKeyboard()
{

}
//===============================================================

//=====================================
//初期化処理（キーボードクラス）
//=====================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_nRepeatCnt = 0;//リピート入力のカウントをする
	if (CInput::Init(hInstance, hWnd))
	{
		return E_FAIL;
	}

	//===================================
	//入力デバイスの生成
	//===================================
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard,
		&m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//========================================================

	//===================================
//データフォーマットを設定
//===================================
if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
{
	return E_FAIL;
}
//========================================================

//===================================
//協調モードを設定
//===================================
if (FAILED(m_pDevice->SetCooperativeLevel(hWnd,
	(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
{
	return E_FAIL;
}
//========================================================

//===================================
//キーボードへのアクセス権を獲得
//===================================
m_pDevice->Acquire();
//========================================================

return S_OK;
}
//===============================================================

//=====================================
//終了処理（キーボードクラス）
//=====================================
void CInputKeyboard::Uninit()
{
	//==========================================
	//入力デバイス（キーボード）の破棄
	//==========================================
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();//キーボードへのアクセスを放棄
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	//========================================================================

	CInput::Uninit();
}
//===============================================================

//=====================================
//更新処理（キーボードクラス）
//=====================================
void CInputKeyboard::Update()
{
	BYTE aKeyState[MAX_KEY];//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState
	(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];//キーボードのトリガー情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];//キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevice->Acquire();//キーボードへのアクセス権を獲得
	}
	//=====================================================================================================================================================

}
//===============================================================

//=====================================
//プレス情報（キーボードクラス）
//=====================================
bool CInputKeyboard::GetPress(int nKey)
{
	return ((m_aKeyState[nKey] & 0x80) != 0) ? true : false;
}
//===============================================================

//=====================================
//トリガー情報（キーボードクラス）
//=====================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return ((m_aKeyStateTrigger[nKey] & 0x80) != 0) ? true : false;
}
//===============================================================

//=====================================
//リピート情報（キーボードクラス）
//=====================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	if (GetPress(nKey) == true)
	{
		if (GetTrigger(nKey) == true)
		{
			m_nRepeatCnt = 0;
			return true;
		}

		m_nRepeatCnt++;

		if (m_nRepeatCnt >= m_nMAX_REPEATCNT)
		{
			m_nRepeatCnt = 0;
			return true;
		}
	}
	return false;
}
//===============================================================

//=====================================
//コンストラクタ（ジョイパッドクラス）
//=====================================
CInputJoypad::CInputJoypad() : m_joykeyStatePress(),m_joykeyStateTrigger(),m_fLSitckAimRot(0.0f)
{
	//=======================
	//メモリのクリア
	//=======================
	memset(&m_joykeyStatePress, 0, sizeof(XINPUT_STATE));
	memset(&m_joykeyStateTrigger, 0, sizeof(XINPUT_STATE));

	//===============================================================

}
//================================================================

//=====================================
//デストラクタ（ジョイパッドクラス）
//=====================================
CInputJoypad::~CInputJoypad()
{

}
//================================================================

//=====================================
//初期化処理（ジョイパッドクラス）
//=====================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	return S_OK;
}
//================================================================

//=====================================
//終了処理（ジョイパッドクラス）
//=====================================
void CInputJoypad::Uninit()
{
	//XInputのステートを設定（無効にする）
	XInputEnable(false);
}
//================================================================

//=====================================
//更新処理（ジョイパッドクラス）
//=====================================
void CInputJoypad::Update()
{
	XINPUT_STATE joykeyState;//ジョイパッドの入力情報
	//GamePad.bRightTrigger

	//ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = m_joykeyStatePress.Gamepad.wButtons;
		m_joykeyStateTrigger.Gamepad.wButtons = ~OldButton & Button;//トリガー処理（1f前と同じだったら）（絶対に１になる）

		BYTE RTTrigger = joykeyState.Gamepad.bRightTrigger;
		BYTE OldRTrigger = m_joykeyStatePress.Gamepad.bRightTrigger;
		m_joykeyStateTrigger.Gamepad.bRightTrigger = ~OldRTrigger & RTTrigger;//トリガー処理（1f前と同じだったら）（絶対に１になる）

		m_joykeyStatePress = joykeyState;//ジョイパッドのプレス情報を保存
	}
}
//================================================================

//=====================================
//プレス情報の取得（ジョイパッドクラス）
//=====================================
bool CInputJoypad::GetPress(JOYKEY key)
{
	return (m_joykeyStatePress.Gamepad.wButtons & (0x01 << key)) != 0;
}
//================================================================

//=====================================
//トリガー情報の取得（ジョイパッドクラス）
//=====================================
bool CInputJoypad::GetTrigger(JOYKEY key)
{
	return (m_joykeyStateTrigger.Gamepad.wButtons & (0x01 << key)) != 0;
}
//================================================================

//=====================================
//Lトリガーのプレス情報の取得（ジョイパッドクラス）
//=====================================
bool CInputJoypad::GetRT_Press()
{
	return m_joykeyStatePress.Gamepad.bRightTrigger != 0;//0x0004（JOYKEY_LEFT)なら0x01<<2 = 00000111 = 0x0004;
}

//==================================================================================
//Lスティックのプレス情報の取得（ジョイパッドクラス）
//==================================================================================
bool CInputJoypad::GetLStickPress(const int nDivisionRot)
{
	float LX = m_joykeyStatePress.Gamepad.sThumbLX;
	float LY = m_joykeyStatePress.Gamepad.sThumbLY;

	bool bActive = false;//スティックを押しているかどうか
	bool bSuccessDivision = false;//nDivisionRotで割った値の取得に成功したかどうか

	//determine how far the controller is pushed
	float magnitude = sqrt(LX * LX + LY * LY);

	//上で求めた角度を正規化する
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	//正規化した角度で目的の角度を求める
	float fAimRot = atan2f(normalizedLX, normalizedLY);

	//======================================
	//大まかな方向を決める
	//======================================
	float fLapRot = D3DX_PI * 2;//一周分の値
	float fDivRot = fLapRot / nDivisionRot;       //分割した向きの値

	float fRangeRotA = 0.0f;
	float fRangeRotB = 0.0f;
	for (int nCnt = 0; nCnt < nDivisionRot; nCnt++)
	{
		fRangeRotA = fDivRot * nCnt - D3DX_PI - fDivRot * 0.5f;
		fRangeRotB = fDivRot * (nCnt + 1) - D3DX_PI - fDivRot * 0.5f;

		if (fAimRot >= fRangeRotA && fAimRot <= fRangeRotB)
		{
			fAimRot = fDivRot * nCnt - D3DX_PI;
			bSuccessDivision = true;
			break;
		}
	}

	if (bSuccessDivision == false)
	{
		fAimRot = fDivRot * nDivisionRot - D3DX_PI;
	}

	//============================================================================================================



	float normalizedMagnitude = 0.0f;

	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{//スティックを押している

		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

		m_fLSitckAimRot = fAimRot;//求めた目的の角度を代入
		bActive = true;
	}
	else //if the controller is in the deadzone zero out the magnitude
	{//スティックを押していない
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
		bActive = false;
	}

	//CManager::GetDebugProc()->PrintDebugProc("LX：%f\n", normalizedLX);
	//CManager::GetDebugProc()->PrintDebugProc("LY：%f\n", normalizedLY);
	//CManager::GetDebugProc()->PrintDebugProc("if文判定：%d\n", bIfUnderstand);
	//CManager::GetDebugProc()->PrintDebugProc("スティックの角度：%f\n", fRot);

	return bActive;
}
//===============================================================

//=====================================
//Lトリガーのトリガー情報の取得（ジョイパッドクラス）
//=====================================
bool CInputJoypad::GetRT_Trigger()
{
	return  m_joykeyStateTrigger.Gamepad.bRightTrigger != 0;//0x0004（JOYKEY_LEFT)なら0x01<<2 = 00000111 = 0x0004
}
//================================================================
