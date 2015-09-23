/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef posixtestclient_h__INCLUDED
#define posixtestclient_h__INCLUDED

#include <QObject>
#include <QThread>
#include <QString>
#include <QDateTime>
#include "EWrapper.h"
#include <memory>
#include <QMap>


class EPosixClientSocket;
struct Tick;
struct Depth;
struct TickerData;

enum State {
    ST_CONNECT,
    ST_PLACEORDER,
    ST_PLACEORDER_ACK,
    ST_CANCELORDER,
    ST_CANCELORDER_ACK,
    ST_PING,
    ST_PING_ACK,
    ST_IDLE,
    ST_REQMKTDATA,
    ST_REQMKTDATA_ACK
};


class PosixIBClient :public QObject, public EWrapper
{
   Q_OBJECT

public:

    explicit PosixIBClient(QObject *parent = 0);
    virtual ~PosixIBClient();
    void processMessages();

    long getNextValidUId();
    long nextValidUId;
    qint64 timeDiffMS;

public slots:
    void onConnect(QString host, int port,  int clientID);
    void onDisconnect();
    void onReqCurrentTime();
    void onReqMktData(QString contractId, QString exchange);
    void onReqMktDepth(QString contractId, QString exchange);
    void onCancelMktData(QString contractId);
    void onCancelMktDepth(QString contractId);

    void onProcessMessages();
    void onTest();

signals:
    void AdapterConnected();
    void AdapterDisconnected();
    void OrderSubmitted(QString msg);
    void OrderCanceled(QString msg);
    void OrderUpdated(QString msg);
    void OrderFilled(QString msg);

    void TickUpdating(const Tick &tick);
    void DepthUpdating(const Depth &depth);
    void BBidUpdating(const Depth &depth);
    void BAskUpdating(const Depth &depth);

    void ContractDetailUpdating(const TickerData &tickerData);


public:

    bool connect(const char *host, int port, int clientId = 0);
	void disconnect() const;
	bool isConnected() const;

private:

    long count = 0;
    const char *host;
    int port;
    int clientId;

    QMap<QString, long> conIdTickMap;
    QMap<QString, long> conIdDepthMap;


	void reqCurrentTime();
	void placeOrder();
	void cancelOrder();


public:
	// events
	void tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute);
	void tickSize(TickerId tickerId, TickType field, int size);
	void tickOptionComputation( TickerId tickerId, TickType tickType, double impliedVol, double delta,
		double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice);
	void tickGeneric(TickerId tickerId, TickType tickType, double value);
	void tickString(TickerId tickerId, TickType tickType, const IBString& value);
	void tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const IBString& formattedBasisPoints,
		double totalDividends, int holdDays, const IBString& futureExpiry, double dividendImpact, double dividendsToExpiry);
	void orderStatus(OrderId orderId, const IBString &status, int filled,
		int remaining, double avgFillPrice, int permId, int parentId,
		double lastFillPrice, int clientId, const IBString& whyHeld);
	void openOrder(OrderId orderId, const Contract&, const Order&, const OrderState&);
	void openOrderEnd();
	void winError(const IBString &str, int lastError);
	void connectionClosed();
	void updateAccountValue(const IBString& key, const IBString& val,
		const IBString& currency, const IBString& accountName);
	void updatePortfolio(const Contract& contract, int position,
		double marketPrice, double marketValue, double averageCost,
		double unrealizedPNL, double realizedPNL, const IBString& accountName);
	void updateAccountTime(const IBString& timeStamp);
	void accountDownloadEnd(const IBString& accountName);
	void nextValidId(OrderId orderId);
	void contractDetails(int reqId, const ContractDetails& contractDetails);
	void bondContractDetails(int reqId, const ContractDetails& contractDetails);
	void contractDetailsEnd(int reqId);
	void execDetails(int reqId, const Contract& contract, const Execution& execution);
	void execDetailsEnd(int reqId);
	void error(const int id, const int errorCode, const IBString errorString);
	void updateMktDepth(TickerId id, int position, int operation, int side,
		double price, int size);
	void updateMktDepthL2(TickerId id, int position, IBString marketMaker, int operation,
		int side, double price, int size);
	void updateNewsBulletin(int msgId, int msgType, const IBString& newsMessage, const IBString& originExch);
	void managedAccounts(const IBString& accountsList);
	void receiveFA(faDataType pFaDataType, const IBString& cxml);
	void historicalData(TickerId reqId, const IBString& date, double open, double high,
		double low, double close, int volume, int barCount, double WAP, int hasGaps);
	void scannerParameters(const IBString &xml);
	void scannerData(int reqId, int rank, const ContractDetails &contractDetails,
		const IBString &distance, const IBString &benchmark, const IBString &projection,
		const IBString &legsStr);
	void scannerDataEnd(int reqId);
	void realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
		long volume, double wap, int count);
	void currentTime(long time);
	void fundamentalData(TickerId reqId, const IBString& data);
	void deltaNeutralValidation(int reqId, const UnderComp& underComp);
	void tickSnapshotEnd(int reqId);
	void marketDataType(TickerId reqId, int marketDataType);
	void commissionReport( const CommissionReport& commissionReport);
	void position( const IBString& account, const Contract& contract, int position, double avgCost);
	void positionEnd();
	void accountSummary( int reqId, const IBString& account, const IBString& tag, const IBString& value, const IBString& curency);
	void accountSummaryEnd( int reqId);
	void verifyMessageAPI( const IBString& apiData);
	void verifyCompleted( bool isSuccessful, const IBString& errorText);
	void displayGroupList( int reqId, const IBString& groups);
	void displayGroupUpdated( int reqId, const IBString& contractInfo);

private:

    //use unique_ptr for release version, in qt unique_ptr makes code assistance disappeared

    std::auto_ptr<EPosixClientSocket> pClient;
    std::auto_ptr<QThread> pThread;
    State state;
    time_t sleepDeadline;

};

#endif

