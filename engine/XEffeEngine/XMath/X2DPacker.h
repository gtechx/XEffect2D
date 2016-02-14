#ifndef _JIA_X2DPACKER_
#define _JIA_X2DPACKER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//#include "XBasicClass.h"
#include "XVector2.h"
namespace XE{
//2D,����תװ������
//1��ʹ�õ�һ����װ�뼰װ����㷨

/*�������Ͳο���ͼ
|- 0 1-|
|_ 3 2_|
*/
//struct XCorner
//{
//	XBool m_isEnable;			//��������Ƿ���Ч
//	XVector2 m_position;	//�����λ��;
//	int m_type;				//���������
//};

//��Ҫ���õ�����
class XObjectBox
{
public:
	int m_setOrder;			//�ⲿ��ţ����û����õģ����ֵ�ڲ�����ı�
    int m_order;			//����ı�ţ�������ڲ����
    int m_basicBoxOrder;    //��������ķ���ı�� -1��ʾ��û�з���
    XVector2 m_position;	//�����λ��

    XVector2 m_size;		//����ĳߴ�
    float m_acreage;        //��������

	XRect m_AABB;			//����ĺ��ӣ������ж��Ƿ����
	XBool m_needRotate;		//���屻����֮���Ƿ���Ҫ������ת
};

//���ڷ�������Ļ������
class XBasicBox
{
public:
    int m_order;				//���ӵı��
    int m_beUsed;				//�����Ƿ�ʹ��:0��ʾ��װ��������n��ʾװ����n������
	XVector2 m_size;			//���ӵĳߴ�
    float m_releaseAcreage;		//���ӵ�ʣ��ռ�

	int *m_objectBoxesOrder;	//�Ѿ��������ӵĺ��ӵı���б�
	//���ǵ����ַ��÷�ʽ(û��ʵ��)
//    int *m_xEdge;				//��Ч��������ˮƽ��
//    int *m_yEdge;				//��Ч���Կ��Ĵ�ֱ��
	//���ǵ�һ�ַ��÷�ʽ
	int m_mayPositionSum;		//��Ч����������
	XVector2 *m_mayPosition;			//���Է���ĵ�
	//���ǵڶ��ַ��÷�ʽ
	//�Ż��������п��ܵĹս�
	int m_mayPositionXSum;		//�п��ܵ�X������
	int *m_mayPositionX;		//�п��ܵ�X
	int m_mayPositionYSum;		//�п��ܵ�Y������
	int *m_mayPositionY;		//�п��ܵ�Y

    int putInRatio(const XObjectBox &temp);    //���ӷ���ĺ��ʶ�
};

//2D����תװ�����
class X2DPacker
{
private:
	int putInABox(XObjectBox &objectBox);	//��һ������װ�����ӣ������Ƿ�װ��ɹ�
	int canPutIn(XObjectBox &objectBox,XBasicBox &basicBox);	//�Ƿ��ܷŵ����������
	bool m_isAdvancedOptimize;	//�Ƿ�Ϊ�߼��Ż����߼��Ż�Ч�����ã����ĵ�ʱ�����

	void orderObjectByWidth();	//ͨ����ȶ������������
	void orderObjectByArea();	//ͨ������������������
	void orderObjectByBigEdgeAndArea();	//ͨ������������������
	void updatePutOperate(XObjectBox &objectBox,XBasicBox &basicBox,XBool needRotate,float positionX,float positionY);	//��ɷ�������������Ӧ������
	void updateNewPoint(XBasicBox &basicBox,int sizeX,int sizeY,int pointOrder);	//���ڵ�һ�ַ��÷��������·���֮��ķ�����Ϣ
	void updateNewXY(XBasicBox &basicBox,int x,int y);						//���ڵڶ��ַ��÷�ʽ�������µĿ��Է��õĵ�

	XBool m_canRotate;			//�Ƿ�֧����ת��������תֻ֧��0����90�ȵ���ת����֧������Ƕȵ���ת
public:
	int m_basicBoxSum;			//���ӵ�����
    XBasicBox * m_basicBox;	//���ӵ�ָ��
	int m_objectBoxSum;			//��Ҫ�ŵ����������
    XObjectBox * m_objectBox;	//��Ҫ�ŵ������ָ��
	//float m_wellRate;			//�û��ĳ̶ȣ����ڱ�ʶ��ĺû��̶�

	//����Ϊ����Դ���ض������һЩ����
	int init(int boxSum,int objectSum);

    int findResult(int orderMode = 0);			//�����Ƿ��ҵ��˺��ʵĽ�,orderMode����˳��Ĭ��ʹ���������
	int getNeedBoxSum() const;		//����ʹ�õ��˶��ٸ�����

	void setOptimizeLevel(bool flag)	//0:�����Ż����ٶȽϿ죬1:�߼��Ż����ٶȽ���
	{
		m_isAdvancedOptimize = flag;
	}
	bool getOptimizeLevel() const {return m_isAdvancedOptimize;}
	//���÷��õ�ʱ���Ƿ����������ת90��
	void setCanRotate(XBool temp) {m_canRotate = temp;}
	XBool getCanRotate() const {return m_canRotate;}
	void reset();	//����������Ϣ���Ա��ڿ������°ڷ�
	float getUseRate()
	{//����������
		float all = 0.0f;
		float noUsed = 0.0f;
		for(int i = 0;i < m_basicBoxSum;++ i)
		{
			if(m_basicBox[i].m_beUsed == 0) break;
			all += m_basicBox[i].m_size.x * m_basicBox[i].m_size.y;
			noUsed += m_basicBox[i].m_releaseAcreage;
		}
		if(all == 0.0f) return 0.0f;
		return 1.0f - noUsed / all;
	}
	float getAllSpace()
	{//��ȡ�ܿռ�
		float all = 0.0f;
		for(int i = 0;i < m_basicBoxSum;++ i)
		{
			if(m_basicBox[i].m_beUsed == 0) break;
			all += m_basicBox[i].m_size.x * m_basicBox[i].m_size.y;
		}
		return all;
	}

	X2DPacker()
		:m_isAdvancedOptimize(false)
		,m_basicBoxSum(0)
		,m_basicBox(NULL)
		,m_objectBoxSum(0)
		,m_objectBox(NULL)
		//,m_wellRate(1.0f)
		,m_canRotate(XFalse)
	{}
};
}
#endif

