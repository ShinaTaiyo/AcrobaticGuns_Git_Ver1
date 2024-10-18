//====================================================
//
//�V���P�O���FC++�Ŋ�ՂÂ���[block.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BLOCK_H_  
#define _BLOCK_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
#include "objectX.h"
#include "object.h"
//==========================================

//===================================
//�}�b�v�̃e�L�X�g�t�@�C���̃}�N��
//===================================
#define MAP00_SENTRAL_TXT "data\\TEXTFILE\\Map\\Sentral.txt"

//==========================================

//==========================================
//�u���b�N�N���X
//==========================================
class CBlock : public CObjectX
{
public:
	//===========================
	//�u���b�N�̎��
	//===========================
    typedef enum
	{
		BLOCKTYPE00_NORMAL = 0,    //���ʃu���b�N
		BLOCKTYPE_MAX
	}BLOCKTYPE;
	//======================================================================

	//===========================
	//�}�b�v�̎��
	//===========================
	typedef enum
	{
		MAP00_SENTRAL = 0,
		MAP_MAX
	}MAPTYPE;
	//======================================================================

	CBlock();                 //�R���X�g���N�^
	~CBlock() override;       //�f�X�g���N�^
	HRESULT Init() override;  //����������
	void Uninit() override;   //�I������
	void Update() override;   //�X�V����
	void Draw() override;     //�`�揈��
	void SetDeath() override; //���S�t���O��ݒ�
	static CBlock * Create(BLOCKTYPE type, int nLife,D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 scale);//�u���b�N�𐶐�
	void ReleaseBlock();                //�u���b�N������
	BLOCKTYPE GetType();//�u���b�N�̎�ނ��擾����
	void Collision();                                                             //�����蔻����s��
	static void CollisionSquare(CObjectX * pObjX);                                //�����`�̓����蔻����s��
	static void LandingCorrection(D3DXVECTOR3& Pos,CObject * pSaveObj,D3DXVECTOR3 VtxMin);              //��Q�A��R�����Ŏw�肵���u���b�N�̏�ɏ���Ă���ꍇ�A�ʒu��␳����
	static int GetNumFile() { return m_nNumFile; }                                                                 //�t�@�C�������擾����
	BLOCKTYPE GetBlockType() { return m_type; }                                                                    //�u���b�N�̃^�C�v���擾����
	bool GetbCollision() { return m_bCollision; }                                  //����\���ǂ������擾����     
private:
	//======================================
	//�ÓI�����o
	//======================================
	static const int m_nMAX_FALLBLOCKRESPAWNCNT = 150;
	static const char* m_BLOCK_FILENAME[BLOCKTYPE_MAX];                            //�u���b�N�̃t�@�C���l�[��
	static const float m_fBLOCKCORRECTIONCOLLISION;                                //����ɗ]�T�����������
	static int m_nNumFile;                                                         //�t�@�C�������i�[����
	//======================================================================================================================

	//======================================
	//��{�n
	//======================================
	BLOCKTYPE m_type;                                                              //�u���b�N�̎��
	bool m_bCollision;                                                             //�����蔻������邩�ǂ���
	//======================================================================================================================

	//======================================
	//�u���b�N�Ƃ̉����o������p
	//======================================
	void ExtrusionCollisionX(CObjectX* pMyObjX,CBlock * pBlock);//X�����̉����o��������s��
	void ExtrusionCollisionY(CObjectX* pMyObjX, CBlock* pBlock);//Y�����̉����o��������s��
};

#endif
