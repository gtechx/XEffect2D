#ifndef _JIA_XPASSWARDPAD_
#define _JIA_XPASSWARDPAD_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#include "XButton.h"
namespace XE{
//����һ�������������ֻ��������޸�������࣬Ŀǰ�����δʵ�֣�֮����ʱ����ʵ��

//�����������Ҫ��һЩ����
struct XPasswordPadData
{
	XVector2 bottonOffsetPosition;	//��ť����ƫ������
	XVector2 bottonEffectArea;		//��ť��Ӧ������
	XVector2 bottonDistance;		//��ť֮��ľ���
	XVector2 bottonTextPosition;	//��ť�����ֵ�λ��
	XVector2 captionPosition;		//�����ֵ�λ��
};
class XPasswordPadSkin
{
private:
	XBool m_isInited;
public:
	XButtonSkin m_bottomTex;		//��������ͼ
	XTextureData *backGround;				//��������ı�����ͼ
	XPasswordPadData m_data;			//����
	XPasswordPadSkin()
		:m_isInited(XFalse)
		,backGround(NULL)
	{}
	~XPasswordPadSkin(){release();}
	XBool init(const char *normal,const char *down,const char *on,const char *disable,
		const char *BG,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool initEx(const char *filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
private:
	bool loadFromFolder(const char *filename,XResourcePosition resPos);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename,XResourcePosition resPos);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename,XResourcePosition resPos);		//����ҳ�ж�ȡ��Դ
};

//Ŀǰ����ؼ��в����ƣ����ܴӿؼ��Ļ�����������������Ҫ����
class XPasswordPad:public XControlBasic
{
private:
	static const int m_maxEnterTry = 3;			//�������������Դ���
	static const int m_maxPasswardLength = 14;	//�����������볤��
	static const int m_minPasswardLength = 4;	//��С���������볤��

	int m_mode;			//0:���������ģʽ 1:�����޸ĵ�ģʽ
//	XVector2 m_position;
	XPasswordPadData m_passwardPadData;	//�����������Ե�һЩ����

	int m_stage;		//��������Ľ׶��Ա����
	int m_couter;
	char m_oldPassword[m_minPasswardLength + 1];	//�ϵ�����
	char m_newPassword[m_maxPasswardLength + 1];	//�µ�����
	char m_enterPassword[m_maxPasswardLength + 1];	//���������
	char m_enterPasswordLength;
	char m_showString[m_maxPasswardLength + 1];		//������ʾ���ַ���
	XBool m_isInited;	//�Ƿ��ʼ��

	XFontUnicode m_caption;			//��ʾ���ֵ�����
	XSprite m_sprite;		//��ʾ������ͼ
	XButton m_button[12];	//12��С���̵İ���

	const XPasswordPadSkin *m_texture;	//�ؼ�����ͼ
	//XButtonTexture m_buttonTexture;	//��ť�ؼ�����ͼ

	//XBool m_isVisible;	//�Ƿ���ʾ

	static void ctrlProc(void*,int,int);

	void addChar(char temp);
	void btn11Proc();
	void btn10Proc();

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//û����ͼ����ʽ
public:
	XBool init(const XVector2& position,XPasswordPadSkin *tex,const XFontUnicode& font,const XPasswordPadData& data);
	XBool initEx(const XVector2& position,XPasswordPadSkin *tex,const XFontUnicode& font);
	XBool initPlus(const char * path,const XFontUnicode& font,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool initWithoutSkin(const XFontUnicode& font);	
	XBool initWithoutSkin() {return initWithoutSkin(getDefaultFont());}	
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);
	XBool keyboardProc(int,XKeyState) {return XTrue;}
	virtual void insertChar(const char *,int) {;}
	XBool canGetFocus(float x,float y);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
public:
	int getIsEnd();		//��������Ľ�� ��������ģʽ��0����δ���� 1������������ȷ 2������������� 3���û������˳�
										//�����޸�ģʽ��0����δ���� 1�������޸ĳɹ� 2����������������� 3���û������˳�
	void setPassword(const char *p);	//������Ҫ�������ȷ����
	void getPassword(char *p) const;	//��ȡ�µ�����
	void setStart(int mode);	//������������忪ʼ����
	void setDisShow();

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(float x,float y);
	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
	//virtual void justForTest() {;}

	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	XPasswordPad();
	~XPasswordPad();
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XPasswordPad(const XPasswordPad &temp);
	XPasswordPad& operator = (const XPasswordPad& temp);
};
#if WITH_INLINE_FILE
#include "XPasswordPad.inl"
#endif
}
#endif