//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "glew.h"
#include "XResourceManager.h"
#include "XResourcePack.h"
#include "XBasicOpenGl.h"
#include "XLogBook.h"
#include "XControl/XControls.h"
#include "XBasicWindow.h"
void _XResourceTex::release()
{
	if(glIsTexture(m_texID)) 
	{
		LogNull("delete texture:%d",m_texID);
		glDeleteTextures(1,&m_texID);
	}
}
_XBool _XResourceInfo::load(_XResourcePosition resoursePosition)			//��Դ�����뺯��
{
	if(m_isInited) return XTrue;	//��ֹ�ظ�������
	if(m_name == NULL) return XFalse;
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	//���߲�ͬ�����͵��ò�ͬ�����뺯��
	std::string logstr = "";
	switch(m_type)
	{
	case RESOURCE_TYPE_NULL:
		return XFalse;
		break;
	case RESOURCE_TYPE_TEXTURE:
		{
			_XResourceTex * temp = createMem<_XResourceTex>();
			if(temp == NULL) return XFalse;
			if(!TextureLoadEx(temp->m_texID,m_name,&temp->m_width,&temp->m_height,resoursePosition)) 
			{
				XDELETE(temp);
				return XFalse;
			}
			logstr += toString(temp->m_texID) + "|";
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_SOUND:
		{
			_XResourceSound * temp = createMem<_XResourceSound>();
			if(temp == NULL) return XFalse;
			if(resoursePosition == RESOURCE_LOCAL_FOLDER)
			{
				if(!_XSoundHandle.loadSound(m_name,temp->m_handle)) 
				{
					logstr += "Sound load error!";
					LogStr(logstr.c_str());
					XDELETE(temp);
					return XFalse;
				}
			}else
			if(resoursePosition == RESOURCE_LOCAL_PACK)
			{
				int lengthTemp = _XResourcePack::GetInstance().getFileLength(m_name);
				unsigned char *p = createArrayMem<unsigned char>(lengthTemp + 1);
				if(p == NULL) 
				{
					XDELETE(temp);
					return XFalse;
				}

				_XResourcePack::GetInstance().unpackResource(m_name,p);
				if(!_XSoundHandle.loadSound(p,lengthTemp,temp->m_handle))
				{
					logstr += "Sound load error!";
					LogStr(logstr.c_str());
					XDELETE(temp);
					return XFalse;
				}
				XDELETE_ARRAY(p);
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XBUTTON_TEX:
		{
			_XButtonTexture * temp = createMem<_XButtonTexture>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_name,resoursePosition))
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XCHECK_TEX:
		{
			_XCheckTexture * temp = createMem<_XCheckTexture>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_name,resoursePosition))
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XEDIT_TEX:
		{
			_XEditTexture * temp = createMem<_XEditTexture>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_name,resoursePosition))
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XSLIDER_TEX:
		{
			_XSliderTexture * temp = createMem<_XSliderTexture>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_name,resoursePosition))
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XPROGRESS_TEX:
		{
			_XProgressTexture * temp = createMem<_XProgressTexture>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_name,resoursePosition))
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XMULTILIST_TEX:
		{
			_XMultiListTexture * temp = createMem<_XMultiListTexture>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_name,resoursePosition))
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XCOMBO_TEX:
		{
			_XComboTexture * temp = createMem<_XComboTexture>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_name,resoursePosition))
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XDIRLIST_TEX:
		{
			_XDirListTexture * temp = createMem<_XDirListTexture>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_name,resoursePosition))
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XPASSWORDPAD_TEX:
		{
			_XPasswordPadTexture * temp = createMem<_XPasswordPadTexture>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_name,resoursePosition))
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	}
	logstr += "Load resource:" + std::string(m_name);
	LogStr(logstr.c_str());

	m_isInited = XTrue;
	return XTrue;
}
void _XResourceInfo::release()			//��Դ���ͷź���
{
	if(!m_isInited) return;
	//�ͷ�ʵ�ʵ���Դ
	if(m_counter > 0)
	{
		LogNull("There is something wrong! - %d:%s",m_counter,m_name);
	}
	XDELETE_ARRAY(m_name);
	switch(m_type)
	{
	case RESOURCE_TYPE_TEXTURE:
		{
			_XResourceTex * temp = (_XResourceTex *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_SOUND:
		{
			_XResourceSound * temp = (_XResourceSound *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XBUTTON_TEX:
		{
			_XButtonTexture * temp = (_XButtonTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XCHECK_TEX:
		{
			_XCheckTexture * temp = (_XCheckTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XEDIT_TEX:
		{
			_XEditTexture * temp = (_XEditTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XSLIDER_TEX:
		{
			_XSliderTexture * temp = (_XSliderTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XPROGRESS_TEX:
		{
			_XProgressTexture * temp = (_XProgressTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XMULTILIST_TEX:
		{
			_XMultiListTexture * temp = (_XMultiListTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XCOMBO_TEX:
		{
			_XComboTexture * temp = (_XComboTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XDIRLIST_TEX:
		{
			_XDirListTexture * temp = (_XDirListTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XPASSWORDPAD_TEX:
		{
			_XPasswordPadTexture * temp = (_XPasswordPadTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	}
	m_isInited = XFalse;
}
_XResourceInfo *_XResourceManager::loadResource(const char * name,_XResourceType type,_XResourcePosition resoursePosition)	//����ָ����Դ
{
	if(name == NULL) return NULL;
	_XResourceInfo * temp = isLoad(name);
	if(temp != NULL && temp->m_type == type)
	{//�Ѿ�����,������Դ��ƥ���
		++ temp->m_counter;	//����һ������
		LogNull("%d:%s",temp->m_counter,temp->m_name);
		return temp;
	}else
	{//��δ����
		temp = createMem<_XResourceInfo>();
		if(temp == NULL) return NULL;
		temp->m_name = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
		if(temp->m_name == NULL) return NULL;
		strcpy(temp->m_name,name);
		temp->m_type = type;
		if(temp->load(resoursePosition))
		{
			m_resourceBuff.push_back(temp);
			++ m_resourceSum;
			++ temp->m_counter;
			return temp;	//��Դ����ɹ�
		}else
		{//��Դ����ʧ��
			XDELETE_ARRAY(temp->m_name);
			XDELETE(temp);
			return NULL;
		}
	}
}
_XBool _XResourceManager::releaseResource(const _XResourceInfo *p)	//�ͷ�һ����Դ
{
	//for(int i = 0;i < m_resourceSum;++ i)
	//{
	//	if(m_resourceBuff[i] == p)
	//	{
	//		m_resourceBuff[i]->m_counter --;
	//		LogNull("%d:%s",m_resourceBuff[i]->m_counter,m_resourceBuff[i]->m_name);
	//		//if(m_resourceBuff[i]->m_counter == 0) //û�����õ�ʱ��Ϳ����ͷ���
	//		if(m_resourceBuff[i]->m_counter < 0){LogStr("Error");}
	//		return XTrue;
	//	}
	//}
	if(m_resourceBuff.size() <= 0)//����������������ڶ����Լ��ͷŵ��Ⱥ���ɵģ�������Ӧ��ûʲô������
	{//��������˳����ɵ����⣬������Ҫ����
		LogStr("Error!");
		return XFalse;
	}
	std::list<_XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		if(* it == p)
		{
			-- (*it)->m_counter;
			LogNull("%d:%s",(*it)->m_counter,(*it)->m_name);
			if((*it)->m_counter <= 0)
			{//�ͷŵ������Դ
				XDELETE_ARRAY((*it)->m_name);
				XDELETE((* it));
				m_resourceBuff.erase(it);
				-- m_resourceSum;
			}
			return XTrue;
		}
	}
	LogStr("Error:invalid resource!");
	return XFalse;
}
void _XResourceManager::release()				//�ͷ����е���Դ
{
	//int sum = m_resourceBuff.size();
	//if(sum == 0) return;	//��ֹ�ظ��ͷ�
	//for(int i = 0;i < sum;++ i)
	//{
	//	m_resourceBuff[i]->release();
	//	XDELETE(m_resourceBuff[i]);
	//}
	//m_resourceBuff.clear();
	//m_resourceBuff.swap(std::vector<_XResourceInfo *>());
	std::list<_XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		//(* it)->release();
		LogNull("%d:%s",(*it)->m_counter,(*it)->m_name);
		XDELETE_ARRAY((*it)->m_name);
		XDELETE((* it));
	}
	m_resourceBuff.clear();
	m_resourceSum = 0;
}
_XBool _XResourceManager::isLoaded(const char * name)	//�ж�ָ����Դ�Ƿ��Ѿ�����
{
	if(name == NULL) return XFalse;
	//for(int i = 0;i < m_resourceSum;++ i)
	//{
	//	if(m_resourceBuff[i]->isLoaded() && fileNameCompare(name,m_resourceBuff[i]->m_name))
	//	{
	//		return XTrue;
	//	}
	//}
	std::list<_XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		if((*it)->isLoaded() && fileNameCompare(name,(*it)->m_name))
		{
			return XTrue;
		}
	}
	return XFalse;
}
_XResourceInfo * _XResourceManager::isLoad(const char * name)
{
	if(name == NULL) return NULL;
	//for(int i = 0;i < m_resourceSum;++ i)
	//{
	//	if(m_resourceBuff[i]->isLoaded() && fileNameCompare(name,m_resourceBuff[i]->m_name))
	//	{
	//		return m_resourceBuff[i];
	//	}
	//}
	std::list<_XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		if((*it)->isLoaded() && fileNameCompare(name,(*it)->m_name))
		{
			return (*it);
		}
	}
	return NULL;
}