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

	static D3DXCOLOR CalRaibowColor();

	// �X�N���[�����W�����[���h���W�ɕϊ�
	static D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout,
		float Sx,  // �X�N���[��X���W
		float Sy,  // �X�N���[��Y���W
		float fZ,  // �ˉe��Ԃł�Z�l�i0�`1�j
		int Screen_w,
		int Screen_h,
		D3DXMATRIX* View,
		D3DXMATRIX* Prj
	);

	//���[���h���W���X�N���[�����W�ɕϊ�
	static D3DXVECTOR3 CalcWorldToScreenNoViewport(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight);
	// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
	static D3DXVECTOR3* CalcScreenToXZ(
		D3DXVECTOR3* pout,
		float Sx,
		float Sy,
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

	//�_�Ǝl�p�`�̊e�ӂƂ̋��������߂�
	static float pointLineDistance(float cx, float cy, float x1, float y1, float x2, float y2);

	//�l�p�`�Ɖ~�̓����蔻��
	static bool checkCollisionCircleRectangle(float cx, float cy, float r,
		float x1, float y1, float x2, float y2,
		float x3, float y3, float x4, float y4);

	//�������v�Z����
	static int CalculationDigit(int nNum);

	//�x�N�g�����v�Z����
	static D3DXVECTOR3 CalcVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos,bool bNormalize);

	//�p�����߂�
	static float CalcElevationAngle(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos);

	//================================================
	//Yaw��Pitch�����߁A�ړI�̈ʒu�ւ̊p�x�����߂�
	//================================================

	//�ړI�̈ʒu�ւ̌������v�Z����
	static void CalcRotToTarget(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos, float& OutYaw, float& OutPitch);

	//�ړI�̈ʒu�ւ̌�����Yaw��Pitch���g�p���ċ��߂�
	static D3DXVECTOR3 CalcDirectionFromYawPitch(const float Yaw, const float Pitch);

	//�ړI�̈ʒu�ւ̊p�x���܂Ƃ߂ċ��߂�
	static D3DXVECTOR3 CalcSummarizeRotToTarget(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos);
	//=======================================================================================================================


	//=============================================================================================================
private:
};
#endif
