#ifndef _JIA_XRESOURCEPACK_
#define _JIA_XRESOURCEPACK_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//Ŀǰ������������������
//--------------------------------
#include "XCommonDefine.h"
#include "XMemory.h"

#include "stdio.h"
#include <memory>
#include "string.h"
namespace XE{
class XTextureInfo;

#pragma pack(push)
#pragma pack(1)
struct XResourceFileStruct
{
	char fileName[MAX_FILE_NAME_LENGTH];		//�ļ�����
	int fileLength;								//�ļ�����
	int filePosition;							//�ļ�ƫ������
};
#pragma pack(pop)

class XResourcePack
{
private:
	static const int m_maxFileSum = 3000;	//��Դ��������ܰ�������Դ����
	static const int m_lockCodeLength = 32;	//��Կ�ĳ��ȣ���Կ�ֳ��ĸ����֣�ÿ������32��bytes
	static const int m_allLockCodeLength = 128;	//��Կ�ܳ���

	static const int m_memoryChangeTimes = 36;
	static const int m_md5CodeSize = 16;
public:
	XResourcePack();
	virtual ~XResourcePack();
protected:
	XResourcePack(const XResourcePack&);
	XResourcePack &operator= (const XResourcePack&);
	//+++++++++++++++++++++++++++++++++++++++++++
	//2011����������ڴ���Ҽ���
private:
	unsigned char *m_fileData;
	int m_fileDataLength;
	int m_fileDataPoint;
	//-------------------------------------------
public:
	XResourceFileStruct *m_fileInfo;						//����ܴ��m_maxFileSum���ļ�
	int m_fileSum;											//��Դ���е��ļ�����
	int m_normalFileSum;									//��ͨ�ļ�������
	XBool m_haveReadedFileList;								//�Ƿ��Ѿ���ȡ������Ϣ
	XBool m_haveReadedNormalFileList;						//�Ƿ��Ѿ���ȡ����ͨ�ļ�������Ϣ
	long m_headLength;										//��Դ�����ļ�ͷ�ĳ���
	unsigned char tempCode[m_lockCodeLength];				//��Դ���еļ�����
	unsigned char m_lockKey[m_allLockCodeLength];			//����
	unsigned char m_hardLockKey[m_lockCodeLength];			//��Կ����������Ӳ���ļ�����Կ

	char m_outFileName[MAX_FILE_NAME_LENGTH];				//����ļ�������
	void setOutFileName(const char *temp = NULL);			//��������ļ�������
private:
	XBool m_isGetOptimizeInfo;
	XBool getOptimizeInfo();
	XBool releaseOptimizeInfo();
	XTextureInfo *m_texInfo;
	int *m_targetOrder;	//��ͼ��Ŀ����ͼ֮��Ķ�Ӧ��ϵ
	char *m_targetName;	//��Դ�Ż�֮���Ŀ������
	int m_texInfoSum;
	int m_targetTextureSum;
public:
	int makeFileList();										//�����嵥�ļ�
	int makeResourcePack(int packMode = 0);					//�����Դ����packMode = 0ʱ��װ���ǲ���Ҫ�������ܺ�У�����Դ��
															//			��packMode = 1ʱ��װ���Ǿ������ܺ�У�����Դ
	//Ŀǰ���������������ѹ������Դ����������
	int unpackAllResource();								//������е���Դ	//����û��Ŀ¼�����Ĵ�����������ֻ�ܽ�ѹ����Ŀ¼���ļ�
	int unpackResource(const char *fileName);				//���ĳһ����Դ	//����û��Ŀ¼�����Ĵ�����������ֻ�ܽ�ѹ����Ŀ¼���ļ�
	int unpackResource(const char *fileName, unsigned char *p);	//���ĳһ����Դ��һ���ڴ�ռ�
	int getStringLength(const char *p) const;				//����ַ����ĳ���
	void setHardLockKey(const void *p);						//����Ӳ���μ�����Կ

	int checkFileCode(const char *p) const;					//����ļ��������Ƿ���ȷ
	//int xFileNameCMP(char *p1,char *p2);					//�Ƚ������ļ�·���Ƿ���ͬ

	//int getPathDeep(char *p);								//��ȡ·�����
	int readFileListFromResouce();							//��ѹ�����ж�ȡ�ļ�������Ϣ		//���������Ҫ�ع�
	int readNormalFileListFromResouce();					//��ѹ�����ж�ȡ��ͨ�ļ���������Ϣ	//���������Ҫ�ع�
	int getFileLength(const char *fileName);				//��Դ������Դ�ļ��Ĵ�С
	unsigned char *getFileData(const char *filename);		//��ȡָ���ļ����ļ�����

	int writeCheckData();	//����Դ�ļ���д��У���
	XBool checkCheckData();	//У����Դ�ļ���У���

	void lockOrUnlockProc(unsigned char *p, int startPoint, int length) const;	//��������p���м��ܻ��߽��� startPointΪ��ʼλ�� lengthΪ���ݳ���
	void getlockKey();		//��������

	int isOptimized(const char *p) const;	//�ж�����ļ��Ƿ��Ż� 0:û�в����Ż� 1:�Ѿ������Ż�
	void release();	//�ͷ���Դ
};
inline int XResourcePack::getStringLength(const char *p) const
{
	if(strlen(p) >= MAX_FILE_NAME_LENGTH) return 0;
	else return (int)(strlen(p));
//	for(int i=0;i < MAX_FILE_NAME_LENGTH;++ i)
//	{
//		if(p[i] == '\0') return i;
//	}
//	return 0;
}
inline void XResourcePack::setHardLockKey(const void *p)
{
	memcpy(m_hardLockKey,p,32);
}
inline XResourcePack::~XResourcePack()
{
	release();
}
inline void XResourcePack::release()
{
	XMem::XDELETE_ARRAY(m_fileInfo);
	XMem::XDELETE_ARRAY(m_fileData);
	m_haveReadedFileList = XFalse;
}
}
#endif