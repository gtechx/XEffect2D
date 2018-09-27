#ifndef _JIA_XSIMPLETCPCLIENT_
#define _JIA_XSIMPLETCPCLIENT_
#include <string>
namespace XE{
class XSimpleTcpClient
{
private:
	bool m_isConnected;	//�Ƿ��Ѿ�����
	WSADATA m_wsaData;	
	SOCKET m_cmdsocket;		//���ӷ��������׽���
public:
	XSimpleTcpClient()
		:m_isConnected(false)
	{}
	~XSimpleTcpClient(){disConnect();}
	bool connectServer(const std::string& ip,int port);
	void disConnect();
	bool sendData(const void *data,int len);
	bool sendData(const std::string& str){return sendData(str.data(),str.size());}
	int recvData(void *data,int len);
	bool getIsConnected()const{return m_isConnected;}
};
}
#endif