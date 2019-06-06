#include "../../include/DataReaders/DataFromClientReaderChooser.hpp"

#include "../../include/DataReaders/DataFromClientReader.hpp"
#include "../../include/DataReaders/SSLDataFromClientReader.hpp"

std::map<ClientConnectionType, std::function<int(Client&)>> 
	DataFromClientReaderChooser::UNENCRYPTED_DATA_FROM_CLIENT_READERS_BY_CONNECTION_TYPE = {
		{ClientConnectionType::ENCRYPTED, [] (Client& client){ return SSLDataFromClientReader::readUnencryptedData(client); } },
		{ClientConnectionType::UNENCRYPTED, [] (Client& client){ return DataFromClientReader::readUnencryptedData(client); } }
};

std::map<ClientConnectionType, std::function<int(Client&)>> 
	DataFromClientReaderChooser::ENCRYPTED_DATA_FROM_CLIENT_READERS_BY_CONNECTION_TYPE = {
		{ClientConnectionType::ENCRYPTED, [] (Client& client){ return SSLDataFromClientReader::readEncryptedData(client); } },
		{ClientConnectionType::UNENCRYPTED, [] (Client& client){ return DataFromClientReader::readEncryptedData(client); } }
};