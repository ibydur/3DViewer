#include "../include/Scene.h"

void Scene::addObjectOnScene(const std::shared_ptr<SceneObject>& obj)
{
	m_sceneObjectsLst.push_back(obj);
}

bool Scene::isEmpty()
{
	if (m_sceneObjectsLst.empty())
		return true;
	return false;
}
