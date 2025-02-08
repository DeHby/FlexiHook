#ifndef __CONTEXT_HOOK_HEADER_
#define __CONTEXT_HOOK_HEADER_

#include <string>

#include "BaseHook.hpp"
#include "CodeBuilder.h"

class ContextHook : public BaseHook
{
private:
    CodeBuilder _builder;

public:
    ContextHook() {};

    template <typename T>
    ContextHook(T *s, CodeBuilder::ContextHandler handler)
    {
        auto ret = Install(s, std::forward<CodeBuilder::ContextHandler &&>(handler));
        if (ret != 0)
            throw std::runtime_error("Context hook failed" + std::to_string(ret));
    };

    ~ContextHook()
    {
        UnInstall();
    };

    template <typename T>
    int Install(T *s, CodeBuilder::ContextHandler handler)
    {
        if (BaseHook::Installed())
            return 0;
        const CodeBuilder::CodeDeleter deleter = [this]()
        {
            BaseHook::UnInstall();
        };
        if (!_builder.Allocate(std::move(deleter)))
            return -1;
        if (!BaseHook::Install(s, _builder.GetMemory()))
            return -2;
        if (!_builder.SetHandler(std::move(handler)))
            return -3;
        if (!_builder.SetOriginal(_pOriginalFun))
            return -4;
        return 0;
    }

    bool UnInstall()
    {
        return _builder.Deallocate();
    }
};

#endif