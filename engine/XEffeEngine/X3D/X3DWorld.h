#ifndef _JIA_X3DWORLD_
#define _JIA_X3DWORLD_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.9.20
//--------------------------------
#include "XBasic3D.h"
#include "XLight.h"
#include "XLineFog.h"
#include "XMaterial.h"
#include "XSkyBox.h"
#include "XModelObj.h"
#include "XModelStl.h"
#include "XCam.h"
#include "XTerrain.h"
#include "XInputEventCommon.h"
#include "XShadowMap.h"
#include "XArcBall.h"
#include "XBasicModel.h"
namespace XE{
//for test
//extern double matModelView[16];
//extern double matProjection[16]; 
//extern int viewport[4]; 

//����һ��������࣬�����й��õĲ���ȫ��������ж���
//������ǵ���ϵͳ
enum XWorldShaderType
{
	SHADER_DUMP,
	SHADER_SHADOW,
	SHADER_DUMP_AND_SHADOW,
};
class X3DWorld
{
	friend XShadowMap;
public:
	X3DWorld()
		:m_isInited(XFalse)
		, m_drawFun(NULL)
		//,m_withShadow(XFalse)
		, m_curShader(NULL)
		, m_withDumpMap(XFalse)
		, m_isMouseBtnDown(XFalse)
		, m_targetObj(NULL)
		, m_withSkyBox(XTrue)
		, m_withLight(XTrue)
		, m_withFog(XTrue)
		, m_3dDrawMode(DRAW_MODE_POINT)
		, m_withShadow(XFalse)
		, m_drawWithTexture(0)
		, m_isArcBallAuto(true)
		, m_withKeyCtrlCam(true)
	{}
	virtual ~X3DWorld() {}
protected:
	X3DWorld(const X3DWorld&);
	X3DWorld &operator= (const X3DWorld&);
private:
	XBool m_isInited;
	//��������������İ���
	XKeyState m_keyUpState;
	XKeyState m_keyDownState;
	XKeyState m_keyAState;
	XKeyState m_keyDState;
	XKeyState m_keyWState;
	XKeyState m_keySState;
	XBool m_isMouseBtnDown;

	XBasic3DObject * m_targetObj;

	XShadowMap m_shadowMap;
	XMat4 m_shadowMatrix;		//������Ӱ�ľ���
	//�����ǹ���shadow��shader�ı���
	XBool m_withShadow;				//�Ƿ�ʹ����Ӱ
	unsigned int m_shadowMapTexture;
	XShaderGLSL m_shadowMapShader; //������Ӱ��shader
	int m_drawWithTexture;			//����Ƿ�����ͼ
	//������dumpmap shader�ı���
	XShaderGLSL m_dumpMapShader;	//������ͼ��shader
	XBool m_withDumpMap;
	unsigned int m_dumpMapTexture;
	//XBool m_withShader;	//�Ƿ�ʹ��shader
	XShaderGLSL *m_curShader;	//���ʹ�õ�һ��shader

	XShaderGLSL m_DMapAndSMapGLSL;

	void(*m_drawFun)(XBool withTexture);	//��ͼ����

	XMat3 m_mouseRotateMatrixO;		//������ת����
	XMat3 m_mouseRotateMatrixN;
	//	XVec3 m_mouseRotateAngle;			//��������ת�ĽǶ�
	XArcBall m_mouseArcBall;
	//	XVec4 m_mouseRotate;

	XBool m_withSkyBox;	//�Ƿ�������
	XBool m_withLight;		//�Ƿ����õƹ�
	XBool m_withFog;		//�Ƿ�ʹ����

	X3DDrawMode m_3dDrawMode;
public:
	void setUseSkyBox(XBool flag) { m_withSkyBox = flag; }
	void setUseLight(XBool flag)
	{
		m_withLight = flag;
		if (m_withLight) m_worldLight.useLight();
		else m_worldLight.disAllLight();
	}
	void setUseFog(XBool flag)
	{
		m_withFog = flag;
		if (m_withFog) m_worldLineFog.useFog();
		else m_worldLineFog.disFog();
	}
	void set3DDrawMode(X3DDrawMode mode)
	{//���Ŀǰ�������⣬��δʵ����Ч
		m_3dDrawMode = mode;
		switch (m_3dDrawMode)
		{
		case DRAW_MODE_FILL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			break;
		case DRAW_MODE_LINE:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			break;
		case DRAW_MODE_POINT:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			break;
		}
	}
public: //3D�����е�һЩ����
	XSkyBox m_skyBox;				//�����Χ�У���պ���
	XCamRoam m_worldCam;			//ͨ�õ����������
	XLight m_worldLight;			//����ƹ�
	XMaterial m_worldMaterial;		//�����ͨ�ò���
	XLineFog m_worldLineFog;		//������
	void useShadow(XBool withTexture, XWorldShaderType type = SHADER_SHADOW, unsigned int dumpTex = 0);
	void removeShadow();
	//����������ӳ��
	XVec3 worldToScreen(const XVec3& point, const XRect& view) { return m_worldCam.worldToScreen(point, view); }	//Ŀǰ���������������
	void setTargetObj(XBasic3DObject * obj) { m_targetObj = obj; }
private:
	bool m_isArcBallAuto;	//�Ƿ��Զ�ʹ��arcBall
	bool m_withKeyCtrlCam;	//�Ƿ����ʹ�ü��̿��������
public:
	void setWithKeyCtrlCam(bool flag) { m_withKeyCtrlCam = flag; }
	bool getWithKeyCtrlCam()const { return m_withKeyCtrlCam; }
	void setIsArcBallAuto(bool flag) { m_isArcBallAuto = flag; }
	bool getIsArcBallAuto()const { return m_isArcBallAuto; }
	void updateArcBall(const XVec2& pos, bool isClick = false)
	{
		if (m_isArcBallAuto || m_targetObj == NULL) return;
		if (isClick)
		{
			m_mouseRotateMatrixO = m_mouseRotateMatrixN;
			m_mouseArcBall.click(pos);
		}
		else
		{
			XVec4 mouseRotate;
			m_mouseArcBall.drag(pos, mouseRotate);
			m_mouseRotateMatrixN = m_mouseRotateMatrixO * XMath::toMatrix3x3(mouseRotate).anti();
			m_targetObj->setMultRotate(XMat4().getRotate(m_mouseRotateMatrixN));
		}
	}
	void updateArcBallEx(const XVec2& pos, const XVec3& angle)
	{
		if (m_isArcBallAuto || m_targetObj == NULL) return;
		m_mouseRotateMatrixO = m_mouseRotateMatrixN * XMath::getRotate3D(angle).anti();
		m_mouseArcBall.click(pos);
		m_mouseRotateMatrixN = m_mouseRotateMatrixO;
		m_targetObj->setMultRotate(XMat4().getRotate(m_mouseRotateMatrixN));
	}
	void resetArcBall()
	{
		if (m_isArcBallAuto || m_targetObj == NULL) return;
		m_mouseRotateMatrixN.loadIdentity();
		m_mouseRotateMatrixO.loadIdentity();
	}
	XBool init(void(*drawFun)(XBool), XResPos resourcePosition = RES_SYS_DEF);
	void draw();
	void move(float stepTime);
	void setWithShadow(XBool flag) { m_withShadow = flag; }
	void setWithDumpMap(XBool flag) { m_withDumpMap = flag; }
	XBool getWithShadow()const { return m_withShadow; }
	void keyEvent(const XInputEvent& inputEvent);	//�����¼�
};
namespace X3D
{
	extern void addCameraToCFG();	//�����������ݵ�����
}
}
#endif