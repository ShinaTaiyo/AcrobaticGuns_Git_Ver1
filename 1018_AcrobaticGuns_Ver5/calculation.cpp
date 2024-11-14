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
#include "debugproc.h"
#include "particle.h"
#include "attack.h"
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
		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::UP) == true)
		{
			fMoveZ = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::DOWN) == true)
		{
			fMoveZ = -1.0f;
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RIGHT) == true)
		{
			fMoveX = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LEFT) == true)
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
		bMove = CManager::GetInputJoypad()->GetLStickPress(8,0.0f);
	}
	if (bMove == true)
	{//�ړ���ԂȂ�
		//�J��������Ɍ��������߂�
		if (bUseStick == true)
		{
			fRot = fCameraRot + CManager::GetInputJoypad()->GetLStickAimRot() + D3DX_PI;
		}
		else
		{
			fRot = atan2f(fMoveX, fMoveZ) + fCameraRot + D3DX_PI;
		}
		switch (MoveAim)
		{
		case MOVEAIM_XY:
			Move.x = sinf(fRot) * fSpeed;
			Move.y = cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_XZ:
			Move.x = sinf(fRot - D3DX_PI) * fSpeed;
			Move.z = cosf(fRot - D3DX_PI) * fSpeed;
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
	D3DXVECTOR3 VecAim = D3DXVECTOR3(0.0f,0.0f,0.0f);       //���ꂼ��̕����̃x�N�g��
	D3DXVECTOR3 ResultMove = D3DXVECTOR3(0.0f,0.0f,0.0f);   //���ʂ̈ړ���
	float fVLaim = 0.0f;                     //�����x�N�g��

	//==========================
	//�R�����x�N�g�������
	//==========================

	//���ꂼ��̕����̃x�N�g�������߂�
	VecAim.x = AimPos.x - MyPos.x;
	VecAim.y = AimPos.y - MyPos.y;
	VecAim.z = AimPos.z - MyPos.z;

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
//���F���v�Z����
//=========================================================
D3DXCOLOR CCalculation::CalRaibowColor()
{
	D3DXCOLOR RaibowColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	RaibowColor.r = float(rand() % 100 + 1) / 100;
	RaibowColor.g = float(rand() % 100 + 1) / 100;
	RaibowColor.b = float(rand() % 100 + 1) / 100;
	return RaibowColor;
}
//===========================================================================================================

//=========================================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//=========================================================
D3DXVECTOR3* CCalculation::CalcScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);//�r���[�}�g���b�N�X�Ƃ̋t����
	D3DXMatrixInverse(&InvPrj, NULL, Prj);  //�v���W�F�N�V�����}�g���b�N�X�Ƃ̋t����
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	//����
	D3DXVECTOR3 MyPos = D3DXVECTOR3(Sx,Sy,fZ);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;//���[���h���W�����߂�
	D3DXVec3TransformCoord(pout, &MyPos, &tmp);     //�ʒu�����߂�
	return pout;
}
//===========================================================================================================

//=========================================================
// ���[���h���W���X�N���[�����W�ɕϊ�����
//=========================================================
D3DXVECTOR3 CCalculation::CalcWorldToScreenNoViewport(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight)
{
	//���[���h���W���N���b�v���W�ɕϊ�
	D3DXVECTOR4 ClipSpacePos;
	D3DXMATRIX mtxTrans;

	mtxTrans = viewMatrix * projectionMatrix;
	D3DXVec3Transform(&ClipSpacePos, &worldPos, &mtxTrans);

	//�������Z�i�N���b�v���W����NDC��Ԃցj
	if (ClipSpacePos.w != 0.0f)
	{
		ClipSpacePos.x /= ClipSpacePos.w;
		ClipSpacePos.y /= ClipSpacePos.w;
		ClipSpacePos.z /= ClipSpacePos.w;
	}

	//�X�N���[�����W�֕ϊ�
	D3DXVECTOR3 ScreenPos;
	ScreenPos.x = (ClipSpacePos.x * 0.5f + 0.5f) * screenWidth;
	ScreenPos.y = (1.0f - (ClipSpacePos.y * 0.5f + 0.5f)) * screenHeight;
	ScreenPos.z = ClipSpacePos.z;//�[�x�l�i�O�`�P�j�͈̔�

	return ScreenPos;
}
//===========================================================================================================

//=========================================================
// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
//=========================================================
D3DXVECTOR3* CCalculation::CalcScreenToXZ(D3DXVECTOR3* pout,float Sx,float Sy, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	bool bCross = false;

	D3DXVECTOR3 Pos1 = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);//�i�Ŗ��j�����J�����̈ʒu
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj); //�i�Ŗ��j�����`��͈͂̈�ԉ��̈ʒu
	ray = farpos - nearpos;

	D3DXVec3Normalize(&ray, &ray);

	nearpos *= -1;
	// ���Ƃ̌������N���Ă���ꍇ�͌�_��
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	//if (ray.y <= 0) {
	//	// ����_
	//	float Lray = D3DXVec3Dot(&ray, &Pos1);
	//	float LP0 = D3DXVec3Dot(&nearpos, &Pos1);
	//	*pout = nearpos + (LP0 / Lray) * ray;
	//	bCross = true;
	//	//CManager::GetDebugProc()->PrintDebugProc("Lray:%f\n", Lray);
	//	//CManager::GetDebugProc()->PrintDebugProc("LP0:%f\n",LP0);
	//}
	//else {
	* pout = farpos;
	bCross = false;
	//}
	//CManager::GetDebugProc()->PrintDebugProc("���C�̌����F%f %f %f\n", ray.x, ray.y, ray.z);
	//CManager::GetDebugProc()->PrintDebugProc("���C�����ƌ������Ă��邩�ǂ����F%d\n", bCross);

	return pout;
}
//===========================================================================================================

//=========================================================
// ���C�Ƌ��̏Փ˔���
//=========================================================
bool CCalculation::CalcRaySphere(D3DXVECTOR3 LayPos, D3DXVECTOR3 LayVec, D3DXVECTOR3 SphereSenterPos, float r, D3DXVECTOR3& CollisionStartPos, D3DXVECTOR3& CollisoinEndPos)
{
	SphereSenterPos.x = SphereSenterPos.x - LayPos.x;
	SphereSenterPos.y = SphereSenterPos.y - LayPos.y;
	SphereSenterPos.z = SphereSenterPos.z - LayPos.z;

	float A = LayVec.x * LayVec.x + LayVec.y * LayVec.y + LayVec.z * LayVec.z;
	float B = LayVec.x * SphereSenterPos.x + LayVec.y * SphereSenterPos.y + LayVec.z * SphereSenterPos.z;
	float C = SphereSenterPos.x * SphereSenterPos.x + SphereSenterPos.y * SphereSenterPos.y + SphereSenterPos.z * SphereSenterPos.z - r * r;
	
	CManager::GetDebugProc()->PrintDebugProc("A = %f\n", A);
	CManager::GetDebugProc()->PrintDebugProc("B = %f\n", B);
	CManager::GetDebugProc()->PrintDebugProc("C = %f\n", C);


	if (A == 0.0f)
		return false; // ���C�̒�����0

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // �Փ˂��Ă��Ȃ�

	CManager::GetDebugProc()->PrintDebugProc("s = %f\n", s);

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	CManager::GetDebugProc()->PrintDebugProc("a1 = %f\n", a1);
	CManager::GetDebugProc()->PrintDebugProc("a2 = %f\n", a2);

	if (a1 < 0.0f || a2 < 0.0f)
		return false; // ���C�̔��΂ŏՓ�

	//�ՓˊJ�n�ʒu�����߂�
	CollisionStartPos.x = LayPos.x + a1 * LayVec.x;
	CollisionStartPos.y = LayPos.y + a1 * LayVec.y;
	CollisionStartPos.z = LayPos.z + a1 * LayVec.z;
	CManager::GetDebugProc()->PrintDebugProc("���C�ђʊJ�n�ʒu�F %f %f %f\n",CollisionStartPos.x,CollisionStartPos.y,CollisionStartPos.z);

	//�ՓˏI���ʒu�����߂�
	CollisoinEndPos.x = LayPos.x + a2 * LayVec.x;
	CollisoinEndPos.y = LayPos.y + a2 * LayVec.y;
	CollisoinEndPos.z = LayPos.z + a2 * LayVec.z;
	CManager::GetDebugProc()->PrintDebugProc("���C�ђʏI���ʒu�F %f %f %f\n", CollisoinEndPos.x, CollisoinEndPos.y, CollisoinEndPos.z);

	return true;
}
//===========================================================================================================

//=========================================================
//�ړI�̈ʒu�Ƒ_���Ă���ʒu�Ƃ̃��C����v���Ă��邩�ǂ����𔻒�
//=========================================================
bool CCalculation::CalcMatchRay(D3DXVECTOR3 AimPos, float fSx, float fSy, int nScreen_w, int nScreen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos1;
	D3DXVECTOR3 farpos1;
	D3DXVECTOR3 ray1;
	bool bCross = false;

	D3DXVECTOR3 Pos1 = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	CalcScreenToWorld(&nearpos1,fSx,fSy, 0.0f, nScreen_w, nScreen_h, View, Prj);//�i�Ŗ��j�����J�����̈ʒu
	CalcScreenToWorld(&farpos1, fSx,fSy, 1.0f, nScreen_w, nScreen_h, View, Prj); //�i�Ŗ��j�����`��͈͂̈�ԉ��̈ʒu
	ray1 = farpos1 - nearpos1;

	D3DXVec3Normalize(&ray1, &ray1);

	D3DXVECTOR3 farpos2;
	D3DXVECTOR3 ray2;

	ray2 = AimPos - nearpos1;//�ړI�̈ʒu�ƃJ�����̈ʒu�̃��C�����߂�

	D3DXVec3Normalize(&ray2, &ray2);

	CManager::GetDebugProc()->PrintDebugProc("ray1:%f %f %f\n", ray1.x, ray1.y, ray1.z);
	CManager::GetDebugProc()->PrintDebugProc("ray2:%f %f %f\n", ray2.x, ray2.y, ray2.z);

	if (ray1.x >= ray2.x - 0.05f && ray1.x <= ray2.x + 0.05f &&
		ray1.y >= ray2.y - 0.05f && ray1.y <= ray2.y + 0.05f &&
		ray1.z >= ray2.z - 0.05f && ray1.z <= ray2.z + 0.05f)
	{
		CManager::GetDebugProc()->PrintDebugProc("���C�̕�������v\n");
		return true;
	}
	return false;
}
//===========================================================================================================

//=========================================================
//�_�ƕӂ̋��������߂�
//=========================================================
float CCalculation::pointLineDistance(float cx, float cy, float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;

	// �~�̒��S����ӂ̒[�_�܂ł̃x�N�g�����v�Z
	float t = ((cx - x1) * dx + (cy - y1) * dy) / (dx * dx + dy * dy);

	// t�͈̔͂�[0, 1]�ɐ������A�~�̒��S�ɍł��߂��_��������
	t = (std::max)(0.0f, (std::min)(1.0f, t));

	// �ŒZ�������v�Z
	float closestX = x1 + t * dx;
	float closestY = y1 + t * dy;
	float distanceX = cx - closestX;
	float distanceY = cy - closestY;

	return std::sqrt(distanceX * distanceX + distanceY * distanceY);
}
//===========================================================================================================

//=========================================================
//�����`�Ɖ~�̓����蔻��i2D)
//=========================================================
bool CCalculation::checkCollisionCircleRectangle(float cx, float cy, float r, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	// �l�p�`�̊e�ӂɑ΂��ĉ~�Ƃ̋������v�Z
	if (pointLineDistance(cx, cy, x1, y1, x2, y2) <= r) return true;
	if (pointLineDistance(cx, cy, x2, y2, x3, y3) <= r) return true;
	if (pointLineDistance(cx, cy, x3, y3, x4, y4) <= r) return true;
	if (pointLineDistance(cx, cy, x4, y4, x1, y1) <= r) return true;

	// �ǂ̕ӂƂ��������Ă��Ȃ��ꍇ��false
	return false;
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