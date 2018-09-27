#ifndef _JIA_XSCENEMANAGER_
#define _JIA_XSCENEMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.5.21
//--------------------------------
#include <map>
namespace XE{
class XScene;
enum XSceneChangeMode
{//�����任��ģʽ
	SCENE_MODE_NULL,		//ֱ�ӱ任û���κ�Ч��
	SCENE_MODE_FADE_IN,		//��������
	SCENE_MODE_FADE_OUT,	//��������
	SCENE_MODE_BLACK,		//����
	SCENE_MODE_WHITE,		//����
	//TODO:���������������µĳ�������ģʽ
};
class XFBO;
class XSprite;
class XSceneManager
{
protected:
	XSceneManager();
	XSceneManager(const XSceneManager&);
	XSceneManager &operator= (const XSceneManager&);
	virtual ~XSceneManager();
public:
	static XSceneManager& getInstance()
	{
		static XSceneManager m_instance;
		return m_instance;
	}
public:
	void update(float dt);
	void draw();
	
	bool addScene(XScene* newScene);			//��ӳ���
	bool goToScene(const std::string& name,XSceneChangeMode mode = SCENE_MODE_NULL,float time = 1.0f);	//���ó�������
	bool isChanged()const{return !m_isChange;}	//�жϳ����Ƿ��Ѿ��任���

	void input(const XInputEvent &event);
	unsigned int getNumScenes()const{return m_scenes.size();}
	XScene* getCurrentScene(){return m_currentScene;}
	XScene* getScene(const std::string& name);
	const std::string& getCurrentSceneName()const;
	
private:
	std::map <std::string,XScene *> m_scenes; //���г������б�
			
	XScene* m_currentScene;		//��ǰ����
	XScene* m_futureScene;			//�������ĳ���

	XSceneChangeMode m_changeMode;	//�仯ģʽ
	float m_timer;					//�����仯�ļ�ʱ
	float m_changeSpeed;			//�����仯���ٶ�

	XFBO *m_sceneFbo;
	XSprite *m_sprite;
	bool m_isChange;				//�Ƿ��ڷ����仯
};
#if WITH_INLINE_FILE
#include "XSceneManager.inl"
#endif
#define XSManager XSceneManager::getInstance()
}
#endif