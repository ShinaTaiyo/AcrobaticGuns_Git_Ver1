//===============================================================================
//
//�V���P�P���F�I�u�W�F�N�g��Release���@��ύX[edit.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _EDIT_H_  
#define _EDIT_H_


//========================================
//�C���N���[�h
//========================================
#include "scene.h"
#include "player.h"
//===============================================================================

//========================================
//�G�f�B�b�g�N���X
//========================================
class CEdit : public CScene
{
public:
	CEdit();                           //�R���X�g���N�^
	~CEdit() override;                 //�f�X�g���N�^
	HRESULT Init() override;           //����������
	void Uninit() override;            //�I������
	void Update() override;            //�X�V����
	static CPlayer* GetPlayer() { return m_pPlayer; }           //�v���C���[���擾����
private:
	static CPlayer* m_pPlayer;                                             //�v���C���[
};
#endif