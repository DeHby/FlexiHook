#ifndef __ARCH_BUILDER_DEF_HPP__
#define __ARCH_BUILDER_DEF_HPP__

#include <memory>
#include <cstdint>
#include <wtypes.h>


#if defined(_M_X64) || defined(__amd64__)  // x64 架构
#include "X64BuilderDef.h"
#elif defined(_M_IX86) || defined(__i386__)  // x86 架构
#include "X86BuilderDef.h"
#else
#error "Unsupported CPU architecture. Only x86 and x64 are supported."
#endif


class CodeWriter
{
private:
	void* _buffer;
public:
	CodeWriter() :_buffer(nullptr) {};

	template<typename T = void>
	bool Attach(T* buf)
	{
		if (!buf) return false;
		std::memcpy(buf, BYTE_SHELLCODE, CodeSize());
		_buffer = buf;
		return true;
	}

	bool isAttach() const
	{
		return _buffer != nullptr;
	}

	void Detach()
	{
		_buffer = nullptr;
	}

	static constexpr size_t CodeSize()
	{
		return sizeof(BYTE_SHELLCODE);
	}

	void WriteSelf(void* self)
	{
		*reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(_buffer) + OFFSET_SELF) = self;
	}

	void WriteHandle(void* handle)
	{
		*reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(_buffer) + OFFSET_HANDLE) = handle;
	}

	void WriteOriginal(void* original)
	{
		*reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(_buffer) + OFFSET_ORIGINAL) = original;
	}
};

#endif