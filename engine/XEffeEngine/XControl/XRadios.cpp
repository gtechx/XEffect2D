//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XRadios.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XRadios::_XRadios()
:m_isInited(XFalse)			//�ռ�ʱ���Ѿ���ʼ��
,m_radioSum(1)			//ѡ�������
,m_nowChoose(0)			//��ǰ��ѡ����ı��
,m_radio(NULL)			//���е�ѡ���ָ��
,m_checkPosition(NULL)
,m_funStateChange(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
{
	m_ctrlType = CTRL_OBJ_RADIOS;
}
void _XRadios::release()	//�ͷŷ������Դ
{
	if(!m_isInited) return;
	XDELETE_ARRAY(m_radio);
	XDELETE_ARRAY(m_checkPosition);
	_XCtrlManger.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
_XBool _XRadios::init(int radioSum,			//ѡ�������
		const _XVector2& distance,
		const _XVector2& position,	//�ؼ���λ��
		const _XRect &Area,			//ѡ��ͼ��������Ӧ��Χ
		const _XRadiosTexture *tex,const _XFontUnicode &font,float captionSize,
		const _XVector2& textPosition)	//��ѡ���ʼ��
{
	if(m_isInited) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	if(radioSum < 1) return XFalse;	//����û��ѡ�����Ҫ��һ��ѡ��
	if(captionSize <= 0) return XFalse;
	m_mouseRect = Area;
	m_position = position;
	m_distance = distance;
	m_withoutTex = XFalse;

	if(tex->checkChoosed == NULL || tex->checkDisableChoosed == NULL 
		|| tex->checkDisableDischoose == NULL || tex->checkDischoose == NULL) return XFalse;//�����ͼȱ�٣�Ҳֱ�ӷ���ʧ��
	//�����ڴ�ռ�
	m_radioSum = radioSum;
	m_radio = createArrayMem<_XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//����ڴ����ʧ�ܣ�ֱ�ӷ��ش���
	m_checkPosition = createArrayMem<_XVector2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XDELETE_ARRAY(m_radio);
		return XFalse;
	}
	m_size.set(1.0f,1.0f);

	m_texture = tex;
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_captionSize = captionSize;
	m_textPosition = textPosition;

#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjManger.getNowObjectSum());
#endif
	//��ʼ�����е�ѡ��
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_checkPosition[i].set(m_distance.x * i,m_distance.y * i);
		if(!m_radio[i].init(_XVector2(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y),
			m_mouseRect,* m_texture," ",m_caption,m_captionSize,m_textPosition))
		{
			XDELETE_ARRAY(m_radio);
			XDELETE_ARRAY(m_checkPosition);
			return XFalse;
		}
		m_radio[i].setState(XFalse);
		m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y);

		//�������������ע������Щ���
		_XCtrlManger.decreaseAObject(&(m_radio[i]));
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&(m_radio[i]));
#endif
	}

#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjManger.getNowObjectSum());
#endif
	m_nowChoose = 0;
	m_radio[0].setState(XTrue);

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XRadios::initWithoutTex(int radioSum,
		const _XVector2& distance,
		const _XRect &area,
		const _XFontUnicode &font,float captionSize,
		const _XVector2& textPosition)
{
	if(m_isInited ||
		radioSum < 1 ||	//����û��ѡ�����Ҫ��һ��ѡ��
		captionSize <= 0) return XFalse;
	m_mouseRect = area;
	m_position.set(0.0f,0.0f);
	m_distance = distance;
	m_withoutTex = XTrue;

	//�����ڴ�ռ�
	m_radioSum = radioSum;
	m_radio = createArrayMem<_XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//����ڴ����ʧ�ܣ�ֱ�ӷ��ش���
	m_checkPosition = createArrayMem<_XVector2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XDELETE_ARRAY(m_radio);
		return XFalse;
	}
	m_size.set(1.0f,1.0f);

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_captionSize = captionSize;
	m_textPosition = textPosition;

#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjManger.getNowObjectSum());
#endif
	//��ʼ�����е�ѡ��
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_checkPosition[i].set(m_distance.x * i,m_distance.y * i);
		if(!m_radio[i].initWithoutTex(" ",m_caption,m_captionSize,m_mouseRect,m_textPosition))
		{
			XDELETE_ARRAY(m_radio);
			XDELETE_ARRAY(m_checkPosition);
			return XFalse;
		}
		m_radio[i].setState(XFalse);
		m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y);
	//�������������ע������Щ���
		_XCtrlManger.decreaseAObject(&(m_radio[i]));
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&(m_radio[i]));
#endif
	}

#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjManger.getNowObjectSum());
#endif
	m_nowChoose = 0;
	m_radio[0].setState(XTrue);
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XRadios::setChoosed(int temp)
{
	if(temp < 0 || temp >= m_radioSum ||
		temp == m_nowChoose) return;
	m_nowChoose = temp;
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setState(XFalse);
	}
	m_radio[m_nowChoose].setState(XTrue);
	if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
}
_XBool _XRadios::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	int tempOrder = -1;	//��¼��������ѡ��ı��
	for(int i = 0;i < m_radioSum;++ i)
	{
		if(m_radio[i].mouseProc(x,y,mouseState))
		{//����Ĭ��ֻ����һ��ѡ�����
			if(m_nowChoose != i) tempOrder = i;
			else m_radio[m_nowChoose].setState(XTrue);
		}
	}
	if(tempOrder != -1)
	{
		//printf("NowChoose:%d,%d\n",tempOrder,m_objectID);
		m_nowChoose = tempOrder;
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].setState(XFalse);
		}
		m_radio[m_nowChoose].setState(XTrue);
		if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
		m_isBeChoose = XTrue;
		stateChange();
	}
	return XTrue;
}
_XBool _XRadios::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	if(keyState == KEY_STATE_UP && m_isBeChoose)
	{//��������ʱ������Ӧ
		if(keyOrder == XKEY_LEFT || keyOrder == XKEY_UP)
		{//������������
			--m_nowChoose;
			if(m_nowChoose < 0) m_nowChoose = m_radioSum - 1;
			for(int i = 0;i < m_radioSum;++ i)
			{
				m_radio[i].setState(XFalse);
			}
			m_radio[m_nowChoose].setState(XTrue);
			if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
			stateChange();
		}else
		if(keyOrder == XKEY_RIGHT || keyOrder == XKEY_DOWN)
		{//������������
			++m_nowChoose;
			if(m_nowChoose >= m_radioSum) m_nowChoose = 0;
			for(int i = 0;i < m_radioSum;++ i)
			{
				m_radio[i].setState(XFalse);
			}
			m_radio[m_nowChoose].setState(XTrue);
			if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
			stateChange();
		}
	}
	return XTrue;
}
void _XRadios::setSize(float x,float y)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		x <= 0 || y <= 0) return;
	m_size.set(x,y);
	//�ƶ�����ѡ��ؼ������λ��
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y);
		m_radio[i].setSize(m_size);
	}
}
void _XRadios::setPosition(float x,float y)
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_position.set(x,y);
	//�ƶ�����ѡ��ؼ������λ��
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y);
	}
}
_XBool _XRadios::setRadioSum(int radioSum)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		radioSum <= 0) return XFalse;
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjManger.getNowObjectSum());
#endif
	XDELETE_ARRAY(m_radio);
	XDELETE_ARRAY(m_checkPosition);
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjManger.getNowObjectSum());
#endif
	m_radioSum = radioSum;
	m_radio = createArrayMem<_XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//����ڴ����ʧ�ܣ�ֱ�ӷ��ش���

#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjManger.getNowObjectSum());
#endif
	m_checkPosition = createArrayMem<_XVector2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XDELETE_ARRAY(m_radio);
		return XFalse;
	}

	//��ʼ�����е�ѡ��
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_checkPosition[i].set(m_distance.x * i,m_distance.y * i);
		if((!m_withoutTex && !m_radio[i].init(_XVector2(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y),
			m_mouseRect,* m_texture," ",m_caption,m_captionSize,m_textPosition))
			|| (m_withoutTex && !m_radio[i].initWithoutTex(" ",m_caption,m_captionSize,m_mouseRect,m_textPosition)))
		{
			XDELETE_ARRAY(m_radio);
			XDELETE_ARRAY(m_checkPosition);
			return XFalse;
		}
		//�ڿؼ���������ע������Щ���
		_XCtrlManger.decreaseAObject(&(m_radio[i]));	
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&(m_radio[i]));
#endif
		m_radio[i].setState(XFalse);
	}
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjManger.getNowObjectSum());
#endif

	m_nowChoose = 0;
	m_radio[0].setState(XTrue);
	return XTrue;
}
_XBool _XRadios::setACopy(const _XRadios &temp)
{
	if(&temp == this) return XFalse;
	if(!temp.m_isInited) return XFalse;
	if(m_isInited) release();
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjManger.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;
	m_radioSum = temp.m_radioSum;	//ѡ�������
	m_nowChoose = temp.m_nowChoose;	//��ǰ��ѡ����ı��
	m_distance = temp.m_distance;	//��ǰ��ѡ����ı��

	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_texture = temp.m_texture;
	m_caption.setACopy(temp.m_caption);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_captionSize = temp.m_captionSize;
	m_textColor = temp.m_textColor;
	m_textPosition = temp.m_textPosition;
	m_radio = createArrayMem<_XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//����ڴ����ʧ�ܣ�ֱ�ӷ��ش���

	m_checkPosition = createArrayMem<_XVector2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XDELETE_ARRAY(m_radio);
		return XFalse;
	}
	for(int i = 0;i < m_radioSum;++i)
	{
		if(!m_radio[i].setACopy(temp.m_radio[i])) return XFalse;
		m_checkPosition[i] = temp.m_checkPosition[i];
		//�ڿؼ���������ע������Щ���
		_XCtrlManger.decreaseAObject(&(m_radio[i]));	
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&(m_radio[i]));
#endif
	}

	m_funStateChange = temp.m_funStateChange;			//�ؼ�״̬�ı�ʱ����
	m_pClass = temp.m_pClass;
	return XTrue;
}
_XVector2 _XRadios::getBox(int order)
{
	if(!m_isInited) return _XVector2::zero;
	_XVector2 ret;
	_XVector2 temp;
	//�������еİ�ť���ҵ�����Χ�Ŀ�ÿ�ζ��������᲻��̫��Ч��
	switch(order)
	{
	case 0:
		ret = m_radio[0].getBox(0);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(0);
			if(temp.x < ret.x) ret.x = temp.x;
			if(temp.y < ret.y) ret.y = temp.y;
		}
		return ret;
	case 1:
		ret = m_radio[0].getBox(1);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(1);
			if(temp.x > ret.x) ret.x = temp.x;
			if(temp.y < ret.y) ret.y = temp.y;
		}
		return ret;
	case 2:
		ret = m_radio[0].getBox(2);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(2);
			if(temp.x > ret.x) ret.x = temp.x;
			if(temp.y > ret.y) ret.y = temp.y;
		}
		return ret;
	case 3:
		ret = m_radio[0].getBox(3);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(3);
			if(temp.x < ret.x) ret.x = temp.x;
			if(temp.y > ret.y) ret.y = temp.y;
		}
		return ret;
	}
	return _XVector2::zero;
}
void _XRadios::setRadiosText(const char * temp)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		temp == NULL) return;
	int sum = getCharSum(temp,';');
	if(sum < 0) sum = 0;
	int len = strlen(temp);
	if(temp[len - 1] != ';') sum += 1;	//������';'����
	if(!setRadioSum(sum)) return;
	int offset = 0;
	for(int i = 0;i < sum;++ i)
	{
		len = getCharPosition(temp + offset,';');
		string tmpStr = temp + offset;
		if(len >= 0) tmpStr.at(len) = '\0';
		setRadioText(tmpStr.c_str(),i);
		offset += len + 1;
	}
}