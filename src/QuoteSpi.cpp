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
            (char*)"AG",
            (char*)"AL",
            (char*)"AU",
            (char*)"BU",
            (char*)"CU",
            (char*)"FU",
            (char*)"HC",
            (char*)"NI",
            (char*)"PB",
            (char*)"RB",
            (char*)"RU",
            (char*)"SN",
            (char*)"SP",
            (char*)"SS",
            (char*)"WR",
            (char*)"ZN",
    };
    for (int i = 0; i < sizeof(SHFEContract) / sizeof(SHFEContract[0]); i++)
    {
        m_SHFEList.push_back(SHFEContract[i]);
    }
    m_ExchangeMap.insert(pair<const char*, vector<char*> >("SHFE", m_SHFEList));

    char* m_CZCEContract[] = {
            (char*)"AP",     /// 苹果
            (char*)"CF",     /// 棉花
            (char*)"CJ",     /// 红枣
            (char*)"CY",     /// 棉纱
            (char*)"FG",     /// 玻璃
            (char*)"JR",     /// 粳稻
            (char*)"LR",     /// 晚籼稻
            (char*)"MA",     /// 甲醇
            (char*)"OI",     /// 菜油
            (char*)"PF",     /// 短纤
            (char*)"PK",     /// 花生
            (char*)"PM",     /// 普麦
            (char*)"RI",     /// 早籼稻
            (char*)"RM",     /// 菜粕
            (char*)"RS",     /// 菜籽
            (char*)"SA",     /// 纯碱
            (char*)"",     ///
            (char*)"",     ///
            (char*)"",     ///
            (char*)"",     ///
            (char*)"",     ///
            (char*)"",     ///
            (char*)"",     ///
            (char*)"",     ///
            (char*)"",     ///
            (char*)"",     ///
            (char*)"",     ///
            (char*)"",     ///
            (char*)"",     ///
            (char*)"",     ///
    };
    for (int i = 0; i < sizeof(m_CZCEContract) / sizeof(m_CZCEContract[0]); i++)
    {
        m_CZCEList.push_back(m_CZCEContract[i]);
    }
    m_ExchangeMap.insert(pair<const char*, vector<char*> >("CZCE", m_CZCEList));
/*
    char* m_DCEList[] = {
            (char*)"AG",
            (char*)"AL",
            (char*)"AU",
            (char*)"BU",
            (char*)"CU",
            (char*)"FU",
            (char*)"HC",
            (char*)"NI",
            (char*)"PB",
            (char*)"RB",
            (char*)"RU",
            (char*)"SN",
            (char*)"SP",
            (char*)"SS",
            (char*)"WR",
            (char*)"ZN",
    };
    for (int i = 0; i < sizeof(SHFEContract) / sizeof(SHFEContract[0]); i++)
    {
        m_SHFEList.push_back(SHFEContract[i]);
    }
    m_ExchangeMap.insert(pair<const char*, vector<char*> >("SHFE", m_SHFEList));

    char* m_CFFEXList[] = {
            (char*)"AG",
            (char*)"AL",
            (char*)"AU",
            (char*)"BU",
            (char*)"CU",
            (char*)"FU",
            (char*)"HC",
            (char*)"NI",
            (char*)"PB",
            (char*)"RB",
            (char*)"RU",
            (char*)"SN",
            (char*)"SP",
            (char*)"SS",
            (char*)"WR",
            (char*)"ZN",
    };
    for (int i = 0; i < sizeof(SHFEContract) / sizeof(SHFEContract[0]); i++)
    {
        m_SHFEList.push_back(SHFEContract[i]);
    }
    m_ExchangeMap.insert(pair<const char*, vector<char*> >("SHFE", m_SHFEList));*/
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


    //设定服务器IP、端口
    iErr = m_pAPI->SetHostAddress(quoteLoginMsg.IP, quoteLoginMsg.Port);
    if(TAPIERROR_SUCCEED != iErr) {
        cout << "SetHostAddress Error:" << iErr <<endl;
        return;
    }

    //登录服务器
    TapAPIQuoteLoginAuth stLoginAuth;
    memset(&stLoginAuth, 0, sizeof(stLoginAuth));
    stLoginAuth.ISModifyPassword = quoteLoginMsg.ISModifyPassword;
    stLoginAuth.ISDDA = quoteLoginMsg.ISDDA;
    iErr = m_pAPI->Login(&stLoginAuth);
    if(TAPIERROR_SUCCEED != iErr) {
        cout << "Login Error:" << iErr <<endl;
        return;
    }


    //等待APIReady
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
                    //订阅行情
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
    //取得API的版本信息
    cout << GetTapQuoteAPIVersion() << endl;

    //创建API实例
    TAPIINT32 iResult = TAPIERROR_SUCCEED;
    TapAPIApplicationInfo stAppInfo;
    strcpy(stAppInfo.AuthCode, quoteLoginMsg.AuthCode);
    strcpy(stAppInfo.KeyOperationLogPath, quoteLoginMsg.KeyOperationLogPath);

    m_pAPI = CreateTapQuoteAPI(&stAppInfo, iResult);
    if (NULL == m_pAPI){
        cout << "创建API实例失败，错误码：" << iResult <<endl;
        return -1;
    }

    m_pAPI->SetAPINotify(this); // 注册Spi实现的类
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
        cout << "登录成功，等待API初始化..." << endl;
        m_bIsAPIReady = true;

    } else {
        cout << "登录失败，错误码:" << errorCode << endl;
        m_Event.SignalEvent();
    }
}

void TAP_CDECL QuoteSpi::OnAPIReady()
{
    cout << "--->>> " << "QuoteSpi::OnAPIReady" << " <<<---" << endl;
    cout << "API初始化完成" << endl;
    m_Event.SignalEvent();
}

void TAP_CDECL QuoteSpi::OnDisconnect(TAPIINT32 reasonCode)
{
    cout << "--->>> " << "QuoteSpi::OnDisconnect" << " <<<---" << endl;
    cout << "API断开,断开原因:"<<reasonCode << endl;
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
        cout << "行情订阅成功 ";
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
        cout << "行情订阅失败，错误码：" << errorCode <<endl;
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
        cout << "行情更新:"
             << info->DateTimeStamp << " "
             << info->Contract.Commodity.CommodityNo << ""
             << info->Contract.ContractNo1 << " "
             << info->QLastPrice << " "
             << info->QLastQty
             << endl;
    }
}
