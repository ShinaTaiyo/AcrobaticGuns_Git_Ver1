//============================================================================================
//
//���U���g�\�����鏈��[Text.cpp]
// Author seiya kagaya
//
//============================================================================================
#include <stdarg.h>  // �ϒ������֘A�̃w�b�_
#include "text.h"
#include "manager.h"
#include "renderer.h"
#include <stdio.h>

//============================================
//�ÓI�����o�錾
//============================================
const CText::FONTINFO CText::m_FontInfo[CText::FONT_MAX] = 
	{
		{"data\\FONT\\DokiDokiFantasia.otf", "�ǂ��ǂ��t�@���^�W�A"},
		{ "data\\FONT\\akabara-cinderella.ttf", "���K�N�V���f����" },
		{ "data\\FONT\\keifont.ttf", "�����ӂ����" },
		{ "data\\FONT\\SoukouMincho.ttf", "���b����" },
		{ "data\\FONT\\kiraraji04.ttf", "����玚04" },
	};
//=============================================================================================

//==================================================================
//�R���X�g���N�^
//==================================================================
CText::CText() : m_pFont_Comment()
{

}
//=========================================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CText::~CText()
{
}

//==================================================================
//����������
//==================================================================
void CText::Init(void)
{
	Uninit();

	//�t�H���g�̓ǂݍ��݃e�X�g//
	for (int nCnt = 0; nCnt < FONT_MAX; nCnt++)
	{
		LPCSTR font_path = m_FontInfo[nCnt].pFilePass; // �����Ŋe�t�H���g���g�p�\�Ɂi�Ŗ��R�����g�F���̃f�[�^�^�H�j


		//==============================================
		// �Ŗ��R�����g�iAddFontResourceEx�ɂ���)
		//==============================================
		//�E������LPCSTR�́A�f�[�^�^�iconst * char)�Ɠ��`
		//�E�������́AFP_PRIVATE�A���́AFM_NOT_ENUM��ݒ�o����B
	    // ��FM_PRIVATE�́A�֐����Ăяo�����v���Z�X�݂̂����̃t�H���g���g�p�ł��邱�Ƃ��w�肷��B �t�H���g�����p�u���b�N �t�H���g�ƈ�v����ƁA
        //�v���C�x�[�g �t�H���g���I���B �v���Z�X���I������ƁA AddFontResourceEx �֐����g�p���āA
		//�v���Z�X�ɂ���ăC���X�g�[�����ꂽ���ׂẴt�H���g���폜�����B
		// FR_NOT_ENUM
		//��AddFontResourceEx �֐����Ăяo�����v���Z�X���܂߁A���̃t�H���g��񋓂ł���v���Z�X���Ȃ����Ƃ��w�肷��B
		//�E��O�����́A�O���w��B
		//�E�߂�l�ɂ���
		//���֐������������ꍇ�A�߂�l�͒ǉ����ꂽ�t�H���g�̐����w�肷��B�֐������s�����ꍇ�́A0 ��Ԃ��B �g���G���[���͎g�p�ł��Ȃ��B
		///=================================================================================================================================================

		if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0)
		{
		}
		else
		{
			//�t�H���g�Ǎ��G���[����
			MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
		}
	}
}

//==================================================================
//�I������
//==================================================================
void CText::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAXFONT; nCnt++)
	{
		//UIComment�\���p�t�H���g�̔j��
		if (m_pFont_Comment[nCnt] != NULL)
		{
			m_pFont_Comment[nCnt]->Release();
			m_pFont_Comment[nCnt] = NULL;
		}
	}
}

//==================================================================
//UI�R�����g�\������
//==================================================================
void CText::DrawSet(D3DXVECTOR3 DrawPos, int FontSize, FONT Font, D3DXCOLOR RGBA, const char* Text, ...)
{
	//...�͉ϒ������iVariable Arguments�j

	 //�������厖�@�i�Ŗ��R�����g�j���񏉊������邱�Ƃɂ���āA�Ă΂��x�ɕ\������R�����g��ς���H�iQuestion)
	if (m_pFont_Comment[Font] != nullptr)
	{
		m_pFont_Comment[Font]->Release();
		m_pFont_Comment[Font] = nullptr;
	}

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�i�Ŗ��R�����g�j�f�o�C�X�̎擾

	RECT rect = { (long)(DrawPos.x),(long)(DrawPos.y),SCREEN_WIDTH,SCREEN_HEIGHT };//�i�Ŗ��R�����g�j�l�p�`�̒��ɕ�����ݒ肷��H
	char aStr[1024];

	D3DXCreateFont(pDevice,              //�i�Ŗ��R�����g�j�f�o�C�X�ւ̃|�C���^ 
		FontSize,                        //�i�Ŗ��R�����g�j�����̍���
		0,                               //�i�Ŗ��R�����g�j�����̉��� 
		FW_HEAVY,                        //�i�Ŗ��R�����g�j�����̑���
		1,                               //�i�Ŗ��R�����g�j����
		FALSE,                           //�i�Ŗ��R�����g�j�Α�
		DEFAULT_CHARSET,                 //�i�Ŗ��R�����g�j�����Z�b�g
		OUT_TT_ONLY_PRECIS,              //�i�Ŗ��R�����g�j�o�͐��x
		ANTIALIASED_QUALITY,             //�i�Ŗ��R�����g�j�����i��
		FF_DONTCARE,                     //�i�Ŗ��R�����g�j�s�b�`�ƃt�@�~��
		TEXT(m_FontInfo[Font].pFontName),//--�ǂݍ��񂾃t�H���g�̖��O(�v���p�e�B�̃^�C�g���ŕ�����)
		&m_pFont_Comment[Font]);         //�i�Ŗ��R�����g�j�\������R�����g

	// �ϒ������̏���-------
	va_list args;				 //�ϒ������̈������X�g���w�肷��i�Ŗ��R�����g�F�ϒ������̂Ȃ��ɁA%d�Ȃ�int�A%f�Ȃ�float�A%s�Ȃ�char�Ƃ�����񂪊��ɓ����Ă���j
	va_start(args, Text);		 //va_list ��������--��//�ϒ������̈�O�̈������w��
	vsprintf_s(aStr, Text, args);//�����w�蕶������g�p���ĉϒ��������當����𐶐����邽�߂̈��S�Ȋ֐��i�Ŗ��R�����g�F�����̊֐��́A�������X�g�ւ̃|�C���^�[���g�p���A�w�肳�ꂽ�f�[�^�������w�肵�� buffer ���w���������ɏ������݂܂��B�j
	va_end(args);				 //va_start �}�N���ɂ���ĊJ�n���ꂽ�ϒ��������X�g���I�����邽�߂Ɏg�p

	// �e�L�X�g�̕`��
	m_pFont_Comment[Font]->DrawText(NULL, aStr, -1, &rect, DT_LEFT | DT_WORDBREAK, RGBA);

	//�g�p��I�I

	//	DrawTextSet(�@���W�@, �t�H���g�T�C�Y�@, �t�H���g���i��.�ō�������x���I�ȁj, �J���[, "�ł���������(�C�ӂ�%d.%f,%s)",(�C�ӂŕϐ�));

	// printf���ɓ���(\n�͂܂�����...)
}
