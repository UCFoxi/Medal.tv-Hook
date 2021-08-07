#include <Windows.h>
#include <Psapi.h>
#include <cstdint>

__forceinline bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
	{
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	}
	return (*szMask) == NULL;
}

__forceinline const uint64_t FindPattern(const char* szModule, const char* bMask, const char* szMask)
{
	MODULEINFO mi = { 0 };
	(K32GetModuleInformation)((GetCurrentProcess)(), (GetModuleHandleA)(szModule), (LPMODULEINFO)&mi, (DWORD)sizeof(mi));

	uint64_t dwBaseAddress = uint64_t(mi.lpBaseOfDll);
	const auto dwModuleSize = mi.SizeOfImage;
	if (!dwBaseAddress || !dwModuleSize)
		return 0;
	for (auto i = 0ul; i < dwModuleSize; i++)
	{
		if (bDataCompare(PBYTE(dwBaseAddress + i), (const BYTE*)bMask, szMask))
			return uint64_t(dwBaseAddress + i);
	}
	return 0;
}

uint64_t(__fastcall* Present_Original)(uint64_t a1, uint32_t a2, uint32_t a3);
uint64_t __fastcall Present_Hook(uint64_t a1, uint32_t a2, uint32_t a3) {
	static bool once = false;
	if (!once) {
		MessageBoxA(0, "Called Present_Hook", "Info", 0);
		once = true;
	}
	return Present_Original(a1, a2, a3);
}

uint64_t(__fastcall* ResizeBuffers_Original)(uint64_t a1, uint32_t a2, uint32_t a3, uint32_t a4, int a5, int a6);
uint64_t __fastcall ResizeBuffers_Hook(uint64_t a1, uint32_t a2, uint32_t a3, uint32_t a4, int a5, int a6) {
	static bool once = false;
	if (!once) {
		MessageBoxA(0, "Called ResizeBuffers_Hook", "Info", 0);
		once = true;
	}
	return ResizeBuffers_Original(a1, a2, a3, a4, a5, a6);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
		const auto pPresentCall = FindPattern("medal-hook64.dll",
			"\xFF\x15\x00\x00\x00\x00\x33\xD2\x48\x8D\x0D\x00\x00\x00\x00\x8B\xF8\xE8\x00\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\x83\xB9\x00\x00\x00\x00\x00\x74\x08",
			"xx????xxxxx????xxx????xxx????xx?????xx");
		if (pPresentCall) {
			const auto PresentPtr = PVOID(pPresentCall + *(uint32_t*)(pPresentCall + 2) + 6);
			*(PVOID*)&Present_Original = _InterlockedExchangePointer((PVOID*)PresentPtr, &Present_Hook);
		}

		const auto pResizeBuffersCall = FindPattern("medal-hook64.dll",
			"\xFF\x15\x00\x00\x00\x00\x33\xD2\x48\x8D\x0D\x00\x00\x00\x00\x8B\xD8\xE8\x00\x00\x00\x00\x48\x8B\x6C\x24\x00\x8B\xC3\x48\x8B\x5C\x24\x00\x48\x8B\x74\x24\x00\xC6\x05\x00\x00\x00\x00\x00",
			"xx????xxxxx????xxx????xxxx?xxxxxx?xxxx?xx?????");
		if (pResizeBuffersCall) {
			const auto ResizeBuffersPtr = PVOID(pResizeBuffersCall + *(uint32_t*)(pResizeBuffersCall + 2) + 6);
			*(PVOID*)&Present_Original = _InterlockedExchangePointer((PVOID*)ResizeBuffersPtr, &Present_Hook);
		}
    }
    return TRUE;
}

