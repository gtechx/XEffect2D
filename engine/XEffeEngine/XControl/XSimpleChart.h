#ifndef _JIA_XSIMPLECHART_
#define _JIA_XSIMPLECHART_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
//#include "XMath\XVector2.h"
//#include "XFont\XFontUnicode.h"
#include <deque>
#include "XControlBasic.h"
#include "XResourceManager.h"
//����һ���򵥵����ݱ����
//�Ż���
//1��ͳ�Ƶ�ǰ��������ֵ����Сֵ
//2��ͳ�Ƶ�ǰ����ı�׼��
//3��ͳ�Ƶ�ǰ�����ƽ��ֵ
namespace XE{
enum XSimpleChartType
{
	TYPE_0_UP,		//0���������ģʽ
	TYPE_0_MIDDLE,	//0�����м��ģʽ
	TYPE_0_BOTTOM,	//0���������ģʽ
};
enum XSimpleChartShowInfo	//������ʾ����Ϣ
{
	INFO_NULL,	//û����ʾ��Ϣ
	INFO_CUR_VALUE,	//��ǰ����ֵ
	INFO_ALL_RANGLE,	//�����ķ�Χ
	INFO_LOCAL_RANGLE,	//��ǰ�ķ�Χ
	INFO_LOCAL_AVERAGE,	//��ǰ��ƽ��ֵ
	INFO_LOCAL_SD,		//��׼��,standard deviation
};
class XSimpleChart:public XControlBasic
{
private:
	bool m_isInited;
	XVec2 m_size;		//��С
	float m_rateY;			//����Ŀǰ������ϵ��
	float m_rateX;			//����֮��ľ��룬��λΪ����
	XSimpleChartType m_simpleChartType;	//���ߵ���ʽ�����������δ��Ч
	XSimpleChartShowInfo m_showInfo;

	std::deque<float> m_datas;
	int m_maxDataSum;	//��������ʾ��data����
	//���Ե�����
	bool m_isAutoRateY;		//Y�����ϵ������Ƿ�ʹ������Ӧ,trueʱ���ֵ�仯������ϵ�������仯��falseʱ�����зŴ�ʱ�����仯
	XFontUnicode m_caption;	//����
	bool m_showCaption;
	//�����Ƿ�Χͳ�Ƶ�����
	bool m_isFirstPush;	//�Ƿ��ǵ�һ�η�������
	float m_maxData;	//�������ݵ����ֵ
	float m_minData;	//�������ݵ���Сֵ
	float m_curData;	//��ǰ������
	float m_localMaxData;
	float m_localMinData;
	float m_localAverageData;	//��ǰ��ƽ��ֵ
	float m_localSDData;	//��ǰ�ı�׼��

	XFontUnicode m_infoFont;
	XFColor m_textColor;		//���ֵ���ɫ
public:
	XSimpleChartType getSimpleChartType()const{return m_simpleChartType;}
	void setSimpleChartType(XSimpleChartType type){m_simpleChartType = type;}
	XSimpleChartShowInfo getShowInfo()const{return m_showInfo;}
	void setShowInfo(XSimpleChartShowInfo info){m_showInfo = info;}
	XSimpleChart()
		:m_isInited(false)
		,m_maxDataSum(0)
		,m_rateY(1.0f)
		,m_rateX(1.0f)
//		,m_scale(1.0f)
		,m_size(128.0f,512.0f)
		,m_isAutoRateY(true)
		,m_showCaption(false)
		,m_isFirstPush(false)
		,m_maxData(0.0f)
		,m_minData(0.0f)
		,m_curData(0.0f)
		,m_localMaxData(0.0f)
		,m_localMinData(0.0f)
		,m_localAverageData(0.0f)
		,m_localSDData(0.0f)
		,m_simpleChartType(TYPE_0_MIDDLE)
		,m_showInfo(INFO_NULL)
	{
		m_ctrlType = CTRL_OBJ_SIMPLECHART;
	}
	~XSimpleChart(){release();}
	void release();

	void setIsAutoRateY(bool flag){m_isAutoRateY = flag;}
	bool getIsAutoRateY()const{return m_isAutoRateY;}
	XBool init(const XFontUnicode& font,const XVec2& size,float xRate);
	XBool init(const XVec2& size,float xRate)
	{
		return init(getDefaultFont(),size,xRate);
	}
	void setCaption(const char *p);
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
protected:						
	void draw();
	void drawUp(){}
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState){return XFalse;}			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2& ){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
//	void setLostFocus();	//����ʧȥ����
public:
	void update(float stepTime){}
	bool insertAData(float data);
	void clearAll();
	XBool setACopy(const XButton &temp){return XFalse;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XSimpleChart(const XSimpleChart &temp);
	XSimpleChart& operator = (const XSimpleChart& temp);
private:
	void updateInfoStr();
public:
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);
	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(const XVec2& s);
	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(const XFColor& c);
	void setAlpha(float a);
};
#if WITH_INLINE_FILE
#include "XSimpleChart.inl"
#endif
}
#endif