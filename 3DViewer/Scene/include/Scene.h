#pragma once

#include "../include/SceneObject.h"

class Scene {
public:
	Scene() = default;
	~Scene() = default;

	void addObjectOnScene(const std::shared_ptr<SceneObject>& obj);
	QVector<std::shared_ptr<SceneObject>> getObjectsLst() const { return this->sceneObjectsLst; };
	bool isEmpty();
private:
	QVector<std::shared_ptr<SceneObject>> sceneObjectsLst;
};