#ifndef _JIA_XIMM_
#define _JIA_XIMM_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
#include "XOSDefine.h"
#include <string>
#include <imm.h>
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "ole32.lib")
namespace XE{
//����һ�����ڶ�windows�µ����뷨�ķ�װ
namespace XImm
{
	extern LRESULT inputMsgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam,bool &end);
	extern bool getIsInputChange();				//���뷨�Ƿ����ı�
	extern std::string getImmDescription();		//��ȡ��ǰ���뷨�������ı�
	extern std::string getImmFilename();		//��ȡ��ǰ���뷨���ļ���
	extern void setCompositionPos(float x,float y);			//�������뷨����ʾλ��
	extern bool getIsCompositionStrChange();	//�������ַ����Ƿ����˸ı�
	extern std::string getCompositionStr();		//��ȡ��ǰ�������ַ���
	extern bool getCompositionState();			//��ȡ������״̬����ʼ�����߽���
	extern bool getIsCandidateListChange();		//��ѡ�б��Ƿ�������
	extern bool getHaveCandidateList();			//�Ƿ��к�ѡ�б�
	extern int getCandidateListCount();			//��ѡ�б������
	extern std::string getCandidateListStr(int index);	//��ȡ��ѡ�б��е��ַ���
	extern void showImmUI(bool hideOrShow);		//Ӱ�ػ���ʾ���뷨����

	extern bool getIsImm();	//�ж����뷨�����ͣ����뷨����ΪIMM������TSF
	extern std::string getImeName();	//��ȡ��ǰ���뷨������
	extern bool getIsEnglish();		//�Ƿ���Ӣ�����뷨
	extern bool getIsFullShape();	//�Ƿ���ȫ��
	extern bool getIsSybol();		//�Ƿ��������������
}
}
#endif