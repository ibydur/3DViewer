#include "../include/Scene.h"

void Scene::addObjectOnScene(const std::shared_ptr<SceneObject>& obj)
{
	sceneObjectsLst.push_back(obj);
}

bool Scene::isEmpty()
{
	if (sceneObjectsLst.empty())
		return true;
	return false;
}
