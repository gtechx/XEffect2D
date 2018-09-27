#ifndef _JIA_XPICRECT_
#define _JIA_XPICRECT_
#include "XPicRectCommon.h"
#include "XSprite.h"

namespace XE {
class XPicRect
{
private:
	bool m_isInited;

	XVec2 m_rectSize;		//���εĳߴ�
	XVec2 m_windowsPosition;	//���δ��ڵ�λ��
	XVec2 m_texturePosition;	//������ͼ��λ��
	GLuint *m_texture;			//���ε���ͼ
	XPicState m_state;				//�����״̬
	XSprite m_sprite;			//����
	float m_turnAngle;
	float m_turnSpeed;
	XSprite m_spriteShader;	//�������Ӱ
	GLuint *m_textureShader;	//�������Ӱ
	XVec2 m_shaderSize;		//������Ӱ�ĳߴ�
	bool m_isIntTurn;		//�Ƿ��ڷ�ת�Ĺ�����

	GLuint *m_textureSecond;	//����ͼʱ�ĵڶ�����ͼ
	bool m_isChangeTexture;

	//Ϊ���ǵö����Ĳ�����������������
	//XVec2 m_lastPoint;		//��һ������λ��
public:
	unsigned int* getCurTex() { return m_texture; }
	void setCurTex(unsigned int* tex) { m_texture = tex; }
	void setChangeTex(unsigned int* tex)
	{
		m_isChangeTexture = true;
		m_textureSecond = tex;
	}
	void setCurState(XPicState s) { m_state = s; }
	XPicState getCurState()const { return m_state; }
	XVec2 getCenterPos()const { return m_windowsPosition + m_rectSize * 0.5f; }
	void setTurnSpeed(float s) { m_turnSpeed = s; }
	XPicRect()
		:m_texture(nullptr)
		, m_textureSecond(nullptr)
		, m_textureShader(nullptr)
		, m_isInited(false)
	{}
	bool init(const XVec2& rectSize,	//���ӵĴ�С
		const XVec2& windowsPosition,	//���ӵ�λ��
		const XVec2& texturePosition,	//��ͼ��λ��
		GLuint *texture, const XVec2& textureSize,
		const XVec2& shaderSize, GLuint *textureShader);
	//withS:�Ƿ������Ӱ
	void draw(bool withS = true);
	void move(float timeDelay);
};
}
#endif