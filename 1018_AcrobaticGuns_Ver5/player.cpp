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
    CObjectX::Init();                 //Xオブジェクト初期化

    return S_OK;
}
//==========================================================================================================

//====================================================
//終了処理
//====================================================
void CPlayer::Uninit()
{
    CObjectX::Uninit();//Xオブジェクト終了
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
    CObjectX::Update();
}
//==========================================================================================================

//====================================================
//描画処理
//====================================================
void CPlayer::Draw()
{
    CObjectX::Draw();
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
            pPlayer->SetMove(move);                                                          //移動量を設定
            pPlayer->CObject::SetType(CObject::TYPE_PLAYER);                                 //オブジェクトの種類を決める
            pPlayer->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_PLAYER);                    //オブジェクトXのタイプを設定
            pPlayer->CObjectX::SetTypeNum(0);                                                //オブジェクトXごとのタイプ番号を設定
            pPlayer->SetLife(1);
            pPlayer->SetMaxLife(1);
            pPlayer->SetAutoSubLife(false);//自動的に体力を減らすかどうか
            nIdx = CManager::GetObjectXInfo()->Regist("data\\MODEL\\Player\\Player_ProtoType.x");
            pPlayer->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
                CManager::GetObjectXInfo()->GetBuffMat(nIdx),
                CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
                CManager::GetObjectXInfo()->GetTexture(nIdx),
                CManager::GetObjectXInfo()->GetColorValue(nIdx));                       //モデル情報を割り当てる
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
    D3DXVECTOR3& Rot = GetRot();
    float fRotDiff = 0.0f;//向きの差分
    bool bMove = false;//移動しているかどうか
    bMove = CCalculation::CaluclationMove(GetPos(), 5.0f, CCalculation::MOVEAIM_XZ,m_fRotAim);
    
    CCalculation::CalculationCollectionRot2D(Rot.y, m_fRotAim, 0.25f);
    
    CManager::GetDebugProc()->PrintDebugProc("向き：%f\n",Rot.y);
    CManager::GetDebugProc()->PrintDebugProc("向きの差分：%f\n", fRotDiff);
    CManager::GetDebugProc()->PrintDebugProc("目的の向き：%f\n", m_fRotAim);
}
//==========================================================================================================