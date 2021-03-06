// scanner.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;
int timers=0;
struct pingresult
{
	ULONG ip;
	PingReply reply;
};
pingresult pingt(in_addr ipa)
{
	int i = 0;
	CPing objPing;
	pingresult res;
	PingReply reply;
	memset(&res, 0, sizeof(pingresult));
	res.ip = ipa.S_un.S_addr;
	char* szDestIP = inet_ntoa(ipa);
	int t = 0,m=0;
	while (i < 4)
	{
		i++;
		
		
		if (objPing.Ping(szDestIP, &reply))
			m++;
		else
			t += reply.m_dwRoundTripTime;
		res.reply = reply;
	}
	if (m == 4)
		res.reply.m_dwRoundTripTime = -1;
	else
		res.reply.m_dwRoundTripTime = t / (4-m);
	
	return res;
}
void pinging(in_addr ipa)
{
	
	auto a = pingt(ipa);
	char* s = inet_ntoa(ipa);
	if (a.reply.m_dwRoundTripTime >= 5000)
		return ;
	if(time!=0)
		_sleep(timers);
	cout << s << " is available,ping is " << a.reply.m_dwRoundTripTime << endl;
	return ;
}
int main()
{
	in_addr startaddr, endaddr;
	char a[20], b[20];
	cout << "欢迎使用IP扫描工具\n"
		<< "作者：fx-moon\n"
		<< "输入起始与结束IP\n";
	cin >> a >> b;
	cout << "为减小网卡负担，请输入扫描间隔（推荐ms>15）\n";
	cin >> timers;
	cout << "请耐心等待！\n";
	startaddr.S_un.S_addr = ntohl(inet_addr(a));
	endaddr.S_un.S_addr = ntohl(inet_addr(b));
	threadpool executor{ 1024 };
	
	for (auto i = startaddr.S_un.S_addr; i <= endaddr.S_un.S_addr; i++)
	{
		for (int t = 0; t < 1024&& i <= endaddr.S_un.S_addr; t++,i++)
		{
			auto x = htonl(i);
			
			executor.commit(pinging, *((in_addr*)(&x)));
		}
	}


	system("pause");
    return 0;
}

