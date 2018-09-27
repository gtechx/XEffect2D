#ifndef _JIA_XSPRITE_
#define _JIA_XSPRITE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//������Ҫ֧�����漸������
//1���ƶ�
//2������
//3���ü�
//4����ת
//5���������ŵĻ�׼��
//6��������ת�Ļ�׼��
//7���������ҷ�ת
//ע������֡��������Ҫ����2��n�η�

//��������ź�λ�ô����߼��Ĳ�������

//���飺ʵ�־���Ķ���βü�����ʵ��һЩ����ı���Ч��

#include "XTextureInformation.h"
#include "XObjectBasic.h"
#include "XStringFun.h"
#include "XThread.h"

namespace XE {
class XShaderGLSL;
enum XTransformMode
{
	POINT_LEFT_TOP = 0,	//���Ͻ�Ϊ��׼��
	POINT_CENTER = 1	//���ĵ�Ϊ��׼��
};
enum XTurnOverMode	//����ķ�תģʽ
{
	TURN_OVER_MODE_NULL,	//û�н��з�ת
	TURN_OVER_MODE_LEFT_TO_RIGHT,	//���ҷ�ת
	TURN_OVER_MODE_UP_TO_DOWN,		//���·�ת
	TURN_OVER_MODE_L2R_AND_U2D,	//�������ҷ�ת
};
#if WITHXSPRITE_EX
class XSprite :public XObjectBasic
{
private:
	XResPos m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�

	XTurnOverMode m_turnOverMode;	//��תģʽ
	XVec2 m_position;			//λ��
	XVec2 m_scale;				//���ųߴ�
	XVec2 m_pixelSize;			//���سߴ�
	float m_angle;					//�Ƕ�
	XVec2 m_changeCenter;		//��ת�������ŵ�����
	//������Ϊ�˱��ڼ������ʱ����
	float m_sinAngle;				//�Ƕȵ���ʱ����
	float m_cosAngle;
	XTextureData m_textureData;	//��ͼTexture����Ϣ
	XFColor m_color;				//�������ɫ

	void updateData(const XTextureData *tex = NULL);
	void drawInside();
	//�����ǹ���alpha��Ϸ�ʽ�Ĵ���
//	XGL::XBlendType m_blendType;	//��Ϸ�ʽ��0���򵥻��2���ӻ�ϡ�1�����ӵ�4���ӻ��
	XGL::XBlendModel m_blendModel;
//	char m_blendRGBScr;
//	char m_blendRGBDst;
//	char m_blendAScr;
//	char m_blendADst;

	XBool m_isInited;				//�Ƿ���ɳ�ʼ��
	XBool m_needAngleClip;			//�Ƿ���Ҫ���нǶȲü�
//	char m_blendTypeScr;
//	char m_blendTypeDst;
	XBool m_isVisible;				//����Ƿ�ɼ�
	XBool m_needUpdateData;		//�Ƿ���Ҫ�����ڲ�����
	XBool m_needClip;				//�Ƿ���Ҫ�ü�
	XRect m_clipRect;				//�ü�������
	//������shader��ָ��
	XShaderGLSL *m_pShader;
	//�����Ƕ��ڽǶȲü��Ķ��壺�ǶȲü�������Ϊ�е㣬��ʱ��������һ��ĽǶȲü�
	XVec2 m_rectPoint[4];	//��Χ��
	int m_pointSum;			//���������
	int m_upTexDataID;		//��һ��������ͼ��ID��������Է�ֹ�ظ���ˢ������

	float m_vPointer[14];	//��������
	float m_uPointer[14];	//��ͼ����
	float m_clipAngle;		//�ü��ĽǶ�[0 - 360]
private:
	XSprite(const XSprite& temp);
public:
	void setAngle(float angle);		//[0-360]
	float getAngle() const;
	using XObjectBasic::setScale;
	void setScale(const XVec2& s);	//���þ�������ű���
	const XVec2& getScale() const;
	void fitScale(float x, float y, bool deformation = true);	//�Զ���Ӧ�ƶ������سߴ�
	void fitScale(const XVec2& s, bool deformation = true);	//�Զ���Ӧ�ƶ������سߴ�
	using XObjectBasic::setPosition;
	void setPosition(const XVec2& p);			//���þ��������
	void setPositionX(float x);
	void setPositionY(float y);
	const XVec2& getPosition() const;
	void setTurnOverMode(XTurnOverMode mode);
	void setClipRect(const XRect& clipRect);	//ʹ����������
	void setClipRect(float left, float top, float right, float bottom);
	void disClip();
	void setChangeCenter(const XVec2& center);
	XBool init(const char * filename,										//�ļ���
		XResPos resPos = RES_SYS_DEF,	//��Դ��λ�� 0�ⲿ 1�ڲ�
		const XVec2& changePoint = XVec2(0.5f));					//���εľ�׼��
	XBool init(int tempW = 0, int tempH = 0, int needSizeCheck = 1, const XVec2& changePoint = XVec2(0.5f));
	XBool init(XTextureData& texData, const XVec2& changePoint = XVec2(0.5f));
	XBool init(int w, int h, XColorMode colorMode, unsigned int tex, const XVec2& changePoint = XVec2(0.5f));	//��һ����ͼ����һ������
	void draw();
	void draw(GLuint tex);	//�����ڲ���Դ�����ڲ��ü����Ի��������
	void draw(const XTextureData& texData);	//������δ�����ڲ��ü�������Ŀǰ���������
	void draw(const XVec2 *u, const XVec2 *v, int w, int h);	//�����Ϊ�˱��ζ�����Ľӿ�
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++															//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	bool m_isThreadLoading;		//�Ƿ����ڶ��߳�����
	XThreadState m_loadThread;
	std::string m_threadFilename;
	XResPos m_threadResPos;
public:
	//ʹ�ö��̵߳ķ�ʽ�Ӻ�̨������Դ
	bool initThread(const char *filename,			//ͼƬ������
		XResPos resPos = RES_SYS_DEF,
		const XVec2& changePoint = XVec2(0.5f));
	static DWORD WINAPI loadThread(void *);
	//�����Ƿ�������
	bool getIsLoaded()const { return !m_isThreadLoading; }
	bool updateTex(const char *filename);	//���ļ��и������ݵ���ͼ��Ҫ��ߴ�����ظ�ʽһ��
	bool updateThread(const char *filename);	//���ļ��и������ݵ���ͼ��Ҫ��ߴ�����ظ�ʽһ��
	static DWORD WINAPI updateThread(void *);
	//------------------------------------------------------------
	XSprite()
		:m_isInited(XFalse)
		, m_turnOverMode(TURN_OVER_MODE_NULL)
		, m_needClip(XFalse)
		, m_color(1.0f, 1.0f)
		, m_isVisible(XTrue)
		, m_pShader(NULL)
		//,m_pShaderProc(NULL)	//Ϊ�����¼��ݶ������
//		, m_blendType(XGL::BLEND_TWO_DATA)
//		, m_blendTypeScr(4)
//		, m_blendTypeDst(5)
		, m_blendModel(XGL::BLEND_ALPHA)
		, m_needAngleClip(XFalse)
		, m_clipAngle(0.0f)
		, m_upTexDataID(-1)
		, m_isThreadLoading(false)
	{
		m_objType = OBJ_SPRITE;
		//	for(int i = 0;i < 14;++ i)
		//	{
		//		m_vPointer[i] = 0.0f;	//��������
		//		m_uPointer[i] = 0.0f;	//��ͼ����
		//	}
		memset(m_vPointer, 0, 14 * sizeof(float));
		memset(m_uPointer, 0, 14 * sizeof(float));
	}
	~XSprite() { release(); }
	void release();
	bool getIsInited()const { return m_isInited; }

	//������һЩ��δʵ�ֵĽӿ�
	XBool isInRect(const XVec2& p);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);

	void setAlpha(float temp);
	float getAlpha() const;
	using XObjectBasic::setColor;
	void setColor(const XFColor&c);		//С��0����ı���һԪ��
	const XFColor& getColor() const { return m_color; }

	void setVisible();			//��������ɼ�
	void disVisible();			//����������ɼ�
	XBool getVisible() const;
	//alpha��ϵĽӿ�
//	void setBlendType(int typeScr, int typeDst);
//	void setBlendType(int rgbScr, int rgbDst, int aScr, int aDst);
//	void setBlendMode(XGL::XBlendType mode);
	void setBlendModel(XGL::XBlendModel model);
	//shader�Ľӿ�
	void setShaderClass(XShaderGLSL * shader);
	//��������δʵ�ֵĽӿ�
	XTexture * getTexture();	//���ص�ǰ֡����ͼָ��
	XTextureData * getTextureData();	//���ص�ǰ֡����ͼ����

	void setAngleClip(float angle);
	void disAngleClip();

	XSprite& operator = (const XSprite& temp);	//��Ŀ�����Դ���й��ã�������
	void setACopy(const XSprite& temp);
	//����ʵ��һЩ���¼��ݵĽӿ�
	void setIsTransformCenter(XTransformMode temp)
	{
		if (temp == POINT_LEFT_TOP) setChangeCenter(XVec2::zero);
		else setChangeCenter(XVec2(0.5f));
	}
	void draw(GLuint *tex) { draw(*tex); }	//�����ڲ���Դ�����ڲ��ü����Ի��������
	void draw(const XTextureData* texData) { draw(*texData); }	//������δ�����ڲ��ü�������Ŀǰ���������
	void drawWithoutBlend() { draw(); }
	XBool init(const char * filename,					//�ļ���
		XResPos resPos,					//��Դ��λ�� 0�ⲿ 1�ڲ�
		XTransformMode isTransformCenter)	//���εľ�׼��
	{
		if (isTransformCenter == POINT_CENTER) return init(filename, resPos, XVec2(0.5f));
		else return init(filename, resPos, XVec2::zero);
	}
	XBool init(int tempW, int tempH, int needSizeCheck, XTransformMode isTransformCenter)
	{
		if (isTransformCenter == POINT_CENTER) return init(tempW, tempH, needSizeCheck, XVec2(0.5f));
		else return init(tempW, tempH, needSizeCheck, XVec2::zero);
	}
	XBool init(XTextureData& texData, XTransformMode isTransformCenter)
	{
		if (isTransformCenter == POINT_CENTER) return init(texData, XVec2(0.5f));
		else return init(texData, XVec2::zero);
	}
	XBool init(int w, int h, XColorMode colorMode, unsigned int tex, XTransformMode isTransformCenter)
	{
		if (isTransformCenter == POINT_CENTER) return init(w, h, colorMode, tex, XVec2(0.5f));
		else return init(w, h, colorMode, tex, XVec2::zero);
	}
	//	void (*m_pShaderProc)(void);
	void setOverturn(char temp)	//�������ҷ�ת(Ϊ�˱�����ɰ汾�ļ��ݱ�������ӿ�)
	{
		if (temp == 0) resetLeftRightUpDown();
		else setLeft2Right();
	}
	void setLeft2Right() { setTurnOverMode(TURN_OVER_MODE_LEFT_TO_RIGHT); }//�������ҷ�ת
	void setUp2Down() { setTurnOverMode(TURN_OVER_MODE_UP_TO_DOWN); }		//�������·�ת
	void resetLeftRightUpDown() { setTurnOverMode(TURN_OVER_MODE_NULL); }//�ظ��������ҵ���ȷλ��
	XTurnOverMode getTurnOverMode() const { return m_turnOverMode; }
	XTransformMode getTransformMode() const
	{
		if (m_changeCenter.x == 0.0f && m_changeCenter.y == 0.0f) return POINT_LEFT_TOP;
		else return POINT_CENTER;
	}
	int XSprite::getTW() const { return m_textureData.texture.m_w; }	//��ȡ��ͼ�Ŀ�ȣ���������Ż�����Ϊ�Ż�֮��Ŀ��
	int XSprite::getTH() const { return m_textureData.texture.m_h; }
	int XSprite::getW() const { return m_textureData.getOpWidth(); }	//��ȡ��ͼ����Ч���ֵĿ�ȣ����û�о����Ż���Ϊ������ͼ��С
	int XSprite::getH() const { return m_textureData.getOpHeight(); }

	//	void justForTest() {;}
};
#else
//�¹��ܵ�ʵ�ֽ��飺ʵ�ֶ���βü�
//#define MAX_SPRITE_FILENAME_LENGTH (256)
#define IS_USE_SOLIDIFY (0)

class XSprite :public XObjectBasic	//����Ľṹ��
{
private:
	XResPos m_resoursePosition;				//��Դλ�� 0:�ⲿ 1:�ڲ�

	XTextureData m_textureData;

	XVec2 m_setPosition;			//���鱻���õ�λ�ã��ڲ�ʹ�õ�λ�ã������õ�λ�����������
	XVec2 m_setTransformCenter;		//�û����õ���״�任��λ�ã�ʵ��ʹ�õı��ε�λ�������õ�λ�����������

	XRect m_clipOutsideRect;			//�û����õ��ⲿ�ü��ߴ�
	XRect m_clipRect;					//���յĲü��ߴ磬����ߴ����ڲ��ü��ߴ����ⲿ�ü��ߴ����
	char m_isEnableOutsideChip;			//�Ƿ��������
	char m_isVisible;

	void updateClipAndRotatoData();		//������ͼ�ı�������²ü�����ת������
	char m_needUpdateInsideData;		//�Ƿ���Ҫ�����ڲ�����
	void updateInsideData();			//���ھ�����������Ա仯����Ҫ���¾����һЩ�ڲ�����
public:
	void disClip();						//ȡ��2D����ü���Ĭ��Ϊ���ü�
	void setClipRect(const XRect& temp);		//����2D����ü���Χ
	//ע������������⣬���ͼƬС���귽���п��࣬�ü�֮��ƴ�ϣ�Ȼ����������������Ҫ��֤left��topΪ0�������������⡣
	void setClipRect(float left, float top, float right, float bottom);	//����2D����ü���Χ

	int isInRect(float x, float y);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getPosition() const { return m_setPosition; }
	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	XVec2 getBox(int order);
private:
	// �������һЩ������������εĴ���
	XTurnOverMode m_turnOverMode;	//�Ƿ�����������״�ı�ǩ
	float m_shapeData[16];	//�������״����������
private:
	//GLfloat x, y;			//�������ʾλ��
	XTransformMode m_isTransformCenter;

	//ע��Ĭ�ϵ���ת�����ŵ����ĵ���ͳһ��ΪͼƬ�����ģ����ͼƬ���ڲü�����Ϊ�ü���������Ͻ�
	XVec2 m_transformCenter;		//���λ���ת����ʵλ��
	char m_changeTransformCenterManual;	//�û��Ƿ��ֶ����ù���ת���ĵ�λ�ã�������ת�������Ͻ���ת�������û�����
public:
	void setIsTransformCenter(XTransformMode temp);	//�������ŵĻ�׼��
	XTransformMode getTransformMode() const { return m_isTransformCenter; }

	void setTransformCenter(float x, float y)	//����Ƿ񾭹�����
	{
		m_changeTransformCenterManual = 1;
		if (m_textureData.isEnableInsideClip != 0)
		{
			m_setTransformCenter.set(x, y);
			if (m_clipOutsideRect.left < m_textureData.textureMove.x) m_transformCenter.x = m_clipOutsideRect.left - m_textureData.textureMove.x + m_setTransformCenter.x;
			else m_transformCenter.x = m_setTransformCenter.x;
			if (m_clipOutsideRect.top < m_textureData.textureMove.y) m_transformCenter.y = m_clipOutsideRect.top - m_textureData.textureMove.y + m_setTransformCenter.y;
			else m_transformCenter.y = m_setTransformCenter.y;

			//m_transformCenter.set(x - m_textureData.textureMove.x,y - m_textureData.textureMove.y);
		}
		else
		{
			m_transformCenter.set(x, y);
		}
	}
public:
	void setAlpha(float temp);
	float getAlpha() const;
	float getAngle() const { return angle; }
	void setAngle(float temp);
	int getW() const;
	int getH() const;
	void setVisible() { m_isVisible = 1; }					//��������ɼ�
	void disVisible() { m_isVisible = 0; }						//����������ɼ�
	char getVisible() const { return m_isVisible; }					//��ȡ����Ƿ�ɼ���״̬ 

private:
	GLfloat angle;			//����ĽǶ�
	float sinAngle;			//�Ƕȵ�����	//�����Ϊ���Ż������ٶȶ��趨�ı���
	float cosAngle;			//�Ƕȵ�����

	GLfloat m_positionX;	//������õ�λ��
	GLfloat m_positionY;
	GLfloat xsize, ysize;	//��������ųߴ�
	GLfloat alpha;			//͸����
	GLfloat colorRed;		//��ɫ
	GLfloat colorGreen;		//��ɫ
	GLfloat colorBlue;		//��ɫ
//	char m_isOverturn;		//�Ƿ�x�������ҷ�ת
	//Ϊ�����������ٶȣ����涨��һЩ�м����
	int wr, hr, xp, yp;
	//	char *m_frameName;				//����֡�ļ���
public:
	void setPosition(const XVec2& position);			//���þ��������
	void setPosition(float a, float b);			//���þ��������
	void setPositionX(float x);
	void setPositionY(float y);

	void setSize(const XVec2& size);		//���þ�������ű���
	void setSize(float a, float b);	//���þ�������ű���
	void setSize(float s);
	XVec2 getScale() const
	{
		return XVec2(xsize, ysize);
	}

	XTexture * getTexture();	//���ص�ǰ֡����ͼָ��
	XTextureData * getTextureData();	//���ص�ǰ֡����ͼ����
#if IS_USE_SOLIDIFY
//++++++++++++++++++++++++++++++++++++++++++++
//����Ϊ�������ٶȶ�����ı���
//���̻���������ʾ��Ϣ�̻�����ʾ�б��У�����ʾ�����в��ٱ仯��ʾϸ�ڣ���������̶���û��ϸ�ڱ仯��Ԫ������ʹ���������
	void setIsUseSolidify(int temp);
private:
	int m_glListOrder;		//��ʾ�б���
	int m_needSolidify;		//�Ƿ���Ҫ�̻�
	//--------------------------------------------
#endif
private:
	char m_isInited;
	char m_blendTypeScr;
	char m_blendTypeDst;
public:
	void setBlendType(int typeScr, int typeDst)
	{
		if (typeScr < 0) typeScr = 0;
		if (typeDst < 0) typeDst = 0;
		if (typeScr >= 9) typeScr = 8;
		if (typeDst >= 8) typeDst = 7;
		m_blendTypeScr = typeScr;
		m_blendTypeDst = typeDst;
	}
	//�����ǶԻ�����͵�һЩǿ��
private:
	XBlendType m_blendType;	//��Ϸ�ʽ��0���򵥻��2���ӻ�ϡ�1�����ӵ�4���ӻ��
	char m_blendRGBScr;
	char m_blendRGBDst;
	char m_blendAScr;
	char m_blendADst;
public:
	void setBlendType(int rgbScr, int rgbDst, int aScr, int aDst)
	{
		if (rgbScr < 0) rgbScr = 0;
		if (rgbScr >= 9) rgbScr = 8;
		if (aScr < 0) aScr = 0;
		if (aScr >= 9) aScr = 8;

		if (rgbDst < 0) rgbDst = 0;
		if (rgbDst >= 8) rgbDst = 7;
		if (aDst < 0) aDst = 0;
		if (aDst >= 8) aDst = 7;
		m_blendRGBScr = rgbScr;
		m_blendRGBDst = rgbDst;
		m_blendAScr = aScr;
		m_blendADst = aDst;
	}
	void setBlendMode(XBlendType mode) { m_blendType = mode; }
	XShaderGLSL *m_pShader;
	void(*m_pShaderProc)(void);	//����һ��shader�ĺ���ָ��
public:
	int init(const char *filename,						//ͼƬ������
		XResPos resPos = RES_SYS_DEF,					//��Դ��λ�� 0�ⲿ 1�ڲ�
		XTransformMode isTransformCenter = POINT_CENTER);	//�Ƿ�ʹ��������ת	

	int init(int tempW = 0, int tempH = 0, int needSizeCheck = 1, XTransformMode isTransformCenter = POINT_CENTER);	//�Ƿ�Ϊ����֡����
	int init(XTextureData& texData, XTransformMode isTransformCenter = POINT_CENTER);
	int init(int w, int h, int colorMode, unsigned int tex, XTransformMode isTransformCenter = POINT_CENTER);

	int release();									//�ͷ���Դ

	//ע�⣺
	//�ⲿ����drawϵ�к�����ʱ���ǲ��ܴ��ݲ�����

	//������Ļ�ͼ��ʽ
	void drawBasic(const GLuint *pTexnum = NULL);					//��ԭʼ�������Ļ�ͼ��ʽ�����ܽϵ�
	void drawBasic(const XTextureData *pTexData);// = NULL);					//��ԭʼ�������Ļ�ͼ��ʽ�����ܽϵ�
	//���������������Ⱥ�黯�Ż�
	//ע��������������ⲿ����ͼƬʱһ��Ҫȷ�ϲ���ƴ���Ż��е�ͼƬ������������ʾ����
	void draw(const GLuint *pTexnum = NULL);			//���ⲿ������ͼ�����л��ƣ�����������ͼ������ԭ�е���ͼ������ͬ�ĳߴ磬������������
	void draw(const XTextureData *pTexData);// = NULL);			//���ⲿ������ͼ�����л��ƣ�����������ͼ������ԭ�е���ͼ������ͬ�ĳߴ磬������������
	//û�вü���û�з�ת��û��͸��ͨ����ʱ���ʹ�����������������������,���ַ����ٶȽϿ죬��Դ��ʡ
	void drawEx(const GLuint *pTexnum = NULL);				//���Ч�Ļ�ͼ��ʽ������Ҫ�ü��ȹ���
	void drawEx(const XTextureData *pTexDatam);// = NULL);				//���Ч�Ļ�ͼ��ʽ������Ҫ�ü��ȹ���
	//����Ҫ���л��
	void drawWithoutBlend(const GLuint *pTexnum = NULL);
	void drawWithoutBlend(const XTextureData *pTexDatam);
private:
	void drawInside();	//�ڲ����õĻ�ͼ����
public:
	void setColor(float r, float g, float b, float a);		//С��0����ı���һԪ��
	void setColor(const XFColor& color);		//С��0����ı���һԪ��
	//���ƾ���
private:
	int m_isACopy;	//�ж��Ƿ�Ϊ�����壬������û���Լ�����Դ�����ͷ���Դ��ʱ����Ҫ����
	XSprite(const XSprite& temp);					//�ض��帳ֵ���캯��Ϊ˽�У���ֹ������ö���ɴ���

	//�����ǹ��ڽǶȲü�������
	char m_needAngleClip;	//�Ƿ���Ҫ�ǶȲü�
	float m_clipAngle;		//�ǶȲü��ĽǶ� 0 - 360��
	XVec2 m_pointArray[14];	//����Ҫ��ʱ��ˢ����Щ����
	float m_uPoint[14];	//��ͼ����
	float m_vPoint[14];
	int m_pointArraySize;
#ifdef GET_ALL_PIXEL
	float m_thisDrawArea;	//����������
#endif
	char m_needUpdatePointArray;	//�Ƿ���Ҫ�����ڲ���������
	void updatePointArray();	//�����ڲ�����ˢ��
public:
	void setAngleClip(float angle)
	{
		m_needAngleClip = 1;
		m_clipAngle = angle;	//����Ƕ���Ҫ����
		m_needUpdatePointArray = 1;
	}
	void disAngleClip()
	{
		m_needAngleClip = 0;
		m_needUpdatePointArray = 1;
	}
public:
	XSprite& operator = (const XSprite& temp);	//��Ŀ�����Դ���й��ã�������
	void setACopy(const XSprite& temp);			//��Ŀ�����Դ���й��ã�������

	void setOverturn(char temp);	//�������ҷ�ת(Ϊ�˱�����ɰ汾�ļ��ݱ�������ӿ�)
	void setLeft2Right();	//�������ҷ�ת
	void setUp2Down();		//�������·�ת
	void resetLeftRightUpDown();	//�ظ��������ҵ���ȷλ��
	XTurnOverMode getTurnOverMode() const
	{
		return m_turnOverMode;
	}

	XSprite();
	~XSprite();
};
#endif

struct XSpriteParamData
{//��������Ĳ���
	XResPos resourcePosition;	//��ȡ��Դ��λ��
	XVec2 changePoint;					//��ת���������ĵ�λ��
	char * getStrFromData()
	{
		char *tempStr = XMem::createArrayMem<char>(256);
		if (tempStr == NULL) return NULL;
		int offset = 0;
		sprintf_s(tempStr + offset, 256 - offset, "ResPos:%d,\n", resourcePosition);
		offset = strlen(tempStr);
		sprintf_s(tempStr + offset, 256 - offset, "ChaCnt:%f|%f,\n", changePoint.x, changePoint.y);
		offset = strlen(tempStr);
		return tempStr;
	}
	int getDataFromStr(const char * str)
	{
		if (str == NULL) return 0;
		int offset = 0;
		if (sscanf(str + offset, "ResPos:%d,\n", &resourcePosition) != 1) return offset;	//���ݴ���
		offset += XString::getCharPosition(str + offset, ',') + 1;
		if (sscanf(str + offset, "ChaCnt:%f|%f,\n", &changePoint.x, &changePoint.y) != 1) return offset;
		offset += XString::getCharPosition(str + offset, ',') + 1;
		return offset;
	}
	XBool getDataFromFile(FILE *fp)
	{
		if (fp == NULL) return XFalse;
		if (fscanf(fp, "ResPos:%d,\n", &resourcePosition) != 1) return XFalse;
		if (fscanf(fp, "ChaCnt:%f|%f,\n", &changePoint.x, &changePoint.y) != 2) return XFalse;
		return XTrue;
	}
	//Ĭ������
	XSpriteParamData()
		:resourcePosition(RES_SYS_DEF)
		, changePoint(0.5f, 0.5f)
	{}
};
#if WITH_INLINE_FILE
#include "XSprite.inl"
#endif
}
#endif