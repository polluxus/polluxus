Poluxus - Qt based trading system, currently with IB TWS API connection

-----------------About multithreading design:-----------------------------------------------------------------------
PolluxusMain runs in the main GUI thread
ContractManager runs in a seperate thread
MessageProcessor runs in a seperate thread
PosixIBClient runs in a seperate thread, but functions such as processmessage(), tickString() run in the same thread as MessageProcessor


----------------About ContractDetails()-----------------------------------------------------------------------------
Contractdetails will be requested when reqMktData() or reqMktDepth()
