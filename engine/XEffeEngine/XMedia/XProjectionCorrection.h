#ifndef _JIA_XPROJECTIONCORRECTION_
#define _JIA_XPROJECTIONCORRECTION_
#include "XControl\XControls.h"
#include "XMath\XVector2.h"
#include "XShaderGLSL.h"
//����һ�����ڵ�̨ͶӰ�Ľ�������
//��Ҫʵ���������š���ת���ƶ�
//��ݼ�˵��
//�ո���ʾ��������
//B������
//M���ƶ�
//R����ת
//S������
namespace XE{
enum XPCAction
{
	PC_ACTION_NULL,		//û�ж���
	PC_ACTION_MOVE,		//�ƶ�
	PC_ACTION_SCALE,	//����
	PC_ACTION_ROTATION	//��ת
};
enum XProjectionCorMode
{
	PC_MODE_TEXTURE,	//��ͼģʽ
	PC_MODE_FILL,		//���ģʽ
	PC_MODE_TEXTURE_FILL,	//ͼƬ���
};
class XProjectionCorrection
{
private:
	bool m_isInputEnable;	//�Ƿ�����������
	bool m_isInited;
	bool m_isShowCtrl;	//�Ƿ���ʾ�ؼ�
	bool m_withFBO;		//�Ƿ��FBO
	XVec2 m_position;	//ƫ��λ��
	XVec2 m_scale;	//���Ƶ������ԭʼ��С����
	XVec2 m_size;	//���巶Χ�Ĵ�С��Ĭ��Ϊ������С
	XPointCtrl m_pointPcl[4];	//�ĸ��ǵ�Ŀؼ�
	std::vector<XVec2> m_pointsPos;	//�ĸ����λ��
	XFBO m_fbo;
	XProjectionCorMode m_mode;
	XShaderGLSL m_fillShader;

	XPCAction m_curAction;
	XE::XVec2 m_curMousePos;
	void initCtrl();	//��ʼ���ؼ�
	std::string m_configFileName;

	//bool m_haveMsg;
	//std::string m_massageStr;
	int m_meshSubSum;	//����ϸ�ֵ�����
public:
	//const std::string& getMsgStr()const{return m_massageStr;}
	////����������ܵ���һ����Ч
	//bool getHaveMsg()
	//{
	//	if(m_haveMsg)
	//	{
	//		m_haveMsg = false;
	//		return true;
	//	}
	//	return false;
	//}
	void setConfigFilename(const std::string& filename){m_configFileName = filename;}
	const std::string& getConfigFilename()const{return m_configFileName;}
	bool getEnableInput()const{return m_isInputEnable;}
	void setEnableInput(bool flag){m_isInputEnable = flag;}
	XProjectionCorrection()
		:m_isInited(false)
		, m_isShowCtrl(false)
		, m_curAction(PC_ACTION_NULL)
		, m_isInputEnable(true)
		, m_configFileName("PCConfig.xml")
	//	, m_haveMsg(false)
		, m_meshSubSum(10)
	{}
	void begin()
	{
		if(m_withFBO)
			m_fbo.useFBO();
	}
	void end()
	{
		if(m_withFBO)
			m_fbo.removeFBO();
	}
	bool getIsShowCtrl()const{return m_isShowCtrl;}
	void setIsShowCtrl(bool flag);
	void draw(bool withClear = true);
	XVec2 getCornerPos(int index)
	{
		if(index < 0 || index >= 4) return XVec2::zero;
		return m_pointPcl[index].getPosition();
	}
	void draw(unsigned int tex,bool withClear = true);
	void init(const XVec2& size,const XVec2& pos,const XVec2& scale,bool withFBO = true,XProjectionCorMode mode = PC_MODE_TEXTURE);
	void init(bool withFBO = true,XProjectionCorMode mode = PC_MODE_TEXTURE);
	bool inputProc(const XInputEvent &event);	//�������������Ӧ
	bool saveData(const char *filename = NULL);	//�������ݵ��ļ�
	bool loadData(const char *filename = NULL);
	XVec2 mapPos(const XVec2& pos) const;	//pos���꾭������任֮�����任֮���λ��
};
}
#endif