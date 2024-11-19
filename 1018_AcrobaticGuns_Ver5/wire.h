//===============================================================================
//
//１１月１７日：ワイヤーを作る[wire.h]
//Author:ShinaTaiyo
//
//===============================================================================

//==========================================
//インクルード
//==========================================
#include "main.h"
#include "meshcylinder.h"
//===============================================================================

class CWire : public CMeshCylinder
{
public:
	enum class WIRETYPE
	{
		NORMAL = 0,
		MAX
	};

	CWire(WIRETYPE WireType,float fRadius,float fHeight,int nNumDivsionXZ,D3DXVECTOR3 Pos,D3DXVECTOR3 Rot,int nNumDivisionY,int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::WIRE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_3D);//コンストラクタ
	~CWire();//デストラクタ
	HRESULT Init() override; //初期化処理
	void Uninit() override;  //終了処理
	void Update() override;  //更新処理
	void Draw() override;    //描画処理
	void SetDeath() override;//死亡フラグ設定処理
	static CWire* Create(WIRETYPE Type,D3DXVECTOR3 Pos, D3DXVECTOR3 Rot,float fRadius, float fHeight,int nNumDivisionXZ,int nNumDivisionY);
protected:
	static const string s_WIRE_FILENAME[static_cast<int>(WIRETYPE::MAX)];
private:
	WIRETYPE m_Type;//ワイヤーの種類
};