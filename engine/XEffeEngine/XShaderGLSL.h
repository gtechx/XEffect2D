#ifndef _JIA_XSHADER_GLSL_
#define _JIA_XSHADER_GLSL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//#include "XBasicFun.h"
//#include "XBasicOpenGL.h"
#include <vector>
//
#include "XEngineCommon.h"
namespace XE{
//��������
enum XShaderDataType		//GLSL���ݵ���������
{
	DATA_TYPE_INT,			//����
	DATA_TYPE_FLOAT,		//�����ȸ�����float
	DATA_TYPE_INT_ARRAY,	//1D����
	DATA_TYPE_FLOAT_ARRAY,	//1D������
	DATA_TYPE_2INT_ARRAY,	//2D����
	DATA_TYPE_2FLOAT_ARRAY,	//2D������
	DATA_TYPE_3INT_ARRAY,	//3D����
	DATA_TYPE_3FLOAT_ARRAY,	//3D������
	DATA_TYPE_3FLOAT_MATRIX,//3x3����
	DATA_TYPE_4FLOAT_MATRIX,//4x4����
	DATA_TYPE_2INT,			//����
	DATA_TYPE_3INT,			//����
	DATA_TYPE_2FLOAT,		//�����ȸ�����vec2
	DATA_TYPE_3FLOAT,		//�����ȸ�����vec3
	DATA_TYPE_4FLOAT,		//�����ȸ�����vec4
};
class XShaderDataInfo
{
public:
	int m_handle;				//���ݵľ��
	//char m_name[256];			//���ݵ�����
	XShaderDataType m_type;	//���ݵ�����
	void *m_p;					//���ݵ�ָ��
	int m_size;

	void updateData();	//��������
};
//�����ǹ���UBO�ķ�װ
class XUBO
{
private:
	XBool m_isInited;
	unsigned int m_uboHandle;
public:
	XUBO()
		:m_isInited(XFalse)
		,m_uboHandle(0)
	{}
	~XUBO() {release();}
	void release();
	XBool init(int size,const void * p);
	void useUBO(unsigned int index);	//indexΪ��Ӧ��shader�нṹ������
};
class XShaderUBOInfo
{
public:
	unsigned int m_handle;
	XUBO *m_pUBO;

	void updateData() {m_pUBO->useUBO(m_handle);}
};
enum XShaderTexType
{
	TEX_TYPE_2D,
	TEX_TYPE_CUBE,
};
struct XSaderTexInfo
{
	unsigned int * texID;
	int texHandle;
	XShaderTexType texType;
};
class XShaderUBOData;
class XShaderGLSL
{//Ŀǰ��UBO��֧������
public:
	static const int m_maxShaderGLSLDataSum = 256;	//�������ӵ����ݵ��������
private:
	XBool m_isInited;		//�Ƿ��ʼ��
	XShaderHandle m_shaderHandle;
	std::vector<XShaderDataInfo> m_dataInfos;
	std::vector<XSaderTexInfo> m_texInfos;
	std::vector<XShaderUBOInfo> m_UBOInfos;
	void release();
public:
	void updateData();
	XBool init(const char* vertFile,const char* fragFile,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,const char* geomFile = NULL);
	XBool initFromStr(const char *vStr,const char *fStr,const char * gStr = NULL);
	XBool connectData(const char *name,XShaderDataType type,int size,void *p);//��������
	XBool connectTexture(const char *name,unsigned int * tex,XShaderTexType type = TEX_TYPE_2D);//������ͼ
	void useShader(bool withTex0 = false);	//ʹ��shader,����withTex0�����������Ƿ���Ҫ�󶨵�һ����ͼ��
											//�����Ҫ�󶨵�һ����ͼ������Ϊtrue,��Ϊ���ڹ��õ�һ����ͼ������������������
	void useShaderEx(unsigned int tex0,XShaderTexType type = TEX_TYPE_2D);	//���õ�ʱ��Ŵ����һ����ͼ
	//++++++++++++++++++++++++++++++++++++++++++++++
	//geometryShader��صĽӿ�
	void setGeometryInfo(unsigned int inType,unsigned int outType,int outSum);
	int getGeometryMaxOutputCount() const;
	//----------------------------------------------
	void disShader();
	unsigned int getShaderHandle()const{ return m_shaderHandle.shaderHandle;}
	XShaderGLSL()
		:m_isInited(XFalse)
	{}
	~XShaderGLSL() {release();}
	//������Ϊ���������ܶ����ӵĽӿ�
	XBool connectUBO(const char *uboName,XUBO *ubo);
	//��shader�ж�ȡUBO�������Ϣ
	XBool getUBOInfo(XShaderUBOData &uboData,int valueSum,const char *uboName,const char **valueNames);	//����UBO
};
class XShaderUBOData
{
public:
	unsigned int m_handle;
	char m_name[256];
	int m_offset[XShaderGLSL::m_maxShaderGLSLDataSum];
	unsigned int m_index[XShaderGLSL::m_maxShaderGLSLDataSum];
	int m_size;
};

//ʹ��UBO����ֱ�ӵ�GLSL�����ݴ��ݣ�������Ч��
//�����ǹ���ʹ��UBO������
//GLuint blockIndex = glGetUniformBlockIndex(m_shaderHandle,"BlobSettings");  //��ȡ�ṹ�ı��
//GLint blockSize;  
//glGetActiveUniformBlockiv(m_shaderHandle,  
//                            blockIndex,  
//                            GL_UNIFORM_BLOCK_DATA_SIZE,  
//                            &blockSize);					//��ȡ�ṹ�ĳߴ�
//GLubyte *blockBuffer = (GLubyte*)malloc(blockSize);		//����һ���ڴ�ռ�
//const GLchar *names[] = {"BlobSettings.InnerColor","BlobSettings.OuterColor",  
//                            "BlobSettings.RadiusInner","BlobSettings.RadiusOuter"};  
//GLuint indices[4];  
//glGetUniformIndices(m_shaderHandle, 4, names, indices);  //��ȡÿ�������ı�ǩ
//  
//GLint offset[4];  
//glGetActiveUniformsiv(m_shaderHandle, 4, indices, GL_UNIFORM_OFFSET, offset);	//��ȡÿ��������ƫ��
//  
//GLfloat outerColor[] = {0.0f, 0.0f, 0.0f, 0.0f};  
//GLfloat innerColor[] = {1.0f, 1.0f, 0.75f, 1.0f};  
//GLfloat innerRadius = 0.25f, outerRadius = 0.45f;	//��ʼ����Ӧ�ı���
//  
//memcpy(blockBuffer + offset[0],innerColor,4 * sizeof(float));  
//memcpy(blockBuffer + offset[1],outerColor,4 * sizeof(float));  
//memcpy(blockBuffer + offset[2],&innerRadius,sizeof(float));  
//memcpy(blockBuffer + offset[3],&outerRadius,sizeof(float));		//��������
////create OpenGL buffer UBO to store the data  
//GLuint uboHandle;  
//glGenBuffers(1,&uboHandle);  
//glBindBuffer(GL_UNIFORM_BUFFER,uboHandle);  
//glBufferData(GL_UNIFORM_BUFFER,blockSize,blockBuffer,GL_DYNAMIC_DRAW);			//����UBO                   
////bind the UBO th the uniform block  
//glBindBufferBase(GL_UNIFORM_BUFFER,blockIndex,uboHandle);		//��UBO
//free(blockBuffer);												//�ͷ���ʱ���ڴ�ռ�
////GLSL�еĴ���
//layout(std140) uniform BlobSettings
//{  
//    vec4 InnerColor;  
//    vec4 OuterColor;  
//    float RadiusInner;  
//    float RadiusOuter;  
//}Blob;//Use an instance name with a uniform block  
//����ʹ��PBO�ķ�ʽ��ȡ��Ļ������������������
#ifndef GL_FRONT
#define GL_FRONT (0x4040)
#endif
enum XPboMode
{
	PBO_MODE_UNPACK,
	PBO_MODE_PACK,
};
enum XColorMode;
//˵������ʹ��PBO��������ͼ��ʱ��ֻ�е���Ҫ��RGBת����BGR��ʱ��������м�ֵ������������
//������ʽ��ȫû�б�Ҫ
class XPBO	
{
private:
	XBool m_isInited;
	unsigned int m_pboID[2];	//ʹ����ż�ֻ��ķ�ʽ
	int m_curPBOIndex;
	int m_size;
	XColorMode m_colorType;
	int m_w;	//��Ļ���
	int m_h;	//��Ļ�߶�
	int m_px;	//��ȡͼ������Ͻǵ�
	int m_py;
	int m_wx;	//��ȡͼ��Ŀ��
	int m_wy;
	int m_dataSize;
	XPboMode m_mode;
public:
	XPBO()
		:m_isInited(XFalse)
		,m_curPBOIndex(0)
		,m_mode(PBO_MODE_PACK)
	{}
	~XPBO(){release();}
	void release();
	XBool init(int w,int h,int px,int py,int wx,int wy,XColorMode colorType,XPboMode mode = PBO_MODE_PACK);
	XBool getPixel(unsigned char * buff,int target = GL_FRONT);
	XBool setPixel(unsigned char * buff);
	void bind() const;
	void unbind() const;
};
////++++++++++++++++++++++++++++++++++++++++++++++
////�����ǹ���PBO��
////����
//GLuint pboID;
//unsigned char *dataBuff;
//glGenBuffersARB(1,&pboID);
//glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,pboID);
//glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB,1280 * 720 * 3,0,GL_STREAM_READ_ARB);
//glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,0);
////ʹ��
//glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,pboID);
//glReadPixels(0,0,1280,720,GL_RGB,GL_UNSIGNED_BYTE,0);
////glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, pboIds[nextIndex]); //��֪������Ǹ�ɶ��
//GLubyte* src = (GLubyte*)glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB,GL_READ_ONLY_ARB);
//if(src)
//{//�Ѿ���ȡ����
//	//���ｫ����ȡ��
//	memcpy(dataBuff,src,1280 * 720 * 3);
//	glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB);     // release pointer to the mapped buffer
//}
//glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,0);
////�ͷ�
//glDeleteBuffersARB(1,&pboID);
////--------------------------------------------------
//����ʹ��VBO�ķ�ʽ�������ܣ����vbo��ʾ�����������л���releaseģʽ����һ�ԣ�
//VBOʹ�ü���
//1��̫Ƶ���ĸ���VBO������Ӱ�����ܣ�����˵������ϵͳ�У�ÿ�����Ӿ���һ��VBO�Ļ���
//Ƶ���ĸ���VBO����������ܴ���ۿۣ���ʱӦ�ý��кϲ���������С��vbo�ϲ��ɴ��VBO(�ϲ��Ƿ����������������δ��������)
//2�����������Ƚ�С��ʱ��BufferSubData��ԶԶ����MapBuffer��Ч�ʵģ����ǵ������㹻���ʱ��
//MapBuffer��ѹ���Եĳ�ԽBufferSubData���ݲ������ֵ�����32k�����廹�Ǹ���������������(��δ������֤)
class XVBO
{
private:
	XBool m_withVAO;	//ʹ��ʹ��VAO(ʹ��VAOû��������������������������Ա�ø����)
	XBool m_isInited;
	XBool m_withV;	//����
	XBool m_withT;	//��ͼ
	XBool m_withN;	//����
	XBool m_withC;	//��ɫ
	XBool m_withI;	//����
	unsigned int m_vao;
	unsigned int m_v;
	unsigned int m_t;
	unsigned int m_n;
	unsigned int m_c;
	unsigned int m_i;
	int m_size;			//VBO�����ݵ�������һ���Ƕ��������
	int m_indexSize;	//������������һ����˵����ģ����Ⱦ����������Ķ��������
	int m_vSize;	//����һ�����������������3D����Ϊ3��2D����Ϊ2
	XBool initX(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL,bool withVAO = false);
	XBool updateDateX(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL);
public:
	//sizeΪ�����������ʵ�ʵ�v�ĳߴ�Ϊsize * 3����Ϊ3D�ռ���һ��������3��������ȷ��
	//t�ĳߴ�Ϊsize * 2,n�ĳߴ�Ϊsize * 3,c�ĳߴ�Ϊsize * 4��
	//iSize Ϊ������������(��������Ĭ��Ϊ������)������i�ĳߴ�ΪiSize * 3	//����Լ�������⣬��Ҫ����
	XBool init(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL,bool withVAO = false)
	{
		m_vSize = 3;
		return initX(size,v,t,n,c,iSize,i,withVAO);
	}
	XBool updateDate(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL)
	{
		if(!m_isInited) return init(size,v,t,n,c,iSize,i);//�����δ���г�ʼ����������ֱ�ӵ��ó�ʼ������
		return updateDateX(size,v,t,n,c,iSize,i);
	}
	XBool init2D(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL,bool withVAO = false)
	{
		m_vSize = 2;
		return initX(size,v,t,n,c,iSize,i,withVAO);
	}
	XBool updateDate2D(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL)
	{
		if(!m_isInited) return init2D(size,v,t,n,c,iSize,i);//�����δ���г�ʼ����������ֱ�ӵ��ó�ʼ������
		return updateDateX(size,v,t,n,c,iSize,i);
	}
	XBool updateDataV(int size,const float *v);	//sizeΪ�����������Ĭ��һ��������3������������
	XBool updateDataT(int size,const float *t);	//sizeΪ�����������Ĭ��һ�������t��2������������
	XBool updateDataN(int size,const float *n);	//sizeΪ�����������Ĭ��һ�������n��3������������
	XBool updateDataC(int size,const float *c);	//sizeΪ�����������Ĭ��һ�������c��4������������
	XBool updateDataI(int size,const unsigned int *i);	//sizeΪ������������
	void use(XBool withTex = XTrue);	//�������������Ƿ���ʾ��ͼ
	void disuse();	//ȡ������
	void drawByIndex(unsigned int type,XBool withTex = XTrue);
	void drawByIndex(unsigned int type,int size,unsigned int indexType,void *pIndex,XBool withTex = XTrue);
	void drawByArray(unsigned int type,XBool withTex = XTrue);
	void drawByArray(unsigned int type,int index,int size,XBool withTex);
	void release();
	int getSize()const{return m_size;}
	int getIndexSize()const{return m_indexSize;}
	//��һ��VBO�е���������һ��VBO������ͬʱ������Ҫʹ���������ݣ���������һ��VBO��������һ��VBO������ֱ�Ӵ���һ��VBO�л�ȡ����
	//temp:�����Ķ���
	//v,n,c,t:�Ƿ�����Щ����
	//cv,cn,cc,ct:�����Ƿ�Ӵ��������л�ȡ
	XBool setAsSubject(const XVBO &temp,XBool v,XBool n,XBool c,XBool t,XBool i,XBool cv,XBool cn,XBool cc,XBool ct,XBool ci);
	XBool m_isSubject;
	XBool m_subjectV;
	XBool m_subjectN;
	XBool m_subjectC;
	XBool m_subjectT;
	XBool m_subjectI;

	XVBO()
		:m_isInited(XFalse)
		,m_withV(XFalse)
		,m_withT(XFalse)
		,m_withN(XFalse)
		,m_withC(XFalse)
		,m_isSubject(XFalse)
		,m_v(0)
		,m_n(0)
		,m_t(0)
		,m_c(0)
		,m_i(0)
		,m_vSize(3)
		,m_withVAO(false)
	{}
	~XVBO(){release();}
};
//VBO��ʹ�÷�������
//tmpVBO.use();	//����VBO
//��һ���������������ͺ�OpenGL֧�ֵ�����һ��
//�ڶ�������Ϊģ������������ֵ������
//����������Ϊ����ֵ������
//���ĸ�����Ϊ����ֵ��ָ��
//���VBO���Ѿ������������ݣ����������ΪNULL
//Ŀǰ��VBO��װ��װ��Ĭ��֧�ֵ���淽ʽΪ�����Σ����Զ��ڷ������λ�������
//glDrawElements(GL_QUADS,150 * 150 * 4,GL_UNSIGNED_INT,index);	//��涯��
//tmpVBO.disuse();	//ֹͣVBO��ʹ��

namespace XGL
{
	extern XBool getIsFramebufferReady();	//�ж�FBO״̬�Ƿ����
}
//�����Ƕ�FBO��֧��
class XFBOEx;
class XFBO
{
private:
	unsigned int m_fboId;					//FBO��ID
	unsigned int m_rboId;					//RBO��ID
	std::vector<XTexture *> m_pTextures;
	XColorMode m_type;
	int m_zeroTexIndex;	//����0��λ�õ���ͼ���
public:
	XBool init(int w,int h,XColorMode type = COLOR_RGBA); //w��h������ĳߴ磬���س�ʼ���Ƿ�ɹ�
	void useFBO(bool newSize = false,int w = 0,int h = 0);	//����Ϊ�Ƿ�ʹ���µ��ӿھ���
	void bind();	//���3D����������װһ��
	void attachTex(unsigned int order = 0);	//��Ҫ��useFBO֮�����
	XBool attachTexs(int sum,int index,...);	//��Ҫ��useFBO֮�����
	void removeFBO();
	void unbind();
	void addOneTexture(int w,int h,XColorMode type = COLOR_RGBA);
	void updateTexture(void *p,unsigned int index);
	unsigned int getTexture(unsigned int order) const;	//��������
	int getWidth(unsigned int order) const;
	int getHeight(unsigned int order) const;
	void release();
	XFBO()
		:m_zeroTexIndex(-1)
		, m_fboId(0)
		, m_rboId(0)
		, m_type(COLOR_RGBA)
	{}
	~XFBO() {release();}
	//���������ڲ��ԵĽӿ�
	unsigned int getFboID() const {return m_fboId;}
	void getPixelFromFboEx(const XFBOEx& ex);
	int getW() const;
	int getH() const;
};
//�����֧�ֶ��ز�����FBO��ʵ��
//����fbo�Ƚ�������÷�����������֮ǰ��FBO��װ�Ѿ��õıȽ�˳�֣���������������ķ�װ
class XFBOEx
{
private:
	unsigned int m_fboId;
	unsigned int m_rboId;
	unsigned int m_rbdId;
	int m_w;
	int m_h;

	bool m_isInited;
public:
	bool init(int w,int h,XColorMode type = COLOR_RGBA);
	void useFBO(bool newSize = false,int w = 0,int h = 0);	//����Ϊ�Ƿ�ʹ���µ��ӿھ���
	void removeFBO();
	XFBOEx()
		:m_isInited(false)
		, m_fboId(0)
		, m_rboId(0)
		, m_rbdId(0)
		, m_w(0)
		, m_h(0)
	{}
	~XFBOEx(){release();}
	void release();
	unsigned int getFboID() const {return m_fboId;}
	int getW() const{return m_w;}
	int getH() const{return m_h;}
};
//Ϊ�˽��shader��FBO�Ľ�϶�����ĺ���
namespace XRender
{
extern void drawBlankPlane(int w,int h,unsigned int tex,XShaderGLSL *pShader = NULL);
}
#if WITH_INLINE_FILE
#include "XShaderGLSL.inl"
#endif
}
#endif //_JIA_XSHADER_GLSL_
