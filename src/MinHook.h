#ifndef __MINHOOK_HEADER__
#define __MINHOOK_HEADER__

#include <shared_mutex>
#include "minhook/MinHook.h"

class MinHook {
private:
	static bool _init;
	static std::shared_mutex _mutex;
	static thread_local MH_STATUS _lastError;

	static bool isStatusValid(MH_STATUS s);
public:
	static MH_STATUS GetLastError();

	static bool Initialize();
	static bool Uninitialize();

	static bool Create(void* s, void* d, void*& pOriginal)
	{
		if (!_init) return false;
		return isStatusValid(MH_CreateHook(s, d, &pOriginal));
	}

	static bool Create(const LPCWSTR pszModule, const LPCSTR pszProcName, void* d, void*& pOriginal, void*& pSource)
	{
		if (!_init) return false;
		return isStatusValid(MH_CreateHookApiEx(pszModule, pszProcName, d, &pOriginal, &pSource));

	}

	static bool Remove(void* pTarget = MH_ALL_HOOKS)
	{
		if (!_init) return false;
		return isStatusValid(MH_RemoveHook(pTarget));
	}

	static bool Enable(void* pTarget = MH_ALL_HOOKS)
	{
		if (!_init) return false;
		return isStatusValid(MH_EnableHook(pTarget));;
	}

	static bool Disable(void* pTarget = MH_ALL_HOOKS)
	{
		if (!_init) return false;
		return isStatusValid(MH_DisableHook(pTarget));
	}

	static const char* StatusToString(MH_STATUS status)
	{
		return MH_StatusToString(status);
	}
};

#endif