inline _XCtrlObjType getCtrlTypeByString(const std::string &name)
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
	if(name == "CTRL_OBJ_XCHART") return CTRL_OBJ_XCHART;		//ͼ���
	if(name == "CTRL_OBJ_XTAB") return CTRL_OBJ_XTAB;			//��ǩ�ؼ�
	if(name == "CTRL_OBJ_XTEXT") return CTRL_OBJ_XTEXT;			//���ֿؼ�
	if(name == "CTRL_OBJ_XSIMPLELINE") return CTRL_OBJ_XSIMPLELINE;	//�ָ��߿ؼ�
	if(name == "CTRL_OBJ_PROGRESSRING") return CTRL_OBJ_PROGRESSRING;	//�����������
	if(name == "CTRL_OBJ_CALENDAR") return CTRL_OBJ_CALENDAR;		//�����ؼ�
	if(name == "CTRL_OBJ_XMENU") return CTRL_OBJ_XMENU;			//�˵���Ŀؼ�
	if(name == "CTRL_OBJ_XIMAGE") return CTRL_OBJ_XIMAGE;		//ͼƬ�Ŀؼ�
	if(name == "CTRL_OBJ_BTNBAR") return CTRL_OBJ_BTNBAR;		//��ť���Ŀؼ�
	if(name == "CTRL_OBJ_XTOOLBAR") return CTRL_OBJ_XTOOLBAR;		//�������Ŀؼ�
	if(name == "CTRL_OBJ_FUNCTION") return CTRL_OBJ_FUNCTION;	//��ͼ����	
	return CTRL_OBJ_NULL;
}
inline std::string getCtrlTypeByString(_XCtrlObjType type)
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
	case CTRL_OBJ_XCHART:return "CTRL_OBJ_XCHART";		//ͼ���
	case CTRL_OBJ_XTAB:return "CTRL_OBJ_XTAB";			//��ǩ�ؼ�
	case CTRL_OBJ_XTEXT:return "CTRL_OBJ_XTEXT";			//���ֿؼ�
	case CTRL_OBJ_XSIMPLELINE:return "CTRL_OBJ_XSIMPLELINE";	//�ָ��߿ؼ�
	case CTRL_OBJ_PROGRESSRING:return "CTRL_OBJ_PROGRESSRING";	//�����������
	case CTRL_OBJ_CALENDAR:return "CTRL_OBJ_CALENDAR";		//�����ؼ�
	case CTRL_OBJ_XMENU:return "CTRL_OBJ_XMENU";			//�˵���Ŀؼ�
	case CTRL_OBJ_XIMAGE:return "CTRL_OBJ_XIMAGE";		//ͼƬ�Ŀؼ�
	case CTRL_OBJ_BTNBAR:return "CTRL_OBJ_BTNBAR";		//��ť���Ŀؼ�
	case CTRL_OBJ_XTOOLBAR:return "CTRL_OBJ_XTOOLBAR";		//�������Ŀؼ�
	case CTRL_OBJ_FUNCTION:return "CTRL_OBJ_FUNCTION";	//��ͼ����
	}	
	return "";
}
inline std::string getCtrlNameByType(_XCtrlObjType type)
{
	switch(type)
	{
	case CTRL_OBJ_NULL:return "NULL";		//��Ч�����
	case CTRL_OBJ_BUTTON:return "_XButton";	//��ť���
	case CTRL_OBJ_BUTTONEX:return "_XButtonEx";	//��ť���
	case CTRL_OBJ_CHECK:return "_XCheck";		//��ѡ������
	case CTRL_OBJ_EDIT:return "_XEdit";		//���������
	case CTRL_OBJ_MOUSERIGHTBUTTONMENU:return "_XMouseRightButtonMenu";	//����Ҽ��˵������
	case CTRL_OBJ_SLIDER:return "_XSlider";	//�����������
	case CTRL_OBJ_RADIOS:return "_XRadios";	//��ѡ������
	case CTRL_OBJ_PROGRESS:return "_XProgress";	//�����������
	case CTRL_OBJ_MUTITEXT:return "_XMultiText";	//�����ı������
	case CTRL_OBJ_MUTILIST:return "_XMultiList";	//�����б��
	case CTRL_OBJ_COMBO:return "_XCombo";		//�����б������
	case CTRL_OBJ_DIRECTORYLIST:return "_XDirectoryList";	//·���б��
	case CTRL_OBJ_GROUP:return "_XGroup";		//Ⱥ���
	case CTRL_OBJ_POINTCTRL:return "_XPointCtrl";		//��ԭ
	case CTRL_OBJ_LINECTRL:return "_XLineCtrl";		//��ԭ
	case CTRL_OBJ_SLIDEREX:return "_XSliderEx";	//�����������
	case CTRL_OBJ_PASSWORDPAD:return "_XPasswordPad";	//���������
	case CTRL_OBJ_IMAGELIST:return "_XImageList";		//ͼ����п�
	case CTRL_OBJ_XCHART:return "_XChart";		//ͼ���
	case CTRL_OBJ_XTAB:return "_XTab";			//��ǩ�ؼ�
	case CTRL_OBJ_XTEXT:return "_XText";			//���ֿؼ�
	case CTRL_OBJ_XSIMPLELINE:return "_XSimpleLine";	//�ָ��߿ؼ�
	case CTRL_OBJ_PROGRESSRING:return "_XProgressRing";	//�����������
	case CTRL_OBJ_CALENDAR:return "_XCalendar";		//�����ؼ�
	case CTRL_OBJ_XMENU:return "_XMenu";			//�˵���Ŀؼ�
	case CTRL_OBJ_XIMAGE:return "_XImage";		//ͼƬ�Ŀؼ�
	case CTRL_OBJ_BTNBAR:return "_XButtonBar";		//��ť���Ŀؼ�
	case CTRL_OBJ_XTOOLBAR:return "_XToolBar";		//�������Ŀؼ�
	case CTRL_OBJ_FUNCTION:return "NULL";	//��ͼ����
	}	
	return "";
}