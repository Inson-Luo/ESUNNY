#include "Esunny.h"
#include <iostream>
#include <cstring>
using namespace std;

Esunny::Esunny()
{

}

Esunny::~Esunny()
{

}

void Esunny::EsunnyStart()
{
    /// ***************************�����¼*************************** ///
    quote = new QuoteSpi();
    strcpy(quote->quoteLoginMsg.IP, "61.163.243.173");
    quote->quoteLoginMsg.Port = 6161;
    quote->quoteLoginMsg.ISModifyPassword = APIYNFLAG_NO;
    quote->quoteLoginMsg.ISDDA = APIYNFLAG_NO;
    strcpy(quote->quoteLoginMsg.AuthCode,"B112F916FE7D27BCE7B97EB620206457946CED32E26C1EAC946CED32E26C1EAC946CED32E26C1EAC946CED32E26C1EAC5211AF9FEE541DDE9D6F622F72E25D5DEF7F47AA93A738EF5A51B81D8526AB6A9D19E65B41F59D6A946CED32E26C1EACCAF8D4C61E28E2B1ABD9B8F170E14F8847D3EA0BF4E191F5DCB1B791E63DC196D1576DEAF5EC563CA3E560313C0C3411B45076795F550EB050A62C4F74D5892D2D14892E812723FAC858DEBD8D4AF9410729FB849D5D8D6EA48A1B8DC67E037381A279CE9426070929D5DA085659772E24A6F5EA52CF92A4D403F9E46083F27B19A88AD99812DADA44100324759F9FD1964EBD4F2F0FB50B51CD31C0B02BB437");
    strcpy(quote->quoteLoginMsg.KeyOperationLogPath, "\\log");
    int result = quote->Init();
    quote->SubscribeQuote("RB2301");
    /// ************************************************************ ///

    /// ***************************���׵�¼************************** ///
    trade = new TradeSpi();
    strcpy(trade->tradeLoginMsg.m_FrontIp, "61.163.243.173");
    trade->tradeLoginMsg.m_FrontPort = 6161;
    strcpy(trade->tradeLoginMsg.m_LoginReq.AccountNo, "6013");
    strcpy(trade->tradeLoginMsg.m_LoginReq.Password, "60136013");
    strcpy(trade->tradeLoginMsg.m_LoginReq.AppId, "esunny_epolestar_9.0");
    strcpy(trade->tradeLoginMsg.m_LoginReq.LicenseNo, "esunny_epolestar");
#if defined WIN32 || defined _WIN32
    char logPath[] = "C:/apidemo/";
#else
    char logPath[] = "/home/ESUNNY/";
#endif
    strcpy(trade->tradeLoginMsg.m_LogPath, logPath);
    /*
    int resu = trade->Init();
    while (!trade->IsReady())
    {

    }*/
    while(true) {
        m_Event.WaitEvent();
    }
    /// ************************************************************ ///
}
