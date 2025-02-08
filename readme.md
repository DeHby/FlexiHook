# [FlexiHook](https://github.com/DeHby/FlexiHook)

这是一个基于minhook封装的windows hook库，支持普通函数、类方法、虚函数（理论支持）、任意上下文（ContextHook 类似Frida interceptor）Hook方式

ContextHook的方式目前暂不支持修改ESP/RSP寄存器

# 项目开发环境

- C++开发标准:14
- 依赖库:minhook
- 调用约定:默认
***
# 编译项目
你可以根据自己的喜好进行编译,不一定参照本教程,但请一定要更新`submodules`依赖
### 克隆项目与获取依赖模块
`git clone --recurse-submodules https://github.com/DeHby/FlexiHook.git`
### 创建构建目录并进入目录
`mkdir build && cd build`
### 配置与编译
`cmake .. && cmake --build .`
***
# 使用例子
## APIHOOK
```c_cpp
#include "FlexiHook/FlexiHook.hpp"

static ProxyHook<decltype(MessageBoxA)> s_hookInstace;

int WINAPI MyMessageBoxW(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCWSTR lpText,
    _In_opt_ LPCWSTR lpCaption,
    _In_ UINT uType)
{
    return s_hookInstace.Invoke(hWnd, L"测试拦截", lpCaption, uType);
}

int main()
{
    MinHook::Initialize();
    s_hookInstace.Install(MessageBoxW, MyMessageBoxW);
    s_hookInstace.Enable();
    MessageBoxW(0, L"Test", 0, 64);
    return 0;
}
```

## MethodHook

```c_cpp
#include "FlexiHook/FlexiHook.hpp"
#include "FlexiHook/Utils.hpp"

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


int main()
{
      TestClass a1{};
      TestHookManager::Install();
      int ret = a1.add(100); // return 55
      return 0;
}
```

## ContextHook X64

```c_cpp
#include "FlexiHook/FlexiHook.hpp"
#include "FlexiHook/Utils.hpp"

int main()
{
    MinHook::Initialize();
    ContextHook ctxHook;
    ctxHook.Install(MessageBoxW, [](ThreadContext &ctx){
      static std::wstring str(pointer_cast<wchar_t*>(ctx.RDX.pu));
        str.append(L"Kai");
        ctx.RDX.pv = const_cast<wchar_t*>(str.data());
        return true; });
    ctxHook.Enable();
    MessageBoxW(0, L"Test", 0, 64);
    return 0;
}
```

## ContextHook X86

```c_cpp
#include "FlexiHook/FlexiHook.hpp"
#include "FlexiHook/Utils.hpp"

int main()
{
    MinHook::Initialize();

    ContextHook ctxHook;
    ctxHook.Install(MessageBoxW, [](ThreadContext &ctx){
      static std::wstring str(*reinterpret_cast<wchar_t**>(const_cast<uint8_t*>(ctx.ESP.pu + 8)));
        str.append(L"Kai");
        pointer_set(ctx.ESP.dw + 8, str.data());
        return true; });
    ctxHook.Enable();
    MessageBoxW(0, L"Test", 0, 64);
    return 0;
}
```
# 未来计划
- [ ] 使用masm作为组译器
- [ ] 使用RtlCaptureContext作为上下文管理
- [ ] 实现Register类 方便快速读写寄存器 避免额外的转换
