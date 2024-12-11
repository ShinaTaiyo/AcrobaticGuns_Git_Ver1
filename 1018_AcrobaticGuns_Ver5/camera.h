//==========================================================================
//
//�U���R���F3D�I�u�W�F�N�g��`�悷��[camera.h]
//Author:ShinaTaiyo
//
//==========================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//==========================================================================
//�C���N���[�h
//==========================================================================
#include "main.h"
//==========================================================================

//==========================================================================
//�O���錾
//==========================================================================
class CCameraState;

//==========================================================================
//�J�����N���X
//==========================================================================
class CCamera
{
public:

	//===================================================
	//�J�������[�h�񋓌^
	//===================================================
	typedef enum
	{
		CAMERATYPE_NONE = 0,  //�Ȃ�
		CAMERATYPE_BIRD,      //�o�[�h
		CAMERATYPE_BESIDE,    //�r�T�C�h
		CAMERATYPE_SIDE,      //�T�C�h
		CAMERATYPE_MAX
	}CAMERATYPE;
	//=================================================================================================

	//===================================================
	//�J������ԗ񋓌^
	//===================================================
	enum class CAMERASTATE
	{
		NORMAL = 0,
		TURNAROUND,
		MAX
	};

	CCamera();            //�R���X�g���N�^
	~CCamera();           //�f�X�g���N�^
	HRESULT Init();       //����������
	void Uninit();        //�I������
	void Update();        //�X�V����
	void SetCamera();     //�J�����̐ݒ�

	void SetPosR(D3DXVECTOR3 PosR) { m_PosR = PosR; }  //�����_��ݒ肷��
	void SetRot(const D3DXVECTOR3 Rot) { m_Rot = Rot; }//������ݒ�
	const D3DXVECTOR3& GetRot() const{ return m_Rot; } //�������擾����
	void SetShake(int nShakeFrame, float fShakePower); //�J�����̗h���ݒ肷��
	CAMERATYPE GetCameraType() { return m_CameraType; }//�J�����^�C�v���擾����
	void SetCameraType(CAMERATYPE type) { m_CameraType = type; m_ModeTime = 0; }//�J�����^�C�v��ݒ肷��
	void SetPosV(D3DXVECTOR3 PosV) { m_PosV = PosV; }//���_��ݒ�
	D3DXVECTOR3& GetPosV() { return m_PosV; }      //���_���擾
	D3DXVECTOR3& GetPosR() { return m_PosR; }      //�����_���擾
	void SetTurningInfo(float fLength, float fRotSpeed, D3DXVECTOR3 PosR,float fPosVY,float fTurningSpeedY,float fAddLength)
	{
		m_fLength = fLength;            //�����_�Ƃ̋���
		m_fTurningRotSpeed = fRotSpeed; //��]���x
		m_PosR = PosR;                  //�����_
		m_PosV.y = fPosVY;              //�x�_��Y�ʒu
		m_fTurningSpeedY = fTurningSpeedY;//���񒆂�Y�����̈ړ���
		m_fAddLength = fAddLength;      //���Z����
	}
	void SetLength(float fLength) { m_fLength = fLength; }
	float& GetLength() { return m_fLength; }

	void SetTurningRotSpeed(float fTurningRotSpeed) { m_fTurningRotSpeed = fTurningRotSpeed; }
	float& GetTurningRotSpeed() { return m_fTurningRotSpeed; }

	void SetTurningSpeedY(float fTurningSpeedY) { m_fTurningSpeedY = fTurningSpeedY; }
	float& GetTurningSpeedY() { return m_fTurningSpeedY; }

	void SetAddLength(float fAddLength) { m_fAddLength = fAddLength; }
	float& GetAddLength() { return m_fAddLength; }

	void SetCustomMode(bool bActive) { m_bCustom = bActive; }

	void ChengeState(CCameraState * pCameraState);//�X�e�[�g��ς���

	//======================================
	//�}�g���b�N�X�n
	//======================================
	D3DXMATRIX* GetMtxView() { return &m_mtxView; }//�r���[�}�g���b�N�X���擾
	D3DXMATRIX* GetMtxProjection() { return &m_mtxProjection; }//�v���W�F�N�V�����}�g���b�N�X���擾
	//==========================================================================================================================================
private:
	//======================================
    //�ÓI�����o�錾
    //======================================
	static const float m_BESIDECAMERALENGTH;      //�r�T�C�h���[�h�̃J�����̋���
	static const float s_fINITIAL_LENGTH;         //�ŏ��̋���
	static const float s_fNORMAL_AROUNDROTSPEED;  //�J�����̉�]���x
	//==========================================================================================================================================

	//======================================
	//�ϐ��錾
	//======================================
	D3DXVECTOR3 m_PosV;                           //���_!
	D3DXVECTOR3 m_PosR;                           //�����_!
	D3DXVECTOR3 m_AddPosR;                        //���Z�����_
	D3DXVECTOR3 m_AddPosV;                        //���Z���_
	D3DXVECTOR3 m_VecU;                           //������x�N�g��!
	D3DXVECTOR3 m_Rot;                            //����!
	D3DXMATRIX m_mtxProjection;                   //�v���W�F�N�V�����s��!
	D3DXMATRIX m_mtxView;                         //�r���[�}�g���b�N�X!
	CAMERATYPE m_CameraType;                      //�J�������[�h�̎��!
	D3DXVECTOR3 m_DifferenceLength;               //����!
	D3DXVECTOR3 m_ZoomSpeed;                      //�Y�[�����鑬��!
	D3DXVECTOR3 m_SupportPos;                     //�Q�l�ʒu!

	CAMERASTATE m_State;                          //��Ԗ�
	CCameraState* m_pCameraState;                 //�J�����̏�ԃN���X

	float m_fLength;                              //����!
	float m_fAddLength;                           //�ǉ�����!
	float m_fTurningRotSpeed;                     //���񑬓x!
	float m_fTurningSpeedY;                       //���񒆂�Y�����̈ړ���!

	int m_nShakeFrame;                            //�J������h�炷�t���[����!
	int m_ModeTime;                               //�J�������[�h�̎���!
	float m_fShakePower;                          //�J������h�炷��!

	bool m_bCustom;                               //�J�����̋������J�X�^���ɂ��邩�ǂ���
	//==========================================================================================================================================

	//======================================
	//�v���g�^�C�v�錾
	//======================================
	void BossDefeatCameraProcess();               //�{�X��|�����Ƃ��̃J�����̏������s��
	void TurningCameraProcess();                  //����̃J�����̏������s��
	void NormalCameraMove();                      //���ʂ̃J�����̒����_��ݒ肵������
	//==========================================================================================================================================
};

class CCameraState
{
public:
	CCameraState() {};                          //�R���X�g���N�^
	virtual ~CCameraState() {};                 //�f�X�g���N�^
	virtual void Process(CCamera* pCamera) {};  //����
};

//�}�Ɍ�����������
class CCameraState_TurnAround : public CCameraState
{
public:
	CCameraState_TurnAround(float fAimRot,float fAdjustTurnSpeed);//�R���X�g���N�^
	~CCameraState_TurnAround() override;//�f�X�g���N�^
	void Process(CCamera * pCamera) override;//����
private:
	const float m_fAimRot;          //�ړI�̌���
	const float m_fAdjustTurnSpeed; //������ς��鑬�x�̕␳�x
};
#endif
