//======================================================================================================================
//
//�P�O���Q�S���F�U���p�̃N���X�����[BgModel.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//==================================================================
//�C���N���[�h
//==================================================================
#include "bgModel.h"
#include "manager.h"
#include "objectX.h"
#include "objectXInfo.h"
//======================================================================================================================


//==================================================================
//�O���錾
//==================================================================
const string CBgModel::BGMODEL_FILENAME[CBgModel::BGMODELTYPE::TYPE_MAX] =
{
	"data\\MODEL\\BgModel\\bill.x",
	"data\\MODEL\\BgModel\\Tree_000.x",
};
//======================================================================================================================

//==================================================================
//�R���X�g���N�^
//==================================================================
CBgModel::CBgModel() : m_Type(BGMODELTYPE::TYPE_BILL_00)
{

}
//======================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CBgModel::~CBgModel()
{

}
//======================================================================================================================

//==================================================================
//����������
//==================================================================
HRESULT CBgModel::Init()
{
	CObjectX::Init();
	return S_OK;
}
//======================================================================================================================

//==================================================================
//�I������
//==================================================================
void CBgModel::Uninit()
{
	CObjectX::Uninit();
}
//======================================================================================================================

//==================================================================
//�X�V����
//==================================================================
void CBgModel::Update()
{
	CObjectX::Update();
}
//======================================================================================================================

//==================================================================
//�`�揈��
//==================================================================
void CBgModel::Draw()
{
	CObjectX::Draw();
}
//======================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CBgModel::SetDeath()
{
	CObject::SetDeath();
}
//======================================================================================================================

//==================================================================
//��������
//==================================================================
CBgModel* CBgModel::Create(BGMODELTYPE bgModelType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CBgModel* pBgModel = DBG_NEW CBgModel;     //����

	pBgModel->Init();                        //����������
	pBgModel->SetType(CObject::TYPE_BGMODEL);//�I�u�W�F�N�g���Ƃ̃^�C�v��ݒ肷��
	pBgModel->SetBgModelType(bgModelType);   //�w�i���f���̎�ނ�ݒ肷��
	pBgModel->SetPos(pos);                   //�ʒu  
	pBgModel->SetSupportPos(pos);            //�x�_�ƂȂ�ʒu��ݒ�
	pBgModel->SetRot(rot);                   //����
	pBgModel->SetScale(Scale);               //�g�嗦

	//���f�����ݒ�
	int nIdx = CManager::GetObjectXInfo()->Regist(BGMODEL_FILENAME[bgModelType]);

	//���f���������蓖�Ă�
	pBgModel->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pBgModel->SetManagerObjectType(CObject::MANAGEROBJECTTYPE_BGMODEL);           //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�

	return pBgModel;
}
//======================================================================================================================


//==================================================================
//�X�e�[�W�}�l�[�W���[�ɏ���ۑ�����
//==================================================================
void CBgModel::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETBGMODEL" << endl;
	WritingFile << "TYPE = " << m_Type;
	switch (m_Type)
	{
	case BGMODELTYPE::TYPE_BILL_00:
		WritingFile << " # BILL00" << endl;
		break;
	case BGMODELTYPE::TYPE_TREE_00:
		WritingFile << " # TREE00" << endl;
		break;
	default:
		break;
	}

	CObjectX::SaveInfoTxt(WritingFile);

	WritingFile << "END_SETBGMODEL" << endl;

}
//======================================================================================================================

//==================================================================
//�X�e�[�W�}�l�[�W���[�������ǂݍ���
//==================================================================
void CBgModel::LoadInfoTxt(fstream& LoadingFile, vector<CObject*>& VecSaveManager, string& Buff)
{
	int nType = 0;//���
	int nLife = 0;//�̗�
	D3DXVECTOR3 Move = NULL_VECTOR3;  //�ړ���
	D3DXVECTOR3 Pos = NULL_VECTOR3;   //�ʒu
	D3DXVECTOR3 Scale = NULL_VECTOR3; //�g�嗦
	D3DXVECTOR3 Rot = NULL_VECTOR3;   //����
	BGMODELTYPE Type = {};            //�w�i���f���̎��
	while (Buff != "END_SETBGMODEL")
	{
		LoadingFile >> Buff;//�P���ǂݍ���
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "TYPE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nType;      //���
		}
		else if (Buff == "POS")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Pos.x;      //�ʒuX
			LoadingFile >> Pos.y;      //�ʒuY
			LoadingFile >> Pos.z;      //�ʒuZ
		}
		else if (Buff == "ROT")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Rot.x;      //�ʒuX
			LoadingFile >> Rot.y;      //�ʒuY
			LoadingFile >> Rot.z;      //�ʒuZ
		}
		else if (Buff == "SCALE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Scale.x;      //�g�嗦X
			LoadingFile >> Scale.y;      //�g�嗦Y
			LoadingFile >> Scale.z;      //�g�嗦Z
		}
	}
	Type = BGMODELTYPE(nType);

	VecSaveManager.push_back(CBgModel::Create(Type,Pos, Rot, Scale));//vector�ɏ���ۑ�����

}
//======================================================================================================================

//==================================================================
//��ނ�ς���
//==================================================================
CObject* CBgModel::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_Type);
	BGMODELTYPE NewType = {};
	const D3DXVECTOR3& Pos = GetPos();
	//=======================================
	//��ނ�ς���
	//=======================================
	if (bAim == true)
	{
		nNewType++;
	}
	else
	{
		nNewType--;
	}
	if (nNewType >= BGMODELTYPE::TYPE_MAX)
	{
		nNewType = 0;
	}
	if (nNewType < 0)
	{
		nNewType = BGMODELTYPE::TYPE_MAX - 1;
	}
	//======================================================================================

	//=======================================
	//�ݒ肷��
	//=======================================
	NewType = BGMODELTYPE(nNewType);
	//======================================================================================

	//=======================================
	//���S�t���O��ݒ肷��
	//=======================================
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CBgModel::Create(NewType, GetPos(), GetRot(), GetScale());//���������I�u�W�F�N�g��Ԃ�
}
//======================================================================================================================

//==================================================================
//����ۑ�����
//==================================================================
CObject* CBgModel::ManagerSaveObject()
{
	return CBgModel::Create(m_Type,GetPos(),GetRot(),GetScale());//���������I�u�W�F�N�g��Ԃ�
}
//======================================================================================================================