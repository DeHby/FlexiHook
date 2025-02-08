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