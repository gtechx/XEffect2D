#include "XStdHead.h"
#include "XDataGuess.h"
namespace XE{
void XDataGuess::pushAData(const XVec2& data)
{
	if(!m_isInited) return;
	switch (m_mode)
	{
	case GUESS_MODLE_LINE:
		if(m_datas.size() == 0)
		{
			m_datas.push_back(data);
		}else
		{//��������Ƿ����ظ�
			XVec2& end = *(m_datas.end() - 1);
			if(end.x == data.x) end = data;
			else m_datas.push_back(data);
			if(m_datas.size() > 2) m_datas.pop_front();
		}
		break;
	case GUESS_MODLE_SECOND:
		if(m_datas.size() == 0)
		{
			m_datas.push_back(data);
		}else
		{//��������Ƿ����ظ�
			XVec2& end = *(m_datas.end() - 1);
			if(end.x == data.x) end = data;
			else m_datas.push_back(data);
			if(m_datas.size() > 3) m_datas.pop_front();
		}
		break;
	case GUESS_MODLE_LINE_FIT:
		if(m_datas.size() == 0)
		{
			m_datas.push_back(data);
		}else
		if(m_datas.size() == 1)
		{//��������Ƿ����ظ�
			XVec2& end = m_datas[0];
			if(end.x == data.x) end = data;
			else m_datas.push_back(data);
		}else
		{
			XVec2 tmp = data;
			tmp.y = (tmp.y + guessData(tmp.x)) * 0.5f;
			XVec2& end = *(m_datas.end() - 1);
			if(end.x == tmp.x) end = tmp;
			else m_datas.push_back(tmp);
			if(m_datas.size() > 2) m_datas.pop_front();
		}
		break;
	case GUESS_MODLE_SECOND_FIT:
		switch (m_datas.size())
		{
		case 0:m_datas.push_back(data);break;
		case 1:
		case 2:
			{
				XVec2& end = *(m_datas.end() - 1);
				if(end.x == data.x) end = data;
				else m_datas.push_back(data);
			}
			break;
		default:
			{
				XVec2 tmp = data;
				tmp.y = (tmp.y + guessData(tmp.x)) * 0.5f;
				XVec2& end = *(m_datas.end() - 1);
				if(end.x == tmp.x) end = tmp;
				else m_datas.push_back(tmp);
				if(m_datas.size() > 3) m_datas.pop_front();
			}
			break;
		}
		break;
	case GUESS_MODLE_PERIODIC:
		m_pDG->pushAData(data);	//Ϊ���ֲ�������Χ��ֵ

		m_cycDatas.pushAData(data,m_cycleInfo);
		if(m_cycDatas.getHaveHead()) m_cycDatasTmp.pushAData(data,m_cycleInfo);
		if(m_cycDatas.getIsComplete())
		{//������Ҫ������ݵĺϷ��ԣ��Ա��ڶ������Բ�����������
			if(!getIsInvalid(m_cycDatas))
			{
				for(int i = 0;i < static_cast<int>(m_oldCycData.size());++ i)
				{
					if(getIsInvalid(m_oldCycData[i]))
					{
						m_oldCycData.erase(m_oldCycData.begin() + i);
						-- i;
					}
				}
				if(m_oldCycData.size() >= 5) m_oldCycData.pop_front();
				m_oldCycData.push_back(m_cycDatas);
				m_isCycDataUpdate = true;

				m_cycleInfo.addAMaxValue(m_cycDatas.getMaxValue());
				m_cycleInfo.addAMinValue(m_cycDatas.getMinValue());
				m_cycleInfo.addAUpTime(m_cycDatas.getUpTime());
				m_cycleInfo.addADownTime(m_cycDatas.getDownTime());
			}

			m_cycDatas.clear();
			m_cycDatas = m_cycDatasTmp;
			m_cycDatasTmp.clear();
		}
		break;
	}
}
bool XDataGuess::getIsInvalid(const XCycleData &data,bool bSimple)const
{
	if(bSimple)
	{
		if(m_cycleInfo.getIsEnable())
		{
			if(abs(data.getStartValue() - m_cycleInfo.m_minValue) >
				m_cycleInfo.getAmplitude() * 0.01f)
			{
#ifdef WITH_DEBUG_INFO
				std::cout << "��Ч������" << std::endl;
#endif
				return true;
			}
		}	
	}else
	{
		if(data.getDownDatasSum() < 10 || data.getUpDatasSum() < 10)
		{
#ifdef WITH_DEBUG_INFO
			std::cout << "��Ч������" << std::endl;
#endif
			return true;
		}
		if(abs(data.getStartValue() - data.getMinValue()) >
			(data.getMaxValue() - data.getMinValue()) * 0.01f ||
			abs(data.getEndValue() - data.getMinValue()) >
			(data.getMaxValue() - data.getMinValue()) * 0.01f)
		{
#ifdef WITH_DEBUG_INFO
			std::cout << "��Ч������" << std::endl;
#endif
			return true;
		}
		if(m_cycleInfo.getIsEnable())
		{
			if(abs(data.getStartValue() - m_cycleInfo.m_minValue) >
				m_cycleInfo.getAmplitude() * 0.01f ||
				abs(data.getEndValue() - m_cycleInfo.m_minValue) >
				m_cycleInfo.getAmplitude() * 0.01f)
			{
#ifdef WITH_DEBUG_INFO
				std::cout << "��Ч������" << std::endl;
#endif
				return true;
			}
		}
	}
	return false;
}
float XDataGuess::guessData(float x)const
{
	if(!m_isInited) return 0.0f;
	switch (m_mode)
	{
	case GUESS_MODLE_LINE:
	case GUESS_MODLE_LINE_FIT:
		{
			float speed = (m_datas[1].y - m_datas[0].y) / (m_datas[1].x - m_datas[0].x);
			float curT = x - m_datas[1].x;
			return speed * curT + m_datas[1].y;
		}
		break;
	case GUESS_MODLE_SECOND:
	case GUESS_MODLE_SECOND_FIT:
		{
			float speed = (m_datas[2].y - m_datas[1].y) / (m_datas[2].x - m_datas[1].x);
			float tmpSpeed = (m_datas[1].y - m_datas[0].y) / (m_datas[1].x - m_datas[0].x);
			float acc = (speed - tmpSpeed) / (m_datas[2].x - m_datas[1].x);
			float curT = x - m_datas[2].x;
			return speed * curT + acc * curT * curT * 0.5f + m_datas[2].y;
		}
		break;
	case GUESS_MODLE_PERIODIC:
		if(m_oldCycData.size() == 0 || getIsInvalid(m_cycDatas,true))
		{
			return m_pDG->guessData(x);
		}else
		{
			float value = 0.0f;
			float time = x - m_cycDatas.getStartTime();
			float sum = 0.0f;
			float tmpValue;
			for(int i = 0;i < m_oldCycData.size();++ i)
			{
				if(m_oldCycData[i].getValue(time,tmpValue))
				{
					value += tmpValue;
					sum += 1.0f;
				}
			}
			if(sum > 0.0f)
			{
				if(m_cycleInfo.getIsEnable())
				{//������ݳ���Ԥ��̫����ʹ�ýϿ��׵Ĺ���
					float tmp = m_pDG->guessData(x);
					float result = value / sum;
					if(abs(tmp - result) > m_cycleInfo.getAmplitude() * 0.05f)
					{
						return tmp;	//�����ֵ����̫�࣬��ʹ�ýϱ��صķ�ʽ
					}else
						return result;
				}else
				{
					return value / sum;
				}				
			}else
				return m_pDG->guessData(x);
		}
		break;
	}
	return 0.0f;
}
bool XDataGuess::getCanGuess()const
{
	if(!m_isInited) return false;
	switch (m_mode)
	{
	case GUESS_MODLE_LINE:
	case GUESS_MODLE_LINE_FIT:
		return m_datas.size() >= 2;
	case GUESS_MODLE_SECOND:
	case GUESS_MODLE_SECOND_FIT:
		return m_datas.size() >= 3;
	case GUESS_MODLE_PERIODIC:
		if(m_oldCycData.size() == 0)
			return m_pDG->getCanGuess();
		else 
			return true;
		break;
	}
	return false;
}
void XDataGuess::release()
{
	XMem::XDELETE(m_pDG);
}
bool XDataGuess::init(XGuessModle mode)
{
	if(m_isInited) return false;
	m_mode = mode;
	switch (m_mode)
	{
	case GUESS_MODLE_PERIODIC:
		m_pDG = XMem::createMem<XDataGuess>();
		if(!m_pDG->init(GUESS_MODLE_LINE_FIT)) return false;
		break;
	}
	m_isInited = true;
	return true;
}
//����������ݶ�����������߼��������⣬��Ҫ�����佡׳��
void XCycleData::pushAData(const XE::XVec2& data,const XCycleInfo &info)
{
	if(getIsComplete()) return;	//����Ѿ��������������ٲ�������
	if(m_datas.size() == 0)
	{
		m_datas.push_back(data);
		m_maxValue = data.y;
		m_minValue = data.y;
	}else
	{
		XVec2& tmp = *(m_datas.end() - 1);
		if(tmp.x == data.x) tmp = data;
		else m_datas.push_back(data);
		if(data.y > m_maxValue) m_maxValue = data.y;
		if(data.y < m_minValue) m_minValue = data.y;

		if(m_datas.size() >= 2)
		{//�����ж����ڵ�����
			XVec2 endOne = *(m_datas.end() - 1);
			XVec2 endTwo = *(m_datas.end() - 2);
			if(endOne.y < endTwo.y)
			{//������Ҫ���㶥�����Ϣ
				m_state = CYC_STA_DOWN;
				if(calTop())//������㶥��
					m_upTime = m_topTime - m_startTime;
			}else
			if(endOne.y > endTwo.y)
			{
				if(calEnd())
				{
					m_downTime = m_endTime - m_topTime;
					//�������m_downDatasSum �� m_upDatasSum
					m_downDatasSum = m_endIndex - m_topIndex;
					m_upDatasSum = m_topIndex - m_startIndex;
				}
				calStart(info);

				m_state = CYC_STA_UP;
			}
		}
	}
}
bool XCycleData::getValue(float x,float &y)const
{
	float startT = getStartTime();
	x += startT;
	for(auto it = m_datas.begin();it != m_datas.end() - 1;++ it)
	{
		auto nextT = it + 1;
		if(it->x <= x && nextT->x >= x)
		{
			y = XMath::mapping1D(x, it->x, nextT->x, it->y, nextT->y);
			return true;
		}
	}
	return false;
}
bool XCycleData::calStart(const XCycleInfo &info)	//���㿪ͷ
{
	if(m_haveStart) return false;
	int downSum = 0;
	int upSum = 0;
	int errorSum = 0;	//�����쳣����������
	for(auto it = m_datas.begin();it != m_datas.end() - 1;++ it)
	{
		auto nextT = it + 1;
		if(it->y < nextT->y)
		{
			downSum = 0;
			//���Զ���֮ǰ������
			m_datas.erase(m_datas.begin(), it);
			continue;
		}
		if(it->y == nextT->y) continue;
		if(it->y > nextT->y) ++ downSum;
		if(downSum >= m_turnThreshold)
		{
			int flag = 0;
			errorSum = 0;
			upSum = 0;
			for(auto jt = it + 1;jt != m_datas.end() - 1;++ jt)
			{
				auto nextJT = jt + 1;
				if(it->y == nextJT->y) continue;
				if(jt->y < nextJT->y)
				{//����Ҫ��
					++ upSum;
					errorSum = 0;
					if(upSum == 1)
						m_startEndIndex = nextJT - m_datas.begin();
					if(upSum >= m_turnThreshold)
					{
						flag = 1;
						break;
					}
				}else
				{//���ڶ��������
					upSum = 0;
					++ errorSum;
					if(errorSum >= m_turnThreshold)
					{//�����쳣�����������������ƣ��������������
						flag = 2;
						break;
					}
				}
			}
			if(flag == 0) return false;	//��������������û���㹻�����ȷ��Ҳû���㹻��Ĵ���
			if(flag == 1)
			{
				m_haveStart = true;
#ifdef WITH_DEBUG_INFO
				std::cout<<"Start"<<std::endl;
#endif
				m_startIndex = ((it - m_datas.begin()) + m_startEndIndex) >> 1;
				auto startEndT = m_datas.begin() + m_startEndIndex;
				//������Ҫ������ʼ��ʱ��
				if(info.getIsEnable())
				{
					float a = it->y - info.m_minValue;
					float b = (startEndT->y - info.m_minValue) * info.m_upDownRate;
					if(a < 0.0f) a = 0.0f;
					if(b < 0.0f) b = 0.0f;
					float c = a + b;
					if(c == 0.0f)
						m_startTime = (it->x + startEndT->x) * 0.5f;
					else
					{
						m_startTime = it->x + (startEndT->x - it->x) * a / c;
					}
				}else
				{
					m_startTime = (it->x + startEndT->x) * 0.5f;
				}
				m_startValue = it->y;
				for(auto jt = it + 1;jt != startEndT;++ jt)
				{
					if(jt->y < m_startValue) m_startValue = jt->y;
				}
				break;
			}else
			if(flag == 2)
			{//��Ҫ�����������ֵ
				m_datas.pop_front();
				downSum = 0;
				it = m_datas.begin();
			}
		}
	}
	//assert(m_haveStart);
	return m_haveStart;
}
bool XCycleData::calEnd()		//�������
{
	if(m_haveEnd || !m_haveStart || !m_haveTop) return false;
	int downSum = 0;
	int upSum = 0;
	int errorSum = 0;	//�����쳣����������
	for(auto it = m_datas.begin() + m_startEndIndex;it != m_datas.end() - 1;++ it)
	{
		auto nextT = it + 1;
		if(it->y < nextT->y)
		{
			downSum = 0;
			continue;
		}
		if(it->y == nextT->y) continue;
		if(it->y > nextT->y) ++ downSum;
		if(downSum >= m_turnThreshold)
		{//�½���������Ҫ��
			bool flag = false;
			int endIndex = 0;
			errorSum = 0;
			upSum = 0;
			for(auto jt = it + 1;jt != m_datas.end() - 1;++ jt)
			{
				auto nextJT = jt + 1;
				if(jt->y == nextJT->y) continue;
				if(jt->y < nextJT->y)
				{//����Ҫ��
					errorSum = 0;
					++ upSum;
					if(upSum == 1)
						endIndex = nextJT - m_datas.begin();
					if(upSum >= m_turnThreshold)
					{//�ҵ����
						flag = true;
						m_haveEnd = true;
#ifdef WITH_DEBUG_INFO
						std::cout<<"End"<<std::endl;
#endif
						m_endIndex = ((it - m_datas.begin()) + endIndex) >> 1;
						m_endTime = (it->x + m_datas[m_endIndex].x) * 0.5f;	//��������Ż�
						m_endValue = it->y;
						auto et = m_datas.begin() + endIndex;
						for(auto kt = it + 1;kt != et;++ kt)
						{
							if(kt->y < m_endValue) m_endValue = kt->y;
						}
						return true;
					}
				}else
				{
					upSum = 0;
					++ errorSum;
					if(errorSum >= m_turnThreshold)
					{//̫��Ĵ���
						flag = true;
						-- downSum;
						break;
					}
				}
			}
			if(!flag)
			{//û���㹻�����ȷ���ߴ�������Ϊ������������
				return false;
			}
		}
	}
	//assert(m_haveEnd);
	return m_haveEnd;
}
bool XCycleData::calTop()
{
	if(m_haveTop || !m_haveStart) return false;
	int endIndex = 0;
	int downSum = 0;
	int upSum = 0;
	int errorSum = 0;	//�����쳣����������
	for(auto it = m_datas.end() - 1;it != m_datas.begin();-- it)
	{
		auto nextT = it - 1;
		if(it->y >= nextT->y) return false;	//����Ҫ�ټ�������
		if(it->y < nextT->y) ++ downSum;
		if(downSum >= m_turnThreshold)
		{
			upSum = 0;
			errorSum = 0;
			for(auto jt = it - 1;jt != m_datas.begin();-- jt)
			{
				auto nextJT = jt - 1;
				if(jt->y < nextJT->y)
				{
					upSum = 0;
					++ errorSum;
					if(errorSum >= m_turnThreshold)
						return false;
				}else
				if(jt->y > nextJT->y)
				{
					++ upSum;
					errorSum = 0;
					if(upSum == 1) endIndex = nextJT - m_datas.begin();
					if(upSum >= m_turnThreshold)
					{//����Ҫ��
						m_haveTop = true;
#ifdef WITH_DEBUG_INFO
						std::cout<<"Top"<<std::endl;
#endif
						m_topIndex = ((it - m_datas.begin()) + endIndex) >> 1;
						m_topTime = (it->x + m_datas[m_topIndex].x) * 0.5f;	//��������Ż�
						return true;
					}
				}
			}
			return false;
		}
	}
	return m_haveTop;
}
}