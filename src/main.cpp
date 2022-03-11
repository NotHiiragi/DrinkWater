#include "main.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/LevelSelectionNavigationController.hpp"

#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Vector2.hpp"

#include "TMPro/TextMeshProUGUI.hpp"

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace GlobalNamespace;

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

TMPro::TextMeshProUGUI* text;

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

MAKE_HOOK_MATCH(ResultsViewController_DidActivate, &ResultsViewController::DidActivate, void,
    ResultsViewController* self,
    bool firstActivation,
    bool addedToHierarchy,
    bool screenSystemEnabling
) {
    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    getLogger().info("Level Completed Opened");

    if(firstActivation){
        text = BeatSaberUI::CreateText(self->get_transform(), "Remember to drink water!");

        text->get_transform()->set_localPosition({ 15.0f, -40.0f, 0.0f });
    } else {

    }
}

MAKE_HOOK_MATCH(LevelSelectionNavigationController_DidActivate, &LevelSelectionNavigationController::DidActivate, void,
    LevelSelectionNavigationController* self,
    bool firstActivation,
    bool addedToHierarchy,
    bool screenSystemEnabling
) {
        LevelSelectionNavigationController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

        getLogger().info("Level Selection Opened");

    if(firstActivation){
        text = BeatSaberUI::CreateText(self->get_transform(), "Remember to touch grass!");

        text->get_transform()->set_localPosition({ 53.0f, -40.5f, 0.0f });
    } else {

    }
}
        


// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    LoggerContextObject logger = getLogger().WithContext("load");

    QuestUI::Init();
    getLogger().info("Successfully installed Settings UI!");

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(logger, ResultsViewController_DidActivate);
    INSTALL_HOOK(logger, LevelSelectionNavigationController_DidActivate);
    getLogger().info("Installed all hooks!");
}
