INLINE void XWindLine::release()	//�ͷ���Դ
{
	if(!m_isInited) return;
	XSmoothLine::release();
	m_pointsLife.clear();
}
INLINE void XWindLine::pushAPoint(const XVec2& p,float life)	//��������Ѿ�������ֵ�����˵�
{
	XSmoothLine::pushAPoint(p);
	m_pointsLife.push_back(life);
}
INLINE bool XWindLine::init(int insertSum,float insertLength,float life,float w,float a)
{
	if(!XSmoothLine::init(insertSum,insertLength)) return false;
	m_maxLife = life;
	m_wRate = w;
	m_aRate = a;
	return true;
}
INLINE void XWindLine::clear() 
{
	for(auto it = m_points.begin();it != m_points.end();++ it)
	{
		XMem::XDELETE(*it);
	}
	m_points.clear();
	m_pointsLife.clear();
}