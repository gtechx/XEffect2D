#ifndef _JIA_XCHART_
#define _JIA_XCHART_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.5.19
//--------------------------------
//����һ��ͼ���Ŀؼ��࣬������ʾС��ģ���еȹ�ģ������
//���ܻ�����ɣ�������δ�����Ż��ʹ�������
//#include <vector>
#include <deque>
#include "XCheck.h"
namespace XE{
class XChart:public XControlBasic
{
private:
	static const int m_chartMaxLineSum = 12;	//�����������ߵ�����
	static XFColor m_chartLineColor[m_chartMaxLineSum];
	bool m_isInited;	//�Ƿ��ʼ��
	bool m_withPoint;
	char m_chooseArm;	//ѡ���Ŀ�� 0��û��ѡ��1ѡ��ͷ��2ѡ��β��
	bool m_neadUpdateAllLineData;	//�Ƿ���Ҫ�������е�������
	bool m_neddUpdateCurLineData;	//�Ƿ���Ҫ���µ�ǰ��������

	XRect m_dataRect;		//������ʾ������

	int m_horizontalNetSum;	//ˮƽϸ�ָ������
	int m_verticalNetSum;	//��ֱϸ�ָ������

	//std::vector<XVector2> m_datas[m_chartMaxLineSum];	//���������е�����
	std::deque<XVector2> m_datas[m_chartMaxLineSum];
	int m_maxDataSum;	//��������������ʾ��������ݵ�����
	//std::string m_dataName[m_chartMaxLineSum];	//�ߵ�˵������
	XCheck m_dataChecks[m_chartMaxLineSum];
	int m_maxLineLen;	//���Ա���ĵ������
	//������������ֵ���Ļ�׼����
	XVector2 m_baseValue;	//ԭ��Ļ�׼��ֵ
	XVector2 m_sizeValue;	//����Ĵ�С

	XRect m_dataRange;	//���ݵķ�Χ
	int m_dataSum;	//���ݵ�����
	XVector2 m_showDataRange;

	//������������ʾ�Ķ���
	XFontUnicode m_caption;
	XFontUnicode m_textFont;	//��ʾ������

	//���������ڻ�ͼ����ʱ����
	XVector2 m_firstNetValue;	//�������ĵ�һ������
	XVector2 m_stepWidth;		//�������Ĳ���
	int m_dataAccuracyX;		//���ֵľ���
	int m_dataAccuracyY;		//���ֵľ���

	void checkRange();	//���ȡֵ��Χ�Ƿ񳬹��������ݵķ�Χ��������������������Χ
	void updateRange();	//���·�Χ
	//������vbo����ض���
	unsigned int m_v[m_chartMaxLineSum];	//����ͼ�Ķ�����Ϣ
	void allLineUpdateVbo();
	unsigned int m_curPoint[m_chartMaxLineSum];	//���µ�ǰ�����ϵĵ�
	unsigned int m_curV[m_chartMaxLineSum];	//���µ�ǰ���ߵĵ�
	void curLineUpdateVbo();
	int m_linePointSum[m_chartMaxLineSum];	//��ǰ��ͼ����Ҫ���ĵ������
	int m_linePointsSum[m_chartMaxLineSum];	//��ǰ��ͼ����Ҫ���ĵ������
	float *m_drawDataBuff;
	int m_drawDataBuffSize;

	static void chartChkCtrlFun(void *,int,int);
	bool checkCanHideLine();	//����Ƿ����Ӱ�����ߣ�����Ҫ������һ�����ߴ�����ʾ״̬
	//����Լ������
	float m_downHeight;			//����������ʾ�ĸ߶�
	float m_allLineRectHeight;	//������ʾ�����ߵ�����ĸ߶�
public:
	bool initWithoutSkin(const XRect &rect,const char *caption,const XFontUnicode &font);
	bool initWithoutSkin(const XRect &rect,const char *caption) {return initWithoutSkin(rect,caption,getDefaultFont());}
	bool initWithoutSkin(const XVector2 &pixelSize,const char *caption) 
	{
		return initWithoutSkin(XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),caption,getDefaultFont());
	}
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);		//������궯������Ӧ����
	XBool keyboardProc(int,XKeyState) {return XFalse;}
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);//�Ƿ���Ի�ý���
	XBool canLostFocus(float,float) {return XTrue;}	//��ǰ�ؼ��Ƿ����ʧȥ���㣬���ڴ���������������
public:
	int getMaxDataSum()const{ return m_maxDataSum; }
	void setMaxDataSum(int sum);	//���õ�����������������������,<=0 Ϊ��������
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(float x,float y);
	void insertData(const XVector2 &data,int lineIndex);
	void setLineName(const std::string &name,int lineIndex);

	XBool isInRect(float x,float y);
	XVector2 getBox(int order);
	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
	void release();
	void setDrawPointer(bool flag);	//�����Ƿ�����
	bool getDrawPointer(){return m_withPoint;}
	void disable();
	void enable();
	XChart()
		:m_isInited(false)
		,m_baseValue(0.0f,0.0f)
		,m_sizeValue(1.0f,1.0f)
		//,m_isMouseDown(false)
		,m_chooseArm(0)
		,m_dataSum(0)
		,m_maxLineLen(0)
		,m_drawDataBuff(NULL)
		,m_withPoint(false)
		,m_neadUpdateAllLineData(false)
		,m_neddUpdateCurLineData(false)
		, m_maxDataSum(-1)
	{
		m_ctrlType = CTRL_OBJ_CHART;
	}
	~XChart(){release();}

	XBool readDataFromFile(const char * filename);	//���ļ��ж�ȡ��������
	XBool saveDataToFile(const char * filename);	//�ӽ��������ݱ��浽�ļ���
};
#if WITH_INLINE_FILE
#include "XChart.inl"
#endif
}
#endif