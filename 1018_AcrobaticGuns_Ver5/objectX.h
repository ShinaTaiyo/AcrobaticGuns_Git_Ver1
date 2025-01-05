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

	//=================================
	//�`����\����
	//=================================
	struct DrawInfo
	{
		//�e
		bool bUseShadow = true;//�e���g�p���邩�ǂ���!

		//�`����g�p���邩�ǂ���
		bool bUseDraw = true;//!

		//�J�����O���邩�ǂ���
		bool bUseCulling = false;

		//�F
		int nChengeColorTime = 0;                    //�F��ς��鎞��!
		bool bColorChenge = false;                       //�F��ς��Ă��邩�ǂ���!

		//���[���h�ϊ��s��
		D3DXMATRIX mtxWorld = {};                     //�}�g���b�N�X���[���h!
		D3DXMATRIX* pMtxParent = nullptr;             //�e�}�g���b�N�X 
		D3DXCOLOR Color = { 1.0f,1.0f,1.0f,1.0f };    //���݂̓���̐F����

		//�e
		void SetUseShadow(bool bUse) { bUseShadow = bUse; }
		const bool& GetUseDraw() const { return bUseDraw; }

		//�`�悷�邩�ǂ���
		void SetUseDraw(bool bUse) { bUseDraw = bUse; }

		//�J�����O���邩�ǂ���
		void SetUseCulling(bool bUse) { bUseCulling = bUse; }

        //���[���h�ϊ��s��
		void SetMtxParent(D3DXMATRIX* mtx) {pMtxParent = mtx; }              //�e�}�g���b�N�X���擾                   
		D3DXMATRIX& GetMatrixWorld() { return mtxWorld; }                     //���[���h�ϊ��s��̎擾
		//================================================================================================================================================
	};
	//============================================================================================================

	struct PosInfo
	{
		//==========================================================
        //�ʒu
        //==========================================================
		D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };                              //�ʒu!
		D3DXVECTOR3 SupportPos = { 0.0f,0.0f,0.0f };                       //�ŏ��ɐݒu���ꂽ�ʒu!
		D3DXVECTOR3 PosOld = { 0.0f,0.0f,0.0f };                           //1f�O�̈ʒu!
		D3DXVECTOR3 SenterPos = { 0.0f,0.0f,0.0f };                        //���S�_�̈ʒu
		//=================================================================================================================
    
	    //�ʒu
		void SetPos(D3DXVECTOR3 CopyPos) { Pos = CopyPos; }                                           //�ʒu�̐ݒ�
		const D3DXVECTOR3& GetPos() const { return Pos; }                                     //�ʒu�̎擾

		//1f�O�̈ʒu
		void SetPosOld(D3DXVECTOR3 CopyPosOld) { PosOld = CopyPosOld; }                               //1f�O�̈ʒu�̐ݒ�
		const D3DXVECTOR3& GetPosOld() const { return PosOld; }                                            //1f�O�̈ʒu��ݒ�

		//�����ʒu
		void SetSupportPos(D3DXVECTOR3 CopySupportPos) { SupportPos = CopySupportPos; }               //�ݒu�ʒu��ݒ肷��
		D3DXVECTOR3& GetSupportPos() { return SupportPos; }                                  //�ݒu�ʒu���擾����

		//���S�_�̈ʒu
		D3DXVECTOR3& GetSenterPos() { return SenterPos; }                                     //���S�_���擾����
		//============================================================================================================
	};

	struct RotInfo
	{
		//�ϐ�
		D3DXVECTOR3 Rot = {0.0f,0.0f,0.0f};                              //����!
		D3DXVECTOR3 AddRot = { 0.0f,0.0f,0.0f};//���Z�������!
		bool bUseAddRot = false;   //�����̉��Z���g�p���邩�ǂ���!

		//�֐�
		void SetRot(D3DXVECTOR3 CopyRot) { Rot = CopyRot; }                                           //�����̐ݒ�
		D3DXVECTOR3& GetRot() { return Rot; }                                                 //�����̎擾
		void SetUseAddRot(bool bUse, D3DXVECTOR3 CopyRot) { bUseAddRot = bUse; AddRot = CopyRot; }
		bool& GetUseAddRot() { return bUseAddRot; }//�����̉��Z���g�p���Ă��邩�ǂ���
		D3DXVECTOR3& GetAddRot() { return AddRot; }//�����̉��Z�ʂ��擾
	};

	struct SizeInfo
	{
		//*�ϐ�

		//���_�̍ŏ��l
		D3DXVECTOR3 VtxMin = { 0.0f,0.0f,0.0f };                           //���ꂼ��̍��W�̃��f���̈ʒu�̍ŏ��l!
		D3DXVECTOR3 OriginVtxMin = { 0.0f,0.0f,0.0f };                     //���X�̍ŏ��l!

		//���_�̍ő�l
		D3DXVECTOR3 VtxMax = { 0.0f,0.0f,0.0f };                           //���ꂼ��̍��W�̃��f���̈ʒu�̍ő�l!
		D3DXVECTOR3 OriginVtxMax = { 0.0f,0.0f,0.0f };                     //���X�̍ő�l!

		//�T�C�Y
		D3DXVECTOR3 Size = { 0.0f,0.0f,0.0f };                             //�T�C�Y!

		//XZ�̃T�C�Y��ς���
		bool bSwapVtxXZ = false;

		//�ϐ�
		D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f };                            //�傫��!
		D3DXVECTOR3 FormarScale = { 1.0f,1.0f,1.0f };                      //���̊g�嗦!
		D3DXVECTOR3 MultiScale = { 0.0f,0.0f,0.0f };                       //��Z�g�嗦
		D3DXVECTOR3 AddScale = { 0.0f,0.0f,0.0f };                         //���Z����g�嗦��ݒ�!
		bool bUseMultiScale = false;                                       //�g�嗦����Z���邩�ǂ���
		bool bUseAddScaling = false;                                       //�g�嗦���g�p���邩�ǂ���!


		//*�֐�
		D3DXVECTOR3& GetVtxMax() { return VtxMax; }                                          //�e���_�ő�l�̎擾
		void SetVtxMax(D3DXVECTOR3 CopyVtxMax) { VtxMax = CopyVtxMax; }
		D3DXVECTOR3& GetOriginVtxMax() { return OriginVtxMax; }                              //���̊e���_�ő�l�̎擾
		void SetOriginVtxMax(D3DXVECTOR3 CopyVtxMax) { OriginVtxMax = CopyVtxMax; }                   //���̍ő咸�_�̐ݒ�

		//�ŏ����_
		D3DXVECTOR3& GetVtxMin() { return VtxMin; }                                          //�e���_�ŏ��l�̎擾
		void SetVtxMin(D3DXVECTOR3 CopyVtxMin) { VtxMin = CopyVtxMin; }
		D3DXVECTOR3& GetOriginVtxMin() { return OriginVtxMin; }                              //���̊e���_�ŏ��l�̎擾
		void SetOriginVtxMin(D3DXVECTOR3 CopyVtxMin) { OriginVtxMin = CopyVtxMin; }                   //���̍ŏ����_�̐ݒ�

		D3DXVECTOR3 GetSize() { return Size; }                                                //X�I�u�W�F�N�g�̃T�C�Y���擾

		//�ő咸�_�ƍŏ����_���X���b�v
		void ActiveSwapVtxMaxMin()
		{
			if (bSwapVtxXZ == true)
			{
				swap(OriginVtxMax.x, OriginVtxMax.z);
				swap(OriginVtxMin.x, OriginVtxMin.z);
			}
		}
		//�֐�
		void SetScale(D3DXVECTOR3 CopyScale) { Scale = CopyScale; }                                  //�g�嗦�̐ݒ�
		D3DXVECTOR3& GetScale() { return Scale; }                                            //�g�嗦�̎擾

		//���̊g�嗦
		void SetFormarScale(D3DXVECTOR3 CopyScale) { FormarScale = CopyScale; }                      //���̊g�嗦��ݒ�
		D3DXVECTOR3& GetFormarScale() { return FormarScale; }                                //���̊g�嗦���擾

		//���Z�g�嗦
		void SetUseAddScale(D3DXVECTOR3 CopyAddScale, bool bUse);                                  //�g�嗦�̉��Z���g�p���邩�ǂ���

		//��Z�g�嗦
		void SetUseMultiScale(bool bUse, D3DXVECTOR3 CopyScale) { bUseMultiScale = bUse; MultiScale = CopyScale; }

		void SetUseSwapVtxXZ(bool bUse) { bSwapVtxXZ = bUse; }
		const bool& GetUseSwapVtxXZ()const { return bSwapVtxXZ; }
	};



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
	
	//�N�H�[�^�j�I��
	void SetAxis(float fAxis) { m_fAxis = fAxis; }//��]�p��ݒ�
	const float& GetAxis() const { return m_fAxis; }//��]�p���擾 
	void SetVecAxis(D3DXVECTOR3 VecAxis) { m_VecAxis = VecAxis; }//��]����ݒ�
	const D3DXVECTOR3& GetVecAxis() const { return m_VecAxis; }  //��]�����擾

	//============================================================================================================

	//==========================================================
	//�g�嗦
	//==========================================================
	//============================================================================================================

	//==========================================================
	//�T�C�Y
	//==========================================================
	void CalculateSenterPos();
	void SetSize();                                                                         //�T�C�Y��ݒ肷�� 
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
	//���f�����
	//==========================================================
	CObjectX::ObjectXInfo& GetObjectXInfo() { return m_ObjectXInfo; }
	//================================================================================================================================================

	//===================================
	//�`��
	//===================================
	//�_�����}�e���A���Ƀe�N�X�`�����蓖�āi�|�C���^�̃|�C���^�ɂȂ��Ă邩����v�j
	void SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture);

	//�F����
	void SetColor(D3DXCOLOR col, int nColChengeTime, bool bChoose, bool bSetAlpha);


	//=================================================================================================================

	//===================================
	//�e�\����
	//===================================

	//�`����̎擾
	DrawInfo& GetDrawInfo() { return m_DrawInfo; }

	//�ʒu���̎擾
	PosInfo& GetPosInfo() { return m_PosInfo; }

	//�������̎擾
	RotInfo& GetRotInfo() { return m_RotInfo; }

	//�T�C�Y���̎擾
	SizeInfo& GetSizeInfo() { return m_SizeInfo; }
	//=================================================================================================================

private:
	static bool s_bCOMMON_DRAWSHADOW;

	void DrawShadow();                                                       //�e��`�悷��

	//==========================================================
	//�N�H�[�^�j�I��
	//==========================================================
    float m_fAxis;//�N�H�[�^�j�I���̉�]�p
	D3DXVECTOR3 m_VecAxis;//�N�H�[�^�j�I���̉�]��
	//==========================================================================================================================================================

	//===============================================
	//���f�����
	//===============================================
	ObjectXInfo m_ObjectXInfo;                      //���f�����̍\����!
	//===========================================================================================================================================================
	
	//===================================
	//�`��֌W
	//===================================
	void SetFormarColor();                     //���̐F�����ɖ߂�
	//=================================================================================================================

	//===================================
	//���f���C���f�b�N�X
	//===================================
	int m_nIndexObjectX;
	//==================================================================================================================
	
	//===================================
	//�e�\����
	//===================================
	DrawInfo m_DrawInfo;   //�`����
	PosInfo m_PosInfo;     //�ʒu���
	RotInfo m_RotInfo;     //�������
	SizeInfo m_SizeInfo;   //�T�C�Y���
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