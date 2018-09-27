#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "X2DPacker.h"
namespace XE{
//���ո߶�����
void X2DPacker::orderObjectByWidth()
{//�߶�����
	XObjectBox temp;
	XBool flag = XFalse;
	while(true)
	{
		flag = XFalse;
		for(int i = 0;i < m_objectBoxSum - 1;++ i)
		{
			if(m_objectBox[i].m_size.x < m_objectBox[i + 1].m_size.x)
			{
				XMath::swapTwoSum(m_objectBox[i],m_objectBox[i + 1]);
			//	temp = m_objectBox[i];
			//	m_objectBox[i] = m_objectBox[i + 1];
			//	m_objectBox[i + 1] = temp;
				flag = XTrue;
			}
		}
		if(!flag) break;	//�������
	}
}
//�����������
void X2DPacker::orderObjectByArea()
{
	XObjectBox temp;
	XBool flag = XFalse;
	while(true)
	{
		flag = XFalse;
		for(int i = 0;i < m_objectBoxSum - 1;++ i)
		{
			if(m_objectBox[i].m_acreage < m_objectBox[i + 1].m_acreage)
			{
				XMath::swapTwoSum(m_objectBox[i],m_objectBox[i + 1]);
			//	temp = m_objectBox[i];
			//	m_objectBox[i] = m_objectBox[i + 1];
			//	m_objectBox[i + 1] = temp;
				flag = XTrue;
			}
		}
		if(!flag) break;
	}
}
//������������Ȼ���ٰ��������
void X2DPacker::orderObjectByBigEdgeAndArea()
{
	orderObjectByArea();	//���Ȱ��������
	XObjectBox temp;
	XBool flag = XFalse;
	while(true)
	{
		flag = XFalse;
		for(int i = 0;i < m_objectBoxSum - 1;++ i)
		{
			if((std::max)(m_objectBox[i].m_size.x,m_objectBox[i].m_size.y) < 
				(std::max)(m_objectBox[i + 1].m_size.x,m_objectBox[i + 1].m_size.y))
			{
				XMath::swapTwoSum(m_objectBox[i],m_objectBox[i + 1]);
			//	temp = m_objectBox[i];
			//	m_objectBox[i] = m_objectBox[i + 1];
			//	m_objectBox[i + 1] = temp;
				flag = XTrue;
			}
		}
		if(!flag) break;
	}
}
int X2DPacker::findResult(int orderMode)
{
	//����û�н�����ϸ�����ݺ����Լ�飬֮����Ҫ������ϸ�ļ��
	if(m_basicBox == NULL || m_objectBox == NULL) return 0;
	if(m_objectBoxSum >= 2)
	{//���ȶ���������������ȷ��������
		switch(orderMode)
		{
		case 1:orderObjectByWidth();break;//�������
		case 2:orderObjectByBigEdgeAndArea();break;	//�Ȱ����������ٰ��������
		default:orderObjectByArea();break;	//���������
		}
	}
	//��������б��
	for(int i = 0;i < m_objectBoxSum;++ i)
	{
		m_objectBox[i].m_order = i;
	}
	//���װ�������Ƿ���Ч
	int checkFlag = 0;
	//1����������ߴ粻�ܴ������ӵĳߴ�
	//for(int i = 0;i < m_objectBoxSum - 1;++ i)
	if(!m_canRotate)
	{//������ת��װ������
		for(int i = 0;i < m_objectBoxSum;++ i)
		{
			checkFlag = 0;
			for(int j = 0;j < m_basicBoxSum;++ j)
			{//���￼��װ��Ĵ�С���Բ�һ��
				if(m_basicBox[j].m_size.x >= m_objectBox[i].m_size.x &&
					m_basicBox[j].m_size.y >= m_objectBox[i].m_size.y)
				{//��������װ����
					checkFlag = 1;
					break;
				}
			}
			if(checkFlag == 0) return 0;	//���ڳ�������������Ӷ�װ����
		}
	}else
	{//������ת��װ������
		for(int i = 0;i < m_objectBoxSum;++ i)
		{
			checkFlag = 0;
			for(int j = 0;j < m_basicBoxSum;++ j)
			{//���￼��װ��Ĵ�С���Բ�һ��
				if((m_basicBox[j].m_size.x >= m_objectBox[i].m_size.x &&
					m_basicBox[j].m_size.y >= m_objectBox[i].m_size.y)
					|| (m_basicBox[j].m_size.x >= m_objectBox[i].m_size.y &&
					m_basicBox[j].m_size.y >= m_objectBox[i].m_size.x))
				{//��������װ����
					checkFlag = 1;
					break;
				}
			}
			if(checkFlag == 0) return 0;	//���ڳ�������������Ӷ�װ����
		}
	}
	//���潫���е��������������
	for(int i = 0;i < m_objectBoxSum;++ i)
	{
		if(putInABox(m_objectBox[i]) == 0) return 0;	//����װ���µ����
	}
	return 1;
}
int X2DPacker::putInABox(XObjectBox &objectBox)
{
	if(objectBox.m_basicBoxOrder >= 0) return 1;//�������Ѿ�������ˣ������·�
	for(int i = 0;i < m_basicBoxSum;++ i)
	{//�������е����ӣ����Ƿ��ܷ���
		if(canPutIn(objectBox,m_basicBox[i]) == 1) return 1;	//����ܷ�����ֱ�ӷ��룬���ﲻʹ���Ż�����
	}
	return 0;	//���к��Ӷ�װ������ֱ�ӷ���װ��ʧ��
}
void X2DPacker::updatePutOperate(XObjectBox &objectBox,XBasicBox &basicBox,XBool needRotate,float positionX,float positionY)
{
	XVec2 tempSize(0.0f);
	if(!needRotate) tempSize.set(objectBox.m_size);
	else tempSize.set(objectBox.m_size.y,objectBox.m_size.x);

	objectBox.m_basicBoxOrder = basicBox.m_order;
	objectBox.m_position.set(positionX,positionY);
	objectBox.m_AABB.set(positionX,positionY,positionX + tempSize.x,positionY + tempSize.y);	//������ײ����
	objectBox.m_needRotate = needRotate;
	basicBox.m_objectBoxesOrder[basicBox.m_beUsed] = objectBox.m_order;		//����Ѿ����������ı��
	++ basicBox.m_beUsed;	//����Ѿ�������һ�����
	basicBox.m_releaseAcreage -= objectBox.m_acreage;		//��ȥ�Ѿ���������
}
void X2DPacker::updateNewPoint(XBasicBox &basicBox,int sizeX,int sizeY,int pointOrder)
{
	basicBox.m_mayPosition[basicBox.m_mayPositionSum].set(basicBox.m_mayPosition[pointOrder].x,basicBox.m_mayPosition[pointOrder].y + sizeY);	//�±ߵĵ�
	//���ڵ�ǰ������Ѿ����˶�����������������ֱ�ӱ��滻��
	basicBox.m_mayPosition[pointOrder].set(basicBox.m_mayPosition[pointOrder].x + sizeX,basicBox.m_mayPosition[pointOrder].y);	//��ߵĵ�
	++ basicBox.m_mayPositionSum;
}
void X2DPacker::updateNewXY(XBasicBox &basicBox,int x,int y)
{
	XBool isInsert = XFalse;	//�Ƿ��Ѿ�����
	for(int i = 0;i < basicBox.m_mayPositionXSum;++ i)
	{
		if(basicBox.m_mayPositionX[i] > x)
		{//����
			for(int j = basicBox.m_mayPositionXSum;j > i;-- j)
			{
				basicBox.m_mayPositionX[j] = basicBox.m_mayPositionX[j - 1];
			}
			basicBox.m_mayPositionX[i] = x;
			++ basicBox.m_mayPositionXSum;
			isInsert = XTrue;
			break;
		}else
		if(basicBox.m_mayPositionX[i] == x) 
		{
			isInsert = XTrue;
			break;
		}
	}
	if(!isInsert)
	{//����ʧ��
		basicBox.m_mayPositionX[basicBox.m_mayPositionXSum] = x;
		++ basicBox.m_mayPositionXSum;
	}
	isInsert = XFalse;
	for(int i = 0;i < basicBox.m_mayPositionYSum;++ i)
	{
		if(basicBox.m_mayPositionY[i] > y)
		{//����
			for(int j = basicBox.m_mayPositionYSum;j > i;-- j)
			{
				basicBox.m_mayPositionY[j] = basicBox.m_mayPositionY[j - 1];
			}
			basicBox.m_mayPositionY[i] = y;
			++ basicBox.m_mayPositionYSum;
			isInsert = XTrue;
			break;
		}else
		if(basicBox.m_mayPositionY[i] == y) 
		{
			isInsert = XTrue;
			break;
		}
	}
	if(!isInsert)
	{//����ʧ��
		basicBox.m_mayPositionY[basicBox.m_mayPositionYSum] = y;
		++ basicBox.m_mayPositionYSum;
	}
}
//������������ж�����Ƿ��ܹ�����ָ��������
int X2DPacker::canPutIn(XObjectBox &objectBox,XBasicBox &basicBox)	
{
	if(basicBox.m_releaseAcreage < objectBox.m_acreage) return 0;	//ʣ���������ֱ�ӷ���ʧ��
	if(basicBox.m_beUsed == 0)
	{//�������û��װ����������ֱ�����ϽǶ������
		if(basicBox.m_size.x >= objectBox.m_size.x && basicBox.m_size.y >= objectBox.m_size.y)
		{//ֱ�ӷ��þͿ��Էŵ���
			updatePutOperate(objectBox,basicBox,XFalse,0,0);
			if(!m_isAdvancedOptimize) updateNewPoint(basicBox,objectBox.m_size.x,objectBox.m_size.y,0);
			else updateNewXY(basicBox,objectBox.m_size.x,objectBox.m_size.y);
			return 1;
		}
		if(m_canRotate && basicBox.m_size.x >= objectBox.m_size.y && basicBox.m_size.y >= objectBox.m_size.x)
		{//������ת����
			updatePutOperate(objectBox,basicBox,XTrue,0,0);
			if(!m_isAdvancedOptimize) updateNewPoint(basicBox,objectBox.m_size.y,objectBox.m_size.x,0);
			else updateNewXY(basicBox,objectBox.m_size.y,objectBox.m_size.x);
			return 1;
		}
		return 0;
	}else
	{//������ӱ�װ������
		//�������еķ���㣬�ҵ���һ�����ʵķ����
		int pointOK = 0;	//������Ƿ���������ĺ��ӷ�����ײ
		XRect objRect1;
		if(m_isAdvancedOptimize)
		{
			float tempPx,tempPy;
			for(int k = 0;k < basicBox.m_mayPositionYSum;++ k)
			{
				tempPy = (float)(basicBox.m_mayPositionY[k]);
				//������Ϊ���Ż��ٶȣ����ڲ���Ҫ�жϵĵ�ֱ�ӷ���
				if((!m_canRotate && tempPy + objectBox.m_size.y > basicBox.m_size.y)
					|| (m_canRotate && tempPy + objectBox.m_size.x > basicBox.m_size.y && tempPy + objectBox.m_size.y > basicBox.m_size.y)) continue;
				for(int i = 0;i < basicBox.m_mayPositionXSum;++ i)
				{
					tempPx = (float)(basicBox.m_mayPositionX[i]);
					if((!m_canRotate && tempPx + objectBox.m_size.x > basicBox.m_size.x)
						|| (m_canRotate && tempPx + objectBox.m_size.y > basicBox.m_size.x && tempPx + objectBox.m_size.x > basicBox.m_size.x)) break;

					pointOK = 0;
					if(tempPx + objectBox.m_size.x <= basicBox.m_size.x
						&& tempPy + objectBox.m_size.y <= basicBox.m_size.y)//�жϱ߽��Ƿ�Խ��
					{
						pointOK = 1;
						//������������ӵ���ײ����
						objRect1.set(tempPx,tempPy,tempPx + objectBox.m_size.x,tempPy + objectBox.m_size.y);
						pointOK = 1;
						for(int j = 0;j < basicBox.m_beUsed;++ j)
						{
							if(objRect1.isCrash(m_objectBox[basicBox.m_objectBoxesOrder[j]].m_AABB))
							{
								pointOK = 0;
								break;
							}
						}
					}
					if(pointOK != 0)
					{//��������
						updatePutOperate(objectBox,basicBox,XFalse,tempPx,tempPy);
						//��������ķ�������µĿ���
						updateNewXY(basicBox,tempPx + objectBox.m_size.x,tempPy + objectBox.m_size.y);
						return 1;
					}else
					if(m_canRotate)
					{//��ǰ��ϲ�����Ҫ�󣬿�����ת
						pointOK = 0;
						if(tempPx + objectBox.m_size.y <= basicBox.m_size.x
							&& tempPy + objectBox.m_size.x <= basicBox.m_size.y)//�жϱ߽��Ƿ�Խ��
						{
							pointOK = 1;
							//������������ӵ���ײ����
							objRect1.set(tempPx,tempPy,tempPx + objectBox.m_size.y,tempPy + objectBox.m_size.x);
							pointOK = 1;
							for(int j = 0;j < basicBox.m_beUsed;++ j)
							{
								if(objRect1.isCrash(m_objectBox[basicBox.m_objectBoxesOrder[j]].m_AABB))
								{
									pointOK = 0;
									break;
								}
							}
						}
						if(pointOK != 0)
						{//��������
							updatePutOperate(objectBox,basicBox,XTrue,tempPx,tempPy);
							//��������ķ�������µĿ���
							updateNewXY(basicBox,tempPx + objectBox.m_size.y,tempPy + objectBox.m_size.x);
							return 1;
						}else
						{
							continue;
						}
					}else
					{
						continue;
					}
				}
			}
		}else
		{
			for(int i = 0;i < basicBox.m_mayPositionSum;++ i)
			{
				pointOK = 0;
				if(basicBox.m_mayPosition[i].x + objectBox.m_size.x <= basicBox.m_size.x
					&& basicBox.m_mayPosition[i].y + objectBox.m_size.y <= basicBox.m_size.y)//�жϱ߽��Ƿ�Խ��
				{
					pointOK = 1;
					//������������ӵ���ײ����
					objRect1.set(basicBox.m_mayPosition[i].x,basicBox.m_mayPosition[i].y,
						basicBox.m_mayPosition[i].x + objectBox.m_size.x,basicBox.m_mayPosition[i].y + objectBox.m_size.y);
					for(int j = 0;j < basicBox.m_beUsed;++ j)
					{
						if(objRect1.isCrash(m_objectBox[basicBox.m_objectBoxesOrder[j]].m_AABB))
						{
							pointOK = 0;
							break;
						}
					}
				}
				if(pointOK != 0)
				{//��������
					updatePutOperate(objectBox,basicBox,XFalse,basicBox.m_mayPosition[i].x,basicBox.m_mayPosition[i].y);
					updateNewPoint(basicBox,objectBox.m_size.x,objectBox.m_size.y,i);
					return 1;
				}else
				{//����㲻�У�������ת
					if(m_canRotate)
					{//������ת
						pointOK = 0;
						if(basicBox.m_mayPosition[i].x + objectBox.m_size.y <= basicBox.m_size.x
							&& basicBox.m_mayPosition[i].y + objectBox.m_size.x <= basicBox.m_size.y)//�жϱ߽��Ƿ�Խ��
						{
							pointOK = 1;
							//������������ӵ���ײ����
							objRect1.set(basicBox.m_mayPosition[i].x,basicBox.m_mayPosition[i].y,
								basicBox.m_mayPosition[i].x + objectBox.m_size.y,basicBox.m_mayPosition[i].y + objectBox.m_size.x);
							pointOK = 1;
							for(int j = 0;j < basicBox.m_beUsed;++ j)
							{
								if(objRect1.isCrash(m_objectBox[basicBox.m_objectBoxesOrder[j]].m_AABB))
								{
									pointOK = 0;
									break;
								}
							}
						}
						if(pointOK == 0)
						{//����㲻��
							continue;
						}else
						{//��������
							updatePutOperate(objectBox,basicBox,XTrue,basicBox.m_mayPosition[i].x,basicBox.m_mayPosition[i].y);
							updateNewPoint(basicBox,objectBox.m_size.y,objectBox.m_size.x,i);
							return 1;
						}
					}else
					{//�����������ת��ֱ�Ӽ���
						continue;
					}
				}
			}
		}
	}
	return 0;
}
int X2DPacker::getNeedBoxSum() const
{
	if(m_basicBox == NULL) return 0;
	for(int i = 0;i < m_basicBoxSum;++ i)
	{
		if(m_basicBox[i].m_beUsed == 0) return i;
	}
	return 0;
}
void X2DPacker::reset()
{
	for(int i = 0;i < m_basicBoxSum;++ i)
	{
		m_basicBox[i].m_beUsed = 0;
		m_basicBox[i].m_releaseAcreage = m_basicBox[i].m_size.x * m_basicBox[i].m_size.y;

		m_basicBox[i].m_mayPositionSum = 1;
		m_basicBox[i].m_mayPosition[0].reset();

		m_basicBox[i].m_mayPositionXSum = 1;
		m_basicBox[i].m_mayPositionX[0] = 0;
		m_basicBox[i].m_mayPositionYSum = 1;
		m_basicBox[i].m_mayPositionY[0] = 0;
	}
	for(int i = 0;i < m_objectBoxSum;++ i)
	{
		m_objectBox[i].m_order = i;
		m_objectBox[i].m_setOrder = i;
		m_objectBox[i].m_position.reset();
		m_objectBox[i].m_basicBoxOrder = -1;
	}
}
}