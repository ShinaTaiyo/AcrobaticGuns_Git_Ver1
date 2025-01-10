//==========================================================
//
// �f�o�b�O�\������ [debugproc.cpp]
// Author : Ibuki Okusada
//
//==========================================================

//==========================================================
//�C���N���[�h
//==========================================================
#include "main.h"
#include "debugproc.h"
#include <string.h>
#include <stdio.h>
#include "renderer.h"
#include "manager.h"
#include "input.h"
//====================================================================================================================================

//============================================================
//�R���X�g���N�^
//============================================================
CDebugProc::CDebugProc() : m_aStrDebug(),m_pFont2(), m_bDispDebug(false)
{

}
//====================================================================================================================================

//============================================================
//�f�X�g���N�^
//============================================================
CDebugProc::~CDebugProc()
{

}
//====================================================================================================================================

//============================================================
//����������
//============================================================
HRESULT CDebugProc::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�ւ̃|�C���^���擾

    //�f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont2);

	//�f�o�b�O�\�����̃N���A
	memset(&m_aStrDebug[0], NULL, sizeof(m_aStrDebug));

	//�����\���ݒ�
#ifdef _DEBUG
	m_bDispDebug = true;
#else NDEBUG
	m_bDispDebug = false;
#endif

	return S_OK;
}
//====================================================================================================================================

//============================================================
//�I������
//============================================================
void CDebugProc::Uninit()
{
	//�f�o�b�O�\���p�t�H���g�̔p��
	if (m_pFont2 != nullptr)
	{
		m_pFont2->Release();
		m_pFont2 = nullptr;
	}
}
//====================================================================================================================================

//============================================================
//�X�V����
//============================================================
void CDebugProc::Update()
{
	//��������ݒ�
	SetManual();

	//if (CManager::GetInputKeyboard()->GetTrigger() GetKeyboardTrigger(DIK_F1) == true)
	//{//F1�L�[�������ꂽ�Ƃ�
	//	g_bDispDebug = g_bDispDebug ? false : true;
	//}
}
//====================================================================================================================================

//============================================================
//�`�揈��
//============================================================
void CDebugProc::Draw()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (m_bDispDebug == true)
	{//�f�o�b�N���[�h���I���̎�
	 //�e�L�X�g�̕`��
		m_pFont2->DrawText(NULL, &m_aStrDebug[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	//�f�o�b�O�\�����̃N���A
	memset(&m_aStrDebug[0], NULL, sizeof(m_aStrDebug));
}
//====================================================================================================================================

//============================================================
//�f�o�b�O�\������
//============================================================
void CDebugProc::PrintDebugProc(const char* fmt, ...)
{
	if (m_bDispDebug == true)
	{
		va_list args;
		char aString[MAX_DEBUGSTRING];
		char aSaveString[MAX_DEBUGSTRING];
		int nLength = 0;
		int nStopLength;

		//������̑��
		strcpy(&aString[0], fmt);

		va_start(args, fmt);

		for (char* p = &aString[0]; *p != '\0'; p++)
		{
			if (*p == '%')
			{//�m�F����������ꍇ
				p++;	//�m�F�����܂Ői�߂�

				switch (*p)
				{
				case 'd':	//����
					snprintf(&aSaveString[0],100, "%d", va_arg(args, int));

					//������̒������擾
					nLength = (int)strlen(&aSaveString[0]);
					break;
				case 'f':	//����
					snprintf(&aSaveString[0],100, "%f", va_arg(args, double));

					//������̒������擾
					nLength = (int)strlen(&aSaveString[0]);

					//�����_�ȉ��̕����ڂ܂Ŋm�F
					for (int nCntlength = 0; aSaveString[nCntlength] != '\0'; nCntlength++)
					{
						if (aSaveString[nCntlength] == '.')
						{//�����_���������ꍇ

							//�����_�ȉ��̌��������߂�
							int nMin = (int)strlen(&aSaveString[nCntlength + 1]);

							//�����_����̕������Ǝw�肵���l�܂ł̍��������߂�
							nMin -= MAX_FLOATNUM;

							//������̒����������_�ȉ����ʂ܂łɕ␳
							nLength -= nMin;
						}
					}

					break;
				case 'c':	//����
					snprintf(&aSaveString[0],100, "%c", va_arg(args, char));

					//������̒������擾
					nLength = (int)strlen(&aSaveString[0]);
					break;
				case 's':	//������
					vsnprintf(&aSaveString[0], sizeof(aSaveString), "%s", args);

					//������̒������擾
					nLength = (int)strlen(&aSaveString[0]);
					break;
				}

				nStopLength = (int)strlen(p) + 1;
				//���������̊m�F���������̕���������炷
				memmove(p + nLength - 2, p, nStopLength);

				p--;	//�|�C���^��%�܂Ŗ߂�

				//�ψ������w�肳�ꂽ�ꏊ�ɑ}��
				memcpy(p, &aSaveString[0], nLength);
			}
		}

		va_end(args);

		//�������A������
		strcat(&m_aStrDebug[0], &aString[0]);

		int strLength = std::strlen(&m_aStrDebug[0]);
		if (strLength >= MAX_DEBUGSTRING)
		{
			assert(false);
		}
	}
}
//====================================================================================================================================

//============================================================
//�������
//============================================================
void CDebugProc::SetManual()
{
	//�f�o�b�O�ύX��FPS
}
//====================================================================================================================================