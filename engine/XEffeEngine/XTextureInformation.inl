INLINE  void XTextureData::updateTexture(const void * p)
{
	//if(p == NULL) return;	//�²��Ѿ��������ⲿ�ж�
	texture.updateTexture(p);
}
INLINE  void XTextureData::updateTextureR2B(const void * p)
{
	//if(p == NULL) return;	//�����Ѿ����ж�
	texture.updateTextureR2B(p);
}
INLINE  void XTextureData::updateTextureEx(const void * p)
{
	//if(p == NULL) return;	//�����Ѿ����ж�
	texture.updateTextureEx(p);
}
INLINE  void XTextureData::updateTextureR2BEx(const void * p)
{
	//if(p == NULL) return;	//�����Ѿ����ж�
	texture.updateTextureR2BEx(p);
}
INLINE  XBool XTextureData::loadEmpty()
{
	if(m_isInited) return XTrue;
	isEnableInsideClip = 0;
	m_isInited = XTrue;
	return XTrue;
}
INLINE  XBool XTextureData::createWithTexture(int w,int h,XColorMode colorMode,unsigned int tex)	//colorMode�Ĳ����ο�texture������ʱ��Ĳ���˵��
{
	if(m_isInited) return XTrue;
	isEnableInsideClip = 0;
	texture.createWithTexture(w,h,colorMode,tex);
	textureSize.set(w,h);
	m_isInited = XTrue;
	return XTrue;
}
INLINE  XBool XTextureData::createTexture(int w,int h,XColorMode colorMode)
{
	if(m_isInited) return XTrue;
	isEnableInsideClip = 0;
	texture.createTexture(w,h,colorMode);
	textureSize.set(w,h);
	m_isInited = XTrue;
	return XTrue;
}
INLINE  void XTextureData::release()
{
	if(!m_isInited) return;
	m_isInited = XFalse;
}
INLINE  void XTextureResourceManager::release()
{
	if(!m_isInited) return;
	m_texResInfos.clear();
	m_isInited = XFalse;
	m_isEnable = XFalse;
}