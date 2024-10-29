//==================================================================================
//
//１０月２８日：狙った方向に弾を発射する[ui3d.h]
//Author:ShinaTaiyo
//
//===================================================================================
#ifndef _UI3D_H_
#define _UI3D_H_

//===================================================================================
//インクルード
//===================================================================================
#include "main.h"
#include "billboard.h"
//===================================================================================

//===================================================================================
//UI3Dクラス
//===================================================================================
class CUi3D : public CBillboard
{
public:
	typedef enum
	{
		UI3DTYPE_LOCKON = 0,
		UI3DTYPE_MAX
	}UI3DTYPE;

	CUi3D();                                                                 //コンストラクタ
	~CUi3D();                                                                //デストラクタ
	HRESULT Init(void) override;                                             //初期化処理
	void Uninit(void) override;                                              //終了処理
	void Update(void) override;                                              //更新処理
	void Draw(void) override;                                                //描画処理
	static CUi3D* Create(UI3DTYPE type,D3DXVECTOR3 Pos,D3DXVECTOR3 Move,int nLife,float fWidth,float fHeight); //生成処理
protected:
	static const string m_UI3D_FILENAME[UI3DTYPE_MAX];
	void SetUi3DType(UI3DTYPE type) { m_Type = type; }                           //種類を設定
	UI3DTYPE GetUi3DType() { return m_Type; }                                    //種類を取得
private:
	UI3DTYPE m_Type;
};
//===================================================================================


#endif