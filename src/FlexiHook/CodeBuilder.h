#ifndef __CONTEXT_BUILDER_HPP__
#define __CONTEXT_BUILDER_HPP__

#include <Windows.h>

#include <memory>
#include <functional>

#include "Utils.hpp"

#include "BuilderDef.hpp"

class CodeBuilder
{
public:
	using CodeDeleter = std::function<void()>;
	using ContextHandler = std::function<bool(ThreadContext&)>;

private:
	void* _pMem;
	CodeWriter _writer;

	CodeDeleter _deleter;
	ContextHandler _handler;

	// 回调桥
	bool _HandlerBridge(ThreadContext* ctx);
public:
	CodeBuilder() : _pMem(nullptr) {};

	~CodeBuilder() {
		Deallocate();
	};

	void* GetMemory() const;

	bool Allocate();

	bool Allocate(CodeDeleter deleter);

	bool Deallocate();

	bool SetHandler(ContextHandler handle);

	bool SetOriginal(void* pOriginal);

};

#endif