#ifndef ESUNNY_QUOTESPI_H
#define ESUNNY_QUOTESPI_H

#pragma once
#include "TapQuoteAPI.h"
#include "SimpleEvent.h"
#include <vector>
#include <map>
#include <string>
using namespace std;

class QuoteSpi : public ITapQuoteAPINotify
{
public:

    QuoteSpi(void);

	~QuoteSpi(void);

	void Run();

    int SubscribeQuote(string contract);

    int Init();

public:
    struct QuoteStruct
    {
        TAPICHAR                IP[21];                   ///< ����ǰ��IP��ַ
        TAPIUINT16              Port;                     ///< ����ǰ�ö˿�
        TAPISTR_20			    UserNo;					  ///< �û���
        TAPIYNFLAG			    ISModifyPassword;		  ///< �Ƿ��޸����룬'Y'��ʾ�ǣ�'N'��ʾ��
        TAPISTR_20			    Password;				  ///< �û�����
        TAPISTR_20			    NewPassword;			  ///< �����룬����������޸���������Ҫ��д���ֶ�
        TAPISTR_20			    QuoteTempPassword;		  ///< ������ʱ����
        TAPIYNFLAG			    ISDDA;					  ///< �Ƿ���ѽ��̬��֤��'Y'��ʾ�ǣ�'N'��ʾ��
        TAPISTR_30			    DDASerialNo;			  ///< ��̬��֤��
        TAPIAUTHCODE            AuthCode;				  ///< ��Ȩ��
        TAPISTR_300				KeyOperationLogPath;      ///< �ؼ�������־·��
    };

    QuoteStruct quoteLoginMsg;

public:
    /**
	* @brief	ϵͳ��¼���̻ص���
	* @details	�˺���ΪLogin()��¼�����Ļص�������Login()�ɹ���������·���ӣ�Ȼ��API������������͵�¼��֤��Ϣ��
	*			��¼�ڼ�����ݷ�������͵�¼�Ļ�����Ϣ���ݵ��˻ص������С�
	* @param[in] errorCode ���ش�����,0��ʾ�ɹ���
	* @param[in] info ��½Ӧ����Ϣ�����errorCode!=0����info=NULL��
	* @attention	�ûص����سɹ���˵���û���¼�ɹ������ǲ�����API׼����ϡ���Ҫ�ȵ�OnAPIReady���ܽ��в�ѯ�붩������
	* @ingroup G_Q_Login
	*/
	virtual void TAP_CDECL OnRspLogin(TAPIINT32 errorCode, const TapAPIQuotLoginRspInfo *info);

    /**
	* @brief	֪ͨ�û�API׼��������
	* @details	ֻ���û��ص��յ��˾���֪ͨʱ���ܽ��к����ĸ����������ݲ�ѯ������\n
	*			�˻ص�������API�ܷ����������ı�־��
	* @attention  ������ſ��Խ��к�����������
	* @ingroup G_Q_Login
	*/
	virtual void TAP_CDECL OnAPIReady();

    /**
	* @brief	API�ͷ���ʧȥ���ӵĻص�
	* @details	��APIʹ�ù������������߱��������������ʧȥ���Ӻ󶼻ᴥ���˻ص�֪ͨ�û���������������Ѿ��Ͽ���
	* @param[in] reasonCode �Ͽ�ԭ����롣����ԭ����μ��������б� \n
	* @ingroup G_Q_Disconnect
	*/
	virtual void TAP_CDECL OnDisconnect(TAPIINT32 reasonCode);

    /**
	* @brief	��������Ʒ����Ϣ��
	* @details	�˻ص��ӿ��������û����صõ�������Ʒ����Ϣ��
	* @param[in] sessionID ����ĻỰID
	* @param[in] errorCode �����룬��errorCode!=0ʱ,infoΪNULL��
	* @param[in] isLast ��ʾ�Ƿ������һ�����ݣ�
	* @param[in] info ���ص���Ϣ�������ʼָ�롣
	* @attention  ��Ҫ�޸ĺ�ɾ��info��ָʾ�����ݣ��������ý���������������Ч��
	* @ingroup G_Q_Commodity
	*/
	virtual void TAP_CDECL OnRspQryCommodity(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIQuoteCommodityInfo *info);

    /**
	* @brief ����ϵͳ�к�Լ��Ϣ
	* @param[in] sessionID ����ĻỰID��
	* @param[in] errorCode �����룬��errorCode!=0ʱ,infoΪNULL��
	* @param[in] isLast ��ʾ�Ƿ������һ�����ݣ�
	* @param[in] info		ָ�򷵻ص���Ϣ�ṹ�塣��errorCode��Ϊ0ʱ��infoΪ�ա�
	* @attention ��Ҫ�޸ĺ�ɾ��info��ָʾ�����ݣ��������ý���������������Ч��
	* @ingroup G_Q_Contract
	*/
	virtual void TAP_CDECL OnRspQryContract(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIQuoteContractInfo *info);

    /**
	* @brief	���ض��������ȫ�ġ�
	* @details	�˻ص��ӿ��������ض��������ȫ�ġ�ȫ��Ϊ��ǰʱ���������Ϣ��
	* @param[in] sessionID ����ĻỰID��
	* @param[in] isLast ��ʾ�Ƿ������һ�����ݣ�
	* @param[in] errorCode �����룬��errorCode!=0ʱ,infoΪNULL��
	* @param[in] info		ָ�򷵻ص���Ϣ�ṹ�塣��errorCode��Ϊ0ʱ��infoΪ�ա�
	* @attention  ��Ҫ�޸ĺ�ɾ��info��ָʾ�����ݣ��������ý���������������Ч��
	* @ingroup G_Q_Quote
	*/
	virtual void TAP_CDECL OnRspSubscribeQuote(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIQuoteWhole *info);

    /**
	* @brief �˶�ָ����Լ������Ľ���ص�
	* @param[in] sessionID ����ĻỰID��
	* @param[in] errorCode �����룬��errorCode!=0ʱ,infoΪNULL��
	* @param[in] isLast ��ʾ�Ƿ������һ�����ݣ�
	* @param[in] info		ָ�򷵻ص���Ϣ�ṹ�塣��errorCode��Ϊ0ʱ��infoΪ�ա�
	* @attention  ��Ҫ�޸ĺ�ɾ��info��ָʾ�����ݣ��������ý���������������Ч��
	* @ingroup G_Q_Quote
	*/
	virtual void TAP_CDECL OnRspUnSubscribeQuote(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIContract *info);

    /**
	* @brief	���ض�������ı仯���ݡ�
	* @details	�˻ص��ӿ�����֪ͨ�û�������Ϣ�����˱仯�������û��ύ�µ�����ȫ�ġ�
	* @param[in] info ���µ�����ȫ������
	* @attention ��Ҫ�޸ĺ�ɾ��Quoteָʾ�����ݣ��������ý���������������Ч��
	* @ingroup G_Q_Quote
	*/
	virtual void TAP_CDECL OnRtnQuote(const TapAPIQuoteWhole *info);

private:
	ITapQuoteAPI*  m_pAPI;
	TAPIUINT32	   m_uiSessionID;
    SimpleEvent    m_Event;
	bool		   m_bIsAPIReady;
    vector<char*>  m_SHFEList;
    vector<char*>  m_CZCEList;
    vector<char*>  m_DCEList;
    vector<char*>  m_CFFEXList;
    map< const char*, vector<char*> > m_ExchangeMap;

};
#endif //ESUNNY_QUOTESPI_H