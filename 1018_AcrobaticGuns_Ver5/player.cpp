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
#include "objectXInfo.h"
#include "calculation.h"
#include "fade.h"
#include "game.h"
#include "block.h"
#include "lockon.h"
#include "particle.h"
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
CPlayer::CPlayer() : m_fRotAim(0.0f),m_pLockOn(nullptr)
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
    CObjectXAlive::Init();                 //X�I�u�W�F�N�g������

    m_pLockOn = CLockon::Create(SENTER_VECTOR3, CObject2D::POLYGONTYPE01_SENTERROLLING, 100.0f, 100.0f, NORMAL_COL);
    m_pLockOn->SetUseDeath(true);

    return S_OK;
}
//==========================================================================================================

//====================================================
//�I������
//====================================================
void CPlayer::Uninit()
{
    CObjectXAlive::Uninit();//X�I�u�W�F�N�g�I��
}
//==========================================================================================================

//====================================================
//�X�V����
//====================================================
void CPlayer::Update()
{
    //�ړ�����
    MoveProcess();

    //�X�V����
    CObjectXAlive::Update();

    //�ʏ�U������
    NormalAttackProcess();

    //���b�N�I���̏���
    LockOnProcess();

    //�u���b�N�Ƃ̓����蔻��
    CollisionBlock();
}
//==========================================================================================================

//====================================================
//�`�揈��
//====================================================
void CPlayer::Draw()
{
    CObjectXAlive::Draw();
}
//==========================================================================================================

//====================================================
//���S�t���O��ݒ�
//====================================================
void CPlayer::SetDeath()
{
    if (m_pLockOn != nullptr)
    {
        m_pLockOn->SetUseDeath(true);
        m_pLockOn->SetDeath();
        m_pLockOn = nullptr;
    }

    CObject::SetDeath();
}
//===========================================================================================================

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
    const D3DXVECTOR3& Pos = GetPos();
    const D3DXVECTOR3& Rot = GetRot();
    D3DXVECTOR3 CalRot = Rot;
    float fRotDiff = 0.0f;//�����̍���
    const D3DXVECTOR3 & Move = GetMove();
    D3DXVECTOR3 AddMove = NULL_VECTOR3;
    bool bMove = false;//�ړ����Ă��邩�ǂ���
    bMove = CCalculation::CaluclationMove(true,AddMove, 10.0f, CCalculation::MOVEAIM_XZ,m_fRotAim);
    CCalculation::CalculationCollectionRot2D(CalRot.y, m_fRotAim, 0.25f);
    
    //CManager::GetInputJoypad()->GetLStickPress();

    SetMove(AddMove + D3DXVECTOR3(0.0f,Move.y,0.0f));
    SetRot(CalRot);
    CManager::GetDebugProc()->PrintDebugProc("�v���C���[�̈ʒu�F%f %f %f\n",Pos.x,Pos.y,Pos.z);
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
    bool bCollision = false;//�����蔻��
    D3DXVECTOR3 Move = NULL_VECTOR3;
    D3DXVECTOR3 ShotPos = GetPos() + D3DXVECTOR3(0.0f, GetSize().y, 0.0f);
    D3DXVECTOR3 NearPos = NULL_VECTOR3;//��O
    D3DXVECTOR3 FarPos = NULL_VECTOR3; //��
    D3DXVECTOR3 Ray = NULL_VECTOR3;    //���C�̕���
    D3DXVECTOR3 CollisionStartPos = NULL_VECTOR3;//�Փ˔���J�n�ʒu
    D3DXVECTOR3 CollisionEndPos = NULL_VECTOR3;  //�Փ˔���I���ʒu

    //�J�����̎�O���̃��[���h���W�����߂�
    CCalculation::CalcScreenToWorld(&NearPos, int(m_pLockOn->GetPos().x), int(m_pLockOn->GetPos().y), 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
        CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());

    //�J�����̉����̃��[���h���W�����߂�
    CCalculation::CalcScreenToWorld(&FarPos, int(m_pLockOn->GetPos().x), int(m_pLockOn->GetPos().y), 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
        CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());

    //���C�����߂�
    Ray = FarPos - NearPos;

    //���C�𐳋K������
    D3DXVec3Normalize(&Ray, &Ray);

    //�w�肵�����f���̈ʒu
    bCollision = CCalculation::CalcRaySphere(NearPos, Ray, NULL_VECTOR3, 100.0f, CollisionStartPos, CollisionEndPos);

    if (bCollision == true)
    {//�_���Ă�������̉����̕ǂɌ������Č���
        Move = CCalculation::Calculation3DVec(ShotPos,NULL_VECTOR3, 40.0f);
        CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 12, 60, 30.0f, 30.0f, 100, 10, false, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
        CManager::GetDebugProc()->PrintDebugProc("���C�Ƌ��̓����蔻�萬���I\n");
    }
    else
    {//���C�̕�������v�����I�u�W�F�N�g�̒��S�_�Ɍ������Č���
        Move = CCalculation::Calculation3DVec(ShotPos, m_pLockOn->GetLockOnPos(), 10.0f);
        //����Ώۂ̈ʒu�Ƀp�[�e�B�N��������
        CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 2, 60, 30.0f, 30.0f, 100, 10, false, NULL_VECTOR3, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
        CManager::GetDebugProc()->PrintDebugProc("���C�Ƌ��̓����蔻�莸�s�I\n");
    }

    CAttackPlayer* pAttackPlayer = nullptr;//�v���C���[�U���ւ̃|�C���^

    if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true || CManager::GetInputJoypad()->GetRT_Repeat(6) == true)
    {
        pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE::TYPE00_BULLET, 60, ShotPos, Rot, Move,ONE_VECTOR3);
        pAttackPlayer->SetUseInteria(false);
        pAttackPlayer->SetAutoSubLife(true);
    }
}
//==========================================================================================================

//========================================================
//�u���b�N�Ƃ̓����蔻��
//========================================================
void CPlayer::CollisionBlock()
{
    D3DXVECTOR3 MyPos = GetPos();
    D3DXVECTOR3 MyPosOld = GetPosOld();
    D3DXVECTOR3 MyVtxMax = GetVtxMax();
    D3DXVECTOR3 MyVtxMin = GetVtxMin();
    const D3DXVECTOR3 Move = GetMove();
    bool bCollisionXOld = GetExtrusionCollisionSquareX();
    bool bCollisionYOld = GetExtrusionCollisionSquareY();
    bool bCollisionZOld = GetExtrusionCollisionSquareZ();

    bool bCollisionX = false;
    bool bCollisionY = false;
    bool bCollisionZ = false;

    bool bSuccessCollision = false;//�����蔻�肪�����������ǂ���
    for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
    {
        CObject* pObj = CObject::GetTopObject(nCntPri);

        while (pObj != nullptr)
        {
            //���̃I�u�W�F�N�g���i�[
            CObject* pNext = pObj->GetNextObject();

            //��ނ̎擾�i�G�Ȃ瓖���蔻��j
            CObject::TYPE type = pObj->GetType();

            if (type == CObject::TYPE_BLOCK)
            {
                D3DXVECTOR3 ComPos = ((CBlock*)pObj)->GetPos();
                D3DXVECTOR3 ComVtxMax = ((CBlock*)pObj)->GetVtxMax();
                D3DXVECTOR3 ComVtxMin = ((CBlock*)pObj)->GetVtxMin();

                bSuccessCollision = CCollision::ExtrusionCollisionSquare(MyPos, bCollisionX, bCollisionY, bCollisionZ, Move, MyPosOld, MyVtxMax, MyVtxMin,
                    ComPos, ComVtxMax, ComVtxMin,bCollisionXOld,bCollisionYOld,bCollisionZOld);

                if (bSuccessCollision == true)

                {
                    SetPos(MyPos);
                    SetExtrusionCollisionSquareX(bCollisionX);
                    SetExtrusionCollisionSquareY(bCollisionY);
                    SetExtrusionCollisionSquareZ(bCollisionZ);
                }
            }

            //�I�u�W�F�N�g�����ɐi�߂�
            pObj = pNext;
        }

    }

    CManager::GetDebugProc()->PrintDebugProc("X�����̓����蔻�肪�������Ă��邩�ǂ����i�O�F�������A�P�F�͂��j�F%d\n", bCollisionX);
    CManager::GetDebugProc()->PrintDebugProc("Y�����̓����蔻�肪�������Ă��邩�ǂ����i�O�F�������A�P�F�͂��j�F%d\n", bCollisionY);
    CManager::GetDebugProc()->PrintDebugProc("Z�����̓����蔻�肪�������Ă��邩�ǂ����i�O�F�������A�P�F�͂��j�F%d\n", bCollisionZ);
}
//==========================================================================================================

//========================================================
//���b�N�I���̏���
//========================================================
void CPlayer::LockOnProcess()
{
    //�ړ�����
    LockOnMove();
}
//==========================================================================================================

//========================================================
//���b�N�I���𓮂���
//========================================================
void CPlayer::LockOnMove()
{

}
//==========================================================================================================