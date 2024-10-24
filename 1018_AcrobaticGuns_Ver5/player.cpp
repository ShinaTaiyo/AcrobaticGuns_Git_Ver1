//=============================================================================
//
//�V���S���F����A�N�V��������[player.cpp]
//Author:ShinaTaiyo
//
//=============================================================================

//========================
//�C���N���[�h
//========================
#include "player.h"
#include "objectX.h"
#include "renderer.h"
#include "attack.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "objectXInfo.h"
#include "calculation.h"
#include "fade.h"
#include "game.h"
#include "debugproc.h"
#include "collision.h"
//==========================================================================================================

//====================================================
//�ÓI�����o�錾
//====================================================

//==========================================================================================================

//====================================================
//�R���X�g���N�^
//====================================================
CPlayer::CPlayer() : m_fRotAim(0.0f)
{

}
//==========================================================================================================

//====================================================
//�f�X�g���N�^
//====================================================
CPlayer::~CPlayer()
{

}
//==========================================================================================================

//====================================================
//����������
//====================================================
HRESULT CPlayer::Init()
{
    CObjectXMove::Init();                 //X�I�u�W�F�N�g������

    return S_OK;
}
//==========================================================================================================

//====================================================
//�I������
//====================================================
void CPlayer::Uninit()
{
    CObjectXMove::Uninit();//X�I�u�W�F�N�g�I��
}
//==========================================================================================================

//====================================================
//�ʘg�̏I������
//====================================================
void CPlayer::ExtraUninit()
{
}
//==========================================================================================================

//====================================================
//�X�V����
//====================================================
void CPlayer::Update()
{
    MoveProcess();

    NormalAttackProcess();

    CObjectXMove::Update();
}
//==========================================================================================================

//====================================================
//�`�揈��
//====================================================
void CPlayer::Draw()
{
    CObjectXMove::Draw();
}
//==========================================================================================================

//====================================================
//���S�t���O��ݒ�
//====================================================
void CPlayer::SetDeath()
{
    CObject::SetDeath();
}
//===========================================================================================================

//====================================================
//���f�����̔j��
//====================================================
void CPlayer::Unload()
{

}
//==========================================================================================================

//====================================================
//�v���C���[�̐���
//====================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
    CPlayer* pPlayer = new CPlayer;                                                                             //�v���C���[�𐶐�

    bool bSuccess = pPlayer->CObject::GetCreateSuccess();
    int nIdx = 0;//�e�N�X�`���̃C���f�b�N�X
    if (bSuccess == true)
    {
        if (pPlayer != nullptr)
        {
            pPlayer->Init();                                                                 //����������
            pPlayer->CObject::SetType(CObject::TYPE_PLAYER);                                 //�I�u�W�F�N�g�̎�ނ����߂�
            pPlayer->CObjectXMove::SetObjXType(CObjectXMove::OBJECTXTYPE_PLAYER);                    //�I�u�W�F�N�gX�̃^�C�v��ݒ�
            pPlayer->CObjectXMove::SetTypeNum(0);                                                //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
            pPlayer->SetLife(1);
            pPlayer->SetMaxLife(1);
            pPlayer->SetAutoSubLife(false);//�����I�ɑ̗͂����炷���ǂ���
            //pPlayer->SetUseGravity(true,1.0f);//�d��
            nIdx = CManager::GetObjectXInfo()->Regist("data\\MODEL\\Player\\Player_ProtoType.x");
            pPlayer->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
                CManager::GetObjectXInfo()->GetBuffMat(nIdx),
                CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
                CManager::GetObjectXInfo()->GetTexture(nIdx),
                CManager::GetObjectXInfo()->GetColorValue(nIdx));           
            //���f���������蓖�Ă�
            pPlayer->SetSize();
            pPlayer->SetPos(pos);                                                            //�ʒu�̐ݒ�
            pPlayer->SetPosOld(pos);                                                         //1f�O�̈ʒu��ݒ�
            pPlayer->SetSupportPos(pos);                                                     //�ݒu�ʒu
            pPlayer->SetRot(rot);                                                            //�����̐ݒ�
            pPlayer->SetScale(Scale);                                                        //�g�嗦�̐ݒ�
            pPlayer->SetFormarScale(Scale);                                                               //���̊g�嗦��ݒ肷��
        }
    }
    else
    {//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
        delete pPlayer;
        pPlayer = nullptr;
    }

	return pPlayer;
}
//==========================================================================================================

//========================================================
//�ړ�����
//========================================================
void CPlayer::MoveProcess()
{
    const D3DXVECTOR3& Rot = GetRot();
    D3DXVECTOR3 CalRot = Rot;
    float fRotDiff = 0.0f;//�����̍���
    const D3DXVECTOR3 & Move = GetMove();
    D3DXVECTOR3 AddMove = NULL_VECTOR3;
    bool bMove = false;//�ړ����Ă��邩�ǂ���
    bMove = CCalculation::CaluclationMove(AddMove, 10.0f, CCalculation::MOVEAIM_XZ,m_fRotAim);
    CCalculation::CalculationCollectionRot2D(CalRot.y, m_fRotAim, 0.25f);
    
    SetMove(AddMove + D3DXVECTOR3(0.0f,Move.y,0.0f));
    SetRot(CalRot);
    CManager::GetDebugProc()->PrintDebugProc("�����F%f\n",Rot.y);
    CManager::GetDebugProc()->PrintDebugProc("�ړI�̌����F%f\n", m_fRotAim);
}
//==========================================================================================================

//========================================================
//�U������
//========================================================
void CPlayer::AttackProcress()
{

}
//==========================================================================================================

//========================================================
//�ʏ�U������
//========================================================
void CPlayer::NormalAttackProcess()
{
    const D3DXVECTOR3 & Rot = GetRot();
    const D3DXVECTOR3& Pos = GetPos();
    CAttackPlayer* pAttackPlayer = nullptr;
    if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true)
    {
        pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE::TYPE00_BULLET, 60, Pos, Rot,
            D3DXVECTOR3(sinf(Rot.y) * m_fNORMALATTACK_SPEED, 0.0f, cosf(Rot.y) * m_fNORMALATTACK_SPEED),ONE_VECTOR3);

        pAttackPlayer->SetUseInteria(false);
        pAttackPlayer->SetAutoSubLife(true);
    }
}
//==========================================================================================================