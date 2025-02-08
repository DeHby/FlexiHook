#include "WrapMinHook.h"

bool MinHook::_init = false;
std::shared_mutex MinHook::_mutex;
thread_local MH_STATUS MinHook::_lastError = MH_UNKNOWN;

inline bool MinHook::isStatusValid(MH_STATUS s)
{
	if (s != MH_OK)
	{
		_lastError = s;
		return false;
	}
	return true;
}

MH_STATUS MinHook::GetLastError()
{
	return _lastError;
}
bool MinHook::Initialize() {
	{
		std::shared_lock<std::shared_mutex> lock(_mutex);
		if (_init) return false;
	}
	std::unique_lock<std::shared_mutex> lock(_mutex);
	_init = isStatusValid(MH_Initialize());
	return _init;
}

bool MinHook::Uninitialize() {
	{
		std::shared_lock<std::shared_mutex> lock(_mutex);
		if (!_init) return false;
	}
	std::unique_lock<std::shared_mutex> lock(_mutex);
	_init = !isStatusValid(MH_Uninitialize());
	return !_init;
}
