//===================================================================================================================================
//
//�P�P���Q�P���F���C���[�����[wire_head.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================================

//=================================================================
//�C���N���[�h
//=================================================================
#include "wire_head.h"
#include "renderer.h"
#include "game.h"
#include "objectXInfo.h"
#include "manager.h"
#include "debugproc.h"
#include "collision.h"
//===================================================================================================================================

//=================================================================
//�R���X�g���N�^
//=================================================================
CWireHead::CWireHead(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectXAlive(nPri, bUseintPri, type, ObjType),
m_bCollision(false),m_nCoolTime(0)
{

}
//===================================================================================================================================

//=================================================================
//�f�X�g���N�^
//=================================================================
CWireHead::~CWireHead()
{

}
//===================================================================================================================================

//=================================================================
//����������
//=================================================================
HRESULT CWireHead::Init()
{
	CObjectXAlive::Init();
	return S_OK;
}
//===================================================================================================================================

//=================================================================
//�I������
//=================================================================
void CWireHead::Uninit()
{
	CObjectXAlive::Uninit();
}
//===================================================================================================================================

//=================================================================
//�X�V����
//=================================================================
void CWireHead::Update()
{
	CObjectXAlive::Update();

	CManager::GetDebugProc()->PrintDebugProc("���C���[�w�b�h�̈ʒu�F%f %f %f\n", GetPos().x, GetPos().y, GetPos().z);

	if (m_nCoolTime == 0)
	{//�O�����œ�����Ȃ��悤�ɂ��邽�߂ɃN�[���^�C����ݒ�i�O���Z����j
	    m_bCollision = CollisionSquare();
	}
	else
	{
		m_bCollision = false;
		m_nCoolTime--;
	}
}
//===================================================================================================================================

//=================================================================
//�`�揈��
//=================================================================
void CWireHead::Draw()
{
	CObjectXAlive::Draw();
}
//===================================================================================================================================

//=================================================================
//���S�t���O�ݒ菈��
//=================================================================
void CWireHead::SetDeath()
{
	CObjectXAlive::SetDeath();
}
//===================================================================================================================================

//=================================================================
//��������
//=================================================================
CWireHead* CWireHead::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Move, D3DXVECTOR3 Scale, int nLife)
{
	CWireHead* pWireHead = DBG_NEW CWireHead();
	pWireHead->Init();
	pWireHead->SetPos(Pos);
	pWireHead->SetSupportPos(Pos);
	pWireHead->SetRot(Rot);
	pWireHead->SetMove(Move);
	pWireHead->SetScale(Scale);
	pWireHead->SetLife(nLife);
	pWireHead->SetMaxLife(nLife);
	pWireHead->SetAutoSubLife(true);
	//���f�����ݒ�
	int nIdx = CManager::GetObjectXInfo()->Regist("data\\MODEL\\Wire_Head\\Wire_Head_000.x");

	//���f���������蓖�Ă�
	pWireHead->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pWireHead->SetSize();

	return pWireHead;
}
//===================================================================================================================================

//=================================================================
//�����`�̓����蔻����s��
//=================================================================
bool CWireHead::CollisionSquare()
{
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = GetTopObject(nCntPri);
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();
			CObject::OBJECTTYPE ObjType = pObj->GetObjectType();
			CObject::TYPE type = pObj->GetType();
			if (type == CObject::TYPE::BGMODEL || type == CObject::TYPE::ENEMY || type == CObject::TYPE::BLOCK)
			{
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);
				if (CCollision::CollisionSquare(GetPos(), GetVtxMax(), GetVtxMin(), pObjX->GetPos(), pObjX->GetVtxMax(), pObjX->GetVtxMin()) == true)
				{
					CManager::GetDebugProc()->PrintDebugProc("���C���[�w�b�h�̓����蔻�萬���I\n");
					return true;
				}
			}
			pObj = pNext;
		}
	}

	CManager::GetDebugProc()->PrintDebugProc("�����`�̓����蔻�莸�s�I\n");

	return false;
}
//===================================================================================================================================