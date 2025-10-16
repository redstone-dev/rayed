#include <unordered_map>
#include <filesystem>
#include <string>
#include <raylib.h>
#include <vector>
#include <lua.hpp>

#ifndef CURSOR_STATE
#include "CursorState.cpp"
#endif

#ifndef CODE_EDITOR
#include "CodeEditor.cpp"
#endif

namespace PluginManager {

    class PluginManager {
    public:
        static std::unordered_map<PluginAPI::Plugin, bool> plugins;
        // static std::unordered_map<std::filesystem::path, bool> grammars;

        static void init() {

        }

        static void find_plugins() {
            using namespace std::filesystem;
            path plugin_dir = current_path().append("plugins");
            for (const auto& file : directory_iterator(plugin_dir)) {
                if (file.is_directory()) {
                    // check if directory contains a `plugin.cpp`
                    for (const auto& file : directory_iterator(plugin_dir)) {
                        
                    }
                }
            }
        }
    };

    class PluginManagerWindow {
    private:

    public:
        void update() {

        }
        void draw() {
            
        }
    };
}

namespace PluginAPI {
    class Plugin {
    private:
        CodeEditor* editor;
    public:
        std::string name;
        std::string author;

        Plugin() {};
        ~Plugin() {};
    };

    class UIPlugin : Plugin {
    public:
        void update() {};
        void draw()   {};
    };
}