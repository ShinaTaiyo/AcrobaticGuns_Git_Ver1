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
//静的メンバ宣言
//====================================================
const int CPlayer::s_nNORMAL_MAXLIFE = 100;
//==========================================================================================================

//====================================================
//コンストラクタ
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
    CCharacter::Init();                 //Xオブジェクト初期化

    GetLifeInfo().SetAutoSubLife(false);//自動的に体力を減らすかどうか
    GetMoveInfo().SetUseGravity(true,1.0f);  //重力を使用する

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
        m_pDiveGauge->SetParam(0);//初期値からスタート
        m_pDiveGauge->SetPolygonType(CObject2D::POLYGONTYPE::LEFT);

        m_pDivePossibleNum = CUi::Create(CUi::UITYPE::POSSIBLEDIVENUMTEXT_000, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 1, false, D3DXVECTOR3(200.0f, 100.0f, 0.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pDivePossibleNum->SetNumericState(0, 50.0f, 50.0f);
        m_pDivePossibleNum->SetUseDeath(false);

        m_pWire = CWire::Create(CWire::WIRETYPE::NORMAL, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15.0f, 20.0f, 4, 5);
        m_pWire->SetUseDeath(false);
        m_pWire->SetPlayerPointer(this);//プレイヤーのポインタを設定
        m_pWire->SetUseDraw(false);
    }
    return S_OK;
}
//==========================================================================================================

//====================================================
//終了処理
//====================================================
void CPlayer::Uninit()
{
    CCharacter::Uninit();//Xオブジェクト終了

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
//更新処理
//====================================================
void CPlayer::Update()
{
    if (CScene::GetMode() == CScene::MODE_GAME)
    {
        if (GetLanding())
        {//地面にいるなら重力を最小限に
            GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x,0.0f, GetMoveInfo().GetMove().z));
        }

        m_pMove->MoveProcess(this);//現在のアクションモードの移動処理を実行

        m_pAbnormalState->Process(this);//状態異常の処理を実行

        AdjustRot();//向き調整処理

        ActionModeChenge(); //現在のアクションモードを変更する
    }

    CCharacter::Update();//更新処理

    CollisionProcess();

    if (CScene::GetMode() == CScene::MODE_GAME)
    {
        m_pEffect->EffectProcess(this);//エフェクト処理

        m_pAttack->AttackProcess(this);//現在のアクションモードの攻撃処理を実装

        DiveGaugeMaxEffect();//ダイブゲージがマックスになった時の演出

        m_pWireShot->WireShotProcess(this);//ワイヤー発射状態処理

        CManager::GetDebugText()->PrintDebugText("プレイヤーの位置：%f %f %f\n", GetPosInfo().GetPos().x, GetPosInfo().GetPos().y, GetPosInfo().GetPos().z);
        CManager::GetDebugText()->PrintDebugText("プレイヤーの体力：%d\n", GetLifeInfo().GetLife());

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

    MotionProcess();//モーション処理を行う
}
//==========================================================================================================

//====================================================
//描画処理
//====================================================
void CPlayer::Draw()
{
    CCharacter::Draw();
}
//==========================================================================================================

//====================================================
//死亡フラグを設定
//====================================================
void CPlayer::SetDeath()
{
    if (GetUseDeath() == true)
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
            pPlayer->GetMoveInfo().SetMove(move);//移動量
            pPlayer->CObject::SetType(CObject::TYPE::PLAYER);                                 //オブジェクトの種類を決める
            pPlayer->SetObjXType(CObjectX::OBJECTXTYPE_PLAYER);                    //オブジェクトXのタイプを設定
            pPlayer->SetTypeNum(0);                                                //オブジェクトXごとのタイプ番号を設定
            //pPlayer->SetUseGravity(true,1.0f);//重力
            //モデル情報を割り当てる
            nIdx = CManager::GetObjectXInfo()->Regist("data\\MODEL\\Enemy\\MotionEnemy\\DiveWeakEnemy\\DiveWeakEnemy00_Source.x");
            pPlayer->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
                CManager::GetObjectXInfo()->GetBuffMat(nIdx),
                CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
                CManager::GetObjectXInfo()->GetTexture(nIdx),
                CManager::GetObjectXInfo()->GetColorValue(nIdx));

            pPlayer->Regist("data\\MODEL\\Enemy\\MotionEnemy\\DiveWeakEnemy\\DiveWeakEnemyMotion.txt",pPlayer);//モーションファイルを割り当てる
            pPlayer->SetSize();
            pPlayer->GetDrawInfo().SetUseDraw(false);                                                     //描画しない
            pPlayer->GetPosInfo().SetPos(pos);                                                            //位置の設定
            pPlayer->GetPosInfo().SetPosOld(pos);                                                         //1f前の位置を設定
            pPlayer->GetPosInfo().SetPosFuture(pos);                                                      //1f後の位置を設定
            pPlayer->GetPosInfo().SetSupportPos(pos);                                                     //設置位置
            pPlayer->GetRotInfo().SetRot(rot);                                                            //向きの設定
            pPlayer->GetSizeInfo().SetScale(Scale);                                                        //拡大率の設定
            pPlayer->GetSizeInfo().SetFormarScale(Scale);                                                  //元の拡大率を設定する
            pPlayer->GetLifeInfo().SetAutoDeath(false);                                                    //死亡フラグを自動で発動するかどうか
            pPlayer->GetDrawInfo().SetUseShadow(true);
            //体力
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
    if (CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::X) || CManager::GetInputMouse()->GetMouseRightClickTrigger())
    {
        if (m_pModeDisp != nullptr)
        {
            m_pModeDisp->SetUseDeath(true);
            m_pModeDisp->SetDeath();
            m_pModeDisp = nullptr;
        }
        CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::MODECHENGE);

        //モードを切り替える
        if (m_NowActionMode == ACTIONMODE::SHOT)
        {//ショット→ダイブ
            m_NowActionMode = ACTIONMODE::DIVE;
        }
        else
        {//ダイブ→ショット
            m_NowActionMode = ACTIONMODE::SHOT;
            GetMoveInfo().SetUseInteria(true, GetNormalInertia());
            GetMoveInfo().SetUseGravity(true, GetNormalGravity());
        }

        //モード生成
        switch (m_NowActionMode)
        {
        case ACTIONMODE::SHOT://発射モード
            ChengeMoveMode(DBG_NEW CPlayerMove_Normal()); //通常移動モードにする
            ChengeAttackMode(DBG_NEW CPlayerAttack_Shot()); //攻撃可能モードにする
            ChengeEffectMode(DBG_NEW CPlayerEffect_None()); //エフェクトなしモードにする
            ChengeWireShotMode(DBG_NEW CPlayerWireShot_Dont);//ワイヤー発射状態をオフにする
            m_pModeDisp = CUi::Create(CUi::UITYPE::ACTIONMODE_GUN, CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        case ACTIONMODE::DIVE://ダイブモード
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

        m_pWire->SetUseDraw(false);
        m_pWire->GetWireHead()->GetDrawInfo().SetUseDraw(false);

        m_pModeDisp->SetUseDeath(false);//死亡フラグを発動させない


    }
}
//==========================================================================================================


//========================================================
//ダイブゲージがマックスになった時の演出を行う
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
    SetIsLanding(false);                           //地面に乗っているかどうかのフラグをリセット
    GetCollisionInfo().GetSquareInfo().ResetPushOutFirstFlag();//それぞれの軸の押し出し判定の優先フラグをリセット
    m_bCollision = false;                          //判定状態をリセット
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
                CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにダウンキャスト
                CCollision::ExtrusionCollisionSquarePushOutFirstDecide(this, pObjX);//正方形の押し出し判定のそれぞれの軸の順序の優先度を決める
            }
            //オブジェクトを次に進める
            pObj = pNext;
        }
    }
    //=======================================================================================

    //============================================================
    //押し出し判定開始
    //============================================================
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
                CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにダウンキャスト

                CCollision::ResolveExtrusionCollisionSquare(this, pObjX);//正方形の押し出し判定をする
            }

            pObj = pNext;
        }
    }
    //=======================================================================================
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
//状態異常を変える
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
//ダメージを与える
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

    m_bDamage = true;//ダメージを受けた状態を明示的に示す
    SetNextMotion(2);
}
//==========================================================================================================

//========================================================
//向き調整処理
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
//位置調整処理
//========================================================
void CPlayer::AdjustPos()
{
    const D3DXVECTOR3& Pos = GetPosInfo().GetPos();
    D3DXVECTOR3 ScreenPos = CCalculation::CalcWorldToScreenNoViewport(Pos, *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
        static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));

    //画面外に出ないように補正
    if (ScreenPos.x > SCREEN_WIDTH || ScreenPos.x < 0.0f)
    {
        GetPosInfo().SetPos(GetPosInfo().GetPosOld());
    }
}
//==========================================================================================================

//*******************************************************
//状態異常ステート：スーパークラス
//*******************************************************

//=======================================================
//コンストラクタ
//=======================================================
CPlayerAbnormalState::CPlayerAbnormalState()
{

}
//==========================================================================================================

//=======================================================
//デストラクタ
//=======================================================
CPlayerAbnormalState::~CPlayerAbnormalState()
{

}
//==========================================================================================================

//=======================================================
//処理
//=======================================================
void CPlayerAbnormalState::Process(CPlayer* pPlayer)
{

}
//==========================================================================================================

//*******************************************************
//状態異常ステート：ノックバック
//*******************************************************

//=======================================================
//コンストラクタ
//=======================================================
CPlayerAbnormalState_KnockBack::CPlayerAbnormalState_KnockBack(CPlayer* pPlayer, D3DXVECTOR3 KnockBackMove, float fInertia) : m_KnockBackMove(KnockBackMove),m_fInertia(fInertia)
{

}
//==========================================================================================================

//=======================================================
//デストラクタ
//=======================================================
CPlayerAbnormalState_KnockBack::~CPlayerAbnormalState_KnockBack()
{

}
//==========================================================================================================

//=======================================================
//処理
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