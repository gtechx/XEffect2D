#ifndef _JIA_XMODELOBJ_
#define _JIA_XMODELOBJ_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.9.20
//--------------------------------
//��Ҫ���ӵ�����
//+1�������������Եļ��(δ���)
//+2����f���ݸ�������֧�� x/y/z ��/ͼ/��
//+3���Զ�ģ�͵�֧��
//+4������ͼ��֧��,������ͼ��֧��
//+5�����Ľ����֧��
//+6����Դ�ͷ�

//����һ����Obj��ʽ��ģ���ļ��ж�ȡģ�����ݣ������д������
#include "XBasic3D.h"
#include "XTextureInformation.h" 
#include "XMaterial.h"
#include "XShaderGLSL.h"
#include <vector>
namespace XE{
extern void removeRedundantSpace(char *p);	//ɾ�������Ļ�����β�Ŀո������Ҫ��Ϊ�˶�ȡOBJ�ļ�ʱ�ĸ�ʽ����
//Obj�ļ��ж��������
struct XFaceInfo
{//ֻ֧��������
    XVectorIndex3 vIndex;	//���������
	XVectorIndex3 tIndex;	//��ͼ������
	XVectorIndex3 nIndex;	//���ߵ�����	
};
struct XFaceInfoEx
{//����֧�ֶ����
	std::vector<int> vsIndex;
	std::vector<int> tsIndex;
	std::vector<int> nsIndex;
};
struct XMaterialInfo
{//������Ϣ
	std::string matetialName;	//���ʵ�����
	std::string mapKdName;
	std::string mapDumpName;
	XMaterial material;	//�����Ĳ���
	XTextureData textureData;	//��ͼ
	XBool withTexture;			//�Ƿ�ӵ����ͼ
	XBool withDumpTex;			//�Ƿ�ӵ�з�����ͼ
	XTextureData dumpTexData;	//������ͼ
	//XShaderGLSL * normalShader;//������ͼ��shader
	//��Ҫ��ȡ�ò���
	XVec4 ka;
	XVec4 kd;
	XVec4 ks;
	XVec4 ke;
	float shininess;
	float alpha;
	//����Ĳ����ж�ȡ����û����Ч
	float Ni;
	XVec3 Tf;
	int illum;
	void setData()	//ʹ��������Ч
	{
		material.setAmblient(ka.x,ka.y,ka.z,ka.w);
		material.setDiffuse(kd.x,kd.y,kd.z,kd.w);
		material.setSpecular(ks.x,ks.y,ks.z,ks.w);
		material.setEmission(ke.x,ke.y,ke.z,ke.w);
		material.setShininess(shininess);
	}
	XMaterialInfo()
		:withDumpTex(XFalse)
		,withTexture(XFalse)
		//,normalShader(NULL)
		,ka(0.2f,0.2f,0.2f,1.0f)
		,kd(0.8f,0.8f,0.8f,1.0f)
		,ks(0.0f,0.0f,0.0f,1.0f)
		,ke(0.0f,0.0f,0.0f,0.0f)
		,shininess(0.0f)
		,alpha(1.0f)
	{}
	~XMaterialInfo()
	{
		//XMem::XDELETE(normalShader);
	}
};
struct XGroupInfo
{
	char withTexInfo;		//�Ƿ�����ͼ��Ϣ
	char withNormalInfo;	//�Ƿ��з�������
	std::string groupName;	//����
	int materialIndex;
	std::string materialName;
	std::vector<XFaceInfo> face;	//�����������ֵ�����Ϣ
	std::vector<XFaceInfoEx> originalFaces;	//ԭʼ������Ϣ
	XVBO vbo;

	XGroupInfo()
		:withTexInfo(-1)
		,withNormalInfo(-1)
		,materialIndex(0)
	{}
	~XGroupInfo(){release();}
	void release()
	{
		originalFaces.clear();
		face.clear();
		vbo.release();
	}
};

class XModelObj:public XBasic3DObject
{
private:
	XBool m_isInited;	//�Ƿ��ʼ��
	XVec3 m_bandBox[8];	//ģ�͵İ�Χ���ӣ�ʵ���п��ܾ���λ�ơ���ת�������������ݻ�仯��

	std::vector<XVec3> m_point;				//����	//Ŀǰֻ������Щ����
	std::vector<XVec2> m_texturePoint;		//��ͼ����
	std::vector<XVec3> m_normal;			//����
public:
	std::vector<XMaterialInfo*> m_material;	//������Ϣ
private:
	std::vector<XGroupInfo *> m_group;			//��
	std::vector<std::string> m_materialFilename;		//ģ�����漰�Ĳ��ʵ��ļ���
	std::vector<int> m_materialIndex;			//ÿ�������ļ�����ʼ����

	XBool readFaceInfo(XGroupInfo *group,char *lineData);	//��һ�������ж�ȡ�������
	XBool loadMaterial(const char *filename);	//��ȡ�����ļ�
	XBool saveMaterial();	//�������ʶ��У��������������
	int getMaterialIndex(const std::string& str);
	XMaterialInfo * getMaterialInfo(const std::string& MatName);
	XGroupInfo *getGroup(const char *name);	//ͨ�����ֻ�ȡ���ָ��
public:
	//XBool setNormalShader(const string &MatName,XShaderGLSL *shader);
	//void setWithDumpMap(XBool flag);	//�����Ƿ�ʹ�÷�����ͼ

	XBool save(const char *filename);	//�����ݱ��浽�ļ�����ʽΪOBJ
	XBool load(const char *filename);	//���ļ�����������
	void draw(XBool withTex = XTrue,const XBasic3DObject *base = NULL,bool withBandbox = false);			//withTex����ʱ���Ƿ�ʹ����ͼ��Ϣ
private:
	XBool checkData();	//������ݵĺϷ���
public:
	int getTextureSum()const { return m_texturePoint.size(); }
	XVec2 getTexture(unsigned int index)const;
	int getVectorSum() const {return m_point.size();}
	XVec3 getVector(unsigned int index) const;
	XVec3 getNormal(unsigned int index)const;
	int getFaceSum() const;
	XVectorIndex3 getFaceVectorIndex(unsigned int index) const;
	XVectorIndex3 getFaceTextureIndex(unsigned int index) const;
	XVectorIndex3 getFaceNormalIndex(unsigned int index) const;
	void useMatrix(const XBasic3DObject *base = NULL);	//ʹ�þ���
	void endMatrix() { glPopMatrix(); }	//��������
	//��ȡԭʼ����
	int getOriFaceSum()const;
	std::vector<int> getOriFaceVIndex(unsigned int index) const;
	std::vector<int> getOriFaceTIndex(unsigned int index) const;
	std::vector<int> getOriFaceNIndex(unsigned int index) const;

	void release();
	XModelObj()
		:m_isInited(XFalse)
	{}
	~XModelObj(){release();}
};
namespace XRender {
extern void drawModel(const std::vector<XVec3> &v, const std::vector<XVec2> &t,
	const std::vector<XFaceInfo> &f, const XVec3& offset, const XVec3& scale, unsigned int tex);
extern void drawModel(const std::vector<XVec3> &v, const std::vector<XVec2> &t,
	const std::vector<XFaceInfoEx> &f, const XVec3& offset, const XVec3& scale, unsigned int tex);
}
//extern std::string getObjInfo0FromStr(const char * p);	//��OBJ�ļ���ȡ���ַ����л�ȡ��Ҫ���ַ������磺�������ļ�����
#if WITH_INLINE_FILE
#include "XModelObj.inl" 
#endif
}
#endif