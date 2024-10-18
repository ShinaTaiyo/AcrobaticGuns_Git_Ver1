//=========================================================
//
//リザルト画面で使う変数、宣言、構造体[text.h]
// Author seiya kagaya
//
//=========================================================
#ifndef _TEXT_H

#define _TEXT_H_//二重インクルード防止

#include"main.h"

class CText
{
public:
	//フォントの種類
    //--------------------フォントのラベル的な
	typedef enum
	{
		FONT_DOKIDOKI = 0,//どきどきファンタジア
		FONT_AKABARASINDELERA,//赤薔薇シンデレラ
		FONT_KEIFONT,//けいふぉんと
		FONT_SOUKOUMINCHO,//装甲明朝
		FONT_KIRARAJI04,//きらら字04
		FONT_MAX,
	}FONT;
    #define MAXFONT (FONT_MAX)//---フォントの数

	//プロトタイプ宣言
	CText();              //コンストラクタ
    ~CText();             //デストラクタ 
	void Init(void);  //初期化処理
	void Uninit(void);//終了処理
	void DrawSet(D3DXVECTOR3 DrawPos, int FontSize, FONT Font, D3DXCOLOR RGBA, const char* Text, ...);//テキスト書き出し処理
private:

    //フォント構造体
	typedef struct
	{
		const char* pFilePass;	// ファイルパス
		const char* pFontName;	// フォントネーム
	} FONTINFO;

	// フォントの情報
	//-------------------------------------------
	//　　"ファイルパス","フォント名"
	//-------------------------------------------
	static const FONTINFO m_FontInfo[FONT_MAX];
	LPD3DXFONT m_pFont_Comment[MAXFONT] = {};//フォントへのポインタ!
};


#endif // !_TEXT_H_