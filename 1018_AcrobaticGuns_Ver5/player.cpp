//=============================================================================
//
//�P�P���P�O���F�_�C�u����[player.cpp]
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
#include "enemy.h"
#include "lockon.h"
#include "particle.h"
#include "ui.h"
#include "particle2d.h"
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
CPlayer::CPlayer(CPlayer_ActionMode* pPlayer_ActionMode) : m_fRotAim(0.0f),m_pLockOn(nullptr),m_pActionMode(pPlayer_ActionMode),m_NowActionMode(ACTIONMODE::SHOT)
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

    SetAutoSubLife(false);//�����I�ɑ̗͂����炷���ǂ���
    SetUseGravity(true,1.0f);  //�d�͂��g�p����

    m_pLockOn = CLockon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f), CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
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
    MoveProcess();//�ړ�����

    AdjustRot();//������������

    m_pActionMode->Move(this);//���݂̃A�N�V�������[�h�̈ړ����������s

    ActionModeChenge(); //���݂̃A�N�V�������[�h��ύX����

    CObjectXAlive::Update();//�X�V����

    AdjustPos();//�ʒu��������

    CollisionProcess();//�����蔻��S�ʏ���

    m_pActionMode->Attack(this);//���݂̃A�N�V�������[�h�̍U������������

    //m_PosR = CGame::GetPlayer()->GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f) + m_AddPosR;
    //m_PosV = m_PosR + D3DXVECTOR3(sinf(m_Rot.y) * -200.0f, 0.0f, cosf(m_Rot.y) * -200.0f);
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
    //���b�N�I��
    if (m_pLockOn != nullptr)
    {
        m_pLockOn->SetUseDeath(true);
        m_pLockOn->SetDeath();
        m_pLockOn = nullptr;
    }

    //�s�����[�h
    if (m_pActionMode != nullptr)
    {
        m_pActionMode->SetUseDeath(true);
        m_pActionMode->SetDeath();
        m_pActionMode = nullptr;
    }

    CObject::SetDeath();
}
//===========================================================================================================

//====================================================
//�v���C���[�̐���
//====================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
    CPlayer* pPlayer = new CPlayer(CPlayerShot::Create(CObject2D::POLYGONTYPE::SENTERROLLING,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),D3DXVECTOR3(100.0f,100.0f,0.0f),100.0f,100.0f));                                                                             //�v���C���[�𐶐�

    bool bSuccess = pPlayer->CObject::GetCreateSuccess();
    int nIdx = 0;//�e�N�X�`���̃C���f�b�N�X
    if (bSuccess == true)
    {
        if (pPlayer != nullptr)
        {
            pPlayer->Init();                                                                 //����������
            pPlayer->CObject::SetType(CObject::TYPE::PLAYER);                                 //�I�u�W�F�N�g�̎�ނ����߂�
            pPlayer->CObjectXMove::SetObjXType(CObjectXMove::OBJECTXTYPE_PLAYER);                    //�I�u�W�F�N�gX�̃^�C�v��ݒ�
            pPlayer->CObjectXMove::SetTypeNum(0);                                                //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
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

            //�J���������ݒ�i�v���C���[��Ȃ̂Ńv���C���[����ݒ�j
            //CCamera* pCamera = CManager::GetCamera();
            //pCamera->SetPosR(pPlayer->GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f));
            //pCamera->SetPosV(pCamera->GetPosR() + D3DXVECTOR3(sinf(pPlayer->GetRot().y) * 200.0f, 0.0f, cosf(pPlayer->GetRot().y) * 200.0f));

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
    float fRotDiff = 0.0f;//�����̍���
    const D3DXVECTOR3 & Move = GetMove();
    D3DXVECTOR3 AddMove = D3DXVECTOR3(0.0f,0.0f,0.0f);
    bool bMove = false;//�ړ����Ă��邩�ǂ���
    bMove = CCalculation::CaluclationMove(true,AddMove, 10.0f, CCalculation::MOVEAIM_XZ,m_fRotAim);
    //CCalculation::CalculationCollectionRot2D(CalRot.y, m_fRotAim, 0.25f);
    
    //CManager::GetInputJoypad()->GetLStickPress();
    if (bMove == true)
    {
        SetMove(AddMove + D3DXVECTOR3(0.0f, Move.y, 0.0f));
    }
    CManager::GetDebugProc()->PrintDebugProc("�v���C���[�̈ʒu�F%f %f %f\n",Pos.x,Pos.y,Pos.z);
    CManager::GetDebugProc()->PrintDebugProc("�ړI�̌����F%f\n", m_fRotAim);
}
//==========================================================================================================

//========================================================
//�U���J�n
//========================================================
void CPlayer::ActionModeChenge()
{
    if (CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::X) == true)
    {

        if (m_pActionMode != nullptr)
        {
            m_pActionMode->SetUseDeath(true);//���S�t���O���g�p����
            m_pActionMode->SetDeath();       //���S�t���O��ݒ肷��
            m_pActionMode = nullptr;         //�|�C���^��������
        }

        //���[�h��؂�ւ���
        if (m_NowActionMode == ACTIONMODE::SHOT)
        {//�V���b�g���_�C�u
            m_NowActionMode = ACTIONMODE::DIVE;
            SetUseInteria(false);
            SetUseGravity(false,GetNormalGravity());
        }
        else
        {//�_�C�u���V���b�g
            m_NowActionMode = ACTIONMODE::SHOT;
            SetUseInteria(true);
            SetUseGravity(true, GetNormalGravity());
        }

        //���[�h����
        switch (m_NowActionMode)
        {
        case ACTIONMODE::SHOT:
            m_pActionMode = CPlayerShot::Create(CObject2D::POLYGONTYPE::SENTERROLLING, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 100.0f, 100.0f);
            break;
        case ACTIONMODE::DIVE:
            m_pActionMode = CPlayerDive::Create(CObject2D::POLYGONTYPE::SENTERROLLING, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 100.0f, 100.0f);
            break;
        default:
            break;
        }
        m_pActionMode->SetUseDeath(false);


    }
}
//==========================================================================================================

//========================================================
//�����蔻��S�ʏ���
//========================================================
void CPlayer::CollisionProcess()
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

            if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
            {
                D3DXVECTOR3 ComPos = static_cast<CObjectX*>(pObj)->GetPos();
                D3DXVECTOR3 ComVtxMax = static_cast<CObjectX*>(pObj)->GetVtxMax();
                D3DXVECTOR3 ComVtxMin = static_cast<CObjectX*>(pObj)->GetVtxMin();

                bSuccessCollision = CCollision::ExtrusionCollisionSquare(MyPos, bCollisionX, bCollisionY, bCollisionZ, Move, MyPosOld, MyVtxMax, MyVtxMin,
                    ComPos, ComVtxMax, ComVtxMin, bCollisionXOld, bCollisionYOld, bCollisionZOld);

                if (bCollisionY == true)
                {
                    SetMove(D3DXVECTOR3(GetMove().x, 0.0f, GetMove().z));
                }

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
}
//==========================================================================================================

//========================================================
//�u���b�N�Ƃ̓����蔻��
//========================================================
void CPlayer::CollisionBlock()
{
}
//==========================================================================================================

//========================================================
//������������
//========================================================
void CPlayer::AdjustRot()
{
    D3DXVECTOR3& Rot = GetRot();
    const D3DXVECTOR3& CameraRot = CManager::GetCamera()->GetRot();
    SetRot(D3DXVECTOR3(0.0f,D3DX_PI + CameraRot.y,0.0f));

    //CCamera* pCaemra = CManager::GetCamera();
    //if (m_pLockOn->GetEndState() == CLockon::ENDSTATE::RIGHTEND)
    //{

    //}
    //SetRot(D3DXVECTOR3(0.0f, D3DX_PI + CameraRot.y, 0.0f));

    //SetAxis(CameraRot.y + D3DX_PI);
}
//==========================================================================================================

//========================================================
//�ʒu��������
//========================================================
void CPlayer::AdjustPos()
{
    const D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3 ScreenPos = CCalculation::CalcWorldToScreenNoViewport(Pos, *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
        static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));

    //��ʊO�ɏo�Ȃ��悤�ɕ␳
    if (ScreenPos.x > SCREEN_WIDTH || ScreenPos.x < 0.0f)
    {
        SetPos(GetPosOld());
    }
}
//==========================================================================================================