/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */
#include <stdio.h>
#include <QDebug>
#include "posixibclient.h"

#include "EPosixClientSocket.h"
#include "EPosixClientSocketPlatform.h"
#include "Contract.h"
#include "Order.h"
#include "marketdata.h"

const int PING_DEADLINE = 2; // seconds
const int SLEEP_BETWEEN_PINGS = 30; // seconds

///////////////////////////////////////////////////////////
// member funcs
PosixIBClient::PosixIBClient(QObject *parent) : QObject(parent) //QObject *parent   : QObject(parent)
{
    state = ST_CONNECT;
    sleepDeadline = 0;

    conIdTickMap.clear();
    conIdDepthMap.clear();

    nextValidUId = static_cast<long int> (time(NULL));

    pThread.reset(new QThread);
    pThread->start();
    this->moveToThread(pThread.get());

    pClient.reset(new EPosixClientSocket(this));
}

PosixIBClient::~PosixIBClient()
{
    pClient.release();
    pThread->exit( 0 );
    pThread->wait();
    pThread.release();

}

long PosixIBClient::getNextValidUId()
{
    return nextValidUId++;
}


bool PosixIBClient::connect(const char *host, int port, int clientId)
{
    // trying to connect
    bool bRes = pClient->eConnect( host, port, clientId);
    if(bRes)
    {
        qDebug() << "PosixIBClient: connected to server";

    }
    else
    {
        qDebug() << "PosixIBClient: failed to connect to server";
    }
    return bRes;
}

void PosixIBClient::disconnect() const
{
    pClient->eDisconnect();
    qDebug() << "PosixIBClient: disconnected to server";

}

bool PosixIBClient::isConnected() const
{
    return pClient->isConnected();
}


void PosixIBClient::onConnect(QString host, int port,  int clientId)
{

    qDebug() << "PosixIBClient: onConnect() in thread: " << QThread::currentThreadId();


    this->host = host.toUtf8().constData();
    this->port = port;
    this->clientId = clientId;

    bool bConn = false;
    int attemptCount = 0;

    while(attemptCount <= 10)
    {
        ++attemptCount;
        qDebug() << "Connecting to IB, No of try:" << attemptCount ;
        bConn = connect(this->host, port, clientId);

        if(bConn)
        {
            break;
        }

        sleep(5);

    }

    if(bConn)
    {
        qDebug() << "Connected------------------------------.";
        emit AdapterConnected();
        reqCurrentTime();
        //onProcessMessages();
    }
    else
    {

        qDebug() << "Failed to connect. Max no of try reached.";
        emit AdapterDisconnected();
    }
}

void PosixIBClient::onDisconnect()
{
    qDebug() <<"PosixIBClient::onDisconnect() entered";

    disconnect();

    while(isConnected())
    {
        sleep(2);
    }

    qDebug() <<"Disonnected";
    emit AdapterDisconnected();
}

void PosixIBClient::emitAdapterDisconnected()
{
    emit AdapterDisconnected();
}

void PosixIBClient::onProcessMessages()
{
    while (isConnected())
    {
        processMessages();
    }

}



void PosixIBClient::processMessages()
{
    //qDebug() << "PosixIBClient: processMessages() in thread: " << QThread::currentThreadId();

    fd_set readSet, writeSet, errorSet;

    struct timeval tval;
    tval.tv_usec = 0;
    tval.tv_sec = 0;

    time_t now = time(NULL);

    if( sleepDeadline > 0) {
        // initialize timeout with m_sleepDeadline - now

        tval.tv_sec = sleepDeadline - now;
        qDebug() << "PosixClient::processMessages - tv_sec:" << tval.tv_sec;
    }

    if( pClient->fd() >= 0 )
    {


        FD_ZERO( &readSet);
        writeSet = readSet;

        FD_SET( pClient->fd(), &readSet);

        if( !pClient->isOutBufferEmpty())
            FD_SET( pClient->fd(), &writeSet);

        FD_SET( pClient->fd(), &errorSet);

        int ret = select( pClient->fd() + 1, &readSet, &writeSet, NULL, &tval);

        if( ret == 0) { // timeout
            //qDebug() <<"PosixClient::processMessages: timeout";
            return;
        }

        if( ret < 0) {	// error
           //qDebug() <<"PosixClient::processMessages: disconnect";
            disconnect();

            return;
        }

        if( pClient->fd() < 0)
        {
            //qDebug() << "PosixClient::processMessages, fd() < 0, return b4 writeset";
            return;
        }

        if( FD_ISSET( pClient->fd(), &writeSet)) {
            // socket is ready for writing
            //qDebug() << "PosixClient::processMessages: onSend()";
            pClient->onSend();
        }

        if( pClient->fd() < 0)
        {
            //qDebug() << "PosixClient::processMessages, fd() < 0, return b4 readSet";
            return;
        }

        if( FD_ISSET( pClient->fd(), &readSet)) {
            // socket is ready for reading
            //qDebug() << "PosixClient::processMessages: onReceive()";
            pClient->onReceive();
        }


    }

}

void PosixIBClient::onTest()
{

    //qDebug() << "Placing order ....";
    //placeOrder();

//    contract.symbol = "ES";
//    contract.secType = "FUT";
//    contract.exchange = "GLOBEX";
//    contract.expiry = "201512";
//	  contract.currency = "USD";

    qDebug() << "IBClient onTest():  ES FUT 201512....";

    QString contractId = "167205842";
    QString exchange = "GLOBEX";

    Contract contract;
    contract.conId = contractId.toLong();
    pClient->reqContractDetails(getNextValidUId(), contract);

    if(conIdTickMap.contains(contractId))
    {
        qDebug() << "IBClient onTest: cancelMktData ES 201512";
        onCancelMktData(contractId);
    }
    else
    {
        qDebug() << "IBClient onTest: reqMktData ES 201512";
        onReqMktData(contractId, exchange);
    }

    if(conIdDepthMap.contains(contractId))
    {
        qDebug() << "IBClient onTest: onCancelMktDepth ES 201512";
        onCancelMktDepth(contractId);
    }
    else
    {
        qDebug() << "IBClient onTest: onCancelMktDepth ES 201512";
        onReqMktDepth(contractId, exchange);
    }


}


void PosixIBClient::onReqCurrentTime()
{
    qDebug() << "onReqCurrentTime() entered";
}

void PosixIBClient::onReqMktData(QString contractId, QString exchange)
{
    //check if it is in conIdTickMap

    if(!conIdTickMap.contains(contractId))
    {
        long reqId = getNextValidUId();
        long reqId2 = getNextValidUId();
        conIdTickMap[contractId] = reqId;

        Contract contract;
        contract.conId = contractId.toLong();
        contract.exchange = exchange.toStdString();
        pClient->reqContractDetails(reqId2, contract);
//        contract.symbol = "ES";
//        contract.secType = "FUT";
//        contract.exchange = "GLOBEX";
//        contract.expiry = "201509";
//        contract.currency = "USD";

        qDebug() << "I am going to reqMktData";

        pClient->reqMktData(reqId, contract, "233", false, TagValueListSPtr());
    }

}

void PosixIBClient::onReqMktDepth(QString contractId, QString exchange)
{
    //check if it is in conIdDepthMap

    if(!conIdDepthMap.contains(contractId))
    {
        long reqId = getNextValidUId();
        conIdDepthMap[contractId] = reqId;

        Contract contract;
        contract.conId = contractId.toLong();
        contract.exchange = exchange.toStdString();
//        contract.symbol = "ES";
//        contract.secType = "FUT";
//        contract.exchange = "GLOBEX";
//        contract.expiry = "201512";
//        contract.currency = "USD";

        qDebug() << "I am going to reqMktDepth";

        pClient->reqMktDepth(reqId, contract, 5, TagValueListSPtr());
    }
}

void PosixIBClient::onCancelMktData(QString contractId)
{
    if(conIdTickMap.contains(contractId))
    {
        pClient->cancelMktData(conIdTickMap[contractId]);
        //clear this contract in map
        conIdTickMap.remove(contractId);
    }
}

void PosixIBClient::onCancelMktDepth(QString contractId)
{
    if(conIdDepthMap.contains(contractId))
    {
        pClient->cancelMktDepth(conIdDepthMap[contractId]);
        //clear this contract in map
        conIdDepthMap.remove(contractId);

    }
}


//////////////////////////////////////////////////////////////////
// methods
void PosixIBClient::reqCurrentTime()
{
//    qDebug() << "PosixIBClient: requesting current time";
//    sleepDeadline = time( NULL) + PING_DEADLINE;
//    state = ST_PING_ACK;

      qDebug() << "Start reqTime:" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
      timeReq = QDateTime::currentMSecsSinceEpoch();
      pClient->reqCurrentTime();

}

void PosixIBClient::placeOrder()
{
	Contract contract;
	Order order;

    contract.symbol = "ES";
    contract.secType = "FUT";
    contract.exchange = "GLOBEX";
    contract.expiry = "201512";
	contract.currency = "USD";

    order.action = "BUY";
    order.totalQuantity = 1;
	order.orderType = "LMT";
    order.lmtPrice = 1945.00;

    long nextOrderId = getNextValidUId();
    qDebug() << "Placing order using ID:" << nextOrderId;
    pClient->placeOrder(nextOrderId, contract, order);

}

void PosixIBClient::cancelOrder()
{

}

///////////////////////////////////////////////////////////////////
// events
void PosixIBClient::orderStatus( OrderId orderId, const IBString &status, int filled,
	   int remaining, double avgFillPrice, int permId, int parentId,
	   double lastFillPrice, int clientId, const IBString& whyHeld)

{
    QString msg("OrderStatus | orderId:");
    msg.append(QString::number(orderId));
    msg.append(", Status:");
    msg.append(QString::fromStdString(status));
    msg.append(", remaining:");
    msg.append(QString::number(filled));
    msg.append(", filled:");
    msg.append(QString::number(remaining));
    msg.append(", avgPx:");
    msg.append(QString::number(avgFillPrice));

//    msg =  "OrderStatus:" + "OrderId:" + orderId + ", Status:"
//                          + QString::fromStdString(status) + ", filled:" + filled
//                          + ", remaining:" + remaining + ", avgPx:" + avgFillPrice
//                          + ", permId:" + permId + ", parentId:" + parentId
//                          + ", lastFillPrice:" + lastFillPrice
//                          + ", clientId:" + clientId
//                          + ", whyHeld:" + QString::fromStdString(whyHeld);

    qDebug() << msg;

    emit OrderUpdated(msg);
}

void PosixIBClient::nextValidId( OrderId orderId)
{
    //qDebug() << "Received nextValidId:" << orderId;
    //nextOrderId = orderId;
}

void PosixIBClient::currentTime(long time)
{
    timeResp = QDateTime::currentMSecsSinceEpoch();
    latency =qint64(0.5 * (timeResp - timeReq));
    qDebug() << "End reqTime:" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    timeDiffMS = timeResp - time * 1000;
    emit AdjustTimeDiff( timeDiffMS);
    qDebug() << "timeDiffMS:" << timeDiffMS;
}

void PosixIBClient::error(const int id, const int errorCode, const IBString errorString)
{

	if( id == -1 && errorCode == 1100) // if "Connectivity between IB and TWS has been lost"
		disconnect();
}

void PosixIBClient::tickPrice( TickerId tickerId, TickType field, double price, int canAutoExecute)
{
    //qDebug() << "tickPrice():" << tickerId <<", TickType:" << field << ", price:" << price;

}
void PosixIBClient::tickSize( TickerId tickerId, TickType field, int size)
{
    //qDebug() << "tickSize():" << tickerId <<", TickType:" << field << ", size:" << size;

}
void PosixIBClient::tickOptionComputation( TickerId tickerId, TickType tickType, double impliedVol, double delta,
											 double optPrice, double pvDividend,
											 double gamma, double vega, double theta, double undPrice) {}
void PosixIBClient::tickGeneric(TickerId tickerId, TickType tickType, double value) {}
void PosixIBClient::tickString(TickerId tickerId, TickType tickType, const IBString& value)
{
    //use tickType = 45 to update timeDiffMS
//    if(tickType == 45)
//    {
//        qDebug() << "tickString():" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
//                 <<" >>tickerId:" << tickerId
//                 <<", TickType:" << tickType
//                 << ", Value:" << QString::fromStdString(value);

//    }

    //Handle RTVolume
    QString conId = conIdTickMap.key(tickerId, "");
    if(tickType == 48 && conId != "")
    {


        QString str = QString::fromStdString(value);
        QStringList strList = str.split(";");
        Tick tick;

        tick.contractId = conId;
        tick.timeStamp = strList[2];
        tick.price = strList[0];
        tick.size = strList[1];

        emit TickUpdating(tick);
        qDebug() << "tickString():" << tick.timeStamp
                 <<" >>contractId:" << conId
                 <<", TickType:" << tickType
                 << ", Value:" << str;
    }

}
void PosixIBClient::tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const IBString& formattedBasisPoints,
							   double totalDividends, int holdDays, const IBString& futureExpiry, double dividendImpact, double dividendsToExpiry) {}
void PosixIBClient::openOrder( OrderId orderId, const Contract&, const Order&, const OrderState& ostate) {}
void PosixIBClient::openOrderEnd() {}
void PosixIBClient::winError( const IBString &str, int lastError) {}
void PosixIBClient::connectionClosed(){}
void PosixIBClient::updateAccountValue(const IBString& key, const IBString& val,
										  const IBString& currency, const IBString& accountName) {}
void PosixIBClient::updatePortfolio(const Contract& contract, int position,
		double marketPrice, double marketValue, double averageCost,
		double unrealizedPNL, double realizedPNL, const IBString& accountName){}
void PosixIBClient::updateAccountTime(const IBString& timeStamp) {}
void PosixIBClient::accountDownloadEnd(const IBString& accountName) {}

void PosixIBClient::contractDetails(int reqId, const ContractDetails& contractDetails)
{

    qDebug() << "conId:" << QString::number(contractDetails.summary.conId)
             <<", exch:" << QString::fromStdString(contractDetails.summary.exchange)
             <<", expiry:" << QString::fromStdString(contractDetails.summary.expiry);

    ContractInfo contractInfo;
    contractInfo.contractId = QString::number(contractDetails.summary.conId);
    contractInfo.symbol = QString::fromStdString(contractDetails.summary.symbol);
    contractInfo.secType = QString::fromStdString(contractDetails.summary.secType);
    contractInfo.exchange = QString::fromStdString(contractDetails.summary.exchange);
    contractInfo.primaryExchange = QString::fromStdString(contractDetails.summary.primaryExchange);
    contractInfo.expiry = QString::fromStdString(contractDetails.summary.expiry);
    contractInfo.currency = QString::fromStdString(contractDetails.summary.currency);

    contractInfo.minTick = contractDetails.minTick;

    emit ContractDetailUpdating(contractInfo);


}

void PosixIBClient::bondContractDetails( int reqId, const ContractDetails& contractDetails) {}
void PosixIBClient::contractDetailsEnd( int reqId) {}
void PosixIBClient::execDetails( int reqId, const Contract& contract, const Execution& execution) {}
void PosixIBClient::execDetailsEnd( int reqId) {}

void PosixIBClient::updateMktDepth(TickerId id, int position, int operation, int side,
                                      double price, int size)
{

    QString conId = conIdDepthMap.key(id, "");
    if(conId != "")
    {

        Depth depth;
        depth.contractId = conId;
        depth.timeStamp = QDateTime::currentDateTime().addMSecs(-timeDiffMS);

        depth.side = side;
        depth.size = size;
        depth.price = price;
        depth.position = position;
        depth.operation = operation;

        if(side == 0) //ask
        {

            emit BAskUpdating(depth);
        }
        else
        {
            emit BBidUpdating(depth);
        }

        qDebug() << "updateMktDepth():" << depth.timeStamp.toString("hh:mm:ss.zzz")
                 <<" >>contractId:" << conId
                 <<", side:" << side
                 << ", size:" << size
                 << ", price:" << price
                 << ", position:" << position
                 << ", operation:" << operation;
    }

}

void PosixIBClient::updateMktDepthL2(TickerId id, int position, IBString marketMaker, int operation,
                                        int side, double price, int size)
{

}
void PosixIBClient::updateNewsBulletin(int msgId, int msgType, const IBString& newsMessage, const IBString& originExch) {}
void PosixIBClient::managedAccounts( const IBString& accountsList) {}
void PosixIBClient::receiveFA(faDataType pFaDataType, const IBString& cxml) {}
void PosixIBClient::historicalData(TickerId reqId, const IBString& date, double open, double high,
									  double low, double close, int volume, int barCount, double WAP, int hasGaps) {}
void PosixIBClient::scannerParameters(const IBString &xml) {}
void PosixIBClient::scannerData(int reqId, int rank, const ContractDetails &contractDetails,
	   const IBString &distance, const IBString &benchmark, const IBString &projection,
	   const IBString &legsStr) {}
void PosixIBClient::scannerDataEnd(int reqId) {}
void PosixIBClient::realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
								   long volume, double wap, int count) {}
void PosixIBClient::fundamentalData(TickerId reqId, const IBString& data) {}
void PosixIBClient::deltaNeutralValidation(int reqId, const UnderComp& underComp) {}
void PosixIBClient::tickSnapshotEnd(int reqId) {}
void PosixIBClient::marketDataType(TickerId reqId, int marketDataType) {}
void PosixIBClient::commissionReport( const CommissionReport& commissionReport) {}
void PosixIBClient::position( const IBString& account, const Contract& contract, int position, double avgCost) {}
void PosixIBClient::positionEnd() {}
void PosixIBClient::accountSummary( int reqId, const IBString& account, const IBString& tag, const IBString& value, const IBString& curency) {}
void PosixIBClient::accountSummaryEnd( int reqId) {}
void PosixIBClient::verifyMessageAPI( const IBString& apiData) {}
void PosixIBClient::verifyCompleted( bool isSuccessful, const IBString& errorText) {}
void PosixIBClient::displayGroupList( int reqId, const IBString& groups) {}
void PosixIBClient::displayGroupUpdated( int reqId, const IBString& contractInfo) {}
