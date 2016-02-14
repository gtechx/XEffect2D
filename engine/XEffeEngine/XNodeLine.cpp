#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XNodeLine.h"
#include "XResourcePack.h"
#include "XObjectManager.h"
namespace XE{
XNodeLine::XNodeLine()
	:m_node(NULL)
	,m_isACopy(XFalse)
	,m_myOrder(0)
	,m_leadMode(LEAD_MODE_FOREWORD)	//���ߵ�����ģʽ
	,m_lineLength(0)					//���ߵ��ܳ���
	//,m_nodeSum(0)						//���������е������ڵ������
	,m_offsetPosition(0,0)
	,m_scale(1.0f,1.0f)
	,m_isLoop(XFalse)
	,m_isVisible(XTrue)
{
	m_node = XMem::createArrayMem<XNode>(m_maxNodeSum);
	if(m_node == NULL) printf("Memory error!\n");
	m_bezierPoint = XMem::createArrayMem<XVector2>(m_maxNodeSum * 2);
	if(m_bezierPoint == NULL) printf("Memory error!\n");
	try
	{
		m_cp = new XSCounter;
		if(m_cp == NULL) printf("Memory error!\n");
	}catch(...)
	{
		printf("Memory error!\n");
	}

	m_specialPoint.isEnable = XFalse;
	clearUp();
	m_objType = OBJ_NODELINE;
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
}
XNodeLine::~XNodeLine()
{
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XMem::XDELETE_ARRAY(m_node);
		XMem::XDELETE_ARRAY(m_bezierPoint);
		XMem::XDELETE(m_cp);
	}
	release();
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
}
void XNodeLine::addOneNode(float x,float y)		//������ͷ�����һ����
{
	if(m_isACopy) return;//�������ܽ����������
	int prevNodeNumber;
	if(m_nodeSum >= m_maxNodeSum || m_node == NULL) return;	//�����¼�ĵ��Ѿ�������ֱ�ӷ���
	m_node[m_nodeSum].myPoint.set(x,y);		//��¼��ǰ�ĵ����Ϣ
	m_node[m_nodeSum].toNextAngle = 0;
	m_node[m_nodeSum].toNextCos = 0;
	m_node[m_nodeSum].toNextSin = 0;
	m_node[m_nodeSum].toNextLength = 0;
	if(m_nodeSum > 0)
	{//��д��һ��������������Ϣ
		prevNodeNumber = m_nodeSum - 1;
		m_node[prevNodeNumber].toNextAngle = m_node[prevNodeNumber].myPoint.getAngle(m_node[m_nodeSum].myPoint);
		m_node[prevNodeNumber].toNextCos = cos(m_node[prevNodeNumber].toNextAngle);
		m_node[prevNodeNumber].toNextSin = sin(m_node[prevNodeNumber].toNextAngle);
		m_node[prevNodeNumber].toNextLength = m_node[prevNodeNumber].myPoint.getLength(m_node[m_nodeSum].myPoint);

		//��д��ǰ��������Ϣ
		m_node[m_nodeSum].toPreviousAngle = m_node[prevNodeNumber].toNextAngle + PI;
		if(m_node[m_nodeSum].toPreviousAngle >= PI2) m_node[m_nodeSum].toPreviousAngle -= PI2;
		m_node[m_nodeSum].toPreviousCos = cos(m_node[m_nodeSum].toPreviousAngle);
		m_node[m_nodeSum].toPreviousSin = sin(m_node[m_nodeSum].toPreviousAngle);
		m_node[m_nodeSum].toPreviousLength = m_node[prevNodeNumber].toNextLength;
		m_lineLength += m_node[m_nodeSum].toPreviousLength;
		
		if(!m_isLoop)
		{
			//���һ����ķ�����ǰһ����ķ��򱣳�һ��
			m_node[m_nodeSum].toNextAngle = m_node[prevNodeNumber].toNextAngle;
			m_node[m_nodeSum].toNextCos = m_node[prevNodeNumber].toNextCos;
			m_node[m_nodeSum].toNextSin = m_node[prevNodeNumber].toNextSin;
			if(m_nodeSum == 1)
			{//ͷ�ĵ㱣�ֺ�һ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
		}
	}
	++ m_nodeSum;
	if(m_isLoop) setLoop();	//�������߱պ�
	m_needUpdateData = XTrue;
}
void XNodeLine::setOneNode(float x,float y,int nodeOrder)
{
	if(m_isACopy ||
		m_node == NULL) return;
	if(nodeOrder == 0)
	{//ͷ
		m_lineLength -= m_node[0].toNextLength;
		m_node[0].myPoint.set(x,y);
		m_node[0].toNextAngle = m_node[0].myPoint.getAngle(m_node[1].myPoint);
		m_node[0].toNextCos = cos(m_node[0].toNextAngle);
		m_node[0].toNextSin = sin(m_node[0].toNextAngle);
		m_node[0].toNextLength = m_node[0].myPoint.getLength(m_node[1].myPoint);

		m_lineLength += m_node[0].toNextLength;
		m_node[1].toPreviousAngle = m_node[0].toNextAngle + PI;
		if(m_node[1].toPreviousAngle >= PI2) m_node[1].toPreviousAngle -= PI2;
		m_node[1].toPreviousCos = cos(m_node[1].toPreviousAngle);
		m_node[1].toPreviousSin = sin(m_node[1].toPreviousAngle);
		m_node[1].toPreviousLength = m_node[0].toNextLength;
		if(!m_isLoop)
		{
			if(m_nodeSum > 1)
			{//ͷ�ĵ㱣�ֺ�һ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(m_nodeSum == 2)
			{
				m_node[1].toNextAngle = m_node[0].toNextAngle;
				m_node[1].toNextCos = m_node[0].toNextCos;
				m_node[1].toNextSin = m_node[0].toNextSin;
			}
		}
		if(m_isLoop) setLoop();
	}else
	if(nodeOrder == m_nodeSum - 1)
	{//β
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_node[nodeOrder].myPoint.set(x,y);		//��¼��ǰ�ĵ����Ϣ
		m_node[nodeOrder].toNextAngle = 0;
		m_node[nodeOrder].toNextCos = 0;
		m_node[nodeOrder].toNextSin = 0;
		m_node[nodeOrder].toNextLength = 0;
		if(m_nodeSum > 1)
		{//��д��һ��������������Ϣ
			m_node[nodeOrder - 1].toNextAngle = m_node[nodeOrder - 1].myPoint.getAngle(m_node[nodeOrder].myPoint);
			m_node[nodeOrder - 1].toNextCos = cos(m_node[nodeOrder - 1].toNextAngle);
			m_node[nodeOrder - 1].toNextSin = sin(m_node[nodeOrder - 1].toNextAngle);
			m_node[nodeOrder - 1].toNextLength = m_node[nodeOrder - 1].myPoint.getLength(m_node[nodeOrder].myPoint);

			//��д��ǰ��������Ϣ
			m_node[nodeOrder].toPreviousAngle = m_node[nodeOrder - 1].toNextAngle + PI;
			if(m_node[nodeOrder].toPreviousAngle >= PI2) m_node[nodeOrder].toPreviousAngle -= PI2;
			m_node[nodeOrder].toPreviousCos = cos(m_node[nodeOrder].toPreviousAngle);
			m_node[nodeOrder].toPreviousSin = sin(m_node[nodeOrder].toPreviousAngle);
			m_node[nodeOrder].toPreviousLength = m_node[nodeOrder - 1].toNextLength;
			m_lineLength += m_node[nodeOrder].toPreviousLength;
			//���һ����ķ�����ǰһ����ķ��򱣳�һ��
			if(!m_isLoop)
			{
				m_node[nodeOrder].toNextAngle = m_node[nodeOrder - 1].toNextAngle;
				m_node[nodeOrder].toNextCos = m_node[nodeOrder - 1].toNextCos;
				m_node[nodeOrder].toNextSin = m_node[nodeOrder - 1].toNextSin;
				if(m_nodeSum == 2)
				{//ͷ�ĵ㱣�ֺ�һ����ķ���
					m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
					m_node[0].toPreviousCos = m_node[1].toPreviousCos;
					m_node[0].toPreviousSin = m_node[1].toPreviousSin;
				}
			}
		}
		if(m_isLoop) setLoop();	//�������߱պ�
	}else
	{//�м�
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_lineLength -= m_node[nodeOrder].toNextLength;

		m_node[nodeOrder].myPoint.set(x,y);
		m_node[nodeOrder].toNextAngle = m_node[nodeOrder].myPoint.getAngle(m_node[nodeOrder + 1].myPoint);
		m_node[nodeOrder].toNextCos = cos(m_node[nodeOrder].toNextAngle);
		m_node[nodeOrder].toNextSin = sin(m_node[nodeOrder].toNextAngle);
		m_node[nodeOrder].toNextLength = m_node[nodeOrder].myPoint.getLength(m_node[nodeOrder + 1].myPoint);

		m_lineLength += m_node[nodeOrder].toNextLength;

		m_node[nodeOrder + 1].toPreviousAngle = m_node[nodeOrder].toNextAngle + PI;
		if(m_node[nodeOrder + 1].toPreviousAngle >= PI2) m_node[nodeOrder + 1].toPreviousAngle -= PI2;
		m_node[nodeOrder + 1].toPreviousCos = cos(m_node[nodeOrder + 1].toPreviousAngle);
		m_node[nodeOrder + 1].toPreviousSin = sin(m_node[nodeOrder + 1].toPreviousAngle);
		m_node[nodeOrder + 1].toPreviousLength = m_node[nodeOrder].toNextLength;

		m_node[nodeOrder].toPreviousAngle = m_node[nodeOrder].myPoint.getAngle(m_node[nodeOrder - 1].myPoint);
		m_node[nodeOrder].toPreviousCos = cos(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousSin = sin(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousLength = m_node[nodeOrder].myPoint.getLength(m_node[nodeOrder - 1].myPoint);
		m_lineLength += m_node[nodeOrder].toPreviousLength;

		m_node[nodeOrder - 1].toNextAngle = m_node[nodeOrder].toPreviousAngle + PI;
		if(m_node[nodeOrder - 1].toNextAngle >= PI2) m_node[nodeOrder - 1].toNextAngle -= PI2;
		m_node[nodeOrder - 1].toNextCos = cos(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextSin = sin(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextLength = m_node[nodeOrder - 1].myPoint.getLength(m_node[nodeOrder].myPoint);
		if(!m_isLoop)
		{
			if(nodeOrder == 1)
			{//ͷ�ĵ㱣�ֺ�һ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(nodeOrder == m_nodeSum - 2 && !m_isLoop)
			{//���һ����Ҫ����ǰһ����ķ���
				m_node[nodeOrder + 1].toNextAngle = m_node[nodeOrder].toNextAngle;
				m_node[nodeOrder + 1].toNextCos = m_node[nodeOrder].toNextCos;
				m_node[nodeOrder + 1].toNextSin = m_node[nodeOrder].toNextSin;
			}
		}
	}
	m_needUpdateData = XTrue;	//�²���㣬��Ҫ���¼����ֵ��
}
void XNodeLine::decreaseOneNode()
{
	if(m_isACopy ||	//�������ܽ����������
		m_nodeSum <= 0 || m_node == NULL) return;
	m_lineLength -= m_node[m_nodeSum - 1].toPreviousLength;
	m_node[m_nodeSum - 1].myPoint.set(0,0);
//	m_node[m_nodeSum - 1].toNextAngle = 0;
//	m_node[m_nodeSum - 1].toNextCos = 0;
//	m_node[m_nodeSum - 1].toNextSin = 0;
	m_node[m_nodeSum - 1].toNextLength = 0;
	m_node[m_nodeSum - 1].toPreviousAngle = 0;
	m_node[m_nodeSum - 1].toPreviousCos = 0;
	m_node[m_nodeSum - 1].toPreviousSin = 0;
	m_node[m_nodeSum - 1].toPreviousLength = 0;
	-- m_nodeSum;
	if(m_isLoop) setLoop();	//�������߱պ�
	m_needUpdateData = XTrue;
}

void XNodeLine::addOneNode(float x,float y,int nodeOrder)		//�������м����һ����
{
	if(m_isACopy ||//�������ܽ����������
		m_nodeSum >= m_maxNodeSum || m_node == NULL) return;	//�����¼�ĵ��Ѿ�������ֱ�ӷ���
	if(nodeOrder == 0)
	{//ͷ
		for(int i = m_nodeSum;i > 0;-- i)
		{
			m_node[i] = m_node[i - 1];
		}
		m_node[0].myPoint.set(x,y);
		m_node[0].toNextAngle = m_node[0].myPoint.getAngle(m_node[1].myPoint);
		m_node[0].toNextCos = cos(m_node[0].toNextAngle);
		m_node[0].toNextSin = sin(m_node[0].toNextAngle);
		m_node[0].toNextLength = m_node[0].myPoint.getLength(m_node[1].myPoint);
		++ m_nodeSum;
		//m_lineLength += m_node[1].toPreviousLength;	//����Ӧ�������߼������
		m_lineLength += m_node[0].toNextLength;
		m_node[1].toPreviousAngle = m_node[0].toNextAngle + PI;
		if(m_node[1].toPreviousAngle >= PI2) m_node[1].toPreviousAngle -= PI2;
		m_node[1].toPreviousCos = cos(m_node[1].toPreviousAngle);
		m_node[1].toPreviousSin = sin(m_node[1].toPreviousAngle);
		m_node[1].toPreviousLength = m_node[0].toNextLength;
		if(!m_isLoop)
		{
			if(m_nodeSum > 1)
			{//ͷ����㱣��ǰһ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(m_nodeSum == 2)
			{
				m_node[1].toNextAngle = m_node[0].toNextAngle;
				m_node[1].toNextCos = m_node[0].toNextCos;
				m_node[1].toNextSin = m_node[0].toNextSin;
			}
		}else
		{
			setLoop();
		}
	}else
	if(nodeOrder == m_nodeSum)
	{//β
		addOneNode(x,y);
	}else
	{//�м�
		for(int i = m_nodeSum;i > nodeOrder;-- i)
		{
			m_node[i] = m_node[i - 1];
		}
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_node[nodeOrder].myPoint.set(x,y);
		m_node[nodeOrder].toNextAngle = m_node[nodeOrder].myPoint.getAngle(m_node[nodeOrder + 1].myPoint);
		m_node[nodeOrder].toNextCos = cos(m_node[nodeOrder].toNextAngle);
		m_node[nodeOrder].toNextSin = sin(m_node[nodeOrder].toNextAngle);
		m_node[nodeOrder].toNextLength = m_node[nodeOrder].myPoint.getLength(m_node[nodeOrder + 1].myPoint);
		m_lineLength += m_node[nodeOrder].toNextLength;

		m_node[nodeOrder + 1].toPreviousAngle = m_node[nodeOrder].toNextAngle + PI;
		if(m_node[nodeOrder + 1].toPreviousAngle >= PI2) m_node[nodeOrder + 1].toPreviousAngle -= PI2;
		m_node[nodeOrder + 1].toPreviousCos = cos(m_node[nodeOrder + 1].toPreviousAngle);
		m_node[nodeOrder + 1].toPreviousSin = sin(m_node[nodeOrder + 1].toPreviousAngle);
		m_node[nodeOrder + 1].toPreviousLength = m_node[nodeOrder].toNextLength;

		m_node[nodeOrder].toPreviousAngle = m_node[nodeOrder].myPoint.getAngle(m_node[nodeOrder - 1].myPoint);
		m_node[nodeOrder].toPreviousCos = cos(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousSin = sin(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousLength = m_node[nodeOrder].myPoint.getLength(m_node[nodeOrder - 1].myPoint);
		m_lineLength += m_node[nodeOrder].toPreviousLength;

		m_node[nodeOrder - 1].toNextAngle = m_node[nodeOrder].toPreviousAngle + PI;
		if(m_node[nodeOrder - 1].toNextAngle >= PI2) m_node[nodeOrder - 1].toNextAngle -= PI2;
		m_node[nodeOrder - 1].toNextCos = cos(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextSin = sin(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextLength = m_node[nodeOrder - 1].myPoint.getLength(m_node[nodeOrder].myPoint);
		++ m_nodeSum;
		if(!m_isLoop)
		{
			if(nodeOrder == 1)
			{//ͷ����㱣��ǰһ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(nodeOrder == m_nodeSum - 2)
			{//���һ������ǰһ���㱣��һ��
				m_node[m_nodeSum - 1].toNextAngle = m_node[nodeOrder].toNextAngle;
				m_node[m_nodeSum - 1].toNextCos = m_node[nodeOrder].toNextCos;
				m_node[m_nodeSum - 1].toNextSin = m_node[nodeOrder].toNextSin;
			}
		}else
		{
			setLoop();	//�������߱պ�
		}
	}
	m_needUpdateData = XTrue;
}

void XNodeLine::decreaseOneNode(int nodeOrder)
{
	//������ݵĺ�����
	if(m_isACopy ||//�������ܽ����������
		nodeOrder <= 0 || nodeOrder >= m_nodeSum) return;
	if(nodeOrder == 0)
	{//ͷ
		for(int i = 0;i < m_nodeSum - 1;++ i)
		{
			m_node[i] = m_node[i + 1];
		}
		m_lineLength -= m_node[0].toPreviousLength;
	//	m_node[0].toPreviousAngle = 0;
	//	m_node[0].toPreviousCos = 0;
	//	m_node[0].toPreviousSin = 0;
		m_node[0].toPreviousLength = 0;
		//���һ����Ϊ���к�decreaseOneNode�����еļ����������ӵ�
		m_lineLength += m_node[m_nodeSum - 1].toPreviousLength;
		decreaseOneNode();
	}else
	if(nodeOrder == m_nodeSum - 1)
	{//β
		decreaseOneNode();
	}else
	{//�м�
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_lineLength -= m_node[nodeOrder + 1].toPreviousLength;
		for(int i = nodeOrder;i < m_nodeSum - 1;++ i)
		{
			m_node[i] = m_node[i + 1];
		}
		m_node[nodeOrder - 1].toNextAngle = m_node[nodeOrder - 1].myPoint.getAngle(m_node[nodeOrder].myPoint);
		m_node[nodeOrder - 1].toNextCos = cos(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextSin = sin(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextLength = m_node[nodeOrder - 1].myPoint.getLength(m_node[nodeOrder].myPoint);
		m_lineLength += m_node[nodeOrder - 1].toNextLength;
		m_node[nodeOrder].toPreviousAngle = m_node[nodeOrder - 1].toNextAngle + PI;
		if(m_node[nodeOrder].toPreviousAngle >= PI2) m_node[nodeOrder].toPreviousAngle -= PI2;
		m_node[nodeOrder].toPreviousCos = cos(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousSin = sin(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousLength = m_node[nodeOrder - 1].toNextLength;
		//���һ����Ϊ���к�decreaseOneNode�����еļ����������ӵ�
		m_lineLength += m_node[m_nodeSum - 1].toPreviousLength;
		decreaseOneNode();
	}
	m_needUpdateData = XTrue;
}
void XNodeLine::clearUp()
{//ֻ��Ҫ�����һ�������Ϣ����
	if(m_isACopy) return;//�������ܽ����������
	m_node[0].myPoint.set(0,0);
	m_node[0].toNextAngle = 0;
	m_node[0].toNextCos = 0;
	m_node[0].toNextSin = 0;
	m_node[0].toNextLength = 0;
	m_node[0].toPreviousAngle = 0;
	m_node[0].toPreviousCos = 0;
	m_node[0].toPreviousSin = 0;
	m_node[0].toPreviousLength = 0;
	m_nodeSum = 0;
	m_lineLength = 0;
	m_isLoop = XFalse;
}
XBool XNodeLine::saveNodeLine()
{
	if(m_isACopy ||//�������ܽ����������
		m_nodeSum <= 1) return XFalse;
	FILE *fp;
	char fileName[] = NODELINE_DATA_FILE_NAME;
	if(m_myOrder >= 0) fileName[13] = m_myOrder % 10 + '0';
	if(m_myOrder >= 10) fileName[12] = (m_myOrder/10) % 10 + '0';
	if(m_myOrder >= 100) fileName[11] = (m_myOrder/100) % 10 + '0';
	if(m_myOrder >= 1000) fileName[10] = (m_myOrder/1000) % 10 + '0';
	if(m_myOrder >= 10000) fileName[9] = (m_myOrder/10000) % 10 + '0';
	//���ļ�
	if((fp = fopen(fileName,"wb")) == NULL)
	{
		printf("Line file open error!\n");
		return XFalse;
	}
	//д������
	fwrite(&m_nodeSum,sizeof(int),1,fp);			//�ڵ�����
	fwrite(&m_lineLength,sizeof(float),1,fp);		//�����߳���
	//����д��ڵ���Ϣ
	for(int i = 0;i < m_nodeSum;++ i)
	{
		fwrite(&m_node[i],sizeof(XNode),1,fp);	
	}
	//д�����֮��ر��ļ�
	fclose(fp);
	return XTrue;
}
bool XNodeLine::loadFromFolder(const char *filename)	//���ļ�����������Դ
{
	FILE *fp = NULL;
	if((fp = fopen(filename,"rb")) == NULL)
	{//���ļ�
		printf("Line file open error!\n");
		return XFalse;
	}
	//��ȡ����
	fread(&m_nodeSum,sizeof(int),1,fp);		//�ڵ�����
	fread(&m_lineLength,sizeof(float),1,fp);		//�����߳���
	//���ζ�ȡ�ڵ���Ϣ
	for(int i = 0;i < m_nodeSum;++ i)
	{
		fread(&m_node[i],sizeof(XNode),1,fp);	
	}
	//��ȡ���֮��ر��ļ�
	fclose(fp);
	return true;
}
bool XNodeLine::loadFromPacker(const char *filename)	//��ѹ������������Դ
{
	//����Դ�ļ�����ȡ��Դ
	unsigned char *p = XResPack.getFileData(filename);
	if(p == NULL)
	{
		printf("Load NodeLine Data Error!\n");
		return XFalse;
	}
	//����ȡ�������ͷŵ�ָ����λ��
	int offset = 0;
	memcpy(&m_nodeSum,p + offset,sizeof(int));
	offset += sizeof(int);
	memcpy(&m_lineLength,p + offset,sizeof(float));
	offset += sizeof(float);
	//����д��ڵ���Ϣ
	for(int i = 0;i < m_nodeSum;++ i)
	{
		memcpy(&m_node[i],p + offset,sizeof(XNode));
		offset += sizeof(XNode);
	}
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XNodeLine::loadFromWeb(const char *filename)		//����ҳ�ж�ȡ��Դ
{
	return false;
}

XBool XNodeLine::getNodeLine(XResourcePosition resoursePosition)
{
	if(m_isACopy) return XFalse;//�������ܽ����������
	char fileName[MAX_FILE_NAME_LENGTH] = "";
	sprintf(fileName,"%s%s",BASE_RESOURCE_PATH,NODELINE_DATA_FILE_NAME);
	int len = strlen(fileName);
	if(m_myOrder >= 0) fileName[len - 5] = m_myOrder % 10 + '0';
	if(m_myOrder >= 10) fileName[len - 6] = (m_myOrder/10) % 10 + '0';
	if(m_myOrder >= 100) fileName[len - 7] = (m_myOrder/100) % 10 + '0';
	if(m_myOrder >= 1000) fileName[len - 8] = (m_myOrder/1000) % 10 + '0';
	if(m_myOrder >= 10000) fileName[len - 9] = (m_myOrder/10000) % 10 + '0';
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = getDefResPos();
	switch(resoursePosition)
	{
	case RESOURCE_LOCAL_PACK:
		if(!loadFromPacker(fileName)) return false;
		break;
	case RESOURCE_LOCAL_FOLDER:
		if(!loadFromFolder(fileName)) return false;
		break;
	case RESOURCE_WEB:
		if(!loadFromWeb(fileName)) return false;
		break;
	case RESOURCE_AUTO:
		if(!loadFromPacker(fileName) && !loadFromFolder(fileName) &&
			!loadFromWeb(fileName)) return false;
		break;
	}
	m_needUpdateData = XTrue;
	return XTrue;
}
void XNodeLine::updateData()
{
	if(!m_needUpdateData) return;
	m_needUpdateData = XFalse;	//�����������
	if(m_nodeSum < 2) return;
	//���濪ʼ�����ֵ�ؼ���(ÿ����������������)
	XVector2 tempPoint0;
	XVector2 tempPoint1;
	XVector2 tempPoint2;
	//��һ����Ҫ�ֿ�����
	tempPoint0 = (m_node[0].myPoint + m_node[m_nodeSum - 1].myPoint) * 0.5f;	//��ǰһ������е�
	tempPoint1 = (m_node[0].myPoint + m_node[1].myPoint) * 0.5f;	//���һ������е�
	tempPoint2 = (tempPoint0 + tempPoint1) * 0.5f;	//���е���е�
	m_bezierPoint[0] = tempPoint0 + m_node[0].myPoint - tempPoint2;	//���е����ߵ�ƽ��
	m_bezierPoint[1] = tempPoint1 + m_node[0].myPoint - tempPoint2;
	//���һ����Ҫ�ֿ�����
	//tempPoint0 = (m_node[0].myPoint + m_node[m_nodeSum - 1].myPoint) * 0.5f;
	tempPoint1 = (m_node[m_nodeSum - 1].myPoint + m_node[m_nodeSum - 2].myPoint) * 0.5f;
	tempPoint2 = (tempPoint0 + tempPoint1) * 0.5f;
	m_bezierPoint[(m_nodeSum - 1) << 1] = tempPoint1 + m_node[m_nodeSum - 1].myPoint - tempPoint2;
	m_bezierPoint[((m_nodeSum - 1) << 1) + 1] = tempPoint0 + m_node[m_nodeSum - 1].myPoint - tempPoint2;
	for(int i = 1;i < m_nodeSum - 1;++ i)
	{
		tempPoint0 = (m_node[i].myPoint + m_node[i - 1].myPoint) * 0.5f;
		tempPoint1 = (m_node[i].myPoint + m_node[i + 1].myPoint) * 0.5f;
		tempPoint2 = (tempPoint0 + tempPoint1) * 0.5f;
		m_bezierPoint[i << 1] = tempPoint0 + m_node[i].myPoint - tempPoint2;
		m_bezierPoint[(i << 1) + 1] = tempPoint1 + m_node[i].myPoint - tempPoint2;
	}
}
//ע��������Ҫ��2����
void XNodeLine::moveSpecialPoint(int timeDelay,int isLoop,XBool needBezier)
{
	if(m_nodeSum <= 1) return;
	updateData();	//����б�Ҫ���²�ֵ����
	if(isLoop != 0 && !m_isLoop) setLoop();
	if(m_specialPoint.isEnable && !m_specialPoint.isEnd)
	{
		m_specialPoint.curLength += timeDelay * m_specialPoint.speed;
		if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
		{//˳��
			while(true)
			{
				if(m_specialPoint.curLength > m_specialPoint.upNodeLength + m_node[m_specialPoint.upNodeOrder].toNextLength)
				{//�Ѿ�Խ������һ����
					if(isLoop == 0)
					{//��ѭ��
						if(m_specialPoint.upNodeOrder + 1 == m_nodeSum - 1)
						{//�Ѿ������յ�
							m_specialPoint.isEnd = XTrue;
							m_specialPoint.angle = m_node[m_nodeSum - 2].toNextAngle;
							m_specialPoint.curLength = m_lineLength;
							m_specialPoint.position = m_node[m_nodeSum - 1].myPoint;
							m_specialPoint.upNodeOrder = m_nodeSum - 2;
							m_specialPoint.upNodeLength = m_lineLength - m_node[m_nodeSum - 1].toPreviousLength;
							break;
						}else
						{
							++ m_specialPoint.upNodeOrder;
							m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toPreviousLength;
							if(needBezier)
							{//��������Ҫ���ӵĲ���
								m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
									m_bezierPoint[(m_specialPoint.upNodeOrder << 1) + 1],
									m_bezierPoint[(m_specialPoint.upNodeOrder + 1) << 1],
									m_node[m_specialPoint.upNodeOrder + 1].myPoint);
							}
						}
					}else
					{//ѭ��
						if(m_specialPoint.upNodeOrder == m_nodeSum - 1)
						{//���µ������
							m_specialPoint.isEnd = XTrue;
							m_specialPoint.angle = m_node[0].toNextAngle;
							m_specialPoint.curLength = m_lineLength + m_node[m_nodeSum - 1].toNextLength;
							m_specialPoint.position = m_node[0].myPoint;
							m_specialPoint.upNodeOrder = m_nodeSum - 1;
							m_specialPoint.upNodeLength = m_lineLength;
							break;
						}else
						{
							++ m_specialPoint.upNodeOrder;
							m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toPreviousLength;
							if(needBezier)
							{//��������Ҫ���ӵĲ���
								if(m_specialPoint.upNodeOrder == m_nodeSum - 1)
								{//���һ����
									m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
										m_bezierPoint[(m_specialPoint.upNodeOrder << 1) + 1],
										m_bezierPoint[0],
										m_node[0].myPoint);
								}else
								{
									m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
										m_bezierPoint[(m_specialPoint.upNodeOrder << 1) + 1],
										m_bezierPoint[(m_specialPoint.upNodeOrder + 1) << 1],
										m_node[m_specialPoint.upNodeOrder + 1].myPoint);
								}
							}
						}
					}
				}else
				{//û��Խ������һ����
					if(m_specialPoint.upNodeOrder == m_nodeSum - 1)
					{//�������һ���㣬���������
						if(needBezier)
						{//��������Ҫ���ӵĲ���
							float tempPos = (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength;
							tempPos = m_bezierSpline.getT(tempPos);
							m_specialPoint.position = m_bezierSpline.getBezierSplineValue(tempPos);
							m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(tempPos);
						}else
						{
							m_specialPoint.position.x = m_node[m_specialPoint.upNodeOrder].myPoint.x 
								+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toNextCos;
							m_specialPoint.position.y = m_node[m_specialPoint.upNodeOrder].myPoint.y 
								+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toNextSin;
							if(m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle <= PI
								&& m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle >= -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle);
							}else
							if(m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle > PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (PI2 - (m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle));
							}else
							if(m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle < -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (PI2 + (m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle));
							}
						}
					}else
					{//�����ĵ�
						if(needBezier)
						{//��������Ҫ���ӵĲ���
							float tempPos = (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength;
							tempPos = m_bezierSpline.getT(tempPos);
							m_specialPoint.position = m_bezierSpline.getBezierSplineValue(tempPos);
							m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(tempPos);
						}else
						{
							m_specialPoint.position.x = m_node[m_specialPoint.upNodeOrder].myPoint.x 
								+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toNextCos;
							m_specialPoint.position.y = m_node[m_specialPoint.upNodeOrder].myPoint.y 
								+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toNextSin;
							if(m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle <= PI
								&& m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle >= -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle);
							}else
							if(m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle > PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle - PI2);
							}else
							if(m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle < -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (PI2 + (m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle));
							}	
						}
					}
					break;
				}
			}
		}else
		{//����
			while(true)
			{
				if(m_specialPoint.curLength > m_specialPoint.upNodeLength + m_node[m_specialPoint.upNodeOrder].toPreviousLength)
				{//�Լ�Խ������һ����
					if(isLoop == 0)
					{//��ѭ��
						if(m_specialPoint.upNodeOrder == 1)
						{//�Ѿ������յ�
							m_specialPoint.isEnd = XTrue;
							m_specialPoint.angle = m_node[1].toPreviousAngle;
							m_specialPoint.curLength = m_lineLength;
							m_specialPoint.position = m_node[0].myPoint;
							m_specialPoint.upNodeOrder = 1;
							m_specialPoint.upNodeLength = m_lineLength - m_node[0].toNextLength;
							break;
						}else
						{
							-- m_specialPoint.upNodeOrder;
							m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toNextLength;
							if(needBezier)
							{//��������Ҫ���ӵĲ���
								m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
									m_bezierPoint[(m_specialPoint.upNodeOrder << 1)],
									m_bezierPoint[((m_specialPoint.upNodeOrder - 1) << 1) + 1],
									m_node[m_specialPoint.upNodeOrder - 1].myPoint);
							}
						}
					}else
					{
						if(m_specialPoint.upNodeOrder == 0)
						{//�Ѿ������յ�
							m_specialPoint.isEnd = XTrue;
							m_specialPoint.angle = m_node[m_nodeSum - 1].toPreviousAngle;
							m_specialPoint.curLength = m_lineLength + m_node[0].toPreviousLength;
							m_specialPoint.position = m_node[m_nodeSum - 1].myPoint;
							m_specialPoint.upNodeOrder = 0;
							m_specialPoint.upNodeLength = m_lineLength;
							break;
						}else
						{
							-- m_specialPoint.upNodeOrder;
							m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toNextLength;
							if(needBezier)
							{//��������Ҫ���ӵĲ���
								if(m_specialPoint.upNodeOrder == 0)
								{
									m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
										m_bezierPoint[(m_specialPoint.upNodeOrder << 1)],
										m_bezierPoint[((m_nodeSum - 1) << 1) + 1],
										m_node[m_nodeSum - 1].myPoint);
								}else
								{
									m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
										m_bezierPoint[(m_specialPoint.upNodeOrder << 1)],
										m_bezierPoint[((m_specialPoint.upNodeOrder - 1) << 1) + 1],
										m_node[m_specialPoint.upNodeOrder - 1].myPoint);
								}
							}
						}
					}
				}else
				{//û��Խ������һ����
					if(m_specialPoint.upNodeOrder == 0)
					{//�������е����
						if(needBezier)
						{//��������Ҫ���ӵĲ���
							float tempPos = (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength;
							tempPos = m_bezierSpline.getT(tempPos);
							m_specialPoint.position = m_bezierSpline.getBezierSplineValue(tempPos);
							m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(tempPos);
						}else
						{
							m_specialPoint.position.x = m_node[m_specialPoint.upNodeOrder].myPoint.x 
								+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toPreviousCos;
							m_specialPoint.position.y = m_node[m_specialPoint.upNodeOrder].myPoint.y 
								+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toPreviousSin;
							if(m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle <= PI
								&& m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle >= -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* (m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle);
							}else
							if(m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle > PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* ((m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle) - PI2);
							}else
							if(m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle < -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* (PI2 + (m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle));
							}
						}
					}else
					{//��ͨ�ĵ�
						if(needBezier)
						{//��������Ҫ���ӵĲ���
							float tempPos = (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength;
							tempPos = m_bezierSpline.getT(tempPos);
							m_specialPoint.position = m_bezierSpline.getBezierSplineValue(tempPos);
							m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(tempPos);
						}else
						{
							m_specialPoint.position.x = m_node[m_specialPoint.upNodeOrder].myPoint.x 
								+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toPreviousCos;
							m_specialPoint.position.y = m_node[m_specialPoint.upNodeOrder].myPoint.y 
								+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toPreviousSin;
							if(m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle <= PI
								&& m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle >=  -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* (m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle);
							}else
							if(m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle > PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* ((m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle) - PI2);
							}else
							if(m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle < -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* (PI2 + (m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle));
							}
						}
					}
					break;
				}
			}
		}
	}
}
void XNodeLine::resetSpecialPoint(XBool needBezier)
{
	if(m_nodeSum <= 1) return;
	updateData();	//����б�Ҫ���²�ֵ����
	if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
	{//˳��
		m_specialPoint.isEnable = XTrue;
		m_specialPoint.isEnd = XFalse;
		m_specialPoint.curLength = 0.0f;
		m_specialPoint.position.set(m_node[0].myPoint.x,m_node[0].myPoint.y);
		m_specialPoint.angle = m_node[0].toNextAngle;
		m_specialPoint.upNodeLength = 0.0f;
		m_specialPoint.upNodeOrder = 0;
		if(needBezier)
		{
			m_bezierSpline.init(m_node[0].myPoint,m_bezierPoint[1],	m_bezierPoint[2],m_node[1].myPoint);
		}
	}else
	{//����
		m_specialPoint.isEnable = XTrue;
		m_specialPoint.isEnd = XFalse;
		m_specialPoint.curLength = 0.0f;
		m_specialPoint.position.set(m_node[m_nodeSum - 1].myPoint.x,m_node[m_nodeSum - 1].myPoint.y);
		m_specialPoint.angle = m_node[m_nodeSum - 1].toPreviousAngle;
		m_specialPoint.upNodeLength = 0.0f;
		m_specialPoint.upNodeOrder = m_nodeSum - 1;
		if(needBezier)
		{
			m_bezierSpline.init(m_node[m_nodeSum - 1].myPoint,m_bezierPoint[(m_nodeSum - 1) << 1],m_bezierPoint[((m_nodeSum - 2) << 1) + 1],m_node[m_nodeSum - 2].myPoint);
		}
	}
}
void XNodeLine::setLoop()
{
	if(m_isACopy) return;//�������ܽ����������
	if(m_nodeSum < 2) return;	//С���������ǲ���ѭ����
	//����һ��������һ������������
	m_node[m_nodeSum - 1].toNextAngle = m_node[m_nodeSum - 1].myPoint.getAngle(m_node[0].myPoint);
	m_node[m_nodeSum - 1].toNextCos = cos(m_node[m_nodeSum - 1].toNextAngle);
	m_node[m_nodeSum - 1].toNextSin = sin(m_node[m_nodeSum - 1].toNextAngle);
	m_node[m_nodeSum - 1].toNextLength = m_node[m_nodeSum - 1].myPoint.getLength(m_node[0].myPoint);
	if(m_isLoop)
	{//���������ԭ�����Ǳպϵģ�������Ҫ��ȥԭ���ĳ���
		m_lineLength -= m_node[0].toPreviousLength;
	}
	m_node[0].toPreviousAngle = m_node[m_nodeSum - 1].toNextAngle + PI;
	if(m_node[0].toPreviousAngle >= PI2) m_node[0].toPreviousAngle -= PI2;
	m_node[0].toPreviousCos = cos(m_node[0].toPreviousAngle);
	m_node[0].toPreviousSin = sin(m_node[0].toPreviousAngle);
	m_node[0].toPreviousLength = m_node[m_nodeSum - 1].toNextLength;

	m_lineLength += m_node[0].toPreviousLength; //�����µĳ���
	m_isLoop = XTrue;
}
XBool XNodeLine::setACopy(const XNodeLine &temp)
{
	if(this == &temp) return XFalse;		//��ֹ��������
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XMem::XDELETE_ARRAY(m_node);
		XMem::XDELETE_ARRAY(m_bezierPoint);
		XMem::XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

//	if(m_isACopy == 0)
//	{//��������Ǹ�������Ҫ�ͷ���Դ
//		XMem::XDELETE_ARRAY(m_node);
//		m_isACopy = 1;
//	}
	//��������
	m_isVisible = temp.m_isVisible;					//���������ߵı��
	m_myOrder = temp.m_myOrder;					//���������ߵı��
	m_node = temp.m_node;						//�������еĽڵ�
	m_bezierPoint = temp.m_bezierPoint;	
	m_leadMode = temp.m_leadMode;				//���ߵ�����ģʽ
	m_isLoop = temp.m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���

	m_offsetPosition = temp.m_offsetPosition;	//�������ߵ�ƫ��λ��
	m_scale = temp.m_scale;						//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������
//	m_isACopy = temp.m_isACopy;					//�Ƿ���һ������

	m_lineLength = temp.m_lineLength;			//���ߵ��ܳ���
	m_nodeSum = temp.m_nodeSum;					//���������е������ڵ������
	m_specialPoint = temp.m_specialPoint;		//�����ϵ��ƶ��ĵ�
	return XTrue;
}
XNodeLine::XNodeLine(const XNodeLine &temp)
{
	//m_node = XMem::createArrayMem<XNode>(m_maxNodeSum);
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	m_cp = temp.m_cp;

	//��������
	m_isVisible = temp.m_isVisible;					//���������ߵı��
	m_myOrder = temp.m_myOrder;					//���������ߵı��
	m_leadMode = temp.m_leadMode;				//���ߵ�����ģʽ
	m_isLoop = temp.m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���

	m_offsetPosition = temp.m_offsetPosition;	//�������ߵ�ƫ��λ��
	m_scale = temp.m_scale;						//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������

	m_lineLength = temp.m_lineLength;			//���ߵ��ܳ���
	m_nodeSum = temp.m_nodeSum;					//���������е������ڵ������
	m_specialPoint = temp.m_specialPoint;		//�����ϵ��ƶ��ĵ�
	m_node = temp.m_node;						//�������еĽڵ�
	m_bezierPoint = temp.m_bezierPoint;						//�������еĽڵ�
	m_needUpdateData = temp.m_needUpdateData;						//�������еĽڵ�
	m_bezierSpline = temp.m_bezierSpline;						//�������еĽڵ�
//	for(int i = 0;i < m_nodeSum;++ i)
//	{
//		m_node[i] = temp.m_node[i];
//	}
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_isACopy = XFalse;
}
XNodeLine& XNodeLine::operator = (const XNodeLine& temp)
{
	if(&temp == this) return *this;
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XMem::XDELETE_ARRAY(m_node);
		XMem::XDELETE_ARRAY(m_bezierPoint);
		XMem::XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

//	if(m_node == NULL || m_isACopy != 0)
//	{//��Դ�Ѿ����ͷţ�������Դ�������Լ������������·����ڴ�ռ�
//		m_node = XMem::createArrayMem<XNode>(m_maxNodeSum);
//	}
	m_isVisible = temp.m_isVisible;					//���������ߵı��
	m_myOrder = temp.m_myOrder;					//���������ߵı��
	m_leadMode = temp.m_leadMode;				//���ߵ�����ģʽ
	m_isLoop = temp.m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���

	m_offsetPosition = temp.m_offsetPosition;	//�������ߵ�ƫ��λ��
	m_scale = temp.m_scale;						//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������

	m_lineLength = temp.m_lineLength;			//���ߵ��ܳ���
	m_nodeSum = temp.m_nodeSum;					//���������е������ڵ������
	m_specialPoint = temp.m_specialPoint;		//�����ϵ��ƶ��ĵ�
	m_node = temp.m_node;						//�������еĽڵ�
	m_bezierPoint = temp.m_bezierPoint;	
	m_needUpdateData = temp.m_needUpdateData;						//�������еĽڵ�
	m_bezierSpline = temp.m_bezierSpline;						//�������еĽڵ�
//	for(int i = 0;i < m_nodeSum;++ i)
//	{
//		m_node[i] = temp.m_node[i];
//	}
	m_isACopy = XFalse;

	return *this;
}
XBool XNodeLine::isInRect(float x,float y)
{
//	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
	XVector2 point(x,y);
	XVector2 pointS,pointE;
	if(m_nodeSum == 1)
	{//ֻ��һ����Ľڵ����ߣ��ж������ľ���
		if(point.getLength(XVector2(m_node[0].myPoint.x * m_scale.x + m_offsetPosition.x,
			m_node[0].myPoint.y * m_scale.y + m_offsetPosition.y)) <= 5) return XTrue;
		else return XFalse;
	}else
	if(m_nodeSum >= 1)
	{
		for(int i = 0;i < m_nodeSum - 1;++ i)
		{
			pointS.set(m_node[i].myPoint.x * m_scale.x + m_offsetPosition.x,
				m_node[i].myPoint.y * m_scale.y + m_offsetPosition.y);
			pointE.set(m_node[i + 1].myPoint.x * m_scale.x + m_offsetPosition.x,
				m_node[i + 1].myPoint.y * m_scale.y + m_offsetPosition.y);
			if(XMath::minDistancePointToLine(point,pointS,pointE) <= 5) return XTrue;
		}
	}
	return XFalse;
}
XVector2 XNodeLine::getBox(int order)
{//������Ҫ���ذ�Χ��
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
	if(m_nodeSum > 0)
	{//����ڵ��������нڵ㣬���������ڵ�İ�Χ��
		left = m_node[0].myPoint.x;
		right = m_node[0].myPoint.x;
		top = m_node[0].myPoint.y;
		bottom = m_node[0].myPoint.y;
		//���濪ʼ�������еĵ㣬Ѱ�Ұ�Χ����
		for(int i = 1;i < m_nodeSum;++ i)
		{
			if(m_node[i].myPoint.x > right) right = m_node[i].myPoint.x;
			if(m_node[i].myPoint.x < left) left = m_node[i].myPoint.x;
			if(m_node[i].myPoint.y > bottom) bottom = m_node[i].myPoint.y;
			if(m_node[i].myPoint.y < top) top = m_node[i].myPoint.y;
		}
	}
	//����԰�Χ���ӽ�����չ����ֹ������ߵ��ߵ�����
	left -= 5;
	right += 5;
	top -= 5;
	bottom += 5;
	switch(order)
	{
	case 0:return XVector2(left * m_scale.x + m_offsetPosition.x,	top * m_scale.y + m_offsetPosition.y);
	case 1:return XVector2(right * m_scale.x + m_offsetPosition.x,	top * m_scale.y + m_offsetPosition.y);
	case 2:return XVector2(right * m_scale.x + m_offsetPosition.x,	bottom * m_scale.y + m_offsetPosition.y);
	case 3:return XVector2(left * m_scale.x + m_offsetPosition.x,	bottom * m_scale.y + m_offsetPosition.y);
	}
	return XVector2::zero;
}
void XNodeLine::draw()
{
	for(int j = 0;j < m_nodeSum - 1;++ j)
	{
		//��������
		if(j == 0)
		{//��һ��������ɫ
			XRender::drawPoint(m_node[j].myPoint.x * m_scale.x + m_offsetPosition.x,
				m_node[j].myPoint.y * m_scale.y + m_offsetPosition.y,4,0.0f,1.0f,0.0f,1.0f);
		}else
		{//�������ú�ɫ
			XRender::drawPoint(m_node[j].myPoint.x * m_scale.x + m_offsetPosition.x,
				m_node[j].myPoint.y * m_scale.y + m_offsetPosition.y,4,1.0f,0.0f,0.0f,1.0f);
		}
		XRender::drawLine(m_node[j].myPoint * m_scale + m_offsetPosition,m_node[j + 1].myPoint * m_scale + m_offsetPosition);
	}
	XRender::drawPoint(m_node[m_nodeSum - 1].myPoint.x * m_scale.x + m_offsetPosition.x,
		m_node[m_nodeSum - 1].myPoint.y * m_scale.y + m_offsetPosition.y,4,1.0f,0.0f,0.0f,1.0f);
	if(m_isLoop)
	{
		XRender::drawLine(m_node[m_nodeSum - 1].myPoint * m_scale + m_offsetPosition,m_node[0].myPoint * m_scale + m_offsetPosition);
	}
	XVector2 tmpVec(m_specialPoint.position.x * m_scale.x,m_specialPoint.position.y * m_scale.y);
	XRender::drawLine(tmpVec,m_specialPoint.angle * RADIAN2DEGREE,20.0f);
	//�ڵ�ǰ���ڵ�λ�û�һ��ʮ��
	XRender::drawCross(tmpVec,10.0f * m_scale.x);
}
#if !WITH_INLINE_FILE
#include "XNodeLine.inl"
#endif
}