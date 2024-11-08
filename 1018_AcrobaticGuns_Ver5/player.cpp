//=============================================================================
//
//７月４日：回避アクション実装[player.cpp]
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
CPlayer::CPlayer() : m_fRotAim(0.0f),m_pLockOn(nullptr)
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

    m_pLockOn = CLockon::Create(SENTER_VECTOR3, CObject2D::POLYGONTYPE01_SENTERROLLING, 100.0f, 100.0f, NORMAL_COL);
    m_pLockOn->SetUseDeath(true);

    return S_OK;
}
//==========================================================================================================

//====================================================
//終了処理
//====================================================
void CPlayer::Uninit()
{
    CObjectXAlive::Uninit();//Xオブジェクト終了
}
//==========================================================================================================

//====================================================
//更新処理
//====================================================
void CPlayer::Update()
{
    //移動処理
    MoveProcess();

    //向き調整処理
    AdjustRot();

    //更新処理
    CObjectXAlive::Update();

    //攻撃処理
    AttackProcress();

    //ロックオンの処理
    LockOnProcess();

    //ブロックとの当たり判定
    CollisionBlock();
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
//プレイヤーの生成
//====================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
    CPlayer* pPlayer = new CPlayer;                                                                             //プレイヤーを生成

    bool bSuccess = pPlayer->CObject::GetCreateSuccess();
    int nIdx = 0;//テクスチャのインデックス
    if (bSuccess == true)
    {
        if (pPlayer != nullptr)
        {
            pPlayer->Init();                                                                 //初期化処理
            pPlayer->CObject::SetType(CObject::TYPE::PLAYER);                                 //オブジェクトの種類を決める
            pPlayer->CObjectXMove::SetObjXType(CObjectXMove::OBJECTXTYPE_PLAYER);                    //オブジェクトXのタイプを設定
            pPlayer->CObjectXMove::SetTypeNum(0);                                                //オブジェクトXごとのタイプ番号を設定
            pPlayer->SetLife(1);
            pPlayer->SetMaxLife(1);
            pPlayer->SetAutoSubLife(false);//自動的に体力を減らすかどうか
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
//移動処理
//========================================================
void CPlayer::MoveProcess()
{
    const D3DXVECTOR3& Pos = GetPos();
    float fRotDiff = 0.0f;//向きの差分
    const D3DXVECTOR3 & Move = GetMove();
    D3DXVECTOR3 AddMove = NULL_VECTOR3;
    bool bMove = false;//移動しているかどうか
    bMove = CCalculation::CaluclationMove(true,AddMove, 10.0f, CCalculation::MOVEAIM_XZ,m_fRotAim);
    //CCalculation::CalculationCollectionRot2D(CalRot.y, m_fRotAim, 0.25f);
    
    //CManager::GetInputJoypad()->GetLStickPress();

    SetMove(AddMove + D3DXVECTOR3(0.0f,Move.y,0.0f));
    CManager::GetDebugProc()->PrintDebugProc("プレイヤーの位置：%f %f %f\n",Pos.x,Pos.y,Pos.z);
    CManager::GetDebugProc()->PrintDebugProc("目的の向き：%f\n", m_fRotAim);
}
//==========================================================================================================

//========================================================
//攻撃処理
//========================================================
void CPlayer::AttackProcress()
{
    //通常攻撃処理
    NormalAttackProcess();
}
//==========================================================================================================

//========================================================
//通常攻撃処理
//========================================================
void CPlayer::NormalAttackProcess()
{   
    //攻撃を開始
    AttackStart();
}
//==========================================================================================================

//========================================================
//攻撃開始
//========================================================
void CPlayer::AttackStart()
{
    D3DXVECTOR3 ShotPos = GetPos() + D3DXVECTOR3(0.0f, GetSize().y, 0.0f);
    D3DXVECTOR3 Move = CCalculation::Calculation3DVec(ShotPos, m_pLockOn->GetNearRayColObjPos(), 20.0f);
    CAttackPlayer* pAttackPlayer = nullptr;//プレイヤー攻撃へのポインタ
    if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true || CManager::GetInputJoypad()->GetRT_Repeat(6) == true)
    {
        pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE::TYPE00_BULLET, 60, ShotPos,GetRot(), Move, ONE_VECTOR3);
        pAttackPlayer->SetUseInteria(false);
        pAttackPlayer->SetAutoSubLife(true);
    }
}
//==========================================================================================================

//========================================================
//ブロックとの当たり判定
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

            if (type == CObject::TYPE::BLOCK)
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

            //オブジェクトを次に進める
            pObj = pNext;
        }

    }

    CManager::GetDebugProc()->PrintDebugProc("X方向の当たり判定が発動しているかどうか（０：いいえ、１：はい）：%d\n", bCollisionX);
    CManager::GetDebugProc()->PrintDebugProc("Y方向の当たり判定が発動しているかどうか（０：いいえ、１：はい）：%d\n", bCollisionY);
    CManager::GetDebugProc()->PrintDebugProc("Z方向の当たり判定が発動しているかどうか（０：いいえ、１：はい）：%d\n", bCollisionZ);
}
//==========================================================================================================

//========================================================
//ロックオンの処理
//========================================================
void CPlayer::LockOnProcess()
{
    //移動処理
    LockOnMove();
}
//==========================================================================================================

//========================================================
//ロックオンを動かす
//========================================================
void CPlayer::LockOnMove()
{

}
//==========================================================================================================

//========================================================
//向き調整処理
//========================================================
void CPlayer::AdjustRot()
{
    D3DXVECTOR3& Rot = GetRot();
    const D3DXVECTOR3& CameraRot = CManager::GetCamera()->GetRot();
    SetRot(D3DXVECTOR3(0.0f,D3DX_PI + CameraRot.y,0.0f));
    //SetAxis(CameraRot.y + D3DX_PI);
}
//==========================================================================================================