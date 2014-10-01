#ifndef _JIA_XCHART_
#define _JIA_XCHART_

//����һ��ͼ���Ŀؼ��࣬������ʾС��ģ���еȹ�ģ������
//���ܻ�����ɣ�������δ�����Ż��ʹ�������
#include <vector>
#include "XCheck.h"

#define CHART_MAX_LINE_SUM (12)	//����������ߵ�����

class _XChart:public _XControlBasic
{
private:
	bool m_isInited;	//�Ƿ��ʼ��
	bool m_withPoint;

	_XRect m_dataRect;		//������ʾ������

	int m_horizontalNetSum;	//ˮƽϸ�ָ������
	int m_verticalNetSum;	//��ֱϸ�ָ������

	std::vector<_XVector2> m_datas[CHART_MAX_LINE_SUM];	//���������е�����
	//std::string m_dataName[CHART_MAX_LINE_SUM];	//�ߵ�˵������
	_XCheck m_dataChecks[CHART_MAX_LINE_SUM];
	int m_maxLineLen;	//���Ա���ĵ������
	//������������ֵ���Ļ�׼����
	_XVector2 m_baseValue;	//ԭ��Ļ�׼��ֵ
	_XVector2 m_sizeValue;	//����Ĵ�С

	char m_chooseArm;	//ѡ���Ŀ�� 0��û��ѡ��1ѡ��ͷ��2ѡ��β��
	_XRect m_dataRange;	//���ݵķ�Χ
	int m_dataSum;	//���ݵ�����
	_XVector2 m_showDataRange;

	//������������ʾ�Ķ���
	_XFontUnicode m_caption;
	_XFontUnicode m_textFont;	//��ʾ������

	//���������ڻ�ͼ����ʱ����
	_XVector2 m_firstNetValue;	//�������ĵ�һ������
	_XVector2 m_stepWidth;		//�������Ĳ���
	int m_dataAccuracyX;		//���ֵľ���
	int m_dataAccuracyY;		//���ֵľ���

	void checkRange();	//���ȡֵ��Χ�Ƿ񳬹��������ݵķ�Χ��������������������Χ
	void updateRange();	//���·�Χ
	//������vbo����ض���
	unsigned int m_v[CHART_MAX_LINE_SUM];	//����ͼ�Ķ�����Ϣ
	void allLineUpdateVbo();
	unsigned int m_nowPoint[CHART_MAX_LINE_SUM];	//���µ�ǰ�����ϵĵ�
	unsigned int m_nowV[CHART_MAX_LINE_SUM];	//���µ�ǰ���ߵĵ�
	void nowLineUpdateVbo();
	int m_linePointSum[CHART_MAX_LINE_SUM];	//��ǰ��ͼ����Ҫ���ĵ������
	int m_linePointsSum[CHART_MAX_LINE_SUM];	//��ǰ��ͼ����Ҫ���ĵ������
	float *m_drawDataBuff;
	int m_drawDataBuffSize;
	bool m_neadUpdateAllLineData;	//�Ƿ���Ҫ�������е�������
	bool m_neddUpdateNowLineData;	//�Ƿ���Ҫ���µ�ǰ��������

	friend void chartChkCtrlFun(void *pClass,int objectID);
	bool checkCanHideLine();	//����Ƿ����Ӱ�����ߣ�����Ҫ������һ�����ߴ�����ʾ״̬
	//����Լ������
	float m_downHeight;			//����������ʾ�ĸ߶�
	float m_allLineRectHeight;	//������ʾ�����ߵ�����ĸ߶�
public:
	bool initWithoutTex(const _XRect &rect,const char *caption,const _XFontUnicode &font);
	bool initWithoutTex(const _XRect &rect,const char *caption) {return initWithoutTex(rect,caption,XEE::systemFont);}
	bool initWithoutTex(const _XVector2 &pixelSize,const char *caption) 
	{
		return initWithoutTex(_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),caption,XEE::systemFont);
	}
protected:
	void draw();
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//������궯������Ӧ����
	_XBool keyboardProc(int,_XKeyState) {return XFalse;}
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);//�Ƿ���Ի�ý���
	_XBool canLostFocus(float,float) {return XTrue;}	//��ǰ�ؼ��Ƿ����ʧȥ���㣬���ڴ�������������
public:
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);
	void insertData(const _XVector2 &data,int lineIndex);
	void setLineName(const std::string &name,int lineIndex);

	_XBool isInRect(float x,float y);
	_XVector2 getBox(int order);
	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
	void release();
	void setDrawPointer(bool flag);	//�����Ƿ�����
	bool getDrawPointer(){return m_withPoint;}
	void disable();
	void enable();
	_XChart()
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
		,m_neddUpdateNowLineData(false)
	{
		m_ctrlType = CTRL_OBJ_XCHART;
	}
	~_XChart(){release();}

	_XBool readDataFromFile(const char * filename);	//���ļ��ж�ȡ��������
	_XBool saveDataToFile(const char * filename);	//�ӽ��������ݱ��浽�ļ���
};
#include "XChart.inl"
#endif