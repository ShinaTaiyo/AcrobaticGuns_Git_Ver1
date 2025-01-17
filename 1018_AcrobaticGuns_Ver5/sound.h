//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//==============================================================================
//�T�E���h�N���X
//==============================================================================
class CSound
{
public:
	//==============================================================================
	// �T�E���h�ꗗ
    //==============================================================================
    enum class SOUND_LABEL
	{
		BGM_NOESIS,       //Noesis
	    SE_SHOT_000,      //�ˌ��O
		SE_SHOT_001,      //�ˌ��P
	    SE_EXPLOSION_000, //�����O
		SE_DAMAGE_000,    //�_���[�W�O
		MAX,
	};
	//=================================================================================================================================

	//*****************************************************************************
	// �v���g�^�C�v�錾
	//*****************************************************************************
	CSound();                            //�R���X�g���N�^
	~CSound();                           //�f�X�g���N�^
	HRESULT InitSound(HWND hWnd);        //����������
	void UninitSound(void);              //�I������
	HRESULT PlaySound(SOUND_LABEL label);//�T�E���h�Đ�����
	void StopSound(SOUND_LABEL label);   //�T�E���h�X�g�b�v����
	void StopSound(void);                //�T�E���h�X�g�b�v�����i�I�[�o�[���[�h�j
	//=================================================================================================================================

private:
	//*****************************************************************************
	// �v���g�^�C�v�錾
	//*****************************************************************************
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	//=================================================================================================================================

		//*****************************************************************************
        // �T�E���h���̍\���̒�`
        //*****************************************************************************
		typedef struct
		{
			const char* pFilename;	// �t�@�C����
			int nCntLoop;		// ���[�v�J�E���g
		} SOUNDINFO;

		//*****************************************************************************
		// �O���[�o���ϐ�
		//*****************************************************************************
		IXAudio2* m_pXAudio2 = NULL;								                // XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X!
		IXAudio2MasteringVoice* m_pMasteringVoice = NULL;			                // �}�X�^�[�{�C�X!
		IXAudio2SourceVoice* m_apSourceVoice[static_cast<int>(SOUND_LABEL::MAX)];	// �\�[�X�{�C�X!
		BYTE* m_apDataAudio[static_cast<int>(SOUND_LABEL::MAX)];					// �I�[�f�B�I�f�[�^!
		DWORD m_aSizeAudio[static_cast<int>(SOUND_LABEL::MAX)];					    // �I�[�f�B�I�f�[�^�T�C�Y!

		// �T�E���h�̏��
		SOUNDINFO m_aSoundInfo[static_cast<int>(SOUND_LABEL::MAX)] =
		{
			{"data/BGM/TitleBgm_Noesis.wav", -1},   //BGM_Noesis
			{"data/SE/Shot_000.wav",  0},           //SE_�ˌ��O
			{"data/SE/Shot_001.wav",  0},           //SE_�ˌ��O
			{"data/SE/Explosion_000.wav",  0},      //SE_�����O
			{"data/SE/Damage_000.wav",  0},         //SE_�_���[�W�O
		};

};

#endif
