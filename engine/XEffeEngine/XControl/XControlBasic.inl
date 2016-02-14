namespace XCtrl
{
	INLINE XCtrlObjType getCtrlTypeByString(const std::string &name)
	{
		if(name == "CTRL_OBJ_NULL") return CTRL_OBJ_NULL;
		if(name == "CTRL_OBJ_BUTTON") return CTRL_OBJ_BUTTON;	//��ť���
		if(name == "CTRL_OBJ_BUTTONEX") return CTRL_OBJ_BUTTONEX;	//��ť���
		if(name == "CTRL_OBJ_CHECK") return CTRL_OBJ_CHECK;		//��ѡ������
		if(name == "CTRL_OBJ_EDIT") return CTRL_OBJ_EDIT;		//���������
		if(name == "CTRL_OBJ_MOUSERIGHTBUTTONMENU") return CTRL_OBJ_MOUSERIGHTBUTTONMENU;	//����Ҽ��˵������
		if(name == "CTRL_OBJ_SLIDER") return CTRL_OBJ_SLIDER;	//�����������
		if(name == "CTRL_OBJ_RADIOS") return CTRL_OBJ_RADIOS;	//��ѡ������
		if(name == "CTRL_OBJ_PROGRESS") return CTRL_OBJ_PROGRESS;	//�����������
		if(name == "CTRL_OBJ_MUTITEXT") return CTRL_OBJ_MUTITEXT;	//�����ı������
		if(name == "CTRL_OBJ_MUTILIST") return CTRL_OBJ_MUTILIST;	//�����б��
		if(name == "CTRL_OBJ_COMBO") return CTRL_OBJ_COMBO;		//�����б������
		if(name == "CTRL_OBJ_DIRECTORYLIST") return CTRL_OBJ_DIRECTORYLIST;	//·���б��
		if(name == "CTRL_OBJ_GROUP") return CTRL_OBJ_GROUP;		//Ⱥ���
		if(name == "CTRL_OBJ_POINTCTRL") return CTRL_OBJ_POINTCTRL;		//��ԭ
		if(name == "CTRL_OBJ_LINECTRL") return CTRL_OBJ_LINECTRL;		//��ԭ
		if(name == "CTRL_OBJ_SLIDEREX") return CTRL_OBJ_SLIDEREX;	//�����������
		if(name == "CTRL_OBJ_PASSWORDPAD") return CTRL_OBJ_PASSWORDPAD;	//���������
		if(name == "CTRL_OBJ_IMAGELIST") return CTRL_OBJ_IMAGELIST;		//ͼ����п�
		if(name == "CTRL_OBJ_CHART") return CTRL_OBJ_CHART;		//ͼ���
		if(name == "CTRL_OBJ_SIMPLECHART") return CTRL_OBJ_SIMPLECHART;		//ͼ���
		if(name == "CTRL_OBJ_TAB") return CTRL_OBJ_TAB;			//��ǩ�ؼ�
		if(name == "CTRL_OBJ_TEXT") return CTRL_OBJ_TEXT;			//���ֿؼ�
		if(name == "CTRL_OBJ_SIMPLELINE") return CTRL_OBJ_SIMPLELINE;	//�ָ��߿ؼ�
		if(name == "CTRL_OBJ_PROGRESSRING") return CTRL_OBJ_PROGRESSRING;	//�����������
		if(name == "CTRL_OBJ_CALENDAR") return CTRL_OBJ_CALENDAR;		//�����ؼ�
		if(name == "CTRL_OBJ_MENU") return CTRL_OBJ_MENU;			//�˵���Ŀؼ�
		if(name == "CTRL_OBJ_IMAGE") return CTRL_OBJ_IMAGE;		//ͼƬ�Ŀؼ�
		if(name == "CTRL_OBJ_BTNBAR") return CTRL_OBJ_BTNBAR;		//��ť���Ŀؼ�
		if(name == "CTRL_OBJ_TOOLBAR") return CTRL_OBJ_TOOLBAR;		//�������Ŀؼ�
		if(name == "CTRL_OBJ_PROPERTYBOX") return CTRL_OBJ_PROPERTYBOX;		//�������Ŀؼ�
		if(name == "CTRL_OBJ_COLORCHOOSE") return CTRL_OBJ_COLORCHOOSE;		//�������Ŀؼ�
		if(name == "CTRL_OBJ_SUBWINDOW") return CTRL_OBJ_SUBWINDOW;		//�������Ŀؼ�
		if(name == "CTRL_OBJ_SOFTBOARD") return CTRL_OBJ_SOFTBOARD;		//�������Ŀؼ�
		if(name == "CTRL_OBJ_PARAMCTRL") return CTRL_OBJ_PARAMCTRL;		//�������Ŀؼ�
		if(name == "CTRL_OBJ_FUNCTION") return CTRL_OBJ_FUNCTION;	//��ͼ����	
		return CTRL_OBJ_NULL;
	}
	INLINE std::string getCtrlTypeByString(XCtrlObjType type)
	{
		switch(type)
		{
		case CTRL_OBJ_NULL:return "CTRL_OBJ_NULL";		//��Ч�����
		case CTRL_OBJ_BUTTON:return "CTRL_OBJ_BUTTON";	//��ť���
		case CTRL_OBJ_BUTTONEX:return "CTRL_OBJ_BUTTONEX";	//��ť���
		case CTRL_OBJ_CHECK:return "CTRL_OBJ_CHECK";		//��ѡ������
		case CTRL_OBJ_EDIT:return "CTRL_OBJ_EDIT";		//���������
		case CTRL_OBJ_MOUSERIGHTBUTTONMENU:return "CTRL_OBJ_MOUSERIGHTBUTTONMENU";	//����Ҽ��˵������
		case CTRL_OBJ_SLIDER:return "CTRL_OBJ_SLIDER";	//�����������
		case CTRL_OBJ_RADIOS:return "CTRL_OBJ_RADIOS";	//��ѡ������
		case CTRL_OBJ_PROGRESS:return "CTRL_OBJ_PROGRESS";	//�����������
		case CTRL_OBJ_MUTITEXT:return "CTRL_OBJ_MUTITEXT";	//�����ı������
		case CTRL_OBJ_MUTILIST:return "CTRL_OBJ_MUTILIST";	//�����б��
		case CTRL_OBJ_COMBO:return "CTRL_OBJ_COMBO";		//�����б������
		case CTRL_OBJ_DIRECTORYLIST:return "CTRL_OBJ_DIRECTORYLIST";	//·���б��
		case CTRL_OBJ_GROUP:return "CTRL_OBJ_GROUP";		//Ⱥ���
		case CTRL_OBJ_POINTCTRL:return "CTRL_OBJ_POINTCTRL";		//��ԭ
		case CTRL_OBJ_LINECTRL:return "CTRL_OBJ_LINECTRL";		//��ԭ
		case CTRL_OBJ_SLIDEREX:return "CTRL_OBJ_SLIDEREX";	//�����������
		case CTRL_OBJ_PASSWORDPAD:return "CTRL_OBJ_PASSWORDPAD";	//���������
		case CTRL_OBJ_IMAGELIST:return "CTRL_OBJ_IMAGELIST";		//ͼ����п�
		case CTRL_OBJ_CHART:return "CTRL_OBJ_CHART";		//ͼ���
		case CTRL_OBJ_SIMPLECHART:return "CTRL_OBJ_SIMPLECHART";	//��ť���
		case CTRL_OBJ_TAB:return "CTRL_OBJ_TAB";			//��ǩ�ؼ�
		case CTRL_OBJ_TEXT:return "CTRL_OBJ_TEXT";			//���ֿؼ�
		case CTRL_OBJ_SIMPLELINE:return "CTRL_OBJ_SIMPLELINE";	//�ָ��߿ؼ�
		case CTRL_OBJ_PROGRESSRING:return "CTRL_OBJ_PROGRESSRING";	//�����������
		case CTRL_OBJ_CALENDAR:return "CTRL_OBJ_CALENDAR";		//�����ؼ�
		case CTRL_OBJ_MENU:return "CTRL_OBJ_MENU";			//�˵���Ŀؼ�
		case CTRL_OBJ_IMAGE:return "CTRL_OBJ_IMAGE";		//ͼƬ�Ŀؼ�
		case CTRL_OBJ_BTNBAR:return "CTRL_OBJ_BTNBAR";		//��ť���Ŀؼ�
		case CTRL_OBJ_TOOLBAR:return "CTRL_OBJ_TOOLBAR";		//�������Ŀؼ�
		case CTRL_OBJ_PROPERTYBOX:return "CTRL_OBJ_PROPERTYBOX";		//���Կ�
		case CTRL_OBJ_COLORCHOOSE:return "CTRL_OBJ_COLORCHOOSE";		//���Կ�
		case CTRL_OBJ_SUBWINDOW:return "CTRL_OBJ_SUBWINDOW";		//���Կ�
		case CTRL_OBJ_SOFTBOARD:return "CTRL_OBJ_SOFTBOARD";		//���Կ�
		case CTRL_OBJ_PARAMCTRL:return "CTRL_OBJ_PARAMCTRL";		//���Կ�
		case CTRL_OBJ_FUNCTION:return "CTRL_OBJ_FUNCTION";	//��ͼ����
		}	
		return "";
	}
	INLINE std::string getCtrlNameByType(XCtrlObjType type)
	{
		switch(type)
		{
		case CTRL_OBJ_NULL:return "NULL";		//��Ч�����
		case CTRL_OBJ_BUTTON:return "XButton";	//��ť���
		case CTRL_OBJ_BUTTONEX:return "XButtonEx";	//��ť���
		case CTRL_OBJ_CHECK:return "XCheck";		//��ѡ������
		case CTRL_OBJ_EDIT:return "XEdit";		//���������
		case CTRL_OBJ_MOUSERIGHTBUTTONMENU:return "XMouseRightButtonMenu";	//����Ҽ��˵������
		case CTRL_OBJ_SLIDER:return "XSlider";	//�����������
		case CTRL_OBJ_RADIOS:return "XRadios";	//��ѡ������
		case CTRL_OBJ_PROGRESS:return "XProgress";	//�����������
		case CTRL_OBJ_MUTITEXT:return "XMultiText";	//�����ı������
		case CTRL_OBJ_MUTILIST:return "XMultiList";	//�����б��
		case CTRL_OBJ_COMBO:return "XCombo";		//�����б������
		case CTRL_OBJ_DIRECTORYLIST:return "XDirectoryList";	//·���б��
		case CTRL_OBJ_GROUP:return "XGroup";		//Ⱥ���
		case CTRL_OBJ_POINTCTRL:return "XPointCtrl";		//��ԭ
		case CTRL_OBJ_LINECTRL:return "XLineCtrl";		//��ԭ
		case CTRL_OBJ_SLIDEREX:return "XSliderEx";	//�����������
		case CTRL_OBJ_PASSWORDPAD:return "XPasswordPad";	//���������
		case CTRL_OBJ_IMAGELIST:return "XImageList";		//ͼ����п�
		case CTRL_OBJ_CHART:return "XChart";		//ͼ���
		case CTRL_OBJ_SIMPLECHART:return "XSimpleChart";		//ͼ���
		case CTRL_OBJ_TAB:return "XTab";			//��ǩ�ؼ�
		case CTRL_OBJ_TEXT:return "XText";			//���ֿؼ�
		case CTRL_OBJ_SIMPLELINE:return "XSimpleLine";	//�ָ��߿ؼ�
		case CTRL_OBJ_PROGRESSRING:return "XProgressRing";	//�����������
		case CTRL_OBJ_CALENDAR:return "XCalendar";		//�����ؼ�
		case CTRL_OBJ_MENU:return "XMenu";			//�˵���Ŀؼ�
		case CTRL_OBJ_IMAGE:return "XImage";		//ͼƬ�Ŀؼ�
		case CTRL_OBJ_BTNBAR:return "XButtonBar";		//��ť���Ŀؼ�
		case CTRL_OBJ_TOOLBAR:return "XToolBar";		//�������Ŀؼ�
		case CTRL_OBJ_PROPERTYBOX:return"XPropertyBox";
		case CTRL_OBJ_COLORCHOOSE:return"XColorChoose";
		case CTRL_OBJ_SUBWINDOW:return"XSubWindow";
		case CTRL_OBJ_SOFTBOARD:return"XSoftBoard";
		case CTRL_OBJ_PARAMCTRL:return"XParameterCtrl";
		case CTRL_OBJ_FUNCTION:return "NULL";	//��ͼ����
		}	
		return "";
	}
}