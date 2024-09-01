#include "DisconnectScreen.h"
#include "nlohmann/json.hpp"

#include <memory>

#include <ll/api/Logger.h>
#include <ll/api/memory/Hook.h>
#include <ll/api/mod/RegisterHelper.h>
#include <ll/api/utils/StringUtils.h>
#include <mc/common/SharedConstants.h>
#include <mc/network/ServerNetworkHandler.h>


ll::Logger logger("DisconnectScreen");

std::string trans(std::string reason) {
    try {
        nlohmann::json json;
        std::ifstream  jsonfile("plugins/DisconnectScreen/Language.json");
        if (jsonfile) {
            jsonfile >> json;
            jsonfile.close();
            return json[reason].get<std::string>();
        } else {
            return "";
        }
    } catch (std::exception& ex) {
        return "";
    } catch (...) {
        return "";
    }
}
LL_INSTANCE_HOOK(
    disconnectClientHook,
    ll::memory::HookPriority::Normal,
    "?disconnectClient@ServerNetworkHandler@@QEAAXAEBVNetworkIdentifier@@W4SubClientId@@W4DisconnectFailReason@"
    "Connection@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_N@Z",
    void,
    NetworkIdentifier*                 a1,
    int                                a2,
    ::Connection::DisconnectFailReason a3,
    std::string                        a4,
    bool                               a5
) {
    if (a4 == "disconnectionScreen.disconnected") {
        return origin(a1, a2, a3, a4, a5);
    }
    std::string str = trans(a4);
    if (str == "") {
        return origin(a1, a2, a3, a4, a5);
    } else {
        return origin(a1, a2, a3, str, a5);
    }
}
namespace disconnect_screen {

static std::unique_ptr<DisconnectScreen> instance;

DisconnectScreen& DisconnectScreen::getInstance() { return *instance; }

bool DisconnectScreen::load() {
    logger.debug("Loading...");
    // Code for loading the mod goes here.
    if (!std::filesystem::exists("./plugins/DisconnectScreen")) {
        std::filesystem::create_directories("plugins/DisconnectScreen");
    }
    if (!std::filesystem::exists("./plugins/DisconnectScreen/Language.json")) {
        std::ofstream DefaultFile("./plugins/DisconnectScreen/Language.json");
        logger.warn("语言文件不存在，正在自动创建！");
        if (!DefaultFile.is_open()) {
            logger.error("语言文件创建失败！");
            return false;
        }
        nlohmann::json json;
        json["disconnectionScreen.editionMismatchEduToVanilla"] =
            "连接失败！\n你使用的是教育版客户端。\n请使用正常基岩版客户端。";
        json["disconnectionScreen.editionMismatchVanillaToEdu"] = "连接失败！\n请使用教育版服务端加入服务器。";
        json["disconnectionScreen.invalidSkin"]         = "连接失败！\n无效的皮肤！";
        json["disconnectionScreen.invalidTenant"]       = "无法连接。非法的访客！";
        json["disconnectionScreen.multiplayerDisabled"] = "连接失败！\n此世界已禁用多人游戏。";
        json["disconnectionScreen.notAllowed"] = "你没有服务器白名单！\n请先添加QQ群申请白名单\n服务器QQ群号： 114514";
        json["disconnectionScreen.notAuthenticated"] =
            "你没有通过 Microsoft 身份验证。\n请尝试重启游戏或更换网络环境。";
        json["disconnectionScreen.serverIdConflict"] = "无法加入服务器！\n您的账户目前正在另一个设备上游玩此服务器。";
        DefaultFile << json.dump(4);
        DefaultFile.close();
    }
    ll::memory::HookRegistrar<disconnectClientHook>().hook();
    return true;
}

bool DisconnectScreen::enable() {
    logger.debug("Enabling...");
    // Code for enabling the mod goes here.
    return true;
}

bool DisconnectScreen::disable() {
    logger.debug("Disabling...");
    // Code for disabling the mod goes here.
    return true;
}
bool DisconnectScreen::unload() {
    logger.debug("Unloading...");
    // Code for unloading the mod goes here.
    ll::memory::HookRegistrar<disconnectClientHook>().unhook();
    return true;
}

} // namespace disconnect_screen

LL_REGISTER_MOD(disconnect_screen::DisconnectScreen, disconnect_screen::instance);
