# [FlexiHook](https://github.com/DeHby/FlexiHook)

这是一个基于minhook封装的hook库，支持普通函数、类方法、虚函数（理论支持）、任意上下文（ContextHook 类似Frida interceptor）Hook方式

ContextHook的方式目前暂不支持修改ESP/RSP寄存器

# 项目开发环境

- visual studio 2022 v143
- C++14
- minhook 1.3.3
- 编译器调用约定使用默认

# 使用例子

## APIHOOK

```c_cpp
#include "Hook/ProxyHook.hpp"

static ProxyHook<decltype(MessageBoxA)> s_hookInstace;

int WINAPI MyMessageBoxW(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCWSTR lpText,
    _In_opt_ LPCWSTR lpCaption,
    _In_ UINT uType)
{
    return s_hookInstace.Invoke(hWnd, L"测试拦截", lpCaption, uType);
}

void main()
{
    MinHook::Initialize();
    s_hookInstace.Install(MessageBoxW, MyMessageBoxW);
    s_hookInstace.Enable();
}
```

## MethodHook

```c_cpp
#include "Hook/ProxyHook.hpp"
class TestClass
{
public:
    int num;

    int add(int n)
    {
        return (num += n);
    }
};

class TestHookManager {
private:
    static auto& GetInstace()
    {
        static ProxyHook<decltype(&MyClass::myAdd)> instace;
        return instace;
    }

    class MyClass
    {
    public:
        //unknow
        int myAdd(int n)
        {
            return GetInstace().Invoke(this, 55);
        }
    };
public:
    static void Install()
    {
        GetInstace().Install(&TestClass::add, &MyClass::myAdd);
        GetInstace().Enable();
    }
};


void main()
{
      TestClass a1{};
      TestHookManager::Install();
      int ret = a1.add(100); // return 55
}
```

## ContextHook X64

```c_cpp
#include "Hook/ContextHook.hpp"

void main()
{
      MinHook::Initialize();
      ctxHook.Install(MessageBoxW, [](ThreadContext& ctx) {
      static std::wstring str(reinterpret_cast<wchar_t*>(ctx.RDX.pu));
        str.append(L"Kai");
        ctx.RDX.pu = (unsigned char*)str.data();
        return true;
      });
      ctxHook.Enable();
}
```

## ContextHook X86

```c_cpp
#include "Hook/ContextHook.hpp"

void main()
{
      MinHook::Initialize();
      ctxHook.Install(MessageBoxW, [](ThreadContext& ctx) {
      static std::wstring str(*reinterpret_cast<wchar_t**>(const_cast<uint8_t*>(ctx.ESP.pu + 8)));
        str.append(L"Kai");
        pointer_set(ctx.ESP.dw + 8, str.data());
        return true;
      });
      ctxHook.Enable();
}
```
