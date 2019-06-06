#ifndef OperationStatusHandlerChooser_hpp
#define OperationStatusHandlerChooser_hpp

#include <functional>
#include <map>

#include "../Client.hpp"
#include "../ClientConnectionType.hpp"

class OperationStatusHandlerChooser final 
{
public:

	static auto chooseOperationStatusHandler(ClientConnectionType clientConnectionType) {
		return OPERATION_STATUS_HANDLERS_BY_CLIENT_CONNECTION_TYPE[clientConnectionType];
	}

private:

	static std::map<ClientConnectionType, std::function<void(Client&, int, std::vector<pollfd>::iterator)>> 
		OPERATION_STATUS_HANDLERS_BY_CLIENT_CONNECTION_TYPE;

};

#endif // OperationStatusHandlerChooser_hpp