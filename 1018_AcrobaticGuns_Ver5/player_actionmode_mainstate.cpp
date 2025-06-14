//===================================================================================================================
//
//２０２５年２月１７日：プレイヤーのアクションモードのメイン状態クラスを作る[player_actionmode_mainstate.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================

//===================================================================================================================
//インクルード
//===================================================================================================================
#include "player_actionmode_mainstate.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"
#include "camera.h"
#include "manager.h"
#include "tutorial.h"
#include "game.h"
#include "input.h"
#include "wire_head.h"
#include "sound.h"
#include "calculation.h"
//=========================================================================================================================================================

//*******************************************************************************************************************
//スーパークラス
//*******************************************************************************************************************

//===================================================================================================================
//コンストラクタ
//===================================================================================================================
CPlayerActionMode::CPlayerActionMode()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//デストラクタ
//===================================================================================================================
CPlayerActionMode::~CPlayerActionMode()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//処理（なにもない)
//===================================================================================================================
void CPlayerActionMode::Process(CPlayer* pPlayer)
{

}
//=========================================================================================================================================================

//*******************************************************************************************************************
//射撃移動クラス
//*******************************************************************************************************************

//===================================================================================================================
//コンストラクタ
//===================================================================================================================
CPlayerActionMode_ShotMove::CPlayerActionMode_ShotMove(CPlayer* pPlayer)
{
	CObjectX::MoveInfo& MoveInfo = pPlayer->GetMoveInfo();     //移動情報
	CLockon* pLockOn = pPlayer->GetLockOn();                   //ロックオン
	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Shot());   //攻撃モードを射撃モードにする
	pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_Normal());     //移動モードを普通にする（地面で動く、ジャンプができる、回避ができる)
	MoveInfo.SetUseInteria(true, CObjectX::GetNormalInertia());//慣性を使用する
	MoveInfo.SetUseGravity(true, CObjectX::GetNormalGravity());//重力を使用する

	pLockOn->ChengeTexture(CLockon::TYPE::SHOT);               //ターゲットのテクスチャを射撃仕様に変更

	pPlayer->SetModeDisp(CUi::Create(CUi::UITYPE::ACTIONMODE_GUN,false,CObject2D::POLYGONTYPE::SENTERROLLING,100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));//モード表示を「射撃」に変える

}
//=========================================================================================================================================================

//===================================================================================================================
//デストラクタ
//===================================================================================================================
CPlayerActionMode_ShotMove::~CPlayerActionMode_ShotMove()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//処理
//===================================================================================================================
void CPlayerActionMode_ShotMove::Process(CPlayer* pPlayer)
{

}
//=========================================================================================================================================================

//*******************************************************************************************************************
//ダイブ準備クラス
//*******************************************************************************************************************

//===================================================================================================================
//コンストラクタ
//===================================================================================================================
CPlayerActionMode_PrepDive::CPlayerActionMode_PrepDive(CPlayer* pPlayer)
{
	CObjectX::MoveInfo& MoveInfo = pPlayer->GetMoveInfo();      //移動情報
	CLockon* pLockOn = pPlayer->GetLockOn();                    //ロックオン
	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());    //攻撃モードをダイブモードにする
	pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_Normal());      //移動モードを普通にする（地面で動く、ジャンプができる、回避ができる)
	MoveInfo.SetUseInteria(true, CObjectX::GetNormalInertia()); //慣性を使用する
	MoveInfo.SetUseGravity(true, CObjectX::GetNormalGravity()); //重力を使用する

	pLockOn->ChengeTexture(CLockon::TYPE::DIVE);                //ターゲットのテクスチャを射撃仕様に変更

	pPlayer->SetModeDisp(CUi::Create(CUi::UITYPE::ACTIONMODE_DIVE,false, CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));//モード表示を「ダイブ」に変える
}
//=========================================================================================================================================================

//===================================================================================================================
//デストラクタ
//===================================================================================================================
CPlayerActionMode_PrepDive::~CPlayerActionMode_PrepDive()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//処理
//===================================================================================================================
void CPlayerActionMode_PrepDive::Process(CPlayer* pPlayer)
{
	//==================
	//変数
	//==================
	CWire* pWire = pPlayer->GetWire();                       //ワイヤーを取得
	CWireHead* pWireHead = pWire->GetWireHead();             //ワイヤーの頭を取得
	CLockon* pLockon = pPlayer->GetLockOn();                 //ロックオンを取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();    //マウスを取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad(); //ジョイパッドを取得
	//============================================================================
	pWireHead->GetPosInfo().SetPos(pPlayer->GetPosInfo().GetPos());//ダイブ準備中なのでワイヤーヘッドをプレイヤーの位置に固定

	if ((pInputJoypad->GetRT_Trigger() || pInputMouse->GetMouseLeftClickTrigger()) && pLockon->GetSuccessRayCollision())
	{//ジョイパッドの右トリガーボタン又はマウスの右クリックを押していて、ロックオンのレイが当たっていたら
		pPlayer->ChengeActionMode(DBG_NEW CPlayerActionMode_WireShot(pPlayer));//メインステートをワイヤー発射ステートに変更
	}

}
//=========================================================================================================================================================

//*******************************************************************************************************************
//ワイヤー発射クラス
//*******************************************************************************************************************

//===================================================================================================================
//コンストラクタ
//===================================================================================================================
CPlayerActionMode_WireShot::CPlayerActionMode_WireShot(CPlayer* pPlayer)
{
	//==================
    //変数
    //==================
	CWire* pWire = pPlayer->GetWire();//ワイヤーを取得
	CWireHead* pWireHead = pWire->GetWireHead();//ワイヤーの頭を取得
	CLockon* pLockon = pPlayer->GetLockOn();//ロックオンを取得

	//ワイヤーの頭
	CObjectX::MoveInfo& WireHeadMoveInfo = pWireHead->GetMoveInfo();//ワイヤーの頭の移動情報を取得
	CObjectX::PosInfo& WireHeadPosInfo = pWireHead->GetPosInfo();//ワイヤーの頭の位置情報を取得
	CObjectX::RotInfo& WireHeadRotInfo = pWireHead->GetRotInfo();//ワイヤーの頭の向き情報を取得
	CObjectX::DrawInfo& WireHeadDrawInfo = pWireHead->GetDrawInfo();//ワイヤーの頭の描画情報を取得

	//プレイヤー
	CObjectX::PosInfo& PlayerPosInfo = pPlayer->GetPosInfo();//プレイヤーの位置情報を取得
	CObjectX::MoveInfo& PlayerMoveInfo = pPlayer->GetMoveInfo();//プレイヤーの移動情報を取得
	const D3DXVECTOR3& PlayerPos = PlayerPosInfo.GetPos();   //プレイヤーの位置を取得

	const D3DXVECTOR3& WireHeadPos = WireHeadPosInfo.GetPos();//ワイヤーの頭の位置
	const D3DXVECTOR3& LockOnRayCollisionPos = pLockon->GetNearRayColObjPos();//ロックオンのレイが当たった一番近いオブジェクトの衝突位置を取得する
	const D3DXVECTOR3& LockOnRay = pLockon->GetNowRay(); // ロックオンのレイ
	//============================================================================

	//サブステート設定
	pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_Dont());//移動をしない
	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());//攻撃をしない

	//===============================
	//ワイヤー頭発射処理
	//===============================

	//正規化したベクトルをもとにYawとPitchを求める
	float fYaw = atan2f(LockOnRay.x, LockOnRay.z);//Zを軸にする
	float fPitch = atan2f(LockOnRay.y, sqrtf(powf(LockOnRay.x, 2) + powf(LockOnRay.z, 2)));//XZ平面の長さを軸にして求める
	WireHeadRotInfo.SetRot(D3DXVECTOR3(-fPitch, fYaw, 0.0f));                                                            // 向きを設定                                                                                 

	// ワイヤーの頭をロックオンのレイの衝突位置に発射する
	WireHeadMoveInfo.SetMove(
		Calculation::Move::DirrectionToTarget(
			WireHeadPos, LockOnRayCollisionPos, s_fWIREHEAD_SHOTSPEED)
	); 

	pWireHead->ResetCoolTime(); // ワイヤーの頭が当たるまでのクールタイムをリセット
	WireHeadMoveInfo.SetUseInteria(false, CObjectX::GetNormalInertia()); // ワイヤーの頭の慣性をオフにする
	WireHeadMoveInfo.SetUseGravity(false, 1.0f); // ワイヤーの頭の重力をオフにする
	WireHeadDrawInfo.SetUseDraw(true);           // ワイヤーの頭の描画を復活させる

	//ワイヤー
	pWire->SetUseDraw(true);   // ワイヤーの描画を復活させる
	pWire->SetUseUpdate(true); // ワイヤーの更新を復活させる

	//プレイヤー
	PlayerMoveInfo.SetUseInteria(false, CObjectX::GetNormalInertia()); // 慣性を使用しない
	PlayerMoveInfo.SetUseGravity(false, CObjectX::GetNormalGravity()); // 重力を使用しない
}
//=========================================================================================================================================================

//===================================================================================================================
//デストラクタ
//===================================================================================================================
CPlayerActionMode_WireShot::~CPlayerActionMode_WireShot()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//処理
//===================================================================================================================
void CPlayerActionMode_WireShot::Process(CPlayer* pPlayer)
{
	// === 処理に使用する情報を宣言、初期化 ===

	CWire* pWire = pPlayer->GetWire();           // ワイヤーへのポインタ
    CWireHead* pWireHead = pWire->GetWireHead(); // ワイヤーヘッドへのポインタ
	
	//プレイヤー
	CObjectX::MoveInfo& PlayerMoveInfo = pPlayer->GetMoveInfo();    // プレイヤーの移動情報を取得
	CObjectX::PosInfo& PlayerPosInfo = pPlayer->GetPosInfo();       // プレイヤーの位置情報を取得
	const D3DXVECTOR3& PlayerPos = PlayerPosInfo.GetPos();          // プレイヤーの位置

	//ワイヤーの頭
	CObjectX::PosInfo& WireHeadPosInfo = pWireHead->GetPosInfo();    // ワイヤーの頭の位置情報を取得
	const D3DXVECTOR3& WireHeadPos = WireHeadPosInfo.GetPos();       // ワイヤーの頭の位置
	CObjectX::MoveInfo& WireHeadMoveInfo = pWireHead->GetMoveInfo(); // ワイヤーの頭の移動情報を取得

	// === ワイヤー発射処理開始 ===
    
    FrightenedEnemy(pPlayer); // この処理の途中で狙った敵は怯える
	PlayerMoveInfo.SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); // ワイヤー発射中はプレイヤーの動きを止める

	// ワイヤーヘッドがどれかのオブジェクトに当たったら
	if (pWireHead->GetSuccessCollision())
	{
		DecisionCameraRot(pPlayer); // カメラの向きを決める
		WireHeadMoveInfo.SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); // ワイヤーヘッドの移動を止める

		// メインステートを「ダイブ移動」に変更
		pPlayer->ChengeActionMode(DBG_NEW CPlayerActionMode_DiveMove(
			Calculation::Move::DirrectionToTarget(
				PlayerPos,WireHeadPos,s_fWIREHEAD_SHOTSPEED * 0.5f
			),
			pPlayer));

		pPlayer->SetSuccessCollision(false); // 判定状態を確定解除
	}

}

//===================================================================================================================
//敵を怯えさせる処理
//===================================================================================================================
void CPlayerActionMode_WireShot::FrightenedEnemy(CPlayer* pPlayer)
{
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::TYPE::ENEMY));    //敵のリストの先頭オブジェクトを取得
	CLockon* pLockOn = pPlayer->GetLockOn();                                          //ロックオンを取得
   const D3DXVECTOR3 & FrontPos = pLockOn->GetFrontPos();                             //ロックオンの手前座標を取得
   const D3DXVECTOR3& Ray = pLockOn->GetNowRay();                                     //ロックオンのレイを取得
   D3DXVECTOR3 CollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                          //レイが当たった位置格納用
   while (pObj != nullptr)
   {//オブジェクトがnullptrになるまで検索する

	   CObject* pNext = pObj->GetNextObject();//次のオブジェクトを取得

	   CEnemy* pEnemy = static_cast<CEnemy*>(pObj);//オブジェクトを敵にキャスト
	   if (pEnemy->GetEnemyType() == CEnemy::ENEMYTYPE::DIVEWEAK)
	   {//ダイブに弱い敵だけ処理をする。怯え状態じゃないときに怯え状態にする
		   if (pEnemy->GetState() != CEnemy::STATE::FRIGHTENDED && CCollision::RayIntersectsAABBCollisionPos(FrontPos, Ray, pEnemy->GetPosInfo().GetPos() + pEnemy->GetSizeInfo().GetVtxMin(), pEnemy->GetPosInfo().GetPos() + pEnemy->GetSizeInfo().GetVtxMax(), CollisionPos))
		   {//敵が怯え状態じゃないかつレイが当たっていたら
			   pEnemy->ChengeMove(DBG_NEW CEnemyMove_Frightened(pEnemy, pEnemy->GetPosInfo().GetPos(), 90));//１．５秒間怯え状態にする
		   }
	   }
	   pObj = pNext;//リストを次に進める
   }

}
//=========================================================================================================================================================

//===================================================================================================================
//カメラの向きを決定する（ワイヤーの頭がオブジェクトに当たった時に見ているカメラの向きに応じて補正する）
//===================================================================================================================
void CPlayerActionMode_WireShot::DecisionCameraRot(CPlayer* pPlayer)
{
	// === 変数 ===

	CWire* pWire = pPlayer->GetWire();                //ワイヤーへのポインタ
    CWireHead* pWireHead = pWire->GetWireHead();      //ワイヤーの頭へのポインタ
    CCamera* pCamera = CManager::GetCamera();         //カメラを取得
	const D3DXVECTOR3& CameraRot = pCamera->GetRot(); //カメラの向き
    D3DXVECTOR3 ComRot = pWireHead->GetPosInfo().GetPos() - pPlayer->GetPosInfo().GetPos();//ワイヤーの頭とプレイヤーのベクトルを求める
    D3DXVec3Normalize(&ComRot, &ComRot);                                                   //上記で求めたベクトルを正規化する
	//正規化ベクトルをもとにワイヤーの頭とプレイヤーの向きを求める
    float fYaw = atan2f(ComRot.x, ComRot.z);                                               //Z方向を軸にYawを求める
    float fPitch = atan2f(ComRot.y, sqrtf(powf(ComRot.x, 2) + powf(ComRot.z, 2)));         //XZ平面の長さを軸にPitchを求める

	// === 処理 ===
	 
    // 現在のカメラの向きが目的の向きに近かったらダイブ先に合わせる 
	if (CameraRot.x > fPitch - 0.5f && CameraRot.x < fPitch + 0.5f &&
		CameraRot.y > fYaw - 0.5f && CameraRot.y < fYaw + 0.5f)
    {
    	pCamera->ChengeState(DBG_NEW CCameraState_TurnAround(D3DXVECTOR3(fPitch, fYaw, 0.0f), 0.1f));
    }
    //==============================================================================================

}
//=========================================================================================================================================================

//*******************************************************************************************************************
//ダイブ移動クラス
//*******************************************************************************************************************

//===================================================================================================================
//コンストラクタ
//===================================================================================================================
CPlayerActionMode_DiveMove::CPlayerActionMode_DiveMove(D3DXVECTOR3 Move, CPlayer* pPlayer)
{
	CObjectX::MoveInfo& PlayerMoveInfo = pPlayer->GetMoveInfo();      //プレイヤーの移動情報
	PlayerMoveInfo.SetMove(Move);                                     //プレイヤーに移動量を設定
	PlayerMoveInfo.SetUseInteria(false, CObjectX::GetNormalInertia());//慣性を使用しない
	PlayerMoveInfo.SetUseGravity(false, CObjectX::GetNormalGravity());//重力を使用しない

	pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_Dive());              //移動方法を「ダイブ」に変更（ワイヤーの頭をひたすら狙って飛ぶ)
	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());          //攻撃方法を「なし」に変更
}
//=========================================================================================================================================================

//===================================================================================================================
//デストラクタ
//===================================================================================================================
CPlayerActionMode_DiveMove::~CPlayerActionMode_DiveMove()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//処理
//===================================================================================================================
void CPlayerActionMode_DiveMove::Process(CPlayer* pPlayer)
{
	// === 処理に使用する情報を宣言、初期化 ===

	bool bInput = false;//指定のボタンを押しているかどうか

	// ワイヤー
	CWire* pWire = pPlayer->GetWire(); // ワイヤーへのポインタ
	CWireHead* pWireHead = pWire->GetWireHead(); // ワイヤーの頭へのポインタ
	CObjectX::PosInfo& WireHeadPosInfo = pWireHead->GetPosInfo(); // ワイヤーの頭の位置情報
	const D3DXVECTOR3& WireHeadPos = WireHeadPosInfo.GetPos();    // ワイヤーの頭の位置

	// プレイヤー
	CObjectX::PosInfo& PlayerPosInfo = pPlayer->GetPosInfo(); // プレイヤーの位置情報
	CObjectX::RotInfo& PlayerRotInfo = pPlayer->GetRotInfo(); // プレイヤーの向き情報
	const D3DXVECTOR3& PlayerRot = PlayerRotInfo.GetRot();    // プレイヤーの向き
	const D3DXVECTOR3& PlayerPos = PlayerPosInfo.GetPos();    // プレイヤーの位置

	CCamera* pCamera = CManager::GetCamera(); //カメラへのポインタ
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad(); // ジョイパッド入力情報へのポインタ
	CInputMouse* pInputMouse = CManager::GetInputMouse();    // マウス入力情報へのポインタ

	// === ダイブ移動処理を開始 ===

	// 指定されたボタンを押したらtrueにする
	if (pInputJoypad->GetRT_Press() || pInputMouse->GetMouseLeftClickPress())
	{
		bInput = true;
	}

	// ダイブ時に判定したら移動モードと攻撃モードを通常に戻す
	if (Calculation::Length::ToGoalPos(PlayerPos,WireHeadPos) < s_fCOLLISIONSTARTLENGTH)
	{
		// 引っ付き（敵以外なら引っ付く）
		if (bInput == true && pWireHead->GetCollisionObjType() != CObject::TYPE::ENEMY)
		{
			CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::STUCKWALL);  // 壁に引っ付くチュートリアルを完了
			pPlayer->ChengeActionMode(DBG_NEW CPlayerActionMode_Stuck(pPlayer)); // メインステートを「引っ付き」に変更
		}
		// 引っ付き→ダイブ（もしワイヤーヘッドが衝突したオブジェクトが敵の場合、敵には引っ付きたくないので、攻撃に移行する
		else
		{
			pWire->SetUseDraw(false); // ワイヤーの頭を描画しない
			PlayerRotInfo.SetRot(D3DXVECTOR3(0.0f, pCamera->GetRot().y, 0.0f)); // 向きをカメラの向きに設定
			pPlayer->ChengeActionMode(DBG_NEW CPlayerActionMode_DiveAttack(pPlayer)); // メインステートを「ダイブ攻撃」に変更
		}
	}
}
//=========================================================================================================================================================

//*******************************************************************************************************************
//ダイブ攻撃クラス
//*******************************************************************************************************************

//===================================================================================================================
//コンストラクタ
//===================================================================================================================
CPlayerActionMode_DiveAttack::CPlayerActionMode_DiveAttack(CPlayer* pPlayer)
{
	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dive());//攻撃を「ダイブ」にする
}
//=========================================================================================================================================================

//===================================================================================================================
//デストラクタ
//===================================================================================================================
CPlayerActionMode_DiveAttack::~CPlayerActionMode_DiveAttack()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//処理
//===================================================================================================================
void CPlayerActionMode_DiveAttack::Process(CPlayer* pPlayer)
{
	pPlayer->ChengeActionMode(DBG_NEW CPlayerActionMode_PrepDive(pPlayer));//コンストラクタが通った後にダイブ攻撃処理が終わったので、ダイブ準備モードに戻す
}
//=========================================================================================================================================================

//*******************************************************************************************************************
//引っ付きクラス
//*******************************************************************************************************************

//===================================================================================================================
//コンストラクタ
//===================================================================================================================
CPlayerActionMode_Stuck::CPlayerActionMode_Stuck(CPlayer* pPlayer) : m_bStartWireShot(true)
{
	// === 変数、初期設定 ===

	pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_Dont()); // 移動方法を「なし」に変更
	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont()); // 攻撃方法を「なし」に変更
															                                        
	CCamera* pCamera = CManager::GetCamera(); // カメラへのポインタ
	CWireHead* pWireHead = pPlayer->GetWire()->GetWireHead(); // ワイヤーの頭へのポインタ

	CObjectX::PosInfo& 
		PlayerPosInfo = pPlayer->CObjectX::GetPosInfo(),     // プレイヤー位置情報
		WireHeadPosInfo = pWireHead->CObjectX::GetPosInfo(); // ワイヤー頭の位置情報

	const D3DXVECTOR3&
		PlayerPosOld = PlayerPosInfo.GetPosOld(), // プレイヤーの1f前の位置
		WireHeadPos = WireHeadPosInfo.GetPos(),   // ワイヤーの頭の位置
		CameraRot = pCamera->GetRot();            // カメラの向き

	D3DXVECTOR3 ComRot = WireHeadPos - PlayerPosOld; // ワイヤーの頭とプレイヤーの1f前の位置のベクトルを求める
	D3DXVec3Normalize(&ComRot, &ComRot);     // ベクトルを正規化

	const float
		fYaw = atan2f(ComRot.x, ComRot.z), // Yawを求める
	    fPitch = atan2f(ComRot.y, sqrtf(powf(ComRot.x, 2) + powf(ComRot.z, 2))); // Pitchを求める

	CSound* pSound = CManager::GetSound(); // サウンドへのポインタ

	// === 処理 ===

	pSound->PlaySoundB(CSound::SOUND_LABEL::SE_LANDING_000); // 着地音を鳴らす

	// YawとPitchがカメラの向きと一致していたら後ろを向かせる
	if (CameraRot.x > fPitch - 0.5f && CameraRot.x < fPitch + 0.5f &&
		CameraRot.y > fYaw - 0.5f && CameraRot.y < fYaw + 0.5f)
	{
		pCamera->ChengeState(DBG_NEW CCameraState_TurnAround(D3DXVECTOR3(-fPitch, fYaw + D3DX_PI, 0.0f), 0.15f));
	}
	//==============================================================================================

}
//=========================================================================================================================================================

//===================================================================================================================
//デストラクタ
//===================================================================================================================
CPlayerActionMode_Stuck::~CPlayerActionMode_Stuck()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//処理
//===================================================================================================================
void CPlayerActionMode_Stuck::Process(CPlayer* pPlayer)
{
	pPlayer->GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); //引っ付いているので動きを止める

	//*変数宣言
	CCamera* pCamera = CManager::GetCamera();                      //カメラへのポインタ
	CWire* pWire = pPlayer->GetWire();                             //ワイヤーへのポインタ
	CWireHead* pWireHead = pWire->GetWireHead();                   //ワイヤーヘッドへのポインタ
	CLockon* pLockon = pPlayer->GetLockOn();                       //ロックオンへのポインタ
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //ジョイパッド入力情報を取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();          //マウス入力情報を取得
	pWireHead->GetPosInfo().SetPos(pPlayer->GetPosInfo().GetPos());//ダイブ準備中なのでワイヤーヘッドをプレイヤーの位置に固定

	if (pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::B) || pInputMouse->GetMouseMiddleClickTrigger())
	{//引っ付きながら射撃する
		m_bStartWireShot = m_bStartWireShot ? false : true;     //フラグのONOFFを変える
		if (m_bStartWireShot == false)
		{//m_bStartWireShotをtrueにしたときにステートが変わらないようにするため、falseの時だけステートを変える
			pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Shot());//攻撃ステートを「射撃」に変更する
			pLockon->ChengeTexture(CLockon::TYPE::STUCKSHOT);       //ターゲットのテクスチャをひっつき射撃仕様に変更
		}
		else
		{
			pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());//攻撃ステートを「なし」に変更
			pLockon->ChengeTexture(CLockon::TYPE::DIVE);            //ターゲットのテクスチャをダイブ仕様に戻す
		}
	}

	if ((pInputJoypad->GetRT_Trigger() || pInputMouse->GetMouseLeftClickTrigger()) && pLockon->GetSuccessRayCollision() && m_bStartWireShot)
	{//ジョイパッドの「右トリガーボタン」又は「マウスの右クリックボタン」を押しているかつ「ロックオンのレイが当たっている」かつワイヤー発射ができる(true)なら
		pPlayer->ChengeActionMode(DBG_NEW CPlayerActionMode_WireShot(pPlayer));//アクションモードをワイヤー発射に変える
	}

}
//=========================================================================================================================================================