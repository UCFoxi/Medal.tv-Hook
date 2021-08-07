# Medal.tv-Hook
Medal.tv Hook / D3D Present &amp; ResizeBuffers Hook

[URL="https://medal.tv/"]Medal.tv[/URL] is a program to record small gameplay clips. I think it use something from OSB, because of this xd
[IMG]https://i.imgur.com/sN7qxGf.png[/IMG] 
But the Signature is not the same [url]https://i.imgur.com/VbnYj2r.png[/url]

medal-hook64.dll:

ResizeBuffersHook
[IMG]https://i.imgur.com/ZDP9aYP.png[/IMG]
PresentHook
[IMG]https://i.imgur.com/tsOflGy.png[/IMG]

[CODE]
PresentHookPtr: FF 15 ? ? ? ? 33 D2 48 8D 0D ? ? ? ? 8B F8 E8 ? ? ? ? 48 8B 0D ? ? ? ? 83 B9 ? ? ? ? ? 74 08 
const auto PresentPtr = PVOID(pPresentCall + *(uint32_t*)(pPresentCall + 2) + 6);
ResizeBuffersHookPtr: FF 15 ? ? ? ? 33 D2 48 8D 0D ? ? ? ? 8B D8 E8 ? ? ? ? 48 8B 6C 24 ? 8B C3 48 8B 5C 24 ? 48 8B 74 24 ? C6 05 ? ? ? ? ?
const auto ResizeBuffersPtr = PVOID(pResizeBuffersCall + *(uint32_t*)(pResizeBuffersCall + 2) + 6);

//Just use _InterlockedExchangePointer
[/CODE] 

Works well:
[IMG]https://i.imgur.com/JgvScvB.jpeg[/IMG]
