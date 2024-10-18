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

	CObjectX(int nPriority = 3);                                                           //�R���X�g���N�^
	~CObjectX() override;                                                 //�f�X�g���N�^
	HRESULT Init() override;                                              //����������
	void Uninit() override;                                               //�I������
	void Update() override;                                               //�X�V����
	void Draw() override;                                                 //�`�揈��
	void BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture,D3DCOLORVALUE * pDiffuse);//���f���������蓖�Ă�
	//==========================================================
	//�ʒu
	//==========================================================
	
	//�ʒu
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }                                           //�ʒu�̐ݒ�
	D3DXVECTOR3& GetPos() { return m_Pos; }                                                 //�ʒu�̎擾

	//1f�O�̈ʒu
	void SetPosOld(D3DXVECTOR3 PosOld) { m_PosOld = PosOld; }                               //1f�O�̈ʒu�̐ݒ�
	D3DXVECTOR3& GetPosOld() { return m_PosOld; }                                            //1f�O�̈ʒu��ݒ�

	//�����ʒu
	void SetSupportPos(D3DXVECTOR3 SupportPos) { m_SupportPos = SupportPos; }               //�ݒu�ʒu��ݒ肷��
	D3DXVECTOR3 & GetSupportPos() { return m_SupportPos; }                                  //�ݒu�ʒu���擾����

	//���S�_�̈ʒu
	D3DXVECTOR3& GetSenterPos() { return m_SenterPos; }                                     //���S�_���擾����
	//============================================================================================================

	//==========================================================
	//�ړ���
	//==========================================================
	//�ړ���
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }                                       //�ړ��ʂ�ݒ肷��
	D3DXVECTOR3& GetMove() { return m_Move; }                                               //�ړ��ʂ��擾����

	//����
	void SetUseInteria(bool bUseInteria) { m_bUseInteria = bUseInteria; }                   //�����������邩�ǂ���
	float& GetInertia() { return m_fInertia; }                                              //�������擾����
	void SetInertia(float fInertia) { m_fInertia = fInertia; }                              //������ݒ肷��
	void SetUndoInertia() { m_fInertia = m_fNORMAL_INERTIA; }                               //�����̑傫�������ɖ߂�
	bool& GetUseInteria() { return m_bUseInteria; }                                         //�������|���邩�ǂ������擾

	//�d��
	void SetUseGravity(bool bUse, float fGravityPower) { m_bUseGravity = bUse; m_fGravityPower = fGravityPower; }//�d�͂�ݒ肷��
	bool& GetUseGravity() { return m_bUseGravity; }                                         //�d�͂��g�p���邩�ǂ�����ݒ�

	//��Z����
	void SetUseMultiSpeed(bool bUse, D3DXVECTOR3 MultiSpeed) { m_MultiSpeed = MultiSpeed; m_bUseMultiSpeed = bUse; }//��Z�������g�p���邩�ǂ���

	//���Z���x
	void SetUseAddSpeed(bool bUse, D3DXVECTOR3 AddSpeed) { m_bUseAddSpeed = bUse; m_AddSpeed = AddSpeed; }//�������g�p���邩�ǂ���
	//============================================================================================================

	//==========================================================
	//����
	//==========================================================
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }                                           //�����̐ݒ�
	D3DXVECTOR3 &GetRot() { return m_Rot; }                                                 //�����̎擾
	void SetUseAddRot(bool bUse, D3DXVECTOR3 Rot) { m_bUseAddRot = bUse; m_AddRot = Rot;}
	bool& GetUseAddRot() { return m_bUseAddRot; }//�����̉��Z���g�p���Ă��邩�ǂ���
	D3DXVECTOR3& GetAddRot() { return m_AddRot; }//�����̉��Z�ʂ��擾
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
	D3DXVECTOR3 & GetOriginVtxMax() { return m_OriginVtxMax; }                              //���̊e���_�ő�l�̎擾

	//�ŏ����_
	D3DXVECTOR3 & GetVtxMin() { return m_VtxMin; }                                          //�e���_�ŏ��l�̎擾
	D3DXVECTOR3 & GetOriginVtxMin() { return m_OriginVtxMin; }                              //���̊e���_�ŏ��l�̎擾

	//�T�C�Y
	void SetSize();                                                                         //�T�C�Y��ݒ肷�� 
	D3DXVECTOR3 GetSize() { return m_Size; }                                                //X�I�u�W�F�N�g�̃T�C�Y���擾
	//============================================================================================================

	//==========================================================
	//�G�f�B�^
	//==========================================================

	//�}�l�[�W���[�ԍ�
	void SetManagerType(int nManagerType) { m_nManagerType = nManagerType; }//�}�l�[�W���[�ŌĂяo�����ۂ̃^�C�v�ԍ���ݒ肷��
	int GetManagerType() { return m_nManagerType; }//�}�l�[�W���[�ŌĂяo�����ۂ̃^�C�v�ԍ�

	//�I�u�W�F�N�gX�̎��
	void SetObjXType(OBJECTXTYPE ObjXType) { m_nObjXType = ObjXType; }                      //�I�u�W�F�N�gX�̕��ނ�ݒ�
	OBJECTXTYPE GetObjXType() { return m_nObjXType; }                                       //�I�u�W�F�N�gX�̃^�C�v���擾����

	//�^�C�v�ԍ�
	void SetTypeNum(int nTypeNum) { m_nTypeNum = nTypeNum; }                                //�I�u�W�F�N�g���Ƃ̃^�C�v�ԍ���ݒ�
	int GetTypeNum() { return m_nTypeNum;}                                                  //�I�u�W�F�N�g���Ƃ̃^�C�v�ԍ���ݒ�

	//���\������������
	virtual void DispInfo();                        //���\������

	//================================================================================================================================================

	//==========================================================
	//���[���h�ϊ��s��
	//==========================================================
	D3DXMATRIX &GetMatrixWorld() { return m_mtxWorld; }                                     //���[���h�ϊ��s��̎擾
	//================================================================================================================================================

	//==========================================================
	//���f�����
	//==========================================================
	CObjectX::ObjectXInfo& GetObjectXInfo() { return m_ObjectXInfo; }
	//================================================================================================================================================

	//==========================================================
	//����
	//==========================================================
	
	//�n�ʂɂ��邩
	void SetLanding(bool bLanding) { m_bIsLanding = bLanding; }                             //�n�ʂɂ��邩�ǂ�����ݒ�
	bool& GetLanding() { return m_bIsLanding; }                                             //�n�ʂɂ��邩�ǂ������擾

	//�ǂɂ��邩
	void SetWalling(bool bWalling) { m_bIsWalling = bWalling; }                             //�ǂɂ������Ă��邩�ǂ�����ݒ�
	bool& GetWalling() { return m_bIsWalling; }                                             //�ǂɂ������Ă��邩�ǂ������擾

	//�W�����v���Ă��邩
	void SetJumping(bool bJumping) { m_bIsJumping = bJumping; }                             //�W�����v���Ă��邩�ǂ�����ݒ�
	bool& GetJumping() { return m_bIsJumping; }                                             //�W�����v���Ă��邩�ǂ������擾

    //======================================================
	//�̗͊֌W
	//======================================================

	//�̗�
	void SetLife(int nLife) { m_nLife = nLife; }                    //�̗͂�ݒ肷��
	int& GetLife() { return m_nLife; }                              //�̗͂��擾����

	//�ő�̗�
	void SetMaxLife(int nMaxLife) { m_nMaxLife = nMaxLife; }        //�ő�̗͂�ݒ肷��
	int& GetMaxLife() { return m_nMaxLife; }                        //�ő�̗͂��擾����

	//�q�b�g�X�g�b�v
	void SetHitStopTime(int nHitStopTime) { m_nHitStopTime = nHitStopTime; }//�q�b�g�X�g�b�v���Ԃ�ݒ肷��
	bool& GetHitStop() { return m_bHitStop; }                       //�q�b�g�X�g�b�v��Ԃ��ǂ������擾����

	//�����I�ɑ̗͂����炷
	void SetAutoSubLife(bool bUse) { m_bAutoSubLife = bUse; }       //�̗͂������I�Ɍ��炷���ǂ�����ݒ�
	int& GetHitStopTime() { return m_nHitStopTime; }                //�q�b�g�X�g�b�v���Ԃ��擾����

	//�_���[�W��^����
	virtual void SetDamage(int nDamage, int nHitStopTime);          //�_���[�W��^����

	//�񕜂���
	virtual void SetHeal(int nHeal,D3DXCOLOR col,float fWidth,float fHeight);                  //�񕜗ʂ�^����
	//=================================================================================================================

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
	
	//===================================
	//�ʒu�X�V�֌W
	//===================================
	void SetUseUpdatePos(bool bUse) { m_bUseUpdatePos = bUse; }//�ʒu�̍X�V���g�p���邩�ǂ���
	//=================================================================================================================
protected:
	static const float GetNormalGravity() { return m_fNORMAL_GRAVITY; }//��{�̏d�͂��擾����
	virtual void GravityProcess();                  //�d�͂̏���
private:
	//===============================================
	//�ÓI�����o
	//===============================================
	static constexpr float m_fNORMAL_INERTIA = 0.5f;//���ʂ̊���
	static constexpr float m_fNORMAL_GRAVITY = 1.0f;//���ʂ̏d��
	//=================================================================================================================

	void DrawShadow();                                                       //�e��`�悷��

	//==========================================================
	//�ʒu
	//==========================================================
	D3DXVECTOR3 m_Pos;                              //�ʒu!
	D3DXVECTOR3 m_SupportPos;                       //�ŏ��ɐݒu���ꂽ�ʒu!
	D3DXVECTOR3 m_PosOld;                           //1f�O�̈ʒu!
	D3DXVECTOR3 m_SenterPos;                        //���S�_�̈ʒu
	bool m_bUseUpdatePos;                           //�ʒu�̍X�V���g�p���邩�ǂ���!
	void UpdatePos();                               //�ʒu���X�V������
	void CalculateSenterPos();
	//=================================================================================================================

	//==========================================================
	//�ړ���
	//==========================================================
	//�ړ���
	D3DXVECTOR3 m_Move;                             //�ړ���!
	//����
	float m_fInertia;                               //����!
	bool m_bUseInteria;                             //�����������邩�ǂ���!
	//�d��
	bool m_bUseGravity;                             //�d�͂��g�p���邩�ǂ���!
	float m_fGravityPower;                          //�d�͂̑傫��!
	//��Z����
	bool m_bUseMultiSpeed;                                  //��Z���������邩�ǂ���!
	D3DXVECTOR3 m_MultiSpeed;                               //��Z�����x!
	//����
	bool m_bUseAddSpeed;                                    //�������g�p���邩�ǂ���!
	D3DXVECTOR3 m_AddSpeed;                                 //�����x
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
	//�̗͊֌W
	//===================================
	
	//�q�b�g�X�g�b�v
	bool m_bHitStop;                                //�q�b�g�X�g�b�v!
	int m_nHitStopTime;                             //�q�b�g�X�g�b�v����!
	void HitStopProcess();            //�q�b�g�X�g�b�v�̏���

	//�����I�ɑ̗͂����炷���ǂ���
	bool m_bAutoSubLife;                            //�����I�ɑ̗͂����炵�����邩�ǂ���!

	//�̗�
	int m_nLife;                                    //�̗�!
	int m_nMaxLife;                                 //�ő�̗�!
	//============================================================================================================

	//===================================
	//����֌W
    //===================================
	bool m_bIsLanding;                              //�n�ʂɂ��邩�ǂ���!
	bool m_bIsWalling;                              //�ǂɂ������Ă��邩�ǂ���!
	bool m_bIsJumping;                              //�W�����v���Ă��邩�ǂ���!
	//============================================================================================================

	//===================================
	//�`��֌W
	//===================================

	//�e
	bool m_bUseShadow;//�e���g�p���邩�ǂ���!

	//�`����g�p���邩�ǂ���
	bool m_bUseDraw;//!

	//�J�����O���邩�ǂ���
	bool m_bUseCulling;

	//�F
	bool m_bUseRatioLifeAlpha;                 //�̗͂̊����œ����x��ς��邩�ǂ���!
	int m_nChengeColorTime;                    //�F��ς��鎞��!
	bool m_bColorChenge;                       //�F��ς��Ă��邩�ǂ���!
	void SetFormarColor();                     //���̐F�����ɖ߂� 

	//���[���h�ϊ��s��
	D3DXMATRIX m_mtxWorld;                      //�}�g���b�N�X���[���h!
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
	//=================================================================================================================

};
//============================================================================================================
#endif