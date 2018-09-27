#include "XStdHead.h"
#include "XImm.h"
#include <vector>
#include "XControl\XControlManager.h"
#if _WIN32_WINNT >= 0x0501
#include <msctf.h>
#endif
namespace XE{
namespace XImm
{
	bool m_isInputChange = false;
	HKL m_curHKL = NULL;
	std::string m_immDescription = "Ĭ�����뷨";
	std::string m_immFilename = "Ĭ���ļ�";
	bool m_isInComposition = false;	//�Ƿ������뷨�׶�
	std::string m_curCompositionStr = "";	//��ǰ���뷨���ַ���
	bool m_isCompositionStrChange = false;	//�Ƿ��������ַ��������˸ı�
	bool m_haveCandidateList = false;	//�Ƿ��к�ѡ�б�
	bool m_isCandidateListChange = false;	//��ѡ�б��Ƿ����ı仯
	std::vector<std::string> m_candidateListStr;	//��ѡ�б���ַ��� 
	int m_candidateListBuffSize = 0;
	char *m_pCandidateListBuff = NULL;			//���ڱ����ѡ�б����ݵ��ڴ�ռ䣨ע������ڴ�ռ�û�������ձ��ͷŵ���
	bool m_needShowImmUI = true;
	float m_curPosX = 0;
	float m_curPosY = 0;
	LRESULT inputMsgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam,bool &end)
	{
		switch(Msg)
		{
		case WM_IME_CHAR:
		case WM_CHAR:
			{//��ǰ�������������
				char mtcs[3] = "00";
				if(WM_IME_CHAR == Msg)
				{
					mtcs[0] = HIBYTE(LOWORD(wParam));
					mtcs[1] = LOBYTE(LOWORD(wParam));
				}else
				{
					mtcs[1] = HIBYTE(LOWORD(wParam));
					mtcs[0] = LOBYTE(LOWORD(wParam));
				}
				if(mtcs[0] == 0x00)
				{
				//	printf("%s\n",mtcs + 1);
					XCtrlManager.insertChar(mtcs + 1,1);
				}else 
				{
				//	printf("%s\n",mtcs);
					if(mtcs[0] < 128 && mtcs[1] == 0)
					{
						if(mtcs[0] >= ' ' && mtcs[0] <= '~') XCtrlManager.insertChar(mtcs,1);
					}else
					{
						XCtrlManager.insertChar(mtcs,2);
					}
				}
				//keysym.scancode = (unsigned char)HIWORD(lParam);
			}
			break;
		case WM_INPUTLANGCHANGE:
			{
				m_isInputChange = true;
				m_curHKL = (HKL)lParam;

				int size = ImmGetDescription(m_curHKL,NULL,0);
				if(size <= 0) m_immDescription = "Ĭ�����뷨";
				if(size >= 0 && size >= m_immDescription.size()) m_immDescription.resize(size + 1);
				if(ImmGetDescription(m_curHKL,&m_immDescription[0],size) <= 0)
					m_immDescription = "Ĭ�����뷨";
				else
					m_immDescription[size] = '\0';

				size = ImmGetIMEFileName(m_curHKL,NULL,0);
				if(size <= 0) m_immFilename = "Ĭ���ļ�";
				if(size >= 0 && size >= m_immFilename.size()) m_immFilename.resize(size + 1);
				if(ImmGetIMEFileName(m_curHKL,&m_immFilename[0],size) <= 0)
					m_immFilename = "Ĭ���ļ�";
				else 
					m_immFilename[size] = '\0';
				//��ȡ���뷨������
				//IGP_GETIMEVERSION:	00040000(262144)
				//IGP_PROPERTY:     	000D000A(851978)
				//	[Enum]IME Property
				//	00010000: IME_PROP_AT_CARET
				//	00040000: IME_PROP_CANDLIST_START_FROM_1
				//	00080000: IME_PROP_UNICODE
				//IGP_CONVERSION:   	00000009(9)
				//	[Enum]IME Conversion Mode
				//	00000001: IME_CMODE_NATIVE
				//	00000008: IME_CMODE_FULLSHAPE
				//IGP_SENTENCE:     	00000001(1)
				//	[Enum]IME Sentence Mode
				//	00000001: IME_SMODE_PLAURALCLAUSE
				//IGP_UI:           	00000002(2)
				//	[Enum]IME User Interface
				//	00000002: UI_CAP_ROT90
				//IGP_SETCOMPSTR:   	00000007(7)
				//	[Enum]IME set Composition String
				//	00000001: SCS_CAP_COMPSTR
				//	00000002: SCS_CAP_MAKEREAD
				//	00000004: SCS_CAP_SETRECONVERTSTRING
				//IGP_SELECT:       	00000000(0)
				//	[Enum]IME Selection inheritance capabilities
				//printf("0x%08x\n",ImmGetProperty((HKL)lParam,IGP_GETIMEVERSION));
			}
			break;
		case WM_IME_SETCONTEXT:	//��֪����δ���
			//printf("WM_IME_SETCONTEXT\n");
	//		if(myHIMC == NULL)
	//		{
	//			myHIMC = ImmCreateContext();
	//			ImmAssociateContext(hwnd,myHIMC);
	//		}
	//		lParam = 0;
	//		DefWindowProc(hwnd, Msg, wParam, lParam);
			break;
		case WM_IME_STARTCOMPOSITION:	//���뷨��ʼ����
			//printf("WM_IME_STARTCOMPOSITION\n");
			m_isInComposition = true;
			break;
		case WM_IME_COMPOSITION:
			//GCS_COMPATTR		//ÿ���ֵ�״̬���Ƿ�ת��
			//GCS_COMPCLAUSE	//���Ĳ���Ч��
			//GCS_COMPSTR		//������ַ���
			//GCS_CURSORPOS		//����λ�ã�
			//GCS_RESULTSTR		//���뷨�Ľ��
			if(lParam & GCS_COMPSTR)
			{
				HIMC hIMC = ImmGetContext(hwnd);
				if(hIMC)
				{
					int size = ImmGetCompositionString(hIMC,GCS_COMPSTR,NULL,0);
					if(size > 0)
					{
						m_isCompositionStrChange = true;
						if(size >= m_curCompositionStr.size()) m_curCompositionStr.resize(size + 1);
						ImmGetCompositionString(hIMC,GCS_COMPSTR,&m_curCompositionStr[0],size);
						m_curCompositionStr[size] = '\0';
					}
				}
				ImmReleaseContext( hwnd, hIMC ); 
			}	
			break;
		case WM_IME_ENDCOMPOSITION:	//���뷨��������
			//printf("���뷨��������\n");
			m_isInComposition = false;
			m_haveCandidateList = false;
			break;

		case WM_IME_NOTIFY:	//���뷨״̬�����ı�
			switch(wParam)
			{
			case IMN_SETOPENSTATUS:	//�ַ�ת�����л�ʱ�����
				//printf("IMN_SETOPENSTATUS\n");
				break;
			case IMN_OPENCANDIDATE:
				//printf("IMN_OPENCANDIDATE\n");
				break;
			case IMN_CHANGECANDIDATE:
				{//��ʾ��ʾ���еĺ�ѡ�ַ�������΢��ƴ�����뷨��Ч
					//printf("IMN_CHANGECANDIDATE\n");
					m_haveCandidateList = true;
					m_isCandidateListChange = true;

					HIMC hIMC = ImmGetContext(hwnd);
					if(hIMC)
					{
						int size = ImmGetCandidateList(hIMC,0,NULL,0);
						//bool state = ImmGetConversionStatus(hIMC,,);
						//DWORD sum;
						//DWORD ret = ImmGetCandidateListCount(hIMC,&sum);
						if(size > 0)
						{
							if(size > m_candidateListBuffSize)
							{
								XMem::XDELETE_ARRAY(m_pCandidateListBuff);
								m_pCandidateListBuff = XMem::createArrayMem<char>(size);
							}
							LPCANDIDATELIST pList = (LPCANDIDATELIST)m_pCandidateListBuff; 
							ImmGetCandidateList( hIMC,0,pList,size);
							m_candidateListStr.clear();
							//for(int i = 0;i < pList->dwCount;++ i)   
							for(int i = 0;(i < pList->dwCount - pList->dwSelection) && (i < pList->dwPageSize);++ i)   //�޸��˴���crash��bug
							{   
								//std::string tmp = (char*)pList + pList->dwOffset[pList->dwSelection + i];
								m_candidateListStr.push_back((char*)pList + pList->dwOffset[pList->dwSelection + i]);
							}  
						}
					}
					ImmReleaseContext(hwnd,hIMC);
				}
				break;
			case IMN_CLOSECANDIDATE:
				//printf("IMN_CLOSECANDIDATE\n");
				break;
			case IMN_PRIVATE:	//˽���¼�
				//printf("IMN_PRIVATE\n");
				break;
			}
			break;
		}
		return 0;
	}

	bool getIsInputChange()
	{
		bool ret = m_isInputChange;
		m_isInputChange = false;
		return ret;
	}
	const std::string& getImmDescription(){return m_immDescription;}
	const std::string& getImmFilename(){return m_immFilename;}
	void setCompositionPos(float x,float y)
	{
		m_curPosX = x;
		m_curPosY = y;
		HIMC hIMC = ImmGetContext(XEG.getHWND());
		if(!hIMC) return;
		//����������λ��
		COMPOSITIONFORM Composition;

		Composition.dwStyle = CFS_POINT;
		Composition.ptCurrentPos.x = m_curPosX;
		Composition.ptCurrentPos.y = m_curPosY;

		//Composition.dwStyle = CFS_RECT;
		//Composition.ptCurrentPos.x = m_curPosX;
		//Composition.ptCurrentPos.y = m_curPosY;
		//Composition.rcArea.left = m_curPosX;
		//Composition.rcArea.top = m_curPosY;
		//Composition.rcArea.right = m_curPosX + 300;
		//Composition.rcArea.bottom = m_curPosY + 100;

#ifdef _WIN64
		return;	//x64�������crash�����в���ȷ����Ҫ��һ����ȷ��
#endif
		ImmSetCompositionWindow(hIMC,&Composition);
		ImmReleaseContext(XEG.getHWND(),hIMC); 
	}
	bool getIsCompositionStrChange()
	{
		bool ret = m_isCompositionStrChange;
		m_isCompositionStrChange = false;
		return ret;
	}
	const std::string& getCompositionStr() {return m_curCompositionStr;}
	bool getCompositionState(){return m_isInComposition;}

	bool getIsCandidateListChange()		//��ѡ�б��Ƿ�������
	{
		bool ret = m_isCandidateListChange;
		m_isCandidateListChange = false;
		return ret;
	}
	bool getHaveCandidateList(){return m_haveCandidateList;}			//�Ƿ��к�ѡ�б�
	int getCandidateListCount(){return m_candidateListStr.size();};			//��ѡ�б������
	const std::string& getCandidateListStr(int index)	//��ȡ��ѡ�б������
	{
		if(index < 0 || index >= m_candidateListStr.size()) return XString::gNullStr;
		return m_candidateListStr[index];
	}
	//void showImmUI(bool hideOrShow)
	//{
	//	HIMC hIMC = ImmGetContext(XEG.getHWND());
	//	ImmSetOpenStatus(hIMC,hideOrShow);
	//	ImmReleaseContext(XEG.getHWND(),hIMC);
	//}
	//void showImmUI(bool hideOrShow)
	//{
	//	m_needShowImmUI = hideOrShow;
	//	HIMC hIMC = ImmGetContext(XEG.getHWND());
	//	if(!hIMC) return;
	//	//����������λ��
	//	COMPOSITIONFORM Composition;
	//	Composition.dwStyle = CFS_RECT;
	//	if(m_needShowImmUI)
	//	{
	//		Composition.rcArea.left = 0;
	//		Composition.rcArea.right = 0;
	//	}else
	//	{
	//		Composition.ptCurrentPos.x = m_curPosX;
	//		Composition.ptCurrentPos.y = m_curPosX;
	//	}
	//	ImmSetCompositionWindow(hIMC, &Composition);
	//	ImmReleaseContext(XEG.getHWND(),hIMC);
	//}
	bool getIsImm()
	{
		HKL tmp = GetKeyboardLayout(0);
		int size = ImmGetDescription(tmp,NULL,0);
		return size > 0;
	}
	std::string getTsfImeName()
	{
#if _WIN32_WINNT >= 0x0501
		HRESULT hr = S_OK;  
		hr = CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
		if(!SUCCEEDED(hr)) return XString::gNullStr;
		ITfInputProcessorProfiles *pProfiles;
		LANGID langid;  
		BSTR bstrImeName = NULL;  
		hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, NULL, CLSCTX_INPROC_SERVER, __uuidof(ITfInputProcessorProfiles), (LPVOID*)&pProfiles);
		if(!SUCCEEDED(hr)) return XString::gNullStr;
  
		hr = pProfiles->GetCurrentLanguage(&langid);  
		if(!SUCCEEDED(hr)) return XString::gNullStr;
  
		CLSID textSrvId, profileId;  
		hr = pProfiles->GetDefaultLanguageProfile(langid, GUID_TFCAT_TIP_KEYBOARD, &textSrvId, &profileId);  
		if(!SUCCEEDED(hr)) return XString::gNullStr;
  
		hr = pProfiles->GetActiveLanguageProfile(textSrvId, &langid, &profileId);  
		if(!SUCCEEDED(hr)) return XString::gNullStr;
  
		hr = pProfiles->GetLanguageProfileDescription(textSrvId, langid, profileId, &bstrImeName);  
		if(!SUCCEEDED(hr)) return XString::gNullStr;

		std::string ret = XString::Wchar2ANSI((wchar_t *)bstrImeName);

		SysFreeString(bstrImeName); 
		pProfiles->Release();
		CoUninitialize();
		return ret;
#else
		return XString::gNullStr;
#endif
	}
	std::string getImeName()
	{
		HKL tmp = GetKeyboardLayout(0);
		int size = ImmGetDescription(tmp,NULL,0);
		if(size <= 0)
		{
			return getTsfImeName();
		}else
		{
			std::string ImeName;
			ImeName.resize(size + 1);
			if(ImmGetDescription(tmp,&ImeName[0],size) <= 0)
				return getTsfImeName();
			else return ImeName;
		}
	}
	bool getIsEnglish()
	{
		if(getImeName() == "") return true;
		bool ret = true;
		HIMC hIMC = ImmGetContext(XEG.getHWND());
		if(hIMC)
		{
			DWORD dwConversion, dwSentence;
			ImmGetConversionStatus(hIMC,&dwConversion,&dwSentence);
			ret = !(dwConversion & IME_CMODE_NATIVE);
		}
		ImmReleaseContext(XEG.getHWND(),hIMC); 
		return ret;
	}
	bool getIsFullShape()
	{
		bool ret = true;
		HIMC hIMC = ImmGetContext(XEG.getHWND());
		if(hIMC)
		{
			DWORD dwConversion, dwSentence;
			ImmGetConversionStatus(hIMC,&dwConversion,&dwSentence);
			ret = dwConversion & IME_CMODE_FULLSHAPE;
		}
		ImmReleaseContext(XEG.getHWND(),hIMC); 
		return ret;
	}
	bool getIsSybol()
	{
		bool ret = true;
		HIMC hIMC = ImmGetContext(XEG.getHWND());
		if(hIMC)
		{
			DWORD dwConversion, dwSentence;
			ImmGetConversionStatus(hIMC,&dwConversion,&dwSentence);
			ret = dwConversion & IME_CMODE_SYMBOL;
		}
		ImmReleaseContext(XEG.getHWND(),hIMC); 
		return ret;
	}
}
}