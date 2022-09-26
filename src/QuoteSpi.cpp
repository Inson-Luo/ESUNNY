#include "QuoteSpi.h"
#include "TapAPIError.h"
#include <iostream>
#include <cstring>

using namespace std;

QuoteSpi::QuoteSpi(void):
        m_pAPI(NULL),
        m_bIsAPIReady(false)
{
}

QuoteSpi::~QuoteSpi(void)
{
}

void QuoteSpi::Run()
{
    cout << "--->>> " << "QuoteSpi::Run" << " <<<---" << endl;
    if(NULL == m_pAPI) {
        cout << "Error: m_pAPI is NULL." << endl;
        return;
    }

    TAPIINT32 iErr = TAPIERROR_SUCCEED;


    //�趨������IP���˿�
    iErr = m_pAPI->SetHostAddress(quoteLoginMsg.IP, quoteLoginMsg.Port);
    if(TAPIERROR_SUCCEED != iErr) {
        cout << "SetHostAddress Error:" << iErr <<endl;
        return;
    }

    //��¼������
    TapAPIQuoteLoginAuth stLoginAuth;
    memset(&stLoginAuth, 0, sizeof(stLoginAuth));
    stLoginAuth.ISModifyPassword = quoteLoginMsg.ISModifyPassword;
    stLoginAuth.ISDDA = quoteLoginMsg.ISDDA;
    iErr = m_pAPI->Login(&stLoginAuth);
    if(TAPIERROR_SUCCEED != iErr) {
        cout << "Login Error:" << iErr <<endl;
        return;
    }


    //�ȴ�APIReady
    m_Event.WaitEvent();
    if (!m_bIsAPIReady){
        return;
    }

    //��������
    TapAPIContract stContract;
    memset(&stContract, 0, sizeof(stContract));
    strcpy(stContract.Commodity.ExchangeNo, "SHFE");
    stContract.Commodity.CommodityType = TAPI_COMMODITY_TYPE_FUTURES;
    strcpy(stContract.Commodity.CommodityNo, "RB");
    strcpy(stContract.ContractNo1, "2301");
    stContract.CallOrPutFlag1 = TAPI_CALLPUT_FLAG_NONE;
    stContract.CallOrPutFlag2 = TAPI_CALLPUT_FLAG_NONE;
    m_uiSessionID = 0;
    iErr = m_pAPI->SubscribeQuote(&m_uiSessionID, &stContract);
    if(TAPIERROR_SUCCEED != iErr) {
        cout << "SubscribeQuote Error:" << iErr <<endl;
        return;
    }

    while(true) {
        m_Event.WaitEvent();
    }
}

int QuoteSpi::Init()
{
    cout << "--->>> " << "QuoteSpi::Init" << " <<<---" << endl;
    //ȡ��API�İ汾��Ϣ
    cout << GetTapQuoteAPIVersion() << endl;

    //����APIʵ��
    TAPIINT32 iResult = TAPIERROR_SUCCEED;
    TapAPIApplicationInfo stAppInfo;
    strcpy(stAppInfo.AuthCode, quoteLoginMsg.AuthCode);
    strcpy(stAppInfo.KeyOperationLogPath, quoteLoginMsg.KeyOperationLogPath);

    m_pAPI = CreateTapQuoteAPI(&stAppInfo, iResult);
    if (NULL == m_pAPI){
        cout << "����APIʵ��ʧ�ܣ������룺" << iResult <<endl;
        return -1;
    }

    m_pAPI->SetAPINotify(this); // ע��Spiʵ�ֵ���
    Run();

    return 0;
}

void TAP_CDECL QuoteSpi::OnRspLogin(TAPIINT32 errorCode, const TapAPIQuotLoginRspInfo *info)
{
    cout << "--->>> " << "QuoteSpi::OnRspLogin" << " <<<---" << endl;
    if(TAPIERROR_SUCCEED == errorCode) {
        cout << "��¼�ɹ����ȴ�API��ʼ��..." << endl;
        m_bIsAPIReady = true;

    } else {
        cout << "��¼ʧ�ܣ�������:" << errorCode << endl;
        m_Event.SignalEvent();
    }
}

void TAP_CDECL QuoteSpi::OnAPIReady()
{
    cout << "--->>> " << "QuoteSpi::OnAPIReady" << " <<<---" << endl;
    cout << "API��ʼ�����" << endl;
    m_Event.SignalEvent();
}

void TAP_CDECL QuoteSpi::OnDisconnect(TAPIINT32 reasonCode)
{
    cout << "--->>> " << "QuoteSpi::OnDisconnect" << " <<<---" << endl;
    cout << "API�Ͽ�,�Ͽ�ԭ��:"<<reasonCode << endl;
}

void TAP_CDECL QuoteSpi::OnRspQryCommodity(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIQuoteCommodityInfo *info)
{
    cout << "--->>> " << "QuoteSpi::OnRspQryCommodity" << " <<<---" << endl;
}

void TAP_CDECL QuoteSpi::OnRspQryContract(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIQuoteContractInfo *info)
{
    cout << "--->>> " << "QuoteSpi::OnRspQryContract" << " <<<---" << endl;
}

void TAP_CDECL QuoteSpi::OnRspSubscribeQuote(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIQuoteWhole *info)
{
    cout << "--->>> " << "QuoteSpi::OnRspSubscribeQuote" << " <<<---" << endl;
    if (TAPIERROR_SUCCEED == errorCode)
    {
        cout << "���鶩�ĳɹ� ";
        if (NULL != info)
        {
            cout << info->DateTimeStamp << " "
                 << info->Contract.Commodity.CommodityNo << ""
                 << info->Contract.ContractNo1 << " "
                 << info->QLastPrice << " "
                 << info->QLastQty
                 <<endl;
        }

    } else{
        cout << "���鶩��ʧ�ܣ������룺" << errorCode <<endl;
    }
}

void TAP_CDECL QuoteSpi::OnRspUnSubscribeQuote(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIContract *info)
{
    cout << "--->>> " << "QuoteSpi::OnRspUnSubscribeQuote" << " <<<---" << endl;
}

void TAP_CDECL QuoteSpi::OnRtnQuote(const TapAPIQuoteWhole *info)
{
    if (NULL != info)
    {
        cout << "�������:"
             << info->DateTimeStamp << " "
             << info->Contract.Commodity.CommodityNo << ""
             << info->Contract.ContractNo1 << " "
             << info->QLastPrice << " "
             << info->QLastQty
             << endl;
    }
}
