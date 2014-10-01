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

#include "XOSDefine.h"
#ifdef XEE_OS_WINDOWS
#include "windows.h"
#endif

//#include "SDL.h"
//#include "SDL_image.h"
#include "gl.h"
#include "XBasicClass.h"

#include "math.h"
#include "XBasicFun.h"
#include "XBasicOpenGL.h"
#include "XTextureInformation.h"
#include "XObjectBasic.h"
#include "XShaderGLSL.h"

enum _XTransformMode
{
	POINT_LEFT_TOP = 0,	//���Ͻ�Ϊ��׼��
	POINT_CENTER = 1	//���ĵ�Ϊ��׼��
};
enum _XTurnOverMode	//����ķ�תģʽ
{
	TURN_OVER_MODE_NULL,	//û�н��з�ת
	TURN_OVER_MODE_LEFT_TO_RIGHT,	//���ҷ�ת
	TURN_OVER_MODE_UP_TO_DOWN,		//���·�ת
	TURN_OVER_MODE_L2R_AND_U2D,	//�������ҷ�ת
};

//class _XShaderGLSL;

#if WITH_XSPRITE_EX
class _XSprite:public _XObjectBasic
{
private:
	_XBool m_isInited;						//�Ƿ���ɳ�ʼ��
	_XResourcePosition m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�

	_XTurnOverMode m_turnOverMode;	//��תģʽ
	_XVector2 m_position;			//λ��
	_XVector2 m_size;				//���ųߴ�
	_XVector2 m_pixelSize;			//���سߴ�
	float m_angle;					//�Ƕ�
	_XVector2 m_changeCenter;		//��ת�������ŵ�����
	_XBool m_needClip;				//�Ƿ���Ҫ�ü�
	_XRect m_clipRect;				//�ü�������
	//������Ϊ�˱��ڼ������ʱ����
	float m_sinAngle;				//�Ƕȵ���ʱ����
	float m_cosAngle;
	_XTextureData m_textureData;	//��ͼTexture����Ϣ
	_XFColor m_color;				//�������ɫ
	_XBool m_isVisible;				//����Ƿ�ɼ�

	_XBool m_needUpdateData;			//�Ƿ���Ҫ�����ڲ�����
	void updateData();
	void drawInside();
	//�����ǹ���alpha��Ϸ�ʽ�Ĵ���
	_XBlendType m_blendType;	//��Ϸ�ʽ��0���򵥻��2���ӻ�ϡ�1�����ӵ�4���ӻ��
	char m_blendRGBScr;
	char m_blendRGBDst;
	char m_blendAScr;
	char m_blendADst;

	char m_blendTypeScr;
	char m_blendTypeDst;
	//������shader��ָ��
	_XShaderGLSL *m_pShader;
	//�����Ƕ��ڽǶȲü��Ķ��壺�ǶȲü�������Ϊ�е㣬��ʱ��������һ��ĽǶȲü�
	_XVector2 m_rectPoint[4];	//��Χ��
	int m_pointSum;			//���������

	int m_upTexDataID;		//��һ��������ͼ��ID��������Է�ֹ�ظ���ˢ������

	float m_vPointer[14];	//��������
	float m_uPointer[14];	//��ͼ����
	_XBool m_needAngleClip;	//�Ƿ���Ҫ���нǶȲü�
	float m_clipAngle;		//�ü��ĽǶ�[0 - 360]
private:
	_XSprite(const _XSprite& temp);	
public:
	void setAngle(float angle);		//[0-360]
	float getAngle() const;
	using _XObjectBasic::setSize;
	void setSize(float x,float y);	//���þ�������ű���
	_XVector2 getSize() const;
	using _XObjectBasic::setPosition;
	void setPosition(float x,float y);			//���þ��������
	void setPositionX(float x);
	void setPositionY(float y);
	_XVector2 getPosition() const;
	void setTurnOverMode(_XTurnOverMode mode);
	void setClipRect(const _XRect& clipRect);	//ʹ����������
	void setClipRect(float left,float top,float right,float bottom);
	void disClip();
	void setChangeCenter(const _XVector2& center);
	_XBool init(const char * filename,										//�ļ���
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,	//��Դ��λ�� 0�ⲿ 1�ڲ�
		const _XVector2 &changePoint = _XVector2(0.5f,0.5f));					//���εľ�׼��
	_XBool init(int tempW = 0,int tempH = 0,int needSizeCheck = 1,const _XVector2 &changePoint = _XVector2(0.5f,0.5f));
	_XBool init(_XTextureData& texData,const _XVector2 &changePoint = _XVector2(0.5f,0.5f));	
	_XBool init(int w,int h,_XColorMode colorMode,unsigned int tex,const _XVector2 &changePoint = _XVector2(0.5f,0.5f));	//��һ����ͼ����һ������
	void draw();
	void draw(GLuint tex);	//�����ڲ���Դ�����ڲ��ü����Ի��������
	void draw(const _XTextureData& texData);	//������δ�����ڲ��ü�������Ŀǰ���������
	void draw(const _XVector2 *u,const _XVector2 *v,int w,int h);	//�����Ϊ�˱��ζ�����Ľӿ�

	_XSprite()
		:m_isInited(XFalse)
		,m_turnOverMode(TURN_OVER_MODE_NULL)
		,m_needClip(XFalse)
		,m_color(1.0f,1.0f,1.0f,1.0f)
		,m_isVisible(XTrue)
		,m_pShader(NULL)
		//,m_pShaderProc(NULL)	//Ϊ�����¼��ݶ������
		,m_blendType(BLEND_TWO_DATA)
		,m_blendTypeScr(4)
		,m_blendTypeDst(5)
		,m_needAngleClip(XFalse)
		,m_clipAngle(0.0f)
		,m_upTexDataID(-1)
	{
		m_objType = OBJ_SPRITE;
		for(int i = 0;i < 14;++ i)
		{
			m_vPointer[i] = 0.0f;	//��������
			m_uPointer[i] = 0.0f;	//��ͼ����
		}
	}
	~_XSprite(){release();}
	void release();

	//������һЩ��δʵ�ֵĽӿ�
	_XBool isInRect(float x,float y);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);

	void setAlpha(float temp);
	float getAlpha() const;
	using _XObjectBasic::setColor;
	void setColor(float r,float g,float b,float a);		//С��0����ı���һԪ��
	_XFColor getColor() const {return m_color;}

	void setVisible();			//��������ɼ�
	void disVisible();			//����������ɼ�
	_XBool getVisible() const;
	//alpha��ϵĽӿ�
	void setBlendType(int typeScr,int typeDst);
	void setBlendType(int rgbScr,int rgbDst,int aScr,int aDst);
	void setBlendMode(_XBlendType mode);
	//shader�Ľӿ�
	void setShaderClass(_XShaderGLSL * shader);
	//��������δʵ�ֵĽӿ�
	_XTexture * getTexture();	//���ص�ǰ֡����ͼָ��
	_XTextureData * getTextureData();	//���ص�ǰ֡����ͼ����

	void setAngleClip(float angle);
	void disAngleClip();

	_XSprite& operator = (const _XSprite& temp);	//��Ŀ�����Դ���й��ã�������
	void setACopy(const _XSprite& temp);
	//����ʵ��һЩ���¼��ݵĽӿ�
	void setIsTransformCenter(_XTransformMode temp)
	{
		if(temp == POINT_LEFT_TOP) setChangeCenter(_XVector2::zero);
		else setChangeCenter(_XVector2(0.5f,0.5f));
	}
	void draw(GLuint *tex) {draw(*tex);}	//�����ڲ���Դ�����ڲ��ü����Ի��������
	void draw(const _XTextureData* texData) {draw(*texData);}	//������δ�����ڲ��ü�������Ŀǰ���������
	void drawWithoutBlend() {draw();}
	_XBool init(const char * filename,					//�ļ���
		_XResourcePosition resoursePosition,					//��Դ��λ�� 0�ⲿ 1�ڲ�
		_XTransformMode isTransformCenter)	//���εľ�׼��
	{
		if(isTransformCenter == POINT_CENTER) return init(filename,resoursePosition,_XVector2(0.5f,0.5f));
		else return init(filename,resoursePosition,_XVector2::zero);
	}
	_XBool init(int tempW,int tempH,int needSizeCheck,_XTransformMode isTransformCenter)
	{
		if(isTransformCenter == POINT_CENTER) return init(tempW,tempH,needSizeCheck,_XVector2(0.5f,0.5f));
		else return init(tempW,tempH,needSizeCheck,_XVector2::zero);
	}
	_XBool init(_XTextureData& texData,_XTransformMode isTransformCenter)
	{
		if(isTransformCenter == POINT_CENTER) return init(texData,_XVector2(0.5f,0.5f));
		else return init(texData,_XVector2::zero);
	}
	_XBool init(int w,int h,_XColorMode colorMode,unsigned int tex,_XTransformMode isTransformCenter)
	{
		if(isTransformCenter == POINT_CENTER) return init(w,h,colorMode,tex,_XVector2(0.5f,0.5f));
		else return init(w,h,colorMode,tex,_XVector2::zero);
	}
//	void (*m_pShaderProc)(void);
	void setOverturn(char temp)	//�������ҷ�ת(Ϊ�˱�����ɰ汾�ļ��ݱ�������ӿ�)
	{
		if(temp == 0) resetLeftRightUpDown();
		else setLeft2Right();
	}
	void setLeft2Right()	//�������ҷ�ת
	{
		setTurnOverMode(TURN_OVER_MODE_LEFT_TO_RIGHT);
	}
	void setUp2Down() {setTurnOverMode(TURN_OVER_MODE_UP_TO_DOWN);}		//�������·�ת
	void resetLeftRightUpDown()	{setTurnOverMode(TURN_OVER_MODE_NULL);}//�ظ��������ҵ���ȷλ��
	_XTurnOverMode getTurnOverMode() const {return m_turnOverMode;}
	_XTransformMode getTransformMode() const
	{
		if(m_changeCenter.x == 0.0f && m_changeCenter.y == 0.0f) return POINT_LEFT_TOP;
		else return POINT_CENTER;
	}
	int _XSprite::getW() const {return m_textureData.texture.m_w;}
	int _XSprite::getH() const {return m_textureData.texture.m_h;}

//	void justForTest() {;}
};
#else
//�¹��ܵ�ʵ�ֽ��飺ʵ�ֶ���βü�
//#define MAX_SPRITE_FILENAME_LENGTH (256)
#define IS_USE_SOLIDIFY (0)

class _XSprite:public _XObjectBasic	//����Ľṹ��
{
private:
	_XResourcePosition m_resoursePosition;				//��Դλ�� 0:�ⲿ 1:�ڲ�

	_XTextureData m_textureData;

	_XVector2 m_setPosition;			//���鱻���õ�λ�ã��ڲ�ʹ�õ�λ�ã������õ�λ�����������
	_XVector2 m_setTransformCenter;		//�û����õ���״�任��λ�ã�ʵ��ʹ�õı��ε�λ�������õ�λ�����������

	_XRect m_clipOutsideRect;			//�û����õ��ⲿ�ü��ߴ�
	_XRect m_clipRect;					//���յĲü��ߴ磬����ߴ����ڲ��ü��ߴ����ⲿ�ü��ߴ����
	char m_isEnableOutsideChip;			//�Ƿ��������
	char m_isVisible;

	void updateClipAndRotatoData();		//������ͼ�ı�������²ü�����ת������
	char m_needUpdateInsideData;		//�Ƿ���Ҫ�����ڲ�����
	void updateInsideData();			//���ھ�����������Ա仯����Ҫ���¾����һЩ�ڲ�����
public:
	void disClip();						//ȡ��2D����ü���Ĭ��Ϊ���ü�
	void setClipRect(const _XRect &temp);		//����2D����ü���Χ
	//ע������������⣬���ͼƬС���귽���п��࣬�ü�֮��ƴ�ϣ�Ȼ����������������Ҫ��֤left��topΪ0�������������⡣
	void setClipRect(float left,float top,float right,float bottom);	//����2D����ü���Χ

	int isInRect(float x,float y);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getPosition() const
	{
		return m_setPosition;
	}
	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	_XVector2 getBox(int order);
private:
	// �������һЩ������������εĴ���
	_XTurnOverMode m_turnOverMode;	//�Ƿ�����������״�ı�ǩ
	float m_shapeData[16];	//�������״����������
private:
	//GLfloat x, y;			//�������ʾλ��
	_XTransformMode m_isTransformCenter;

	//ע��Ĭ�ϵ���ת�����ŵ����ĵ���ͳһ��ΪͼƬ�����ģ����ͼƬ���ڲü�����Ϊ�ü���������Ͻ�
	_XVector2 m_transformCenter;		//���λ���ת����ʵλ��
	char m_changeTransformCenterManual;	//�û��Ƿ��ֶ����ù���ת���ĵ�λ�ã�������ת�������Ͻ���ת�������û�����
public:
	void setIsTransformCenter(_XTransformMode temp);	//�������ŵĻ�׼��
	_XTransformMode getTransformMode() const
	{
		return m_isTransformCenter;
	}

	void setTransformCenter(float x,float y)	//����Ƿ񾭹�����
	{
		m_changeTransformCenterManual = 1;
		if(m_textureData.isEnableInsideClip != 0)
		{
			m_setTransformCenter.set(x,y);
			if(m_clipOutsideRect.left < m_textureData.textureMove.x) m_transformCenter.x = m_clipOutsideRect.left - m_textureData.textureMove.x + m_setTransformCenter.x;
			else m_transformCenter.x = m_setTransformCenter.x;
			if(m_clipOutsideRect.top < m_textureData.textureMove.y) m_transformCenter.y = m_clipOutsideRect.top - m_textureData.textureMove.y + m_setTransformCenter.y;
			else m_transformCenter.y = m_setTransformCenter.y; 

			//m_transformCenter.set(x - m_textureData.textureMove.x,y - m_textureData.textureMove.y);
		}else
		{
			m_transformCenter.set(x,y);
		}
	}
public:
	void setAlpha(float temp);
	float getAlpha() const; 
	float getAngle() const
	{
		return angle;
	}
	void setAngle(float temp);
	int getW() const;
	int getH() const;
	void setVisible() {m_isVisible = 1;}					//��������ɼ�
	void disVisible() {m_isVisible = 0;}						//����������ɼ�
	char getVisible() const {return m_isVisible;}					//��ȡ����Ƿ�ɼ���״̬ 

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
	int wr,hr,xp,yp;
//	char *m_frameName;				//����֡�ļ���
public:	
	void setPosition(const _XVector2& position);			//���þ��������
	void setPosition(float a,float b);			//���þ��������
	void setPositionX(float x);
	void setPositionY(float y);

	void setSize(const _XVector2& size);		//���þ�������ű���
	void setSize(float a,float b);	//���þ�������ű���
	void setSize(float s);
	_XVector2 getSize() const
	{
		return _XVector2(xsize,ysize);
	}

	_XTexture * getTexture();	//���ص�ǰ֡����ͼָ��
	_XTextureData * getTextureData();	//���ص�ǰ֡����ͼ����
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
	void setBlendType(int typeScr,int typeDst)
	{
		if(typeScr < 0) typeScr = 0;
		if(typeDst < 0) typeDst = 0;
		if(typeScr >= 9 ) typeScr = 8;
		if(typeDst >= 8 ) typeDst = 7;
		m_blendTypeScr = typeScr;
		m_blendTypeDst = typeDst;
	}
	//�����ǶԻ�����͵�һЩǿ��
private:
	_XBlendType m_blendType;	//��Ϸ�ʽ��0���򵥻��2���ӻ�ϡ�1�����ӵ�4���ӻ��
	char m_blendRGBScr;
	char m_blendRGBDst;
	char m_blendAScr;
	char m_blendADst;
public:
	void setBlendType(int rgbScr,int rgbDst,int aScr,int aDst)
	{
		if(rgbScr < 0) rgbScr = 0;
		if(rgbScr >= 9 ) rgbScr = 8;
		if(aScr < 0) aScr = 0;
		if(aScr >= 9 ) aScr = 8;

		if(rgbDst < 0) rgbDst = 0;
		if(rgbDst >= 8 ) rgbDst = 7;
		if(aDst < 0) aDst = 0;
		if(aDst >= 8 ) aDst = 7;
		m_blendRGBScr = rgbScr;
		m_blendRGBDst = rgbDst;
		m_blendAScr = aScr;
		m_blendADst = aDst;
	}
	void setBlendMode(_XBlendType mode) {m_blendType = mode;}
	_XShaderGLSL *m_pShader;
	void (*m_pShaderProc)(void);	//����һ��shader�ĺ���ָ��
public:
	int init(const char *filename,						//ͼƬ������
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,					//��Դ��λ�� 0�ⲿ 1�ڲ�
		_XTransformMode isTransformCenter = POINT_CENTER);	//�Ƿ�ʹ��������ת	

	int init(int tempW = 0,int tempH = 0,int needSizeCheck = 1,_XTransformMode isTransformCenter = POINT_CENTER);	//�Ƿ�Ϊ����֡����
	int init(_XTextureData& texData,_XTransformMode isTransformCenter = POINT_CENTER);
	int init(int w,int h,int colorMode,unsigned int tex,_XTransformMode isTransformCenter = POINT_CENTER);

	int release();									//�ͷ���Դ

	//ע�⣺
	//�ⲿ����drawϵ�к�����ʱ���ǲ��ܴ��ݲ�����

	//������Ļ�ͼ��ʽ
	void drawBasic(const GLuint *pTexnum = NULL);					//��ԭʼ�������Ļ�ͼ��ʽ�����ܽϵ�
	void drawBasic(const _XTextureData *pTexData);// = NULL);					//��ԭʼ�������Ļ�ͼ��ʽ�����ܽϵ�
	//���������������Ⱥ�黯�Ż�
	//ע��������������ⲿ����ͼƬʱһ��Ҫȷ�ϲ���ƴ���Ż��е�ͼƬ������������ʾ����
	void draw(const GLuint *pTexnum = NULL);			//���ⲿ������ͼ�����л��ƣ�����������ͼ������ԭ�е���ͼ������ͬ�ĳߴ磬������������
	void draw(const _XTextureData *pTexData);// = NULL);			//���ⲿ������ͼ�����л��ƣ�����������ͼ������ԭ�е���ͼ������ͬ�ĳߴ磬������������
	//û�вü���û�з�ת��û��͸��ͨ����ʱ���ʹ�����������������������,���ַ����ٶȽϿ죬��Դ��ʡ
	void drawEx(const GLuint *pTexnum = NULL);				//���Ч�Ļ�ͼ��ʽ������Ҫ�ü��ȹ���
	void drawEx(const _XTextureData *pTexDatam);// = NULL);				//���Ч�Ļ�ͼ��ʽ������Ҫ�ü��ȹ���
	//����Ҫ���л��
	void drawWithoutBlend(const GLuint *pTexnum = NULL);
	void drawWithoutBlend(const _XTextureData *pTexDatam);
private:
	void drawInside();	//�ڲ����õĻ�ͼ����
public:
	void setColor(float r,float g,float b,float a);		//С��0����ı���һԪ��
	void setColor(const _XFColor& color);		//С��0����ı���һԪ��
	//���ƾ���
private:
	int m_isACopy;	//�ж��Ƿ�Ϊ�����壬������û���Լ�����Դ�����ͷ���Դ��ʱ����Ҫ����
	_XSprite(const _XSprite& temp);					//�ض��帳ֵ���캯��Ϊ˽�У���ֹ������ö���ɴ���

	//�����ǹ��ڽǶȲü�������
	char m_needAngleClip;	//�Ƿ���Ҫ�ǶȲü�
	float m_clipAngle;		//�ǶȲü��ĽǶ� 0 - 360��
	_XVector2 m_pointArray[14];	//����Ҫ��ʱ��ˢ����Щ����
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
	_XSprite& operator = (const _XSprite& temp);	//��Ŀ�����Դ���й��ã�������
	void setACopy(const _XSprite& temp);			//��Ŀ�����Դ���й��ã�������

	void setOverturn(char temp);	//�������ҷ�ת(Ϊ�˱�����ɰ汾�ļ��ݱ�������ӿ�)
	void setLeft2Right();	//�������ҷ�ת
	void setUp2Down();		//�������·�ת
	void resetLeftRightUpDown();	//�ظ��������ҵ���ȷλ��
	_XTurnOverMode getTurnOverMode() const
	{
		return m_turnOverMode;
	}

	_XSprite();
	~_XSprite();
};
#endif

struct _XSpriteParamData
{//��������Ĳ���
	_XResourcePosition resourcePosition;	//��ȡ��Դ��λ��
	_XVector2 changePoint;					//��ת���������ĵ�λ��
	char * getStrFromData()
	{
		char *tempStr = createArrayMem<char>(256);
		if(tempStr == NULL) return NULL;
		int offset = 0;
		sprintf(tempStr + offset,"ResPos:%d,\n",resourcePosition);
		offset = strlen(tempStr);
		sprintf(tempStr + offset,"ChaCnt:%f|%f,\n",changePoint.x,changePoint.y);
		offset = strlen(tempStr);
		return tempStr;
	}
	int getDataFromStr(const char * str)
	{
		if(str == NULL) return 0;
		int offset = 0;
		if(sscanf(str + offset,"ResPos:%d,\n",&resourcePosition) != 1) return offset;	//���ݴ���
		offset += getCharPosition(str + offset,',') + 1;
		if(sscanf(str + offset,"ChaCnt:%f|%f,\n",&changePoint.x,&changePoint.y) != 1) return offset;
		offset += getCharPosition(str + offset,',') + 1;
		return offset;
	}
	_XBool getDataFromFile(FILE *fp)
	{
		if(fp == NULL) return XFalse;
		if(fscanf(fp,"ResPos:%d,\n",&resourcePosition) != 1) return XFalse;
		if(fscanf(fp,"ChaCnt:%f|%f,\n",&changePoint.x,&changePoint.y) != 2) return XFalse;
		return XTrue;
	}
	//Ĭ������
	_XSpriteParamData()
		:resourcePosition(RESOURCE_SYSTEM_DEFINE)
		,changePoint(0.5f,0.5f)
	{
	}
};

#include "XSprite.inl"
#endif