//===============================================================================
//
//�P�P���P�V���F���C���[�����[wire.h]
//Author:ShinaTaiyo
//
//===============================================================================

//==========================================
//�C���N���[�h
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

	CWire(WIRETYPE WireType,float fRadius,float fHeight,int nNumDivsionXZ,D3DXVECTOR3 Pos,D3DXVECTOR3 Rot,int nNumDivisionY,int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::WIRE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_3D);//�R���X�g���N�^
	~CWire();//�f�X�g���N�^
	HRESULT Init() override; //����������
	void Uninit() override;  //�I������
	void Update() override;  //�X�V����
	void Draw() override;    //�`�揈��
	void SetDeath() override;//���S�t���O�ݒ菈��
	static CWire* Create(WIRETYPE Type,D3DXVECTOR3 Pos, D3DXVECTOR3 Rot,float fRadius, float fHeight,int nNumDivisionXZ,int nNumDivisionY);
protected:
	static const string s_WIRE_FILENAME[static_cast<int>(WIRETYPE::MAX)];
private:
	WIRETYPE m_Type;//���C���[�̎��
};