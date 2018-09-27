#include "XStdHead.h"
#include "XStageManager.h"
namespace XE{
XVec3 XStageInfo::screenToGround(float x,float y)const
{
	XVec3 ret;
	ret.y = 0.0f;
	//ret.z = XMath::mapping1DEx(getSceneHeight() - y,0.0f,m_depthToUpRate,m_near,m_far) + m_cameraPosition.z;
	ret.z = (getSceneHeight() - y + m_cameraPosition.y) / (m_depthToUpRate - m_cameraPosition.y * (m_scaleRate - 1.0f)) * (m_far - m_near) + m_near + m_cameraPosition.z;
//	tmpz = ((ret.z - m_cameraPosition.z) - m_near) / (m_far - m_near)
//	tmpz * m_depthToUpRate - cy * (tmpz * (m_scaleRate - 1.0f) + 1.0) = getSceneHeight() - y
//	tmpz * (m_depthToUpRate - cy * (m_scaleRate - 1.0f)) = getSceneHeight() - y + cy
//	(in - imin) / (imax - imin) * (omax - omin) + omin;
	float scale = XMath::mapping1DEx((ret.z - m_cameraPosition.z),m_near,m_far,1.0f,m_scaleRate);
	if(scale <= 0.0f) ret.x = 0.0f;
	else
		ret.x = (x - getSceneWidth() * 0.5f) / scale + m_cameraPosition.x;
	return ret;
}
XVec2 XStageInfo::worldToScreen(float x,float y,float z)const//��������ת������Ļ����
{
	XVec2 ret;
	float scale = XMath::mapping1DEx(z - m_cameraPosition.z,m_near,m_far,1.0f,m_scaleRate);
	ret.x =	getSceneWidth() * 0.5f + (x - m_cameraPosition.x) * scale;
	ret.y = (y - m_cameraPosition.y) * scale + XMath::mapping1DEx(z - m_cameraPosition.z,m_near,m_far,0.0f,m_depthToUpRate);
	ret.y = getSceneHeight() - ret.y;
	return ret;
}
void XStageManager::sortObject()
{
	qSort(0,int(m_objects.size()) - 1);
	XVec2 screenPos;
	XVec2 screenScale;
	for(int i = 0;i < m_objects.size();++ i)
	{
		screenScale = m_info.getCameraScale(m_objects[i]->m_position) * m_objects[i]->m_scale;
		screenPos = m_info.worldToScreen(m_objects[i]->m_position);
		screenPos -= m_objects[i]->m_originOffset * screenScale;
		m_objects[i]->setPosition(screenPos);
		m_objects[i]->setScale(screenScale);
	}
}
void XStageManager::draw()
{
	if(!m_isInited) return;
	sortObject();//����������������򣬲���������Ⱦ����
	for(int i = int(m_objects.size()) - 1;i >= 0;-- i)//һ����Ⱦ���е����
	{
		m_objects[i]->draw();
	}
}
void XStageManager::drawFloor(int size)
{
	if(!m_isInited) return;
	for(int i = 0;i <= size;i += 32)
	{
		XRender::drawLine(worldToScreen(i,0,m_info.m_near),worldToScreen(i,0,m_info.m_far),1,XFColor::red);
		XRender::drawLine(worldToScreen(-i,0,m_info.m_near),worldToScreen(-i,0,m_info.m_far),1,XFColor::red);
	}
	for(int i = m_info.m_near;i <= m_info.m_far;i += 32)
	{
		XRender::drawLine(worldToScreen(size,0,i),worldToScreen(-size,0,i),1,XFColor::red);
	}
}
void XStageManager::update(float stepTime)
{
	if(!m_isInited) return;
	for(int i = int(m_objects.size()) - 1;i >= 0;-- i)//һ����Ⱦ���е����
	{
		m_objects[i]->update(stepTime);
	}
}
bool XStageManager::addAObject(XStageObject *obj)	//�򳡾�������һ���������������Ƿ�ɹ�
{
	if(obj == NULL) return false;
	if(haveObject(obj)) return true;
	//�����Ȳ�����
	m_objects.push_back(obj);
	return true;
}
bool XStageManager::removeAObject(XStageObject *obj)	//�ӳ������Ƴ�һ�����
{
	if(obj == NULL) return false;
	for(int i = 0;i < m_objects.size();++ i)
	{
		if(m_objects[i] == obj)
		{
			m_objects.erase(m_objects.begin() + i);
			return true;
		}
	}
	return false;
}
bool XStageManager::haveObject(const XStageObject *obj)	//��ȡ������ָ������ı�ţ�����������򷵻�-1
{
	if(obj == NULL) return false;
	for(int i = 0;i < m_objects.size();++ i)
	{
		if(m_objects[i] == obj) return true;
	}
	return false;
}
}