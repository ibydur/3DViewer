#include "../include/Scene.h"

void Scene::addObjectOnScene(const std::shared_ptr<SceneObject>& obj)
{
	m_sceneObjectsLst.push_back(obj);
}

QVector<unsigned int> Scene::getActiveObjects()
{
	QVector<unsigned int> result;
	for (const auto& obj : m_sceneObjectsLst) {
		if (obj->isActive()) {
			result.push_back(obj->getID());
		}
	}
	return result;
}
