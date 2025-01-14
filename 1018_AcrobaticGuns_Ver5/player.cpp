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
#include "text.h"
#include "game.h"
#include "block.h"
#include "enemy.h"
#include "lockon.h"
#include "particle.h"
#include "ui.h"
#include "particle2d.h"
#include "debugtext.h"
#include "tutorial.h"
#include "collision.h"
//==========================================================================================================

//====================================================
//�ÓI�����o�錾
//====================================================
const int CPlayer::s_nNORMAL_MAXLIFE = 100;
//==========================================================================================================

//====================================================
//�R���X�g���N�^
//====================================================
CPlayer::CPlayer(CPlayerMove* pPlayerMove, CPlayerAttack* pPlayerAttack, CPlayerEffect* pPlayerEffect, CPlayerWireShot* pPlayerWireShot,
    int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CCharacter(nPri, bUseintPri, type, ObjType)
    , m_pMove(pPlayerMove), m_pAttack(pPlayerAttack), m_pEffect(pPlayerEffect), m_pWireShot(pPlayerWireShot),
    m_pMeshOrbit(nullptr),
    m_fRotAim(0.0f), m_pLockOn(nullptr), m_NowActionMode(ACTIONMODE::SHOT), m_pModeDisp(nullptr), m_bCollision(false),m_pWire(nullptr),
    m_pHpGauge(nullptr),m_pAbnormalState(DBG_NEW CPlayerAbnormalState()),m_pDiveGauge(nullptr), m_pDivePossibleNum(nullptr),m_bDamage(false)
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
    CCharacter::Init();                 //X�I�u�W�F�N�g������

    GetLifeInfo().SetAutoSubLife(false);//�����I�ɑ̗͂����炷���ǂ���
    GetMoveInfo().SetUseGravity(true,1.0f);  //�d�͂��g�p����

    if (CScene::GetMode() == CScene::MODE::MODE_GAME)
    {
        m_pLockOn = CLockon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pLockOn->SetUseDeath(false);
        m_pLockOn->SetPolygonRotSpeed(0.01f);

        m_pMeshOrbit = CMeshOrbit::Create(CMeshOrbit::MESHORBITTYPE::DEATHENAGA);
        m_pMeshOrbit->SetUseDeath(false);

        m_pModeDisp = CUi::Create(CUi::UITYPE::ACTIONMODE_GUN, CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pModeDisp->SetUseDeath(false);

        m_pDiveGauge = CGauge::Create(CGauge::GAUGETYPE::DIVE, 20, 200.0f, 25.0f, D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 200.0f, 0.0f));
        m_pDiveGauge->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), false, 1.0f);
        m_pDiveGauge->SetUseDeath(false);
        m_pDiveGauge->SetParam(0);//�����l����X�^�[�g
        m_pDiveGauge->SetPolygonType(CObject2D::POLYGONTYPE::LEFT);

        m_pDivePossibleNum = CUi::Create(CUi::UITYPE::POSSIBLEDIVENUMTEXT_000, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 1, false, D3DXVECTOR3(200.0f, 100.0f, 0.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pDivePossibleNum->SetNumericState(0, 50.0f, 50.0f);
        m_pDivePossibleNum->SetUseDeath(false);

        m_pWire = CWire::Create(CWire::WIRETYPE::NORMAL, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15.0f, 20.0f, 4, 5);
        m_pWire->SetUseDeath(false);
        m_pWire->SetPlayerPointer(this);//�v���C���[�̃|�C���^��ݒ�
        m_pWire->SetUseDraw(false);
    }
    return S_OK;
}
//==========================================================================================================

//====================================================
//�I������
//====================================================
void CPlayer::Uninit()
{
    CCharacter::Uninit();//X�I�u�W�F�N�g�I��

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

    if (m_pAbnormalState != nullptr)
    {
        delete m_pAbnormalState;
        m_pAbnormalState = nullptr;
    }

}
//==========================================================================================================

//====================================================
//�X�V����
//====================================================
void CPlayer::Update()
{
    if (CScene::GetMode() == CScene::MODE_GAME)
    {
        if (GetLanding())
        {//�n�ʂɂ���Ȃ�d�͂��ŏ�����
            GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x,0.0f, GetMoveInfo().GetMove().z));
        }

        m_pMove->MoveProcess(this);//���݂̃A�N�V�������[�h�̈ړ����������s

        m_pAbnormalState->Process(this);//��Ԉُ�̏��������s

        AdjustRot();//������������

        ActionModeChenge(); //���݂̃A�N�V�������[�h��ύX����
    }

    CCharacter::Update();//�X�V����

    CollisionProcess();

    if (CScene::GetMode() == CScene::MODE_GAME)
    {
        m_pEffect->EffectProcess(this);//�G�t�F�N�g����

        m_pAttack->AttackProcess(this);//���݂̃A�N�V�������[�h�̍U������������

        DiveGaugeMaxEffect();//�_�C�u�Q�[�W���}�b�N�X�ɂȂ������̉��o

        m_pWireShot->WireShotProcess(this);//���C���[���ˏ�ԏ���

        CManager::GetDebugText()->PrintDebugText("�v���C���[�̈ʒu�F%f %f %f\n", GetPosInfo().GetPos().x, GetPosInfo().GetPos().y, GetPosInfo().GetPos().z);
        CManager::GetDebugText()->PrintDebugText("�v���C���[�̗̑́F%d\n", GetLifeInfo().GetLife());

        if (GetLifeInfo().GetLife() < 1)
        {
            CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
        }
    }

    if (m_bDamage == true)
    {
        SetNextMotion(3);
        m_bDamage = false;
    }

    MotionProcess();//���[�V�����������s��
}
//==========================================================================================================

//====================================================
//�`�揈��
//====================================================
void CPlayer::Draw()
{
    CCharacter::Draw();
}
//==========================================================================================================

//====================================================
//���S�t���O��ݒ�
//====================================================
void CPlayer::SetDeath()
{
    if (GetUseDeath() == true)
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

        if (m_pHpGauge != nullptr)
        {
            m_pHpGauge->SetUseDeath(true);
            m_pHpGauge->SetDeath();
            m_pHpGauge = nullptr;
        }

        if (m_pDiveGauge != nullptr)
        {
            m_pDiveGauge->SetUseDeath(true);
            m_pDiveGauge->SetDeath();
            m_pDiveGauge = nullptr;
        }

        if (m_pDivePossibleNum != nullptr)
        {
            m_pDivePossibleNum->SetUseDeath(true);
            m_pDivePossibleNum->SetDeath();
            m_pDivePossibleNum = nullptr;
        }
    }
    CCharacter::SetDeath();
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
            pPlayer->GetMoveInfo().SetMove(move);//�ړ���
            pPlayer->CObject::SetType(CObject::TYPE::PLAYER);                                 //�I�u�W�F�N�g�̎�ނ����߂�
            pPlayer->SetObjXType(CObjectX::OBJECTXTYPE_PLAYER);                    //�I�u�W�F�N�gX�̃^�C�v��ݒ�
            pPlayer->SetTypeNum(0);                                                //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
            //pPlayer->SetUseGravity(true,1.0f);//�d��
            //���f���������蓖�Ă�
            nIdx = CManager::GetObjectXInfo()->Regist("data\\MODEL\\Enemy\\MotionEnemy\\DiveWeakEnemy\\DiveWeakEnemy00_Source.x");
            pPlayer->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
                CManager::GetObjectXInfo()->GetBuffMat(nIdx),
                CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
                CManager::GetObjectXInfo()->GetTexture(nIdx),
                CManager::GetObjectXInfo()->GetColorValue(nIdx));

            pPlayer->Regist("data\\MODEL\\Enemy\\MotionEnemy\\DiveWeakEnemy\\DiveWeakEnemyMotion.txt",pPlayer);//���[�V�����t�@�C�������蓖�Ă�
            pPlayer->SetSize();
            pPlayer->GetDrawInfo().SetUseDraw(false);                                                     //�`�悵�Ȃ�
            pPlayer->GetPosInfo().SetPos(pos);                                                            //�ʒu�̐ݒ�
            pPlayer->GetPosInfo().SetPosOld(pos);                                                         //1f�O�̈ʒu��ݒ�
            pPlayer->GetPosInfo().SetPosFuture(pos);                                                      //1f��̈ʒu��ݒ�
            pPlayer->GetPosInfo().SetSupportPos(pos);                                                     //�ݒu�ʒu
            pPlayer->GetRotInfo().SetRot(rot);                                                            //�����̐ݒ�
            pPlayer->GetSizeInfo().SetScale(Scale);                                                        //�g�嗦�̐ݒ�
            pPlayer->GetSizeInfo().SetFormarScale(Scale);                                                  //���̊g�嗦��ݒ肷��
            pPlayer->GetLifeInfo().SetAutoDeath(false);                                                    //���S�t���O�������Ŕ������邩�ǂ���
            pPlayer->GetDrawInfo().SetUseShadow(true);
            //�̗�
            pPlayer->GetLifeInfo().SetLife(s_nNORMAL_MAXLIFE);
            pPlayer->GetLifeInfo().SetMaxLife(s_nNORMAL_MAXLIFE);

            if (CScene::GetMode() == CScene::MODE::MODE_GAME)
            {
                pPlayer->m_pHpGauge = CGauge::Create(CGauge::GAUGETYPE::PLAYERHP, s_nNORMAL_MAXLIFE, 600.0f, 50.0f, D3DXVECTOR3(50.0f, SCREEN_HEIGHT - 50.0f, 0.0f));
                pPlayer->m_pHpGauge->SetUseDeath(false);
            }
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
    if (CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::X) || CManager::GetInputMouse()->GetMouseRightClickTrigger())
    {
        if (m_pModeDisp != nullptr)
        {
            m_pModeDisp->SetUseDeath(true);
            m_pModeDisp->SetDeath();
            m_pModeDisp = nullptr;
        }
        CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::MODECHENGE);

        //���[�h��؂�ւ���
        if (m_NowActionMode == ACTIONMODE::SHOT)
        {//�V���b�g���_�C�u
            m_NowActionMode = ACTIONMODE::DIVE;
        }
        else
        {//�_�C�u���V���b�g
            m_NowActionMode = ACTIONMODE::SHOT;
            GetMoveInfo().SetUseInteria(true, GetNormalInertia());
            GetMoveInfo().SetUseGravity(true, GetNormalGravity());
        }

        //���[�h����
        switch (m_NowActionMode)
        {
        case ACTIONMODE::SHOT://���˃��[�h
            ChengeMoveMode(DBG_NEW CPlayerMove_Normal()); //�ʏ�ړ����[�h�ɂ���
            ChengeAttackMode(DBG_NEW CPlayerAttack_Shot()); //�U���\���[�h�ɂ���
            ChengeEffectMode(DBG_NEW CPlayerEffect_None()); //�G�t�F�N�g�Ȃ����[�h�ɂ���
            ChengeWireShotMode(DBG_NEW CPlayerWireShot_Dont);//���C���[���ˏ�Ԃ��I�t�ɂ���
            m_pModeDisp = CUi::Create(CUi::UITYPE::ACTIONMODE_GUN, CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        case ACTIONMODE::DIVE://�_�C�u���[�h
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

        m_pWire->SetUseDraw(false);
        m_pWire->GetWireHead()->GetDrawInfo().SetUseDraw(false);

        m_pModeDisp->SetUseDeath(false);//���S�t���O�𔭓������Ȃ�


    }
}
//==========================================================================================================


//========================================================
//�_�C�u�Q�[�W���}�b�N�X�ɂȂ������̉��o���s��
//========================================================
void CPlayer::DiveGaugeMaxEffect()
{
    if (m_pDiveGauge->GetFullGaugeFlag() == true)
    {
        CGauge* pGauge = CGauge::Create(CGauge::GAUGETYPE::PLAYERHP, m_pDiveGauge->GetParam(), m_pDiveGauge->GetWidth(), m_pDiveGauge->GetHeight(), m_pDiveGauge->GetPos());
        pGauge->SetUseLife(true, 50, 50);
        pGauge->SetPolygonType(m_pDiveGauge->GetPolygonType());
        pGauge->SetColor(m_pDiveGauge->GetColor(), false, 1.0f);
        pGauge->SetUseLifeRatioColor(true);
        pGauge->SetUseDeath(true);
        pGauge->SetUseAddScale(D3DXVECTOR2(0.3f, 0.3f), true);
        pGauge->SetUseScale(true);
        pGauge->SetScale(D3DXVECTOR2(1.0f, 1.0f));

        m_pDivePossibleNum->SetNumericState(m_pDivePossibleNum->GetValue() + 1, 50.0f, 50.0f);
        m_pDiveGauge->SetParam(0);
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
    SetIsLanding(false);                           //�n�ʂɏ���Ă��邩�ǂ����̃t���O�����Z�b�g
    GetCollisionInfo().GetSquareInfo().ResetPushOutFirstFlag();//���ꂼ��̎��̉����o������̗D��t���O�����Z�b�g
    m_bCollision = false;                          //�����Ԃ����Z�b�g
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
                CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�Ƀ_�E���L���X�g
                CCollision::ExtrusionCollisionSquarePushOutFirstDecide(this, pObjX);//�����`�̉����o������̂��ꂼ��̎��̏����̗D��x�����߂�
            }
            //�I�u�W�F�N�g�����ɐi�߂�
            pObj = pNext;
        }
    }
    //=======================================================================================

    //============================================================
    //�����o������J�n
    //============================================================
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
                CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�Ƀ_�E���L���X�g

                CCollision::ResolveExtrusionCollisionSquare(this, pObjX);//�����`�̉����o�����������
            }

            pObj = pNext;
        }
    }
    //=======================================================================================
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
//��Ԉُ��ς���
//========================================================
void CPlayer::ChengeAbnormalState(CPlayerAbnormalState* pAbnormalState)
{
    if (m_pAbnormalState != nullptr)
    {
        delete m_pAbnormalState;
        m_pAbnormalState = pAbnormalState;
    }
}

//========================================================
//�_���[�W��^����
//========================================================
void CPlayer::SetDamage(int nDamage, int nHitStopTime)
{
    CObjectX::SetDamage(nDamage, nHitStopTime);

    m_pHpGauge->SetParam(GetLifeInfo().GetLife());
    m_pHpGauge->SetShake(5.0f * nDamage, 30);

    CGauge* pGauge = CGauge::Create(CGauge::GAUGETYPE::PLAYERHP, m_pHpGauge->GetParam(), m_pHpGauge->GetWidth(), m_pHpGauge->GetHeight(), m_pHpGauge->GetPos());
    pGauge->SetUseLife(true, 10, 10);
    pGauge->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), false, 1.0f);
    pGauge->SetUseLifeRatioColor(true);
    pGauge->SetUseDeath(true);
    pGauge->SetUseAddScale(D3DXVECTOR2(0.1f, 0.1f), true);
    pGauge->SetUseScale(true);
    pGauge->SetScale(D3DXVECTOR2(1.0f, 1.0f));

    m_bDamage = true;//�_���[�W���󂯂���Ԃ𖾎��I�Ɏ���
    SetNextMotion(2);
}
//==========================================================================================================

//========================================================
//������������
//========================================================
void CPlayer::AdjustRot()
{
    //D3DXVECTOR3& Rot = GetRot();
    const D3DXVECTOR3& CameraRot = CManager::GetCamera()->GetRot();
    GetRotInfo().SetRot(D3DXVECTOR3(GetRotInfo().GetRot().x,D3DX_PI + CameraRot.y, GetRotInfo().GetRot().z));

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
    const D3DXVECTOR3& Pos = GetPosInfo().GetPos();
    D3DXVECTOR3 ScreenPos = CCalculation::CalcWorldToScreenNoViewport(Pos, *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
        static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));

    //��ʊO�ɏo�Ȃ��悤�ɕ␳
    if (ScreenPos.x > SCREEN_WIDTH || ScreenPos.x < 0.0f)
    {
        GetPosInfo().SetPos(GetPosInfo().GetPosOld());
    }
}
//==========================================================================================================

//*******************************************************
//��Ԉُ�X�e�[�g�F�X�[�p�[�N���X
//*******************************************************

//=======================================================
//�R���X�g���N�^
//=======================================================
CPlayerAbnormalState::CPlayerAbnormalState()
{

}
//==========================================================================================================

//=======================================================
//�f�X�g���N�^
//=======================================================
CPlayerAbnormalState::~CPlayerAbnormalState()
{

}
//==========================================================================================================

//=======================================================
//����
//=======================================================
void CPlayerAbnormalState::Process(CPlayer* pPlayer)
{

}
//==========================================================================================================

//*******************************************************
//��Ԉُ�X�e�[�g�F�m�b�N�o�b�N
//*******************************************************

//=======================================================
//�R���X�g���N�^
//=======================================================
CPlayerAbnormalState_KnockBack::CPlayerAbnormalState_KnockBack(CPlayer* pPlayer, D3DXVECTOR3 KnockBackMove, float fInertia) : m_KnockBackMove(KnockBackMove),m_fInertia(fInertia)
{

}
//==========================================================================================================

//=======================================================
//�f�X�g���N�^
//=======================================================
CPlayerAbnormalState_KnockBack::~CPlayerAbnormalState_KnockBack()
{

}
//==========================================================================================================

//=======================================================
//����
//=======================================================
void CPlayerAbnormalState_KnockBack::Process(CPlayer* pPlayer)
{
    m_KnockBackMove.x += (0.0f - m_KnockBackMove.x) * m_fInertia;
    m_KnockBackMove.y += (0.0f - m_KnockBackMove.y) * m_fInertia;
    m_KnockBackMove.z += (0.0f - m_KnockBackMove.z) * m_fInertia;

    pPlayer->GetMoveInfo().SetMove(m_KnockBackMove);

    if (fabsf(m_KnockBackMove.x) < 1.0f && fabsf(m_KnockBackMove.y) < 1.0f && fabsf(m_KnockBackMove.z) < 1.0f)
    {
        pPlayer->ChengeAbnormalState(DBG_NEW CPlayerAbnormalState());
    }
}
//==========================================================================================================