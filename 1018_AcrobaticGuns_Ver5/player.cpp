//=============================================================================
//
//１１月１０日：ダイブ実装[player.cpp]
//Author:ShinaTaiyo
//
//=============================================================================

//========================
//インクルード
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
//静的メンバ宣言
//====================================================

//==========================================================================================================

//====================================================
//コンストラクタ
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
//デストラクタ
//====================================================
CPlayer::~CPlayer()
{

}
//==========================================================================================================

//====================================================
//初期化処理
//====================================================
HRESULT CPlayer::Init()
{
    CObjectXAlive::Init();                 //Xオブジェクト初期化

    SetAutoSubLife(false);//自動的に体力を減らすかどうか
    SetUseGravity(true,1.0f);  //重力を使用する

    m_pLockOn = CLockon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f), CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
    m_pLockOn->SetUseDeath(true);
    m_pLockOn->SetPolygonRotSpeed(0.01f);

    m_pMeshOrbit = CMeshOrbit::Create(CMeshOrbit::MESHORBITTYPE::DEATHENAGA);
    m_pMeshOrbit->SetUseDeath(true);

    m_pModeDisp = CUi::Create(CUi::UITYPE::ACTIONMODE_GUN, CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

    m_pWire = CWire::Create(CWire::WIRETYPE::NORMAL, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15.0f, 20.0f, 4, 5);
    m_pWire->SetUseDeath(false);
    m_pWire->SetPlayerPointer(this);//プレイヤーのポインタを設定
    m_pWire->SetUseDraw(false);
    return S_OK;
}
//==========================================================================================================

//====================================================
//終了処理
//====================================================
void CPlayer::Uninit()
{
    CObjectXAlive::Uninit();//Xオブジェクト終了

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
//更新処理
//====================================================
void CPlayer::Update()
{
    m_pMove->MoveProcess(this);//現在のアクションモードの移動処理を実行

    AdjustRot();//向き調整処理

    ActionModeChenge(); //現在のアクションモードを変更する

    CObjectXAlive::Update();//更新処理

    AdjustPos();//位置調整処理

    CollisionProcess();

    m_pAttack->AttackProcess(this);//現在のアクションモードの攻撃処理を実装

    m_pEffect->EffectProcess(this);//エフェクト処理

    m_pWireShot->WireShotProcess(this);//ワイヤー発射状態処理

    CManager::GetDebugProc()->PrintDebugProc("プレイヤーの位置：%f %f %f\n", GetPos().x, GetPos().y, GetPos().z);

    CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 1, 60, 5.0f, 5.0f, 100, 10, false, GetPos() + GetVtxMax(), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
    //m_PosR = CGame::GetPlayer()->GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f) + m_AddPosR;
    //m_PosV = m_PosR + D3DXVECTOR3(sinf(m_Rot.y) * -200.0f, 0.0f, cosf(m_Rot.y) * -200.0f);
}
//==========================================================================================================

//====================================================
//描画処理
//====================================================
void CPlayer::Draw()
{
    CObjectXAlive::Draw();
}
//==========================================================================================================

//====================================================
//死亡フラグを設定
//====================================================
void CPlayer::SetDeath()
{
    //ロックオン
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
//プレイヤーの生成
//====================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
    CPlayer* pPlayer = DBG_NEW CPlayer(DBG_NEW CPlayerMove_Normal(),DBG_NEW CPlayerAttack_Shot(),DBG_NEW CPlayerEffect(),DBG_NEW CPlayerWireShot_Dont());//プレイヤーを生成

    bool bSuccess = pPlayer->CObject::GetCreateSuccess();
    int nIdx = 0;//テクスチャのインデックス
    if (bSuccess == true)
    {
        if (pPlayer != nullptr)
        {
            pPlayer->Init();                                                                 //初期化処理
            pPlayer->SetMove(move);//移動量
            pPlayer->CObject::SetType(CObject::TYPE::PLAYER);                                 //オブジェクトの種類を決める
            pPlayer->CObjectXMove::SetObjXType(CObjectXMove::OBJECTXTYPE_PLAYER);                    //オブジェクトXのタイプを設定
            pPlayer->CObjectXMove::SetTypeNum(0);                                                //オブジェクトXごとのタイプ番号を設定
            //pPlayer->SetUseGravity(true,1.0f);//重力
            nIdx = CManager::GetObjectXInfo()->Regist("data\\MODEL\\Player\\Player_ProtoType.x");
            pPlayer->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
                CManager::GetObjectXInfo()->GetBuffMat(nIdx),
                CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
                CManager::GetObjectXInfo()->GetTexture(nIdx),
                CManager::GetObjectXInfo()->GetColorValue(nIdx));           
            //モデル情報を割り当てる
            pPlayer->SetSize();
            pPlayer->SetPos(pos);                                                            //位置の設定
            pPlayer->SetPosOld(pos);                                                         //1f前の位置を設定
            pPlayer->SetSupportPos(pos);                                                     //設置位置
            pPlayer->SetRot(rot);                                                            //向きの設定
            pPlayer->SetScale(Scale);                                                        //拡大率の設定
            pPlayer->SetFormarScale(Scale);                                                               //元の拡大率を設定する
            pPlayer->SetAutoDeath(false);
            //カメラ初期設定（プレイヤー基準なのでプレイヤーから設定）
            //CCamera* pCamera = CManager::GetCamera();
            //pCamera->SetPosR(pPlayer->GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f));
            //pCamera->SetPosV(pCamera->GetPosR() + D3DXVECTOR3(sinf(pPlayer->GetRot().y) * 200.0f, 0.0f, cosf(pPlayer->GetRot().y) * 200.0f));

        }
    }
    else
    {//オブジェクトに空きがなかったので破棄する
        delete pPlayer;
        pPlayer = nullptr;
    }

	return pPlayer;
}
//==========================================================================================================

//========================================================
//攻撃開始
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

        //モードを切り替える
        if (m_NowActionMode == ACTIONMODE::SHOT)
        {//ショット→ダイブ
            m_NowActionMode = ACTIONMODE::DIVE;
        }
        else
        {//ダイブ→ショット
            m_NowActionMode = ACTIONMODE::SHOT;
            SetUseInteria(true, CObjectXMove::GetNormalInertia());
            SetUseGravity(true, GetNormalGravity());
        }

        //モード生成
        switch (m_NowActionMode)
        {
        case ACTIONMODE::SHOT://発射モード
            //m_pActionMode = DBG_NEW CPlayerShot;
            ChengeMoveMode(DBG_NEW CPlayerMove_Normal()); //通常移動モードにする
            ChengeAttackMode(DBG_NEW CPlayerAttack_Shot()); //攻撃可能モードにする
            ChengeEffectMode(DBG_NEW CPlayerEffect_None()); //エフェクトなしモードにする
            ChengeWireShotMode(DBG_NEW CPlayerWireShot_Dont);//ワイヤー発射状態をオフにする
            m_pModeDisp = CUi::Create(CUi::UITYPE::ACTIONMODE_GUN, CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        case ACTIONMODE::DIVE://ダイブモード
            //m_pActionMode = DBG_NEW CPlayerDive;
            ChengeMoveMode(DBG_NEW CPlayerMove_PrepDive());//ダイブ準備モードにする
            ChengeAttackMode(DBG_NEW CPlayerAttack_Dont);  //攻撃不能モードにする
            ChengeEffectMode(DBG_NEW CPlayerEffect_None()); //エフェクトなしモードにする
            ChengeWireShotMode(DBG_NEW CPlayerWireShot_Dont);//ワイヤー発射状態をオフにする
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
//移動モードをチェンジ
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
//攻撃モードをチェンジ
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
//エフェクトモードをチェンジ
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
//ワイヤー発射モードをチェンジ
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
//当たり判定全般処理
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

    m_bCollision = false;//判定状態をリセット
    bool bSuccessCollision = false;//当たり判定が成功したかどうか
    for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
    {
        CObject* pObj = CObject::GetTopObject(nCntPri);

        while (pObj != nullptr)
        {
            //次のオブジェクトを格納
            CObject* pNext = pObj->GetNextObject();

            //種類の取得（敵なら当たり判定）
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

            //オブジェクトを次に進める
            pObj = pNext;
        }

    }

    SetExtrusionCollisionSquareX(bCollisionX);
    SetExtrusionCollisionSquareY(bCollisionY);
    SetExtrusionCollisionSquareZ(bCollisionZ);
}
//==========================================================================================================

//========================================================
//ブロックとの当たり判定
//========================================================
void CPlayer::CollisionBlock()
{

}
//==========================================================================================================

//========================================================
//ジャンプ処理
//========================================================
void CPlayer::JumpProcess()
{
    //
}
//==========================================================================================================

//========================================================
//向き調整処理
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
//位置調整処理
//========================================================
void CPlayer::AdjustPos()
{
    const D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3 ScreenPos = CCalculation::CalcWorldToScreenNoViewport(Pos, *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
        static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));

    //画面外に出ないように補正
    if (ScreenPos.x > SCREEN_WIDTH || ScreenPos.x < 0.0f)
    {
        SetPos(GetPosOld());
    }
}
//==========================================================================================================