#include "../include/SceneObject.h"
#include "../../Renderer/include/OpenGLRenderer.h"

unsigned int SceneObject::m_idCounter = 0;

void SceneObject::draw(OpenGLRenderer* renderer)
{
    renderer->drawObject(*this);
}

void SceneObject::intializeVBOandVAO(OpenGLRenderer* renderer)
{
    renderer->initializeObjVBOandVAO(*this);
}

void SceneObject::release()
{
    vbo.destroy();
    vao.destroy();
}
