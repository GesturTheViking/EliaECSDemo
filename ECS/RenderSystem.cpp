#include "RenderSystem.h"

extern "C"
{
#include "../raylib/raylib.h"
}

void Systems::Render(
    ComponentList<TransformComponent>* someTransformComps,
    ComponentList<ModelComponent>* someModelComps)
{
    ModelComponent* modelList = someModelComps->GetDenseComponents();

    const uint32_t count = someModelComps->GetSize();
    for (uint32_t compIndex = 0U; compIndex < count; ++compIndex)
    {
        const TransformComponent& trs = someTransformComps->GetComponent(someModelComps->GetEntityFromComponent(compIndex));
        const ModelComponent& model = modelList[compIndex];

        DrawModel(*ModelManager::GetModel(model.myModel), trs.myPosition, model.myScale, model.myColor);
    }
}