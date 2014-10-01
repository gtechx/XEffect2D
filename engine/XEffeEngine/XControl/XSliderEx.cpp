//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
//#include "XEffeEngine.h"
#include"XSliderEx.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

void stateChangeProc(void *pClass)
{
	_XSliderEx &pPar = *(_XSliderEx *)pClass;
	pPar.stateChange();
}
void sliderExBtnProc(void *pClass,int)
{
	_XSliderEx &pPar = *(_XSliderEx *)pClass;
	if(pPar.m_secondarySld.getVisible()) pPar.m_secondarySld.disVisible();
	else 
	{//������Ҫ���÷�Χ
		pPar.m_secondarySld.setVisible();
		float sum = (pPar.m_mainSld.getMaxValue() - pPar.m_mainSld.getMinValue()) * 0.02;
		float v = pPar.m_mainSld.getNowValue();
		float min = v - sum * 0.5f;
		float max = v + sum * 0.5f;
		if(min < pPar.m_mainSld.getMinValue())
		{
			min = pPar.m_mainSld.getMinValue();
			max = min + sum;
		}else
		if(max > pPar.m_mainSld.getMaxValue())
		{
			max = pPar.m_mainSld.getMaxValue();
			min = max - sum;
		}
		pPar.m_secondarySld.setRange(max,min);
		pPar.m_secondarySld.setNowValue(v);
	}
}
void sliderExSldProc(void *pClass,int id)
{
	_XSliderEx &pPar = *(_XSliderEx *)pClass;
	if(id == pPar.m_secondarySld.getControlID()) pPar.m_mainSld.setNowValue(pPar.m_secondarySld.getNowValue()); else
	if(id == pPar.m_mainSld.getControlID())
	{
		if(pPar.m_secondarySld.getVisible()) pPar.m_secondarySld.disVisible();
		if(pPar.m_funMouseDown != NULL) pPar.m_funMouseDown(pPar.m_pClass,pPar.getControlID());
	}
}
void sliderExSldOnProc(void *pClass,int)
{
	_XSliderEx &pPar = *(_XSliderEx *)pClass;
	if(pPar.m_funMouseOn != NULL) pPar.m_funMouseOn(pPar.m_pClass,pPar.getControlID());
}
//void sliderExSldDownProc(void *pClass,int id)
//{
//	_XSliderEx &pPar = *(_XSliderEx *)pClass;
//	if(pPar.m_funMouseDown != NULL) pPar.m_funMouseDown(pPar.m_pClass,pPar.getControlID());
//}
void sliderExSldUpProc(void *pClass,int)
{
	_XSliderEx &pPar = *(_XSliderEx *)pClass;
	if(pPar.m_funMouseUp != NULL) pPar.m_funMouseUp(pPar.m_pClass,pPar.getControlID());
}
void sliderExSldMoveProc(void *pClass,int)
{
	_XSliderEx &pPar = *(_XSliderEx *)pClass;
	if(pPar.m_funMouseMove != NULL) pPar.m_funMouseMove(pPar.m_pClass,pPar.getControlID());
}
void sliderExSldChangeProc(void *pClass,int)
{
	_XSliderEx &pPar = *(_XSliderEx *)pClass;
	if(pPar.m_funValueChange != NULL) pPar.m_funValueChange(pPar.m_pClass,pPar.getControlID());
}
_XBool _XSliderEx::initWithoutTex(const _XRect& area,float max,float min,
		_XSliderType type,const _XVector2 &fontPosition)
{
	if(m_isInited) return XFalse;	//��ֹ�ظ���ʼ��

	if(!m_mainSld.initWithoutTex(area,max,min,type,fontPosition)) return XFalse;
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//ˮƽģʽ
		m_secondarySld.initWithoutTex(_XRect(0.0f,0.0f,area.getWidth(),area.getHeight()),max,min,type);
		m_chooseBtn.initWithoutTex("+",XEE::systemFont,1.0f,_XRect(0.0f,0.0f,area.getHeight(),area.getHeight()),
			_XVector2(area.getHeight() * 0.5f,area.getHeight() * 0.5f));
	}else
	{//��ֱģʽ
		m_secondarySld.initWithoutTex(_XRect(0.0f,0.0f,area.getWidth(),area.getHeight()),max,min,type);
		m_chooseBtn.initWithoutTex("+",XEE::systemFont,1.0f,_XRect(0.0f,0.0f,area.getWidth(),area.getWidth()),
			_XVector2(area.getWidth() * 0.5f,area.getWidth() * 0.5f));
	}
	m_mainSld.setCallbackFun(NULL,NULL,sliderExSldOnProc,sliderExSldProc,sliderExSldUpProc,sliderExSldChangeProc,sliderExSldMoveProc,this);
	m_mainSld.setStateChangeCB(stateChangeProc,this);
	m_secondarySld.disVisible();
	m_secondarySld.setDataChangeCB(sliderExSldProc,sliderExSldProc,this);
	m_secondarySld.setStateChangeCB(stateChangeProc,this);
	m_chooseBtn.disVisible();
	m_chooseBtn.setMouseUpCB(sliderExBtnProc,this);
	if(m_funInit != NULL) m_funInit(m_pClass,getControlID());
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_mainSld);
	_XObjManger.decreaseAObject(&m_secondarySld);
	_XObjManger.decreaseAObject(&m_chooseBtn);
#endif

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif

	m_isInited = XTrue;
	setPosition(0.0f,0.0f);
	return XTrue;
}
_XBool _XSliderEx::setACopy(const _XSliderEx &temp)			//����һ������
{//
	if(&temp == this) return XFalse;
	if(!temp.m_isInited) return XFalse;
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjManger.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;
	m_timer = temp.m_timer;
	m_funInit = temp.m_funInit;	//��ʼ��
	m_funRelease = temp.m_funRelease;	//��Դ�ͷ�
	m_funMouseOn = temp.m_funMouseOn;	//�������
	m_funMouseDown = temp.m_funMouseDown;//��갴��
	m_funMouseUp = temp.m_funMouseUp;	//��굯��
	m_funMouseMove = temp.m_funMouseMove;//����϶�����ʱ����
	m_funValueChange = temp.m_funValueChange;//����������ֵ�仯
	m_pClass = temp.m_pClass;				//�ص������Ĳ���
//	m_needDrawBtn = temp.m_needDrawBtn;
//	m_needDrawSld = temp.m_needDrawSld;
	if(!m_mainSld.setACopy(temp.m_mainSld)) return XFalse;		
	if(!m_secondarySld.setACopy(temp.m_secondarySld)) return XFalse;		
	if(!m_chooseBtn.setACopy(temp.m_chooseBtn)) return XFalse;		
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_mainSld);
	_XObjManger.decreaseAObject(&m_secondarySld);
	_XObjManger.decreaseAObject(&m_chooseBtn);
#endif
	return XTrue;
}
void _XSliderEx::update(int)
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	//������״̬�жϣ�������Ӧ������update�����жϣ���������ԭ��������пؼ���û��update����������Ŀǰ��draw��������жϣ�֮����Ҫ�����������
	if(m_timer < 1000)
	{
		m_timer += XEE::frameTime;
		if(m_timer >= 1000)
		{
			m_secondarySld.disVisible();
			m_chooseBtn.disVisible();
		}
	}
	//if(m_mainSld.getVisible() && m_mainSld.getActive() && m_mainSld.isInRect(x,y))
	if(m_mainSld.getVisible() && m_mainSld.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
	{
		m_timer = 0;
		m_chooseBtn.setVisible();
	}
	if(m_chooseBtn.getVisible() && m_chooseBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
	{
		m_timer = 0;
	}
	if(m_secondarySld.getVisible() && m_secondarySld.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
	{
		m_timer = 0;
	}

}
void _XSliderEx::setSize(float x,float y)
{
	if(x <= 0 || y <= 0||
		!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_size.set(x,y);

	m_mainSld.setSize(x,y);
	m_secondarySld.setSize(x,y);
	m_chooseBtn.setSize(x,y);
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//ˮƽģʽ
		m_chooseBtn.setPosition(m_mainSld.getBox(1).x + 5.0f * m_size.x,m_mainSld.getBox(1).y);
		m_secondarySld.setPosition(m_mainSld.getBox(3).x,m_mainSld.getBox(3).y + 5.0f * m_size.y);
	}else
	{
		m_chooseBtn.setPosition(m_mainSld.getBox(3).x,m_mainSld.getBox(3).y + 5.0f * m_size.y);
		m_secondarySld.setPosition(m_mainSld.getBox(0).x - 5.0f * m_size.y - m_secondarySld.getMouseRectWidth(),m_mainSld.getBox(0).y);
	}
	m_nowMouseRect = m_mainSld.getMouseRect();
	updateChildSize();
}
void _XSliderEx::setPosition(float x,float y)
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_position.set(x,y);

	m_mainSld.setPosition(x,y);
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//ˮƽģʽ
		m_chooseBtn.setPosition(m_mainSld.getBox(1).x + 5.0f * m_size.x,m_mainSld.getBox(1).y);
		m_secondarySld.setPosition(m_mainSld.getBox(3).x,m_mainSld.getBox(3).y + 5.0f * m_size.y);
	}else
	{
		m_chooseBtn.setPosition(m_mainSld.getBox(3).x,m_mainSld.getBox(3).y + 5.0f * m_size.y);
		m_secondarySld.setPosition(m_mainSld.getBox(0).x - 5.0f * m_size.y - m_secondarySld.getMouseRectWidth(),m_mainSld.getBox(0).y);
	}
	m_nowMouseRect = m_mainSld.getMouseRect();
	updateChildPos();
}
void _XSliderEx::release()
{
	if(!m_isInited) return;
	if(m_funRelease != NULL) m_funRelease(m_pClass,getControlID());
	_XCtrlManger.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	m_isInited = XFalse;
}