#include "LogSystem.hpp"

std::unique_ptr<Logging> LogSystem::logger = nullptr;
bool LogSystem::isInitialized = false;