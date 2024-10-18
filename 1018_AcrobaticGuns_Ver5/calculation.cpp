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
	if (fMyRot > D3DX_PI * 0.5f + fCameraRot && fRotAim < -D3DX_PI * 0.5f + fCameraRot)
	{
		fRotDiff += D3DX_PI * 2;
	}
	if (fMyRot < -D3DX_PI * 0.5f + fCameraRot && fRotAim > D3DX_PI * 0.5f + fCameraRot)
	{
		fRotDiff -= D3DX_PI * 2;
	}

	//���X�ɖړI�̌����֍��킹�Ă���
	fMyRot += fRotDiff * fDecayRot;

	//�����̒����i�l��3.14�`-3.14�̒��ɌŒ肵�����̂ŁE�E�E�j
	if (fMyRot > D3DX_PI + fCameraRot)
	{//3.14��-3.14�ɂ���
		fMyRot -= D3DX_PI * 2;
	}
	else if (fMyRot < -D3DX_PI + fCameraRot)
	{//-3.14��3.14�ɂ���
		fMyRot += D3DX_PI * 2;
	}
}
//===========================================================================================================

//=========================================================
//�ړ������ւ̊p�x���v�Z����
//=========================================================
bool CCalculation::CaluclationMove(D3DXVECTOR3& Pos, float fSpeed, MOVEAIM MoveAim, float& fRot)
{
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	float fMoveX = 0.0f;                                            //X�����̈ړ���
	float fMoveZ = 0.0f;                                            //Z�����̈ړ���
	bool bMove = false;                                             //�ړ����Ă��邩�ǂ��� 
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


	if (bMove == true)
	{//�ړ���ԂȂ�
		fRot = atan2f(fMoveX, fMoveZ) + fCameraRot;
		switch (MoveAim)
		{
		case MOVEAIM_XY:
			Pos.x += sinf(fRot) * fSpeed;
			Pos.y += cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_XZ:
			Pos.x += sinf(fRot) * fSpeed;
			Pos.z += cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_ZY:
			Pos.z += sinf(fRot) * fSpeed;
			Pos.y += cosf(fRot) * fSpeed;
			break;
		default:
			break;
		}
	}

	return bMove;
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