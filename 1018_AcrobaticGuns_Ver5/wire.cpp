//===================================================================================================================
//
//�P�P���P�V���F���C���[�����[wire.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================

//===============================================================
//�C���N���[�h
//===============================================================
#include "wire.h"
#include "texture.h"
#include "renderer.h"
#include "player.h"
#include "manager.h"
#include "debugproc.h"
#include "calculation.h"
//===================================================================================================================

//===============================================================
//�f�X�g���N�^
//===============================================================
const string CWire::s_WIRE_FILENAME[static_cast<int>(CWire::WIRETYPE::MAX)] =
{
	"data\\TEXTURE\\Wire\\LakeField_000.png"
};
//===================================================================================================================

//===============================================================
//�R���X�g���N�^
//===============================================================
CWire::CWire(WIRETYPE WireType, float fRadius, float fHeight,int nNumDivsionXZ, 
	D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, 
	int nNumDivisionY, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CMeshCylinder(fRadius,fHeight,nNumDivsionXZ,nNumDivisionY,
		Pos,Rot,
		nPri,bUseintPri,type,ObjType),m_Type(WireType),m_bUseUpdate(true),m_pWireHead(nullptr),m_pPlayer(nullptr),m_VecMtxCircle()
{
	for (int nCnt = 0; nCnt < nNumDivsionXZ + 1; nCnt++)
	{
		CirclePosInfo Info = { D3DXVECTOR3(0.0f,0.0f,0.0f),{} };
		m_VecMtxCircle.push_back(Info);
	}
}
//===================================================================================================================

//===============================================================
//�f�X�g���N�^
//===============================================================
CWire::~CWire()
{
	m_VecMtxCircle.clear();
}
//===================================================================================================================

//===============================================================
//����������
//===============================================================
HRESULT CWire::Init()
{
	CMeshCylinder::Init();

	//���C���[�w�b�h�𐶐�
	m_pWireHead = CWireHead::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 120);
	m_pWireHead->SetAutoSubLife(false);
	m_pWireHead->SetUseDeath(false);
	m_pWireHead->SetUseDraw(true);

	return S_OK;
}
//===================================================================================================================

//===============================================================
//�I������
//===============================================================
void CWire::Uninit()
{
	CMeshCylinder::Uninit();

	if (m_pPlayer != nullptr)
	{
		m_pPlayer = nullptr;
	}
}
//===================================================================================================================

//===============================================================
//�X�V����
//===============================================================
void CWire::Update()
{
	CMeshCylinder::Update();
	const D3DXVECTOR3& Pos = GetPos();
	//const D3DXVECTOR3* pSenterPos = GetSenterPos();
	const int& nNumDivisionXZ = GetNumDivisionXZ();//XZ������
	const int& nNumDivisionY = GetNumDivisionY();  //Y������
	const int& nNumVtx = GetNumVtx();              //���_��
	const int& nNumIdx = GetNumIdx();              //�C���f�b�N�X��
	const float& fRadius = GetRadius();            //���a
	const float& fHeight = GetHeight();            //����

	float fLength = CCalculation::CalculationLength(m_pPlayer->GetSenterPos(), m_pWireHead->GetSenterPos());
	SetHeight(fLength);
	//CManager::GetDebugProc()->PrintDebugProc("�����F%f\n", fLength);

	VERTEX_3D* pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBufferPointer();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_bUseUpdate == true)
	{
		int nCntArray = nNumVtx - 1;
		float fRatioXZ = 0.0f;
		float fRatioY = 0.0f;
		D3DXVECTOR3 LastSenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 MeasureNor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�@���v�Z�p
		//D3DXVECTOR3 Rot = m_pWireHead->GetRot();
		//D3DXVec3Normalize(&Rot, &Rot);
		for (int nCntVtxY = 0; nCntVtxY < nNumDivisionY; nCntVtxY++)
		{//Y������UV�͂��̂܂܎g��
			fRatioY = (1.0f / (nNumDivisionY - 1)) * nCntVtxY;
			for (int nCntVtxXZ = 0; nCntVtxXZ < nNumDivisionXZ + 1; nCntVtxXZ++)
			{//X������UV�͏d�Ȃ钸�_������̂ŁA+ 1
				fRatioXZ = (1.0f / (nNumDivisionXZ)) * nCntVtxXZ;
				if (nCntVtxY == 0 && nCntVtxXZ == 0)
				{//�ŏ��̎��Ȃ̂ŏ�ʂ̒��S�_��ݒ肷��
					pVtx[nCntArray].pos = GetSenterPos(nNumDivisionY - 2);//Y����������3�̏ꍇ�A��� = 1
					nCntArray--;
				}

				if (nCntVtxY == nNumDivisionY - 1)
				{//�Ō�̎��Ŋ�_�����߂�i9,8,7,6,5,4,3,2,1)
					pVtx[nCntArray].pos = m_VecMtxCircle[nCntVtxXZ].Pos;
					//CManager::GetDebugProc()->PrintDebugProc("���_%d�F%f %f %f\n",nCntVtxXZ,pVtx[nCntArray].pos.x, pVtx[nCntArray].pos.y, pVtx[nCntArray].pos.z);
					/* D3DXVECTOR3(sinf((D3DX_PI * 2) * fRatioXZ) * fRadius,fHeight, cosf((D3DX_PI * 2) * fRatioXZ) * fRadius);*/
				}
				else
				{//��_�ɑ΂��ċO�Օ��ɒ��_�������Ă���(18 = 27)
					pVtx[nCntArray].pos = pVtx[nCntArray - (nNumDivisionXZ + 1)].pos;
				}

				if (nCntVtxXZ == 0 && nCntVtxY != nNumDivisionY - 1)
				{//�Ō�̈���ȊO�Ȃ�e�w�̒��S�_���X�V
					SetSenterPos(nNumDivisionY - nCntVtxY - 1, GetSenterPos(nNumDivisionY - nCntVtxY - 2));
				}

				//�z��J�E���g
				nCntArray--;

				if (nCntArray < 0 || nCntArray >= nNumVtx)
				{//�z��O�A�N�Z�X�`�F�b�N
					int s = 0;
				}

				if (nCntVtxY == nNumDivisionY - 1 && nCntVtxXZ == nNumDivisionXZ)
				{//�Ō�
					pVtx[nCntArray].pos = m_pWireHead->GetPos();//��ʂ̒��S�Ɉʒu��ݒ�
					CManager::GetDebugProc()->PrintDebugProc("���S���_�F%f %f %f\n", pVtx[nCntArray].pos.x, pVtx[nCntArray].pos.y, pVtx[nCntArray].pos.z);
					SetSenterPos(0,m_pWireHead->GetPos());
				}
			}
		}
	}
	//CManager::GetDebugProc()->PrintDebugProc("�X�V���邩�ǂ����F%d\n",m_bUseUpdate);

	//for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	//{
	//	CManager::GetDebugProc()->PrintDebugProc("���_���F%d�F%f %f %f\n", nCntVtx, pVtx[nCntVtx].pos.x, pVtx[nCntVtx].pos.y, pVtx[nCntVtx].pos.z);
	//}
	//���_�o�b�t�@���A�����b�N���� 
	pVtxBuff->Unlock();
}
//===================================================================================================================

//===============================================================
//�`�揈��
//===============================================================
void CWire::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�p�}�g���b�N�X
	D3DXMATRIX& mtxWorld = GetMtxWorld();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBufferPointer();
	LPDIRECT3DINDEXBUFFER9 pIdxBuff = GetIdxBufferPointer();
	LPDIRECT3DTEXTURE9 pTexture = GetTexture();
	const int& nNumPolygon = GetNumPolygon();
	const int& nNumVtx = GetNumVtx();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,0.0f,0.0f,0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//========================================================================================
	//�~��ɓ_��z�u���邽�߂̃��[���h���W�����߂�
	//========================================================================================
	if (m_pWireHead != nullptr)
	{
		D3DXVECTOR3 PosZero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���[���h���W��������Ŏg�p����
		D3DXVECTOR3 WireHeadRot = m_pWireHead->GetRot();//���C���[�w�b�h�̌���
		D3DXVECTOR3 WireHeadPos = m_pWireHead->GetPos();//���C���[�w�b�h�̈ʒu
		CManager::GetDebugProc()->PrintDebugProc("���C���[�w�b�h�����F%f %f %f\n", WireHeadRot.x,WireHeadRot.y,WireHeadRot.z);
		int Size = m_VecMtxCircle.size();

		for (int nCnt = 0; nCnt < Size; nCnt++)
		{
			D3DXMATRIX mtxSenter;
			D3DXMatrixIdentity(&mtxSenter);

			//���������߂�
			float fRatio = (2.0f / (Size - 1)) * nCnt;

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_VecMtxCircle[nCnt].WorldMtx);

			//�����𔽉f�i���C���[�w�b�h�������Ă�������Ɂj
			D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);
			D3DXMatrixMultiply(&m_VecMtxCircle[nCnt].WorldMtx, &m_VecMtxCircle[nCnt].WorldMtx, &mtxRot);

			//�ʒu�𔽉f�i���C���[�w�b�h�𒆐S�ɉ~��Ɂj
			D3DXMatrixTranslation(&mtxTrans,
				sinf(D3DX_PI * fRatio) * GetRadius(),
				0.0f,
				cosf(D3DX_PI * fRatio) * GetRadius());
			D3DXMatrixMultiply(&m_VecMtxCircle[nCnt].WorldMtx, &m_VecMtxCircle[nCnt].WorldMtx, &mtxTrans);

			//���C���[�w�b�h�ƃ��[���h�ϊ��s����|�����킹��
			D3DXMatrixMultiply(&m_VecMtxCircle[nCnt].WorldMtx, &m_VecMtxCircle[nCnt].WorldMtx,&m_pWireHead->GetMatrixWorld());

			//���[���h���W����
			D3DXVec3TransformCoord(&m_VecMtxCircle[nCnt].Pos, &PosZero, &m_VecMtxCircle[nCnt].WorldMtx);

			CManager::GetDebugProc()->PrintDebugProc("���_%d�F%f %f %f\n", nCnt, m_VecMtxCircle[nCnt].Pos.x, m_VecMtxCircle[nCnt].Pos.y, m_VecMtxCircle[nCnt].Pos.z);
		}
	}
	//=======================================================================================================================

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//���ʂ�`�悷��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture);

	if (GetUseDraw() == true)
	{
		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, nNumVtx, 0, nNumPolygon);
	}
	//�Жʂ����`�悷��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//===================================================================================================================

//===============================================================
//���S�t���O�ݒ菈��
//===============================================================
void CWire::SetDeath()
{
	if (m_pWireHead != nullptr)
	{
		m_pWireHead->SetUseDeath(true);
		m_pWireHead->SetDeath();
		m_pWireHead = nullptr;
	}

	CObject::SetDeath();
}
//===================================================================================================================

//===============================================================
//��������
//===============================================================
CWire* CWire::Create(WIRETYPE Type, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, float fRadius, float fHeight, int nNumDivisionXZ, int nNumDivisionY)
{
	CTexture* pTexture = CManager::GetTexture();
	CWire* pWire = DBG_NEW CWire(Type, fRadius, fHeight, nNumDivisionXZ, Pos, Rot, nNumDivisionY);
	pWire->Init();//����������
	pWire->SetTextureIndex(pTexture->Regist(s_WIRE_FILENAME[static_cast<int>(Type)]));
	pWire->BindTexture(pTexture->GetAddress(pWire->GetTextureIndex()));

	return pWire;
}
//===================================================================================================================