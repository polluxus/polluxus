#include "messageprocessor.h"

MessageProcessor::MessageProcessor(PosixIBClient *pIBClient, QObject *parent)
     : QThread(parent), pIBClient(pIBClient){
 }

MessageProcessor::~MessageProcessor(){

}
