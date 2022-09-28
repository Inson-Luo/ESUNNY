

///@system  Dstar V10 api demo
///@file    ApiClient.cpp
///@author  Hao Lin 2021-01-20


#include "TradeSpi.h"
#include <iostream>
#include <cstring>


TradeSpi::TradeSpi()
        : m_pApi(NULL)
        , m_bReady(false)
        , m_bUdpAuth(false)
{

}

TradeSpi::~TradeSpi()
{
    FreeDstarTradeApi(m_pApi);
}

int TradeSpi::Init()
{
    m_pApi = CreateDstarTradeApi();
    if (!m_pApi)
    {
        printf("Create Api Failed\n");
        return 1;
    }
    else
    {
        printf("Create Api Successfully, version:%s\n", m_pApi->GetApiVersion());
    }

    if (!m_pApi)
    {
        return -1;
    }
    m_pApi->RegisterSpi(this);
    m_pApi->RegisterFrontAddress(tradeLoginMsg.m_FrontIp, tradeLoginMsg.m_FrontPort);
    m_pApi->SetApiLogPath(tradeLoginMsg.m_LogPath);
    m_pApi->SetLoginInfo(&tradeLoginMsg.m_LoginReq);
    m_pApi->SetCpuId(-1, -1);
    m_pApi->SetSubscribeStartId(-1);
    int ret = SubmitSystemInfo();
    if (ret != 0)
    {
        return ret;
    }

    return m_pApi->Init(DSTAR_API_INIT_NOQUERY);
}

bool TradeSpi::IsReady()
{
    return m_bReady;
}

bool TradeSpi::IsUdpAuth()
{
    return m_bUdpAuth;
}

DstarApiAccountIndexType TradeSpi::GetAccountIndex()
{
    return m_LoginInfo.AccountIndex;
}

DstarApiAuthCodeType TradeSpi::GetUdpAuthCode()
{
    return m_LoginInfo.UdpAuthCode;
}

void TradeSpi::OnFrontDisconnected()
{
    printf("OnFrontDisconnected\n");
}

void TradeSpi::OnRspError(DstarApiErrorCodeType nErrorCode)
{
    printf("OnRspError:%u\n", nErrorCode);
}

void TradeSpi::OnApiReady(const DstarApiSerialIdType nSerialId)
{
    printf("OnApiReady, serial:%llu\n", nSerialId);

    m_bReady = true;
}

void TradeSpi::OnRspUserLogin(const DstarApiRspLoginField *pLoginRsp)
{
    m_LoginInfo = *pLoginRsp;
    printf("OnRspUserLogin user:%s index:%u error:%u authcode:%u\n",
            m_LoginInfo.AccountNo, m_LoginInfo.AccountIndex, m_LoginInfo.ErrorCode, m_LoginInfo.UdpAuthCode);
}

void TradeSpi::OnRspSubmitInfo(const DstarApiRspSubmitInfoField *pRspSubmitInfo)
{
    printf("OnRspSubmitInfo user:%s error:%u \n",
            pRspSubmitInfo->AccountNo, pRspSubmitInfo->ErrorCode);
}

void TradeSpi::OnRspTrdExchangeState(const DstarApiTrdExchangeStateField *pTrdExchangeState)
{
    printf("OnRspTrdExchangeState ExchangeId: %c, CommodityType: %c, CommodityNo: %s, TradingState: %c ExchangeTime: %s\n",
            pTrdExchangeState->ExchangeId,
            pTrdExchangeState->CommodityType,
            pTrdExchangeState->CommodityNo,
            pTrdExchangeState->TradingState,
            pTrdExchangeState->ExchangeTime);
}

void TradeSpi::OnRtnOrder(const DstarApiOrderField *pOrder)
{
    printf("OnRtnOrder AccountNo:%s ContractNo1:%s ContractNo2:%s Direct:%c "
           "ExchInsertTime:%s Fee:%f FrozenMargin:%f Hedge:%c Margin:%f MatchQty:%d "
           "MinQty:%d Offset:%c OrderId:%llu OrderLocalNo:%s OrderPrice:%f OrderQty:%d "
           "OrderState:%c OrderType:%c UpSeatNo:%s SystemNo:%s UpdateTime:%s SeatIndex:%d "
           "CmbId:%llu ErrCode:%u\n",
            pOrder->AccountNo,
            pOrder->ContractNo1,
            pOrder->ContractNo2,
            pOrder->Direct,
            pOrder->ExchInsertTime,
            pOrder->Fee,
            pOrder->FrozenMargin,
            pOrder->Hedge,
            pOrder->Margin,
            pOrder->MatchQty,
            pOrder->MinQty,
            pOrder->Offset,
            pOrder->OrderId,
            pOrder->OrderLocalNo,
            pOrder->OrderPrice,
            pOrder->OrderQty,
            pOrder->OrderState,
            pOrder->OrderType,
            pOrder->UpSeatNo,
            pOrder->SystemNo,
            pOrder->UpdateTime,
            pOrder->SeatIndex,
            pOrder->CmbId,
            pOrder->ErrCode);
}

void TradeSpi::OnRtnOffer(const DstarApiOfferField *pOffer)
{
    printf("OnRtnOffer AccountNo:%s BuyMatchQty:%d BuyOffset:%c BuyPrice:%f ContractNo:%s EnquiryNo:%s "
           "ExchInsertTime:%s FrozenMargin:%f Margin:%f OrderId:%llu OrderLocalNo:%s OrderQty:%d "
           "OrderState:%c Reference:%llu SellMatchQty:%d SellOffset:%c SellPrice:%f SerialId:%llu "
           "SystemNo:%s UpSeatNo:%s UpdateTime:%s ErrCode:%u\n",
            pOffer->AccountNo,
            pOffer->BuyMatchQty,
            pOffer->BuyOffset,
            pOffer->BuyPrice,
            pOffer->ContractNo,
            pOffer->EnquiryNo,
            pOffer->ExchInsertTime,
            pOffer->FrozenMargin,
            pOffer->Margin,
            pOffer->OrderId,
            pOffer->OrderLocalNo,
            pOffer->OrderQty,
            pOffer->OrderState,
            pOffer->Reference,
            pOffer->SellMatchQty,
            pOffer->SellOffset,
            pOffer->SellPrice,
            pOffer->SerialId,
            pOffer->SystemNo,
            pOffer->UpSeatNo,
            pOffer->UpdateTime,
            pOffer->ErrCode);
}

void TradeSpi::OnRtnMatch(const DstarApiMatchField *pMatch)
{
    printf("OnRtnMatch MatchId: %llu AccountNo:%s CloseProfit:%f ContractNo:%s Direct:%c ExchMatchNo:%s "
           "Fee:%f FrozenMargin:%f Hedge:%c Margin:%f MatchId:%llu MatchPrice:%f MatchQty:%d "
           "MatchTime:%s Offset:%c OrderId:%llu OrderType:%c Premium:%f Reference:%llu SerialId:%llu "
           "SystemNo:%s UpdateTime:%s\n",
            pMatch->MatchId,
            pMatch->AccountNo,
            pMatch->CloseProfit,
            pMatch->ContractNo,
            pMatch->Direct,
            pMatch->ExchMatchNo,
            pMatch->Fee,
            pMatch->FrozenMargin,
            pMatch->Hedge,
            pMatch->Margin,
            pMatch->MatchId,
            pMatch->MatchPrice,
            pMatch->MatchQty,
            pMatch->MatchTime,
            pMatch->Offset,
            pMatch->OrderId,
            pMatch->OrderType,
            pMatch->Premium,
            pMatch->Reference,
            pMatch->SerialId,
            pMatch->SystemNo,
            pMatch->UpdateTime);
}

void TradeSpi::OnRtnEnquiry(const DstarApiEnquiryField *pEnquiry)
{
    printf("OnRtnEnquiry ContractNo:%s Direct:%c EnquiryNo:%s\n",
            pEnquiry->ContractNo,
            pEnquiry->Direct,
            pEnquiry->EnquiryNo);
}

void TradeSpi::OnRtnTrdExchangeState(const DstarApiTrdExchangeStateField *pTrdExchangeState)
{
    printf("OnRspTrdExchangeState ExchangeId: %c, CommodityType: %c, CommodityNo: %s, TradingState: %c ExchangeTime: %s\n",
            pTrdExchangeState->ExchangeId,
            pTrdExchangeState->CommodityType,
            pTrdExchangeState->CommodityNo,
            pTrdExchangeState->TradingState,
            pTrdExchangeState->ExchangeTime);
}

void TradeSpi::OnRtnPosiProfit(const DstarApiPosiProfitField *pPosiProfit)
{
    printf("OnRtnPosiProfit AccountNo: %s PosiProfit: %f SerialId: %llu\n",
            pPosiProfit->AccountNo,
            pPosiProfit->PosiProfit,
            pPosiProfit->SerialId);
}

void TradeSpi::OnRspSeat(const DstarApiSeatField *pSeatInfo)
{
    printf("OnRspSeat seatindex:%d exchange:%c seatno:%s\n", pSeatInfo->SeatIndex, pSeatInfo->Exchange, pSeatInfo->SeatNo);
}

void TradeSpi::OnRspContract(const DstarApiContractField *pContract)
{
    printf("OnRspContract ContractIndex:%u ContractNo:%s\n", pContract->ContractIndex, pContract->ContractNo);
}

void TradeSpi::OnRspTrdFeeParam(const DstarApiTrdFeeParamField* pFeeParam)
{
    printf("OnRspTrdFeeParam AccountNo:%s ContractNo:%s OpenRatio:%f OpenVolume:%f CloseRatio:%f CloseVolume:%f CloseTRatio:%f CloseTVolume:%f\n",
            pFeeParam->AccountNo,
            pFeeParam->ContractNo,
            pFeeParam->OpenRatio,
            pFeeParam->OpenVolume,
            pFeeParam->CloseRatio,
            pFeeParam->CloseVolume,
            pFeeParam->CloseTRatio,
            pFeeParam->CloseTVolume);
}

void TradeSpi::OnRspTrdMarParam(const DstarApiTrdMarParamField* pMarParam)
{
    printf("OnRspTrdMarParam AccountNo:%s ContractNo:%s BuySpeculateParam:%f BuyHedgeParam:%f SellSpeculateParam:%f SellHedgeParam:%f\n",
            pMarParam->AccountNo,
            pMarParam->ContractNo,
            pMarParam->BuySpeculateParam,
            pMarParam->BuyHedgeParam,
            pMarParam->SellSpeculateParam,
            pMarParam->SellHedgeParam);
}

void TradeSpi::OnRspPrePosition(const DstarApiPrePositionField *pPrePosition)
{
    printf("OnRspPrePosition,AccountNo:%s, ContractNo:%s, %d, %f, %d, %f\n",
            pPrePosition->AccountNo,
            pPrePosition->ContractNo,
            pPrePosition->PreBuyQty,
            pPrePosition->PreBuyAvgPrice,
            pPrePosition->PreSellQty,
            pPrePosition->PreSellAvgPrice);
}

void TradeSpi::OnRspPosition(const DstarApiPositionField *pPosition)
{
    printf("OnRspPosition,AccountNo:%s, ContractNo:%s, SerialId:%llu, %d, %d, %f, %d, %d, %f\n",
            pPosition->AccountNo,
            pPosition->ContractNo,
            pPosition->SerialId,
            pPosition->PreBuyQty,
            pPosition->TodayBuyQty,
            pPosition->BuyAvgPrice,
            pPosition->PreSellQty,
            pPosition->TodaySellQty,
            pPosition->SellAvgPrice);
}

void TradeSpi::OnRspFund(const DstarApiFundField *pFund)
{
    printf("OnRspFund AccountNo:%s, PreEquity:%f, Equity:%f, Avail:%f, Fee:%f, Margin:%f, "
           "FrozenMargin:%f, Premium:%f, CloseProfit:%f, PositionProfit:%f, CashIn:%f, CashOut:%f\n",
            pFund->AccountNo, pFund->PreEquity, pFund->Equity, pFund->Avail,
            pFund->Fee, pFund->Margin, pFund->FrozenMargin, pFund->Premium,
            pFund->CloseProfit, pFund->PositionProfit, pFund->CashIn, pFund->CashOut);
}

void TradeSpi::OnRspOrder(const DstarApiOrderField *pOrder)
{
    printf("OnRspOrder ContractNo:%s, OrderPrice:%f, OrderQty:%d, MinQty:%d, MatchQty:%d, Direct:%c, Offset:%c, Hedge:%c, OrderType:%c, "
           "ValidType:%c, OrderState:%c, ErrCode:%d, OrderId:%llu, OrderLocalNo:%s, SystemNo:%s, ExchInsertTime:%s, "
           "SerialId:%llu, FrozenMargin:%f, AccountNo:%s, Ref:%d, SeatIndex:%d\n",
            pOrder->ContractNo1, pOrder->OrderPrice, pOrder->OrderQty, pOrder->MinQty,
            pOrder->MatchQty, pOrder->Direct, pOrder->Offset, pOrder->Hedge,
            pOrder->OrderType, pOrder->ValidType, pOrder->OrderState, pOrder->ErrCode,
            pOrder->OrderId, pOrder->OrderLocalNo, pOrder->SystemNo, pOrder->ExchInsertTime,
            pOrder->SerialId, pOrder->FrozenMargin, pOrder->AccountNo, pOrder->Reference, pOrder->SeatIndex);
}

void TradeSpi::OnRspOffer(const DstarApiOfferField *pOffer)
{
    printf("OnRspOffer BuyOffset:%c, SellOffset:%c, ContractNo:%s, OrderQty:%d, ErrCode:%d, "
           "BuyPrice:%f, SellPrice:%f, OrderId:%llu, "
           "OrderLocalNo:%s, SystemNo:%s, QueryNo:%s, OrderState:%c, SerialId:%lld, FrozenMargin:%f, "
           "AccountNo:%s, SeatIndex:%d Reference:%llu\n",
            pOffer->BuyOffset, pOffer->SellOffset, pOffer->ContractNo,
            pOffer->OrderQty, pOffer->ErrCode, pOffer->BuyPrice, pOffer->SellPrice,
            pOffer->OrderId, pOffer->OrderLocalNo, pOffer->SystemNo,
            pOffer->EnquiryNo, pOffer->OrderState, pOffer->SerialId, pOffer->FrozenMargin,
            pOffer->AccountNo, pOffer->SeatIndex, pOffer->Reference);
}

void TradeSpi::OnRspMatch(const DstarApiMatchField *pMatch)
{
    printf("OnRspMatch contract:%s qty:%d price:%f offset:%c direct:%c hedge:%c time:%s orderid:%llu "
           "matchid:%llu systemno:%s serial:%llu fee:%f margin:%f frozen:%f premium:%f closeprofit:%f "
           "acccount:%s reference:%d ordertype:%c\n",
            pMatch->ContractNo, pMatch->MatchQty, pMatch->MatchPrice, pMatch->Offset,
            pMatch->Direct, pMatch->Hedge, pMatch->MatchTime,
            pMatch->OrderId, pMatch->MatchId, pMatch->SystemNo, pMatch->SerialId,
            pMatch->Fee, pMatch->Margin, pMatch->FrozenMargin, pMatch->Premium,
            pMatch->CloseProfit, pMatch->AccountNo, pMatch->Reference, pMatch->OrderType);
}

void TradeSpi::OnRspCashInOut(const DstarApiCashInOutField *pCashInOut)
{
    printf("OnRspCashInOut SerialId:%llu CashInOutType:%c CashInOutValue:%f AccountNo:%s OperateTime:%s\n",
            pCashInOut->SerialId, pCashInOut->CashInOutType, pCashInOut->CashInOutValue, pCashInOut->AccountNo,
            pCashInOut->OperateTime);
}

void TradeSpi::OnRspUdpAuth(const DstarApiRspUdpAuthField *pRspUdpAuth)
{
    if (pRspUdpAuth->ErrorCode == 0)
    {
        m_bUdpAuth = true;
    }
    printf("OnRspUdpAuth AccountIndex:%d UdpAuthCode:%u ErrorCode:%u\n",
            pRspUdpAuth->AccountIndex, pRspUdpAuth->UdpAuthCode, pRspUdpAuth->ErrorCode);
}

void TradeSpi::OnRspOrderInsert(const DstarApiRspOrderInsertField *pOrderInsert)
{
    printf("OnRspOrderInsert AccountNo:%s ClientReqId:%d SeatIndex:%d OrderId:%llu ErrCode:%u MaxClientReqId:%u InsertTime:%s Reference:%llu\n",
            pOrderInsert->AccountNo,
            pOrderInsert->ClientReqId,
            pOrderInsert->SeatIndex,
            pOrderInsert->OrderId,
            pOrderInsert->ErrCode,
            pOrderInsert->MaxClientReqId,
            pOrderInsert->InsertTime,
            pOrderInsert->Reference);
}

void TradeSpi::OnRspOfferInsert(const DstarApiRspOfferInsertField *pOfferInsert)
{
    printf("OnRspOfferInsert AccountNo:%s ClientReqId:%d SeatIndex:%d OrderId:%llu ErrCode:%u MaxClientReqId:%u InsertTime:%s Reference:%llu\n",
            pOfferInsert->AccountNo,
            pOfferInsert->ClientReqId,
            pOfferInsert->SeatIndex,
            pOfferInsert->OrderId,
            pOfferInsert->ErrCode,
            pOfferInsert->MaxClientReqId,
            pOfferInsert->InsertTime,
            pOfferInsert->Reference);
}

void TradeSpi::OnRspLastReqId(const DstaApiRspLastReqIdField *pLastReqId)
{
    printf("OnRspLastReqId ClientReqId:%d \n", pLastReqId->LastClientReqId);
}

void TradeSpi::OnRspOrderDelete(const DstarApiRspOrderDeleteField *pOrderDelete)
{
    printf("OnRspOrderDelete AccountNo:%s ClientReqId:%d SeatIndex:%d OrderId:%llu ErrCode:%u MaxClientReqId:%u InsertTime:%s Reference:%llu\n",
            pOrderDelete->AccountNo,
            pOrderDelete->ClientReqId,
            pOrderDelete->SeatIndex,
            pOrderDelete->OrderId,
            pOrderDelete->ErrCode,
            pOrderDelete->MaxClientReqId,
            pOrderDelete->InsertTime,
            pOrderDelete->Reference);
}

void TradeSpi::OnRtnCashInOut(const DstarApiCashInOutField *pCashInOut)
{
    printf("OnRtnCashInOut\n");
}

void TradeSpi::OnRtnSeat(const DstarApiSeatField* pSeat)
{
    printf("OnRspSeat seatindex:%d exchange:%c seatno:%s\n", pSeat->SeatIndex, pSeat->Exchange, pSeat->SeatNo);
}

void TradeSpi::OnRspPwdMod(const DstarApiRspPwdModField* pRspPwdModField)
{
    printf("OnRspPwdMod AccountNo:%s ErrorCode:%d\n", pRspPwdModField->AccountNo, pRspPwdModField->ErrorCode);
}

void TradeSpi::OnRtnPwdMod(const DstarApiPwdModField* pPwdModField)
{
    printf("OnRtnPwdMod AccountNo:%s\n", pPwdModField->AccountNo);
}

int TradeSpi::SubmitSystemInfo()
{
    char systeminfo[1024] = {0};
    int nLen = 1024;
    unsigned int nVersion = 0;
    int ret = m_pApi->GetSystemInfo(systeminfo, &nLen, &nVersion);
    printf("GetSystemInfo, ret:%d, len:%d, version:%d\n", ret, nLen, nVersion);
    if (ret != 0)
    {
        return ret;
    }

    DstarApiSubmitInfoField pSubmitInfo = {0};
    strncpy(pSubmitInfo.AccountNo, tradeLoginMsg.m_LoginReq.AccountNo, sizeof(DstarApiAccountNoType) - 1);
    pSubmitInfo.AuthType = DSTAR_API_AUTHTYPE_DIRECT;
    pSubmitInfo.AuthKeyVersion = nVersion;
    memcpy(pSubmitInfo.SystemInfo, systeminfo, nLen);
    strncpy(pSubmitInfo.LicenseNo, tradeLoginMsg.m_LoginReq.LicenseNo, sizeof(DstarApiLicenseNoType) - 1);
    strncpy(pSubmitInfo.ClientAppId, tradeLoginMsg.m_LoginReq.AppId, sizeof(DstarApiAppIdType) - 1);

    m_pApi->SetSubmitInfo(&pSubmitInfo);

    return ret;
}

void TradeSpi::ReqOrderInsert(const DstarApiReqOrderInsertField* pOrder)
{
    m_pApi->ReqOrderInsert(pOrder);
}

void TradeSpi::ReqOfferInsert(const DstarApiReqOfferInsertField* pOffer)
{
    m_pApi->ReqOfferInsert(pOffer);
}

void TradeSpi::ReqOrderDelete(const DstarApiReqOrderDeleteField* pOrder)
{
    m_pApi->ReqOrderDelete(pOrder);
}

void TradeSpi::ReqCmbOrderInsert(const DstarApiReqCmbOrderInsertField* pCmbOrder)
{
    m_pApi->ReqCmbOrderInsert(pCmbOrder);
}
