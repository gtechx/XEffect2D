#ifndef XRADAR_DATA
#define XRADAR_DATA
namespace XE {
enum XRadarDataType
{
	RD_TYPE_NULL,		//��Ч������
	RD_TYPE_BLOB,		//��Ч������
};
struct XRadarData
{
	float angle;		//�Ƕȣ���λ����
	float distance;		//���룬��λ������
	XVec2 pos;		//��ǰ��λ��,��λ������
	XRadarDataType type;
	float r;			//��Ч�뾶
	float intensity;	//���ݵĿɿ���
	XRadarData()
		:angle(0.0f)
		, distance(0.0f)
		, type(RD_TYPE_NULL)
		, pos(0.0f)
		, r(1.0)
		, intensity(0.0f)
	{}
	void set(float a, float d)
	{
		angle = a;
		distance = d;
		type = RD_TYPE_BLOB;
	}
};
enum XRadarType
{
	TYPE_SILAN,		//˼��״�
	TYPE_BEIYANG,	//�����״�
	TYPE_FASHI,		//��ʯ�״�
};
class XRadarBase
{
protected:
public:
	virtual void getRadarData(std::vector<XRadarData> &data) = 0;
	virtual bool update() = 0;
	virtual ~XRadarBase() {}
};

}
#endif