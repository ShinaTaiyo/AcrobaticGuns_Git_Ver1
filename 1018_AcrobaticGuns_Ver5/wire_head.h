//===============================================================================
//
//�P�P���Q�P���F���C���[�����[wire_head.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _WIREHEAD_H_
#define _WIREHEAD_H_

//==========================================
//�C���N���[�h
//==========================================
#include "main.h"
#include "objectXAlive.h"
//===============================================================================

class CWireHead : public CObjectXAlive
{
public:
	CWireHead(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::WIREHEAD, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//�R���X�g���N�^
	~CWireHead();//�f�X�g���N�^
	HRESULT Init() override; //����������
	void Uninit() override;  //�I������
	void Update() override;  //�X�V����
	void Draw() override;    //�`�揈��
	void SetDeath() override;//���S�t���O�ݒ菈��
	static CWireHead* Create(D3DXVECTOR3 Pos,D3DXVECTOR3 Rot,D3DXVECTOR3 Move,D3DXVECTOR3 Scale,int nLife);
	const bool& GetSuccessCollision() const { return m_bCollision; }
private:
	bool CollisionSquare();//�����`�̓����蔻����s��
	bool m_bCollision;//�����蔻�肪�����������ǂ���
};
#endif // !_WIRE_H_