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
	static bool CaluclationMove(D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim,float & fRot);//�ړ��Ɏg�������̊p�x�����߂�
	static D3DXVECTOR3 Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed);  //�ړI�ւ̈ړ��ʂ����߂�
	static int CalculationDigit(int nNum);//�������v�Z����
	//=============================================================================================================
private:
};
#endif
