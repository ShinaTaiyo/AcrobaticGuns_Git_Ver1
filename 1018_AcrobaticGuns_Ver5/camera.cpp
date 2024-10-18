//===================================================================
//
//６月３日：3Dオブジェクトを描画する[Camera.cpp]
//Author:ShinaTaiyo
//
//===================================================================

//===================================================================
//インクルード
//===================================================================
#include "camera.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "object.h"
#include "game.h"
#include "edit.h"
//====================================================================================================

//====================================================================
//静的メンバ宣言
//====================================================================
const float CCamera::m_BESIDECAMERALENGTH = 570.0f;//ビサイドビューのカメラの距離
//====================================================================================================

//====================================================================
//コンストラクタ
//====================================================================
CCamera::CCamera() : m_SupportPos(NULL_VECTOR3),m_fLength(0.0f), m_fTurningRotSpeed(0.0f),m_fTurningSpeedY(0.0f),m_PosV(NULL_VECTOR3),
m_PosR(NULL_VECTOR3),m_VecU(NULL_VECTOR3),m_Rot(NULL_VECTOR3),m_mtxProjection(),m_mtxView(),m_CameraType(CAMERATYPE_BIRD),m_DifferenceLength(NULL_VECTOR3),
m_ZoomSpeed(NULL_VECTOR3),m_nShakeFrame(0),m_ModeTime(0),m_fShakePower(0.0f),m_fAddLength(0.0f)
{

}
//====================================================================================================

//====================================================================
//デストラクタ
//====================================================================
CCamera::~CCamera()
{

}
//====================================================================================================

//====================================================================
//初期化処理
//====================================================================
HRESULT CCamera::Init()
{
	m_PosV = D3DXVECTOR3(0.0f, 200.0f,-400.0f);     //支点
	m_PosR = NULL_VECTOR3;                          //注視点
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);         //上方向ベクトル（法線）
	m_Rot = NULL_VECTOR3;                           //向き
	m_mtxProjection = {};                           //プロジェクションマトリックス
	m_mtxView = {};                                 //ビューマトリックス
	m_CameraType = CAMERATYPE_BIRD;                 //カメラモードの種類
	m_nShakeFrame = 0;                              //カメラを揺らすフレーム数
	m_fShakePower = 0.0f;                           //カメラを揺らす力
	m_DifferenceLength = NULL_VECTOR3;              //対象との距離
	m_ModeTime = 0;                                 //モードの時間
	m_ZoomSpeed = NULL_VECTOR3;                     //ズームする速さ
	return S_OK;
}
//====================================================================================================

//====================================================================
//終了処理
//====================================================================
void CCamera::Uninit()
{

}
//====================================================================================================


//====================================================================
//更新処理
//====================================================================
void CCamera::Update()
{
	//========================================
	//カメラの位置を変える
	//========================================
	if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{
		m_Rot.y += 0.02f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{
		m_Rot.y -= 0.02f;
	}

	//=========================o==============
	//カメラを揺らす
	//========================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F1) == true)
	{
		SetShake(50, 0.2f);
	}

	//カメラの通常の注視点を設定し続ける
	NormalCameraMove();

	//=================================================================================================================================

}
//====================================================================================================


//====================================================================
//カメラ設定処理
//====================================================================
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice;                       //デバイスへのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();  //デバイスの取得

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	if (m_CameraType == CAMERATYPE_BIRD || m_CameraType == CAMERATYPE_BESIDE)
	{
		//プロジェクションマトリックスを作成z
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			10.0f,
			20000.0f);
	}
	else if (m_CameraType == CAMERATYPE_SIDE)
	{
		//平行東映（正射影）
		D3DXMatrixOrthoLH(
			&m_mtxProjection,
			(float)SCREEN_WIDTH / 2,
			(float)SCREEN_HEIGHT / 2,
			10.0f,
			2500.0f);
	}
	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//==========================
	//カメラを揺らす処理
	//==========================
	D3DXVECTOR3 adjust = NULL_VECTOR3;//揺れの補正
	D3DXVECTOR3 ResultPosV = m_PosV;//結果的な支点
	D3DXVECTOR3 ResultPosR = m_PosR;//結果的な注視点
	if (m_nShakeFrame > 0)
	{
		m_nShakeFrame--;
		adjust.x += (float)(rand() % 100 - 50) * m_fShakePower;
		adjust.y += (float)(rand() % 100 - 50) * m_fShakePower;

		ResultPosV += adjust;
		ResultPosR += adjust;
	}
	//========================================================================================
		//ビューマトリックスの作成
		D3DXMatrixLookAtLH(&m_mtxView,
			&ResultPosV,
			&ResultPosR,
			&m_VecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW,&m_mtxView);

}
//====================================================================================================

//====================================================================
//カメラ設定処理
//====================================================================
void CCamera::SetShake(int nShakeFrame, float fShakePower)
{
	m_nShakeFrame = nShakeFrame;
	m_fShakePower = fShakePower;
}
//====================================================================================================

//====================================================================
//普通のカメラの位置を設定し続ける
//====================================================================
void CCamera::NormalCameraMove()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		switch (m_CameraType)
		{
		case CAMERATYPE_BIRD:
			if (CGame::GetPlayer() != nullptr)
			{
				m_PosR = CGame::GetPlayer()->GetPos();
				m_PosV = m_PosR + D3DXVECTOR3(sinf(m_Rot.y) * -300.0f, 300.0f, cosf(m_Rot.y) * -300.0f);
			}
			break;
		default:
			break;
		}
	}
}
//====================================================================================================

//====================================================================
//ボスを倒したときのカメラの処理を行う
//====================================================================
void CCamera::BossDefeatCameraProcess()
{
	float fVXaim = 0.0f;//X方向ベクトル
	float fVYaim = 0.0f;//Y方向ベクトル
	float fVZaim = 0.0f;//Z方向ベクトル
	float fVLaim = 0.0f;//総合ベクトル
	float fLength = 0.0f;
	CObject* pObj = nullptr;
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;

	m_ModeTime++;
}
//====================================================================================================

//====================================================================
//カメラの旋回処理を行う
//====================================================================
void CCamera::TurningCameraProcess()
{
	//旋回向き
	m_Rot.y += m_fTurningRotSpeed;

	//支点Yの位置
	m_PosV.y += m_fTurningSpeedY;

	//距離を更新する
	m_fLength += m_fAddLength;

	//カメラの旋回処理
	m_PosV.x = sinf(m_Rot.y) * m_fLength + m_PosR.x;
	m_PosV.z = cosf(m_Rot.y) * m_fLength + m_PosR.z;
}
//====================================================================================================