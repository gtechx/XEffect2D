#ifndef _JIA_XTERRAIN_
#define _JIA_XTERRAIN_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
#include "XBasicFun.h"
#include "XTextureInformation.h"
#include "XCam.h"
#include "XBasic3D.h"
#include "XShaderGLSL.h"
namespace XE{
//�޸Ľ���
//1��������VBO
//2������������ģʽ

class XWaterFlow;
//����һ������ͼ����
class XTerrain
{
	friend XWaterFlow;
private:
	XBool m_isInited;				//�Ƿ�����˳�ʼ��
	float *m_terrainData;	//��ͼ����
	const float *m_newH;			//���µ�������ʱ������µĸ߶�����
	XTextureData m_texture;		//���ε���ͼ����
	XBool m_withTexture;				//�Ƿ�����ͼ
	int m_width;					//��ͼ�ĳߴ�
	int m_height;	

	XVector3 m_position;	//���ε�λ��
	XVector3 m_scale;		//���εĳߴ�
	XVector3 m_angle;		//���εļҶ�
	//float m_distance;
	float getHeight(float x,float z) const;
	XVector2 getTexturePosition(float x,float z) const;
	int m_allPointSum;		//�ܵĶ�����
	int m_allFaceSum;		//�ܵ�����
	float m_coordOffsetX;	//��ͼ�Ĳ���
	float m_coordOffsetY;

	XVBO m_vbo;
	std::string m_textureName;
public:
	float *m_v;
	float *m_n;
	float *m_t;
	float *m_c;
private:
	XVector3 *m_faceN;		//��ķ���
	//XVector3 *m_pointN;	//��ķ���
	unsigned int *m_index;	//����

	bool m_withTexGen;	//�Ƿ�ʹ�ý�������
	float m_reflectRate;	//��յķ���ϵ��
	int m_faceSum;		//�����������
	XBool m_withColor;	//�Ƿ�ʹ�ò����ɫ

	XShaderGLSL *m_pShader;	//shader��ָ��
	void releaseMemory();
public:
	float getReflectRate(){return m_reflectRate;}
	void setShader(XShaderGLSL *p){m_pShader = p;}
public:
	XBool init(const char *mapFilename,const char * texFilename,int w,int h,
		XBool withColor = XFalse,	//�Ƿ�ʹ����ɫ
		XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);	//���ļ��ж�ȡ��������
	XBool init(const float *mapData,const char * texFilename,int w,int h,
		XBool withColor = XFalse,	//�Ƿ�ʹ����ɫ
		XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);	//ֱ�Ӵ������ж�ȡ����
	XBool initEx(const char *picFilename,const char * texFilename,	//�Ӻڰ׵�ͼƬ������߶�ͼ
		XBool withColor = XFalse,	//�Ƿ�ʹ����ɫ
		XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);	//���ļ��ж�ȡ��������
	void draw(XBool withTex = XTrue);
	void drawNormal(int step = 1);	//��淨��
	void updateTexture(const unsigned char * data);
	void setTexture(unsigned int tex);	//����������ڲ���
	void setTexture(const XTextureData &tex);
	bool saveToObj(const char * filename);	//�����ݱ���Ϊobj��ʽ��ģ���ļ�(��δʵ��)
private:
	bool saveMtl(const char * filename);
	void updateNormal(const float *heights);	//���·��ߵ���ֵ
	bool loadFromFolder(const char *filename,unsigned char *data);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename,unsigned char *data);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename,unsigned char *data);		//����ҳ�ж�ȡ��Դ
public:
	//flags ���ڱ����Щ���Ƿ���Ҫ������
	//pT �����ṩ���ݣ���pT��ΪNULLʱ����ʾ���ݲ����㣬ֱ�Ӵ�pT�ж�ȡ
	XBool updateHeight(const float *heights,const bool *flags = NULL);	//flags ���ڱ�����Ƿ���Ч
	XBool updateByHeightAndNormal(const float *heights,const float *normal);
	XBool updateByFlag(const bool *flags,const float * w);
	XBool updateVBO(bool v,bool n = false,bool c = false,bool t = false,bool i = false);
	XBool updateByStep(int step);	//ʹ�ò���step�������ͼ����ʾ��Ĭ�������step = 1;

	void setPosition(float x,float y,float z) {m_position.set(x,y,z);}
	void setPosition(const XVector3 &pos) {m_position = pos;}
	void setScale(float x,float y,float z) {m_scale.set(x,y,z);}
	void setScale(const XVector3 &scale) {m_scale = scale;}
	void setAngle(float x,float y,float z) {m_angle.set(x,y,z);}
	void setAngle(const XVector3 &angle) {m_angle = angle;}

	void setWithTexGen(bool flag){m_withTexGen = flag;}
	void release();
	void setReflectRate(float rate);
	XVector3 myNormalize(float x,float z); //�����ض��Ż��Ĺ�һ������
	bool getMousePoint(XVector2 &outPoint);	//[��ʰȡ]���ݵ�ǰ����λ�û�ȡĿǰ�����3D������ʰȡ��λ�ã����û��ʰȡ�������򷵻�false
	bool getMousePointEx(const XVector3 &n,const XVector3 &f,XVector2 &outPoint);	//[��ʰȡ]
	XVector3 getMyNormal(int x,bool flag);	//�����ض��Ż������ߺ���
private:
	XTerrain *m_pSubject;	//����Ŀ��
	bool m_subjectV;		//�����ı�־
	bool m_subjectN;
	bool m_subjectC;
	bool m_subjectT;
	bool m_subjectI;
public:
	//t:������Ŀ��
	//w,n,c,t:ӵ�е�����
	//cv,cn,cc,ct:����������
	bool setSubject(XTerrain *T,bool cv,bool cn,bool cc,bool ct,bool ci);	//����Ϊt�Ĵ���������û�����ԣ�����ȫ����t��

	XTerrain()
		:m_isInited(XFalse)
		,m_terrainData(NULL)
		,m_position(0.0f,0.0f,0.0f)
		,m_scale(1.0f,1.0f,1.0f)
		,m_angle(0.0f,0.0f,0.0f)
		,m_v(NULL)
		,m_n(NULL)
		,m_faceN(NULL)
		,m_c(NULL)
		,m_withColor(XFalse)
		//,m_pointN(NULL)
		,m_t(NULL)
		//,m_distance(1.0f)	//������Ƭ�ĳߴ�
		,m_withTexGen(false)
		,m_reflectRate(0.5f)
		,m_faceSum(0)
		,m_pShader(NULL)
		,m_index(NULL)
		,m_pSubject(NULL)
		,m_tempFitTerrain(NULL)
		,m_subjectV(false)
		,m_subjectN(false)
		,m_subjectC(false)
		,m_subjectT(false)
	{}
	~XTerrain() {release();}
private:
	float *m_tempFitTerrain;
	int m_oInX;
	int m_oInY;
public:
	void myFitTerrain(const float *inData,int inX,int inY,float *outData);	//���������Ż���
};
//���������δ��������
namespace X3D
{
	extern void fitTerrain(const float *inData,int inX,int inY,float *outData,int outX,int outY);//������Ӧ,������ߴ�ĵ��ΰ���3�α�������ֵ����Ҫ�ĵ���
}

//����һ�����㷨�������ڶ�̫����ĵ��ξ����Ż������ٲ���Ҫ����
struct XTerrainFaceInfo
{
	bool isEnable;		//�Ƿ���Ч
	int index[3];		//�������������
	XVector3 normal;	//���߷���
	XTerrainFaceInfo()
		:isEnable(false)
	{}
};
struct XTerrainVertexInfo
{//�����ж�����������
	int faceSum;	//�������
	int index;	//���
	int lastIndex;	//���յı��	//���ڶԶ������ݽ����Ż�
	int normalIndex;	//���ߵı�ţ����ڶԷ������ݽ����Ż�
	int x,y;	//λ��
	float h;	//�߶�
	int faceIndex[6];	//�ٽ�6���������
};
class XTerrainOptimization
{
private:
	XBool m_isInited;
	float * m_data;
	int m_w;
	int m_h;
	int m_dataSize;
	std::vector<XTerrainFaceInfo> m_faceInfos;		//������Ϣ�б�
	std::vector<XTerrainVertexInfo> m_vertexInfos;	//����Ϣ�б�
public:
	XBool init(const float *data,int w,int h);//ֱ�Ӵ����ݽ��г�ʼ��
	XBool init(const char * filename);//ֱ�Ӵ��ļ����г�ʼ��
	
	void optimization();//�Ż�������������Ҫ��һ������
	XBool saveToObj(const char * filename,const char *texName = NULL);	//����Ϊָ���ļ�����obj�ļ�
	XTerrainOptimization()
		:m_isInited(XFalse)
		,m_data(NULL)
	{}
	~XTerrainOptimization()
	{release();}
	void release()
	{
		if(!m_isInited) return;
		XMem::XDELETE_ARRAY(m_data);
		m_isInited = XFalse;
	}
};
#if WITH_INLINE_FILE
#include "XTerrain.inl" 
#endif
}
#endif
