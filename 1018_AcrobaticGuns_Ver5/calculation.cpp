//===========================================================================================================
//
//�W���Q�X���F�v�Z�p�̃N���X�����[calculation.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================================

//=========================================================
//�C���N���[�h
//=========================================================
#include "calculation.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "input.h"
//===========================================================================================================

//=========================================================
//�R���X�g���N�^
//=========================================================
CCalculation::CCalculation()
{

}
//===========================================================================================================

//=========================================================
//�f�X�g���N�^
//=========================================================
CCalculation::~CCalculation()
{

}
//===========================================================================================================

//=========================================================
//�������v�Z����
//=========================================================
float CCalculation::CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fLength = 0.0f;
	fLength = sqrtf(powf(PurposePos.x - Pos.x, 2) + powf(PurposePos.y - Pos.y, 2) + powf(PurposePos.z - Pos.z, 2));
	return fLength;
}
//==========================================================================================================

//=========================================================
//XY�x�N�g�����v�Z����
//=========================================================
float CCalculation::CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fVXaim = PurposePos.x - Pos.x;
	float fVYaim = PurposePos.y - Pos.y;
	float fVLaim = atan2f(fVXaim, fVYaim);
	return fVLaim;
}
//==========================================================================================================

//=========================================================
//�������̊p�x���v�Z����
//=========================================================
float CCalculation::CalculationParabola(float fLength, float fGravity, float fSpeed, D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fRot = 0.0f;//�p�x

	float fXG = fLength * fGravity;//�����~�d��
	float fV = fSpeed;             //���x
	float fV2 = powf(fV, 2);       //���x�Q��

	fRot = asinf(fXG / fV2);
	fRot /= 2;

	if (PurposePos.x < Pos.x)
	{//�p�x����Βl�Ȃ̂ŁA�ړI�̈ʒu�������̈ʒu���}�C�i�X�����ɂ���ꍇ�A���̒l�ɂ���
		fRot *= -1;
	}

	return fRot;
}
//===========================================================================================================

//=========================================================
//XY�����̗����x�N�g����p�ӂ���
//=========================================================
float CCalculation::CalculationRandVecXY()
{
	float fRandRot = float(rand() % 628 + 1) / 100;
	return fRandRot;
}
//===========================================================================================================

//=========================================================
//2D�����̌����̕␳���s��
//=========================================================
void CCalculation::CalculationCollectionRot2D(float& fMyRot, float fRotAim, float fDecayRot)
{
	float fRotDiff = 0.0f;//�����̍���
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	fRotDiff = fRotAim - fMyRot;

	//�����̍��������߂�i3.14��-3.14�̋��E�����܂��������ɂȂ�����␳����)
	//if (fMyRot > D3DX_PI * 0.5f + fCameraRot && fRotAim < -D3DX_PI * 0.5f + fCameraRot)//1.57�ȏ� -1.57����
	//{
	//	fRotDiff += D3DX_PI * 2 + fCameraRot;
	//}
	//if (fMyRot < -D3DX_PI * 0.5f + fCameraRot && fRotAim > D3DX_PI * 0.5f + fCameraRot)
	//{
	//	fRotDiff -= D3DX_PI * 2 + fCameraRot;
	//}

	//�����̍����̒���
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= D3DX_PI * 2;
	}
	else if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2;
	}


	//���X�ɖړI�̌����֍��킹�Ă���
	fMyRot += fRotDiff * fDecayRot;

	//�����̒����i�J��������ɒl��3.14�`-3.14�̒��ɌŒ肵�����̂ŁE�E�E�j
	if (fMyRot >= D3DX_PI + fCameraRot)
	{//3.14��-3.14�ɂ���
		fMyRot -= D3DX_PI * 2;
	}
	else if (fMyRot <= -D3DX_PI + fCameraRot)
	 
	{//-3.14��3.14�ɂ���
		fMyRot += D3DX_PI * 2;
	}
}
//===========================================================================================================

//=========================================================
//�ړ������ւ̊p�x���v�Z����
//=========================================================
bool CCalculation::CaluclationMove(bool bUseStick, D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim, float& fRot)
{
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	float fMoveX = 0.0f;                                            //X�����̈ړ���
	float fMoveZ = 0.0f;                                            //Z�����̈ړ���
	bool bMove = true;                                             //�ړ����Ă��邩�ǂ��� 

	if (bUseStick == false)
	{
		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_UP) == true)
		{
			fMoveZ = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_DOWN) == true)
		{
			fMoveZ = -1.0f;
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_RIGHT) == true)
		{
			fMoveX = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_LEFT) == true)
		{
			fMoveX = -1.0f;
		}

		if (fMoveX != 0.0f || fMoveZ != 0.0f)
		{
			bMove = true;//�ړ����
		}
		else
		{
			bMove = false;//�ҋ@���
		}
	}
	else
	{
		bMove = CManager::GetInputJoypad()->GetLStickPress(8);
	}
	if (bMove == true)
	{//�ړ���ԂȂ�

		if (bUseStick == false)
		{//WASD�̃{�^�����͂���Ɍ��������߂�
			fRot = atan2f(fMoveX, fMoveZ);
		}
		else
		{
			//�J��������Ɍ��������߂�
			fRot = fCameraRot + CManager::GetInputJoypad()->GetLStickAimRot();
		}
		switch (MoveAim)
		{
		case MOVEAIM_XY:
			Move.x = sinf(fRot) * fSpeed;
			Move.y = cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_XZ:
			Move.x = sinf(fRot) * fSpeed;
			Move.z = cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_ZY:
			Move.z = sinf(fRot) * fSpeed;
			Move.y = cosf(fRot) * fSpeed;
			break;
		default:
			break;
		}
	}

	return bMove;
}
//===========================================================================================================


//=========================================================
//�ړI�̈ʒu�ւ̈ړ��ʂ��v�Z����
//=========================================================
D3DXVECTOR3 CCalculation::Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed)

{
	D3DXVECTOR3 VecAim = NULL_VECTOR3;       //���ꂼ��̕����̃x�N�g��
	D3DXVECTOR3 ResultMove = NULL_VECTOR3;   //���ʂ̈ړ���
	float fVLaim = 0.0f;                     //�����x�N�g��

	//==========================
	//�R�����x�N�g�������
	//==========================

	//���ꂼ��̕����̃x�N�g�������߂�
	VecAim.x = MyPos.x - AimPos.x;
	VecAim.y = MyPos.y - AimPos.y;
	VecAim.z = MyPos.z - AimPos.z;

	//�����x�N�g�������߂�
	fVLaim = sqrtf(powf(VecAim.x,2) + powf(VecAim.y,2) + powf(VecAim.z,2));

	//�ړI�̈ʒu�ւ̈ړ��ʂ����߂�
	ResultMove.x = VecAim.x / fVLaim * fSpeed;
	ResultMove.y = VecAim.y / fVLaim * fSpeed;
	ResultMove.z = VecAim.z / fVLaim * fSpeed;

	return ResultMove;
}
//===========================================================================================================

//=========================================================
//�������v�Z����
//=========================================================
int CCalculation::CalculationDigit(int nNum)
{
	int nDigit = 0;
	while (nNum != 0) {
		nNum /= 10;
		nDigit++;
	}
	return nDigit;
}
//===========================================================================================================