//========================================================
//
//�U���Q�P���F���b�V���V�����_�[�Ŕw�i���o��[meshcylinder.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

//========================================
//�C���N���[�h
//========================================
#include "main.h"
#include "object.h"
//===========================================================

class CMeshCylinder : public CObject
{
public:
	CMeshCylinder(const float fRadius,const float fHeight,const int nNumDivisionXZ,const int nNumDivisionY,const D3DXVECTOR3 Pos,const D3DXVECTOR3 Rot, int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_3D);                               //�R���X�g���N�^
	~CMeshCylinder() override;                     //�f�X�g���N�^
	HRESULT Init() override;                       //����������
	void Uninit() override;                        //�I������
	void Update() override;                        //�X�V����
	void Draw() override;                          //�`�揈��
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture); //�e�N�X�`�������蓖�Ă�
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }//�e�N�X�`���ԍ������蓖�Ă�4
	int GetTextureIndex() { return m_nTextureIndex; }         //�e�N�X�`���ԍ����擾����

	//�ʒu
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }
	const D3DXVECTOR3 & GetPos() const { return m_Pos; }

	//����
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }
	const D3DXVECTOR3& GetRot() const { return m_Rot; }
private:
	//typedef struct
	//{
	//	int nNumBlockXZ;                   //X�����̃u���b�N��
	//	int nNumBlockY;                    //Y�����̃u���b�N��
	//	int nNumVtxXZ;                     //XZ����������
	//	int nDivisionY;                    //Y����������
	//	float fRadius;                     //���a
	//	float fHeight;                     //����
	//	int nNumVtx;                       //���_��
	//	int nNumIdx;                       //�C���f�b�N�X��
	//	int nNumPolygon;                   //�|���S����
	//}Info;

	PDIRECT3DVERTEXBUFFER9 m_pVtxBuff;   //���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;   //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTexture;       //�e�N�X�`���ւ̃|�C���^

	//�傫���i���I�ɕω�����\��������j
	float m_fRadius;               //���a
	float m_fHeight;               //����

	//������
	const int m_nNumDivisionXZ;          //XZ����������
	const int m_nNumDivisionY;           //Y����������

	//�\���v�f��
	int m_nNumPolygon;             //�|���S����
	const int m_nNumVtx;                 //���_��
	const int m_nNumIdx;                 //�C���f�b�N�X��

	//���[���h���
	D3DXVECTOR3 m_Pos;                   //�ʒu
	D3DXVECTOR3 m_Rot;                   //����
	D3DXMATRIX m_mtxWorld;               //���[���h�}�g���b�N�X

	//�e�N�X�`��
	int m_nTextureIndex;                 //�e�N�X�`���ԍ���ݒ肷��

	//���_�m�F�p
	int m_nCheckVtx;
	int m_nCheckIdx;

	//=============================================
	//�֐�
	//=============================================
	void SetVertexInfo(VERTEX_3D* pVtx,float & fRatioXZ,float & fRatioY,int &nCntVtxXZ,int & nCntVtxY);
	void ChengeNumPolygon();//�|���S������ς��鏈��
	void CheckMeshInfo();//���b�V���̂��ꂼ��̏����m�F����
}; 
#endif

