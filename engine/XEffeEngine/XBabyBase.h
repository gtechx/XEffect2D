#ifndef _JIA_XBABYBASE_
#define _JIA_XBABYBASE_
namespace XE{
class XBabyBase
{
protected:
public:
	XBabyBase(){}
	virtual ~XBabyBase(){}
	virtual bool reset() = 0;	//��λ
	virtual bool needReset() = 0;	//�Ƿ���Ҫ���ø�λ
};
}
#endif