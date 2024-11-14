#include "CodeBuilder.h"

bool CodeBuilder::_HandlerBridge(ThreadContext* ctx)
{
	if (_handler)
		return _handler(*ctx);
	return false;
}

void* CodeBuilder::GetMemory() const
{
	return _pMem;
}

bool CodeBuilder::Allocate()
{
	if (_pMem)
		return true;
	SYSTEM_INFO info = { 0 };
	::GetSystemInfo(&info);
	size_t pageSize = info.dwPageSize;
	size_t allocSize = ((CodeWriter::CodeSize() + pageSize - 1) / pageSize) * pageSize;
	_pMem = ::VirtualAlloc(0, allocSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	return _pMem && _writer.Attach(_pMem);
}

bool CodeBuilder::Allocate(CodeDeleter deleter)
{
	if (Allocate())
	{
		_deleter = std::move(deleter);
		return true;
	}
	return false;
}

bool CodeBuilder::Deallocate()
{
	if (_pMem)
	{
		if (_deleter)
			_deleter();
		if (VirtualFree(_pMem, 0, MEM_RELEASE))
		{
			_pMem = nullptr;
			_deleter = nullptr;
			_handler = nullptr;
			return true;
		}
	}
	return false;
}

bool CodeBuilder::SetHandler(ContextHandler handle)
{
	if (_writer.isAttach())
	{
		_handler = std::move(handle);
		_writer.WriteSelf(this);
		_writer.WriteHandle(pointer_cast<void*>(&CodeBuilder::_HandlerBridge));
		return true;
	}
	return false;
}

bool CodeBuilder::SetOriginal(void* pOriginal)
{
	if (_writer.isAttach())
	{
		_writer.WriteOriginal(pOriginal);
		return true;
	}
	return false;
}
