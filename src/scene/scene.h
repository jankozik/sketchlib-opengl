#pragma once

#include <map>
#include <string>
#include "components/tag.h"
#include "scene/entity.h"

namespace scene {

    class Scene {
      private:
        entt::registry registry;
        std::map<entt::entity, std::string> directory;
        friend class Renderer;

      protected:
        Entity CreateEntity(const std::string& name, components::ETag tag = components::ETag::Untagged);
        void DestroyEntity(Entity entity);

      public:
        std::string title;

        explicit Scene(const std::string& title);
        virtual ~Scene();

        // these virtual functions should be overridden by each derived class
        // here in the base class, they are used to render the welcome screen
        virtual void Init(void);
        virtual void OnSceneRender(void);
        virtual void OnImGuiRender(void);
    };

    namespace world {
        // world space constants (OpenGL adopts a right-handed coordinate system)
        constexpr glm::vec3 origin { 0.0f };
        constexpr glm::vec3 zero { 0.0f };
        constexpr glm::vec3 unit { 1.0f };
        constexpr glm::mat4 eye { 1.0f };
        constexpr glm::vec3 up { 0.0f, 1.0f, 0.0f };
        constexpr glm::vec3 forward { 0.0f, 0.0f, 1.0f };
        constexpr glm::vec3 right { -1.0f, 0.0f, 0.0f };
    }

    namespace color {
        constexpr glm::vec3 white { 1.0f };
        constexpr glm::vec3 black { 0.0f };
        constexpr glm::vec3 red { 1.0f, 0.0f, 0.0f };
        constexpr glm::vec3 green { 0.0f, 1.0f, 0.0f };
        constexpr glm::vec3 blue { 0.0f, 0.0f, 1.0f };
        constexpr glm::vec3 cyan { 0.0f, 1.0f, 1.0f };
        constexpr glm::vec3 yellow { 1.0f, 1.0f, 0.0f };
        constexpr glm::vec3 purple { 0.5f, 0.0f, 1.0f };
    }

}
