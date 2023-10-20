#pragma once

#include "../include/SceneObject.h"

class Scene {
public:
	Scene() = default;
	~Scene() = default;

	void addObjectOnScene(const std::shared_ptr<SceneObject>& obj);
	QVector<std::shared_ptr<SceneObject>> getObjectsLst() const { return this->m_sceneObjectsLst; };
	QVector<unsigned int> getActiveObjects();
private:
	QVector<std::shared_ptr<SceneObject>> m_sceneObjectsLst;
};