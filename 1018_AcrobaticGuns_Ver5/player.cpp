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
#include "sound.h"
#include "objectXInfo.h"
#include "calculation.h"
#include "fade.h"
#include "game.h"
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
CPlayer::CPlayer() : m_fRotAim(0.0f)
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
    CObjectXMove::Init();                 //Xオブジェクト初期化

    return S_OK;
}
//==========================================================================================================

//====================================================
//終了処理
//====================================================
void CPlayer::Uninit()
{
    CObjectXMove::Uninit();//Xオブジェクト終了
}
//==========================================================================================================

//====================================================
//別枠の終了処理
//====================================================
void CPlayer::ExtraUninit()
{
}
//==========================================================================================================

//====================================================
//更新処理
//====================================================
void CPlayer::Update()
{
    MoveProcess();

    NormalAttackProcess();

    CObjectXMove::Update();
}
//==========================================================================================================

//====================================================
//描画処理
//====================================================
void CPlayer::Draw()
{
    CObjectXMove::Draw();
}
//==========================================================================================================

//====================================================
//死亡フラグを設定
//====================================================
void CPlayer::SetDeath()
{
    CObject::SetDeath();
}
//===========================================================================================================

//====================================================
//モデル情報の破棄
//====================================================
void CPlayer::Unload()
{

}
//==========================================================================================================

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
            pPlayer->CObject::SetType(CObject::TYPE_PLAYER);                                 //オブジェクトの種類を決める
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
    const D3DXVECTOR3& Rot = GetRot();
    D3DXVECTOR3 CalRot = Rot;
    float fRotDiff = 0.0f;//向きの差分
    const D3DXVECTOR3 & Move = GetMove();
    D3DXVECTOR3 AddMove = NULL_VECTOR3;
    bool bMove = false;//移動しているかどうか
    bMove = CCalculation::CaluclationMove(AddMove, 10.0f, CCalculation::MOVEAIM_XZ,m_fRotAim);
    CCalculation::CalculationCollectionRot2D(CalRot.y, m_fRotAim, 0.25f);
    
    SetMove(AddMove + D3DXVECTOR3(0.0f,Move.y,0.0f));
    SetRot(CalRot);
    CManager::GetDebugProc()->PrintDebugProc("向き：%f\n",Rot.y);
    CManager::GetDebugProc()->PrintDebugProc("目的の向き：%f\n", m_fRotAim);
}
//==========================================================================================================

//========================================================
//攻撃処理
//========================================================
void CPlayer::AttackProcress()
{

}
//==========================================================================================================

//========================================================
//通常攻撃処理
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