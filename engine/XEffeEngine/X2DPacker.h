#ifndef _JIA_X2DPACKER_
#define _JIA_X2DPACKER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

//#include "XEffeEngine.h"
#include "XBasicClass.h"

//2D,����תװ������
//1��ʹ�õ�һ����װ�뼰װ����㷨

/*�������Ͳο���ͼ
|- 0 1-|
|_ 3 2_|
*/
//struct _XCorner
//{
//	_XBool m_isEnable;			//��������Ƿ���Ч
//	_XVector2 m_position;	//�����λ��;
//	int m_type;				//���������
//};

//��Ҫ���õ�����
class _XObjectBox
{
public:
	int m_setOrder;			//�ⲿ��ţ����û����õģ����ֵ�ڲ�����ı�
    int m_order;			//����ı�ţ�������ڲ����
    int m_basicBoxOrder;    //��������ķ���ı�� -1��ʾ��û�з���
    _XVector2 m_position;	//�����λ��

    _XVector2 m_size;		//����ĳߴ�
    float m_acreage;        //��������

	_XRect m_AABB;			//����ĺ��ӣ������ж��Ƿ����
	_XBool m_needRotate;		//���屻����֮���Ƿ���Ҫ������ת
};

//���ڷ�������Ļ������
class _XBasicBox
{
public:
    int m_order;				//���ӵı��
    int m_beUsed;				//�����Ƿ�ʹ��:0��ʾ��װ��������n��ʾװ����n������
	_XVector2 m_size;			//���ӵĳߴ�
    float m_releaseAcreage;		//���ӵ�ʣ��ռ�

	int *m_objectBoxesOrder;	//�Ѿ��������ӵĺ��ӵı���б�
	//���ǵ����ַ��÷�ʽ(û��ʵ��)
//    int *m_xEdge;				//��Ч��������ˮƽ��
//    int *m_yEdge;				//��Ч���Կ��Ĵ�ֱ��
	//���ǵ�һ�ַ��÷�ʽ
	int m_mayPositionSum;		//��Ч����������
	_XVector2 *m_mayPosition;			//���Է���ĵ�
	//���ǵڶ��ַ��÷�ʽ
	//�Ż��������п��ܵĹս�
	int m_mayPositionXSum;		//�п��ܵ�X������
	int *m_mayPositionX;		//�п��ܵ�X
	int m_mayPositionYSum;		//�п��ܵ�Y������
	int *m_mayPositionY;		//�п��ܵ�Y

    int putInRatio(const _XObjectBox &temp);    //���ӷ���ĺ��ʶ�
};

//2D����תװ�����
class _X2DPacker
{
private:
	int putInABox(_XObjectBox &objectBox);	//��һ������װ�����ӣ������Ƿ�װ��ɹ�
	int canPutIn(_XObjectBox &objectBox,_XBasicBox &basicBox);	//�Ƿ��ܷŵ����������
	int m_optimizeLevel;	//ʹ�����ַ�ʽ��ʽ0��Ϊ��һ�ַ��÷�ʽ 1��Ϊ�ڶ��ַ��÷�ʽ

	void orderObjectByWidth();	//ͨ����ȶ������������
	void orderObjectByArea();	//ͨ������������������
	void orderObjectByBigEdgeAndArea();	//ͨ������������������
	void updatePutOperate(_XObjectBox &objectBox,_XBasicBox &basicBox,_XBool needRotate,float positionX,float positionY);	//��ɷ�������������Ӧ������
	void updateNewPoint(_XBasicBox &basicBox,int sizeX,int sizeY,int pointOrder);	//���ڵ�һ�ַ��÷��������·���֮��ķ�����Ϣ
	void updateNewXY(_XBasicBox &basicBox,int x,int y);						//���ڵڶ��ַ��÷�ʽ�������µĿ��Է��õĵ�

	_XBool m_canRotate;			//�Ƿ�֧����ת��������תֻ֧��0����90�ȵ���ת����֧������Ƕȵ���ת
public:
	int m_basicBoxSum;			//���ӵ�����
    _XBasicBox * m_basicBox;	//���ӵ�ָ��
	int m_objectBoxSum;			//��Ҫ�ŵ����������
    _XObjectBox * m_objectBox;	//��Ҫ�ŵ������ָ��
	//float m_wellRate;			//�û��ĳ̶ȣ����ڱ�ʶ��ĺû��̶�

	//����Ϊ����Դ���ض������һЩ����
	int init(int boxSum,int objectSum);

    int findResult(int orderMode = 0);			//�����Ƿ��ҵ��˺��ʵĽ�,orderMode����˳��Ĭ��ʹ���������
	int getNeedBoxSum() const;		//����ʹ�õ��˶��ٸ�����

	void setOptimizeLevel(int temp)	//0:�����Ż����ٶȽϿ죬1:�߼��Ż����ٶȽ���
	{
		if(temp == 0) m_optimizeLevel = 0;
		else m_optimizeLevel = 1;
	}
	int getOptimizeLevel() const {return m_optimizeLevel;}
	//���÷��õ�ʱ���Ƿ����������ת90��
	void setCanRotate(_XBool temp) {m_canRotate = temp;}
	_XBool getCanRotate() const {return m_canRotate;}
	void reset();	//����������Ϣ���Ա��ڿ������°ڷ�

	_X2DPacker()
		:m_optimizeLevel(0)
		,m_basicBoxSum(0)
		,m_basicBox(NULL)
		,m_objectBoxSum(0)
		,m_objectBox(NULL)
		//,m_wellRate(1.0f)
		,m_canRotate(XFalse)
	{}
};

#endif

