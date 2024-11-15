//==========================================================
//
//�V���P�V���F�I�u�W�F�N�g�̃��X�g�Ǘ�������[stagemanager.cpp]
//Author:ShinaTaiyo
//
//==========================================================

//================================
//�C���N���[�h
//================================
#include "stagemanager.h"
#include "objectX.h"
#include "objectXInfo.h"
#include "block.h"
#include "input.h"
#include "manager.h"
#include "object.h"
#include "camera.h"
#include "calculation.h"
#include "debugproc.h"
#include "bgModel.h"
#include <stdio.h>
#include <string.h>
//==========================================================

//================================
//�ÓI�����o�錾
//================================
const char* CStageManager::m_apWORLDMAP_TXT[CStageManager::WORLDTYPE_MAX] =
{
	"data\\TEXTFILE\\Map\\EasyMap.txt",
	"data\\TEXTFILE\\Map\\NormalMap.txt",
	"data\\TEXTFILE\\Map\\BossRushMap.txt",
};

const string CStageManager::m_aSAVE_FILENAME = "data\\TEXTFILE\\Ver2\\Practice.txt";

//================================
//�R���X�g���N�^
//================================
CStageManager::CStageManager(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri,bUseintPri,type,ObjType),
m_nWorldIndex(0),m_pBg3D(nullptr),m_VecObjList(),m_SaveScale(D3DXVECTOR3(1.0f,1.0f,1.0f)),m_SavePos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_SaveRot(D3DXVECTOR3(0.0f,0.0f,0.0f))
{
	for (int nCnt = 0; nCnt < m_nMAX_MAP; nCnt++)
	{
		for (int nCntWord = 0; nCntWord < m_nMAX_WORD; nCntWord++)
		{
			m_aMapFilePass[nCnt][nCntWord] = {};
		}
	}
}
//==========================================================

//================================
//�f�X�g���N�^
//================================
CStageManager::~CStageManager()
{
	m_VecObjList.clear();//vector�̒��g���N���A�i�Y�ꂽ�Ƃ��΍�j
}
//==========================================================

//================================
//����������
//================================
HRESULT CStageManager::Init()
{
	//===========================
	//�����o�ϐ�
	//===========================
	m_nMapIndex = 0;                                     //�}�b�v�̃C���f�b�N�X
	m_nMapNum = 0;                                       //�}�b�v�̑���
	m_SaveScale = D3DXVECTOR3(1.0f,1.0f,1.0f);                               //�g�嗦
	m_SaveRot = D3DXVECTOR3(0.0f,0.0f,0.0f);                                //����
	m_SavePos = D3DXVECTOR3(0.0f,0.0f,0.0f);                                //�ʒu
	m_SaveBeforeChoosePos = D3DXVECTOR3(0.0f,0.0f,0.0f);                //�I������������O�̈ʒu���L������
 	m_pManagerObject = nullptr;                          //�}�l�[�W���[�ɕ\������I�u�W�F�N�g
	m_ManagerMode = MANAGERMODE_ALREADYSTAGE;            //���݂̃X�e�[�W�}�l�[�V���[�̃��[�h

	//=========================
	//�I���n
	//=========================
	m_pChooseObject = nullptr;                          //�I���I�u�W�F�N�g�ւ�
	m_nIndexChooseObject = 0;                           //�I�����Ă���I�u�W�F�N�g�̔ԍ�
	m_bChooseObject = false;                            //�I�u�W�F�N�g��I�����Ă��邩�ǂ���
	m_bMakeMapMode = false;                             //�}�b�v���샂�[�h���ǂ���
	//=======================================================================================
#ifdef _DEBUG
#endif // _DEBUG

	//===========================
	//�ړ����[�h�����߂�
	//===========================
	m_MoveMode = MOVEMODE00_XY;     //�ړ����@�̎��
	m_FocusType = FOCUSTYPE_NORMAL;//�J�������ǂ��ʒu�̎��
	m_bUseSizeMove = false;//���݂̃I�u�W�F�N�g�̃T�C�Y���ړ����邩�ǂ���
	//=======================================================================================

	//===========================
	//�I�u�W�F�N�g���X�g
	//===========================
	//m_VecObjList = {};//�I�u�W�F�N�g��Vector�^�̃��X�g
	//=======================================================================================

	//===========================
	//�I�u�W�F�N�gX����������
	//===========================
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE00_NORMAL, 1, D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(1.0f,1.0f,1.0f));
		m_pManagerObject->SetUseDeath(false);//���S�t���O���I�t�ɂ���
	}
	//=======================================================================================
	CObject::Init();

	LoadMapTxt(0);
	return S_OK;
}
//==========================================================

//================================
//�I������
//================================
void CStageManager::Uninit()
{

}
//==========================================================

//================================
//�X�V����
//================================
void CStageManager::Update()
{

#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		if (m_bChooseObject == false)
		{

			//�X�e�[�W�}�l�[�W���[������ɃA�N�Z�X���đ��삷��
			m_pManagerObject->ManagerChooseControlInfo();

			//���݂̃u���b�N�̎�ނ�ύX����
			TypeChenge();

			//�u���b�N��ݒ肷��
			SetObjectX();

			//�}�b�v��ς��鏈��
			MapChenge();

			//�����̃I�u�W�F�N�g���������鏈��
			DeleteManagerObject();

			//===========================================================================================
		}

		//===============================================
		//�I�u�W�F�N�g�I������
		//===============================================
		//ChooseObject();
		//===========================================================================================

		//===============================================
		//���\������
		//===============================================
		DispInfo();
		//===========================================================================================

		//===============================================
		//�I�u�W�F�N�g�X�V
		//===============================================
		CObject::Update();
		//===========================================================================================
	}

#endif // _DEBUG
}
//==========================================================

//================================
//�`�揈��
//================================
void CStageManager::Draw()
{
#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
        //�I�u�W�F�N�g�`��
	    CObject::Draw();
	}
#endif // _DEBUG

}
//============================================================================================================

//======================================================
//���S�t���O��ݒ�
//======================================================
void CStageManager::SetDeath()
{
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);
		m_pManagerObject->SetDeath();
		m_pManagerObject = nullptr;
	}

	m_VecObjList.clear();//vector�̒��g���N���A

	CObject::SetDeath();
}
//============================================================================================================

//============================================
//�}�b�v�����[�h���鏈��
//============================================
void CStageManager::LoadMapTxt(int nMapNum)
{

	//vector�ɕۑ������������Z�b�g����
	m_VecObjList.clear();

	fstream ReadingFile;//�ǂݎ��p�t�@�C��
	string Reading_Buff;//�ǂݎ��p�t�@�C���̕�����

	ReadingFile.open(m_aSAVE_FILENAME, ios::in);//�ǂݎ�胂�[�h�ŊJ��

	while (!ReadingFile.eof())
	{
		ReadingFile >> Reading_Buff;

		if (Reading_Buff == "SETBLOCK")
		{
			CBlock::LoadInfoTxt(ReadingFile, m_VecObjList,Reading_Buff);
		}
		else if (Reading_Buff == "SETBGMODEL")
		{
			CBgModel::LoadInfoTxt(ReadingFile, m_VecObjList, Reading_Buff);
		}
	}

	ReadingFile.close();//�t�@�C�������
}
//============================================================================================================

//===========================================
//�}�b�v�̃t�@�C���p�X�����[�h����
//===========================================
void CStageManager::LoadMapFilePass(WORLDTYPE type)
{
	//char aString[512] = {};
	////===============================================================================================

	//m_nMapNum = 0;//�}�b�v������������

	//FILE* pFileTxt = nullptr;
	//pFileTxt = fopen(m_apWORLDMAP_TXT[type], "r");

	//if (pFileTxt != nullptr)
	//{
	//	while (1)
	//	{
	//		fscanf(pFileTxt, "%s", &aString[0]);
	//		if (aString[0] == '#')
	//		{
	//			fgets(&aString[0], 100, pFileTxt);//�s���X�L�b�v
	//		}
	//		else if (strcmp(&aString[0], "FILENAME") == 0)
	//		{
	//			fscanf(pFileTxt, "%s", &aString[0]);//�C�R�[���ǂݎ��p
	//			fscanf(pFileTxt, "%s", &m_aMapFilePass[m_nMapNum][0]);//�t�@�C�����ǂݎ��p
	//			m_nMapNum++;
	//		}
	//		else if (strcmp(&aString[0], "END_SCRIPT") == 0)
	//		{
	//			fclose(pFileTxt);
	//			break;
	//		}
	//	}

	//}

}
//============================================================================================================

//============================================
//�}�b�v���Z�[�u���鏈��
//============================================
void CStageManager::SaveMapTxt(int nMapNum)
{

	fstream WritingFile;    //�t�@�C��
	string Writing_Buff;    //������
	CObject::TYPE Type = {};//�I�u�W�F�N�g���

	WritingFile.open(m_aSAVE_FILENAME, ios::out);//�ǂݎ�胂�[�h�Ńt�@�C�����J��	

	//�t�@�C���ɏ���ۑ�����
	for (vector<CObject*>::iterator it = m_VecObjList.begin(); it != m_VecObjList.end(); it++)
	{//�����܂ŌJ��Ԃ�
		if (&it != nullptr)
		{
			Type = static_cast<CObject*>(*it)->GetType();
			static_cast<CObject*>(*it)->SaveInfoTxt(WritingFile);

			WritingFile << endl << endl;//���s����
		}
	}

	WritingFile.close();//�t�@�C�������

	m_VecObjList.clear();//Vector�̒��g���N���A����
	ReleaseAll();        //�S�Ă̎��S�t���O�𔭓�
}
//======================================================================================================================

//==========================================================
//�}�b�v���o�C�i���t�@�C������ǂݍ���
//==========================================================
void CStageManager::LoadMapBin(int nMapNum)
{
	////======================================
	////�ϐ��錾
	////======================================
	//D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//int nType = 0;            //�I�u�W�F�N�gX���Ƃ̃^�C�v
	//int nManagerType = 0;        //�I�u�W�F�N�gX�̃^�C�v
	//int nRotType = 0;         //�����̃^�C�v
	//D3DXVECTOR3 move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//float fWidth = 0.0f;
	//float fHeight = 0.0f;
	//int nData = 0;
	//char aString[512] = {};
	//char aStringMap[100] = {};
	//CObject* pObject = nullptr;
	////===============================================================================================
	//StageManagerObjectReleaseAll();
	//FILE* pFile = nullptr;
	//pFile = fopen(&m_aMapFilePass[nMapNum][0], "rb");
	////pFile = fopen(&m_aMapFilePass[0][0], "rb");
	//if (pFile != nullptr)
	//{
	//	//==============================================
	//	//�I�u�W�F�N�g�̏���ǂݍ���
	//	//==============================================
	//	fread(&m_nIndexObject, sizeof(int), 1, pFile);
	//	if (m_nIndexObject > 0)
	//	{
	//		for (int nCnt = 0; nCnt < m_nIndexObject; nCnt++)
	//		{//�I�u�W�F�N�g��ݒ�
	//			fread(&nManagerType, sizeof(int), 1, pFile);         //X�I�u�W�F�N�g�̃^�C�v��ǂݍ���
	//			fread(&nType, sizeof(int), 1, pFile);             //X�I�u�W�F�N�g���Ƃ̃^�C�v��ǂݍ���
	//			fread(&nRotType, sizeof(int), 1, pFile);          //�����̎�ނ�ǂݍ���
	//			fread(&Pos, sizeof(D3DXVECTOR3), 1, pFile);       //�ʒu��ǂݍ���
	//			fread(&Scale, sizeof(D3DXVECTOR3), 1, pFile);     //�g�嗦��ǂݍ���
	//			fread(&Rot, sizeof(D3DXVECTOR3), 1, pFile);       //������ǂݍ���

	//			//switch (nManagerType)
	//			//{
	//			//	case (int)(MANAGEROBJECT_BLOCK) :
	//			//		pObject = CBlock::Create(CBlock::BLOCKTYPE(nType), 10, Pos, Rot, Scale, (CObjectX::ROTTYPE)(nRotType), m_nSetSubType);//��������
	//			//		break;
	//			//		case (int)(MANAGEROBJECT_MODEL) :
	//			//			pObject = CModel::Create((CModel::MODELTYPE)(nType), Pos, Rot, D3DXVECTOR3(0.0f,0.0f,0.0f), Scale, (CObjectX::ROTTYPE)(nRotType));//��������
	//			//			break;
	//			//				case (int)(MANAGEROBJECT_ENEMY) :
	//			//					pObject = CEnemy::Create((CEnemy::ENEMYTYPE)(nType), 10, Pos, D3DXVECTOR3(0.0f,0.0f,0.0f), Scale,D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f));//��������
	//			//					break;
	//			//					case (int)(MANAGEROBJECT_ITEM) :
	//			//						pObject = CItem::Create((CItem::ITEMTYPE)(nType), Pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL);//��������
	//			//						break;
	//			//						case (int)(MANAGEROBJECT_MARKER) :
	//			//							pObject = CMarker::Create((CMarker::MARKERTYPE)(nType),Pos,Scale, D3DXVECTOR3(0.0f,0.0f,0.0f), 0);
	//			//							break;
	//			//							case (int)(MANAGEROBJECT_BOSS) :
	//			//								pObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(nType),10, Pos, Scale);
	//			//								break;


	//			//				default:
	//			//					break;
	//			//}

	//			//if (pObject != nullptr)
	//			//{
	//			//	m_apObjectList[nCnt] = pObject;
	//			//}
	//		}
	//	}
	//	//========================================================================
	//	fclose(pFile);
	//}
}
	//======================================================================================================================

//==========================================================
//�}�b�v���o�C�i���t�@�C���ɏ����o��
//==========================================================
void CStageManager::SaveMapBin()
{
	////======================================
	////�ϐ��錾
	////======================================
	//D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//int nType = 0;             //�I�u�W�F�N�gX���Ƃ̃^�C�v
	//int nManagerType = 0;             //�I�u�W�F�N�gX�̃^�C�v
	//D3DXVECTOR3 move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//float fWidth = 0.0f;
	//float fHeight = 0.0f;
	//FILE* pFile = fopen(SAVE_BIN, "wb");
	////===============================================================================================
	//if (pFile != nullptr)
	//{

	//	fwrite(&m_nIndexObject, sizeof(int),1, pFile);
	//	//==============================================
 //       //�u���b�N��X�I�u�W�F�N�g�̏��������o��
 //       //==============================================
	//	for (int nCnt = 0; nCnt < m_nIndexObject; nCnt++)
	//	{//�u���b�N��ݒ�
	//		if (m_apObjectList[nCnt] != nullptr)
	//		{
	//			nManagerType = ((CObjectX*)m_apObjectList[nCnt])->GetManagerType();  //X�I�u�W�F�N�g�̃^�C�v���擾
	//			nType = (int)(((CObjectX*)m_apObjectList[nCnt])->GetTypeNum());       //X�I�u�W�F�N�g���Ƃ̃^�C�v���擾
	//			Pos = ((CObjectX*)m_apObjectList[nCnt])->GetPos();                    //�ʒu�������o��
	//			Scale = ((CObjectX*)m_apObjectList[nCnt])->GetScale();				//�g�嗦�������o��
	//			Rot = ((CObjectX*)m_apObjectList[nCnt])->GetRot();					//�����������o��

	//			fwrite(&nManagerType, sizeof(int), 1, pFile);              //X�I�u�W�F�N�g�̃^�C�v�������o��
	//			fwrite(&nType, sizeof(int), 1, pFile);                  //X�I�u�W�F�N�g���Ƃ̃^�C�v�������o��
	//			fwrite(&Pos, sizeof(D3DXVECTOR3), 1, pFile);            //�ʒu�������o��
	//			fwrite(&Scale, sizeof(D3DXVECTOR3), 1, pFile);          //�g�嗦�������o��
	//			fwrite(&Rot, sizeof(D3DXVECTOR3), 1, pFile);            //�����������o��
	//		}
	//	}
	//	//========================================================================
	//	fclose(pFile);
	//}
}
//======================================================================================================================

//==============================================
//�X�e�[�W�}�l�[�W���[��������
//==============================================
CStageManager* CStageManager::Create()
{
	CStageManager* pStageManager = DBG_NEW CStageManager;                                                                        //�v���C���[�𐶐�

	bool bSuccess = pStageManager->CObject::GetCreateSuccess();
	if (bSuccess == true)
	{//�󂫂��������̂Ő����J�n
		if (pStageManager != nullptr)
		{
			pStageManager->Init();                                                                                           //����������
			pStageManager->SetUseDeath(false);                                                                        //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pStageManager->CObject::SetType(CObject::TYPE::STAGEMANAGER);                                                     //�I�u�W�F�N�g�̎�ނ����߂�   
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pStageManager;
		pStageManager = nullptr;
	}

	return pStageManager;
}
//================================================================================

//=================================================
//�I���I�u�W�F�N�g�̎�ނ�ς���
//=================================================
void CStageManager::TypeChenge()
{
	bool bChengeFlag = false;
	int nNumType = m_pManagerObject->GetManagerObjectType();

	CObject::MANAGEROBJECTTYPE ManagerObjectType = CObject::MANAGEROBJECTTYPE_NONE;
	//=====================================================================
    //�I�u�W�F�N�gX�̎�ނ�ύX����
    //=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{
		nNumType++;
		bChengeFlag = true;//����
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{
		nNumType--;
		bChengeFlag = true;//����
	}

	if (bChengeFlag == true)
	{
		if (nNumType >= CObject::MANAGEROBJECTTYPE_MAX)
		{
			nNumType = CObject::MANAGEROBJECTTYPE_NONE + 1;
		}
		if (nNumType < 0)
		{
			nNumType = CObject::MANAGEROBJECTTYPE_MAX - 1;
		}

		ManagerObjectType = CObject::MANAGEROBJECTTYPE(nNumType);

		ChengeObject(ManagerObjectType);//�I�u�W�F�N�g�̎�ނ�ς���

	}
	//=====================================================================================================================================

	//=====================================================================
	//�I�u�W�F�N�g���Ƃ̎�ނ�ύX����
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{
		m_pManagerObject = m_pManagerObject->ManagerChengeObject(true);
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
	{
		m_pManagerObject = m_pManagerObject->ManagerChengeObject(false);
	}
	//=====================================================================================================================================
}
//===========================================================================================

//==================================================
//X�I�u�W�F�N�g�ݒ菈��
//==================================================
void CStageManager::SetObjectX()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{//�I�u�W�F�N�g��Vector�̐擪�ɕۑ�����
		m_VecObjList.push_back(m_pManagerObject->ManagerSaveObject());
	}

}
//===========================================================================================

//==================================================
//X�I�u�W�F�N�g��������
//==================================================
void CStageManager::DeleteManagerObject()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true)
	{
		if (m_VecObjList.size() > 0)
		{
			auto it = m_VecObjList.end() - 1;//�z��}�b�N�X�[�P

			((CObject*)*it)->SetUseDeath(true);
			((CObject*)*it)->SetDeath();

			m_VecObjList.pop_back();//�����̗v�f���폜����
		}
	}
}
//=======================================================================================================================

//====================================================
//�ݒu�I�u�W�F�N�g�̎�ނ�ς���
//====================================================
void CStageManager::ChengeObject(CObject::MANAGEROBJECTTYPE ManagerObjectType)
{	
	//�I�u�W�F�N�g��j������
	ReleaseObject();

	int nObjectType = ManagerObjectType;

	if (nObjectType >= CObject::MANAGEROBJECTTYPE_MAX)
	{
		nObjectType = CObject::MANAGEROBJECTTYPE_NONE + 1;
	}
	else if (nObjectType < CObject::MANAGEROBJECTTYPE_NONE + 1)
	{
		nObjectType = CObject::MANAGEROBJECTTYPE_MAX - 1;
	}

	ManagerObjectType = MANAGEROBJECTTYPE(nObjectType);

	//�I�u�W�F�N�g�̎�ނ�ς���
	switch (ManagerObjectType)
	{
	case  CObject::MANAGEROBJECTTYPE_BLOCK:
		m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE00_NORMAL, 10, m_SavePos,m_SaveRot,m_SaveScale);
		break;
	case CObject::MANAGEROBJECTTYPE_BGMODEL:
		m_pManagerObject = CBgModel::Create(CBgModel::BGMODELTYPE::BILL_00, m_SavePos, m_SaveRot, m_SaveScale);
		break;
		default:
		break;
	}
	if (m_pManagerObject != nullptr)
	{//���I��ł���I�u�W�F�N�g�����͔j������Ȃ�
		m_pManagerObject->SetUseDeath(false);
	}
}
//=======================================================================================================================

//==================================================================
//�I�u�W�F�N�g�������[�X����
//==================================================================
void CStageManager::ReleaseObject()
{
	//�Œ���̏���ۑ�����
	switch (m_pManagerObject->GetObjectType())
	{
	case CObject::OBJECTTYPE::OBJECTTYPE_X:
		m_SavePos = ((CObjectX*)m_pManagerObject)->GetPos();
		m_SaveRot = ((CObjectX*)m_pManagerObject)->GetRot();
		m_SaveScale = ((CObjectX*)m_pManagerObject)->GetScale();
		break;
	default:
		break;
	}
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);
		m_pManagerObject->SetDeath();
		m_pManagerObject = nullptr;
	}

}
//=======================================================================================================================

//===========================================================
//�}�b�v��ς���
//===========================================================
void CStageManager::MapChenge()
{
	//===============================================================
	//���[�h��ς���
	//===============================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F4) == true)
	{
		SaveMapTxt(m_nMapIndex);//���݂̃}�b�v��ۑ�����

		if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
		{
			m_ManagerMode = MANAGERMODE_NEWSTAGE;
		}
		else
		{
			m_ManagerMode = MANAGERMODE_ALREADYSTAGE;
		}


		LoadMapTxt(m_nMapIndex);
	}
	//=======================================================================================================

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3) == true && m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//���݂̃}�b�v�����Z�[�u����
		m_nMapIndex++;
		if (m_nMapIndex >= m_nMapNum)
		{
			m_nMapIndex = 0;
		}

		LoadMapTxt(m_nMapIndex);//�ԍ��̃}�b�v��ǂݍ���
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_F2) == true && m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//���݂̃}�b�v�����Z�[�u����
		m_nMapIndex--;
		if (m_nMapIndex < 0)
		{
			m_nMapIndex = m_nMapNum - 1;
		}
		LoadMapTxt(m_nMapIndex);//�ԍ��̃}�b�v��ǂݍ���
	}
}
//=======================================================================================================================

//===========================================================
//���\������
//===========================================================
void CStageManager::DispInfo()
{
	char aChooseString[32] = {};//�I����Ԃ��ǂ������L�q
	char aCameraTypeName[CCamera::CAMERATYPE_MAX][100] = {};
	char aMapModeString[32] = {};
	//�}�b�v�̃J�������[�h��
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BIRD][0], "�o�[�h�r���[");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BESIDE][0], "�r�T�C�h�r���[");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_SIDE][0], "�T�C�h�r���[");


	if (m_bChooseObject == false)
	{
		strcpy(&aChooseString[0], "�I�����Ă��Ȃ�");
	}
	else
	{
		strcpy(&aChooseString[0], "�I�𒆁E�E�E");
	}

	if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		strcpy(&aMapModeString[0], "�����̃X�e�[�W��ҏW");
	}
	else
	{
		strcpy(&aMapModeString[0], "�V�����X�e�[�W��ҏW");
	}

	CManager::GetDebugProc()->PrintDebugProc("//=================================\n");
	CManager::GetDebugProc()->PrintDebugProc("//�}�b�v�G�f�B�^�̏��\n");
	CManager::GetDebugProc()->PrintDebugProc("//=================================\n");
	CManager::GetDebugProc()->PrintDebugProc("���݂̃X�e�[�W�}�l�[�W���[�Ǘ��I�u�W�F�N�g�̐��F%d\n", m_VecObjList.size());
	CManager::GetDebugProc()->PrintDebugProc("���݂̃��[���h�F%s\n",&m_apWORLDMAP_TXT[m_nWorldIndex][0]);
	CManager::GetDebugProc()->PrintDebugProc("���݂̃}�b�v�ԍ�(F2�AF3�ŕύX�j�F%d\n", m_nMapIndex);
	CManager::GetDebugProc()->PrintDebugProc("���݂̃}�b�v�t�@�C���p�X�F%s\n",&m_aMapFilePass[m_nMapIndex][0]);
	CManager::GetDebugProc()->PrintDebugProc("���݂̃}�b�v���[�h�iF4�j�F%s\n",&aMapModeString[0]);
	CManager::GetDebugProc()->PrintDebugProc("�F�����̃I�u�W�F�N�g�������FBACKSPACE\n");
	CManager::GetDebugProc()->PrintDebugProc("//=================================\n");

}
//=======================================================================================================================

//===========================================================
//�I�u�W�F�N�g�I������
//===========================================================
void CStageManager::ChooseObject()
{
	//if (GetTotalStageManagerObjNum() <= 0)
	//{//�X�e�[�W�}�l�[�W���[�Ǘ��I�u�W�F�N�g���Ȃ��Ȃ����u�ԂɑI����Ԃ���������B
	//	m_bChooseObject = false;
	//	m_pChooseObject = nullptr;
	//}

	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_L) == true && GetTotalStageManagerObjNum() > 0)
	//{
	//	m_bChooseObject = m_bChooseObject ? false : true;

	//	if (m_bChooseObject == false)
	//	{
	//		m_pChooseObject->SetStageManagerChoose(false);     //�I�����Ă���I�u�W�F�N�g�̑I����Ԃ�����
	//		m_Pos = m_SaveBeforeChoosePos;                     //�ۑ����Ă����ʒu�ɖ߂�
	//	}
	//	else
	//	{
	//		m_SaveBeforeChoosePos = m_Pos;                     //�I����Ԃ𔭓�����O�̃I�u�W�F�N�g�̈ʒu���L������
	//		m_pChooseObject = GetCurStageManagerObject();      //�ŐV�̃I�u�W�F�N�g��I��
	//		m_pChooseObject->SetStageManagerChoose(false);     //�I�����Ă���I�u�W�F�N�g�̑I����Ԃ�����
	//	}
	//}
	//
	//if (m_pChooseObject != nullptr)
	//{//�I���I�u�W�F�N�g�����݂��Ă�����
	//	if (m_bChooseObject == true)
	//	{
	//		//�I���I�u�W�F�N�g�ύX
	//		if (CManager::GetInputKeyboard()->GetTrigger(DIK_N) == true)
	//		{
	//			m_pChooseObject->SetStageManagerChoose(false);//���ݑI��ł���I�u�W�F�N�g�̑I����Ԃ�����

	//			if (m_pChooseObject->GetPrevStageManagerObject() != nullptr)
	//			{//�O�̃I�u�W�F�N�g����������
	//				m_pChooseObject = m_pChooseObject->GetPrevStageManagerObject();//�O�̃I�u�W�F�N�g��I��
	//			}
	//			else
	//			{//�O�̃I�u�W�F�N�g���Ȃ�
	//				m_pChooseObject = GetCurStageManagerObject();//�ŐV�I�u�W�F�N�g��I��
	//			}
	//		}
	//		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_M) == true)
	//		{
	//			m_pChooseObject->SetStageManagerChoose(false);//���ݑI��ł���I�u�W�F�N�g�̑I����Ԃ�����

	//			if (m_pChooseObject->GetNextStageManagerObject() != nullptr)
	//			{//���̃I�u�W�F�N�g������
	//				m_pChooseObject = m_pChooseObject->GetNextStageManagerObject();//���̃I�u�W�F�N�g��I��
	//			}
	//			else
	//			{//���̃I�u�W�F�N�g���Ȃ�
	//				m_pChooseObject = GetTopStageManagerObject();//�g�b�v�I�u�W�F�N�g��I��
	//			}
	//		}

	//		if (m_pChooseObject != nullptr && m_nIndexObject >= 1)
	//		{//�I�u�W�F�N�g��I����Ԃɂ���
	//			m_Pos = ((CObjectX*)m_pChooseObject)->GetPos();//�I���I�u�W�F�N�g�̈ʒu�Ɉړ�����
	//			m_pChooseObject->SetStageManagerChoose(true);
	//		}

	//		if (CManager::GetInputKeyboard()->GetTrigger(DIK_DELETE) == true)
	//		{//�I���I�u�W�F�N�g��j������
	//			m_pChooseObject->SetUseDeath(true);
	//			m_pChooseObject->SetDeath();

	//			//�I���I�u�W�F�N�g��ύX
	//			if (m_pChooseObject->GetNextStageManagerObject() != nullptr)
	//			{//���̃I�u�W�F�N�g������
	//				m_pChooseObject = m_pChooseObject->GetNextStageManagerObject();//���̃I�u�W�F�N�g��I��
	//			}
	//			else
	//			{//���̃I�u�W�F�N�g���Ȃ�
	//				m_pChooseObject = GetTopStageManagerObject();//�g�b�v�I�u�W�F�N�g��I��
	//			}
	//		}
	//	}
	//}


}
//=======================================================================================================================

//===========================================================
//�T�C�Y���������Ƃ��̏���
//===========================================================
D3DXVECTOR3 CStageManager::SizeMoveProcess(float fMoveX, float fMoveY,D3DXVECTOR3 Size)
{
	D3DXVECTOR3 ResultSize = D3DXVECTOR3(0.0f,0.0f,0.0f);

	ResultSize.x = float(floor(Size.x));
	ResultSize.y = float(floor(Size.y));
	ResultSize.z = float(floor(Size.z));

	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_DOWN) == true ||
		CManager::GetInputKeyboard()->GetTrigger(DIK_UP) == true ||
		CManager::GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true ||
		CManager::GetInputKeyboard()->GetTrigger(DIK_LEFT) == true)
	{
		if (m_MoveMode == MOVEMODE00_XY)
		{
			if (fMoveX != 0.0f)
			{
				Move.x = sinf(atan2f(fMoveX, 0.0f)) * ResultSize.x;
			}
			else if (fMoveY != 0.0f)
			{
				Move.y = cosf(atan2f(0.0f, fMoveY)) * ResultSize.x;
			}
		}
	}


	return Move;
}
//=======================================================================================================================

//===========================================================
//�g�嗦�����Z�b�g���鏈��
//===========================================================
void CStageManager::ResetScale()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_V) == true)
	{
		m_SaveScale = D3DXVECTOR3(1.0f,1.0f,1.0f);
	}
}
//=======================================================================================================================
