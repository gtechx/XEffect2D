#include "XStdHead.h"
#include "XTimeLine.h"
namespace XE{
XFrameType XOneTimeLine::getFrameType(int index)
{
	if(m_keyFrames.size() <= 0) return FRAME_TYPE_BLANK;
	if(index < m_keyFrames[0].frameIndex) return FRAME_TYPE_BLANK;
	int upKeyFrame = -1;	//��һ���ؼ�֡�ı��
	if(index > m_keyFrames[(int)(m_keyFrames.size()) - 1].frameIndex)
	{
		if(m_keyFrames[(int)(m_keyFrames.size()) - 1].timeLen >= index * m_frameTimeWidth - m_keyFrames[(int)(m_keyFrames.size()) - 1].time)
			return FRAME_TYPE_LAST;
		return FRAME_TYPE_BLANK;
	}
	for(unsigned int i = 0;i < m_keyFrames.size();++ i)
	{
		if(m_keyFrames[i].frameIndex == index) return FRAME_TYPE_KEY;
		if(m_keyFrames[i].frameIndex < index)
		{//��¼��ǰ��һ���ؼ�֡
			upKeyFrame = i;
		}else
		{//�ж��Ƿ�Ϊ����֡
			if(upKeyFrame >= 0 && m_keyFrames[upKeyFrame].interpolationMode != MD_MODE_NULL) return FRAME_TYPE_INTER;
			if(m_keyFrames[upKeyFrame].timeLen >= index * m_frameTimeWidth - m_keyFrames[upKeyFrame].time)
			{
				return FRAME_TYPE_LAST;
			}else 
				return FRAME_TYPE_BLANK;
		}
	}
	//���������������ֵ�Ĺؼ�֮֡����Ϊ�հ�֡
	return FRAME_TYPE_INTER;
}
XBool XOneTimeLine::init(const std::string& name,int width,const XVec2& position,const XVec2& scale)
{
	if(m_isInited) return XFalse;

	m_name = name;
	m_position = position;
	m_scale = scale;
	m_width = width;

	m_title.setACopy(getDefaultFont());
	m_title.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_title.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_title.setString(XString::getCanShowString(name.c_str(),TIMELINE_TITLE_LEN,16.0f).c_str());
	m_title.setPosition(position);

	m_isInited = XTrue;
	return XTrue;
}
void XOneTimeLine::setFps(int fps)
{
	m_frameTimeWidth = 1000.0f / fps;
	for(unsigned int i = 0;i < m_keyFrames.size();++ i)
	{
		m_keyFrames[i].frameIndex = (int)(m_keyFrames[i].time / m_frameTimeWidth);
	}
}
const XOneFrame *XOneTimeLine::getCurKeyFrame(float time)	//��ǰ���ڵĹؼ�֡
{
	for(unsigned int  i = 0;i < m_keyFrames.size();++ i)
	{
		if(m_keyFrames[i].time == (int)time) return &m_keyFrames[i];
		if(m_keyFrames[i].time > time)
		{
			if(i > 0) return &m_keyFrames[i - 1];
			else return NULL;
		}
	}
	if(m_keyFrames.size() > 0) return &m_keyFrames[(int)(m_keyFrames.size()) - 1];	//������
	else return NULL;
}
const XOneFrame *XOneTimeLine::getNextKeyFrame(float time)	//��ȡ��һ���ؼ�֡��ָ��
{
	for(unsigned int  i = 0;i < m_keyFrames.size();++ i)
	{
		if(m_keyFrames[i].time > time) return &m_keyFrames[i];
	}
	return NULL;
}
void XOneTimeLine::resetAllFrameFlag()
{
	for(unsigned int  i = 0;i < m_keyFrames.size();++ i)
	{
		m_keyFrames[i].overFlag = false;
	}
}
void XOneTimeLine::setAllFrameFlag()
{
	for(unsigned int  i = 0;i < m_keyFrames.size();++ i)
	{
		m_keyFrames[i].overFlag = true;
	}
}
void XOneTimeLine::clearAllFrame()	//������е�֡
{
	for(unsigned int  i = 0;i < m_keyFrames.size();++ i)
	{
		XMem::XDELETE(m_keyFrames[i].pData);
	}
	m_keyFrames.clear();
}
void XOneTimeLine::draw()
{
	if(!m_isInited) return;
	//���滭����
	int sum = (m_width - TIMELINE_TITLE_LEN) / TIMELINE_FRAME_WIDTH;
	XVec2 pos(m_position.x + TIMELINE_TITLE_LEN * m_scale.x,m_position.y);
	for(int i = 0;i < sum;++ i)
	{
		switch(getFrameType(m_startIndex + i))
		{
		case FRAME_TYPE_BLANK:	//�հ�֡
			XRender::drawFillRect(XVec2(pos.x + i * TIMELINE_FRAME_WIDTH * m_scale.x,pos.y),
				XVec2(TIMELINE_FRAME_WIDTH,TIMELINE_HEIGHT) * m_scale,1.0f,1.0f,1.0f,true);
			break;
		case FRAME_TYPE_INTER:	//����֡
			XRender::drawFillRect(XVec2(pos.x + i * TIMELINE_FRAME_WIDTH * m_scale.x,pos.y),
				XVec2(TIMELINE_FRAME_WIDTH,TIMELINE_HEIGHT) * m_scale,0.5f,0.5f,1.0f,true);
			break;
		case FRAME_TYPE_KEY:	//�ؼ�֡
			XRender::drawFillRect(XVec2(pos.x + i * TIMELINE_FRAME_WIDTH * m_scale.x,pos.y),
				XVec2(TIMELINE_FRAME_WIDTH,TIMELINE_HEIGHT) * m_scale,1.0f,0.5f,0.5f,true);
			break;
		case FRAME_TYPE_LAST:	//����֡
			XRender::drawFillRect(XVec2(pos.x + i * TIMELINE_FRAME_WIDTH * m_scale.x,pos.y),
				XVec2(TIMELINE_FRAME_WIDTH,TIMELINE_HEIGHT) * m_scale,0.75f,0.75f,0.75f,true);
			break;
		}
	}
	m_title.draw();
}
void XOneTimeLine::delAKeyFrame(int time)
{
	int index = time / m_frameTimeWidth;
	for(unsigned int i = 0;i < m_keyFrames.size();++ i)
	{//Ŀǰʹ�ø��ǵĲ���
		if(m_keyFrames[i].frameIndex == index)
		{
			XMem::XDELETE(m_keyFrames[i].pData);
		//	for(int j = i;j < (int)(m_keyFrames.size()) - 1;++ j)
		//	{
		//		m_keyFrames[j] = m_keyFrames[j + 1];
		//	}
		//	m_keyFrames.pop_back();
			m_keyFrames.erase(m_keyFrames.begin() + i);
			++ i;
		}
	}
}
void XOneTimeLine::addAKeyFrame(int time,int timeLen,XFrameDataBasic * data,XMoveDataMode mode)
{//����һ���ؼ�֡
	if(time < 0) return;
	//���潨���ؼ�֡(��δ���)
	//������Ҫ���ǹؼ�֡���ǵ����⣬��������ǣ�������ͬһ�ؼ�֡��Ҫ���ɾ������Ч
	//������������ɾɵĹؼ�֡��ʧ
	XOneFrame tmp;
	tmp.time = time;
	tmp.timeLen = timeLen;
	tmp.frameIndex = time / m_frameTimeWidth;
	tmp.interpolationMode = mode;
	tmp.pData = data;
	bool isInsert = false;
	for(unsigned int i = 0;i < m_keyFrames.size();++ i)
	{//Ŀǰʹ�ø��ǵĲ���
		if(m_keyFrames[i].frameIndex == tmp.frameIndex)
		{
			XMem::XDELETE(m_keyFrames[i].pData);
			m_keyFrames[i] = tmp;
			isInsert = true;
			break;
		}
		if(m_keyFrames[i].frameIndex > tmp.frameIndex)
		{//ע���������Ѿ�����ʱ���ź����
			m_keyFrames.push_back(tmp);
			for(int j = (int)(m_keyFrames.size()) - 1;j > i;-- j)
			{
				m_keyFrames[j] = m_keyFrames[j - 1];
			}
			m_keyFrames[i] = tmp;
			isInsert = true;
			break;
		}
	}
	if(!isInsert) m_keyFrames.push_back(tmp);
}
void XOneTimeLine::moveKeyFrame(int fromTime,int toTime)
{//��δ��ɣ�BUG��
	int fromIndex = fromTime / m_frameTimeWidth;	//����λ��
	int toIndex = toTime / m_frameTimeWidth;		//��Ҫ�ƶ�����λ��
	if(fromIndex == toIndex) return;	//û�з����ƶ�
	float time = (toIndex - fromIndex) * m_frameTimeWidth;	//ʱ����
	for(unsigned int i = 0;i < m_keyFrames.size();++ i)
	{
		if(m_keyFrames[i].frameIndex == fromIndex)
		{//�ƶ���Ŀ��֮��
			XOneFrame tmp = m_keyFrames[i];
			tmp.time += time;
			tmp.frameIndex = tmp.time / m_frameTimeWidth;
			if(time >= 0)
			{//����ƶ�
				bool flag = false;
				for(int j = i;j < (int)(m_keyFrames.size()) - 1;++ j)
				{
					if(m_keyFrames[j + 1].time < tmp.time)
					{
						m_keyFrames[j] = m_keyFrames[j + 1];
					}else
					{
						m_keyFrames[j] = tmp;
						flag = true;
						break;	//����
					}
				}
				if(!flag) m_keyFrames[(int)(m_keyFrames.size()) - 1] = tmp;
			}else
			{
				bool flag = false;
				for(int j = i;j > 0;-- j)
				{
					if(m_keyFrames[j - 1].time > tmp.time)
					{
						m_keyFrames[j] = m_keyFrames[j - 1];
					}else
					{
						m_keyFrames[j] = tmp;
						flag = true;
						break;	//����
					}
				}
				if(!flag) m_keyFrames[0] = tmp;
			}
		}
		if(m_keyFrames[i].frameIndex > fromIndex) break;
	}
}
void XTimeLines::ctrlProc(void*pClass,int id,int eventID)
{
	XTimeLines &pPar = *(XTimeLines *)pClass;
	if(id == pPar.m_fpsEdt.getControlID())
	{//����֡��
		if(eventID == XEdit::EDT_INPUT_OVER)
		{
			if(pPar.m_fpsEdt.getEdtIsNumber())
			{//���ݺϷ�
				pPar.setFps(pPar.m_fpsEdt.getAsFloat());
			}else
			{
				pPar.m_fpsEdt.setString(XString::toString(pPar.m_fps).c_str());
			}
		}
	}else
	if(id == pPar.m_allTimeEdt.getControlID())
	{//����֡��
		if(eventID == XEdit::EDT_INPUT_OVER)
		{
			if(pPar.m_allTimeEdt.getEdtIsNumber())
			{//���ݺϷ�
				pPar.setAllTime(pPar.m_allTimeEdt.getAsFloat());
			}else
			{
				pPar.m_allTimeEdt.setString(XString::toString(pPar.m_allTime).c_str());
			}
		}
	}else
	if(id == pPar.m_headBtn.getControlID())
	{
		if(eventID == XButton::BTN_MOUSE_DOWN) pPar.setToPrev();
	}else
	if(id == pPar.m_endBtn.getControlID())
	{
		if(eventID == XButton::BTN_MOUSE_DOWN) pPar.setToNext();
	}else
	if(id == pPar.m_playOrStopBtn.getControlID())
	{
		if(eventID == XButton::BTN_MOUSE_DOWN)
		{
			if(pPar.m_isPlaying) pPar.stop();
			else pPar.play();
		}
	}else
	if(id == pPar.m_timeSld.getControlID())
	{//�������϶�
		if(eventID == XSlider::SLD_MOUSE_MOVE || eventID == XSlider::SLD_VALUE_CHANGE) 
			pPar.setCurTimeSlider(pPar.m_timeSld.getCurValue());
	}else
	if(id == pPar.m_timeLineSld.getControlID())
	{//�������϶�
		if(eventID == XSlider::SLD_MOUSE_MOVE || eventID == XSlider::SLD_VALUE_CHANGE) 
			pPar.setCurTimeLineSlider(pPar.m_timeLineSld.getCurValue());
	}
}
void XTimeLines::updateCtrlsPosition()
{
	int h = m_position.y + (m_showTimeLineSum + 2) * TIMELINE_HEIGHT * m_scale.y;
	m_curTimeTxt.setPosition(m_position.x + 700.0f * m_scale.x,h);
	m_curTimeTxt.setScale(m_scale);
	m_fpsTxt.setPosition(m_position.x,h);
	m_fpsTxt.setScale(m_scale);
	m_fpsEdt.setPosition(m_position.x + 64.0f * m_scale.x,h);
	m_fpsEdt.setScale(m_scale);
	m_allTimeTxt.setPosition(m_position.x + 128.0f * m_scale.x,h);
	m_allTimeTxt.setScale(m_scale);
	m_allTimeEdt.setPosition(m_position.x + 236.0f * m_scale.x,h);
	m_allTimeEdt.setScale(m_scale);

	m_headBtn.setPosition(m_position.x + 400.0f * m_scale.x,h);
	m_headBtn.setScale(m_scale);
	m_playOrStopBtn.setPosition(m_position.x + 464.0f * m_scale.x,h);
	m_playOrStopBtn.setScale(m_scale);
	m_endBtn.setPosition(m_position.x + 528.0f * m_scale.x,h);
	m_endBtn.setScale(m_scale);
	m_timeSld.setPosition(m_position.x + TIMELINE_TITLE_LEN * m_scale.x,h - TIMELINE_HEIGHT * m_scale.y);
	m_timeSld.setScale(m_scale);
	m_timeLineSld.setPosition(m_position.x + m_width * m_scale.x,m_position.y + TIMELINE_HEIGHT * m_scale.y);
	m_timeLineSld.setScale(m_scale);
}
void XTimeLines::updateRule()
{
	int startTime = m_curStartIndex * m_frameTime * 0.001f;
	int endTime = (m_curStartIndex + m_canShowFrameSum) * m_frameTime * 0.001f;
	XRuleInfo tmp;
	m_ruleInfos.clear();
	switch(endTime - startTime)
	{
	case 0:	//ʲô��û��
//		m_needShowRuleHead = false;
//		m_needShowRuleMiddle = false;
//		m_needShowRuleEnd = false;
		break;
	case 1:	//��ʾͷ
		tmp.ruleData = startTime + 1;
		tmp.offset = (tmp.ruleData * m_fps - m_curStartIndex) * TIMELINE_FRAME_WIDTH + TIMELINE_TITLE_LEN;
		tmp.rulePosition.set(m_position.x + tmp.offset * m_scale.x,m_position.y);
		m_ruleInfos.push_back(tmp);
//		m_ruleHead.setString(XString::toString(startTime + 1).c_str());
//		m_ruleHeadOffset = ((startTime + 1) * m_fps - m_curStartIndex) * TIMELINE_FRAME_WIDTH + TIMELINE_TITLE_LEN;
//		m_ruleHead.setPosition(m_position.x + m_ruleHeadOffset * m_scale.x,m_position.y);
//		m_needShowRuleHead = true;
//		m_needShowRuleMiddle = false;
//		m_needShowRuleEnd = false;
		break;
	case 2:	//��ʾͷ��β
		tmp.ruleData = startTime + 1;
		tmp.offset = (tmp.ruleData * m_fps - m_curStartIndex) * TIMELINE_FRAME_WIDTH + TIMELINE_TITLE_LEN;
		tmp.rulePosition.set(m_position.x + tmp.offset * m_scale.x,m_position.y);
		m_ruleInfos.push_back(tmp);
		tmp.ruleData = endTime;
		tmp.offset = (tmp.ruleData * m_fps - m_curStartIndex) * TIMELINE_FRAME_WIDTH + TIMELINE_TITLE_LEN;
		tmp.rulePosition.set(m_position.x + tmp.offset * m_scale.x,m_position.y);
		m_ruleInfos.push_back(tmp);
//		m_ruleHead.setString(XString::toString(startTime + 1).c_str());
//		m_ruleHeadOffset = ((startTime + 1) * m_fps - m_curStartIndex) * TIMELINE_FRAME_WIDTH + TIMELINE_TITLE_LEN;
//		m_ruleHead.setPosition(m_position.x + m_ruleHeadOffset * m_scale.x,m_position.y);
//		m_needShowRuleHead = true;
//		m_needShowRuleMiddle = false;
//		m_ruleEnd.setString(XString::toString(endTime).c_str());
//		m_ruleEndOffset = (endTime * m_fps - m_curStartIndex) * TIMELINE_FRAME_WIDTH + TIMELINE_TITLE_LEN;
//		m_ruleEnd.setPosition(m_position.x + m_ruleEndOffset * m_scale.x,m_position.y);
//		m_needShowRuleEnd = true;
		break;
	default:
		for(int i = startTime + 1;i <= endTime;++ i)
		{
			tmp.ruleData = i;
			tmp.offset = (tmp.ruleData * m_fps - m_curStartIndex) * TIMELINE_FRAME_WIDTH + TIMELINE_TITLE_LEN;
			tmp.rulePosition.set(m_position.x + tmp.offset * m_scale.x,m_position.y);
			m_ruleInfos.push_back(tmp);
		}
//		m_ruleHead.setString(XString::toString(startTime + 1).c_str());
//		m_ruleHeadOffset = ((startTime + 1) * m_fps - m_curStartIndex) * TIMELINE_FRAME_WIDTH + TIMELINE_TITLE_LEN;
//		m_ruleHead.setPosition(m_position.x + m_ruleHeadOffset * m_scale.x,m_position.y);
//		m_needShowRuleHead = true;
//		m_ruleMiddle.setString(XString::toString((startTime + 1 + endTime) >> 1).c_str());
//		m_ruleMiddleOffset = (((startTime + 1 + endTime) >> 1) * m_fps - m_curStartIndex) * TIMELINE_FRAME_WIDTH + TIMELINE_TITLE_LEN;
//		m_ruleMiddle.setPosition(m_position.x + m_ruleMiddleOffset * m_scale.x,m_position.y);
//		m_needShowRuleMiddle = true;
//		m_ruleEnd.setString(XString::toString(endTime).c_str());
//		m_ruleEndOffset = (endTime * m_fps - m_curStartIndex) * TIMELINE_FRAME_WIDTH + TIMELINE_TITLE_LEN;
//		m_ruleEnd.setPosition(m_position.x + m_ruleEndOffset * m_scale.x,m_position.y);
//		m_needShowRuleEnd = true;
		break;
	}
}
XBool XTimeLines::addATimeLine(const std::string& name)
{
	if(getTimeLineIndexByName(name) >= 0) return XFalse;	//�ظ���ʱ��֡
	//�������ʱ��֡
	XOneTimeLine *tmp = XMem::createMem<XOneTimeLine>();
	if(tmp == NULL) return XFalse;
	tmp->init(name,m_width,XVec2(m_position.x,m_position.y + ((int)(m_timeLines.size()) + 1) * TIMELINE_HEIGHT),m_scale);
	tmp->setFps(m_fps);
	tmp->setCurStartIndex(m_curStartIndex);
	if((m_timeLines.size() >= m_curStartTimeLineIndex || m_curStartTimeLineIndex < 0) && 
		(m_timeLines.size() < m_curStartTimeLineIndex + m_showTimeLineSum && m_curStartTimeLineIndex + m_showTimeLineSum >= 0))
	{//��Ҫ��ʾ��ʱ����
		tmp->setPosition(XVec2(m_position.x,m_position.y + ((int)(m_timeLines.size()) + 1 - m_curStartTimeLineIndex) * TIMELINE_HEIGHT));
	}
	m_timeLines.push_back(tmp);
	if(m_timeLines.size() > m_showTimeLineSum || m_showTimeLineSum < 0) m_timeLineSld.setVisible();
	else m_timeLineSld.disVisible();
	updateCtrlsPosition();
	return XTrue;
}
int XTimeLines::getTimeLineIndexByName(const std::string& name)
{
	for(unsigned int i = 0;i < m_timeLines.size();++ i)
	{
		if(m_timeLines[i]->m_name == name) return i;
	}
	return -1;
}
void XTimeLines::setFps(int fps)
{
	if(fps <= 0) return;	//�Ƿ�������
	m_fps = fps;
	m_frameTime = 1000.0f / fps;
	for(unsigned int i = 0;i < m_timeLines.size();++ i)
	{
		m_timeLines[i]->setFps(m_fps);
	}
	m_fpsEdt.setString(XString::toString(m_fps).c_str());

	m_canShowTime = (m_width - TIMELINE_TITLE_LEN) / TIMELINE_FRAME_WIDTH * m_frameTime;
	if(m_visible)
	{
		if(m_allTime <= m_canShowTime) m_timeSld.disVisible();
		else m_timeSld.setVisible();
	}
	setCurTimeSlider(m_timeSld.getCurValue());
}
void XTimeLines::setAllTime(int time)
{
	if(time <= 0) return;
	m_allTime = time;
	if(m_curTimer > m_allTime)
	{
		m_curTimer = 0;
		if(m_funTimeChange != NULL) m_funTimeChange(m_pClass,m_objectID);	//ע�������ID������Ŀǰ�����û�и���ؼ��Ļ��࣬��������ʹ�õ���һ�����õ�ֵ
		m_timeSld.setCurValue(0.0f,true);
	}
	if(m_visible)
	{
		if(m_allTime <= m_canShowTime) m_timeSld.disVisible();
		else m_timeSld.setVisible();
	}
	m_allTimeEdt.setString(XString::toString(m_allTime).c_str());
	setCurTimeSlider(m_timeSld.getCurValue());
}
XBool XTimeLines::init(float width,int showLineSum,int fps)
{
	if(m_isInited ||
		width < 256.0f || fps <= 0 || showLineSum < 0) return XFalse;	//�Ƿ�������

	m_width = width;
	m_showTimeLineSum = showLineSum;
	m_curStartTimeLineIndex = 0;
	setFps(fps);

	m_position.reset();
	m_scale.set(1.0f);

	m_timeLines.clear();

	m_curTimeTxt.init(("��ǰʱ��:" + XString::toString(m_curTimer)).c_str());

	m_fpsTxt.init("FPS:");
	m_fpsEdt.initWithoutSkin(64.0f,XString::toString(m_fps).c_str());
	m_fpsEdt.setEventProc(ctrlProc,this);
	m_allTimeTxt.init("��ʱ��:");
	m_allTimeEdt.initWithoutSkin(128.0f,XString::toString(m_allTime).c_str());
	m_allTimeEdt.setEventProc(ctrlProc,this);

	m_headBtn.initWithoutSkin("<<",64.0f);
	m_headBtn.setEventProc(ctrlProc,this);
	m_playOrStopBtn.initWithoutSkin(">",64.0f);
	m_playOrStopBtn.setEventProc(ctrlProc,this);
	m_endBtn.initWithoutSkin(">>",64.0f);
	m_endBtn.setEventProc(ctrlProc,this);

	m_timeSld.initWithoutSkin(XVec2(m_width - TIMELINE_TITLE_LEN,TIMELINE_HEIGHT));
	m_timeSld.setWithAction(XFalse);
	m_timeSld.setEventProc(ctrlProc,this);
	m_timeLineSld.initWithoutSkin(XVec2(TIMELINE_SLD_WIDTH,TIMELINE_HEIGHT * m_showTimeLineSum),100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	m_timeLineSld.setWithAction(XFalse);
	m_timeLineSld.setEventProc(ctrlProc,this);
	updateCtrlsPosition();
	m_canShowFrameSum = (m_width - TIMELINE_TITLE_LEN) / TIMELINE_FRAME_WIDTH;

	m_ruleFont.setACopy(getDefaultFont());
	m_ruleFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
//	m_ruleHead.setACopy(getDefaultFont());
//	m_ruleHead.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
//	m_ruleMiddle.setACopy(getDefaultFont());
//	m_ruleMiddle.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
//	m_ruleEnd.setACopy(getDefaultFont());
//	m_ruleEnd.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	updateRule();

	setAllTime(2000);
	setFps(30);

	m_isInited = XTrue;
	return XTrue;
}
void XTimeLines::draw()
{
	if(!m_isInited) return;
	if(!m_visible) return;
	XRender::drawFillRectEx(m_position,XVec2(m_width + TIMELINE_SLD_WIDTH,TIMELINE_HEIGHT) * m_scale,0.5f,0.5f,0.5f,false,false,true);	//ʱ����ʾ��
	//�������ʱ���ߵ��ܵ�
	for(int i = m_curStartTimeLineIndex;i < m_timeLines.size();++ i)
	{
		if(i >= m_curStartTimeLineIndex + m_showTimeLineSum) break;
		if(m_curChooseIndex == i) 
			XRender::drawFillRect(m_position + XVec2(0.0f,(i - m_curStartTimeLineIndex + 1) * TIMELINE_HEIGHT * m_scale.y),
				XVec2(m_width + TIMELINE_SLD_WIDTH,TIMELINE_HEIGHT) * m_scale,0.3f,0.3f,0.3f,true);	//��ʾ����Ĺ�����
		else 
			XRender::drawFillRect(m_position + XVec2(0.0f,(i - m_curStartTimeLineIndex + 1) * TIMELINE_HEIGHT * m_scale.y),
				XVec2(m_width + TIMELINE_SLD_WIDTH,TIMELINE_HEIGHT) * m_scale,0.5f,0.5f,0.5f,true);	//��ʾ����Ĺ�����
	}
	XRender::drawFillRect(m_position + XVec2(0.0f,(m_showTimeLineSum + 1) * TIMELINE_HEIGHT * m_scale.y),
		XVec2(m_width + TIMELINE_SLD_WIDTH,TIMELINE_HEIGHT) * m_scale,0.5f,0.5f,0.5f,true);	//��ʾ����Ĺ�����
	XRender::drawFillRectEx(m_position + XVec2(0.0f,(m_showTimeLineSum + 2) * TIMELINE_HEIGHT * m_scale.y),
		XVec2(m_width + TIMELINE_SLD_WIDTH,TIMELINE_HEIGHT) * m_scale,0.5f,0.5f,0.5f);	//��ʾ����Ĺ�����

	//������ʾʱ����(��δ���)
	//�������ʱ����
	for(int i = m_curStartTimeLineIndex;i < m_timeLines.size();++ i)
	{
		if(i >= m_curStartTimeLineIndex + m_showTimeLineSum) break;
		m_timeLines[i]->draw();
	}
	//��浱ǰ��ʱ����
	if(m_timeLines.size() > 0 && m_curTimer >= m_curStartIndex * m_frameTime && m_curTimer <= m_curStartIndex * m_frameTime + m_canShowTime)
	{
		float offset = (m_curTimer * 0.001f * m_fps - m_curStartIndex) * TIMELINE_FRAME_WIDTH * m_scale.x;
		XRender::drawLine(m_position.x + TIMELINE_TITLE_LEN * m_scale.x + offset,m_position.y + TIMELINE_HEIGHT * m_scale.y,
			m_position.x + TIMELINE_TITLE_LEN * m_scale.x + offset,m_position.y + (m_showTimeLineSum + 1) * TIMELINE_HEIGHT * m_scale.y,
			3.0f,XFColor(0.5f,0.5f,0.0f,1.0f));
	}
	//�����
	int tmpI = XMath::toIntCeil(m_curStartIndex * m_frameTime * 0.01f);
	int sum = XMath::toIntFloor(m_canShowFrameSum * m_frameTime * 0.01f);
	float distance = 0.1f;
	while(sum > 100)
	{
		distance *= 2.0f;
		sum = sum >> 1;
	}
	float offset = (TIMELINE_TITLE_LEN + (tmpI * 0.1f - m_curStartIndex * m_frameTime * 0.001f) * m_fps * TIMELINE_FRAME_WIDTH) * m_scale.x;
	float tmpOffset;
	for(int i = 0;i < sum;++ i)
	{
		tmpOffset = offset + i * distance * m_fps * TIMELINE_FRAME_WIDTH * m_scale.x;
		XRender::drawLine(m_position + XVec2(tmpOffset,TIMELINE_HEIGHT * m_scale.y),
			m_position + XVec2(tmpOffset,(TIMELINE_HEIGHT - 4) * m_scale.y),1.0f,XFColor::blue);
	}
	for(int i = 0;i < m_ruleInfos.size();++ i)
	{
		XRender::drawLine(m_position + XVec2(m_ruleInfos[i].offset,TIMELINE_HEIGHT) * m_scale,
			m_position + XVec2(m_ruleInfos[i].offset,TIMELINE_HEIGHT - 5) * m_scale,1.0f,XFColor::red);
		m_ruleFont.setString(XString::toString(m_ruleInfos[i].ruleData).c_str());
		m_ruleFont.setPosition(m_ruleInfos[i].rulePosition);
		m_ruleFont.draw();
	}
	//if(m_needShowRuleHead)
	//{
	//	XRender::drawLine(m_position.x + m_ruleHeadOffset * m_scale.x,m_position.y + TIMELINE_HEIGHT * m_scale.y,
	//		m_position.x + m_ruleHeadOffset * m_scale.x,m_position.y + (TIMELINE_HEIGHT - 5) * m_scale.y,1.0f,1.0f,0.0f,0.0f,1.0f);
	//	m_ruleHead.draw();
	//}
	//if(m_needShowRuleMiddle)
	//{
	//	XRender::drawLine(m_position.x + m_ruleMiddleOffset * m_scale.x,m_position.y + TIMELINE_HEIGHT * m_scale.y,
	//		m_position.x + m_ruleMiddleOffset * m_scale.x,m_position.y + (TIMELINE_HEIGHT - 5) * m_scale.y,1.0f,1.0f,0.0f,0.0f,1.0f);
	//	m_ruleMiddle.draw();
	//}
	//if(m_needShowRuleEnd)
	//{
	//	XRender::drawLine(m_position.x + m_ruleEndOffset * m_scale.x,m_position.y + TIMELINE_HEIGHT * m_scale.y,
	//		m_position.x + m_ruleEndOffset * m_scale.x,m_position.y + (TIMELINE_HEIGHT - 5) * m_scale.y,1.0f,1.0f,0.0f,0.0f,1.0f);
	//	m_ruleEnd.draw();
	//}
	if(m_isMouseDrag && m_isMouseMove)
	{//�����괦����ק״̬�������������λ����ʾһ����ק��Ŀ���
		XRender::drawCross(getMousePos(),10.0f,1.0f,0.0f,0.0f,1.0f);
	}
}
void XTimeLines::move(float stepTime)
{
	if(!m_isPlaying) return;
	m_curTimer += stepTime;
	if(m_curTimer >= m_allTime)
	{
		m_curTimer = m_allTime;
		stop();
	}
	if(m_funTimeChange != NULL) m_funTimeChange(m_pClass,m_objectID);	//ע�������ID������Ŀǰ�����û�и���ؼ��Ļ��࣬��������ʹ�õ���һ�����õ�ֵ
	//�������û���������//�Ż�Ϊ�����볡�ٸ���
	if(m_curTimer * 0.001f * m_fps - m_curStartIndex > (m_canShowFrameSum >> 1))
		m_timeSld.setCurValue((float)(m_curTimer - m_canShowTime * 0.5f) / (float)(m_allTime - m_canShowTime) * 100.0f,
			true);// setCurTimeSlider((float)m_curTimer / (float)(m_allTime - m_canShowTime) * 100.0f);
	m_curTimeTxt.setString(("��ǰʱ��:" + XString::toString(m_curTimer)).c_str());
}
void XTimeLines::release()
{
	if(!m_isInited) return;
	for(unsigned int i = 0;i < m_timeLines.size();++ i)
	{
		XMem::XDELETE(m_timeLines[i]);
	}
	m_timeLines.clear();
	m_isInited = XFalse;
}
XBool XTimeLines::mouseProc(const XVec2& p,XMouseState mouseState)
{
	if(!m_visible) return XFalse;
	if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
		&& !m_isPlaying)
	{//����ڷ�Χ������ת���ƶ���֡
		XRect tmp(m_position.x + TIMELINE_TITLE_LEN * m_scale.x,m_position.y + TIMELINE_HEIGHT * m_scale.y,
			m_position.x + m_width * m_scale.x,m_position.y + (m_showTimeLineSum + 1) * TIMELINE_HEIGHT * m_scale.y);
		if(tmp.isInRect(p))
		{//���õ�ǰ��ʱ��
			m_curTimer = (m_curStartIndex + (int)((p.x - tmp.left) / (TIMELINE_FRAME_WIDTH * m_scale.x)) + 0.5f) * m_frameTime;
			m_curChooseIndex = (p.y - tmp.top) / (TIMELINE_HEIGHT * m_scale.y) + m_curStartTimeLineIndex;
			if(m_curTimer > m_allTime) m_curTimer = m_allTime;
			if(m_funTimeChange != NULL) m_funTimeChange(m_pClass,m_objectID);	//ע�������ID������Ŀǰ�����û�и���ؼ��Ļ��࣬��������ʹ�õ���һ�����õ�ֵ
			m_curTimeTxt.setString(("��ǰʱ��:" + XString::toString(m_curTimer)).c_str());
			//���������ק
			if(m_curChooseIndex >= 0 && m_curChooseIndex < m_timeLines.size() && 
				m_timeLines[m_curChooseIndex]->getFrameType(m_curTimer * 0.001f * m_fps) == FRAME_TYPE_KEY)
			{//ֻ�йؼ�֡�����϶�
				m_isMouseDrag = true;
				m_mouseDragTLIndex = m_curChooseIndex;
				m_mouseDragKFTime = m_curTimer;
				m_isMouseMove = false;
			}
		}
		return XTrue;
	}
	if(mouseState == MOUSE_MOVE)
	{
		if(m_isMouseDrag)
		{//���������ק
			m_isMouseMove = true;
		}
	}
	if(mouseState == MOUSE_LEFT_BUTTON_UP)
	{
		if(m_isMouseDrag)
		{//��������ƶ�
			if(m_isMouseMove)
			{//��Ҫ�϶��˲ż���
				XRect tmp(m_position.x + TIMELINE_TITLE_LEN * m_scale.x,m_position.y + TIMELINE_HEIGHT * m_scale.y,
					m_position.x + m_width * m_scale.x,m_position.y + (m_showTimeLineSum + 1) * TIMELINE_HEIGHT * m_scale.y);
				if(tmp.isInRect(p))
				{
					int toTime = (m_curStartIndex + (int)((p.x - tmp.left) / (TIMELINE_FRAME_WIDTH * m_scale.x)) + 0.5f) * m_frameTime;
					m_timeLines[m_curChooseIndex]->moveKeyFrame(m_mouseDragKFTime,toTime);
				}
			}
			m_isMouseDrag = false;
		}
	}
	return XFalse;
}
void XTimeLines::play()
{
	if(m_isPlaying) return;
	m_isPlaying = true;
	//������Ҫ���ø����ؼ���״̬
	m_fpsEdt.disable();
	m_allTimeEdt.disable();
	m_playOrStopBtn.setCaptionText("��");	//���ź�ֹͣ��ť
	m_headBtn.disable();		//�ص�ͷ�İ�ť
	m_endBtn.disable();		//�ص�β�İ�ť
	m_timeSld.disable();		//ʱ��֡�Ļ�����
	if(m_funPlay != NULL) m_funPlay(m_pClass,m_objectID);
}
void XTimeLines::stop()
{
	if (!m_isPlaying) return;
	m_isPlaying = false;
	//�ظ������ؼ���״̬
	m_fpsEdt.enable();
	m_allTimeEdt.enable();
	m_playOrStopBtn.setCaptionText(">");	//���ź�ֹͣ��ť
	m_headBtn.enable();		//�ص�ͷ�İ�ť
	m_endBtn.enable();		//�ص�β�İ�ť
	m_timeSld.enable();		//ʱ��֡�Ļ�����
	if (m_funStop != NULL) m_funStop(m_pClass, m_objectID);
}
void XTimeLines::setPosition(const XVec2& p)
{
	m_position = p;
	updateCtrlsPosition();
	for(int i = m_curStartTimeLineIndex;i < m_timeLines.size();++ i)
	{
		if(i >= m_curStartTimeLineIndex + m_showTimeLineSum) break;
		m_timeLines[i]->setPosition(XVec2(p.x,p.y + (i + 1 - m_curStartTimeLineIndex) * TIMELINE_HEIGHT * m_scale.y));
	}
//	m_ruleHead.setPosition(m_position.x + m_ruleHeadOffset * m_scale.x,m_position.y);
//	m_ruleMiddle.setPosition(m_position.x + m_ruleMiddleOffset * m_scale.x,m_position.y);
//	m_ruleEnd.setPosition(m_position.x + m_ruleEndOffset * m_scale.x,m_position.y);
	for(int i = 0;i < m_ruleInfos.size();++ i)
	{
		m_ruleInfos[i].rulePosition.set(m_position.x + m_ruleInfos[i].offset * m_scale.x,m_position.y);
	}
}
void XTimeLines::setSize(const XVec2& s)
{
	if(s.x < 0.0f || s.y < 0.0f) return;	//�Ƿ�������
	m_scale = s;
	m_ruleFont.setScale(m_scale);
//	m_ruleHead.setScale(m_scale);
//	m_ruleMiddle.setScale(m_scale);
//	m_ruleEnd.setScale(m_scale);
	setPosition(m_position);
	for(unsigned int i = 0;i < m_timeLines.size();++ i)
	{
		m_timeLines[i]->setScale(m_scale);
	}
}
void XTimeLines::setCurTimeSlider(float p)	//���õ�ǰ�İٷֱ�λ��
{
	m_curStartIndex = ((m_allTime - m_canShowTime) * p * 0.01f) * 0.001f * m_fps;
	if(m_curStartIndex < 0) m_curStartIndex = 0;
	//�������������֡�� 
	for(unsigned int i =0;i < m_timeLines.size();++ i)
	{
		m_timeLines[i]->setCurStartIndex(m_curStartIndex);
	}
	updateRule();
}
void XTimeLines::setCurTimeLineSlider(float p)	//���õ�ǰ�İٷֱ�λ��
{
	m_curStartTimeLineIndex = (((int)(m_timeLines.size()) - m_showTimeLineSum) * p * 0.01f);
	if(m_curStartTimeLineIndex < 0) m_curStartTimeLineIndex = 0;
	//�������������֡�� 
	for(int i = m_curStartTimeLineIndex;i < m_timeLines.size();++ i)
	{
		if(i >= m_curStartTimeLineIndex + m_showTimeLineSum) break;
		m_timeLines[i]->setPosition(XVec2(m_position.x,
			m_position.y + (i + 1 - m_curStartTimeLineIndex) * TIMELINE_HEIGHT * m_scale.y));
	}
}
void XTimeLines::setToPrev()	//��ǰ��һҳ
{
	if(m_timeSld.getVisible()) 
	{
		m_curStartIndex -= m_canShowFrameSum;
		if(m_curStartIndex < 0) m_curStartIndex = 0;
		int allFrame = m_allTime * 0.001f * m_fps;
		m_timeSld.setCurValue((float)m_curStartIndex /(float)(allFrame - m_canShowFrameSum) * 100.0f,
			true);
		updateRule();
	}else
	{
		m_curStartIndex = 0;
	}
}	
void XTimeLines::setToNext()	//�����һҳ
{
	if(m_timeSld.getVisible()) 
	{
		m_curStartIndex += m_canShowFrameSum;
		int allFrame = m_allTime * 0.001f * m_fps;
		if(m_curStartIndex > (allFrame - m_canShowFrameSum)) m_curStartIndex = allFrame - m_canShowFrameSum;
		m_timeSld.setCurValue((float)m_curStartIndex /(float)(allFrame - m_canShowFrameSum) * 100.0f,
			true);
		updateRule();
	}else
	{
		m_curStartIndex = 0;
	}
}
XBool XTimeLines::saveToFile(const char * filename)
{//����Ӧ�ÿ��Ǳ���֡�ʺ��ʱ���һЩ����
	std::string tmpFilename;
	if(filename == NULL) tmpFilename = DEFAULT_TIMELINE_NAME;
	else tmpFilename = filename;

	TiXmlDocument doc(tmpFilename);
	TiXmlDeclaration declaration("1.0","gb2312","yes");
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");
	//����д����ص�����
	if(!XXml::addLeafNode(elmRoot,"fps",XString::toString(m_fps))) return false;
	if(!XXml::addLeafNode(elmRoot,"allTime",XString::toString(m_allTime))) return false;
	for(unsigned int i = 0;i < m_timeLines.size();++ i)
	{
		TiXmlElement elmNode("timeLine");
		if(!XXml::addLeafNode(elmNode,"name",m_timeLines[i]->m_name)) return false;
		for(unsigned int j = 0;j < m_timeLines[i]->m_keyFrames.size();++ j)
		{
			TiXmlElement elmNodeFrame("keyFrame");
			if(!XXml::addLeafNode(elmNodeFrame,"time",XString::toString(m_timeLines[i]->m_keyFrames[j].time))) return false;
			if(!XXml::addLeafNode(elmNodeFrame,"timeLen",XString::toString(m_timeLines[i]->m_keyFrames[j].timeLen))) return false;
			if(!XXml::addLeafNode(elmNodeFrame,"type",XString::toString(m_timeLines[i]->m_keyFrames[j].interpolationMode))) return false;
			if(m_timeLines[i]->m_keyFrames[j].pData != NULL)
			{
				if(!XXml::addLeafNode(elmNodeFrame,"haveData",true)) return false;
				if(!m_timeLines[i]->m_keyFrames[j].pData->writeToXml(&elmNodeFrame)) return false;
			}else
			{
				if(!XXml::addLeafNode(elmNodeFrame,"haveData",false)) return false;
			}
			//if(!XXml::addLeafNode(elmNodeFrame,"data",toString(m_timeLines[i]->m_keyFrames[j].pData))) return false;	//�������Դ���������Ҫ�޸�
			if(elmNode.InsertEndChild(elmNodeFrame) == NULL) return false; 
		}
		if(elmRoot.InsertEndChild(elmNode) == NULL) return false; 
	}
	if(doc.InsertEndChild(elmRoot) == NULL) return XFalse;
	doc.SaveFile();
	return XTrue;
}
XBool XTimeLines::readFromFile(const char * filename)
{
	std::string tmpFilename;
	if(filename == NULL) tmpFilename = DEFAULT_TIMELINE_NAME;
	else tmpFilename = filename;
	TiXmlDocument doc(tmpFilename);
	if(!doc.LoadFile()) return false;
	TiXmlNode *rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return false;
	//���濪ʼ��ȡ����
	int tmpI;
	bool tmpB;
	if(XXml::getXmlAsInt(rootNode,"fps",tmpI)) setFps(tmpI);
	if(XXml::getXmlAsInt(rootNode,"allTime",tmpI)) setAllTime(tmpI);
	TiXmlNode *keyNode = rootNode->FirstChildElement("timeLine");
	std::string timeLineName;
	TiXmlNode *keyFrameNode = NULL;
	XOneFrame tmpFrame;
	while(keyNode != NULL)
	{
		XXml::getXmlAsString(keyNode,"name",timeLineName);
		//�����ȡ�ؼ�֡
		keyFrameNode = keyNode->FirstChildElement("keyFrame");
		while(keyFrameNode != NULL)
		{
			XXml::getXmlAsInt(keyFrameNode,"time",tmpFrame.time);
			XXml::getXmlAsInt(keyFrameNode,"timeLen",tmpFrame.timeLen);
			if(XXml::getXmlAsInt(keyFrameNode,"type",tmpI)) tmpFrame.interpolationMode = (XMoveDataMode)tmpI;
			//if(XXml::getXmlAsInt(keyFrameNode,"data",tmpI)) tmpFrame.pData = (void *)tmpI;
			XXml::getXmlAsBool(keyFrameNode,"haveData",tmpB);
			if(tmpB)
			{
				tmpFrame.pData = createFrameDataBasic(keyFrameNode);
				if(tmpFrame.pData != NULL)
					tmpFrame.pData->readFromXml(keyFrameNode);
			}else
			{
				tmpFrame.pData = NULL;
			}
			addKeyFrame(timeLineName,tmpFrame.time,tmpFrame.timeLen,tmpFrame.pData,tmpFrame.interpolationMode);
			keyFrameNode = keyFrameNode->NextSiblingElement("keyFrame");
		}
		keyNode = keyNode->NextSiblingElement("timeLine");
	}

	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XTimeLine.inl"
#endif
}