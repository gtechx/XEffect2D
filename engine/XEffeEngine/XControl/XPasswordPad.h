#ifndef _JIA_XPASSWARDPAD_
#define _JIA_XPASSWARDPAD_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

#include "XButton.h"
//����һ�������������ֻ��������޸�������࣬Ŀǰ�����δʵ�֣�֮����ʱ����ʵ��

#define MAX_PASSWARD_LENGTH (14)	//�����������볤��
#define MAX_ENTER_TRY (3)			//�������������Դ���
#define MIN_PASSWARD_LENGTH (4)		//��С���������볤��

//�����������Ҫ��һЩ����
struct _XPasswordPadData
{
	_XVector2 bottonOffsetPosition;	//��ť����ƫ������
	_XVector2 bottonEffectArea;		//��ť��Ӧ������
	_XVector2 bottonDistance;		//��ť֮��ľ���
	_XVector2 bottonTextPosition;	//��ť�����ֵ�λ��
	_XVector2 captionPosition;		//�����ֵ�λ��
};
class _XPasswordPadTexture
{
private:
	_XBool m_isInited;
public:
	_XButtonTexture m_bottomTex;		//��������ͼ

	_XTextureData *backGround;				//��������ı�����ͼ

	_XPasswordPadData m_data;			//����

	_XPasswordPadTexture()
		:m_isInited(XFalse)
		,backGround(NULL)
	{}
	~_XPasswordPadTexture(){release();}
	_XBool init(const char *normal,const char *down,const char *on,const char *disable,
		const char *BG,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};

//Ŀǰ����ؼ��в����ƣ����ܴӿؼ��Ļ�����������������Ҫ����
class _XPasswordPad:public _XControlBasic
{
private:
	_XBool m_isInited;	//�Ƿ��ʼ��

	int m_mode;			//0:���������ģʽ 1:�����޸ĵ�ģʽ
//	_XVector2 m_position;
	_XPasswordPadData m_passwardPadData;	//�����������Ե�һЩ����

	int m_stage;		//��������Ľ׶��Ա����
	int m_couter;
	char m_oldPassword[MIN_PASSWARD_LENGTH + 1];	//�ϵ�����
	char m_newPassword[MAX_PASSWARD_LENGTH + 1];	//�µ�����
	char m_enterPassword[MAX_PASSWARD_LENGTH + 1];	//���������
	char m_enterPasswordLength;
	char m_showString[MAX_PASSWARD_LENGTH + 1];		//������ʾ���ַ���

	_XFontUnicode m_caption;			//��ʾ���ֵ�����
	_XSprite m_sprite;		//��ʾ������ͼ
	_XButton m_button[12];	//12��С���̵İ���

	const _XPasswordPadTexture *m_texture;	//�ؼ�����ͼ
	//_XButtonTexture m_buttonTexture;	//��ť�ؼ�����ͼ

	//_XBool m_isVisible;	//�Ƿ���ʾ

	friend void btnFunc(void *pClass,int ID);

	void addChar(char temp);
	void btn11Proc();
	void btn10Proc();

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ
public:
	_XBool init(const _XVector2& position,_XPasswordPadTexture *tex,const _XFontUnicode& font,const _XPasswordPadData& data);
	_XBool initEx(const _XVector2& position,_XPasswordPadTexture *tex,const _XFontUnicode& font);
	_XBool initPlus(const char * path,const _XFontUnicode& font,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(const _XFontUnicode& font);	
	_XBool initWithoutTex() {return initWithoutTex(XEE::systemFont);}	
protected:
	void draw();
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);
	_XBool keyboardProc(int,_XKeyState) {return XTrue;}
	virtual void insertChar(const char *,int) {;}
	_XBool canGetFocus(float x,float y);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
public:
	int getIsEnd();		//��������Ľ�� ��������ģʽ��0����δ���� 1������������ȷ 2������������� 3���û������˳�
										//�����޸�ģʽ��0����δ���� 1�������޸ĳɹ� 2����������������� 3���û������˳�
	void setPassword(const char *p);	//������Ҫ�������ȷ����
	void getPassword(char *p) const;	//��ȡ�µ�����
	void setStart(int mode);	//������������忪ʼ����
	void setDisShow();

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);
	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
	//virtual void justForTest() {;}

	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	_XPasswordPad();
	~_XPasswordPad();
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XPasswordPad(const _XPasswordPad &temp);
	_XPasswordPad& operator = (const _XPasswordPad& temp);
};
#include "XPasswordPad.inl"

#endif