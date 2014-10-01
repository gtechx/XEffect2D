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

_XVBO tmpVBO;
float *v;
float *t;
float *n;
unsigned int *index;

void drawFun(_XBool withTex)
{
	drawOrigin();
//	tempModel.draw(withTex);
	angle = normalize(_XVector3(tmpXSld.getNowValue(),tmpYSld.getNowValue(),tmpZSld.getNowValue()));
	drawBox(angle * 50.0f,_XVector3(2.0f,2.0f,2.0f),_XVector3::zero);
	angle = getAngleFromDirection(angle);
	drawCuboid(_XVector3::zero,_XVector3(100.0f,1.0f,1.0f),angle * RADIAN2DEGREE,_XFColor::white);
	//for(int i = 0;i < 127;++ i)
	//{
	//	for(int j = 0;j < 127;++ j)
	//	{
	//		drawBox(_XVector3(i * 4,j * 4,0),_XVector3(2,2,2),_XVector3::zero,tempSprite.getTexture()->m_texture);
	//	}
	//}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glTranslatef(center.x,center.y,center.z);
	//glRotatef(angle.x,1,0,0);
	//glRotatef(angle.y,0,1,0);
	//glRotatef(angle.z,0,0,1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tempSprite.getTexture()->m_texture);
	glColor4fv(_XFColor::white);

	tmpVBO.drawByIndex(GL_QUADS,150 * 150 * 4,GL_UNSIGNED_INT,index);
//	tmpVBO.use();
//	glDrawElements(GL_QUADS,150 * 150 * 4,GL_UNSIGNED_INT,index);	//m_index
//	tmpVBO.disuse();
	glDisable(GL_BLEND);
	glPopMatrix();
}
void ctrlProc(void *pClass,int id)
{
	float x,y;
	for(int i = 0;i < 150;++ i)
	{
		for(int j = 0;j < 150;++ j)
		{
			x = randomf(1.0f);
			y = randomf(1.0f);
			v[(i * 150 + j) * 4 * 3 + 0] = i * 4 + 2.0f + x * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 1] = j * 4 + -2.0f + y * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 2] = 0.0f;

			v[(i * 150 + j) * 4 * 3 + 0 + 3] = i * 4 + 2.0f + x * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 1 + 3] = j * 4 + 2.0f + y * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 2 + 3] = 0.0f;

			v[(i * 150 + j) * 4 * 3 + 0 + 6] = i * 4 + -2.0f + x * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 1 + 6] = j * 4 + 2.0f + y * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 3 + 6] = 0.0f;

			v[(i * 150 + j) * 4 * 3 + 0 + 9] = i * 4 + -2.0f + x * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 1 + 9] = j * 4 + -2.0f + y * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 2 + 9] = 0.0f;
		}
	}
	tmpVBO.updateDataV(150 * 150 * 4,v);
}
int main(int argc, char **argv)
{
	if(!initWindowEx()) REPORT_ERROR("��������ʧ��");
	setVSync(false);
	//TODO:��ʼ�����еĶ�����Ҳ���Կ����߼��߳�
	tmpXSld.initWithoutTex(_XRect(0.0f,16.0f,256.0f,58.0f),1.0f,-1.0f);
	tmpXSld.setFontEx("X:%.6f",XEE::systemFont,1.0f);
	tmpXSld.setDataChangeCB(ctrlProc,NULL);
	tmpXSld.setPosition(0.0f,16.0f);
	tmpYSld.initWithoutTex(_XRect(0.0f,16.0f,256.0f,58.0f),1.0f,-1.0f);
	tmpYSld.setFontEx("Y:%.6f",XEE::systemFont,1.0f);
	tmpYSld.setPosition(0.0f,86.0f);
	tmpZSld.initWithoutTex(_XRect(0.0f,16.0f,256.0f,58.0f),1.0f,-1.0f);
	tmpZSld.setFontEx("Z:%.6f",XEE::systemFont,1.0f);
	tmpZSld.setPosition(0.0f,156.0f);

	if(tempSprite.init("ResourcePack/obj.png") == 0) return 0;
	tempSprite.setPosition(100,100);
	if(!_X3DWorld::GetInstance().init(drawFun)) return 0;
	if(tempModel.load("ResourcePack/Girl2/peri.obj") == NULL) return 0;
	//��������
	v = createArrayMem<float>(150 * 150 * 4 * 3);
	n = createArrayMem<float>(150 * 150 * 4 * 3);
	t = createArrayMem<float>(150 * 150 * 4 * 2);
	index = createArrayMem<unsigned  int>(150 * 150 * 4);
	//��ʼ������
	for(int i = 0;i < 150;++ i)
	{
		for(int j = 0;j < 150;++ j)
		{
			n[(i * 150 + j) * 4 * 3 + 0] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 1] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 2] = 1.0f;

			t[(i * 150 + j) * 4 * 2 + 0] = 1.0f;
			t[(i * 150 + j) * 4 * 2 + 1] = 0.0f;

			v[(i * 150 + j) * 4 * 3 + 0] = i * 4 + 2.0f;
			v[(i * 150 + j) * 4 * 3 + 1] = j * 4 + -2.0f;
			v[(i * 150 + j) * 4 * 3 + 2] = 0.0f;

			n[(i * 150 + j) * 4 * 3 + 0 + 3] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 1 + 3] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 2 + 3] = 1.0f;

			t[(i * 150 + j) * 4 * 2 + 0 + 2] = 1.0f;
			t[(i * 150 + j) * 4 * 2 + 1 + 2] = 1.0f;

			v[(i * 150 + j) * 4 * 3 + 0 + 3] = i * 4 + 2.0f;
			v[(i * 150 + j) * 4 * 3 + 1 + 3] = j * 4 + 2.0f;
			v[(i * 150 + j) * 4 * 3 + 2 + 3] = 0.0f;

			n[(i * 150 + j) * 4 * 3 + 0 + 6] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 1 + 6] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 2 + 6] = 1.0f;

			t[(i * 150 + j) * 4 * 2 + 0 + 4] = 0.0f;
			t[(i * 150 + j) * 4 * 2 + 1 + 4] = 1.0f;

			v[(i * 150 + j) * 4 * 3 + 0 + 6] = i * 4 + -2.0f;
			v[(i * 150 + j) * 4 * 3 + 1 + 6] = j * 4 + 2.0f;
			v[(i * 150 + j) * 4 * 3 + 3 + 6] = 0.0f;

			n[(i * 150 + j) * 4 * 3 + 0 + 9] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 1 + 9] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 2 + 9] = 1.0f;

			t[(i * 150 + j) * 4 * 2 + 0 + 6] = 0.0f;
			t[(i * 150 + j) * 4 * 2 + 1 + 6] = 0.0f;

			v[(i * 150 + j) * 4 * 3 + 0 + 9] = i * 4 + -2.0f;
			v[(i * 150 + j) * 4 * 3 + 1 + 9] = j * 4 + -2.0f;
			v[(i * 150 + j) * 4 * 3 + 2 + 9] = 0.0f;
			index[(i * 150 + j) * 4 + 0] = (i * 150 + j) * 4;
			index[(i * 150 + j) * 4 + 1] = (i * 150 + j) * 4 + 1;
			index[(i * 150 + j) * 4 + 2] = (i * 150 + j) * 4 + 2;
			index[(i * 150 + j) * 4 + 3] = (i * 150 + j) * 4 + 3;
		}
	}
	//��������
	tmpVBO.init(150 * 150 * 4,v,t,n);

	_XVector2 mouseVector;
	while(!inputEvent()) 
	{//������ѭ��
		ctrlProc(NULL,0);
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