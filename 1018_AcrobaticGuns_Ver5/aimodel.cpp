//====================================================================================================================
//
//�X���V���FAI�p�̃��f�������[aimodel.cpp]
//Author:ShinaTaiyo
//
//====================================================================================================================

//===========================================================
//�C���N���[�h
//===========================================================
#include "manager.h"
#include "renderer.h"
#include "aimodel.h"
#include "objectXInfo.h"
//====================================================================================================================

//===========================================================
//�ÓI�����o
//===========================================================
const string CAIModel::m_aAIMODEL_FILENAME[static_cast<int>(CAIModel::AIMODELTYPE::MAX)] =
{
	"data\\MODEL\\AIModel\\MoveAiPoint_000.x"
};//���f���t�@�C����

//===========================================================
//�R���X�g���N�^
//===========================================================
CAIModel::CAIModel(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :CObjectX(nPri,bUseintPri,type,ObjType)
{

}
//====================================================================================================================

//===========================================================
//�f�X�g���N�^
//===========================================================
CAIModel::~CAIModel()
{
}
//====================================================================================================================

//===========================================================
//����������
//===========================================================
HRESULT CAIModel::Init()
{
	CObjectX::Init();
	return S_OK;
}
//====================================================================================================================

//===========================================================
//�I������
//===========================================================	
void CAIModel::Uninit()
{
	CObjectX::Uninit();
}
//====================================================================================================================

//===========================================================
//�X�V����
//===========================================================	
void CAIModel::Update()
{
	CObjectX::Update();
}
//====================================================================================================================

//===========================================================
//�`�揈��
//===========================================================	
void CAIModel::Draw()
{
	//CObjectX::Draw();
}
//====================================================================================================================

//===========================================================
//�`�揈���i�I�u�W�F�N�g��Draw�ŌĂт����Ȃ��̂ŕʘg�j
//===========================================================	
void CAIModel::ExtraDraw()
{
	CObjectX::Draw();
}
//====================================================================================================================

//===========================================================
//���S�t���O�ݒ菈��
//===========================================================	
void CAIModel::SetDeath()
{
	CObject::SetDeath();
}
//====================================================================================================================

CAIModel* CAIModel::Create(AIMODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale, D3DXMATRIX* mtxWorld)
{
	CAIModel* pAiModel = DBG_NEW CAIModel;   //�G�𐶐�
	bool bSuccess = pAiModel->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	pAiModel->Init();                                                               //����������
	pAiModel->SetUseDeath(false);                                                   //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
	pAiModel->m_Type = type;                                                        //�G�̎��
	pAiModel->CObjectX::SetPos(pos);                                                //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
	pAiModel->SetPosOld(pos);                                                       //1f�O�̈ʒu��ݒ�
	pAiModel->SetUseDraw(true);
	pAiModel->SetUseShadow(true);
	pAiModel->SetSupportPos(pos);                                                   //�ݒu�ʒu
	pAiModel->CObjectX::SetScale(Scale);                                            //�I�u�W�F�N�g�̊g�嗦
	pAiModel->SetFormarScale(Scale);                                                //���̊g�嗦��ݒ肷��
	pAiModel->SetMtxParent(mtxWorld);                                               //�e�}�g���b�N�X��ݒ肷��
	nIdx = CManager::GetObjectXInfo()->Regist(m_aAIMODEL_FILENAME[static_cast<int>(type)]);
	pAiModel->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));                           //���f���������蓖�Ă�

	pAiModel->SetSize();                                                            //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
	return pAiModel;
}
//====================================================================================================================