#ifndef __BASE_HOOK_HEADER__
#define __BASE_HOOK_HEADER__

#include "MinHook.h"

class BaseHook {
protected:
	void* _pSourceFun;
	void* _pOriginalFun;

	bool _isEnabled;

	bool Install(void* s, void* d) {
		if (s == nullptr || d == nullptr || Installed())
			return false;
		if (MinHook::Create(s, d, _pOriginalFun))
			_pSourceFun = s;
		return _pSourceFun != nullptr;
	};

	virtual bool Install(const LPCWSTR pszModule, const LPCSTR pszProcName, void* d) {
		if (pszModule == nullptr || pszProcName == nullptr || d == nullptr || Installed())
			return false;
		return MinHook::Create(pszModule, pszProcName, d, _pOriginalFun, _pSourceFun) && _pSourceFun != nullptr;
	};

	virtual bool UnInstall() {
		if (!Installed())
			return false;
		if (MinHook::Remove(_pSourceFun))
		{
			_isEnabled = false;
			_pSourceFun = _pOriginalFun = nullptr;
			return true;
		}
		return false;
	};
public:
	BaseHook() :_pSourceFun(nullptr), _pOriginalFun(nullptr), _isEnabled(false) {};


	bool Installed() const
	{
		return _pSourceFun != nullptr && _pOriginalFun != nullptr;
	}

	bool Enabled() const
	{
		return _isEnabled;
	}

	static MH_STATUS GetLastError()
	{
		return MinHook::GetLastError();
	}

	virtual bool Enable() {
		if (_isEnabled || !Installed())
			return false;
		_isEnabled = MinHook::Enable(_pSourceFun);
		return _isEnabled;
	};

	virtual bool Disable() {
		if (!_isEnabled || !Installed())
			return false;
		_isEnabled = !MinHook::Disable(_pSourceFun);
		return !_isEnabled;
	};

	virtual ~BaseHook() {
		UnInstall();
	};
};

#endif