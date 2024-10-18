//=======================================================================================
//
//�U���S���F���f����`�悷��[field.h]
//Author:ShinaTaiyo
//
//=======================================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

//==============================================
//�C���N���[�h
//==============================================
#include "main.h"
#include "object.h"
#include "object3d.h"
//=======================================================================================

//=======================================================
//�t�B�[���h�N���X
//=======================================================
class CField : public CObject3D
{
public:
	typedef enum
	{
		FIELDTYPE00_NORMAL = 0,//����
		FIELDTYPE01_FLOOR,     //�t���A
		FIELDTYPE_MAX
	}FIELDTYPE;

	CField();                                                                       //�R���X�g���N�^
	~CField() override;                                                             //�f�X�g���N�^
	HRESULT Init() override;                                                        //����������
	void Uninit() override;                                                         //�I������
	void ExtraUninit() override;                                                    //�ʘg�̏I������
	void Update() override;                                                         //�X�V����
	void Draw() override;                                                           //�`�揈��
	void SetDeath() override;                                                       //���S�t���O��ݒ�
	static CField* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight,FIELDTYPE FieldType);  //��������

private:
	FIELDTYPE m_FieldType;                                                          //�t�B�[���h�̎��
	static const char* m_apFIELD_FILENAME[FIELDTYPE_MAX];                           //�t�B�[���h�̃e�N�X�`���̃t�@�C����
};
//============================================================================================================

#endif