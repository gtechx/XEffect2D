#ifndef _JIA_XTURNBOARDS_
#define _JIA_XTURNBOARDS_
#define TURN_METHOD 1
#if TURN_METHOD == 0
#include "XPicRect.h"
#endif
#if TURN_METHOD == 1
#include "XPicRectEx.h"
#endif
#define WITH_ALPHA

#include "XShaderGLSL.h"
//�Ż�˵����
//+1������϶Ϊ0�ǿ��Ա任�����С
//+2���ٶȿ���ͨ��MD����̬����
//3��ͼ��ɢ��������
//4�������Ժϳ�����
//+5�����Ӹ���Ļ�����ת��ʽ��ˮƽ���������֣���ֱ����������
//6������position��scale�������Լ�rotation
namespace XE {
enum XTurnBoardDir
{//��תͼƬ�ķ���
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_MAX,
};
enum XTurnBoardMode
{
	MODE_DIRECT,
	MODE_UP,
	MODE_DOWN,
	MODE_LEFT,
	MODE_RIGHT,
	MODE_POINT,
	MODE_MANUAL,	//�ֶ�ģʽ
};
class XTurnBoards:public XObjectBasic
{
private:
	bool m_isInited;
	int m_texWidth;
	int m_texHeight;
	XVec2 m_boardSize;	//����Ĵ�С

	int m_sumX;
	int m_sumY;

	XSprite m_sprite;

#if TURN_METHOD == 0
	XPicRect* m_boards;
	XPicRect* m_boardsSmall;		//С��
	XPicRect* m_boardsWidth;		//���
	XPicRect* m_boardsHeight;		//�ߵ�
#endif
#if TURN_METHOD == 1
	XPicRectEx* m_boards;			//ָ��
	XPicRectEx* m_boardsSmall;		//С��
	XPicRectEx* m_boardsWidth;		//���
	XPicRectEx* m_boardsHeight;		//�ߵ�
#endif
	std::vector<bool>* m_boardsFlag;
	std::vector<bool> m_boardsSmallFlag;
	std::vector<bool> m_boardsWidthFlag;
	std::vector<bool> m_boardsHeightFlag;
	XPicState m_setState;

	XTextureData m_shaderTex[4];	//��Ӱ��ͼ
	std::vector<unsigned int> m_shaderTexs;

	bool m_isInChangeImg;
	std::vector<XMoveData> m_changeRate;
	std::vector<XVec2> m_changeCenter;
	XTurnBoardMode m_turnMode;
	float m_changeTime;	//�仯��ʱ��
	XFBO m_fbo;
#ifdef WITH_ALPHA
	XShaderGLSL m_shader;
	unsigned int m_tex[2];
#endif
	bool initBoards(unsigned int *initTex,int distance);
public:
	float getChangeTime()const { return m_changeTime; }
	void setChangeTime(float s) { m_changeTime = s; }
	//dist:�����뷽��֮��ľ���
	bool init(int texW, int texH, const XVec2& boardSize,unsigned int *initTex,int dist = 0);
	void update(float steptime);
	void draw();
	void setBlendModel(XGL::XBlendModel model) { m_sprite.setBlendModel(model); }
	XTurnBoards()
		:m_isInited(false)
		, m_boards(nullptr)
		, m_isInChangeImg(false)
		, m_changeTime(2.0f)
		, m_boardsSmall(nullptr)
		, m_boardsWidth(nullptr)
		, m_boardsHeight(nullptr)
		, m_boardsFlag(nullptr)
		, m_setState(PIC_STA_NULL)
	{
		m_objType = OBJ_TURNBOARD;
	}
	~XTurnBoards() { release(); }
	void release();
	void turnBoard(const XVec2& pos, XTurnBoardDir dir);

	bool getCanChangeImg()const;
	bool changeImage(unsigned int *targetTex, XTurnBoardMode mode,
		const XVec2& center);
	bool addACenter(const XVec2& c);
public:
	virtual XBool isInRect(const XVec2& p) { return m_sprite.isInRect(p); }
	virtual XVec2 getBox(int order) { return m_sprite.getBox(order); }	
	virtual void setPosition(const XVec2& pos) { m_sprite.setPosition(pos); }
	virtual const XVec2& getPosition() const { return m_sprite.getPosition(); }	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	virtual void setScale(const XVec2& scale) { m_sprite.setScale(scale); }
	virtual const XVec2& getScale() const { return m_sprite.getScale(); }					//��ȡ����ĳߴ�
	virtual void setAngle(float angle) { m_sprite.setAngle(angle); }				//��������ĽǶ�	+Child����
	virtual float getAngle() const { return m_sprite.getAngle(); }						//��ȡ����ĽǶ�

	virtual void setVisible() { m_sprite.setVisible(); }					//��������ɼ�		+Child����
	virtual void disVisible() { m_sprite.disVisible(); }					//����������ɼ�	+Child����
	virtual XBool getVisible() const { return m_sprite.getVisible(); }					//��ȡ����Ƿ�ɼ���״̬ 
	
	virtual void setColor(const XFColor& color) { m_sprite.setColor(color); }
	virtual const XFColor& getColor() const { return m_sprite.getColor(); }
	virtual void setAlpha(float a) { m_sprite.setAlpha(a); }							//+Child����

	void setIsTransformCenter(XTransformMode tmp) { m_sprite.setIsTransformCenter(tmp); }
};
}
#endif