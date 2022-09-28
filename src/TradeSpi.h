#ifndef ESUNNY_TRADESPI_H
#define ESUNNY_TRADESPI_H

#include "DstarTradeApiError.h"
#include "DstarTradeApiDataType.h"
#include "DstarTradeApiStruct.h"
#include "DstarTradeApi.h"
#include "SimpleEvent.h"


class TradeSpi : public IDstarTradeSpi
{
public:
    TradeSpi();
    virtual ~TradeSpi();

    int Init();

    bool IsReady();

    bool IsUdpAuth();

    DstarApiAccountIndexType GetAccountIndex();
    DstarApiAuthCodeType GetUdpAuthCode();

public:
    struct TradeStruct
    {
        DstarApiReqLoginField   m_LoginReq;
        char                    m_FrontIp[21];
        int                     m_FrontPort;
        char                    m_LogPath[64];
    };

    TradeStruct             tradeLoginMsg;
    IDstarTradeApi*         m_pApi;
    DstarApiRspLoginField   m_LoginInfo;
    SimpleEvent    m_Event;

    bool                    m_bReady;
    bool                    m_bUdpAuth;

protected:
    ///�ͻ�����֪ͨ�ӿ�ͨ�����ӶϿ�
    virtual void OnFrontDisconnected();

    ///����Ӧ��
    virtual void OnRspError(DstarApiErrorCodeType nErrorCode);

    ///��¼������Ӧ,������Ϊ0˵���û���¼�ɹ���
    virtual void OnRspUserLogin(const DstarApiRspLoginField * pLoginRsp);

    ///ϵͳ��Ϣ�ύ��Ӧ
    virtual void OnRspSubmitInfo(const DstarApiRspSubmitInfoField *pRspSubmitInfo);

    ///ϯλ��Ϣ��Ӧ
    virtual void OnRspSeat(const DstarApiSeatField* pSeat);

    ///��Լ��Ӧ
    virtual void OnRspContract(const DstarApiContractField *pContract);

    ///�����Ѳ�����Ӧ
    virtual void OnRspTrdFeeParam(const DstarApiTrdFeeParamField* pFeeParam);

    ///��֤�������Ӧ
    virtual void OnRspTrdMarParam(const DstarApiTrdMarParamField* pMarParam);

    ///�г�״̬��Ϣ��Ӧ
    virtual void OnRspTrdExchangeState(const DstarApiTrdExchangeStateField *pTrdExchangeState);

    ///��ֲֿ�����Ӧ
    virtual void OnRspPrePosition(const DstarApiPrePositionField *pPrePosition);

    ///�ֲֿ�����Ӧ
    virtual void OnRspPosition(const DstarApiPositionField *pPosition);

    ///�ʽ������Ӧ
    virtual void OnRspFund(const DstarApiFundField *pFund);

    ///ί����Ӧ
    virtual void OnRspOrder(const DstarApiOrderField *pOrder);

    ///������Ӧ
    virtual void OnRspOffer(const DstarApiOfferField *pOffer);

    ///�ɽ���Ӧ
    virtual void OnRspMatch(const DstarApiMatchField *pTrade);

    ///�������Ӧ
    virtual void OnRspCashInOut(const DstarApiCashInOutField *pCashInOut);

    ///API׼������,ֻ���û��յ��˾���֪ͨʱ���ܽ��к����Ĳ���
    virtual void OnApiReady(const DstarApiSerialIdType nSerialId);

    ///UDP��֤������Ӧ,������Ϊ0˵����֤�ɹ���
    virtual void OnRspUdpAuth(const DstarApiRspUdpAuthField *pRspUdpAuth);

    ///����Ӧ��
    virtual void OnRspOrderInsert(const DstarApiRspOrderInsertField *pOrderInsert);

    ///����Ӧ��
    virtual void OnRspOrderDelete(const DstarApiRspOrderDeleteField *pOrderDelete);

    ///����Ӧ��
    virtual void OnRspOfferInsert(const DstarApiRspOfferInsertField *pOfferInsert);

    ///���������Ӧ��
    virtual void OnRspLastReqId(const DstaApiRspLastReqIdField *pLastReqId);

    ///����֪ͨ
    virtual void OnRtnOrder(const DstarApiOrderField *pOrder);

    ///�ɽ�֪ͨ
    virtual void OnRtnMatch(const DstarApiMatchField *pTrade);

    ///�����֪ͨ
    virtual void OnRtnCashInOut(const DstarApiCashInOutField *pCashInOut);

    ///����֪ͨ
    virtual void OnRtnOffer(const DstarApiOfferField *pOffer);

    ///ѯ��֪ͨ
    virtual void OnRtnEnquiry(const DstarApiEnquiryField *pEnquiry);

    ///�г�״̬֪ͨ
    virtual void OnRtnTrdExchangeState(const DstarApiTrdExchangeStateField *pTrdExchangeState);

    ///��ӯ֪ͨ
    virtual void OnRtnPosiProfit(const DstarApiPosiProfitField *pPosiProfit);

    ///ϯλ��Ϣ֪ͨ
    virtual void OnRtnSeat(const DstarApiSeatField* pSeat);

    // �����޸�Ӧ��
    virtual void OnRspPwdMod(const DstarApiRspPwdModField *pRspPwdModField);

    // �����޸�֪ͨ
    virtual void OnRtnPwdMod(const DstarApiPwdModField *pPwdModField);

public:
    //udp��֤
    void UdpAuth();

    //�ύ�ɼ���Ϣ
    int SubmitSystemInfo();

    // ����
    void ReqOrderInsert(const DstarApiReqOrderInsertField *pOrder);

    // ����
    void ReqOfferInsert(const DstarApiReqOfferInsertField *pOffer);

    // ����
    void ReqOrderDelete(const DstarApiReqOrderDeleteField *pOrder);

    // ��ϱ���
    void ReqCmbOrderInsert(const DstarApiReqCmbOrderInsertField *pCmbOrder);

};

#endif //ESUNNY_TRADESPI_H
