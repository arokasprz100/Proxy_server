#include "DataToClientWriterChooser.hpp"

#include "DataToClientWriter.hpp"
#include "SSLDataToClientWriter.hpp"

std::map<ClientConnectionType, std::function<int(Client&)>> 
	DataToClientWriterChooser::DATA_TO_CLIENT_WRITERS_BY_CONNECTION_TYPE = {
		{ ClientConnectionType::ENCRYPTED, [](Client& client) {return SSLDataToClientWriter::write(client); } },
		{ ClientConnectionType::UNENCRYPTED, [](Client& client) {return DataToClientWriter::write(client); } }
};