//=======================================================================================
//
//６月４日：モデルを描画する[field.h]
//Author:ShinaTaiyo
//
//=======================================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

//==============================================
//インクルード
//==============================================
#include "main.h"
#include "object.h"
#include "object3d.h"
//=======================================================================================

//=======================================================
//フィールドクラス
//=======================================================
class CField : public CObject3D
{
public:
	typedef enum
	{
		FIELDTYPE00_NORMAL = 0,//普通
		FIELDTYPE01_FLOOR,     //フロア
		FIELDTYPE_MAX
	}FIELDTYPE;

	CField();                                                                       //コンストラクタ
	~CField() override;                                                             //デストラクタ
	HRESULT Init() override;                                                        //初期化処理
	void Uninit() override;                                                         //終了処理
	void ExtraUninit() override;                                                    //別枠の終了処理
	void Update() override;                                                         //更新処理
	void Draw() override;                                                           //描画処理
	void SetDeath() override;                                                       //死亡フラグを設定
	static CField* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight,FIELDTYPE FieldType);  //生成処理

private:
	FIELDTYPE m_FieldType;                                                          //フィールドの種類
	static const char* m_apFIELD_FILENAME[FIELDTYPE_MAX];                           //フィールドのテクスチャのファイル名
};
//============================================================================================================

#endif