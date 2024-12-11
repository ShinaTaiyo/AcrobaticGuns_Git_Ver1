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
CPlayer::CPlayer(CPlayerMove* pPlayerMove, CPlayerAttack* pPlayerAttack, CPlayerEffect* pPlayerEffect, CPlayerWireShot* pPlayerWireShot,
    int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectXAlive(nPri, bUseintPri, type, ObjType)
    , m_pMove(pPlayerMove), m_pAttack(pPlayerAttack), m_pEffect(pPlayerEffect), m_pWireShot(pPlayerWireShot),
    m_pMeshOrbit(nullptr),
    m_fRotAim(0.0f), m_pLockOn(nullptr), m_NowActionMode(ACTIONMODE::SHOT), m_pModeDisp(nullptr), m_bCollision(false),m_pWire(nullptr)
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
    m_pLockOn->SetPolygonRotSpeed(0.01f);

    m_pMeshOrbit = CMeshOrbit::Create(CMeshOrbit::MESHORBITTYPE::DEATHENAGA);
    m_pMeshOrbit->SetUseDeath(true);

    m_pModeDisp = CUi::Create(CUi::UITYPE::ACTIONMODE_GUN, CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

    m_pWire = CWire::Create(CWire::WIRETYPE::NORMAL, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15.0f, 20.0f, 4, 5);
    m_pWire->SetUseDeath(false);
    m_pWire->SetPlayerPointer(this);//�v���C���[�̃|�C���^��ݒ�
    m_pWire->SetUseDraw(false);
    return S_OK;
}
//==========================================================================================================

//====================================================
//�I������
//====================================================
void CPlayer::Uninit()
{
    CObjectXAlive::Uninit();//X�I�u�W�F�N�g�I��

    if (m_pMove != nullptr)
    {
        delete m_pMove;
        m_pMove = nullptr;
    }

    if (m_pAttack != nullptr)
    {
        delete m_pAttack;
        m_pAttack = nullptr;
    }

    if (m_pEffect != nullptr)
    {
        delete m_pEffect;
        m_pEffect = nullptr;
    }

    if (m_pWireShot != nullptr)
    {
        delete m_pWireShot;
        m_pWireShot = nullptr;
    }
}
//==========================================================================================================

//====================================================
//�X�V����
//====================================================
void CPlayer::Update()
{
    m_pMove->MoveProcess(this);//���݂̃A�N�V�������[�h�̈ړ����������s

    AdjustRot();//������������

    ActionModeChenge(); //���݂̃A�N�V�������[�h��ύX����

    CObjectXAlive::Update();//�X�V����

    AdjustPos();//�ʒu��������

    CollisionProcess();

    m_pAttack->AttackProcess(this);//���݂̃A�N�V�������[�h�̍U������������

    m_pEffect->EffectProcess(this);//�G�t�F�N�g����

    m_pWireShot->WireShotProcess(this);//���C���[���ˏ�ԏ���

    CManager::GetDebugProc()->PrintDebugProc("�v���C���[�̈ʒu�F%f %f %f\n", GetPos().x, GetPos().y, GetPos().z);

    CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 1, 60, 5.0f, 5.0f, 100, 10, false, GetPos() + GetVtxMax(), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
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

    if (m_pModeDisp != nullptr)
    {
        m_pModeDisp->SetUseDeath(true);
        m_pModeDisp->SetDeath();
        m_pModeDisp = nullptr;
    }

    if (m_pMeshOrbit != nullptr)
    {
        m_pMeshOrbit->SetUseDeath(true);
        m_pMeshOrbit->SetDeath();
        m_pMeshOrbit = nullptr;
    }

    if (m_pWire != nullptr)
    {
        m_pWire->SetUseDeath(true);
        m_pWire->SetDeath();
        m_pWire = nullptr;
    }

    CObject::SetDeath();
}
//===========================================================================================================

//====================================================
//�v���C���[�̐���
//====================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
    CPlayer* pPlayer = DBG_NEW CPlayer(DBG_NEW CPlayerMove_Normal(),DBG_NEW CPlayerAttack_Shot(),DBG_NEW CPlayerEffect(),DBG_NEW CPlayerWireShot_Dont());//�v���C���[�𐶐�

    bool bSuccess = pPlayer->CObject::GetCreateSuccess();
    int nIdx = 0;//�e�N�X�`���̃C���f�b�N�X
    if (bSuccess == true)
    {
        if (pPlayer != nullptr)
        {
            pPlayer->Init();                                                                 //����������
            pPlayer->SetMove(move);//�ړ���
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
            pPlayer->SetAutoDeath(false);
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
//�U���J�n
//========================================================
void CPlayer::ActionModeChenge()
{
    if (CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::X) == true)
    {
        if (m_pModeDisp != nullptr)
        {
            m_pModeDisp->SetUseDeath(true);
            m_pModeDisp->SetDeath();
            m_pModeDisp = nullptr;
        }

        //���[�h��؂�ւ���
        if (m_NowActionMode == ACTIONMODE::SHOT)
        {//�V���b�g���_�C�u
            m_NowActionMode = ACTIONMODE::DIVE;
        }
        else
        {//�_�C�u���V���b�g
            m_NowActionMode = ACTIONMODE::SHOT;
            SetUseInteria(true, CObjectXMove::GetNormalInertia());
            SetUseGravity(true, GetNormalGravity());
        }

        //���[�h����
        switch (m_NowActionMode)
        {
        case ACTIONMODE::SHOT://���˃��[�h
            //m_pActionMode = DBG_NEW CPlayerShot;
            ChengeMoveMode(DBG_NEW CPlayerMove_Normal()); //�ʏ�ړ����[�h�ɂ���
            ChengeAttackMode(DBG_NEW CPlayerAttack_Shot()); //�U���\���[�h�ɂ���
            ChengeEffectMode(DBG_NEW CPlayerEffect_None()); //�G�t�F�N�g�Ȃ����[�h�ɂ���
            ChengeWireShotMode(DBG_NEW CPlayerWireShot_Dont);//���C���[���ˏ�Ԃ��I�t�ɂ���
            m_pModeDisp = CUi::Create(CUi::UITYPE::ACTIONMODE_GUN, CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        case ACTIONMODE::DIVE://�_�C�u���[�h
            //m_pActionMode = DBG_NEW CPlayerDive;
            ChengeMoveMode(DBG_NEW CPlayerMove_PrepDive());//�_�C�u�������[�h�ɂ���
            ChengeAttackMode(DBG_NEW CPlayerAttack_Dont);  //�U���s�\���[�h�ɂ���
            ChengeEffectMode(DBG_NEW CPlayerEffect_None()); //�G�t�F�N�g�Ȃ����[�h�ɂ���
            ChengeWireShotMode(DBG_NEW CPlayerWireShot_Dont);//���C���[���ˏ�Ԃ��I�t�ɂ���
            m_pModeDisp = CUi::Create(CUi::UITYPE::ACTIONMODE_DIVE, CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        default:
            break;
        }


    }
}
//==========================================================================================================

//========================================================
//�ړ����[�h���`�F���W
//========================================================
void CPlayer::ChengeMoveMode(CPlayerMove* pPlayerMove)
{
    if (m_pMove != nullptr)
    {
        delete m_pMove;
        m_pMove = nullptr;

        m_pMove = pPlayerMove;
    }
}
//==========================================================================================================

//========================================================
//�U�����[�h���`�F���W
//========================================================
void CPlayer::ChengeAttackMode(CPlayerAttack* pPlayerAttack)
{
    if (m_pAttack != nullptr)
    {
        delete m_pAttack;
        m_pAttack = nullptr;

        m_pAttack = pPlayerAttack;
    }
}
//==========================================================================================================

//========================================================
//�G�t�F�N�g���[�h���`�F���W
//========================================================
void CPlayer::ChengeEffectMode(CPlayerEffect* pPlayerEffect)
{
    if (m_pEffect != nullptr)
    {
        delete m_pEffect;
        m_pEffect = nullptr;

        m_pEffect = pPlayerEffect;
    }
}


//========================================================
//���C���[���˃��[�h���`�F���W
//========================================================
void CPlayer::ChengeWireShotMode(CPlayerWireShot* pPlayerWireShot)
{
    if (m_pWireShot != nullptr)
    {
        delete m_pWireShot;
        m_pWireShot = nullptr;

        m_pWireShot = pPlayerWireShot;
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
    bool bIsLanding = false;

    m_bCollision = false;//�����Ԃ����Z�b�g
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
                    ComPos, ComVtxMax, ComVtxMin, bCollisionXOld, bCollisionYOld, bCollisionZOld, bIsLanding);

                if (bSuccessCollision == true)
                {
                    SetPos(MyPos);
                    m_bCollision = true;
                }

                if (bCollisionY == true)
                {
                    if (bIsLanding == true)
                    {
                        SetMove(D3DXVECTOR3(GetMove().x, -0.1f, GetMove().z));
                        SetIsLanding(true);
                    }
                }

            }

            //�I�u�W�F�N�g�����ɐi�߂�
            pObj = pNext;
        }

    }

    SetExtrusionCollisionSquareX(bCollisionX);
    SetExtrusionCollisionSquareY(bCollisionY);
    SetExtrusionCollisionSquareZ(bCollisionZ);
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
//�W�����v����
//========================================================
void CPlayer::JumpProcess()
{
    //
}
//==========================================================================================================

//========================================================
//������������
//========================================================
void CPlayer::AdjustRot()
{
    //D3DXVECTOR3& Rot = GetRot();
    const D3DXVECTOR3& CameraRot = CManager::GetCamera()->GetRot();
    SetRot(D3DXVECTOR3(GetRot().x,D3DX_PI + CameraRot.y,GetRot().z));

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