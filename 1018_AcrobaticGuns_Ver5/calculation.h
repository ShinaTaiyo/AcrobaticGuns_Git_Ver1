//====================================================
//
//�W���Q�X���F�v�Z�p�̃N���X�����[calculation.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _CALCULATION_H_  
#define _CALCULATION_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
//==========================================

//==========================================

//==========================================
//�v�Z�N���X
//==========================================
class CCalculation
{
public:
    //==============================
	//�ړ������񋓌^
	//==============================
	typedef enum
	{
		MOVEAIM_XY = 0,//XY�����ɓ���
		MOVEAIM_XZ,    //XZ�����ɓ���
		MOVEAIM_ZY,    //ZY�����ɓ���
		MOVEAIM_MAX
	}MOVEAIM;
	//=============================================================================================================

	//==============================
	//�v���g�^�C�v�錾
	//==============================
	CCalculation();                   //�R���X�g���N�^
	~CCalculation();                  //�f�X�g���N�^
	static float CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos);//2�_�̋������v�Z����
	static float CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos);//�Q�����̖ړI�����ւ̊p�x�����߂�
	static float CalculationParabola(float fLength, float fGravity, float fSpeed,D3DXVECTOR3 Pos,D3DXVECTOR3 PurposePos);//�����A�d�́A���x�����ƂɂQ�_�̕������ړ����v�Z����
	static float CalculationRandVecXY();//�����_���ȂQ�����x�N�g����Ԃ�
	static void CalculationCollectionRot2D(float& fMyRot, float fRotAim, float fDecayRot);//�Q���������̌����̕␳���s��
	static bool CaluclationMove(bool bUseStick,D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim,float & fRot);//�ړ��Ɏg�������̊p�x�����߂�
	static D3DXVECTOR3 Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed);  //�ړI�ւ̈ړ��ʂ����߂�

	// �X�N���[�����W�����[���h���W�ɕϊ�
	static D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout,
		int Sx,  // �X�N���[��X���W
		int Sy,  // �X�N���[��Y���W
		float fZ,  // �ˉe��Ԃł�Z�l�i0�`1�j
		int Screen_w,
		int Screen_h,
		D3DXMATRIX* View,
		D3DXMATRIX* Prj
	);

	// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
	static D3DXVECTOR3* CalcScreenToXZ(
		D3DXVECTOR3* pout,
		int Sx,
		int Sy,
		int Screen_w,
		int Screen_h,
		D3DXMATRIX* View,
		D3DXMATRIX* Prj
	);

	//���C�Ƌ��̏Փ˔���
	static bool CalcRaySphere(
		D3DXVECTOR3 LayPos,//���C�̎x�_
		D3DXVECTOR3 LayVec,//���C�̕����x�N�g��
		D3DXVECTOR3 SphereSenterPos,//���̒��S�_
		float r,                    //���̔��a
		D3DXVECTOR3 & CollisionStartPos,//���C�̏ՓˊJ�n�ʒu
		D3DXVECTOR3 & CollisoinEndPos   //���C�̏ՓˏI���ʒu
	);
	//�ړI�̕��̂ƃ��C�̌�������v���Ă��邩�ǂ����𔻒�
	static bool CalcMatchRay(D3DXVECTOR3 AimPos, float fSx, float fSy, int nScreen_w, int nScreen_h, D3DXMATRIX* View, D3DXMATRIX * Prj);

	static int CalculationDigit(int nNum);//�������v�Z����
	//=============================================================================================================
private:
};
#endif
