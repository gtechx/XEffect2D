#ifndef _JIA_XSTAGEMANAGER_
#define _JIA_XSTAGEMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include "XStageObject.h"
#include <vector>
//����һ����̨���࣬������2.5D����̨�ϱ���2D�����壬Ŀǰ���ܱȽϴֲ�
//����Ҫ֧��sprite��frame��flash��movie
namespace XE{
//��̨����Ϣ
class XStageInfo
{
public:
	XVec3 m_cameraPosition;	//�������λ��
	float m_near;	//�����Ľ�����	������ϵ��Ϊ1.0	��upΪ0
	float m_far;	//������Զ����	������ϵ��Ϊm_scaleRate	��upΪm_depthToUpRate
	float m_scaleRate;	//��ȵ�����ϵ��
	float m_depthToUpRate;	//��������ߵ�ϵ����
	XVec3 screenToGround(float x,float y)const;	//������Ļ���꣬���ض�Ӧ�ĵ�������
	XVec2 worldToScreen(const XVec3& worldPos)const{return worldToScreen(worldPos.x,worldPos.y,worldPos.z);}	//��������ת������Ļ����
	XVec2 worldToScreen(float x,float y,float z)const;//��������ת������Ļ����
	XVec2 getCameraScale(const XVec3& worldPos)const{return getCameraScale(worldPos.x,worldPos.y,worldPos.z);}	//��ȡ�����������ϵ��
	XVec2 getCameraScale(float x,float y,float z)const	//��ȡ�����������ϵ��
	{
		return XVec2(XMath::mapping1DEx(z - m_cameraPosition.z, m_near, m_far, 1.0f, m_scaleRate));
	}
	const bool check()const	//�������Ƿ�Ϸ�
	{
		if(m_scaleRate <= 0.0f) return false;	//�Ƿ��Ĳ���
		if(m_far <= m_near) return false;
		return true;
	}
};

class XStageManager
{
private:
	bool m_isInited;
	XStageInfo m_info;	//��̨��Ϣ
	std::vector<XStageObject *> m_objects;	//��̨�ϵ��������
	void sortObject();	//����z˳�������������򣬲������������ʾ��Ϣ
	void qSort(int low,int high)
	{//�������԰ٶ�
		if (low >= high) return;
		int first = low;
		int last = high;
		XStageObject *key = m_objects[first];	//���ֱ�ĵ�һ����¼��Ϊ����
		while (first<last)
		{
			while(first < last && m_objects[last]->m_position.z >= key->m_position.z)
				--last;
			m_objects[first] = m_objects[last];	//���ȵ�һ��С���Ƶ��Ͷ�
			while(first < last && m_objects[first]->m_position.z <= key->m_position.z)
				++first;
			m_objects[last] = m_objects[first];	//���ȵ�һ������Ƶ��߶�
		}
		m_objects[first] = key;	//�����¼��λ
		qSort(low, first - 1);
		qSort(first + 1, high);
	}
public:
	void release()
	{
		if(!m_isInited) return;
		m_objects.clear();
		m_isInited = false;
	}
	void setCameraPosition(const XVec3& pos)
	{
		m_info.m_cameraPosition = pos;
	}
	void setCameraPosition(float x,float y,float z)
	{
		m_info.m_cameraPosition.set(x,y,z);
	}
	XStageManager()
		:m_isInited(false)
	{}
	~XStageManager(){release();}
	void init(const XStageInfo &info)
	{
		if(m_isInited || !info.check()) return;
		m_info = info;
		m_isInited = true;
	}
	XStageInfo getStageInfo()const{return m_info;}
	void setStageInfo(const XStageInfo &info){if(info.check()) m_info = info;}
	void draw();
	void drawFloor(int size);
	void update(float stepTime);
	bool addAObject(XStageObject *obj);			//�򳡾�������һ���������������Ƿ�ɹ�
	bool removeAObject(XStageObject *obj);		//�ӳ������Ƴ�һ�����
	bool haveObject(const XStageObject *obj);	//��ȡ������ָ������ı�ţ�����������򷵻�-1
	XVec3 screenToGround(float x,float y)const{return m_info.screenToGround(x,y);}	//������Ļ���꣬���ض�Ӧ�ĵ�������
	XVec2 worldToScreen(float x,float y,float z)const{return m_info.worldToScreen(x,y,z);}	//��������ת������Ļ����
};
}
#endif