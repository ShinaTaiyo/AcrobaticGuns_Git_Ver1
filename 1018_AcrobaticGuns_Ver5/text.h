//=========================================================
//
//���U���g��ʂŎg���ϐ��A�錾�A�\����[text.h]
// Author seiya kagaya
//
//=========================================================
#ifndef _TEXT_H

#define _TEXT_H_//��d�C���N���[�h�h�~

#include"main.h"

class CText
{
public:
	//�t�H���g�̎��
    //--------------------�t�H���g�̃��x���I��
	typedef enum
	{
		FONT_DOKIDOKI = 0,//�ǂ��ǂ��t�@���^�W�A
		FONT_AKABARASINDELERA,//���K�N�V���f����
		FONT_KEIFONT,//�����ӂ����
		FONT_SOUKOUMINCHO,//���b����
		FONT_KIRARAJI04,//����玚04
		FONT_MAX,
	}FONT;
    #define MAXFONT (FONT_MAX)//---�t�H���g�̐�

	//�v���g�^�C�v�錾
	CText();              //�R���X�g���N�^
    ~CText();             //�f�X�g���N�^ 
	void Init(void);  //����������
	void Uninit(void);//�I������
	void DrawSet(D3DXVECTOR3 DrawPos, int FontSize, FONT Font, D3DXCOLOR RGBA, const char* Text, ...);//�e�L�X�g�����o������
private:

    //�t�H���g�\����
	typedef struct
	{
		const char* pFilePass;	// �t�@�C���p�X
		const char* pFontName;	// �t�H���g�l�[��
	} FONTINFO;

	// �t�H���g�̏��
	//-------------------------------------------
	//�@�@"�t�@�C���p�X","�t�H���g��"
	//-------------------------------------------
	static const FONTINFO m_FontInfo[FONT_MAX];
	LPD3DXFONT m_pFont_Comment[MAXFONT] = {};//�t�H���g�ւ̃|�C���^!
};


#endif // !_TEXT_H_