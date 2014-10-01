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
extern void removeRedundantSpace(char *p);	//ɾ�������Ļ�����β�Ŀո������Ҫ��Ϊ�˶�ȡOBJ�ļ�ʱ�ĸ�ʽ����
//Obj�ļ��ж��������
struct _XFaceInfo
{
    _XVectorIndex3 vIndex;	//���������
	_XVectorIndex3 tIndex;	//��ͼ������
	_XVectorIndex3 nIndex;	//���ߵ�����	
};
struct _XMaterialInfo
{//������Ϣ
	string matetialName;	//���ʵ�����
	string mapKdName;
	string mapDumpName;
	_XMaterial material;	//�����Ĳ���
	_XTextureData textureData;	//��ͼ
	_XBool withTexture;			//�Ƿ�ӵ����ͼ
	_XBool withDumpTex;			//�Ƿ�ӵ�з�����ͼ
	_XTextureData dumpTexData;	//������ͼ
	//_XShaderGLSL * normalShader;//������ͼ��shader
	//��Ҫ��ȡ�ò���
	_XVector4 ka;
	_XVector4 kd;
	_XVector4 ks;
	_XVector4 ke;
	float shininess;
	float alpha;
	//����Ĳ����ж�ȡ����û����Ч
	float Ni;
	_XVector3 Tf;
	int illum;
	void setData()	//ʹ��������Ч
	{
		material.setAmblient(ka.x,ka.y,ka.z,ka.w);
		material.setDiffuse(kd.x,kd.y,kd.z,kd.w);
		material.setSpecular(ks.x,ks.y,ks.z,ks.w);
		material.setEmission(ke.x,ke.y,ke.z,ke.w);
		material.setShininess(shininess);
	}
	_XMaterialInfo()
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
	~_XMaterialInfo()
	{
		//XDELETE(normalShader);
	}
};
struct _XGroupInfo
{
	string groupName;	//����
	char withTexInfo;		//�Ƿ�����ͼ��Ϣ
	char withNormalInfo;	//�Ƿ��з�������
	int materialIndex;
	string materialName;
	std::vector<_XFaceInfo> face;
	_XVBO vbo;

	_XGroupInfo()
		:withTexInfo(-1)
		,withNormalInfo(-1)
		,materialIndex(0)
	{}
	~_XGroupInfo(){release();}
	void release()
	{
		face.clear();
		vbo.release();
	}
};

class _XModelObj:public _XBasic3DObject
{
private:
	_XBool m_isInited;	//�Ƿ��ʼ��
	_XVector3 m_bandBox[8];	//ģ�͵İ�Χ���ӣ�ʵ���п��ܾ���λ�ơ���ת�������������ݻ�仯��

	std::vector<_XVector3> m_point;				//����	//Ŀǰֻ������Щ����
	std::vector<_XVector2> m_texturePoint;		//��ͼ����
	std::vector<_XVector3> m_normal;			//����
	std::vector<_XMaterialInfo*> m_material;	//������Ϣ
	std::vector<_XGroupInfo *> m_group;			//��
	std::vector<string> m_materialFilename;		//ģ�����漰�Ĳ��ʵ��ļ���
	std::vector<int> m_materialIndex;			//ÿ�������ļ�����ʼ����

	_XBool readFaceInfo(_XGroupInfo *group,char *lineData);	//��һ�������ж�ȡ�������
	_XBool loadMaterial(const char *filename);	//��ȡ�����ļ�
	_XBool saveMaterial();	//�������ʶ��У��������������
	int getMaterialIndex(const string &str);
	_XMaterialInfo * getMaterialInfo(const string &MatName);
	_XGroupInfo *getGroup(const char *name);	//ͨ�����ֻ�ȡ���ָ��
public:
	//_XBool setNormalShader(const string &MatName,_XShaderGLSL *shader);
	//void setWithDumpMap(_XBool flag);	//�����Ƿ�ʹ�÷�����ͼ

	_XBool save(const char *filename);	//�����ݱ��浽�ļ�����ʽΪOBJ
	_XBool load(const char *filename);	//���ļ�����������
	void draw(_XBool withTex = XTrue,const _XBasic3DObject *base = NULL);			//withTex����ʱ���Ƿ�ʹ����ͼ��Ϣ
	void drawBandbox()
	{//��ʱ�Ȳ�����λ�ơ���ת������(��δ���)
		drawLine(m_bandBox[0],m_bandBox[1]);
		drawLine(m_bandBox[1],m_bandBox[2]);
		drawLine(m_bandBox[2],m_bandBox[3]);
		drawLine(m_bandBox[3],m_bandBox[0]);
		drawLine(m_bandBox[4],m_bandBox[5]);
		drawLine(m_bandBox[5],m_bandBox[6]);
		drawLine(m_bandBox[6],m_bandBox[7]);
		drawLine(m_bandBox[7],m_bandBox[4]);
		drawLine(m_bandBox[0],m_bandBox[4]);
		drawLine(m_bandBox[1],m_bandBox[5]);
		drawLine(m_bandBox[2],m_bandBox[6]);
		drawLine(m_bandBox[3],m_bandBox[7]);
	}
private:
	_XBool checkData();	//������ݵĺϷ���
public:
	int getVectorSum() const {return m_point.size();}
	_XVector3 getVector(int index) const 
	{
		if(index < 0 || index >= m_point.size()) return _XVector3::zero;
		return m_point[index];
	}
	_XVector3 getNormal(int index)
	{
		if(index < 0 || index >= m_normal.size()) return _XVector3::zero;
		return m_normal[index];
	}
	_XVectorIndex3 getFaceNormalIndex(int index) const
	{
		if(m_group.size() <= 0 || index < 0 || index >= m_group[0]->face.size()) return _XVectorIndex3(0.0f,0.0f,0.0f);
		return m_group[0]->face[index].nIndex;
	}
	int getFaceSum() const
	{
		if(m_group.size() <= 0) return 0;
		return m_group[0]->face.size();
	}
	_XVectorIndex3 getFaceVectorIndex(int index) const
	{
		if(m_group.size() <= 0 || index < 0 || index >= m_group[0]->face.size()) return _XVectorIndex3(0.0f,0.0f,0.0f);
		return m_group[0]->face[index].vIndex;
	}

	void release();
	_XModelObj()
		:m_isInited(XFalse)
	{}
	~_XModelObj(){release();}
};

//extern std::string getObjInfo0FromStr(const char * p);	//��OBJ�ļ���ȡ���ַ����л�ȡ��Ҫ���ַ������磺�������ļ�����

#endif