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
//UBO�Ĵ�С��������Ϊ16k - 128k
//�����Ҫ���ڴ棬����Կ���SSBO���Ե�128M��С
//https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
class XUBO
{
private:
	XBool m_isInited;
	unsigned int m_uboHandle;
public:
	XUBO()
		:m_isInited(XFalse)
		, m_uboHandle(0)
	{}
	~XUBO() { release(); }
	void release();
	XBool init(int size, const void * p);
	void useUBO(unsigned int index);	//indexΪ��Ӧ��shader�нṹ������
};
//���Դ洢128M��С������
class XSSBO
{
private:
	XBool m_isInited;
	unsigned int m_ssboHandle;
	int m_coreSum;
	int m_size;
public:
	XSSBO()
		:m_isInited(XFalse)
		, m_ssboHandle(0)
		, m_coreSum(0)
		, m_size(0)
	{}
	~XSSBO() { release(); }
	void release();
	XBool init(int size, int sum, const void * p);
	void useSSBO(unsigned int index);	//indexΪ��Ӧ��shader�нṹ������
	void unuseSSBO();	//indexΪ��Ӧ��shader�нṹ������
	void* getBuff(void* buff)
	{
		glBindBufferARB(GL_SHADER_STORAGE_BUFFER, m_ssboHandle);
		memcpy(buff, glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY), m_size);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
	void* getBuff(void* buff,int size)
	{
		glBindBufferARB(GL_SHADER_STORAGE_BUFFER, m_ssboHandle);
		if(size <= m_size)
			memcpy(buff, glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY), size);
		else
			memcpy(buff, glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY), m_size);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
	void setBuff(void* buff)
	{
		glBindBufferARB(GL_SHADER_STORAGE_BUFFER, m_ssboHandle);
		memcpy(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY), buff, m_size);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
	void setBuff(void* buff,int size)
	{
		glBindBufferARB(GL_SHADER_STORAGE_BUFFER, m_ssboHandle);
		if (size <= m_size)
			memcpy(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY), buff, size);
		else
			memcpy(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY), buff, m_size);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
};
class XGLBuffer
{//Ŀǰֻ֧��float���Ժ�������չ
private:
	XBool m_isInited;
	unsigned int m_buffHandle;
	int m_useIndex;
public:
	XGLBuffer()
		:m_isInited(XFalse)
		, m_buffHandle(0)
		, m_useIndex(-1)
	{}
	~XGLBuffer() { release(); }
	void release();
	XBool init(int size, const float * p);
	//index:0 - 15,��16������
	//coreSize 1-4;float:1,vec2:2,vec3:3,vec4:4
	void useBuff(unsigned int index, int coreSize);	//indexΪshader�����ݵ������ο�����
	void removeBuff();
};
class XShaderUBOInfo
{
public:
	unsigned int m_handle;
	XUBO *m_pUBO;

	void updateData()
	{
		if (m_pUBO == NULL) return;
		m_pUBO->useUBO(m_handle);
	}
};
enum XShaderTexType
{
	TEX_TYPE_2D,
	TEX_TYPE_CUBE,
};
struct XShaderTexInfo
{
	unsigned int * texID;
	int texHandle;
	XShaderTexType texType;
};
class XShaderUBOData;
enum defaultAttributes {
	POS_ATT = 0,  // tig: was =1, and BOY, what a performance hog this was!!! see: http://www.chromium.org/nativeclient/how-tos/3d-tips-and-best-practices
	TEX_ATT,
	NOR_ATT,
	COL_ATT,
};
class XShaderGLSL
{//Ŀǰ��UBO��֧������
public:
	static const int m_maxShaderGLSLDataSum = 256;	//�������ӵ����ݵ��������
private:
	XBool m_isInited;		//�Ƿ��ʼ��
	XShaderHandle m_shaderHandle;
	std::vector<XShaderDataInfo> m_dataInfos;
	std::vector<XShaderTexInfo> m_texInfos;
	std::vector<XShaderUBOInfo> m_UBOInfos;
	void release();
	void bindTex(int index);	//�ڰ�shader��ʱ�����ͼ
public:
	void updateData();
	XBool init(const char* vertFile, const char* fragFile = NULL,
		XResPos resPos = RES_SYS_DEF, const char* geomFile = NULL);
	XBool initFromStr(const char *vStr, const char *fStr = NULL, const char * gStr = NULL);
	XBool connectData(const char *name, XShaderDataType type, int size, void *p);//��������
	XBool connectTexture(const char *name, unsigned int * tex, XShaderTexType type = TEX_TYPE_2D);//������ͼ
	void useShader(bool withTex0 = false);	//ʹ��shader,����withTex0�����������Ƿ���Ҫ�󶨵�һ����ͼ��
											//�����Ҫ�󶨵�һ����ͼ������Ϊtrue,��Ϊ���ڹ��õ�һ����ͼ������������������
	void useShaderEx(unsigned int tex0, XShaderTexType type = TEX_TYPE_2D);	//���õ�ʱ��Ŵ����һ����ͼ
	//++++++++++++++++++++++++++++++++++++++++++++++
	//geometryShader��صĽӿ�
	void setGeometryInfo(unsigned int inType, unsigned int outType, int outSum);
	int getGeometryMaxOutputCount() const;
	//----------------------------------------------
	void disShader();
	unsigned int getShaderHandle()const { return m_shaderHandle.shaderHandle; }
	XShaderGLSL()
		:m_isInited(XFalse)
	{}
	~XShaderGLSL() { release(); }
	//������Ϊ���������ܶ����ӵĽӿ�
	XBool connectUBO(const char *uboName, XUBO *ubo);
	//��shader�ж�ȡUBO�������Ϣ
	XBool getUBOInfo(XShaderUBOData &uboData, int valueSum, 
		const char *uboName, const char **valueNames);	//����UBO
	bool bindDefaults()
	{
		if (!m_isInited) return false;
		glBindAttribLocation(m_shaderHandle.shaderHandle, POS_ATT, "position");
		glBindAttribLocation(m_shaderHandle.shaderHandle, COL_ATT, "color");
		glBindAttribLocation(m_shaderHandle.shaderHandle, NOR_ATT, "normal");
		glBindAttribLocation(m_shaderHandle.shaderHandle, TEX_ATT, "texcoord");
		return true;
	}
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
		, m_curPBOIndex(0)
		, m_mode(PBO_MODE_PACK)
	{}
	~XPBO() { release(); }
	void release();
	XBool init(int w, int h, int px, int py, int wx, int wy, 
		XColorMode colorType, XPboMode mode = PBO_MODE_PACK);
	XBool getPixel(unsigned char * buff, int target = GL_FRONT);
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
	XBool m_withVAO;	//ʹ��VAO(ʹ��VAOû��������������������������Ա�ø����)
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
	int m_curISize;	//��ǰ������������m_indexSizeΪ���I������ʵ�ʿ��ܱ�û��ȫ��ʹ�õ�
	int m_vSize;	//����һ�����������������3D����Ϊ3��2D����Ϊ2
	XBool initX(int size, const float *v, const float *t = NULL, 
		const float *n = NULL, const float *c = NULL,
		int iSize = 0, const unsigned int *i = NULL, bool withVAO = false);
	XBool updateDateX(int size, const float *v, const float *t = NULL,
		const float *n = NULL, const float *c = NULL,
		int iSize = 0, const unsigned int *i = NULL);
public:
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����Է�װ���и��º�����
	XBool create();
	//------------------------------------------------------
	unsigned int getVertexID()const { return m_v; }
	//sizeΪ�����������ʵ�ʵ�v�ĳߴ�Ϊsize * 3����Ϊ3D�ռ���һ��������3��������ȷ��
	//t�ĳߴ�Ϊsize * 2,n�ĳߴ�Ϊsize * 3,c�ĳߴ�Ϊsize * 4��
	//iSize Ϊ������������(��������Ĭ��Ϊ������)������i�ĳߴ�ΪiSize * 3	//����Լ�������⣬��Ҫ����
	XBool init(int size, const float *v, const float *t = NULL,
		const float *n = NULL, const float *c = NULL,
		int iSize = 0, const unsigned int *i = NULL, bool withVAO = false)
	{
		m_vSize = 3;
		return initX(size, v, t, n, c, iSize, i, withVAO);
	}
	XBool updateDate(int size, const float *v, const float *t = NULL, 
		const float *n = NULL, const float *c = NULL,
		int iSize = 0, const unsigned int *i = NULL)
	{
		if (!m_isInited) return init(size, v, t, n, c, iSize, i);//�����δ���г�ʼ����������ֱ�ӵ��ó�ʼ������
		return updateDateX(size, v, t, n, c, iSize, i);
	}
	XBool init2D(int size, const float *v, const float *t = NULL, 
		const float *n = NULL, const float *c = NULL,
		int iSize = 0, const unsigned int *i = NULL, bool withVAO = false)
	{
		m_vSize = 2;
		return initX(size, v, t, n, c, iSize, i, withVAO);
	}
	XBool updateDate2D(int size, const float *v, const float *t = NULL,
		const float *n = NULL, const float *c = NULL,
		int iSize = 0, const unsigned int *i = NULL)
	{
		if (!m_isInited) return init2D(size, v, t, n, c, iSize, i);//�����δ���г�ʼ����������ֱ�ӵ��ó�ʼ������
		return updateDateX(size, v, t, n, c, iSize, i);
	}
	bool getIsInited()const { return m_isInited; }
	XBool updateDataV(int size, const float *v);	//sizeΪ�����������Ĭ��һ��������3������������
	XBool updateDataT(int size, const float *t);	//sizeΪ�����������Ĭ��һ�������t��2������������
	XBool updateDataN(int size, const float *n);	//sizeΪ�����������Ĭ��һ�������n��3������������
	XBool updateDataC(int size, const float *c);	//sizeΪ�����������Ĭ��һ�������c��4������������
	XBool updateDataI(int size, const unsigned int *i);	//sizeΪ������������
	void use(XBool withTex = XTrue);	//�������������Ƿ���ʾ��ͼ
	void disuse();	//ȡ������
	void drawByIndex(unsigned int type, XBool withTex = XTrue);
	void drawByIndex(unsigned int type, int size, unsigned int indexType,
		void *pIndex, XBool withTex = XTrue);
	void drawByArray(unsigned int type, XBool withTex = XTrue);
	void drawByArray(unsigned int type, int index, int size, XBool withTex);
	void release();
	int getSize()const { return m_size; }
	int getIndexSize()const { return m_indexSize; }
	int getCurIndexSize()const { return m_curISize; }
	//��һ��VBO�е���������һ��VBO������ͬʱ������Ҫʹ���������ݣ���������һ��VBO��������һ��VBO������ֱ�Ӵ���һ��VBO�л�ȡ����
	//temp:�����Ķ���
	//v,n,c,t:�Ƿ�����Щ����
	//cv,cn,cc,ct:�����Ƿ�Ӵ��������л�ȡ
	XBool setAsSubject(const XVBO &temp, XBool v, XBool n, XBool c, XBool t, 
		XBool i, XBool cv, XBool cn, XBool cc, XBool ct, XBool ci);
	XBool m_isSubject;
	XBool m_subjectV;
	XBool m_subjectN;
	XBool m_subjectC;
	XBool m_subjectT;
	XBool m_subjectI;

	XVBO()
		:m_isInited(XFalse)
		, m_withV(XFalse)
		, m_withT(XFalse)
		, m_withN(XFalse)
		, m_withC(XFalse)
		, m_isSubject(XFalse)
		, m_v(0)
		, m_n(0)
		, m_t(0)
		, m_c(0)
		, m_i(0)
		, m_withVAO(false)
		, m_size(0)
		, m_indexSize(0)
		, m_curISize(0)
		, m_vSize(3)
	{}
	~XVBO() { release(); }
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
	extern XBool getIsFramebufferReadyN();	//�ж�FBO״̬�Ƿ����
}
//Ϊ�˷�ֹFBO��Ƕ�׳������⣬��������һ������ƣ������ƻ�����߼����Ӷȼ�������ӣ���Ҫ��ϸ���
class XFboBase;
enum XFBOOpertion
{
	OPER_USEFBO,
	OPER_REMOVEFBO,
	OPER_BIND,
	OPER_UNBIND,
	OPER_ATTACHTEX,	//��һ����ͼ
	OPER_ATTACHTEXS,//�󶨶�����ͼ
};
class XFBOInfo
{
public:
	XFboBase *pClass;
	bool isNewSize;
	int width;
	int height;
	XFBOOpertion opertion;
	std::vector<int> texIndexs;
	XFBOInfo(XFboBase *p, XFBOOpertion op, bool isNew = false, int w = 0, int h = 0)
		:pClass(p)
		, isNewSize(isNew)
		, width(w)
		, height(h)
		, opertion(op)
	{}
	void doIt();	//���иò�����
};
class XFboBase
{
protected:
	friend XFBOInfo;
	static std::vector<XFBOInfo> m_fboBuffer;	//FBO���õĶ���,��������ɶ��̰߳�ȫ����
	virtual void _useFBO(bool newSize = false,int w = 0,int h = 0) = 0;
	virtual void _removeFBO() = 0;
	virtual void _bind() = 0;
	virtual void _unbind() = 0;
	virtual XBool _attachTex(unsigned int order = 0) = 0;
	virtual XBool _attachTexs(const std::vector<int> &indexs) = 0;
	//����ʹ��������غ���
	virtual void opertion(XFBOOpertion op,bool isNew = false,int w = 0,int h = 0,const std::vector<int> *indexs = NULL);
	virtual void resumeBuff();	//����buff���ظ��ֳ�
	virtual void reset();	//�ָ���һ�β����ĳ�������Ҫִ����β���ʱ�������һ�εĲ����Ѿ��ı���״̬����Ҫ�ظ���һ�β�����״̬
};
//�����Ƕ�FBO��֧��
//#define FBO_WITH_EXT	//�Ƿ����ü�ǿ��Ľӿ�
class XFBOEx;
class XFBO :public XFboBase
{
private:
	unsigned int m_fboId;					//FBO��ID
	unsigned int m_rboId;					//RBO��ID
	std::vector<XTexture *> m_pTextures;
	XColorMode m_type;
	int m_zeroTexIndex;	//����0��λ�õ���ͼ���
	void _bind();
	void _unbind();
	void _useFBO(bool newSize = false, int w = 0, int h = 0);	//����Ϊ�Ƿ�ʹ���µ��ӿھ���
	XBool _attachTex(unsigned int order = 0);	//��Ҫ��useFBO֮�����
	XBool _attachTexs(const std::vector<int> &indexs);	//��Ҫ��useFBO֮�����
	void _removeFBO();
	int m_MSSum;	//���ز�����
public:
	//֧�ֶ��ز�����������ֱ�����ڻ���
	//���ַ���ʵ�ֵĶ������������������
	//�ڲ����Կ��Ϻ���ֻ֧��RGB��RGBA�ĸ�ʽ
	XBool init(int w, int h, XColorMode type = COLOR_RGBA, bool withRBO = false,
		int filter = GL_LINEAR, int MSSum = 0); //w��h������ĳߴ磬���س�ʼ���Ƿ�ɹ�,withDepth�Ƿ���Ҫ��ȼ��
	void useFBO(bool newSize = false, int w = 0, int h = 0)	//����Ϊ�Ƿ�ʹ���µ��ӿھ���
	{
		_useFBO(newSize, w, h);
		opertion(OPER_USEFBO, newSize, w, h);
	}
	void bind();	//���3D����������װһ��
	void attachTex(unsigned int order = 0)	//��Ҫ��useFBO֮�����
	{
		if (_attachTex(order))
		{
			//std::vector<int> tmp;
			//tmp.push_back(order);
			//opertion(OPER_ATTACHTEX,false,0,0,&tmp);
			opertion(OPER_ATTACHTEX, false, order);
		}
	}
	XBool attachTexs(int sum, int index, ...);	//��Ҫ��useFBO֮�����
	XBool attachTexs(const std::vector<int> &indexs)	//��Ҫ��useFBO֮�����
	{
		if (_attachTexs(indexs))
		{
			opertion(OPER_ATTACHTEXS, false, 0, 0, &indexs);
			return true;
		}
		return false;
	}
	void removeFBO()
	{
		_removeFBO();
		opertion(OPER_REMOVEFBO);
	}
	void unbind();
	void addOneTexture(int w, int h, XColorMode type = COLOR_RGBA, int filter = GL_LINEAR);
	void updateTexture(void *p, unsigned int index);
	unsigned int getTexture(unsigned int order = 0) const;	//��������
	XTexture *getTexturePtr(unsigned int order = 0);
	int getWidth(unsigned int order) const;
	int getHeight(unsigned int order) const;
	XColorMode getColorMode(unsigned int order) const;
	void release();
	XFBO()
		:m_zeroTexIndex(-1)
		, m_fboId(0)
		, m_rboId(0)
		, m_type(COLOR_RGBA)
		, m_MSSum(0)
	{}
	~XFBO() { release(); }
	//���������ڲ��ԵĽӿ�
	unsigned int getFboID() const { return m_fboId; }
	void getPixelFromFboEx(XFBOEx& ex, int dstIndex = 0);
	void getPixelFromFbo(XFBO& srcFbo, int srcIndex = 0, int dstIndex = 0);
	int getW() const;
	int getH() const;
	//��ʹ�ö��ز���ʱ������ĺ�������ڷ������⣬��Ҫͨ��һ���Ƕ��ز�����FBO���������ݵ�һ�£��Ա��ڽ������
	//ͨ������getPixelFromFbo�����
	void draw(unsigned int index = 0, bool verticalFlip = false);
};
//�����֧�ֶ��ز�����FBO��ʵ��
//����fbo�Ƚ�������÷�����������֮ǰ��FBO��װ�Ѿ��õıȽ�˳�֣���������������ķ�װ
class XFBOEx :public XFboBase
{
private:
	unsigned int m_fboId;
	unsigned int m_rboId;
	unsigned int m_rbdId;
	int m_w;
	int m_h;

	bool m_isInited;
#ifdef FBO_WITH_EXT
	void _bind() { glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId); }
	void _unbind() { glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); }
#else
	void _bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_fboId); }
	void _unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
#endif
	void _useFBO(bool newSize = false, int w = 0, int h = 0);	//����Ϊ�Ƿ�ʹ���µ��ӿھ���
	void _removeFBO();
	XBool _attachTex(unsigned int order = 0) { return XTrue; }	//do nothing
	XBool _attachTexs(const std::vector<int> &indexs) { return XTrue; }	//do nothing
public:
	bool init(int w, int h, XColorMode type = COLOR_RGBA, int MSSum = 4);
	void useFBO(bool newSize = false, int w = 0, int h = 0)	//����Ϊ�Ƿ�ʹ���µ��ӿھ���
	{
		_useFBO(newSize, w, h);
		opertion(OPER_USEFBO, newSize, w, h);
	}
	void removeFBO()
	{
		_removeFBO();
		opertion(OPER_REMOVEFBO);
	}
	void bind()//do nothing
	{
		reset();
		_bind();
		opertion(OPER_BIND);
	}
	void unbind()//do nothing
	{
		_unbind();
		opertion(OPER_UNBIND);
	}

	XFBOEx()
		:m_isInited(false)
		, m_fboId(0)
		, m_rboId(0)
		, m_rbdId(0)
		, m_w(0)
		, m_h(0)
	{}
	~XFBOEx() { release(); }
	void release();
	unsigned int getFboID() const { return m_fboId; }
	int getW() const { return m_w; }
	int getH() const { return m_h; }
};
//Ϊ�˽��shader��FBO�Ľ�϶�����ĺ���
namespace XRender
{
extern void drawBlankPlane(const XVec2& p, const XVec2& s, unsigned int tex, XShaderGLSL *pShader = NULL,
	const XFColor&color = XFColor::white);
extern void drawBlankPlanePlus(const XVec2& p, const XVec2& s, unsigned int tex, XShaderGLSL *pShader = NULL,
	const XFColor&color = XFColor::white);
inline void drawBlankPlane(const XVec2& s, unsigned int tex, XShaderGLSL *pShader = NULL,
	const XFColor&color = XFColor::white)
{
	drawBlankPlane(XVec2::zero, s, tex, pShader, color);
}
inline void drawBlankPlane(float w, float h, unsigned int tex, XShaderGLSL *pShader = NULL,
	const XFColor&color = XFColor::white)
{
	drawBlankPlane(XVec2(w, h), tex, pShader, color);
}
inline void drawBlankPlane(float x, float y, float w, float h, unsigned int tex, XShaderGLSL *pShader = NULL,
	const XFColor&color = XFColor::white)
{
	drawBlankPlane(XVec2(x, y), XVec2(w, h), tex, pShader, color);
}
//posΪ�ĸ��ǵĶ���λ�ã���ʱ�뷽��
//0  3
//1  2
extern void drawBlankPlane(const std::vector<XVec2>& pos, unsigned int tex, 
	XShaderGLSL* pShader = NULL, const XFColor&color = XFColor::white);
//���Խ��в�ֵ
//0  3
//1  2
extern void drawBlankPlaneEx(const std::vector<XVec2>& pos, unsigned int tex, 
	XShaderGLSL* pShader = NULL, const XFColor& color = XFColor::white, int subSum = 1);
}
#if WITH_INLINE_FILE
#include "XShaderGLSL.inl"
#endif
}
#endif //_JIA_XSHADER_GLSL_
