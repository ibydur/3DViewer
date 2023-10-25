#include "../include/Scene.h"

void Scene::addObjectOnScene(const std::shared_ptr<SceneObject>& obj)
{
	m_sceneObjectsLst.push_back(obj);
	m_currentSelection = obj;
}

void Scene::handleSceneItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
	if (nullptr != current) {
		auto current_id = current->data(Qt::UserRole);
		m_currentSelection = std::move(getObjectByID(current_id.toUInt()));
	}
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

std::shared_ptr<SceneObject> Scene::getObjectByID(unsigned int id) const
{
	for (const auto& obj : m_sceneObjectsLst) {
		if (id == obj->getID()) {
			return obj;
		}
	}
	return nullptr;
}

void Scene::removeCurrentObjSelection()
{
	const auto& current_obj = std::move(getCurrentObjSelection());
	if (nullptr == current_obj) {
		return;
	}
	current_obj->release();
	auto founded_it = std::find(m_sceneObjectsLst.begin(), m_sceneObjectsLst.end(), current_obj);
	if (founded_it != m_sceneObjectsLst.end()) {
		m_sceneObjectsLst.erase(founded_it);
		m_currentSelection.reset();
	}
}
