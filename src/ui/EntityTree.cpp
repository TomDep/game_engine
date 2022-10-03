#include "EntityTree.h"

#include <spdlog/spdlog.h>
#include <time.h>

EntityTree::EntityTree(Scene* scene) : scene(scene) {
    base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
}

void EntityTree::render() {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Entity Tree", nullptr, windowFlags);

    std::vector<Entity*> entities = *scene->getEntities();
    for (int i = 0; i < entities.size(); i++) {
        int node_clicked = -1;

        Entity* entity = entities.at(i);
        renderEntityChildren(entity);
        idOpened.clear();
        if(selectedEntity != nullptr) idOpened.push_back(selectedEntity->getId());
    }

	ImGui::End();
}

int EntityTree::renderEntityChildren(Entity* entity) {
    int id = entity->getId();

    ImGuiTreeNodeFlags node_flags = base_flags;
    if (selectedEntity != nullptr && isCurrentIdOpened(id))
        node_flags |= ImGuiTreeNodeFlags_Selected;

    if (entity->hasChildren()) {
        // Node
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, "%s", entity->getName().c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            selectedEntity = entity;

            // Add the id to opened
            if (!isCurrentIdOpened(id)) idOpened.push_back(id);
        }
            
        if (node_open)
        {
            for (Entity* child : entity->getChildren()) {
                renderEntityChildren(child);
            }
            ImGui::TreePop();
        }
    }
    else
    {
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
        ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, "%s", entity->getName().c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            selectedEntity = entity;
    }

    return id;
}

bool EntityTree::isCurrentIdOpened(int currentId) {
    for (int id : idOpened) {
        if (currentId == id) return true;
    }

    return false;
}