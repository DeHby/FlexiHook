#ifndef __PROXY_HOOK_HEADER__
#define __PROXY_HOOK_HEADER__

#include "BaseHook.hpp"


template<typename OriginFunType>
class ProxyHook : public BaseHook {
public:
	using BaseHook::Install;
	using BaseHook::UnInstall;

	ProxyHook() {};

	ProxyHook(OriginFunType* s, OriginFunType* d, bool noExcept = true) {
		if (!this->Install(s, d) && !noExcept)
			throw std::runtime_error("Install Error");
	};

	template<typename... Args>
	auto Invoke(Args... args) -> decltype((reinterpret_cast<OriginFunType*>(this->_pOriginalFun))(args...)) {
		if (!this->_pOriginalFun)
			throw std::runtime_error("Original function is not set");

		return (reinterpret_cast<OriginFunType*>(this->_pOriginalFun))(args...);
	}
};

template<typename ClassType, typename ReturnType, typename... Args>
class ProxyHook<ReturnType(ClassType::*)(Args...)> : public BaseHook {
public:
	using BaseHook::Install;
	using BaseHook::UnInstall;

	ProxyHook() {}

	ProxyHook(ClassType* instance, ReturnType(ClassType::* s)(Args...), ReturnType(ClassType::* d)(Args...), bool noExcept = true) {
		if (!this->Install(instance, s, d) && !noExcept)
			throw std::runtime_error("Install Error");
	}

	bool Install(void* s, ReturnType(ClassType::* d)(Args...)) {
		if (s == nullptr || d == nullptr || Installed())
			return false;
		if (MinHook::Create(s, pointer_cast<void*>(d), _pOriginalFun))
			_pSourceFun = s;
		return _pSourceFun != nullptr;
	};

	template<typename C, typename R, typename... A, typename C1, typename R1, typename... A1>
	bool Install(R(C::* s)(A...), R1(C1::* d)(A1...)) {
		return BaseHook::Install(pointer_cast<void*>(s), pointer_cast<void*>(d));
	};

	auto Invoke(ClassType* self, Args... args) -> ReturnType {
		if (!this->_pOriginalFun)
			throw std::runtime_error("Original function is not set");
		return (self->*pointer_cast<ReturnType(ClassType::*)(Args...)>(this->_pOriginalFun))(args...);
	}
};

#endif