#if AUDIO_MATHOD == 0
#include "XSoundSDLMixer.h"
#endif
#if AUDIO_MATHOD == 1
#include "XSoundFmod.h"
#endif
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
//namespace XE{
//#define XCurSndCore XSoundSDLMixer::GetInstance()
////#define XCurSndCore XSoundFmod::GetInstance()
//}