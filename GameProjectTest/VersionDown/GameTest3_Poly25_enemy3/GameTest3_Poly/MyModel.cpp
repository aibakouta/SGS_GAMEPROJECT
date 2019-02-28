#include "MyModel.h"


float MyModel::m_fAnimationDelayTimeRimit = 0.12f;

MyModel::MyModel() : m_vForward(0, 0, 1), m_vUp(0, 1, 0), m_vRight(1, 0, 0)
, m_fPolySize(MyPoly::Poly::PolySize), m_fPolyHalfSize(MyPoly::Poly::PolyHalfSize)
, m_vCenter(0, 0, 0), m_vPos(0, 0, 0)
, m_vLocalForward(0, 0, 1), m_vLocalRight(1, 0, 0), m_vLocalUp(0, 1, 0)
, m_fAnimTime(0.3f), m_bAnimeFlg(false), m_fAnimeWaitTime(0.0f)
, m_fAnimationRotX(0.0f), m_fAnimationRotZ(0.0f),m_fAnimationMargineY(0.0f)
,m_vAnimePos(0,0,0),m_fAnimTimeDelay(0.0)//55)
,m_bRenderflg(true)
,m_bAnimeReverceFlg(false),m_bAnimeReverceInitFlg(false)
,m_bFallFlg(false)
,m_iModelSize(0)
{
	D3DXMATRIX tran;
	D3DXMatrixTranslation(&tran, 0, 0, 0);
	m_mAnimeRot = tran;
	/*m_pCube = new MyPoly::Poly;*/
}

MyModel::~MyModel() {
	
}

void MyModel::InitAdjustment()
{
	float tmpx = (float)((m_iModelHalfSizeX ) * m_fPolySize) + m_fPolyHalfSize;
	float tmpy = (float)((m_iModelHalfSizeY ) * m_fPolySize) + m_fPolyHalfSize;
	float tmpz = (float)((m_iModelHalfSizeZ ) * m_fPolySize) + m_fPolyHalfSize;

	D3DXMatrixTranslation(&m_mAdjustment, tmpx, tmpy, tmpz);
}

void MyModel::SetLV1()
{
	m_iLV = 1;
	m_iModelSizeX = 1;
	m_iModelSizeY = 1;
	m_iModelSizeZ = 1;
	m_iModelSize = 1;
	m_iModelHalfSizeX = 0;
	m_iModelHalfSizeY = 0;
	m_iModelHalfSizeZ = 0;
	InitAdjustment();
	D3DXMATRIX tran;
	
	D3DXMatrixTranslation(&tran,0, 0, 0);
	m_lmModelMat.push_back(tran);

	//debug  localZ
	//D3DXMatrixTranslation(&tran, 0, 0.5, 1);
	//m_lmModelMat.push_back(tran);
	UpdateCenter();
}
void MyModel::InitLv(int LV)
{
	SetLV1();
	for (int i = 1; i < LV; i++)
	{
		LVUp();
	}
}

void MyModel::UpdateCenter()
{
	D3DXVECTOR3 vCenter = m_vPos + m_vAnimePos;
	D3DXVec3TransformCoord(&m_vCenter, &vCenter,&m_mAdjustment);	
}


bool MyModel::GroundCheck(float deltaTime) {
	if (this->GetCenter().x < 0) {
		return FallAnimationLeft(deltaTime);
	}
	if (this->GetCenter().x > MyGround::Modelx * GroundPanel::PanelSize) {
		return FallAnimationRight(deltaTime);
	}
	if (this->GetCenter().z < 0) {
		return FallAnimationBackward(deltaTime);
	}
	if (this->GetCenter().z > MyGround::Modelz * GroundPanel::PanelSize) {
		return FallAnimationForward(deltaTime);
	}
	return false;
}


bool MyModel::FallAnimationLeft(float deltaTime)
{
	if (!m_bAnimeFlg) {
		m_bAnimeFlg = true;
		m_bFallFlg = true;
		m_vAnimationPrevPos = m_vPos;
	}
	if (m_bFallFlg) {
		float AnimDelay = m_fAnimTimeDelay * m_iLV;
		if (m_fAnimTimeDelay > MyModel::m_fAnimationDelayTimeRimit) {
			AnimDelay = MyModel::m_fAnimationDelayTimeRimit;
		}
		float AnimTime = m_fAnimTime + AnimDelay;
		m_fAnimationRotZ += 90.0f * deltaTime / AnimTime;
		m_vPos.y -= 3 * deltaTime;
		D3DXMatrixRotationZ(&m_mAnimeRot, m_fAnimationRotZ / 180 * D3DX_PI);
	}
	if (m_vPos.y <= -15.0f)
	{
		m_vPos = m_vAnimationPrevPos;
		m_vPos.x += m_iModelSizeX * m_fPolySize;
		m_bAnimeFlg = false;
		m_bFallFlg = false;
		D3DXMATRIX tran;
		D3DXMatrixTranslation(&tran, 0, 0, 0);
		m_mAnimeRot = tran;
		return true;
	}
	return false;
}


bool MyModel::FallAnimationRight(float deltaTime)
{
	if (!m_bAnimeFlg) {
		m_bAnimeFlg = true;
		m_bFallFlg = true;
		m_vAnimationPrevPos = m_vPos;
	}
	if (m_bFallFlg) {
		float AnimDelay = m_fAnimTimeDelay * m_iLV;
		if (m_fAnimTimeDelay > MyModel::m_fAnimationDelayTimeRimit) {
			AnimDelay = MyModel::m_fAnimationDelayTimeRimit;
		}
		float AnimTime = m_fAnimTime + AnimDelay;
		m_fAnimationRotZ += -90.0f * deltaTime / AnimTime;
		m_vPos.y -= 3 * deltaTime;
		D3DXMatrixRotationZ(&m_mAnimeRot, m_fAnimationRotZ / 180 * D3DX_PI);
	}
	if (m_vPos.y <= -15.0f)
	{
		m_vPos = m_vAnimationPrevPos;
		m_vPos.x -= m_iModelSizeX * m_fPolySize;
		m_bAnimeFlg = false;
		m_bFallFlg = false;
		D3DXMATRIX tran;
		D3DXMatrixTranslation(&tran, 0, 0, 0);
		m_mAnimeRot = tran;
		return true;
	}
	return false;
}



bool MyModel::FallAnimationBackward(float deltaTime)
{
	if (!m_bAnimeFlg) {
		m_bAnimeFlg = true;
		m_bFallFlg = true;
		m_vAnimationPrevPos = m_vPos;
	}
	if (m_bFallFlg) {
		float AnimDelay = m_fAnimTimeDelay * m_iLV;
		if (m_fAnimTimeDelay > MyModel::m_fAnimationDelayTimeRimit) {
			AnimDelay = MyModel::m_fAnimationDelayTimeRimit;
		}
		float AnimTime = m_fAnimTime + AnimDelay;
		m_fAnimationRotX += -90.0f * deltaTime / AnimTime;
		m_vPos.y -= 3 * deltaTime;
		D3DXMatrixRotationX(&m_mAnimeRot, m_fAnimationRotX / 180 * D3DX_PI);
	}
	if (m_vPos.y <= -15.0f)
	{
		m_vPos = m_vAnimationPrevPos;
		m_vPos.z += m_iModelSizeZ * m_fPolySize;
		m_bAnimeFlg = false;
		m_bFallFlg = false;
		D3DXMATRIX tran;
		D3DXMatrixTranslation(&tran, 0, 0, 0);
		m_mAnimeRot = tran;
		return true;
	}
	return false;
}


bool MyModel::FallAnimationForward(float deltaTime)
{
	if (!m_bAnimeFlg) {
		m_bAnimeFlg = true;
		m_bFallFlg = true;
		m_vAnimationPrevPos = m_vPos;
	}
	if (m_bFallFlg) {
		float AnimDelay = m_fAnimTimeDelay * m_iLV;
		if (m_fAnimTimeDelay > MyModel::m_fAnimationDelayTimeRimit) {
			AnimDelay = MyModel::m_fAnimationDelayTimeRimit;
		}
		float AnimTime = m_fAnimTime + AnimDelay;
		m_fAnimationRotX += 90.0f * deltaTime / AnimTime;
		m_vPos.y -= 3 * deltaTime;
		D3DXMatrixRotationX(&m_mAnimeRot, m_fAnimationRotX / 180 * D3DX_PI);
	}
	if (m_vPos.y <= -15.0f)
	{
		m_vPos = m_vAnimationPrevPos;
		m_vPos.z -= m_iModelSizeZ * m_fPolySize;
		m_bAnimeFlg = false;
		m_bFallFlg = false;
		D3DXMATRIX tran;
		D3DXMatrixTranslation(&tran, 0, 0, 0);
		m_mAnimeRot = tran;
		return true;
	}
	return false;
}

void MyModel::Render(D3DXMATRIX view,D3DXMATRIX proj)
{
	if (m_bRenderflg) {
		m_VertexShader.SetShader(0);
		m_VertexShader.SetLayout();
		m_PixelShader.SetShader(0);

		D3DXMATRIX mTran;
		D3DXMatrixTranslation(&mTran, m_vPos.x, m_vPos.y, m_vPos.z);

		D3DXMATRIX mW = m_mAnimeRot * mTran * m_mAdjustment;
		for (auto itr = m_lmModelMat.begin(); itr != m_lmModelMat.end(); ++itr)
		{
			D3DXMATRIX tmpmatrix;
			tmpmatrix = (*itr) * mW;
			m_ConstantBuffer.SetConstantBuffer(tmpmatrix, view, proj);
			m_ConstantBuffer.SetCBtoPS();
			m_ConstantBuffer.SetCBtoVS();
			m_Cube.Render();
		}
	}
	//UpdateCenter();
}
//
//void MyModel::Render(D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX tran) 
//{
//	m_VertexShader.SetShader(0);
//	m_VertexShader.SetLayout();
//	m_PixelShader.SetShader(0);
//	
//	D3DXMATRIX mW = tran * m_mAdjustment;
//	for (auto itr = m_lmModelMat.begin(); itr != m_lmModelMat.end(); ++itr)
//	{
//		D3DXMATRIX tmpmatrix;
//		tmpmatrix = (*itr) * mW;
//		m_ConstantBuffer.SetConstantBuffer(tmpmatrix, view, proj);
//		m_ConstantBuffer.SetCBtoPS();
//		m_ConstantBuffer.SetCBtoVS();
//		m_Cube.Render();
//	}
//	UpdateCenter();
//}
//void MyModel::Render(D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX tran, D3DXMATRIX rotation)
//{
//	m_VertexShader.SetShader(0);
//	m_VertexShader.SetLayout();
//	m_PixelShader.SetShader(0);
//	
//	D3DXMATRIX mW = rotation * tran * m_mAdjustment;
//
//	for (auto itr = m_lmModelMat.begin(); itr != m_lmModelMat.end(); ++itr)
//	{
//		D3DXMATRIX tmpmatrix;
//		tmpmatrix = (*itr) * mW;
//		m_ConstantBuffer.SetConstantBuffer(tmpmatrix, view, proj);
//		m_ConstantBuffer.SetCBtoPS();
//		m_ConstantBuffer.SetCBtoVS();
//		m_Cube.Render();
//	}
//	UpdateCenter();
//}
//
//
//void MyModel::Render(D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX tran, D3DXMATRIX rotation,D3DXMATRIX scale)
//{
//	m_VertexShader.SetShader(0);
//	m_VertexShader.SetLayout();
//	m_PixelShader.SetShader(0);
//
//	D3DXMATRIX mW = rotation * tran * m_mAdjustment;
//	for (auto itr = m_lmModelMat.begin(); itr != m_lmModelMat.end(); ++itr)
//	{
//		D3DXMATRIX tmpmatrix;
//		tmpmatrix = scale * (*itr) * mW ;
//		m_ConstantBuffer.SetConstantBuffer(tmpmatrix, view, proj);
//		m_ConstantBuffer.SetCBtoPS();
//		m_ConstantBuffer.SetCBtoVS();
//		m_Cube.Render();
//	}
//	UpdateCenter();
//}

void MyModel:: SetPos(D3DXVECTOR3 vPos) {
	m_vPos = vPos;
}
void MyModel::SetPos(float x, float y, float z) {
	m_vPos.x = x;
	m_vPos.y = y;
	m_vPos.z = z;
}
//
//void MyModel::MoveLocalForward(float deltaTime)
//{
//	m_vPos += m_fPolySize * m_iModelSizeZ * m_vLocalForward * deltaTime / m_fAnimTime;
//}
//
//void MyModel::MoveLocalBackward(float deltaTime)
//{
//	m_vPos += m_fPolySize * m_iModelSizeZ * -m_vLocalForward * deltaTime / m_fAnimTime;
//}
//
//void MyModel::MoveLocalRight(float deltaTime)
//{
//	m_vPos += m_fPolySize * m_iModelSizeX * m_vLocalRight * deltaTime / m_fAnimTime;
//}
//
//void MyModel::MoveLocalLeft(float deltaTime)
//{
//	m_vPos += m_fPolySize * m_iModelSizeX * -m_vLocalRight * deltaTime / m_fAnimTime;
//
//}
//
//
//void MyModel::Move(float deltaTime, const D3DXVECTOR3& cvForwardVec)
//{
//	if (cvForwardVec == m_vLocalForward) {
//		MoveLocalForward(deltaTime);
//	}
//	if (cvForwardVec == -m_vLocalForward) {
//		MoveLocalBackward(deltaTime);
//	}
//	if (cvForwardVec == m_vLocalRight) {
//		MoveLocalRight(deltaTime);
//	}
//	if (cvForwardVec == -m_vLocalRight) {
//		MoveLocalLeft(deltaTime);
//	}
//}


bool MyModel::MoveA(float deltaTime, const D3DXVECTOR3& cvForwardVec)
{
	if (!m_bFallFlg) {
		if (cvForwardVec == m_vLocalForward) {
			return AnimationLocalForwardR(deltaTime);
		}
		if (cvForwardVec == -m_vLocalForward) {
			return AnimationLocalBackwardR(deltaTime);
		}
		if (cvForwardVec == m_vLocalRight) {

			return AnimationLocalRightR(deltaTime);
		}
		if (cvForwardVec == -m_vLocalRight) {
			return AnimationLocalLeftR(deltaTime);
		}
	}
	return false;
}
//
//bool MyModel::MoveReverceA(float deltaTime, const D3DXVECTOR3& cvForwardVec)
//{
//	if (cvForwardVec == m_vLocalForward) {
//		return AnimationLocalForwardA(deltaTime);
//	}
//	if (cvForwardVec == -m_vLocalForward) {
//		return AnimationLocalBackwardA(deltaTime);
//	}
//	if (cvForwardVec == m_vLocalRight) {
//		return AnimationLocalRightA(deltaTime);
//	}
//	if (cvForwardVec == -m_vLocalRight) {
//		return AnimationLocalLeftA(deltaTime);
//	}
//	return false;
//}

void MyModel::LVUp()
{
	m_iLV++;
	int tmpx, tmpy, tmpz;

	tmpx = m_iModelHalfSizeX + 2;
	tmpy = m_iModelHalfSizeY + 2;
	tmpz = m_iModelHalfSizeZ + 2;

	for (int x = 0 ; x < (m_iModelSizeX + 2); x++)
	{
		for (int y = 0; y < (m_iModelSizeY + 2); y++) {
			for (int z = 0; z < (m_iModelSizeZ + 2); z++)
			{
				if (x == 0 || y == 0 || z == 0 || x == m_iModelSizeX + 1 || y == m_iModelSizeY +1 || z == m_iModelSizeZ + 1) {
					D3DXMATRIX tran;
					D3DXMatrixTranslation(&tran, (-m_fPolySize * tmpx + ((x + 1)*m_fPolySize)), (-m_fPolySize * tmpy + ((y + 1)*m_fPolySize)), (-m_fPolySize * tmpz + ((z + 1)*m_fPolySize)));

					m_lmModelMat.push_back(tran);
				}
			}
		}
	}
	ModelSizeUp();
	InitAdjustment();
	UpdateCenter();
}


void MyModel::LVUpA(MyModel* pModel)
{
	MyModel tmpModel;

	tmpModel.InitLv(this->GetLV() + 1);

	if (Colision(&tmpModel, pModel) == false)
	{
		m_iLV++;
		int tmpx, tmpy, tmpz;

		tmpx = m_iModelHalfSizeX + 2;
		tmpy = m_iModelHalfSizeY + 2;
		tmpz = m_iModelHalfSizeZ + 2;

		for (int x = 0; x < (m_iModelSizeX + 2); x++)
		{
			for (int y = 0; y < (m_iModelSizeY + 2); y++) {
				for (int z = 0; z < (m_iModelSizeZ + 2); z++)
				{
					if (x == 0 || y == 0 || z == 0 || x == m_iModelSizeX + 1 || y == m_iModelSizeY + 1 || z == m_iModelSizeZ + 1) {
						D3DXMATRIX tran;
						D3DXMatrixTranslation(&tran, (-m_fPolySize * tmpx + ((x + 1)*m_fPolySize)), (-m_fPolySize * tmpy + ((y + 1)*m_fPolySize)), (-m_fPolySize * tmpz + ((z + 1)*m_fPolySize)));

						m_lmModelMat.push_back(tran);
					}
				}
			}
		}
		//D3DXVECTOR3 vTmp(m_fPolySize, 0, m_fPolySize);

		//m_vPos += vTmp;
		ModelSizeUp();
		InitAdjustment();
		UpdateCenter();

	}
}

int MyModel::LVDown()
{
	if (m_iLV == 0) {
		return 0;
	}
	if (m_iLV == 1) {//���񂾂肷�鏈��������
		m_bRenderflg = false;
		m_iLV = 0;
		return 3;
	}
	
	m_iLV--;
	int cubecnt = 0;
	for (int x = 0; x < m_iModelSizeX; x++)
	{
		for (int y = 0; y < m_iModelSizeY; y++) {
			for (int z = 0; z < m_iModelSizeZ; z++)
			{
				if (x == 0 || y == 0 || z == 0 || x == m_iModelSizeX - 1 || y == m_iModelSizeY - 1 || z == m_iModelSizeZ - 1) {
					m_lmModelMat.pop_back();
					cubecnt++;
				}
			}
		}
	}
	D3DXVECTOR3 vTmp(m_fPolySize, 0, m_fPolySize);

	m_vPos += vTmp;

	

	ModelSizeDown();
	InitAdjustment();
	UpdateCenter();
	
	return cubecnt;
}

D3DXVECTOR3 MyModel::GetCenter() { 
	UpdateCenter();
	return m_vCenter; 
}

//D3DXVECTOR3 MyModel::GetColisionPos() { return m_vPos + m_vAnimePos; }


void MyModel::ModelSizeUp()
{
	m_iModelSizeX += 2;
	m_iModelSizeY += 2;
	m_iModelSizeZ += 2;
	m_iModelSize += 2;


	m_iModelHalfSizeX += 1;
	m_iModelHalfSizeY += 1;
	m_iModelHalfSizeZ += 1;	
}

void MyModel::ModelSizeDown() 
{
	m_iModelSizeX -= 2;
	m_iModelSizeY -= 2;
	m_iModelSizeZ -= 2;


	m_iModelHalfSizeX -= 1;
	m_iModelHalfSizeY -= 1;
	m_iModelHalfSizeZ -= 1;
}

float MyModel::GetSize()
{
	return m_iModelSizeY * m_fPolySize;
}

D3DXVECTOR3 MyModel:: GetCameraMargin()
{
	D3DXVECTOR3 retvec(0,0,0);
	retvec.y = m_iModelSizeY * m_fPolySize * 0.5f + m_iModelHalfSizeY * m_fPolySize * 0.65f;
	retvec.z = -(m_iModelSizeZ * m_fPolySize + m_iModelSizeZ * m_fPolySize * 1.3f);
	return retvec;
}

D3DXVECTOR3 MyModel::GetCameraTarget()
{
	D3DXVECTOR3 retvec = m_vCenter;
	retvec.y += m_iModelSizeY * m_fPolySize * 0.5;
	return retvec;
}
//
//void MyModel::AnimationMargineY(float deltaTime)
//{
//	//���������̃A�j���[�V�����̍����̒����̓C�}�C�`�킩�����
//		//�������������ɂȂ������炱��ł���
//	
//	if (m_vPos.y < -m_fAnimationMargineY / 2.0f) {
//		m_vPos.y = m_vAnimationPrevPos.y - m_fAnimationMargineY / 2.0f;
//	}
//
//	if ((m_fAnimTime)* m_iModelSizeX / 2 < m_fAnimeWaitTime)
//	{
//		m_vPos.y += -m_fAnimationMargineY * deltaTime / (m_fAnimTime) / m_iModelSizeX;
//	}
//	else
//	{
//		m_vPos.y += +m_fAnimationMargineY * deltaTime / (m_fAnimTime) / m_iModelSizeX;
//	}
//	
//}
//
//void MyModel::AnimationMargineSet() {
//	
//	//test
//
//	//�Y��ȃA�j���[�V���������Ȃ�
//	//�f���^Y�ƃf���^X�����߂�K�v�����邯���
//	//�Ƃ肠�������͂���őË�����
//	float HalfLineX = m_iModelSizeX * m_fPolySize;
//	float HalfLineY = m_iModelSizeY * m_fPolySize;
//	
//	m_fAnimationMargineY = sqrt(pow(HalfLineX, 2) + pow(HalfLineY, 2)) / 2;
//	m_fAnimationMargineY -= m_vPos.y;
//}
//
//bool MyModel::AnimationLocalRight(float deltaTime) {
//	
//	if (m_bAnimeFlg == false) {
//		m_bAnimeFlg = true;
//		m_vAnimationPrevPos = m_vPos;
//		m_fAnimeWaitTime = 0;
//		m_fAnimationRotX = 0;
//		AnimationMargineSet();
//	}
//	if (m_bAnimeFlg == true) {
//		m_fAnimeWaitTime += deltaTime;
//
//		if ((m_fAnimTime) * m_iModelSizeX <= m_fAnimeWaitTime) {
//			m_vPos = m_vAnimationPrevPos + m_fPolySize * m_iModelSizeX * m_vLocalRight;
//			m_fAnimationRotX = 0;
//			m_fAnimeWaitTime = 0;
//			m_bAnimeFlg = false;
//
//			return true;
//		}
//
//		m_vPos += m_fPolySize * m_iModelSizeX * m_vLocalRight * deltaTime / (m_fAnimTime) / m_iModelSizeX;
//
//		m_fAnimationRotX += -90.0f * deltaTime / (m_fAnimTime) / m_iModelSizeX;
//		
//		AnimationMargineY(deltaTime);
//		
//		
//	}
//	UpdateCenter();
//	return false;
//}
//
//bool MyModel::AnimationLocalRightA(float deltaTime)
//{
//	if (m_bAnimeFlg == false) {
//		m_bAnimeFlg = true;
//		m_vAnimationPrevPos = m_vPos;
//		m_fAnimeWaitTime = 0;
//		m_fAnimationRotX = 0;
//	}
//
//	if (m_bAnimeFlg == true) {
//		m_fAnimeWaitTime += deltaTime;
//		float AnimTime = m_fAnimTime + m_fAnimTimeDelay * m_iLV;
//		if ((AnimTime)/** m_iModelSizeX*/ <= m_fAnimeWaitTime) {
//			m_vPos = m_vAnimationPrevPos + m_fPolySize * m_iModelSizeX * m_vLocalRight;
//			m_fAnimationRotX = 0;
//			m_fAnimeWaitTime = 0;
//			m_bAnimeFlg = false;
//			D3DXMATRIX tran;
//			D3DXMatrixTranslation(&tran, 0, 0, 0);
//			m_mAnimeRot = tran;
//
//			m_vAnimePos = D3DXVECTOR3(0, 0, 0);
//			return true;
//		}
//
//		m_vAnimePos += m_fPolySize * m_iModelSizeX * m_vLocalRight * deltaTime / (AnimTime); // m_iModelSizeX;
//
//		m_fAnimationRotX += -90.0f * deltaTime / (AnimTime); // m_iModelSizeX;
//
//
//		D3DXMATRIX mPipotTran;
//		D3DXMatrixTranslation(&mPipotTran, -m_iModelSizeX * m_fPolyHalfSize, m_iModelSizeY * m_fPolyHalfSize, 0);
//		D3DXMATRIX mInverse;
//		D3DXMatrixInverse(&mInverse, NULL, &mPipotTran);
//		D3DXMATRIX mRotZ;
//		D3DXMatrixRotationZ(&mRotZ, m_fAnimationRotX / 180 * D3DX_PI);
//
//		m_mAnimeRot = mPipotTran * mRotZ * mInverse;
//	}
//	UpdateCenter();
//	return false;
//}

//
//bool MyModel::AnimationLocalLeft(float deltaTime) {
//	if (m_bAnimeFlg == false) {
//		m_bAnimeFlg = true;
//		m_vAnimationPrevPos = m_vPos;
//		m_fAnimeWaitTime = 0;
//		m_fAnimationRotX = 0;
//		AnimationMargineSet();
//	}
//	if (m_bAnimeFlg == true) {
//		m_fAnimeWaitTime += deltaTime;
//		float AnimTime = m_fAnimTime + m_fAnimTimeDelay * m_iLV;
//		if ((m_fAnimTime)/* * m_iModelSizeX*/ <= m_fAnimeWaitTime) {
//			m_vPos = m_vAnimationPrevPos +  m_fPolySize * m_iModelSizeX * -m_vLocalRight;
//			m_fAnimationRotX = 0;
//			m_fAnimeWaitTime = 0;
//			m_bAnimeFlg = false;
//			return true;
//		}
//
//		m_vPos += m_fPolySize * m_iModelSizeX * -m_vLocalRight * deltaTime / AnimTime;// m_iModelSizeX;
//
//			m_fAnimationRotX += 90.0f * deltaTime / (AnimTime); // m_iModelSizeX;
//
//		AnimationMargineY(deltaTime);
//	}
//	UpdateCenter();
//	return false;
//}
//
//bool MyModel::AnimationLocalLeftA(float deltaTime)
//{
//	if (m_bAnimeFlg == false) {
//		m_bAnimeFlg = true;
//		m_vAnimationPrevPos = m_vPos;
//		m_fAnimeWaitTime = 0;
//		m_fAnimationRotX = 0;
//	}
//
//	if (m_bAnimeFlg == true) {
//		m_fAnimeWaitTime += deltaTime;
//		float AnimTime = m_fAnimTime + m_fAnimTimeDelay * m_iLV;
//
//		if ((AnimTime)/** m_iModelSizeX */<= m_fAnimeWaitTime) {
//			m_vPos = m_vAnimationPrevPos + m_fPolySize * m_iModelSizeX * -m_vLocalRight;
//			m_fAnimationRotX = 0;
//			m_fAnimeWaitTime = 0;
//			m_bAnimeFlg = false;
//			D3DXMATRIX tran;
//			D3DXMatrixTranslation(&tran, 0, 0, 0);
//			m_mAnimeRot = tran;
//
//			m_vAnimePos = D3DXVECTOR3(0, 0, 0);
//			return true;
//		}
//
//		m_vAnimePos += m_fPolySize * m_iModelSizeX * -m_vLocalRight * deltaTime / (AnimTime);// m_iModelSizeX;
//
//		m_fAnimationRotX += 90.0f * deltaTime / (AnimTime);// m_iModelSizeX;
//
//
//		D3DXMATRIX mPipotTran;
//		D3DXMatrixTranslation(&mPipotTran, m_iModelSizeX * m_fPolyHalfSize, m_iModelSizeY * m_fPolyHalfSize, 0);
//		D3DXMATRIX mInverse;
//		D3DXMatrixInverse(&mInverse, NULL, &mPipotTran);
//		D3DXMATRIX mRotZ;
//		D3DXMatrixRotationZ(&mRotZ, m_fAnimationRotX / 180 * D3DX_PI);
//
//		m_mAnimeRot = mPipotTran * mRotZ * mInverse;
//	}
//	UpdateCenter();
//	return false;
//}



//
//
//bool MyModel::AnimationLocalForward(float deltaTime) {
//	if (m_bAnimeFlg == false) {
//		m_bAnimeFlg = true;
//		m_vAnimationPrevPos = m_vPos;
//		m_fAnimeWaitTime = 0;
//		m_fAnimationRotZ = 0;
//		AnimationMargineSet();
//	}
//	if (m_bAnimeFlg == true) {
//		m_fAnimeWaitTime += deltaTime;
//
//		if ((m_fAnimTime) * m_iModelSizeZ <= m_fAnimeWaitTime) {
//			m_vPos = m_vAnimationPrevPos + m_fPolySize * m_iModelSizeX * m_vLocalForward;
//			m_fAnimationRotZ = 0;
//			m_fAnimeWaitTime = 0;
//			m_bAnimeFlg = false;
//			return true;
//		}
//
//		m_vPos += m_fPolySize * m_iModelSizeZ * m_vLocalForward * deltaTime / (m_fAnimTime) / m_iModelSizeX;
//
//		m_fAnimationRotZ += 90.0f * deltaTime / (m_fAnimTime) / m_iModelSizeX;
//
//		AnimationMargineY(deltaTime);
//	}
//	UpdateCenter();
//	return false;
//}

//
//bool MyModel::AnimationLocalForwardA(float deltaTime)
//{
//	if (m_bAnimeFlg == false) {
//		m_bAnimeFlg = true;
//		m_vAnimationPrevPos = m_vPos;
//		m_fAnimeWaitTime = 0;
//		m_fAnimationRotX = 0;
//	}
//
//	if (m_bAnimeFlg == true) {
//		m_fAnimeWaitTime += deltaTime;
//		float AnimTime = m_fAnimTime + m_fAnimTimeDelay * m_iLV;
//		
//		if ((AnimTime)/* * m_iModelSizeZ*/ <= m_fAnimeWaitTime) {
//			m_vPos = m_vAnimationPrevPos + m_fPolySize * m_iModelSizeZ * m_vLocalForward;
//			m_fAnimationRotZ = 0;
//			m_fAnimeWaitTime = 0;
//			m_bAnimeFlg = false;
//			D3DXMATRIX tran;
//			D3DXMatrixTranslation(&tran, 0, 0, 0);
//			m_mAnimeRot = tran;
//
//			m_vAnimePos = D3DXVECTOR3(0, 0, 0);
//			return true;
//		}
//
//		m_vAnimePos += m_fPolySize * m_iModelSizeZ * m_vLocalForward * deltaTime / (AnimTime);// m_iModelSizeX;
//
//		m_fAnimationRotZ += 90.0f * deltaTime / (AnimTime);// m_iModelSizeX;
//
//
//		D3DXMATRIX mPipotTran;
//		//��]���鎲�ɓ������Ă�񂾂��ǁ@���[�J����0,0,0���Z���^�[������Y�𓮂��ăL���[�u�̉��̕ӂ�y=0�ɂ���
//		//���ɉ�]������ɂ��Ă��������̂�Z���}�C�i�X���Ă�����΁@ForwardUnder�Ƃ������ׂ��ꏊ���@�s�|�b�g�̈ʒu�ɂȂ�
//		//�����𒆐S�ɉ�]�����邱�ƂŁ@�Y��ɉ�]����
//		//���̌�ړ����̋t�s��������Ă����邱�ƂŁA��]�������ێ�������Ԃ�
//		//���̈ʒu�ɖ߂��Ă���
//		D3DXMatrixTranslation(&mPipotTran, 0, m_iModelSizeY * m_fPolyHalfSize, -m_iModelSizeZ * m_fPolyHalfSize);
//		D3DXMATRIX mInverse;
//		D3DXMatrixInverse(&mInverse, NULL, &mPipotTran);
//		D3DXMATRIX mRotX;
//		D3DXMatrixRotationX(&mRotX, m_fAnimationRotZ / 180 * D3DX_PI);
//
//		m_mAnimeRot = mPipotTran * mRotX * mInverse;
//	}
//	UpdateCenter();
//	return false;
//}

//
//bool MyModel::AnimationLocalBackward(float deltaTime) {
//	if (m_bAnimeFlg == false) {
//		m_bAnimeFlg = true;
//		m_vAnimationPrevPos = m_vPos;
//		m_fAnimeWaitTime = 0;
//		m_fAnimationRotZ = 0;
//		AnimationMargineSet();
//	}
//	if (m_bAnimeFlg == true) {
//		m_fAnimeWaitTime += deltaTime;
//
//		if ((m_fAnimTime)* m_iModelSizeZ <= m_fAnimeWaitTime) {
//			m_vPos = m_vAnimationPrevPos + m_fPolySize * m_iModelSizeX * -m_vLocalForward;
//			m_fAnimationRotZ = 0;
//			m_fAnimeWaitTime = 0;
//			m_bAnimeFlg = false;
//			return true;
//		}
//
//		m_vPos += m_fPolySize * m_iModelSizeZ * -m_vLocalForward * deltaTime / (m_fAnimTime) / m_iModelSizeX;
//
//		m_fAnimationRotZ += -90.0f * deltaTime / (m_fAnimTime) / m_iModelSizeX;
//
//		AnimationMargineY(deltaTime);
//	}
//	UpdateCenter();
//	return false;
//}


//
//bool MyModel::AnimationLocalBackwardA(float deltaTime)
//{
//	if (m_bAnimeFlg == false) {
//		m_bAnimeFlg = true;
//		m_vAnimationPrevPos = m_vPos;
//		m_fAnimeWaitTime = 0;
//		m_fAnimationRotX = 0;
//	}
//
//	if (m_bAnimeFlg == true) {
//		m_fAnimeWaitTime += deltaTime;
//		float AnimTime = m_fAnimTime + m_fAnimTimeDelay * m_iLV;
//
//		if ((AnimTime)/** m_iModelSizeZ*/ <= m_fAnimeWaitTime) {
//			m_vPos = m_vAnimationPrevPos + m_fPolySize * m_iModelSizeZ * -m_vLocalForward;
//			m_fAnimationRotZ = 0;
//			m_fAnimeWaitTime = 0;
//			m_bAnimeFlg = false;
//			D3DXMATRIX tran;
//			D3DXMatrixTranslation(&tran, 0, 0, 0);
//			m_mAnimeRot = tran;
//
//			m_vAnimePos = D3DXVECTOR3(0, 0, 0);
//			return true;
//		}
//		//���̃A�j���|�X�̓J�������W���X�V���邽�߂ɃZ���^�[���������炵�Ă�����K�v��������
//		//���̂��߂̃}�[�W���A�A�j���[�V���������ĂȂ��Ƃ��͂���͉��̐������Ȃ���
//		//�A�j���[�V���������Ă�Ƃ��͂��ꂪ���܂���ɃZ���^�[�𓮂����Ă����
//		m_vAnimePos += m_fPolySize * m_iModelSizeZ * -m_vLocalForward * deltaTime / (AnimTime);// m_iModelSizeX;
//
//		m_fAnimationRotZ += -90.0f * deltaTime / (AnimTime);// m_iModelSizeX;
//
//
//		D3DXMATRIX mPipotTran;
//		//��]���鎲�ɓ������Ă�񂾂��ǁ@���[�J����0,0,0���Z���^�[������Y�𓮂��ăL���[�u�̉��̕ӂ�y=0�ɂ���
//		//���ɉ�]������ɂ��Ă��������̂�Z���}�C�i�X���Ă�����΁@ForwardUnder�Ƃ������ׂ��ꏊ���@�s�|�b�g�̈ʒu�ɂȂ�
//		//�����𒆐S�ɉ�]�����邱�ƂŁ@�Y��ɉ�]����
//		//���̌�ړ����̋t�s��������Ă����邱�ƂŁA��]�������ێ�������Ԃ�
//		//���̈ʒu�ɖ߂��Ă���
//		D3DXMatrixTranslation(&mPipotTran, 0, m_iModelSizeY * m_fPolyHalfSize, m_iModelSizeZ * m_fPolyHalfSize);
//		D3DXMATRIX mInverse;
//		D3DXMatrixInverse(&mInverse, NULL, &mPipotTran);
//		D3DXMATRIX mRotX;
//		D3DXMatrixRotationX(&mRotX, m_fAnimationRotZ / 180 * D3DX_PI);
//
//		m_mAnimeRot = mPipotTran * mRotX * mInverse;
//	}
//	UpdateCenter();
//	return false;
//}


bool MyModel::Colision(MyModel* pModel) {
	D3DXVECTOR3 vModelA = this->GetCenter();
	D3DXVECTOR3 vModelB = pModel->GetCenter();

	D3DXVECTOR2 vTopRightA;

	float SizeA = this->GetSize() * 0.98;
	float SizeB = pModel->GetSize() * 0.98;

	vTopRightA.x = vModelA.x + SizeA / 2.0f;
	vTopRightA.y = vModelA.z + SizeA / 2.0f;
	D3DXVECTOR2 vTopLeftA;
	vTopLeftA.x = vModelA.x - SizeA / 2.0f;
	vTopLeftA.y = vModelA.z + SizeA / 2.0f;
	D3DXVECTOR2 vUnderRightA;
	vUnderRightA.x = vModelA.x + SizeA / 2.0f;
	vUnderRightA.y = vModelA.z - SizeA / 2.0f;
	D3DXVECTOR2 vUnderLeftA;
	vUnderLeftA.x = vModelA.x - SizeA / 2.0f;
	vUnderLeftA.y = vModelA.z - SizeA / 2.0f;


	D3DXVECTOR2 vTopRightB;
	vTopRightB.x = vModelB.x + SizeB / 2.0f;
	vTopRightB.y = vModelB.z + SizeB / 2.0f;
	D3DXVECTOR2 vTopLeftB;
	vTopLeftB.x = vModelB.x - SizeB / 2.0f;
	vTopLeftB.y = vModelB.z + SizeB / 2.0f;
	D3DXVECTOR2 vUnderRightB;
	vUnderRightB.x = vModelB.x + SizeB / 2.0f;
	vUnderRightB.y = vModelB.z - SizeB / 2.0f;
	D3DXVECTOR2 vUnderLeftB;
	vUnderLeftB.x = vModelB.x - SizeB; // 2.0f;
	vUnderLeftB.y = vModelB.z - SizeB; // 2.0f;

	if (vTopLeftA.x < vUnderRightB.x && vTopLeftA.y > vUnderRightB.y
		&& vTopRightA.x > vUnderLeftB.x && vTopRightA.y > vUnderLeftB.y
		&& vUnderLeftA.x < vTopRightB.x && vUnderLeftA.y < vTopRightB.y
		&& vUnderRightA.x > vTopLeftB.x && vUnderRightA.y < vTopLeftB.y
		)
	{
		return true;
	}

	return false;
}



//
//bool MyModel::ColisionCalc(MyModel* pModel) {
//	if (this->GetLV() > pModel->GetLV()) {
//		if (this->Colision(pModel)) {
//			return true;
//		}
//	}
//	else if (this->GetLV() == 1 && pModel->GetLV() == 1) {
//		if (this->Colision(pModel))
//		{
//			return true;
//		}
//	}
//	else {
//		if (this->Colision(pModel))
//		{
//			m_bAnimeReverceFlg = true;
//		}
//	}
//
//	return false;
//}




bool MyModel::AnimationLocalRightR(float deltaTime)
{
	if (m_bAnimeFlg == false) {
		m_bAnimeFlg = true;
		m_vAnimationPrevPos = m_vPos;
		m_fAnimeWaitTime = 0;
		m_fAnimationRotX = 0;
	}

	if (m_bAnimeFlg == true) {
		if (!m_bAnimeReverceFlg) {
			m_fAnimeWaitTime += deltaTime;
			float AnimDelay = m_fAnimTimeDelay * m_iLV;
			if (m_fAnimTimeDelay > MyModel::m_fAnimationDelayTimeRimit) {
				AnimDelay = MyModel::m_fAnimationDelayTimeRimit;
			}
			float AnimTime = m_fAnimTime + AnimDelay;
			if ((AnimTime)/** m_iModelSizeX*/ <= m_fAnimeWaitTime) {
				m_vPos = m_vAnimationPrevPos + m_fPolySize * m_iModelSizeX * m_vLocalRight;
				m_fAnimationRotX = 0;
				m_fAnimeWaitTime = 0;
				m_bAnimeFlg = false;
				D3DXMATRIX tran;
				D3DXMatrixTranslation(&tran, 0, 0, 0);
				m_mAnimeRot = tran;

				m_vAnimePos = D3DXVECTOR3(0, 0, 0);
				return true;
			}

			m_vAnimePos += m_fPolySize * m_iModelSizeX * m_vLocalRight * deltaTime / (AnimTime); // m_iModelSizeX;

			m_fAnimationRotX += -90.0f * deltaTime / (AnimTime); // m_iModelSizeX;

		}
		else if (m_bAnimeReverceFlg) {
			if (!m_bAnimeReverceInitFlg) {
				m_bAnimeReverceInitFlg = true;
				m_fAnimeReverceTime = m_fAnimeWaitTime;
				m_fAnimeReverceRot = m_fAnimationRotX;
				m_vAnimeRevercePos = m_vAnimePos;
				m_fAnimeWaitTime = 0;
			}

			if (m_bAnimeReverceInitFlg) {
				m_fAnimeWaitTime += deltaTime;
				float AnimTime = m_fAnimeReverceTime + m_fAnimTimeDelay * m_iLV;

				if ((AnimTime)/** m_iModelSizeX*/ <= m_fAnimeWaitTime) {
					m_vPos = m_vAnimationPrevPos;
					m_fAnimationRotX = 0;
					m_fAnimeWaitTime = 0;
					m_bAnimeFlg = false;
					D3DXMATRIX tran;
					D3DXMatrixTranslation(&tran, 0, 0, 0);
					m_mAnimeRot = tran;
					m_vAnimePos = D3DXVECTOR3(0, 0, 0);
					m_bAnimeReverceInitFlg = false;
					m_bAnimeReverceFlg = false;
					return true;
				}
				m_vAnimePos -= m_vAnimeRevercePos * deltaTime / (AnimTime); // m_iModelSizeX;
				m_fAnimationRotX -= m_fAnimeReverceRot * deltaTime / (AnimTime); // m_iModelSizeX;

			}
		}

		D3DXMATRIX mPipotTran;
		D3DXMatrixTranslation(&mPipotTran, -m_iModelSizeX * m_fPolyHalfSize, m_iModelSizeY * m_fPolyHalfSize, 0);
		D3DXMATRIX mInverse;
		D3DXMatrixInverse(&mInverse, NULL, &mPipotTran);
		D3DXMATRIX mRotZ;
		D3DXMatrixRotationZ(&mRotZ, m_fAnimationRotX / 180 * D3DX_PI);

		m_mAnimeRot = mPipotTran * mRotZ * mInverse;
	}
	UpdateCenter();
	return false;
}





bool MyModel::AnimationLocalLeftR(float deltaTime)
{
	if (m_bAnimeFlg == false) {
		m_bAnimeFlg = true;
		m_vAnimationPrevPos = m_vPos;
		m_fAnimeWaitTime = 0;
		m_fAnimationRotX = 0;
	}

	if (m_bAnimeFlg == true) {
		if (!m_bAnimeReverceFlg) {
			m_fAnimeWaitTime += deltaTime;

			float AnimDelay = m_fAnimTimeDelay * m_iLV;
			if (m_fAnimTimeDelay > MyModel::m_fAnimationDelayTimeRimit) {
				AnimDelay = MyModel::m_fAnimationDelayTimeRimit;
			}
			float AnimTime = m_fAnimTime + AnimDelay;

			if ((AnimTime)/** m_iModelSizeX */ <= m_fAnimeWaitTime) {
				m_vPos = m_vAnimationPrevPos + m_fPolySize * m_iModelSizeX * -m_vLocalRight;
				m_fAnimationRotX = 0;
				m_fAnimeWaitTime = 0;
				m_bAnimeFlg = false;
				D3DXMATRIX tran;
				D3DXMatrixTranslation(&tran, 0, 0, 0);
				m_mAnimeRot = tran;

				m_vAnimePos = D3DXVECTOR3(0, 0, 0);
				return true;
			}

			m_vAnimePos += m_fPolySize * m_iModelSizeX * -m_vLocalRight * deltaTime / (AnimTime);// m_iModelSizeX;

			m_fAnimationRotX += 90.0f * deltaTime / (AnimTime);// m_iModelSizeX;
		}
		else if (m_bAnimeReverceFlg) {
			if (!m_bAnimeReverceInitFlg) {
				m_bAnimeReverceInitFlg = true;
				m_fAnimeReverceTime = m_fAnimeWaitTime;
				m_fAnimeReverceRot = m_fAnimationRotX;
				m_vAnimeRevercePos = m_vAnimePos;
				m_fAnimeWaitTime = 0;
			}

			if (m_bAnimeReverceInitFlg) {
				m_fAnimeWaitTime += deltaTime;
				float AnimTime = m_fAnimeReverceTime + m_fAnimTimeDelay * m_iLV;

				if ((AnimTime)/** m_iModelSizeX*/ <= m_fAnimeWaitTime) {
					m_vPos = m_vAnimationPrevPos;
					m_fAnimationRotX = 0;
					m_fAnimeWaitTime = 0;
					m_bAnimeFlg = false;
					D3DXMATRIX tran;
					D3DXMatrixTranslation(&tran, 0, 0, 0);
					m_mAnimeRot = tran;
					m_vAnimePos = D3DXVECTOR3(0, 0, 0);
					m_bAnimeReverceInitFlg = false;
					m_bAnimeReverceFlg = false;
					return true;
				}
				m_vAnimePos -= m_vAnimeRevercePos * deltaTime / (AnimTime); // m_iModelSizeX;
				m_fAnimationRotX -= m_fAnimeReverceRot * deltaTime / (AnimTime); // m_iModelSizeX;

			}
		}

		D3DXMATRIX mPipotTran;
		D3DXMatrixTranslation(&mPipotTran, m_iModelSizeX * m_fPolyHalfSize, m_iModelSizeY * m_fPolyHalfSize, 0);
		D3DXMATRIX mInverse;
		D3DXMatrixInverse(&mInverse, NULL, &mPipotTran);
		D3DXMATRIX mRotZ;
		D3DXMatrixRotationZ(&mRotZ, m_fAnimationRotX / 180 * D3DX_PI);

		m_mAnimeRot = mPipotTran * mRotZ * mInverse;
	}
	UpdateCenter();
	return false;
}



bool MyModel::AnimationLocalForwardR(float deltaTime)
{
	if (m_bAnimeFlg == false) {
		m_bAnimeFlg = true;
		m_vAnimationPrevPos = m_vPos;
		m_fAnimeWaitTime = 0;
		m_fAnimationRotZ = 0;
	}

	if (m_bAnimeFlg == true) {
		if (!m_bAnimeReverceFlg) {
			m_fAnimeWaitTime += deltaTime;
			//animtime�Ɋւ��Ă̓����o�ɂ��p���Ǝv�������A�G�ɓ���������A�j���[�V�������x�������Ȃ肩���̂�
			//����ł���
			float AnimDelay = m_fAnimTimeDelay * m_iLV;
			if (m_fAnimTimeDelay > MyModel::m_fAnimationDelayTimeRimit) {
				AnimDelay = MyModel::m_fAnimationDelayTimeRimit;
			}
			float AnimTime = m_fAnimTime + AnimDelay;
			if ((AnimTime)/* * m_iModelSizeZ*/ <= m_fAnimeWaitTime) {
				m_vPos = m_vAnimationPrevPos + m_fPolySize * m_iModelSizeZ * m_vLocalForward;
				m_fAnimationRotZ = 0;
				m_fAnimeWaitTime = 0;
				m_bAnimeFlg = false;
				D3DXMATRIX tran;
				D3DXMatrixTranslation(&tran, 0, 0, 0);
				m_mAnimeRot = tran;

				m_vAnimePos = D3DXVECTOR3(0, 0, 0);
				return true;
			}

			m_vAnimePos += m_fPolySize * m_iModelSizeZ * m_vLocalForward * deltaTime / (AnimTime);// m_iModelSizeX;

			m_fAnimationRotZ += 90.0f * deltaTime / (AnimTime);// m_iModelSizeX;

		}
		else if (m_bAnimeReverceFlg) {
			if (!m_bAnimeReverceInitFlg) {
				m_bAnimeReverceInitFlg = true;
				m_fAnimeReverceTime = m_fAnimeWaitTime;
				m_fAnimeReverceRot = m_fAnimationRotZ;
				m_vAnimeRevercePos = m_vAnimePos;
				m_fAnimeWaitTime = 0;
			}

			if (m_bAnimeReverceInitFlg) {
				m_fAnimeWaitTime += deltaTime;
				float AnimTime = m_fAnimeReverceTime + m_fAnimTimeDelay * m_iLV;

				if ((AnimTime)/** m_iModelSizeX*/ <= m_fAnimeWaitTime) {
					m_vPos = m_vAnimationPrevPos;
					m_fAnimationRotZ = 0;
					m_fAnimeWaitTime = 0;
					m_bAnimeFlg = false;
					D3DXMATRIX tran;
					D3DXMatrixTranslation(&tran, 0, 0, 0);
					m_mAnimeRot = tran;
					m_vAnimePos = D3DXVECTOR3(0, 0, 0);
					m_bAnimeReverceInitFlg = false;
					m_bAnimeReverceFlg = false;
					return true;
				}
				m_vAnimePos -= m_vAnimeRevercePos * deltaTime / (AnimTime); // m_iModelSizeX;
				m_fAnimationRotZ -= m_fAnimeReverceRot * deltaTime / (AnimTime); // m_iModelSizeX;

			}
		}

		D3DXMATRIX mPipotTran;
		D3DXMatrixTranslation(&mPipotTran, 0, m_iModelSizeY * m_fPolyHalfSize, -m_iModelSizeZ * m_fPolyHalfSize);
		D3DXMATRIX mInverse;
		D3DXMatrixInverse(&mInverse, NULL, &mPipotTran);
		D3DXMATRIX mRotX;
		D3DXMatrixRotationX(&mRotX, m_fAnimationRotZ / 180 * D3DX_PI);

		m_mAnimeRot = mPipotTran * mRotX * mInverse;
	}
	UpdateCenter();
	return false;
}



bool MyModel::AnimationLocalBackwardR(float deltaTime)
{
	if (m_bAnimeFlg == false) {
		m_bAnimeFlg = true;
		m_vAnimationPrevPos = m_vPos;
		m_fAnimeWaitTime = 0;
		m_fAnimationRotZ = 0;
	}

	if (m_bAnimeFlg == true) {
		if (!m_bAnimeReverceFlg) {
			m_fAnimeWaitTime += deltaTime;
			float AnimDelay = m_fAnimTimeDelay * m_iLV;
			if (m_fAnimTimeDelay > MyModel::m_fAnimationDelayTimeRimit) {
				AnimDelay = MyModel::m_fAnimationDelayTimeRimit;
			}
			float AnimTime = m_fAnimTime + AnimDelay;
			if ((AnimTime)/** m_iModelSizeZ*/ <= m_fAnimeWaitTime) {
				m_vPos = m_vAnimationPrevPos + m_fPolySize * m_iModelSizeZ * -m_vLocalForward;
				m_fAnimationRotZ = 0;
				m_fAnimeWaitTime = 0;
				m_bAnimeFlg = false;
				D3DXMATRIX tran;
				D3DXMatrixTranslation(&tran, 0, 0, 0);
				m_mAnimeRot = tran;

				m_vAnimePos = D3DXVECTOR3(0, 0, 0);
				return true;
			}
			//���̃A�j���|�X�̓J�������W���X�V���邽�߂ɃZ���^�[���������炵�Ă�����K�v��������
			//���̂��߂̃}�[�W���A�A�j���[�V���������ĂȂ��Ƃ��͂���͉��̐������Ȃ���
			//�A�j���[�V���������Ă�Ƃ��͂��ꂪ���܂���ɃZ���^�[�𓮂����Ă����
			m_vAnimePos += m_fPolySize * m_iModelSizeZ * -m_vLocalForward * deltaTime / (AnimTime);// m_iModelSizeX;

			m_fAnimationRotZ += -90.0f * deltaTime / (AnimTime);// m_iModelSizeX;
		}else if (m_bAnimeReverceFlg) {
			if (!m_bAnimeReverceInitFlg) {
				m_bAnimeReverceInitFlg = true;
				m_fAnimeReverceTime = m_fAnimeWaitTime;
				m_fAnimeReverceRot = m_fAnimationRotZ;
				m_vAnimeRevercePos = m_vAnimePos;
				m_fAnimeWaitTime = 0;
			}

			if (m_bAnimeReverceInitFlg) {
				m_fAnimeWaitTime += deltaTime;
				float AnimTime = m_fAnimeReverceTime + m_fAnimTimeDelay * m_iLV;

				if ((AnimTime)/** m_iModelSizeX*/ <= m_fAnimeWaitTime) {
					m_vPos = m_vAnimationPrevPos;
					m_fAnimationRotZ = 0;
					m_fAnimeWaitTime = 0;
					m_bAnimeFlg = false;
					D3DXMATRIX tran;
					D3DXMatrixTranslation(&tran, 0, 0, 0);
					m_mAnimeRot = tran;
					m_vAnimePos = D3DXVECTOR3(0, 0, 0);
					m_bAnimeReverceInitFlg = false;
					m_bAnimeReverceFlg = false;
					return true;
				}
				m_vAnimePos -= m_vAnimeRevercePos * deltaTime / (AnimTime); // m_iModelSizeX;
				m_fAnimationRotZ -= m_fAnimeReverceRot * deltaTime / (AnimTime); // m_iModelSizeX;

			}
		}

		D3DXMATRIX mPipotTran;
		D3DXMatrixTranslation(&mPipotTran, 0, m_iModelSizeY * m_fPolyHalfSize, m_iModelSizeZ * m_fPolyHalfSize);
		D3DXMATRIX mInverse;
		D3DXMatrixInverse(&mInverse, NULL, &mPipotTran);
		D3DXMATRIX mRotX;
		D3DXMatrixRotationX(&mRotX, m_fAnimationRotZ / 180 * D3DX_PI);

		m_mAnimeRot = mPipotTran * mRotX * mInverse;
	}
	UpdateCenter();
	return false;
}




bool MyModel::Colision(MyModel* pModelA, MyModel* pModelB)
{
	//D3DXVECTOR3 vLength = pModelB->GetCenter() - pModelA->GetCenter();
	//float fLength = D3DXVec3Length(&vLength);
	//float fModelArad = pModelA->GetSize() *0.49;//sqrt(pow(pModelA->GetSize(), 2) + pow(pModelA->GetSize(), 2))*0.49;
	//float fModelBrad = sqrt(pow(pModelB->GetSize(), 2) + pow(pModelB->GetSize(), 2))*0.49;
	//if (fLength < fModelArad + fModelBrad)
	//{
	//	return true;
	//}

	D3DXVECTOR3 vModelA = pModelA->GetCenter();
	D3DXVECTOR3 vModelB = pModelB->GetCenter();

	D3DXVECTOR2 vTopRightA;

	float SizeA = pModelA->GetSize() * 0.98;
	float SizeB = pModelB->GetSize() * 0.98;

	vTopRightA.x = vModelA.x + SizeA / 2.0f;
	vTopRightA.y = vModelA.z + SizeA / 2.0f;
	D3DXVECTOR2 vTopLeftA;
	vTopLeftA.x = vModelA.x - SizeA / 2.0f;
	vTopLeftA.y = vModelA.z + SizeA / 2.0f;
	D3DXVECTOR2 vUnderRightA;
	vUnderRightA.x = vModelA.x + SizeA / 2.0f;
	vUnderRightA.y = vModelA.z - SizeA / 2.0f;
	D3DXVECTOR2 vUnderLeftA;
	vUnderLeftA.x = vModelA.x - SizeA / 2.0f;
	vUnderLeftA.y = vModelA.z - SizeA / 2.0f;


	D3DXVECTOR2 vTopRightB;
	vTopRightB.x = vModelB.x + SizeB / 2.0f;
	vTopRightB.y = vModelB.z + SizeB / 2.0f;
	D3DXVECTOR2 vTopLeftB;
	vTopLeftB.x = vModelB.x - SizeB / 2.0f;
	vTopLeftB.y = vModelB.z + SizeB / 2.0f;
	D3DXVECTOR2 vUnderRightB;
	vUnderRightB.x = vModelB.x + SizeB / 2.0f;
	vUnderRightB.y = vModelB.z - SizeB / 2.0f;
	D3DXVECTOR2 vUnderLeftB;
	vUnderLeftB.x = vModelB.x - SizeB; // 2.0f;
	vUnderLeftB.y = vModelB.z - SizeB; // 2.0f;

	if (vTopLeftA.x < vUnderRightB.x && vTopLeftA.y > vUnderRightB.y
		&& vTopRightA.x > vUnderLeftB.x && vTopRightA.y > vUnderLeftB.y
		&& vUnderLeftA.x < vTopRightB.x && vUnderLeftA.y < vTopRightB.y
		&& vUnderRightA.x > vTopLeftB.x && vUnderRightA.y < vTopLeftB.y
		)
	{
		return true;
	}

	return false;
}