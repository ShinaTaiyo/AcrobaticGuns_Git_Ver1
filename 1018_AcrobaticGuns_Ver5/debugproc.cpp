//==========================================================
//
// デバッグ表示処理 [debugproc.cpp]
// Author : Ibuki Okusada
//
//==========================================================

//==========================================================
//インクルード
//==========================================================
#include "main.h"
#include "debugproc.h"
#include <string.h>
#include <stdio.h>
#include "renderer.h"
#include "manager.h"
#include "input.h"
//====================================================================================================================================

//============================================================
//コンストラクタ
//============================================================
CDebugProc::CDebugProc() : m_aStrDebug(),m_pFont2(), m_bDispDebug(false)
{

}
//====================================================================================================================================

//============================================================
//デストラクタ
//============================================================
CDebugProc::~CDebugProc()
{

}
//====================================================================================================================================

//============================================================
//初期化処理
//============================================================
HRESULT CDebugProc::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスへのポインタを取得

    //デバッグ表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont2);

	//デバッグ表示情報のクリア
	memset(&m_aStrDebug[0], NULL, sizeof(m_aStrDebug));

	//初期表示設定
#ifdef _DEBUG
	m_bDispDebug = true;
#else NDEBUG
	m_bDispDebug = false;
#endif

	return S_OK;
}
//====================================================================================================================================

//============================================================
//終了処理
//============================================================
void CDebugProc::Uninit()
{
	//デバッグ表示用フォントの廃棄
	if (m_pFont2 != nullptr)
	{
		m_pFont2->Release();
		m_pFont2 = nullptr;
	}
}
//====================================================================================================================================

//============================================================
//更新処理
//============================================================
void CDebugProc::Update()
{
	//操作説明設定
	SetManual();

	//if (CManager::GetInputKeyboard()->GetTrigger() GetKeyboardTrigger(DIK_F1) == true)
	//{//F1キーが押されたとき
	//	g_bDispDebug = g_bDispDebug ? false : true;
	//}
}
//====================================================================================================================================

//============================================================
//描画処理
//============================================================
void CDebugProc::Draw()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (m_bDispDebug == true)
	{//デバックモードがオンの時
	 //テキストの描画
		m_pFont2->DrawText(NULL, &m_aStrDebug[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	//デバッグ表示情報のクリア
	memset(&m_aStrDebug[0], NULL, sizeof(m_aStrDebug));
}
//====================================================================================================================================

//============================================================
//デバッグ表示処理
//============================================================
void CDebugProc::PrintDebugProc(const char* fmt, ...)
{
	if (m_bDispDebug == true)
	{
		va_list args;
		char aString[MAX_DEBUGSTRING];
		char aSaveString[MAX_DEBUGSTRING];
		int nLength = 0;
		int nStopLength;

		//文字列の代入
		strcpy(&aString[0], fmt);

		va_start(args, fmt);

		for (char* p = &aString[0]; *p != '\0'; p++)
		{
			if (*p == '%')
			{//確認文字がある場合
				p++;	//確認文字まで進める

				switch (*p)
				{
				case 'd':	//数字
					snprintf(&aSaveString[0],100, "%d", va_arg(args, int));

					//文字列の長さを取得
					nLength = (int)strlen(&aSaveString[0]);
					break;
				case 'f':	//小数
					snprintf(&aSaveString[0],100, "%f", va_arg(args, double));

					//文字列の長さを取得
					nLength = (int)strlen(&aSaveString[0]);

					//小数点以下の文字目まで確認
					for (int nCntlength = 0; aSaveString[nCntlength] != '\0'; nCntlength++)
					{
						if (aSaveString[nCntlength] == '.')
						{//小数点があった場合

							//小数点以下の桁数を求める
							int nMin = (int)strlen(&aSaveString[nCntlength + 1]);

							//小数点より先の文字数と指定した値までの差分を求める
							nMin -= MAX_FLOATNUM;

							//文字列の長さを小数点以下第二位までに補正
							nLength -= nMin;
						}
					}

					break;
				case 'c':	//文字
					snprintf(&aSaveString[0],100, "%c", va_arg(args, char));

					//文字列の長さを取得
					nLength = (int)strlen(&aSaveString[0]);
					break;
				case 's':	//文字列
					vsnprintf(&aSaveString[0], sizeof(aSaveString), "%s", args);

					//文字列の長さを取得
					nLength = (int)strlen(&aSaveString[0]);
					break;
				}

				nStopLength = (int)strlen(p) + 1;
				//メモリ内の確認文字より後ろの文字列をずらす
				memmove(p + nLength - 2, p, nStopLength);

				p--;	//ポインタを%まで戻す

				//可変引数を指定された場所に挿入
				memcpy(p, &aSaveString[0], nLength);
			}
		}

		va_end(args);

		//文字列を連結する
		strcat(&m_aStrDebug[0], &aString[0]);

		int strLength = std::strlen(&m_aStrDebug[0]);
		if (strLength >= MAX_DEBUGSTRING)
		{
			assert(false);
		}
	}
}
//====================================================================================================================================

//============================================================
//操作説明
//============================================================
void CDebugProc::SetManual()
{
	//デバッグ変更とFPS
}
//====================================================================================================================================