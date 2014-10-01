#ifndef _JIA_XFRAME_
#define _JIA_XFRAME_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//�Ż�˵��
//1����������ͼ���ļ�������ÿ֡�ĳߴ綼��ͬ����һ���ĺ���������һ��ͼƬ�ϣ�����ʱ�Զ��ü�
//2����һ��С�����Զ���ͼ�������Ч�ߴ�Ĳü���Ȼ�����е�ͼƬ�����������ƴ�ϡ�
//С������ȡ���Ϊx��֡��λ�úͳߴ磬�Լ�ƫ��λ�ã�������ָ�����ļ���

#include "XOSDefine.h"
#ifdef XEE_OS_WINDOWS
#include "windows.h"
#endif
#include "SDL.h"
#include "SDL_image.h"
#include "gl.h"
#include "XBasicClass.h"

#include "math.h"
#include "XBasicFun.h"
#include "XBasicOpenGL.h"
#include "XShaderGLSL.h"

#define MAX_FRAME_SUM (65)		//����֡���ֻ��256֡

class _XFrame
{
private:
	_XSCounter *m_cp;		//���ü�����

	_XResourcePosition m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
	_XBool m_isInited;		//�Ƿ��Ѿ������˳�ʼ��

	float angle;			//����ĽǶ�
	float angleRadian;		//���ȱ�־�ĽǶ�
	float sinAngle;
	float cosAngle;
	_XShaderGLSL *m_pShader;
public:
	void setAngle(float temp);
	float getAngleDegree();	//��ýǶ�
	float getAngleRadian();	//��û���
	_XBool m_isFlash;	//�Ƿ�����
	//void (*m_pShaderProc)(void);	//����һ��shader�ĺ���ָ��
	void setShaderClass(_XShaderGLSL * shader) {m_pShader = shader;}
private:
	float x, y;			//�����λ��
	float xsize, ysize;	//��������ųߴ�
	float alpha;			//͸����
	float colorRed;		//��ɫ
	float colorGreen;		//��ɫ
	float colorBlue;		//��ɫ
	_XBool m_isOverturn;		//�Ƿ�x�������ҷ�ת
	_XBool m_isVisible;

	_XTexture *m_texnum;						//����֡ͼƬ��ָ��

	int m_allFramesSum;							//����֡��������֡��
	int m_allKeyFramesSum;						//����֡�����йؼ�֡������
	float m_nowFramesNumble;					//��ǰ���ŵ��ڼ�֡
	float m_actionSpeed;						//����֡���ŵ��ٶ�
	_XBool m_isLoop;								//����֡�Ƿ����ѭ��
	int m_startFrame;							//����һ֡��ʼ
	_XBool m_isEnd;								//����֡�Ƿ񲥷����
	_XBool m_isEndImmediately;					//�����Ƿ�Ϊ���̽�������
	_XBool m_isSetEnd;							//is end by user			//�Ƿ���������֡����
	_XBool m_isDisappearAtEnd;					//����֡�Ƿ��ڲ������֮����ʧ
	//GLint *m_width;
	//GLint *m_height;							//����ĳߴ�
	int *wre;
	int *hre;
	int *xpa;
	int *ypa;

	char *m_frameName;				//����֡�ļ���
	int *m_keyFrameArray;			//����֡�Ĺؼ�֡�������

	//++++++++++++++++++++++++++++++++++++
	//Ϊ����ƫ�ƶ�����ı���������ͼ�����ת�������Ƕ���ͼƬ���Ľ��еģ����Ե�����֡��������ƫ��ʱ��
	//������Ҫ��������ƫ�ƴ�������ת���������⣨������ʱû�п��ǣ���
	float m_centerX;			//�ܵ�����֡��������
	float m_centerY;
	float *m_centerDW;							//ÿ֡������ƫ��
	float *m_centerDH;			
	_XVector2 *m_keyFramePosition;	//�ؼ�֡ƫ������
	//------------------------------------
public:
	void setVisible() {m_isVisible = XTrue;}					//��������ɼ�
	void disVisible() {m_isVisible = XFalse;}						//����������ɼ�
	_XBool getVisible() const {return m_isVisible;}					//��ȡ����Ƿ�ɼ���״̬ 

	void setAttribute(const _XVector2& position,			//����֡���ŵ�λ��
		_XBool loop,				//����֡�Ƿ�ѭ��
		_XBool endImmediately,		//����֡�Ƿ���������
		int startFrame,				//����֡����ʼ֡
		float actionSpeed,			//����֡�Ĳ����ٶ�(fps * 0.001)
		_XBool disappearAtEnd,		//����֡�����������֮���Ƿ���ʧ
		_XBool isOverturn = XFalse);	//�Ƿ�ת

	void setPosition(const _XVector2& position);			//���þ��������
	void setPosition(float a,float b);			//���þ��������
	_XVector2 getPosition() const;

	int getNowX() const;	//��þ���ͼƬ��ǰ��λ��X(����������ƫ��֮�������Ҫ���¿���)
	int getNowY() const;	//��þ���ͼƬ��ǰ��λ��Y(����������ƫ��֮�������Ҫ���¿���)

	int getNowWidth() const;	//��õ�ǰ֡�Ŀ��
	int getNowHeight() const;	//��õ�ǰ֡�ĸ߶�

	int getInitHalfWidth() const;		//��ȡ��һ֡�İ���,Ҳ��������֡���Ͻǵ�����֡�е�Ŀ��
	int getInitHalfHeight() const;	//��ȡ��һ֡�İ�߶�,Ҳ��������֡���Ͻǵ�����֡�е�ĸ߶�

	void setSize(const _XVector2& size);		//���þ�������ű���
	void setSize(float x,float y);	//���þ�������ű���
	_XVector2 getSize();

	_XBool getIsEnd() const;	//�������֡�Ƿ񲥷����
	void setEnd();	//�����������������
	void reset();	//�����Ѿ�������ɵ�����֡�������²���
	void move(int timeDelay);	//�ƽ�����֡����

	_XTexture * getTexture(int frameFlag = 0);	//���ص�ǰ֡����ͼָ�� frameFlag = 1ʱ���ص�������֡��ͼƬ���������ָ��

	_XBool init(const char *filename,			//ͼƬ������
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);		//��Դ��λ��				
private:
	_XBool releaseMem();									//�ͷ���Դ
public:
	void release(){}		//����ԭ�еĽӿ����¼��ݣ���ʵ����ɶҲ����
	void draw(const GLuint *pTexnum = NULL) const;			//��ͼ��������֡������pTexnumΪ��ͼ��ָ�� ����֡ʱpTexnumΪ����֡��ͼ���ָ��
	void setColor(float r,float g,float b,float a);		//С��0����ı���һԪ��
	void setAlpha(float alpha);	//����͸����
	float getAlpha() const;

	void setActionSpeed(float actionSpeed);	//��������֡���ŵ��ٶ�
private:
	//char m_haveSelfMemery;			//�Ƿ��ڹ��캯���з����ڴ�ռ�
	//��󷵻ص���һ��Ӧ�ã����Բ���Ҫ���忽�����캯��
	_XBool m_isACopy;	//�ж��Ƿ�Ϊ�����壬������û���Լ�����Դ�����ͷ���Դ��ʱ����Ҫ����
	_XFrame(const _XFrame& temp);	//���ظ�ֵ���캯��Ϊ˽�г�Ա����ֹ����Ҫ���������
public:
	_XFrame& operator = (const _XFrame& temp);
	void setACopy(const _XFrame& temp);		//����һ��ͬ���壬��������з����ڴ������︴����ͬ����Դ�����û�з����ڴ�ռ䣬�����ָ��Ŀ����ڴ�ռ�

	void setRestart();			//�������»ص���ʼ֡��ʼ����

	_XFrame();
	//_XFrame(char withNoMemry);	//�������ڴ�ռ䣬����������֡ʹ��ͬһ��ͼƬ��Դʱ����ʹ���������
	~_XFrame();
	//����Ϊ����Դ��ԭ����һЩ����Ľӿ�
	int getAllKeyFrameSum() const {return m_allKeyFramesSum;}
	_XVector2 getKeyFramePos(int index) const {return m_keyFramePosition[index];}
	int getKeyFrameWre(int index) const {return wre[index];}
	int getKeyFrameHre(int index) const {return hre[index];}
};

#include "XFrame.inl"

#endif