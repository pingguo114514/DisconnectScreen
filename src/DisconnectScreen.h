#pragma once

#include "ll/api/mod/NativeMod.h"

namespace disconnect_screen {

class DisconnectScreen {

public:
    static DisconnectScreen& getInstance();

    DisconnectScreen(ll::mod::NativeMod& self) : mSelf(self) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    /// @return True if the mod is loaded successfully.
    bool load();

    /// @return True if the mod is enabled successfully.
    bool enable();

    /// @return True if the mod is disabled successfully.
    bool disable();

    /// @return True if the mod is unloaded successfully.
    bool unload();

private:
    ll::mod::NativeMod& mSelf;
};

} // namespace disconnect_screen
