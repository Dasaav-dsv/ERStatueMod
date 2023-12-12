#include "hooks.h"

void placeHooks() {
    if (CallHook::initialize()) {
        CallHook::CallMap callMap{};
        (void*)new CallHookTemplate<EntryHook>(GLOBAL_GAMEREPOSITORY.calls.callsGXRend.updateBrightness, onBrightnessUpdate);
        CallHook::hookFunction<EntryHook>(callMap.getCalls((void*)GLOBAL_GAMEREPOSITORY.functions.fnCSChrActionFlagModule.getMaxTurnSpeed), onGetMaxTurnSpeed);
    }
    if ((new RTTIScanner())->scan()) {
        (void*)new VFTHookTemplate<EntryHook>("CS::CSHkBehWorldListener", 8, onHkbChrUpdate);
    }
}