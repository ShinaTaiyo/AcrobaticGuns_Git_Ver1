//==========================================================
//
// デバッグ表示処理 [debugproc.h]
// Author Ibuki Okusada
//
//==========================================================
#ifndef _DEBUGPROC_H_		//このマクロが定義されていない場合
#define _DEBUGPROC_H_

//=================================================
//デバッグ表示クラス
//=================================================
class CDebugProc
{
public:

	CDebugProc();  //コンストラクタ
	~CDebugProc(); //デストラクタ
	HRESULT Init();//初期化処理
	void Uninit(); //終了処理
	void Update(); //更新処理
	void Draw();   //描画処理
	void PrintDebugProc(const char* fmt, ...);//デバッグ表示処理

private:
	//==========================================================
    // スイッチ列挙型
    //==========================================================
	typedef enum
	{
		DEBUGTYPE_OFF = 0,	//対象機能が動作していない状態
		DEBUGTYPE_ON,		//対象機能が動作している状態
		DEBUGTYPE_MAX
	}DEBUGTYPE;
	//====================================================================================================

	//==========================================================
    // マクロ定義
    //==========================================================
    #define MAX_DEBUGSTRING	(4096)	//デバッグ表示の最大文字数
    #define MAX_FLOATNUM	(2)		//小数点以下の表示桁数
    //====================================================================================================

	void SetManual();                   //操作説明

	LPD3DXFONT m_pFont2 = NULL;	        //フォントへのポインタ!
	char m_aStrDebug[MAX_DEBUGSTRING];	//デバッグ表示用の文字列!
	bool m_bDispDebug = false;	        //デバッグ表示のON/OFF!
};
//=====================================================================================================================================
#endif