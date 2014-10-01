//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XShaderGLSL.h"
#include "stdlib.h"
#include "glew.h"
#include "XBasicOpenGL.h"
#include "XMath/XVector2.h"
#include "XMath/XVector3.h"

void _XShaderDataInfo::updateData()	//��������
{
	if(m_p == NULL) return;
	switch(m_type)
	{
	case DATA_TYPE_INT:
		glUniform1i(m_handle,* (int *)(m_p));
		break;
	case DATA_TYPE_FLOAT:
		glUniform1f(m_handle,* (float *)(m_p));
		break;
	case DATA_TYPE_INT_ARRAY:
		glUniform1iv(m_handle,m_size,(int *)(m_p));
		break;
	case DATA_TYPE_FLOAT_ARRAY:
		glUniform1fv(m_handle,m_size,(float *)(m_p));
		break;
	case DATA_TYPE_2INT_ARRAY:
		glUniform2iv(m_handle,m_size,(int *)(m_p));
		break;
	case DATA_TYPE_2FLOAT_ARRAY:
		glUniform2fv(m_handle,m_size,(float *)(m_p));
		break;
	case DATA_TYPE_3INT_ARRAY:
		glUniform3iv(m_handle,m_size,(int *)(m_p));
		break;
	case DATA_TYPE_3FLOAT_ARRAY:
		glUniform3fv(m_handle,m_size,(float *)(m_p));
		break;
	case DATA_TYPE_3FLOAT_MATRIX:
		glUniformMatrix3fv(m_handle,m_size,GL_FALSE,(float *)(m_p));
		break;
	case DATA_TYPE_4FLOAT_MATRIX:
		glUniformMatrix4fv(m_handle,m_size,GL_FALSE,(float *)(m_p));
		break;
	case DATA_TYPE_2INT:
		glUniform2i(m_handle,((_XVector2 *)m_p)->x,((_XVector2 *)m_p)->y);
		break;
	case DATA_TYPE_2FLOAT:
		glUniform2f(m_handle,((_XVector2 *)m_p)->x,((_XVector2 *)m_p)->y);
		break;
	case DATA_TYPE_3INT:
		glUniform3i(m_handle,((_XVector3 *)m_p)->x,((_XVector3 *)m_p)->y,((_XVector3 *)m_p)->z);
		break;
	case DATA_TYPE_3FLOAT:
		glUniform3f(m_handle,((_XVector3 *)m_p)->x,((_XVector3 *)m_p)->y,((_XVector3 *)m_p)->z);
		break;
	}
}
_XBool _XShaderGLSL::init(const char* vertFile,const char* fragFile,_XResourcePosition resoursePosition,const char* geomFile)
{
	if(m_isInited) return XTrue;
	if(!setShader(vertFile,fragFile,geomFile,m_shaderHandle,resoursePosition)) return XFalse;
	m_dataSum = 0;
	m_texSum = 0;
	m_UBOSum = 0;
	m_isInited = XTrue;
	return XTrue;
}
void _XShaderGLSL::setGeometryInfo(unsigned int inType,unsigned int outType,int outSum)
{
	if(m_shaderHandle.shaderHandle == 0) m_shaderHandle.shaderHandle = glCreateProgram();
	glProgramParameteriEXT(m_shaderHandle.shaderHandle,GL_GEOMETRY_INPUT_TYPE_EXT,inType);
	glProgramParameteriEXT(m_shaderHandle.shaderHandle,GL_GEOMETRY_OUTPUT_TYPE_EXT,outType);
	glProgramParameteriEXT(m_shaderHandle.shaderHandle,GL_GEOMETRY_VERTICES_OUT_EXT,outSum);
}
int getGeometryMaxOutputCount()
{
	int temp = 0;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &temp);
	return temp;
}
//��������
_XBool _XShaderGLSL::connectData(const char *name,_XShaderDataType type,int size,void *p)
{
	if(size < 0 ||
		p == NULL ||
		name == NULL ||
		!m_isInited ||
		m_dataSum >= MAX_SHADER_GLSL_DATA_SUM - 1) return XFalse;
//	strcpy(m_dataInfo[m_dataSum].m_name,name);
//	m_dataInfo[m_dataSum].m_handle = glGetUniformLocation(m_shaderHandle.shaderHandle,name);
	m_dataInfo[m_dataSum].m_handle = glGetUniformLocation(m_shaderHandle.shaderHandle,name);
	m_dataInfo[m_dataSum].m_type = type;
	m_dataInfo[m_dataSum].m_p = p;
	m_dataInfo[m_dataSum].m_size = size;
	++ m_dataSum;
	return XTrue;
}
//������ͼ
_XBool _XShaderGLSL::connectTexture(const char *name,unsigned int * tex,int type)
{
	if(name == NULL) return XFalse;
	if(m_texSum != 0 && tex == NULL) return XFalse;
	//if(m_texSum >= MAX_SHADER_GLSL_DATA_SUM - 1) return XFalse;
	if(m_texSum >= 32 - 1) return XFalse;
	m_texHandle[m_texSum] = glGetUniformLocation(m_shaderHandle.shaderHandle,name);
	//glUniform1i(handle,m_texSum);
	m_texture[m_texSum] = tex;
	m_texType[m_texSum] = type;
	++ m_texSum;
	return XTrue;
}
_XBool _XShaderGLSL::connectUBO(const char *uboName,_XUBO *ubo)
{
	if(uboName == NULL || ubo == NULL) return XFalse;
	if(m_UBOSum >= MAX_SHADER_GLSL_DATA_SUM - 1) return XFalse;
	m_UBOInfo[m_UBOSum].m_handle = glGetUniformBlockIndex(m_shaderHandle.shaderHandle,uboName);
	m_UBOInfo[m_UBOSum].m_pUBO = ubo;
	++ m_UBOSum;
	return XTrue;
}
_XBool _XShaderGLSL::getUBOInfo(_XShaderUBOData &uboData,int valueSum,const char *uboName,const char **valueNames)	//����UBO
{
	if(valueSum <= 0 || valueSum >= MAX_SHADER_GLSL_DATA_SUM) return XFalse;
	uboData.m_handle = glGetUniformBlockIndex(m_shaderHandle.shaderHandle,uboName);  //��ȡ�ṹ�ı�� 
	if(GL_INVALID_INDEX == uboData.m_handle) return XFalse;
	glGetActiveUniformBlockiv(m_shaderHandle.shaderHandle,  
		                        uboData.m_handle,  
		                        GL_UNIFORM_BLOCK_DATA_SIZE,  
								&uboData.m_size);					//��ȡ�ṹ�ĳߴ�
	glGetUniformIndices(m_shaderHandle.shaderHandle,valueSum,valueNames,uboData.m_index);  //���ÿ�������ı�ǩ
	glGetActiveUniformsiv(m_shaderHandle.shaderHandle,valueSum,uboData.m_index,GL_UNIFORM_OFFSET,uboData.m_offset);	//��ȡÿ��������ƫ��
	return XTrue;
}
void _XShaderGLSL::useShader(bool withTex0)	//ʹ��shader
{
	if(!m_isInited) return;
	glUseProgram(m_shaderHandle.shaderHandle);
	//glUseProgramObjectARB(m_shaderHandle);
	//�����������
	for(int i = 0;i < m_dataSum;++ i)
	{
		m_dataInfo[i].updateData();
	}
	for(int i = 0;i < m_UBOSum;++ i)
	{
		m_UBOInfo[i].updateData();
	}
	int i;
	if(withTex0) i = 0;
	else
	{
		i = 1;
		glUniform1i(m_texHandle[0],0);
	}
	for(;i < m_texSum;++ i)
	{
		glUniform1i(m_texHandle[i],i);
		glActiveTexture(GL_TEXTURE0 + i);
		if(m_texType[i] == 0)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,*(m_texture[i]));
		}else
		if(m_texType[i] == 1)
		{//GL_TEXTURE_CUBE_MAP��ͼ
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP,*(m_texture[i]));
		}
	}
}
void _XShaderGLSL::useShaderEx(unsigned int tex0,int type)
{
	if(!m_isInited) return;
	glUseProgram(m_shaderHandle.shaderHandle);
	//glUseProgramObjectARB(m_shaderHandle);
	//�����������
	for(int i = 0;i < m_dataSum;++ i)
	{
		m_dataInfo[i].updateData();
	}
	for(int i = 0;i < m_UBOSum;++ i)
	{
		m_UBOInfo[i].updateData();
	}
	//�󶨵�һ����ͼ
	glUniform1i(m_texHandle[0],0);
	glActiveTexture(GL_TEXTURE0);
	if(type == 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,tex0);
	}else
	if(type == 1)
	{//GL_TEXTURE_CUBE_MAP��ͼ
		glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP,tex0);
	}
	//����������ͼ
	for(int i = 1;i < m_texSum;++ i)
	{
		glUniform1i(m_texHandle[i],i);
		glActiveTexture(GL_TEXTURE0 + i);
		if(m_texType[i] == 0)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,*(m_texture[i]));
		}else
		if(m_texType[i] == 1)
		{//GL_TEXTURE_CUBE_MAP��ͼ
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP,*(m_texture[i]));
		}
	}
}
void _XShaderGLSL::disShader()
{
	glUseProgram(0);
	//����ͼ���з�����
	if(m_texSum > 1)
	{
		for(int i = 1;i < m_texSum;++ i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			if(m_texType[i] == 0) glBindTexture(GL_TEXTURE_2D,0); else
			if(m_texType[i] == 1) glBindTexture(GL_TEXTURE_CUBE_MAP,0);
		}
		glActiveTexture(GL_TEXTURE0);
	}
}
void _XShaderGLSL::release()
{
	if(m_shaderHandle.shaderHandle == 0) return;
	if(m_shaderHandle.shaderV != 0)
	{
		glDetachShader(m_shaderHandle.shaderHandle,m_shaderHandle.shaderV);
		glDeleteShader(m_shaderHandle.shaderV);
		m_shaderHandle.shaderV = 0;
	}
	if(m_shaderHandle.shaderF != 0)
	{
		glDetachShader(m_shaderHandle.shaderHandle,m_shaderHandle.shaderF);
		glDeleteShader(m_shaderHandle.shaderF);
		m_shaderHandle.shaderF = 0;
	}
	if(m_shaderHandle.shaderG != 0)
	{
		glDetachShader(m_shaderHandle.shaderHandle,m_shaderHandle.shaderG);
		glDeleteShader(m_shaderHandle.shaderG);
		m_shaderHandle.shaderG = 0;
	}
	glDeleteProgram(m_shaderHandle.shaderHandle);
	m_shaderHandle.shaderHandle = 0;
}
_XBool _XUBO::init(int size,const void * p)
{
	if(m_isInited) return XTrue;
	if(p == NULL) return XFalse;
	int maxS = 0;
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE,&maxS);
	if(size > maxS) return XFalse;
	glGenBuffersARB(1,&m_uboHandle);  
	glBindBufferARB(GL_UNIFORM_BUFFER,m_uboHandle);  
	glBufferDataARB(GL_UNIFORM_BUFFER,size,p,GL_DYNAMIC_DRAW);

	m_isInited = XTrue;
	return XTrue;
}
void _XUBO::release()
{
	if(m_isInited)
	{
		glDeleteBuffersARB(1,&m_uboHandle);
		m_isInited = XFalse;
	}
}
void _XUBO::useUBO(unsigned int index)	//indexΪ��Ӧ��shader�нṹ������
{
	if(!m_isInited) return;
	glBindBufferBase(GL_UNIFORM_BUFFER,index,m_uboHandle);	
}
void _XPBO::release()
{
	if(m_isInited)
	{
		glDeleteBuffersARB(2,m_pboID);
		m_isInited = XFalse;
	}
}
_XBool _XPBO::init(int w,int h,int px,int py,int wx,int wy,_XColorMode colorType,_XPboMode mode)
{
	if(m_isInited) return XTrue;
	m_nowPBOIndex = 0;
	m_mode = mode;
	m_w = w;
	m_h = h;
	m_px = px;
	m_py = py;
	m_wx = wx;
	m_wy = wy;
	m_colorType = colorType;
	switch(m_colorType)
	{
	case COLOR_RGBA:m_dataSize = m_w * m_h * 4;break;
	case COLOR_RGB:m_dataSize = m_w * m_h * 3;break;
	case COLOR_GRAY:m_dataSize = m_w * m_h;break;
	case COLOR_RGBA_F:m_dataSize = m_w * m_h * 4 * sizeof(float);break;
	case COLOR_GRAY_F:m_dataSize = m_w * m_h * sizeof(float);break;
	}
	glGenBuffersARB(2,m_pboID);
	if(m_mode == PBO_MODE_PACK)
	{
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,m_pboID[0]);
		glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB,m_dataSize,0,GL_STREAM_READ_ARB);	//������԰�һ�����ݣ�����������ݵĻ���opengl���Լ������ڴ�ռ�
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,m_pboID[1]);
		glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB,m_dataSize,0,GL_STREAM_READ_ARB);
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,0);
	}else
	{
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_pboID[0]);
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_dataSize,0,GL_STREAM_DRAW_ARB);	//������԰�һ�����ݣ�����������ݵĻ���opengl���Լ������ڴ�ռ�
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_pboID[1]);
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_dataSize,0,GL_STREAM_DRAW_ARB);
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);
	}

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XPBO::getPixel(unsigned char * buff,int target)
{
	if(!m_isInited) return XFalse;
	if(buff == NULL) return XFalse;
	if(m_mode == PBO_MODE_UNPACK) return XFalse;
	int nextIndex = m_nowPBOIndex;
	m_nowPBOIndex = (m_nowPBOIndex + 1) % 2;

	glReadBuffer(target);
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,m_pboID[m_nowPBOIndex]);
	switch(m_colorType)
	{
	case COLOR_RGBA:glReadPixels(m_px,m_py,m_wx,m_wy,GL_RGBA,GL_UNSIGNED_BYTE,0);break;
	case COLOR_RGB:glReadPixels(m_px,m_py,m_wx,m_wy,GL_RGB,GL_UNSIGNED_BYTE,0);break;
	case COLOR_GRAY:glReadPixels(m_px,m_py,m_wx,m_wy,GL_LUMINANCE,GL_UNSIGNED_BYTE,0);break;
	case COLOR_RGBA_F:glReadPixels(m_px,m_py,m_wx,m_wy,GL_RGBA,GL_FLOAT,0);break;
	case COLOR_GRAY_F:glReadPixels(m_px,m_py,m_wx,m_wy,GL_LUMINANCE,GL_FLOAT,0);break;
	}
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,m_pboID[nextIndex]);
	unsigned char * src = (unsigned char *)glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB,GL_READ_ONLY_ARB);
	if(src != NULL)
	{//�Ѿ���ȡ����
		memcpy(buff,src,m_dataSize);
		glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB);
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,0);
		glDrawBuffer(GL_BACK);
		return XTrue;
	}else
	{
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,0);
		glDrawBuffer(GL_BACK);
		return XFalse;
	}
}
_XBool _XPBO::setPixel(unsigned char * buff)
{
	if(!m_isInited) return XFalse;
	if(buff == NULL) return XFalse;
	if(m_mode == PBO_MODE_PACK) return XFalse;
	//int nextIndex = m_nowPBOIndex;
	m_nowPBOIndex = (m_nowPBOIndex + 1) % 2;

	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_pboID[m_nowPBOIndex]);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_dataSize, 0, GL_STREAM_DRAW_ARB);
    GLubyte * src = (GLubyte *)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
    if(src != NULL)
    {
		memcpy(src,buff,m_dataSize);
        glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer

		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);
		return XTrue;
    }else
	{
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);
		return XFalse;
	}
}
void _XPBO::bind() const
{
	if(!m_isInited) return;
	if(m_mode == PBO_MODE_PACK) glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,m_pboID[m_nowPBOIndex]);
	else glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_pboID[m_nowPBOIndex]); 
}
_XBool _XVBO::init(int size,const float *v,const float *t,const float *n,const float *c,
				   int iSize,const unsigned int *i)
{
	if(m_isInited) return XFalse;
	if(size <= 0) return XFalse;
	if(v == NULL) m_withV = XFalse; else m_withV = XTrue;
	if(t == NULL) m_withT = XFalse; else m_withT = XTrue;
	if(n == NULL) m_withN = XFalse; else m_withN = XTrue;
	if(c == NULL) m_withC = XFalse; else m_withC = XTrue;
	if(i == NULL) m_withI = XFalse; else m_withI = XTrue;
	m_size = size;
	m_indexSize = iSize;

	if(m_withV)
	{
		glGenBuffersARB(1,&m_v);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 3 * sizeof(float),v,GL_STATIC_DRAW_ARB);
	}
	if(m_withT)
	{
		glGenBuffersARB(1,&m_t);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_t);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 2 * sizeof(float),t,GL_STATIC_DRAW_ARB);
	}
	if(m_withN)
	{
		glGenBuffersARB(1,&m_n);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_n);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 3 * sizeof(float),n,GL_STATIC_DRAW_ARB);
	}
	if(m_withC)
	{
		glGenBuffersARB(1,&m_c);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_c);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 4 * sizeof(float),c,GL_STATIC_DRAW_ARB);
	}
	if(m_withI)
	{
		glGenBuffersARB(1,&m_i);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_i);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_indexSize * sizeof(int),i,GL_STATIC_DRAW_ARB);
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//ȡ����

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XVBO::setAsSubject(const _XVBO &temp,_XBool v,_XBool n,_XBool c,_XBool t,_XBool i,_XBool cv,_XBool cn,_XBool cc,_XBool ct,_XBool ci)
{
	if(!temp.m_isInited) return XFalse;	//���Ŀ��û�г�ʼ��������Ϊ����
	//if(m_isInited) release(); //��������Ѿ���ʼ�����������ͷ���Դ
	if(!m_isInited || m_isSubject) return XFalse;	//����Ѿ��д�����ϵ�������ظ����ô�����ϵ
	if(cv && !temp.m_withV) return XFalse;	//Ŀ��û���������
	if(cn && !temp.m_withN) return XFalse;
	if(cc && !temp.m_withC) return XFalse;
	if(ct && !temp.m_withT) return XFalse;
	if(ci && !temp.m_withI) return XFalse;
	//��ʼ���������
	if(v) m_withV = XTrue; else m_withV = XFalse;
	if(t) m_withT = XTrue; else m_withT = XFalse;
	if(n) m_withN = XTrue; else m_withN = XFalse;
	if(c) m_withC = XTrue; else m_withC = XFalse;
	if(i) m_withI = XTrue; else m_withI = XFalse;
	m_size = temp.m_size;
	if(v)
	{
		if(cv) 
		{
			if(m_withV && !m_subjectV) glDeleteBuffersARB(1,&m_v);	//�ͷŵ�ԭ�е���Դ
			m_v = temp.m_v;
			m_subjectV = XTrue;
		}else 
		{
			//glGenBuffersARB(1,&m_v);
			m_subjectV = XFalse;
		}
	}
	if(n)
	{
		if(cn) 
		{
			if(m_withN && !m_subjectN) glDeleteBuffersARB(1,&m_n);	//�ͷŵ�ԭ�е���Դ
			m_n = temp.m_n;
			m_subjectN = XTrue;
		}else
		{
			//glGenBuffersARB(1,&m_n);
			m_subjectN = XFalse;
		}
	}
	if(c)
	{
		if(cc) 
		{
			if(m_withC && !m_subjectC) glDeleteBuffersARB(1,&m_c);	//�ͷŵ�ԭ�е���Դ
			m_c = temp.m_c;
			m_subjectC = XTrue;
		}else
		{
			//glGenBuffersARB(1,&m_c);
			m_subjectC = XFalse;
		}
	}
	if(t)
	{
		if(ct)
		{
			if(m_withT && !m_subjectT) glDeleteBuffersARB(1,&m_t);	//�ͷŵ�ԭ�е���Դ
			m_t = temp.m_t;
			m_subjectT = XTrue;
		}else
		{
			//glGenBuffersARB(1,&m_t);
			m_subjectT = XFalse;
		}
	}
	if(i)
	{
		if(ci)
		{
			m_indexSize = temp.m_indexSize;
			if(m_withI && !m_subjectI) glDeleteBuffersARB(1,&m_i);	//�ͷŵ�ԭ�е���Դ
			m_i = temp.m_i;
			m_subjectI = XTrue;
		}else
		{
			//glGenBuffersARB(1,&m_i);
			m_subjectI = XFalse;
		}
	}
	m_isSubject = XTrue;
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XVBO::updateDate(int size,const float *v,const float *t,const float *n,const float *c,
						 int iSize,const unsigned int *i)
{
	if(!m_isInited)
	{//�����δ���г�ʼ����������ֱ�ӵ��ó�ʼ������
		return init(size,v,t,n,c,iSize,i);
	}
	if(size != m_size || iSize != m_indexSize) return XFalse;

	if(m_withV && v != NULL)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 3 * sizeof(float),v,GL_STATIC_DRAW_ARB);
	}
	if(m_withT && t != NULL)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_t);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 2 * sizeof(float),t,GL_STATIC_DRAW_ARB);
	}
	if(m_withN && n != NULL)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_n);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 3 * sizeof(float),n,GL_STATIC_DRAW_ARB);
	}
	if(m_withC && c != NULL)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_c);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 4 * sizeof(float),c,GL_STATIC_DRAW_ARB);
	}
	if(m_withI && i != NULL)
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_i);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_indexSize * sizeof(float),c,GL_STATIC_DRAW_ARB);
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//ȡ����
	return XTrue;
}
_XBool _XVBO::updateDataV(int size,const float *v)
{
	if(!m_isInited) return XFalse;
	if(size != m_size || !m_withV || v == NULL) return XFalse;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 3 * sizeof(float),v,GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//ȡ����
	return XTrue;
}
_XBool _XVBO::updateDataT(int size,const float *t)
{
	if(!m_isInited) return XFalse;
	if(size != m_size || !m_withT || t == NULL) return XFalse;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_t);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 2 * sizeof(float),t,GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//ȡ����
	return XTrue;
}
_XBool _XVBO::updateDataN(int size,const float *n)
{
	if(!m_isInited) return XFalse;
	if(size != m_size || !m_withN || n == NULL) return XFalse;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_n);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 3 * sizeof(float),n,GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//ȡ����
	return XTrue;
}
_XBool _XVBO::updateDataC(int size,const float *c)
{
	if(!m_isInited) return XFalse;
	if(size != m_size || !m_withC || c == NULL) return XFalse;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_c);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 4 * sizeof(float),c,GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//ȡ����
	return XTrue;
}
_XBool _XVBO::updateDataI(int size,const unsigned int *i)
{
	if(!m_isInited) return XFalse;
	//if(size != m_indexSize || !m_withI || i == NULL) return false;
	if(!m_withI || i == NULL) return XFalse;
	m_indexSize = size;
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_i);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,size * 3 * sizeof(float),i,GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);	//ȡ����
	return XTrue;
}
void _XVBO::use(_XBool withTex)
{
	if(!m_isInited) return;
	if(m_withC)
	{
		glEnableClientState(GL_COLOR_ARRAY);	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_c);
		glColorPointer(4,GL_FLOAT,0,NULL);
	}
	if(m_withT && withTex)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_t);
		glTexCoordPointer(2,GL_FLOAT,0,NULL);
	}
	if(m_withN)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_n);
		glNormalPointer(GL_FLOAT,0,NULL);
	}
	if(m_withV)
	{
		glEnableClientState(GL_VERTEX_ARRAY);	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v);
		glVertexPointer(3,GL_FLOAT,0,NULL);
	}
	if(m_withI)
	{
		glEnableClientState(GL_INDEX_ARRAY);	
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_i);
		glIndexPointer(GL_INT,0,NULL);
	}
}
void _XVBO::disuse()
{
	if(!m_isInited) return;
	if(m_withC) glDisableClientState(GL_COLOR_ARRAY);
	if(m_withT) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if(m_withN) glDisableClientState(GL_NORMAL_ARRAY);
	if(m_withV) glDisableClientState(GL_VERTEX_ARRAY);
	if(m_withI) glDisableClientState(GL_INDEX_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//ȡ����
}
void _XVBO::drawByIndex(unsigned int type,_XBool withTex)
{
	if(!m_withI) return;
	use(withTex);
	glDrawElements(type,m_indexSize,GL_UNSIGNED_INT,0);
	disuse();
}
void _XVBO::drawByIndex(unsigned int type,int size,unsigned int indexType,void *pIndex,_XBool withTex)
{
	use(withTex);
	glDrawElements(type,size,indexType,pIndex);
	disuse();
}
void _XVBO::drawByArray(unsigned int type,_XBool withTex)
{
	use(withTex);
	glDrawArrays(type,0,m_size); 
	disuse();
}
void _XVBO::release()
{
	if(!m_isInited) return;
	if(m_isSubject)
	{
		if(m_withV && !m_subjectV) glDeleteBuffersARB(1,&m_v);
		if(m_withN && !m_subjectN) glDeleteBuffersARB(1,&m_n);
		if(m_withT && !m_subjectT) glDeleteBuffersARB(1,&m_t);
		if(m_withC && !m_subjectC) glDeleteBuffersARB(1,&m_c);
		if(m_withI && !m_subjectI) glDeleteBuffersARB(1,&m_i);
	}else
	{
		if(m_withV) glDeleteBuffersARB(1,&m_v);
		if(m_withN) glDeleteBuffersARB(1,&m_n);
		if(m_withT) glDeleteBuffersARB(1,&m_t);
		if(m_withC) glDeleteBuffersARB(1,&m_c);
		if(m_withI) glDeleteBuffersARB(1,&m_i);
	}
	m_isInited = XFalse;
}
#include "XEffeEngine.h"
_XBool isFBOSupported()
{
	//���FBO�Ƿ��Կ�֧��
	if(glewIsSupported("GL_EXT_framebuffer_object"))
    {
		printf("FBO is supported.\n");
		return XTrue;
    }else
	{
        printf("FBO is not supported.");
		return XFalse;
	}
//	return XFalse;
}
_XBool isFramebufferReady()
{
    //���FBO״̬
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        printf("Framebuffer complete.\n");
        return XTrue;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        printf("[ERROR] Framebuffer incomplete: Attachment is NOT complete.\n");
        return XFalse;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        printf("[ERROR] Framebuffer incomplete: No image is attached to FBO.\n");
        return XFalse;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        printf("[ERROR] Framebuffer incomplete: Attached images have different dimensions.\n");
        return XFalse;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        printf("[ERROR] Framebuffer incomplete: Color attached images have different internal formats.\n");
        return XFalse;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        printf("[ERROR] Framebuffer incomplete: Draw buffer.\n");
        return XFalse;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        printf("[ERROR] Framebuffer incomplete: Read buffer.\n");
        return XFalse;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        printf("[ERROR] Unsupported by FBO implementation.\n");
        return XFalse;
    default:
        printf("[ERROR] Unknow error.\n");
        return XFalse;
    }
}
_XBool _XFBO::init(int w,int h,_XFBOTextureType type)
{
	if(m_nowFBOTextureSum != 0) return XTrue;
	//����Ƿ�֧��FBO
	if(!isFBOSupported()) return XFalse;

	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_maxTextureSum);
	//������ͼ
	addOneTexture(w,h,type);
	m_type = type;
	if(type == TEXTURE_DEEPTH)	//������Ҫ�����Ƿ���Ժϲ������ж�
	{
		glGenFramebuffersEXT(1,&m_fboId);					//����FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_fboId);	//��FBO
		glGenRenderbuffersEXT(1, &m_rboId);					
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,m_rboId);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_RGBA,w,h);

		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT,m_rboId);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D,m_textureId[0],0);
		m_zeroTexIndex = 0;
	}else
	{
		//����һ��framebuffer object�����˳�ʱ��Ҫɾ��
		glGenFramebuffersEXT(1, &m_fboId);					//����FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);	//��FBO
		//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_RGBA,w,h);
		//ʹ�ø����GL_RGBA32F_ARB�����Ա����Ӱ�����˵����⣬��δ���
		//glRenderbufferStorageMultisample(GL_RENDERBUFFER,GL_RGBA32F_ARB,GL_DEPTH_STENCIL,w,h);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,m_fboId);

		//���������Ⱦbuff�����������ڲ���Ҫ��ȼ����Ӧ��ò��Ҳû������
		//glGenRenderbuffersEXT(1, &m_rboId);					
		//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_rboId);
		//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
		//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

		//�ҽ���ͼ
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_textureId[0], 0);
		m_zeroTexIndex = 0;
		//�ҽ������Ⱦ��
		//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT,m_rboId);
	}

    //�����û�йҽ���ͼ�Ļ���ȡ����ɫ����
    //���統��ֻ��Ҫ����ͼ���������Ⱦ��ʱ��
    //����glCheckFramebufferStatusEXT������׼������.
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);

	if(!isFramebufferReady()) return XFalse;

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return XTrue;
}
void _XFBO::useFBO(bool newSize,int w,int h)
{
	if(newSize)
	{
		glMatrixMode(GL_PROJECTION);					//���õ�ǰ����ģʽ ����ͶӰ����Ӧ��֮��ľ��������
		glPushMatrix();
		glLoadIdentity();								//�任����ϵ����
		switch(XEE::windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			glOrtho(0,w,0,h,-1,1);
			glViewport(0,0,w,h);
			break;
		case WINDOW_ROTATE_MODE_90:
			glOrtho(w,0,h,0,-1,1);
			glViewport(0,0,w,h);
			break;
		case WINDOW_ROTATE_MODE_180:
			glOrtho(0,w,0,h,-1,1);
			glViewport(0,0,w,h);
			break;
		case WINDOW_ROTATE_MODE_270:
			glOrtho(w,0,h,0,-1,1);
			glViewport(0,0,w,h);
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		switch(XEE::windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
			glTranslatef(w,h,0);
			glRotatef(180,0,0,1);
			break;
		case WINDOW_ROTATE_MODE_180: 
			break;
		case WINDOW_ROTATE_MODE_270: 
			glTranslatef(w,h,0);
			glRotatef(180,0,0,1);
			break;
		}	
	}else
	{
		glMatrixMode(GL_PROJECTION);					//���õ�ǰ����ģʽ ����ͶӰ����Ӧ��֮��ľ��������
		glPushMatrix();
		glLoadIdentity();								//�任����ϵ����
		switch(XEE::windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			glOrtho(0,XEE::windowData.sceneW,0,XEE::windowData.sceneH,-1,1);
			glViewport(0,0,XEE::windowData.sceneW,XEE::windowData.sceneH);
			break;
		case WINDOW_ROTATE_MODE_90:
			glOrtho(XEE::windowData.sceneW,0,XEE::windowData.sceneH,0,-1,1);
			glViewport(0,0,XEE::windowData.sceneW,XEE::windowData.sceneH);
			break;
		case WINDOW_ROTATE_MODE_180:
			glOrtho(0,XEE::windowData.sceneW,0,XEE::windowData.sceneH,-1,1);
			glViewport(0,0,XEE::windowData.sceneW,XEE::windowData.sceneH);
			break;
		case WINDOW_ROTATE_MODE_270:
			glOrtho(XEE::windowData.sceneW,0,XEE::windowData.sceneH,0,-1,1);
			glViewport(0,0,XEE::windowData.sceneW,XEE::windowData.sceneH);
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		switch(XEE::windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
			glTranslatef(XEE::windowData.sceneW,XEE::windowData.sceneH,0);
			glRotatef(180,0,0,1);
			break;
		case WINDOW_ROTATE_MODE_180: 
			break;
		case WINDOW_ROTATE_MODE_270: 
			glTranslatef(XEE::windowData.sceneW,XEE::windowData.sceneH,0);
			glRotatef(180,0,0,1);
			break;
		}
	}
	bind();
}
void _XFBO::bind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_fboId);
	if(XEE::isMultiSampleSupport == 1)
	{
		glDisable(GL_MULTISAMPLE);
		glDisable(GL_POLYGON_SMOOTH);			//��������������ͼ����
		glDisable(GL_POINT_SMOOTH);		//�������ֿ���ݹ���
		if(XEE::isLineSmooth) glDisable(GL_LINE_SMOOTH);
	}
}
void _XFBO::unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind
	if(XEE::isMultiSampleSupport == 1)
	{
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_POLYGON_SMOOTH);			//��������������ͼ����
		glEnable(GL_POINT_SMOOTH);		//�������ֿ���ݹ���
		if(XEE::isLineSmooth) glEnable(GL_LINE_SMOOTH);
	}
}
void _XFBO::attachTex(int order)
{
	if(m_nowFBOTextureSum > 1 && order >= 0 && order < m_nowFBOTextureSum)
	{//�ҽ���ͼ
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_textureId[order], 0);
		m_zeroTexIndex = order;
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	}
}
//ͬʱ�󶨶��FBO
_XBool _XFBO::attachTexs(int sum,int index,...)
{
	if(sum <= 0) return XFalse;
	if(sum > m_maxTextureSum) return XFalse;
	int indexs[16] = {-1,-1,-1,-1,-1,
						-1,-1,-1,-1,-1,
						-1,-1,-1,-1,-1,-1};	//������ʹ��16����ͼ
	va_list arg_ptr;
	va_start(arg_ptr,index);
	indexs[0] = index;
	for(int i = 1;i < sum;++ i)
	{
		indexs[i] = va_arg(arg_ptr, int);
		if(indexs[i] < 0) return XFalse;
	}
	va_end(arg_ptr);
	//Ȼ��ʼ�ֱ����ͼ
	for(int i = 0;i < sum;++ i)
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT + i,GL_TEXTURE_2D,m_textureId[indexs[i]],0);
	}
	m_zeroTexIndex = indexs[0];
	//m_upTextureSum = sum;
	GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT,GL_COLOR_ATTACHMENT1_EXT,
		GL_COLOR_ATTACHMENT2_EXT,GL_COLOR_ATTACHMENT3_EXT,
		GL_COLOR_ATTACHMENT4_EXT,GL_COLOR_ATTACHMENT5_EXT,
		GL_COLOR_ATTACHMENT6_EXT,GL_COLOR_ATTACHMENT7_EXT,
		GL_COLOR_ATTACHMENT8_EXT,GL_COLOR_ATTACHMENT9_EXT,
		GL_COLOR_ATTACHMENT10_EXT,GL_COLOR_ATTACHMENT11_EXT,
		GL_COLOR_ATTACHMENT12_EXT,GL_COLOR_ATTACHMENT13_EXT,
		GL_COLOR_ATTACHMENT14_EXT,GL_COLOR_ATTACHMENT15_EXT};
	glDrawBuffers(sum,buffers);
	if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
		return XFalse;
	return XTrue;
}
void _XFBO::removeFBO()
{
	unbind();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);					//���õ�ǰ����ģʽ ����ͶӰ����Ӧ��֮��ľ��������
	glPopMatrix();
	switch(XEE::windowData.rotateMode)
	{
	case WINDOW_ROTATE_MODE_0:
		glViewport(0,0,XEE::windowData.w,XEE::windowData.h);
		break;
	case WINDOW_ROTATE_MODE_90:
		glViewport(0,0,XEE::windowData.h,XEE::windowData.w);
		break;
	case WINDOW_ROTATE_MODE_180:
		glViewport(0,0,XEE::windowData.w,XEE::windowData.h);
		break;
	case WINDOW_ROTATE_MODE_270:
		glViewport(0,0,XEE::windowData.h,XEE::windowData.w);
		break;
	}
}
void _XFBO::addOneTexture(int w,int h,_XFBOTextureType type)
{
//	if(type != TEXTURE_RGBA_F)
//	{
		m_w[m_nowFBOTextureSum] = w;
		m_h[m_nowFBOTextureSum] = h;
		glGenTextures(1, &m_textureId[m_nowFBOTextureSum]);
		glBindTexture(GL_TEXTURE_2D, m_textureId[m_nowFBOTextureSum]);
		//Ч����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#if WITH_XSPRITE_EX
		if(type == TEXTURE_DEEPTH)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
#endif	//�ٶȿ�
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		switch(type)
		{
		case TEXTURE_RGBA:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,0);
			break;
		case TEXTURE_RGB:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,0);
			break;
		case TEXTURE_DEEPTH:
			glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,w,h,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE,0);
			break;
		case TEXTURE_RGBA_F:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,w,h,0,GL_RGBA,GL_FLOAT,0);
			break;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	//}else
	//{//��δ��������
	//	glGenTextures(1,&m_textureId[m_nowFBOTextureSum]);
	//	glBindTexture(GL_TEXTURE_2D,m_textureId[m_nowFBOTextureSum]);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
	//	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,GL_DECAL);
	//	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,w,h,0,GL_RGBA,GL_FLOAT,0);
	//	glBindTexture(GL_TEXTURE_2D,0);
	//}
	m_nowFBOTextureSum ++;
}
void _XFBO::release()
{
	if(m_nowFBOTextureSum == 0) return;
	for(int i = 0;i < m_nowFBOTextureSum;++ i)
	{
		if(glIsTexture(m_textureId[i])) 
		{
			printf("delete texture:%d\n",m_textureId[i]);
			glDeleteTextures(1, &m_textureId[i]);
		}
	}

	glDeleteFramebuffersEXT(1, &m_fboId);
	if(m_type == TEXTURE_DEEPTH) glDeleteRenderbuffersEXT(1, &m_rboId);
	m_nowFBOTextureSum = 0;
}
void _XFBO::getPixelFromFboEx(const _XFBOEx& ex)
{
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT,ex.getFboID());
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT,m_fboId);
	glBlitFramebufferEXT(0, 0, getW(), getH(), 0, 0, ex.getW(), ex.getH(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
}
bool _XFBOEx::init(int w,int h,_XFBOTextureType type)
{
	if(m_isInited) return false;
	m_w = w;
	m_h = h;
	unsigned int colorMode = GL_RGBA;
	switch(type)
	{
		case TEXTURE_RGBA:colorMode = GL_RGBA;break;
		case TEXTURE_RGB:colorMode = GL_RGB;break;
		case TEXTURE_DEEPTH:colorMode = GL_DEPTH_COMPONENT;break;
		case TEXTURE_RGBA_F:colorMode = GL_RGBA;break;
	}
	glGenFramebuffersEXT(1,&m_fboId);				//����FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_fboId);  

	glGenRenderbuffersEXT(1,&m_rboId);			//������ȾĿ��
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,m_rboId);	
	glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT,4,colorMode,w,h);  //���ö��ز���
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);  
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_RENDERBUFFER_EXT,m_rboId);//�ҽ���ȾĿ��

	glGenRenderbuffersEXT(1,&m_rbdId);			//���������ȾĿ��
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,m_rbdId);  
	glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT,4,GL_DEPTH_COMPONENT,w,h);    //���ö��ز���
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);  
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER,m_rbdId);  //�ҽ������ȾĿ��

	if(!isFramebufferReady()) return false;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);		//FBO�������

	m_isInited = true;
	return true;
}
void _XFBOEx::useFBO(bool newSize,int w,int h)
{
	if(newSize)
	{
		glMatrixMode(GL_PROJECTION);					//���õ�ǰ����ģʽ ����ͶӰ����Ӧ��֮��ľ��������
		glPushMatrix();
		glLoadIdentity();								//�任����ϵ����
		switch(XEE::windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			glOrtho(0,w,0,h,-1,1);
			glViewport(0,0,w,h);
			break;
		case WINDOW_ROTATE_MODE_90:
			glOrtho(w,0,h,0,-1,1);
			glViewport(0,0,w,h);
			break;
		case WINDOW_ROTATE_MODE_180:
			glOrtho(0,w,0,h,-1,1);
			glViewport(0,0,w,h);
			break;
		case WINDOW_ROTATE_MODE_270:
			glOrtho(w,0,h,0,-1,1);
			glViewport(0,0,w,h);
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		switch(XEE::windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
			glTranslatef(w,h,0);
			glRotatef(180,0,0,1);
			break;
		case WINDOW_ROTATE_MODE_180: 
			break;
		case WINDOW_ROTATE_MODE_270: 
			glTranslatef(w,h,0);
			glRotatef(180,0,0,1);
			break;
		}	
	}else
	{
		glMatrixMode(GL_PROJECTION);					//���õ�ǰ����ģʽ ����ͶӰ����Ӧ��֮��ľ��������
		glPushMatrix();
		glLoadIdentity();								//�任����ϵ����
		switch(XEE::windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			glOrtho(0,XEE::windowData.sceneW,0,XEE::windowData.sceneH,-1,1);
			glViewport(0,0,XEE::windowData.sceneW,XEE::windowData.sceneH);
			break;
		case WINDOW_ROTATE_MODE_90:
			glOrtho(XEE::windowData.sceneW,0,XEE::windowData.sceneH,0,-1,1);
			glViewport(0,0,XEE::windowData.sceneW,XEE::windowData.sceneH);
			break;
		case WINDOW_ROTATE_MODE_180:
			glOrtho(0,XEE::windowData.sceneW,0,XEE::windowData.sceneH,-1,1);
			glViewport(0,0,XEE::windowData.sceneW,XEE::windowData.sceneH);
			break;
		case WINDOW_ROTATE_MODE_270:
			glOrtho(XEE::windowData.sceneW,0,XEE::windowData.sceneH,0,-1,1);
			glViewport(0,0,XEE::windowData.sceneW,XEE::windowData.sceneH);
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		switch(XEE::windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
			glTranslatef(XEE::windowData.sceneW,XEE::windowData.sceneH,0);
			glRotatef(180,0,0,1);
			break;
		case WINDOW_ROTATE_MODE_180: 
			break;
		case WINDOW_ROTATE_MODE_270: 
			glTranslatef(XEE::windowData.sceneW,XEE::windowData.sceneH,0);
			glRotatef(180,0,0,1);
			break;
		}
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_fboId);
}
void _XFBOEx::removeFBO()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);					//���õ�ǰ����ģʽ ����ͶӰ����Ӧ��֮��ľ��������
	glPopMatrix();
	switch(XEE::windowData.rotateMode)
	{
	case WINDOW_ROTATE_MODE_0:
		glViewport(0,0,XEE::windowData.w,XEE::windowData.h);
		break;
	case WINDOW_ROTATE_MODE_90:
		glViewport(0,0,XEE::windowData.h,XEE::windowData.w);
		break;
	case WINDOW_ROTATE_MODE_180:
		glViewport(0,0,XEE::windowData.w,XEE::windowData.h);
		break;
	case WINDOW_ROTATE_MODE_270:
		glViewport(0,0,XEE::windowData.h,XEE::windowData.w);
		break;
	}
}
void _XFBOEx::release()
{
	if(!m_isInited) return;
	glDeleteRenderbuffersEXT(1,&m_fboId);  
	glDeleteRenderbuffersEXT(1,&m_rboId);  
	glDeleteFramebuffersEXT(1,&m_rbdId);  
	m_isInited = false;
}
void drawBlankPlane(int w,int h,unsigned int tex,_XShaderGLSL *pShader)
{
	if(pShader != NULL)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		pShader->useShader();
	}
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(0,0);
		glTexCoord2f(0,1);
		glVertex2f(0,h);
		glTexCoord2f(1,1);
		glVertex2f(w,h);
		glTexCoord2f(1,0);
		glVertex2f(w,0);
	glEnd();
	if(pShader != NULL)
	{
		pShader->disShader();
		glPopAttrib();
	}
}