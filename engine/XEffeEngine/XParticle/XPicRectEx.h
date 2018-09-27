#ifndef _JIA_XPICRECTEX_
#define _JIA_XPICRECTEX_
#include "XPicRectCommon.h"
#include "XSprite.h"
#include "XMath\XVector3.h"

namespace XE {
class XPicRectEx
{
private:
	bool m_isInited;

	XVec2 m_texturePosition;	//������ͼ��λ��
	GLuint *m_texture;			//���ε���ͼ
	XPicState m_state;				//�����״̬
	float m_turnAngle;
	float m_turnSpeed;
	GLuint *m_textureShader;	//�������Ӱ
	bool m_isIntTurn;			//�Ƿ��ڷ�ת�Ĺ�����

	GLuint *m_textureSecond;	//����ͼʱ�ĵڶ�����ͼ
	//bool m_isChangeTexture;

	XVec2 m_position;
	XVec2 m_size;
	std::vector<XVec3> m_v;
	std::vector<XVec2> m_u;
	std::vector<XVec2> m_su;

	void setCurAngle(float angle);	//ͨ���Ƕȼ���������λ��
public:
	bool init(const XVec2& rectSize,	//���ӵĴ�С
		const XVec2& windowsPosition,	//���ӵ�λ��
		const XVec2& texturePosition,	//��ͼ��λ��
		GLuint *texture, const XVec2& textureSize,
		GLuint *textureShader);

	XPicRectEx()
		:m_isInited(false)
		, m_texture(nullptr)
		, m_textureSecond(nullptr)
		, m_textureShader(nullptr)
		, m_isIntTurn(false)
	{}

	unsigned int* getCurTex() { return m_texture; }
	void setCurTex(unsigned int* tex) { m_texture = tex; }
	void setChangeTex(unsigned int* tex) { m_textureSecond = tex;}
	void setCurState(XPicState s) { m_state = s; }
	XPicState getCurState()const { return m_state; }
	XVec2 getCenterPos()const { return m_position; }
	void setTurnSpeed(float s) { m_turnSpeed = s; }
	//withS:�Ƿ������Ӱ
	void draw(bool withS = true);
	void move(float timeDelay);
};
}
#endif