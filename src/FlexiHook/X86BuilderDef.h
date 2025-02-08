#ifndef __X86_BUILDER_DEF_HEADER__
#define __X86_BUILDER_DEF_HEADER__

#include <cstdint>
#include <wtypes.h>

struct ThreadContext
{
	union Register {
		DWORD_PTR dw;
		void* pv;
		uint8_t* pu;
	};
	union ObserverRegister {
		const DWORD_PTR dw;
		const void* pv;
		const uint8_t* pu;
	};

	DWORD EFlags;

	Register EDI;
	Register ESI;
	Register EBP;
	ObserverRegister ESP;
	Register EBX;
	Register EDX;
	Register ECX;
	Register EAX;
};


static constexpr size_t OFFSET_SELF = 25;
static constexpr size_t OFFSET_HANDLE = 30;
static constexpr size_t OFFSET_ORIGINAL = 53;

static const std::uint8_t BYTE_SHELLCODE[] = {
  0x60, 					   // pushad
  0x9C,						   // pushfd
  0x83, 0xEC, 0x28, 		   // sub esp, 40
  0xB9, 0x09, 0x00, 0x00, 0x00,// mov ecx, 9
  0x8D, 0x74, 0x24, 0x28, 	   // lea esi, [esp+40]
  0x8D, 0x3C, 0x24, 		   // lea edi, [esp]
  0xFC,						   // cld
  0xF3, 0xA5, 				   // rep movsd
  0x8D, 0x04, 0x24, 		   // lea eax, [esp]
  0x50, 					   // push eax
  0xB9, 0xFF, 0xFF, 0xFF, 0xFF,// mov ecx, $this
  0xB8, 0xFF, 0xFF, 0xFF, 0xFF,// mov eax, $handle
  0xFF, 0xD0,				   // call eax
  0x3C, 0x01, 				   // cmp al, 1
  0x75, 0x07, 				   // short jne $exit
  0x9D, 					   // popfd
  0x61, 					   // popad
  0x83, 0xC4, 0x28, 		   // add esp, 40
  0xEB, 0x05, 				   // short jmp $original
  // $exit:
  0x83, 0xC4, 0x28, 		   // add esp, 40
  0x9D, 					   // popfd
  0x61,						   // popad
  // $original:
  0x68, 0xFF, 0xFF, 0xFF, 0xFF,// push $original
  0xC3						   // ret
};

#endif