#ifndef _JIA_XBYTEFUN_
#define _JIA_XBYTEFUN_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "../XCommonDefine.h"
namespace XE{
union XBytes
{
	unsigned int bytes;
	struct 
	{
	unsigned int b00:1,b01:1,b02:1,b03:1,b04:1,b05:1,b06:1,b07:1;
	unsigned int b08:1,b09:1,b0a:1,b0b:1,b0c:1,b0d:1,b0e:1,b0f:1;
	unsigned int b10:1,b11:1,b12:1,b13:1,b14:1,b15:1,b16:1,b17:1;
	unsigned int b18:1,b19:1,b1a:1,b1b:1,b1c:1,b1d:1,b1e:1,b1f:1;
	}bits;
};
namespace XByte
{
	//bits operate
	inline int getBit(unsigned char ch,int pos)			//��ȡchĳһλ��ֵ
	{
		if(pos < 0 || pos >=8) return 0;
		//unsigned char temp = 0x01;
		//temp = temp << pos;
		//temp = temp & ch;
		//if(temp == 0) return 0;
		//else return 1;
		return ((0x01 << pos) & ch);
	}
	inline XBool setBit(unsigned char &ch,int pos,int state)	//����chĳһλ��ֵ
	{
		if(pos < 0 || pos >= 8) return XFalse;
	
		unsigned char temp = (0x01 << pos);
		if(state == 0) ch = ch & (~temp);
		else ch = ch | temp;
		return XTrue;
	}
	inline XBool setBitAnti(unsigned char &ch,int pos)		//��chĳһλ��ֵ�÷�
	{
		if(pos < 0 || pos >= 8) return XFalse;
		unsigned char temp = (0x01 << pos);
		//if(temp & (* ch) != 0) (* ch) = (* ch) | temp;
		if(getBit(ch,pos) == 0) ch = ch | temp;
		else ch = ch & (~temp);
		return XTrue;
	}
	//12345678��תΪ87654321��˳��ת
	inline void reverseChar(unsigned char &c)
	{
		 c = ( c & 0x55 ) << 1 | ( c & 0xAA ) >> 1;
		 c = ( c & 0x33 ) << 2 | ( c & 0xCC ) >> 2;
		 c = ( c & 0x0F ) << 4 | ( c & 0xF0 ) >> 4;
	}
	inline void setByteAnti(unsigned char &ch){ch = ~ch;}	//��ch��λȡ��
	template<class T> void setAnti(T& data){data = ~data;}
	//ȡch��posλ֮��len����λ����ֵ
	//unsigned char ch = 01101010;
	//getByteValue(ch,1,3) = 110
	inline int getByteValue(unsigned char ch,int pos,int len)
	{
		if(pos < 0 || pos >= 8) return 0;
		if(len <= 0 || pos + len > 8) return 0;
		unsigned char temp = ch << pos;
		temp = temp >> (pos + (8 - pos - len));
		return temp;
	}
	extern unsigned int getByteValue(unsigned char *ch,int byteSum,int pos,int len,bool flag = true);
}
}
#endif