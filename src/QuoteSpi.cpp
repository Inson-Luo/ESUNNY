#include "QuoteSpi.h"
#include "TapAPIError.h"
#include <iostream>
#include <cstring>
#include <string>
#include <regex>

using namespace std;

QuoteSpi::QuoteSpi(void):
        m_pAPI(NULL),
        m_bIsAPIReady(false),
        m_uiSessionID(0)

{
    char* SHFEContract[] = {
            (char*)"AG",     /// ����
            (char*)"AL",     /// ����
            (char*)"AU",     /// ����
            (char*)"BU",     /// ʯ������
            (char*)"CU",     /// ��ͭ
            (char*)"FU",     /// ȼ��
            (char*)"HC",     /// �������
            (char*)"NI",     /// ����
            (char*)"PB",     /// ��Ǧ
            (char*)"RB",     /// ���Ƹ�
            (char*)"RU",     /// ��
            (char*)"SN",     /// ����
            (char*)"SP",     /// ֽ��
            (char*)"SS",     /// �����
            (char*)"WR",     /// �߲�
            (char*)"ZN",     /// ��п
    };
    for (int i = 0; i < sizeof(SHFEContract) / sizeof(SHFEContract[0]); i++)
    {
        m_SHFEList.push_back(SHFEContract[i]);
    }
    m_ExchangeMap.insert(pair<const char*, vector<char*> >("SHFE", m_SHFEList));

    char* CZCEContract[] = {
            (char*)"AP",     /// ƻ��
            (char*)"CF",     /// �޻�
            (char*)"CJ",     /// ����
            (char*)"CY",     /// ��ɴ
            (char*)"FG",     /// ����
            (char*)"JR",     /// ����
            (char*)"LR",     /// ���̵�
            (char*)"MA",     /// �״�
            (char*)"OI",     /// ����
            (char*)"PF",     /// ����
            (char*)"PK",     /// ����
            (char*)"PM",     /// ����
            (char*)"RI",     /// ���̵�
            (char*)"RM",     /// ����
            (char*)"RS",     /// ����
            (char*)"SA",     /// ����
            (char*)"SF",     /// ����
            (char*)"SM",     /// �̹�
            (char*)"SR",     /// ����
            (char*)"TA",     /// PTA
            (char*)"UR",     /// ����
            (char*)"WH",     /// ǿ��
            (char*)"ZC",     /// ����ú
    };
    for (int i = 0; i < sizeof(CZCEContract) / sizeof(CZCEContract[0]); i++)
    {
        m_CZCEList.push_back(CZCEContract[i]);
    }
    m_ExchangeMap.insert(pair<const char*, vector<char*> >("CZCE", m_CZCEList));

    char* DCEContract[] = {
            (char*)"A",      /// ��һ
            (char*)"B",      /// ����
            (char*)"BB",     /// ���ϰ�
            (char*)"C",      /// ����
            (char*)"CS",     /// ���׵��
            (char*)"EB",     /// ����ϩ
            (char*)"EG",     /// �Ҷ���
            (char*)"FB",     /// ��ά��
            (char*)"I",      /// ����ʯ
            (char*)"J",      /// ��̿
            (char*)"JD",     /// ����
            (char*)"JM",     /// ��ú
            (char*)"L",      /// ����ϩ
            (char*)"LH",     /// ����
            (char*)"M",      /// ����
            (char*)"P",      /// �����
            (char*)"PG",     /// LPG(Һ��ʯ����)
            (char*)"PP",     /// �۱�ϩ
            (char*)"RR",     /// ����
            (char*)"V",      /// ������ϩ
            (char*)"Y",      /// ����
    };
    for (int i = 0; i < sizeof(DCEContract) / sizeof(DCEContract[0]); i++)
    {
        m_DCEList.push_back(DCEContract[i]);
    }
    m_ExchangeMap.insert(pair<const char*, vector<char*> >("DCE", m_DCEList));

    char* CFFEXContract[] = {
            (char*)"IC",     /// ��֤500
            (char*)"IF",     /// ����
            (char*)"IH",     /// ��֤
            (char*)"IM",     /// ��֤1000
            (char*)"TS",     /// ��ծ
            (char*)"TF",     /// ��ծ
            (char*)"T",      /// ʮծ
    };
    for (int i = 0; i < sizeof(CFFEXContract) / sizeof(CFFEXContract[0]); i++)
    {
        m_CFFEXList.push_back(CFFEXContract[i]);
    }
    m_ExchangeMap.insert(pair<const char*, vector<char*> >("CFFEX", m_CFFEXList));
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
}

int QuoteSpi::SubscribeQuote(string contract)
{
    TAPIINT32 iErr = TAPIERROR_SUCCEED;
    char contractNum[8];
    smatch result;
    regex pattern_num("(\\d+)");
    if (regex_search(contract, result, pattern_num))
    {
        string a = result[0];
        strcpy(contractNum, a.c_str());
    }
    regex pattern_English("([a-zA-Z]+)");

    if (regex_search(contract, result, pattern_English))
    {
        auto contractNo = result[0];
        for (map< const char*, vector<char*> >::iterator it = m_ExchangeMap.begin(); it != m_ExchangeMap.end(); it++)
        {
            for (vector<char*>::iterator iter = it->second.begin(); iter != it->second.end(); iter++)
            {
                if (contractNo == *iter)
                {
                    //��������
                    TapAPIContract stContract;
                    memset(&stContract, 0, sizeof(stContract));
                    strcpy(stContract.Commodity.ExchangeNo, it->first);
                    stContract.Commodity.CommodityType = TAPI_COMMODITY_TYPE_FUTURES;
                    strcpy(stContract.Commodity.CommodityNo, *iter);
                    strcpy(stContract.ContractNo1, contractNum);
                    stContract.CallOrPutFlag1 = TAPI_CALLPUT_FLAG_NONE;
                    stContract.CallOrPutFlag2 = TAPI_CALLPUT_FLAG_NONE;
                    iErr = m_pAPI->SubscribeQuote(&m_uiSessionID, &stContract);
                    m_uiSessionID += 1;
                    if(TAPIERROR_SUCCEED != iErr) {
                        cout << "SubscribeQuote Error:" << iErr <<endl;
                        return -1;
                    }

                }
            }
        }
    }
    else
    {
        return -1;
    }

    return 0;
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
    SubscribeQuote("RB2301");
    while(true) {
        m_Event.WaitEvent();
    }
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
