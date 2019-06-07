#include "../../include/DataWriters/DataToClientWriterChooser.hpp"

#include "../../include/DataWriters/TCPDataToClientWriter.hpp"
#include "../../include/DataWriters/SSLDataToClientWriter.hpp"

std::map<ClientConnectionType, std::function<int(Client&)>> 
	DataToClientWriterChooser::DATA_TO_CLIENT_WRITERS_BY_CONNECTION_TYPE = 
{
		{ 
			ClientConnectionType::ENCRYPTED, [](Client& client) 
			{
				return SSLDataToClientWriter::write(client); 
			} 
		},
		{ 
			ClientConnectionType::UNENCRYPTED, [](Client& client) {
				return TCPDataToClientWriter::write(client); 
			} 
		}
};