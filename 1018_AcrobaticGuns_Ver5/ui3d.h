//==================================================================================
//
//�P�O���Q�W���F�_���������ɒe�𔭎˂���[ui3d.h]
//Author:ShinaTaiyo
//
//===================================================================================
#ifndef _UI3D_H_
#define _UI3D_H_

//===================================================================================
//�C���N���[�h
//===================================================================================
#include "main.h"
#include "billboard.h"
//===================================================================================

//===================================================================================
//UI3D�N���X
//===================================================================================
class CUi3D : public CBillboard
{
public:
	typedef enum
	{
		UI3DTYPE_LOCKON = 0,
		UI3DTYPE_MAX
	}UI3DTYPE;

	CUi3D();                                                                 //�R���X�g���N�^
	~CUi3D();                                                                //�f�X�g���N�^
	HRESULT Init(void) override;                                             //����������
	void Uninit(void) override;                                              //�I������
	void Update(void) override;                                              //�X�V����
	void Draw(void) override;                                                //�`�揈��
	static CUi3D* Create(UI3DTYPE type,D3DXVECTOR3 Pos,D3DXVECTOR3 Move,int nLife,float fWidth,float fHeight); //��������
protected:
	static const string m_UI3D_FILENAME[UI3DTYPE_MAX];
	void SetUi3DType(UI3DTYPE type) { m_Type = type; }                           //��ނ�ݒ�
	UI3DTYPE GetUi3DType() { return m_Type; }                                    //��ނ��擾
private:
	UI3DTYPE m_Type;
};
//===================================================================================


#endif