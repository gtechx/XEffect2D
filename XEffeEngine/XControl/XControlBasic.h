#ifndef _JIA_XCONTROLBASIC_
#define _JIA_XCONTROLBASIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "../XBasicClass.h"
#include "../XMouseAndKeyBoardDefine.h"
#include "../XObjectBasic.h" 

//说明：引入友元的目的是为了让控件管理器可以访问控件的绘图函数和消息处理函数，
//但是控件的实体被定义之后，定义的程序员不能自己调用这些函数，防止重复绘图和重
//复的消息处理，但是友元的引入造成了类结构设计的耦合度和复杂度，这里需要思考更
//好的解决方式
class _XControlManager;
//控件类的基类
class _XControlBasic:_XObjectBasic
{
	friend _XControlManager;
protected:
	_XRect m_mouseRect;		//控件的鼠标响应范围
	_XRect m_nowMouseRect;	//当前鼠标的响应范围，这个范围受到位置和大小的变化而变化
	_XVector2 m_size;		//控件的大小
	_XVector2 m_position;	//控件的位置
	_XFColor m_color;		//控件的颜色

	char m_isEnable;		//控件是否有效，无效的物件可以看到，但是却显示为不能操作的模式
	char m_isVisiable;		//控件是否可见，可见的物件才会显示出来
	char m_isActive;		//控件是否处于激活状态，激活的物件才能接收控制信号,同时允许多个物体被激活
	char m_isBeChoose;		//控件是否被选中，接收键盘操作，例如用table键选择(暂时尚未实现)，但是同时只有一个物件能成为焦点
	int m_objectID;			//控件的ID
public:
	_XRect getMouseRect() const {return m_mouseRect;}
	float getMouseRectWidth() const {return m_mouseRect.getWidth();}
	float getMouseRectHeight() const {return m_mouseRect.getHeight();}
	int getControlID() const {return m_objectID;}	//获取控件的ID
	void setActive(){m_isActive = 1;}
	void disActive(){m_isActive = 0;}		//设置控件不接收控制信息
	char getActive() const {return m_isActive;}
	void setVisiable(){m_isVisiable = 1;}
	void disVisiable(){m_isVisiable = 0;}	//设置控件不可见
	char getVisiable() const {return m_isVisiable;}
	int setACopy(const _XControlBasic & temp);
protected:	//设计为下面的函数不允许在外部调用
	virtual void draw() = 0;
	virtual void drawUp() = 0;	//用于描绘上一个层面的东西，控件分两个层面，底层和漂浮层，漂浮层也就是上层有：例如右键菜单，下拉菜单等
	virtual int mouseProc(float x,float y,_XMouseState mouseState) = 0;	//鼠标响应函数,关于这个函数的返回值的意义，各个空间类的定义尚不统一，有返回控件是否发生状态变更的，等等
	virtual int keyboardProc(int keyOrder,_XKeyState keyState) = 0;		//键盘响应函数

	virtual int canGetFocus(float x,float y) = 0;	//是否可以获得焦点
	virtual int canLostFocus(float x,float y) = 0;	//当前控件是否可以失去焦点，用于处理焦点抢夺问题
	virtual void setLostFocus() {m_isBeChoose = 0;}	//设置失去焦点
public:
	virtual void setPosition(const _XVector2& position) = 0;
	virtual void setPosition(float x,float y) = 0;
	virtual _XVector2 getPosition() const = 0;

	virtual void setSize(const _XVector2& size) = 0;
	virtual void setSize(float x,float y) = 0;
	virtual _XVector2 getSize() const = 0;

	float getAngle() const{return 0;}				//获取物件的角度
	void setAngle(float angle) {}					//设置物件的角度

	_XControlBasic();
	virtual ~_XControlBasic() {};
private:	//为了防止意外调用的错误，这里重载赋值操作符和赋值构造函数
	_XControlBasic(const _XControlBasic &temp);
	_XControlBasic& operator = (const _XControlBasic& temp);
};

/*
//需要实现的控件包括以下一些
1、按钮		Button			x
2、单行输入框	Edit		x	(目前还不能解决中文输入的问题)
3、进度条	Progress		x
4、滑动条	Slider			x
5、多列列表框 MutiList		x
6、单列列表框 MutiText		x
7、单选按钮	Radio			x
8、多选按钮 Check			x
9、下拉菜单	Combo			x
10、群组框	Group
11、页面（标签按钮） Tab
12、多行输入框	MutiEdit		(目前还不能解决中文输入的问题)
13、控件管理的类 用于统一管理所有空间直接的ID以及以内互斥关系
*/

#endif