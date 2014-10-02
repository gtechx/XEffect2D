//********************************************************************************
//������һ������Ŀ�ܣ���ܰ������ڵĽ������������¼��Ĵ������Լ�ͼ����Ⱦ����
//program by ��ʤ��
//********************************************************************************
#include "XEffeEngine.h"
#include "X3D/X3DWorld.h"
//Environment="PATH=%PATH%;..\..\engine\dll\SDL;..\..\engine\dll\gl;..\..\engine\dll\MemoryPool;"	//��Ҫ����dll·��
#ifdef OS_WINDOWS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//���ؿ���̨�ı���˵��
#endif
int inputEvent();	//�������¼������������磺�����¼���������¼�

_XSprite tempSprite;
_XModelObj tempModel;
_XSlider tmpXSld;
_XSlider tmpYSld;
_XSlider tmpZSld;
_XVector3 angle;

void drawFun(_XBool withTex)
{
	drawOrigin();
	tempModel.draw(withTex);
	angle = normalize(_XVector3(tmpXSld.getNowValue(),tmpYSld.getNowValue(),tmpZSld.getNowValue()));
	drawBox(angle * 50.0f,_XVector3(2.0f,2.0f,2.0f),_XVector3::zero);
	angle = getAngleFromDirection(angle);
	drawCuboid(_XVector3::zero,_XVector3(100.0f,1.0f,1.0f),angle * RADIAN2DEGREE,_XFColor::white);
}
int main(int argc, char **argv)
{
	if(!initWindowEx()) REPORT_ERROR("��������ʧ��");
	setVSync(false);
	//TODO:��ʼ�����еĶ�����Ҳ���Կ����߼��߳�
	tmpXSld.initWithoutTex(_XRect(0.0f,16.0f,256.0f,58.0f),1.0f,-1.0f);
	tmpXSld.setFontEx("X:%.6f",XEE::systemFont,1.0f);
	tmpXSld.setPosition(0.0f,16.0f);
	tmpYSld.initWithoutTex(_XRect(0.0f,16.0f,256.0f,58.0f),1.0f,-1.0f);
	tmpYSld.setFontEx("Y:%.6f",XEE::systemFont,1.0f);
	tmpYSld.setPosition(0.0f,86.0f);
	tmpZSld.initWithoutTex(_XRect(0.0f,16.0f,256.0f,58.0f),1.0f,-1.0f);
	tmpZSld.setFontEx("Z:%.6f",XEE::systemFont,1.0f);
	tmpZSld.setPosition(0.0f,156.0f);

	if(!tempSprite.init("ResourcePack/obj.png")) return 0;
	tempSprite.setPosition(100,100);
	if(!_X3DWorld::GetInstance().init(drawFun)) return 0;
	if(!tempModel.load("ResourcePack/Girl2/peri.obj")) return 0;

	_XVector2 mouseVector;
	while(!inputEvent()) 
	{//������ѭ��
		//TODO:������������߼�
		_X3DWorld::GetInstance().move(XEE::engineIdle());
		//����ģ�͵����е㣬�ҵ����������ĵ㲢��ǳ���
		float len = 100000000.0f;
		float tempLen;
		_XVector3 tempV3;
		_XVector2 tempV2;
		for(int i = 0;i < tempModel.getVectorSum();++ i)
		{
			tempV3 = _X3DWorld::GetInstance().worldToScreen(tempModel.getVector(i),_XRect(0.0f,0.0f,1024.0f,768.0f));
			tempV2.set(tempV3.x,tempV3.y);
			tempLen = tempV2.getLengthSqure(XEE::mousePosition);
			if(tempLen < len)
			{
				len = tempLen;
				mouseVector = tempV2;
			}
		}

		XEE::clearScreen();			//�����Ļ
		//TODO:���������Ҫ����3D����
		_X3DWorld::GetInstance().draw();
		XEE::begin2DDraw();
		//TODO:������Ҫ����2D����
		tempSprite.draw();
		drawBox(mouseVector.x,mouseVector.y,10,10);
		drawBox(XEE::mousePosition.x,XEE::mousePosition.y,10,10);

		XEE::updateScreen();			//������Ļ������
		//mySleep(1);
	}
	//TODO:�����˳��������ͷ�������Դ
	XEE::release();
	return 0;	
}

int inputEvent()
{
	_XInputEvent tmpEvent;		//SDL�¼����

	while(getInputEventSDL(tmpEvent)) 
	{
		switch(tmpEvent.type)
		{
		case EVENT_EXIT:return 1;
		case EVENT_KEYBOARD:
			if(tmpEvent.keyState == KEY_STATE_DOWN)
			{
				switch(tmpEvent.keyValue)
				{
				case XKEY_ESCAPE:return 1;
				}
			}
			break;
		}
		XEE::inputEvent(tmpEvent);
		_X3DWorld::GetInstance().keyEvent(tmpEvent);
	}
	return 0;
}