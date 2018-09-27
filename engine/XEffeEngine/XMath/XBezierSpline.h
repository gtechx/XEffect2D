#ifndef _JIA_XBEZIERSPLINE_
#define _JIA_XBEZIERSPLINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//���Բο�������
//https://sites.google.com/site/nurbsintersection/
////set up texture map for bezier surface
//glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts[0][0][0]);
//glEnable(GL_MAP2_TEXTURE_COORD_2);
//glEnable(GL_MAP2_VERTEX_3);
//glEnable(GL_AUTO_NORMAL);
#include "XVector2.h"
//���ڱ��������ߵļ����ص�
//1�����α��������ߣ���pe(���)��ps(�յ�)��pc(���Ƶ�)����ʱ����pcΪpe��ps���е�ʱ���ϵ�ķֲ��Ǿ��ȵ�
//2�����α��������ߣ���pe��ps��pc0��pc1����ʱ����pc0Ϊpe��ps��1/3�ǣ�pc1Ϊ2/3ʱ���ϵ�ķֲ��Ǿ��ȵ�
//3�������Ҫ��ȡ�����������ϵľ��ȷֲ��ĵ� ��Ҫ���㳤�ȣ��������ϴ�
namespace XE{
//Linear Bezier:
//B(t) = (1 - t) * P0 + t * P1;
//Quadratic Bezier:
//B(t) = (1 - t)^2 * P0 + 2 * t * (1 - t) * P1 + t^2 * P2;
//Cubic Bezier:
//B(t) = (1 - t)^3 * P0 + 3 * t * (1 - t)^2 * P1 + 3 * t^2(1 - t) * P2 + t^3 * P3;
//�򵥵Ĳ����ƶ��ı��������ߵ���
class XSimpleBezierSpline
{
protected:
	XVec2 m_pS;		//3�α����������е����
	XVec2 m_pSP;		//���ƫ�Ƶ�
	XVec2 m_pEP;		//�յ�ƫ�Ƶ�
	XVec2 m_pE;		//�յ�
	bool m_withOpti;
public:
	virtual void init(const XVec2& pStart, const XVec2& pSP, 
		const XVec2& pEP, const XVec2& pEnd, bool withOpti = false);
	virtual void init(float pSx, float pSy, float pSPx, float pSPy, 
		float pEPx, float pEPy, float pEx, float pEy, bool withOpti = false);
	virtual XVec2 getBezierSplineValue(float temp)const;	//temp����(0,1)
	virtual std::vector<XVec2> getPolygon()const
	{
		std::vector<XVec2> ret;
		ret.push_back(m_pS);
		ret.push_back(m_pSP);
		ret.push_back(m_pEP);
		ret.push_back(m_pE);
		return ret;
	}
	virtual ~XSimpleBezierSpline(){}
};
//�����α��������ߵĿ��Ƶ�Ϊֱ�ߵ�1/3ʱ�����Ǿ��ȷֲ���
//�����ƶ������α��������ߵ���
class XBezierSpline:public XSimpleBezierSpline
{
protected:
	XBool m_isEnd;				//�Ƿ��Ѿ��ƶ���ĩβ
	float m_speed;				//�����е���ƶ��ٶ�
	XVec2 m_curValue;		//��ǰ�ı�����ȡֵ
	float m_curAngle;			//��ǰ�ĽǶ�
	float m_curPosition;		//�����е�ĵ�ǰλ��
public:
	float m_centerValue;	//�е��ֵ�����ڱ��������߲���Ǳ�ǲ��֮��ı��������ߵ�λ��
	float m_partLenght;		//��ǰ��һ����ռ�ܳ��ȵı���

	float getBezierSplineAngle(float temp);		//Ŀǰ�ķ����Ƚ�ԭʼ������Ľ�����ѧ��ʽ�Ƶ�	
	XBool getIsEnd() const {return m_isEnd;}
	float getSpeed() const {return m_speed;}
	void setSpeed(float speed) {m_speed = speed;}
	XVec2 getCurValue() const {return m_curValue;}
	float getCurAngle() const {return m_curAngle;}
	float getCurPosition() const {return m_curPosition;}

	//���������߿����Զ��ƶ�ȡֵ
	void move(float timeDelay);	//�ƶ������еĵ�
	XBezierSpline();
	virtual ~XBezierSpline(){}
	void draw();

	virtual void init(const XVec2& pStart, const XVec2& pSP,
		const XVec2& pEP, const XVec2& pEnd, bool withOpti = false);
	virtual void init(float pSx,float pSy,float pSPx,float pSPy,
		float pEPx,float pEPy,float pEx,float pEy, bool withOpti = false);
	void reset();				//���������ƶ���
	//������Ϊ��ʵ������ֱ���˶�������Ľӿ�
private:
	double m_length;	//�ߵĳ���
	double m_startSpeed;
	double m_endSpeed;
//	XVec2 m_ts,m_tp,m_te;
	void calculate();
	//ͨ���ռ任ʱ��ķ�ʽ��Ч�ʽ����Ż�
	std::vector<double> m_tmpData;	//�ȷ�����
public:
	XVec2 getBezierSplineValueEx(float temp)
	{	
		if (m_withOpti) return getBezierSplineValue(getTEx(temp));
		else return getBezierSplineValue(getT(temp));
	}
	//Ŀǰ�ķ����Ƚ�ԭʼ������Ľ�����ѧ��ʽ�Ƶ�	
	float getBezierSplineAngleEx(float temp)
	{
		if (m_withOpti) return getBezierSplineAngle(getTEx(temp));
		else return getBezierSplineAngle(getT(temp));
	}
	double speedX(double t);
	double speedY(double t);
	double speed(double t);
	double getLength(double t);
	//double getLengthX(double t);
	double getLength()const{return m_length;}
	double getT(double t);  
	double getTEx(double t);  //��������㷨�����Ż�

	XBezierSpline getUpHalf()const;	//��ȡ�ϰ벿������
	XBezierSpline getDownHalf()const;	//��ȡ�ϰ벿������
};
//�򵥵Ĳ����ƶ��ı��������ߵ���
class XSimpleBezierSpline2
{
protected:
	XVec2 m_pS;		//�����������е����
	XVec2 m_pP;	//ƫ�Ƶ�
	XVec2 m_pE;		//�յ�
public:
	virtual XVec2 getBezierSplineValue(float temp)const;	//temp����(0,1)
	virtual void init(const XVec2& pStart,const XVec2& pP,const XVec2& pEnd);
	virtual void init(float pSx,float pSy,float pPx,float pPy,float pEx,float pEy);
	virtual std::vector<XVec2> getPolygon()const
	{
		std::vector<XVec2> ret;
		ret.push_back(m_pS);
		ret.push_back(m_pP);
		ret.push_back(m_pE);
		return ret;
	}
	virtual ~XSimpleBezierSpline2(){}
};
//�����α��������ߵĿ��Ƶ�Ϊֱ�ߵ��е�ʱ�����Ǿ��ȷֲ���
//�����ƶ��Ķ��α��������ߵ���
class XBezierSpline2:public XSimpleBezierSpline2
{
protected:
	XBool m_isEnd;				//�Ƿ��Ѿ��ƶ���ĩβ
	float m_speed;				//�����е���ƶ��ٶ�
	XVec2 m_curValue;		//��ǰ�ı�����ȡֵ
	float m_curAngle;			//��ǰ�ĽǶ�
	float m_curPosition;		//�����е�ĵ�ǰλ��
public:
	float m_centerValue;	//�е��ֵ�����ڱ��������߲���Ǳ�ǲ��֮��ı��������ߵ�λ��
	float m_partLenght;		//��ǰ��һ����ռ�ܳ��ȵı���

	/*float getLength(float tmp)	//tmpΪ[0 - 1]
	{
		const float csX = m_pP.x - m_pS.x;
        const float csY = m_pP.y - m_pS.y;
        const float nvX = m_pE.x - m_pP.x - csX;
        const float nvY = m_pE.y - m_pP.y - csY;
                        
        const float c0 = 4.0f * (csX * csX + csY * csY);
        const float c1 = 8.0f * (csX * nvX + csY * nvY);
        const float c2 = 4.0f * (nvX * nvX + nvY * nvY);
                        
        float ft = 0.0f;
        float f0 = 0.0f;
                        
		if(c2 == 0.0f)
		{
			if(c1 == 0.0f)
			{
				ft = sqrt(c0) * tmp;
				return ft;
			}else
			{
				ft = (2 / 3) * (c1 * tmp + c0) * sqrt(c1 * tmp + c0) / c1;
				f0 = (2 / 3) * c0 * sqrt(c0) / c1;
				return (ft - f0);
			}
		}else
        {
			const float sqrt_0 = sqrt(c2 * tmp * tmp + c1 * tmp + c0);
			const float sqrt_c0 = sqrt(c0);
			const float sqrt_c2 = sqrt(c2);
                                                
			ft = 0.25 * (2 * c2 * tmp + c1) * sqrt_0 / c2;
			if((0.5 * c1 + c2 * tmp) / sqrt_c2 + sqrt_0 >= 1e-10)
				ft +=0.5 * log((0.5 * c1 + c2 * tmp) / sqrt_c2 + sqrt_0) / sqrt_c2 * (c0 - 0.25 * c1 * c1 / c2);
                                
			f0 = 0.25 * (c1) * sqrt_c0 / c2;
			if((0.5 * c1) / sqrt_c2 + sqrt_c0 >= 1e-10)
				f0 += 0.5 * log((0.5 * c1) / sqrt_c2 + sqrt_c0) / sqrt_c2 * (c0 - 0.25 * c1 * c1 / c2);
                                
			return ft - f0;
        }
	}*/
private:
	double m_length;	//�ߵĳ���
	double ax,ay,bx,by;
	double A,B,C;
	//������һЩ���ٵ���ʱ����
	double D,E,sqrtA,sqrtC,powA;
public:
	void calculate();
public:
	double getLength(double t);  //����[0 - 1]֮��ĳ���
	double getLength()const{return m_length;}
	//ʹ��ţ�����߷����
	float speed(float t) { return sqrt(A * t * t + B * t + C); }
	double getT(float t);  //���㳤�ȵ��ٷֱ�֮���ת��
/*	float getTimeByDistance(float distance)
    {
		if(distance <= 0.0f) return 0.0f;
                       
		float arcLength;
		float diffArcLength;
		float curveLength = getLength(1.0f);
		float time = distance / curveLength;
		if(distance >= curveLength) return 1;
                        
        const float csX = m_pP.x - m_pS.x;
        const float csY = m_pP.y - m_pS.y;
        const float ecX = m_pE.x - m_pP.x;
        const float ecY = m_pE.y - m_pP.y;
        const float nvX = ecX - csX;
        const float nvY = ecY - csY;
        
        const float c0 = 4.0f * (csX * csX + csY * csY);
        const float c1 = 8.0f * (csX * nvX + csY * nvY);
        const float c2 = 4.0f * (nvX * nvX + nvY * nvY);
                        
        const float c025 = c0 - 0.25 * c1 * c1 / c2;
        const float f0Base = 0.25 * c1 * sqrt(c0) / c2;
        const float exp2 = 0.5 * c1 / sqrt(c2) + sqrt(c0);
        
        const float c00sqrt = sqrt(c0);
        const float c20sqrt = sqrt(c2);
        float c22sqrt;
                        
        float exp1;
        float ft;
        float ftBase;
                        
        float f0;
        int maxIterations = 100;
        
        if(c2 == 0)
        {
            if(c1 == 0)
            {
                do{
                    arcLength = c00sqrt * time;
                    diffArcLength = sqrt(abs((c2 * time * time + c1 * time + c0))) || 1e-10; 
                    time = time - (arcLength - distance) / diffArcLength;
                }while (abs(arcLength - distance) > 1e-10 && maxIterations--);
            }else
            {
                do{
                    arcLength = (2 / 3) * (c1 * time + c0) * sqrt(c1 * time + c0) / c1 - (2 / 3) * c0 * c00sqrt / c1; 
                    diffArcLength = sqrt(abs((c2 * time * time + c1 * time + c0))) || 1e-10;
                    time = time - (arcLength - distance) / diffArcLength;
                } while (abs(arcLength - distance) > 1e-10 && maxIterations--);
            }
        }else
        {
            do{
                c22sqrt = sqrt(abs(c2 * time * time + c1 * time + c0));
                exp1 = (0.5 * c1 + c2 * time) / c20sqrt + c22sqrt;
                ftBase = 0.25 * (2 * c2 * time + c1) * c22sqrt / c2;
                if(exp1 < 1e-10)  ft = ftBase;
                else  ft = ftBase + 0.5 * log((0.5 * c1 + c2 * time) / c20sqrt + c22sqrt) / c20sqrt * c025;
                                        
                if(exp2 < 1e-10) f0 = f0Base;
                else f0 = f0Base + 0.5 * log((0.5 * c1) / c20sqrt + c00sqrt) / c20sqrt * c025;
                                        
                arcLength = ft - f0;
                diffArcLength = c22sqrt || 1e-10; 
                time = time - (arcLength - distance) / diffArcLength;
            }while(abs(arcLength - distance) > 1e-10 && maxIterations--);
        }
                        
        return time;
    }*/
	float getBezierSplineAngle(float temp);		//Ŀǰ�ķ����Ƚ�ԭʼ������Ľ�����ѧ��ʽ�Ƶ�	
	XBool getIsEnd() const {return m_isEnd;}
	float getSpeed() const {return m_speed;}
	void setSpeed(float speed) {m_speed = speed;}
	XVec2 getCurValue() const {return m_curValue;}
	float getCurAngle() const {return m_curAngle;}
	float getCurPosition() const {return m_curPosition;}

	//���������߿����Զ��ƶ�ȡֵ
	void move(float timeDelay);	//�ƶ������еĵ�
	XBezierSpline2();
	void draw();

	void reset();				//���������ƶ���
	XBezierSpline2 getUpHalf()const;	//��ȡ�ϰ벿������
	XBezierSpline2 getDownHalf()const;	//��ȡ�ϰ벿������
};
namespace XMath
{
//��ȡ�������α��������ߵĽ���,���ص��ǽ��������
void getBez2IntersectPoints(const XBezierSpline2 &bez0,const XBezierSpline2 &bez1,std::vector<XVec2> &points);
//��ȡ�������α��������ߵĽ���,���ص��ǽ��������������еĹ�һ��λ��
void getBez2IntersectPointsEx(const XBezierSpline2 &bez0,const XBezierSpline2 &bez1,std::vector<XVec2> &points);
//��ȡ�������α��������ߵĽ��㣬���ؽ��������
void getBezIntersectPoints(const XBezierSpline &bez0,const XBezierSpline &bez1,std::vector<XVec2> &points);
//��ȡ�������α��������ߵĽ���,���ص��ǽ��������������еĹ�һ��λ��
void getBezIntersectPointsEx(const XBezierSpline &bez0,const XBezierSpline &bez1,std::vector<XVec2> &points);
//Ŀǰ��������㷨��Ч���Ϻ�׼ȷ������˵����Ҫ�Ż�
void getBezIntersectPoints(const XBezierSpline &bez,std::vector<XVec2> &points);		//��ȡ���α��������������Ƿ��н���
}

#if WITH_INLINE_FILE
#include "XBezierSpline.inl"
#endif
/*
#include <stdio.h>  
  
#include <math.h>  
  
#include <windows.h>  
  
  
  
//�ĸ����Ƶ�  
  
POINT P0={50,50}, P1={300,600}, P2={600,600}, P3={800,200};  
  
  
  
//�����ܳ���  
  
double total_length = 0.0;  
  
  
  
//���߷ָ�ķ���  
  
const int STEP = 70;  
  
  
  
//���ڱ�����Ƶ����ݵ�����  
  
POINT pixels[STEP];  
  
  
  
  
  
//-------------------------------------------------------------------------------------  
//x���귽��  
double beze_x(double t)  
{  
    double it = 1-t;  
    return it*it*it*P0.x + 3*it*it*t*P1.x + 3*it*t*t*P2.x + t*t*t*P3.x;  
}  
//-------------------------------------------------------------------------------------  
//y���귽��  
double beze_y(double t)  
{  
    double it = 1-t;  
    return it*it*it*P0.y + 3*it*it*t*P1.y + 3*it*t*t*P2.y + t*t*t*P3.y;  
}  
//-------------------------------------------------------------------------------------  
//x�����ٶȷ���  
double beze_speed_x(double t)  
{  
    double it = 1-t;  
    return -3*P0.x*it*it + 3*P1.x*it*it - 6*P1.x*it*t + 6*P2.x*it*t - 3*P2.x*t*t + 3*P3.x*t*t;  
}  
//-------------------------------------------------------------------------------------  
//y�����ٶȷ���  
double beze_speed_y(double t)  
{  
    double it = 1-t;  
    return -3*P0.y*it*it + 3*P1.y*it*it - 6*P1.y*it*t + 6*P2.y*it*t - 3*P2.y*t*t + 3*P3.y*t*t;  
}  
//-------------------------------------------------------------------------------------  
//�ٶȷ���  
double beze_speed(double t)  
{  
    double sx = beze_speed_x(t);  
    double sy = beze_speed_y(t);  
    return sqrt(sx*sx+sy*sy);  
}  
//-------------------------------------------------------------------------------------  
//���ȷ���,ʹ��Simpson�����㷨  
double beze_length(double t)  
{  
    //���ܳ��ȷ�Χ�ڣ�ʹ��simpson�㷨�ķָ���  
    #define TOTAL_SIMPSON_STEP  (10000)  
    //�ָ����  
    int stepCounts = (int)(TOTAL_SIMPSON_STEP*t);  
    if(stepCounts & 1) ++ stepCounts;    //ż��  
    if(stepCounts==0) return 0.0;  
    int halfCounts = stepCounts/2;  
    double sum1=0.0, sum2=0.0;  
    double dStep = t/stepCounts;  
    for(int i=0; i<halfCounts; ++i)  
    {  
        sum1 += beze_speed((2*i+1)*dStep);  
    }  
    for(int i=1; i<halfCounts; ++i)  
    {  
        sum2 += beze_speed((2*i)*dStep);  
    }  
    return (beze_speed(0.0)+beze_speed(1.0)+2*sum2+4*sum1)*dStep/3.0;  
}  
//-------------------------------------------------------------------------------------  
//����t�Ƶ��������˶��Ա���t'�ķ���(ʹ��ţ�����߷�)  
double beze_even(double t)  
{  
    double len = t*total_length; //���������������,��ʱ��Ӧ�����߳���  
    double t1=t, t2;  
    do  
    {  
        t2 = t1 - (beze_length(t1)-len)/beze_speed(t1);  
        if(abs(t1-t2)<0.0000001) break;  
        t1=t2;  
    }while(true);  
    return t2;  
}  

//-------------------------------------------------------------------------------------  
LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message)   
    {  
    case WM_TIMER:  
        {  
            static unsigned int nIndex = 0;  
            if(nIndex>=0 && nIndex<=STEP)  
            {  
                double t = (double)nIndex/STEP;  
                //��������˶���Ӧ��tֵ  
                t = beze_even(t);  

				//���ݱ��������ߺ��������ȡ�ô�ʱ��x,y����  
                double x = beze_x(t);  
                double y = beze_y(t);  

				//ȡ��  
                pixels[nIndex].x = (int)(x+0.5);  
                pixels[nIndex].y = (int)(y+0.5);  

				++ nIndex;  
                InvalidateRect(hWnd, 0, 0);  
            }  
            else  
            {  
                KillTimer(hWnd, 101);  
            }  
        }  
        break;  
    case WM_PAINT:  
        {  
            PAINTSTRUCT ps;  
            HDC hdc = BeginPaint(hWnd, &ps);  
            ::MoveToEx(hdc, P0.x, P0.y, 0);  
            LineTo(hdc, P1.x, P1.y);  
            LineTo(hdc, P2.x, P2.y);  
            LineTo(hdc, P3.x, P3.y);  
            for(int i=0; i<STEP; ++i)  
            {  
                const POINT &pt = pixels[i];  
                if(pt.x==0 && pt.y==0) break;  
                ::MoveToEx(hdc, pt.x-2, pt.y, 0);  
                ::LineTo(hdc, pt.x+3, pt.y);  
                ::MoveToEx(hdc, pt.x, pt.y-2, 0);  
                ::LineTo(hdc, pt.x, pt.y+3);  
            }  
            EndPaint(hWnd, &ps);  
        }  
        break;  
    case WM_DESTROY:  
        PostQuitMessage(0);  
        break;  
    default:  
        return DefWindowProc(hWnd, message, wParam, lParam);  
    }  
    return 0;  
}  

//-------------------------------------------------------------------------------------  
int APIENTRY WinMain(HINSTANCE hInstance,  
                     HINSTANCE hPrevInstance,  
                     LPTSTR    lpCmdLine,  
                     int       nCmdShow)  
{  
    //�����ܳ���  
    total_length = beze_length(1.0);  
  
    //ע�ᴰ����  
    WNDCLASSEX wcex;  
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));  

	wcex.cbSize = sizeof(WNDCLASSEX);   
    wcex.style          = CS_HREDRAW | CS_VREDRAW;  
    wcex.lpfnWndProc    = (WNDPROC)_WndProc;  
    wcex.hInstance      = hInstance;  
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);  
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);  
    wcex.lpszClassName  = "BezierClass";  
    RegisterClassEx(&wcex);  

	//��������  
    HWND hWnd = CreateWindow("BezierClass", "BezierDemo", WS_OVERLAPPEDWINDOW,  
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);  
    ShowWindow(hWnd, nCmdShow);  
    UpdateWindow(hWnd);  

	//��ջ��Ƶ�����  
    ZeroMemory(&pixels, sizeof(pixels));  

	//�趨��ʱˢ�¼�ʱ��  
    SetTimer(hWnd, 101, 10, 0);  

	//��Ϣѭ��  
    MSG msg;  
    while(GetMessage(&msg, NULL, 0, 0))   
    {  
        TranslateMessage(&msg);  
        DispatchMessage(&msg);  
    }  
    return (int) msg.wParam;  
  
}  
*/
}
#endif