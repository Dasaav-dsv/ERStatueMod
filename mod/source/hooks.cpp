#include "hooks.h"

void placeHooks() {
    if (CallHook::initialize()) {
        new CallHookTemplate<EntryHook>(GLOBAL_GAMEREPOSITORY.calls.callsGXRend.updateBrightness, onBrightnessUpdate);
    }
    if ((new RTTIScanner())->scan()) {
        (void*)new VFTHookTemplate<EntryHook>("CS::CSHkBehWorldListener", 8, onHkbChrUpdate);
    }
}