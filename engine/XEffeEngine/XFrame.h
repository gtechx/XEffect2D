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

#include "XBasicClass.h"
namespace XE{
class XShaderGLSL;
class XTexture;
class XFrame
{
private:
	static const int m_maxFrameSum = 65;//����֡���֡������
	XSCounter *m_cp;		//���ü�����

	XResPos m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
	XBool m_isInited;		//�Ƿ��Ѿ������˳�ʼ��

	float angle;			//����ĽǶ�
	float angleRadian;		//���ȱ�־�ĽǶ�
	float sinAngle;
	float cosAngle;
	XShaderGLSL *m_pShader;
public:
	void setAngle(float temp);
	float getAngleDegree();	//��ýǶ�
	float getAngleRadian();	//��û���
	XBool m_isFlash;	//�Ƿ�����
	//void (*m_pShaderProc)(void);	//����һ��shader�ĺ���ָ��
	void setShaderClass(XShaderGLSL * shader) {m_pShader = shader;}
private:
	float x, y;			//�����λ��
	float xsize, ysize;	//��������ųߴ�
	float alpha;			//͸����
	float colorRed;		//��ɫ
	float colorGreen;		//��ɫ
	float colorBlue;		//��ɫ
	XBool m_isOverturn;		//�Ƿ�x�������ҷ�ת
	XBool m_isVisible;

	XTexture *m_texnum;						//����֡ͼƬ��ָ��

	int m_allFramesSum;							//����֡��������֡��
	int m_allKeyFramesSum;						//����֡�����йؼ�֡������
	float m_curFramesNumble;					//��ǰ���ŵ��ڼ�֡
	float m_actionSpeed;						//����֡���ŵ��ٶ�
	XBool m_isLoop;								//����֡�Ƿ����ѭ��
	int m_startFrame;							//����һ֡��ʼ
	XBool m_isEnd;								//����֡�Ƿ񲥷����
	XBool m_isEndImmediately;					//�����Ƿ�Ϊ���̽�������
	XBool m_isSetEnd;							//is end by user			//�Ƿ���������֡����
	XBool m_isDisappearAtEnd;					//����֡�Ƿ��ڲ������֮����ʧ
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
	XVec2 *m_keyFramePosition;	//�ؼ�֡ƫ������
	//------------------------------------
public:
	void setVisible() {m_isVisible = XTrue;}					//��������ɼ�
	void disVisible() {m_isVisible = XFalse;}						//����������ɼ�
	XBool getVisible() const {return m_isVisible;}					//��ȡ����Ƿ�ɼ���״̬ 

	void setAttribute(const XVec2& position,			//����֡���ŵ�λ��
		XBool loop,				//����֡�Ƿ�ѭ��
		XBool endImmediately,		//����֡�Ƿ���������
		int startFrame,				//����֡����ʼ֡
		float actionSpeed,			//����֡�Ĳ����ٶ�(fps * 0.001)
		XBool disappearAtEnd,		//����֡�����������֮���Ƿ���ʧ
		XBool isOverturn = XFalse);	//�Ƿ�ת

	void setPosition(const XVec2& position);			//���þ��������
	void setPosition(float a,float b);			//���þ��������
	XVec2 getPosition() const;

	int getCurX() const;	//��þ���ͼƬ��ǰ��λ��X(����������ƫ��֮�������Ҫ���¿���)
	int getCurY() const;	//��þ���ͼƬ��ǰ��λ��Y(����������ƫ��֮�������Ҫ���¿���)

	int getCurWidth() const;	//��õ�ǰ֡�Ŀ��
	int getCurHeight() const;	//��õ�ǰ֡�ĸ߶�

	int getInitHalfWidth() const;		//��ȡ��һ֡�İ���,Ҳ��������֡���Ͻǵ�����֡�е�Ŀ��
	int getInitHalfHeight() const;	//��ȡ��һ֡�İ�߶�,Ҳ��������֡���Ͻǵ�����֡�е�ĸ߶�
	XVec2 getInitHalfSize()const { return XVec2(getInitHalfWidth(), getInitHalfHeight()); }
	void setScale(const XVec2& size);		//���þ�������ű���
	void setScale(float x,float y);	//���þ�������ű���
	void setScale(float s);
	XVec2 getScale();

	XBool getIsEnd() const;	//�������֡�Ƿ񲥷����
	void setEnd();	//�����������������
	void reset();	//�����Ѿ�������ɵ�����֡�������²���
	void move(float timeDelay);	//�ƽ�����֡����

	XTexture * getTexture(bool isAllFrames = false);	//���ص�ǰ֡����ͼָ�� isAllFrames:�Ƿ񷵻���������֡��ͼƬ���������ָ��

	XBool init(const char *filename,			//ͼƬ������
		XResPos resPos = RES_SYS_DEF);		//��Դ��λ��				
private:
	bool loadFromFolder(const char *filename);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename);		//����ҳ�ж�ȡ��Դ

	XBool releaseMem();									//�ͷ���Դ
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
	XBool m_isACopy;	//�ж��Ƿ�Ϊ�����壬������û���Լ�����Դ�����ͷ���Դ��ʱ����Ҫ����
	XFrame(const XFrame& temp);	//���ظ�ֵ���캯��Ϊ˽�г�Ա����ֹ����Ҫ���������
public:
	XFrame& operator = (const XFrame& temp);
	void setACopy(const XFrame& temp);		//����һ��ͬ���壬��������з����ڴ������︴����ͬ����Դ�����û�з����ڴ�ռ䣬�����ָ��Ŀ����ڴ�ռ�

	void setRestart();			//�������»ص���ʼ֡��ʼ����

	XFrame();
	//XFrame(char withNoMemry);	//�������ڴ�ռ䣬����������֡ʹ��ͬһ��ͼƬ��Դʱ����ʹ���������
	~XFrame();
	//����Ϊ����Դ��ԭ����һЩ����Ľӿ�
	int getAllKeyFrameSum() const {return m_allKeyFramesSum;}
	XVec2 getKeyFramePos(int index) const {return m_keyFramePosition[index];}
	int getKeyFrameWre(int index) const {return wre[index];}
	int getKeyFrameHre(int index) const {return hre[index];}
};
#if WITH_INLINE_FILE
#include "XFrame.inl"
#endif
}
#endif