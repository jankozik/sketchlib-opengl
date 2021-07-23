// THIS SOURCE FILE IS AUTO GENERATED BY LUA SCRIPT, DO NOT MODIFY.
// THIS IS THE SIMPLE FACTORY PATTERN TO LOAD NEW SCENE AT RUNTIME.

// once you have created a new scene, rebuild the solution on the command line,
// the content of this file will be update automatically to reflect your changes.

#include "pch.h"

#include "core/log.h"
#include "utils/factory.h"

#include "scene_01/scene_01.h"
#include "scene_02/scene_02.h"

namespace factory {

    const std::vector<std::string> titles {
        "Welcome Screen",
        "Colorful Cubes",
        "Skybox Reflection"
    };

    Scene* LoadScene(const std::string& title) {
        if (title == "Welcome Screen") return new Scene(title);
        if (title == "Colorful Cubes") return new Scene01(title);
        if (title == "Skybox Reflection") return new Scene02(title);

        CORE_ERROR("Scene \"{0}\" is not registered in the factory ...", title);
        std::cin.get();
        exit(EXIT_FAILURE);
    }
}
