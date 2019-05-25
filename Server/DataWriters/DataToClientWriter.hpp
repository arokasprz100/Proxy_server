#ifndef DataToClientWriter_hpp
#define DataToClientWriter_hpp

class DataToClientWriter
{
public:

	// TUTAJ POLL SA DOBRZE
	static int writeDataToClient(Client& client) {
		int dataToSendSize = client.m_httpResponseFromServer.size();
		//std::cout << "DATA TO SEND SIZE = " << dataToSendSize << std::endl;
		int dataAlreadyWritenSize = 0;
		int operationStatus = 0;
		while (true) {
			operationStatus = send(client.clientSocket, 
				client.m_httpResponseFromServer.data() + dataAlreadyWritenSize, 
				client.m_httpResponseFromServer.size() - dataAlreadyWritenSize, MSG_NOSIGNAL);
			//std::cout << "[WRITING TO CLIENT STATUS] " << operationStatus << std::endl;
			if (operationStatus <= 0) {
				client.clientConnectionPollFD->events |= POLLOUT; // nie udalo sie zapisac danych, chcemy pisac jeszcze raz
				return operationStatus;
			}
			dataAlreadyWritenSize += operationStatus;
			if (dataAlreadyWritenSize == dataToSendSize) {
				client.m_httpResponseFromServer.clear();
				client.clientConnectionPollFD->events = POLLIN; // udalo sie zapisac dane, chcemy czytac dane od klienta
				return operationStatus;
			}
		}
	}

};

#endif // DataToClientWriter_hpp