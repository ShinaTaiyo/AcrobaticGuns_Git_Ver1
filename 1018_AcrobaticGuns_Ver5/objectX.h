//=======================================================================================
//
//�U���S���F���f����`�悷��[objectX.h]
//Author:ShinaTaiyo
//
//=======================================================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//==============================================
//�C���N���[�h
//==============================================
#include "main.h"
#include "object.h"
//=======================================================================================

//=======================================================
//�I�u�W�F�N�gX�N���X
//=======================================================
class CObjectX : public CObject
{
public:
	static const int m_nMAX_MAT = 100;                                                      //�}�e���A���̍ő吔
	typedef enum
	{
		OBJECTXTYPE_BLOCK = 0,        //�u���b�N
		OBJECTXTYPE_MODEL,            //���f��
		OBJECTXTYPE_PLAYER,           //�v���C���[
		OBJECTXTYPE_ENEMY,            //�G
		OBJECTXTYPE_ITEM,             //�A�C�e��
		OBJECTXTYPE_MARKER,           //�}�[�J�[
		OBJECTXTYPE_ATTACK,           //�U��
		OBJECTXTYPE_BOSS,             //�{�X
		OBJECTXTYPE_TRAP,             //�g���b�v
		OBJECTXTYPE_SIGNBOARD,        //���ĊŔ�
		OBJECTXTYPE_AIMODEL,          //AI���f��
		OBJECTXTYPE_MAX
	}OBJECTXTYPE;

	typedef enum
	{
		ORIGINPOSTYPE_SENTER = 0,
		ORIGINPOSTYPE_DOWN,
	    ORIGINPOSTYPE_MAX
	}ORIGINPOSTYPE;

	typedef enum
	{
		COLORTYPE_RED = 0,
		COLORTYPE_GREEN,
		COLORTYPE_BRUE,
		COLORTYPE_ALPHA,
		COLORTYPE_MAX
	}COLORTYPE;

	//=================================
    //���f�����\����
    //=================================
	typedef struct
	{
		LPD3DXMESH pMesh;                           //���b�V���i���_���j�ւ̃|�C���^�i���f���̎�ސ����j
		LPD3DXBUFFER pBuffMat;                      //�}�e���A���ւ̃|�C���^
		DWORD dwNumMat;                             //�}�e���A���̐�
		LPDIRECT3DTEXTURE9* pTexture;               //�e�N�X�`���ւ̃|�C���^
		D3DCOLORVALUE* Diffuse;          //�F����
		D3DCOLORVALUE* FormarDiffuse;    //���̐F����
	}ObjectXInfo;
	//============================================================================================================

	CObjectX(int nPri = 0, bool bUseintPri = false,CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                                                           //�R���X�g���N�^
	~CObjectX() override;                                                 //�f�X�g���N�^
	HRESULT Init() override;                                              //����������
	void Uninit() override;                                               //�I������
	void Update() override;                                               //�X�V����
	void Draw() override;                                                 //�`�揈��
	void BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture,D3DCOLORVALUE * pDiffuse);//���f���������蓖�Ă�

	//==========================================================
	//�ÓI�����o�錾
	//==========================================================
	static void SetCommonDraw(bool bDraw) { s_bCOMMON_DRAWSHADOW = bDraw; }

	//==========================================================
	//�ʒu
	//==========================================================
	
	//�ʒu
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }                                           //�ʒu�̐ݒ�
	const D3DXVECTOR3& GetPos() const { return m_Pos; }                                     //�ʒu�̎擾

	//1f�O�̈ʒu
	void SetPosOld(D3DXVECTOR3 PosOld) { m_PosOld = PosOld; }                               //1f�O�̈ʒu�̐ݒ�
	const D3DXVECTOR3& GetPosOld() const { return m_PosOld; }                                            //1f�O�̈ʒu��ݒ�

	//�����ʒu
	void SetSupportPos(D3DXVECTOR3 SupportPos) { m_SupportPos = SupportPos; }               //�ݒu�ʒu��ݒ肷��
	D3DXVECTOR3 & GetSupportPos() { return m_SupportPos; }                                  //�ݒu�ʒu���擾����

	//���S�_�̈ʒu
	D3DXVECTOR3& GetSenterPos() { return m_SenterPos; }                                     //���S�_���擾����
	//============================================================================================================
	
	//==========================================================
	//����
	//==========================================================
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }                                           //�����̐ݒ�
	D3DXVECTOR3 &GetRot() { return m_Rot; }                                                 //�����̎擾
	void SetUseAddRot(bool bUse, D3DXVECTOR3 Rot) { m_bUseAddRot = bUse; m_AddRot = Rot;}
	bool& GetUseAddRot() { return m_bUseAddRot; }//�����̉��Z���g�p���Ă��邩�ǂ���
	D3DXVECTOR3& GetAddRot() { return m_AddRot; }//�����̉��Z�ʂ��擾

	//�N�H�[�^�j�I��
	void SetAxis(float fAxis) { m_fAxis = fAxis; }//��]�p��ݒ�
	const float& GetAxis() const { return m_fAxis; }//��]�p���擾 
	void SetVecAxis(D3DXVECTOR3 VecAxis) { m_VecAxis = VecAxis; }//��]����ݒ�
	const D3DXVECTOR3& GetVecAxis() const { return m_VecAxis; }  //��]�����擾

	//============================================================================================================

	//==========================================================
	//�g�嗦
	//==========================================================
	
	//�g�嗦
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }                                   //�g�嗦�̐ݒ�
	D3DXVECTOR3 & GetScale() { return m_Scale; }                                            //�g�嗦�̎擾

	//���̊g�嗦
	void SetFormarScale(D3DXVECTOR3 Scale) { m_FormarScale = Scale; }                       //���̊g�嗦��ݒ�
	D3DXVECTOR3& GetFormarScale() { return m_FormarScale; }                                 //���̊g�嗦���擾

	//���Z�g�嗦
	void SetUseAddScale(D3DXVECTOR3 AddScale, bool bUse);              //�g�嗦�̉��Z���g�p���邩�ǂ���

	//��Z�g�嗦
	void SetUseMultiScale(bool bUse, D3DXVECTOR3 Scale) { m_bUseMultiScale = bUse; m_MultiScale = Scale; }
	//============================================================================================================

	//==========================================================
	//�T�C�Y
	//==========================================================

	//�ő咸�_
	D3DXVECTOR3 & GetVtxMax() { return m_VtxMax; }                                          //�e���_�ő�l�̎擾
	void SetVtxMax(D3DXVECTOR3 VtxMax) { m_VtxMax = VtxMax; }
	D3DXVECTOR3 & GetOriginVtxMax() { return m_OriginVtxMax; }                              //���̊e���_�ő�l�̎擾
	void SetOriginVtxMax(D3DXVECTOR3 VtxMax) { m_OriginVtxMax = VtxMax; }                   //���̍ő咸�_�̐ݒ�

	//�ŏ����_
	D3DXVECTOR3 & GetVtxMin() { return m_VtxMin; }                                          //�e���_�ŏ��l�̎擾
	void SetVtxMin(D3DXVECTOR3 VtxMin) { m_VtxMin = VtxMin; }
	D3DXVECTOR3 & GetOriginVtxMin() { return m_OriginVtxMin; }                              //���̊e���_�ŏ��l�̎擾
	void SetOriginVtxMin(D3DXVECTOR3 VtxMin) { m_OriginVtxMin = VtxMin; }                   //���̍ŏ����_�̐ݒ�

	//�T�C�Y
	void SetSize();                                                                         //�T�C�Y��ݒ肷�� 
	D3DXVECTOR3 GetSize() { return m_Size; }                                                //X�I�u�W�F�N�g�̃T�C�Y���擾

    //�ő咸�_�ƍŏ����_���X���b�v
	void ActiveSwapVtxMaxMin()
	{
		if (m_bSwapVtxXZ == true)
		{
			swap(m_OriginVtxMax.x, m_OriginVtxMax.z);
			swap(m_OriginVtxMin.x, m_OriginVtxMin.z);
		}
	}
	void SetUseSwapVtxXZ(bool bUse) { m_bSwapVtxXZ = bUse; }
	const bool& GetUseSwapVtxXZ()const{ return m_bSwapVtxXZ; }
	//============================================================================================================

	//==========================================================
	//�G�f�B�^
	//==========================================================

	//�I�u�W�F�N�gX�̎��
	void SetObjXType(OBJECTXTYPE ObjXType) { m_nObjXType = ObjXType; }                      //�I�u�W�F�N�gX�̕��ނ�ݒ�
	OBJECTXTYPE GetObjXType() { return m_nObjXType; }                                       //�I�u�W�F�N�gX�̃^�C�v���擾����

	//�^�C�v�ԍ�
	void SetTypeNum(int nTypeNum) { m_nTypeNum = nTypeNum; }                                //�I�u�W�F�N�g���Ƃ̃^�C�v�ԍ���ݒ�
	int GetTypeNum() { return m_nTypeNum;}                                                  //�I�u�W�F�N�g���Ƃ̃^�C�v�ԍ���ݒ�

	//���\������������
	void ManagerChooseControlInfo() override;                                               //�X�e�[�W�}�l�[�W���[�����𑀍삷��

	void SaveInfoTxt(fstream& WritingFile) override;//�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�

	//================================================================================================================================================

	//==========================================================
	//���[���h�ϊ��s��
	//==========================================================
	void SetMtxParent(D3DXMATRIX* mtx) { m_pMtxParent = mtx; }              //�e�}�g���b�N�X���擾                   
	D3DXMATRIX &GetMatrixWorld() { return m_mtxWorld; }                     //���[���h�ϊ��s��̎擾
	//================================================================================================================================================

	//==========================================================
	//���f�����
	//==========================================================
	CObjectX::ObjectXInfo& GetObjectXInfo() { return m_ObjectXInfo; }
	//================================================================================================================================================

	//===================================
	//�`��
	//===================================
	
	//�e
	void SetUseShadow(bool bUse) { m_bUseShadow = bUse; }

	//�`�悷�邩�ǂ���
	void SetUseDraw(bool bUse) { m_bUseDraw = bUse; }

	//�J�����O���邩�ǂ���
	void SetUseCulling(bool bUse) { m_bUseCulling = bUse; }

	//�_�����}�e���A���Ƀe�N�X�`�����蓖�āi�|�C���^�̃|�C���^�ɂȂ��Ă邩����v�j
	void SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture);

	//�F����
	void SetColor(D3DXCOLOR col, int nColChengeTime,bool bChoose,bool bSetAlpha);
	//=================================================================================================================
private:
	static bool s_bCOMMON_DRAWSHADOW;

	void DrawShadow();                                                       //�e��`�悷��

	//==========================================================
	//�N�H�[�^�j�I��
	//==========================================================
    float m_fAxis;//�N�H�[�^�j�I���̉�]�p
	D3DXVECTOR3 m_VecAxis;//�N�H�[�^�j�I���̉�]��

	//==========================================================
	//�ʒu
	//==========================================================
	D3DXVECTOR3 m_Pos;                              //�ʒu!
	D3DXVECTOR3 m_SupportPos;                       //�ŏ��ɐݒu���ꂽ�ʒu!
	D3DXVECTOR3 m_PosOld;                           //1f�O�̈ʒu!
	D3DXVECTOR3 m_SenterPos;                        //���S�_�̈ʒu
	void CalculateSenterPos();
	//=================================================================================================================

	//==========================================================
	//����
	//==========================================================
	//����
	D3DXVECTOR3 m_Rot;                              //����!

	//���Z����
	D3DXVECTOR3 m_AddRot;//���Z�������!
	bool m_bUseAddRot = false;   //�����̉��Z���g�p���邩�ǂ���!
	//=================================================================================================================

	//==========================================================
	//�g�嗦
	//==========================================================
	//�g�嗦
	D3DXVECTOR3 m_Scale;                            //�傫��!

	//���̊g�嗦
	D3DXVECTOR3 m_FormarScale;                      //���̊g�嗦!

	//��Z�g�嗦
	D3DXVECTOR3 m_MultiScale;//!
	bool m_bUseMultiScale;//!

	//���Z�g�嗦
	bool m_bUseAddScaling;                                  //�g�嗦���g�p���邩�ǂ���!
	D3DXVECTOR3 m_AddScale;                                 //���Z����g�嗦��ݒ�!
	//=================================================================================================================

	//==========================================================
	//�T�C�Y
	//==========================================================

	//���_�̍ŏ��l
	D3DXVECTOR3 m_VtxMin;                           //���ꂼ��̍��W�̃��f���̈ʒu�̍ŏ��l!
	D3DXVECTOR3 m_OriginVtxMin;                     //���X�̍ŏ��l!

	//���_�̍ő�l
	D3DXVECTOR3 m_VtxMax;                           //���ꂼ��̍��W�̃��f���̈ʒu�̍ő�l!
	D3DXVECTOR3 m_OriginVtxMax;                     //���X�̍ő�l!

	//�T�C�Y
	D3DXVECTOR3 m_Size;                             //�T�C�Y!
	//==========================================================================================================================================================

	//===============================================
	//���f�����
	//===============================================
	ObjectXInfo m_ObjectXInfo;                      //���f�����̍\����!
	//===========================================================================================================================================================
	
	//===================================
	//�`��֌W
	//===================================

	//�e
	bool m_bUseShadow;//�e���g�p���邩�ǂ���!

	//�`����g�p���邩�ǂ���
	bool m_bUseDraw;//!

	//�J�����O���邩�ǂ���
	bool m_bUseCulling;

	//�ő咸�_�ƍŏ����_���`�F���W���邩�ǂ���
	bool m_bSwapVtxXZ;

	//�F
	int m_nChengeColorTime;                    //�F��ς��鎞��!
	bool m_bColorChenge;                       //�F��ς��Ă��邩�ǂ���!
	void SetFormarColor();                     //���̐F�����ɖ߂� 

	//���[���h�ϊ��s��
	D3DXMATRIX m_mtxWorld;                     //�}�g���b�N�X���[���h!
	D3DXMATRIX * m_pMtxParent;                   //�e�}�g���b�N�X 
	//=================================================================================================================

	//===================================
	//���f���C���f�b�N�X
	//===================================
	int m_nIndexObjectX;
	//==================================================================================================================
	
	//===================================
	//�G�f�B�^�֌W
	//===================================
	int m_nManagerType;                             //�}�l�[�W���[�ŌĂяo����鎞�̃^�C�v�ԍ�!
	int m_nTypeNum;                                 //���f�����Ƃ̃^�C�v�ԍ�!
	OBJECTXTYPE m_nObjXType;                        //�I�u�W�F�N�gX�̕���!

	void ChengeEditScale();             //�g�嗦��ύX����
	void ChengeEditScaleX();
	void ChengeEditScaleY();
	void ChengeEditScaleZ();
	void ChengeEditPos();               //�ʒu���ړ�����
	void ChengeEditSwapVtxXZ();         //�ő咸�_�ƍŏ����_��ς��邩�ǂ���
	//=================================================================================================================

};
//============================================================================================================
#endif