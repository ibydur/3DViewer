#include "Scene.h"

void Scene::addObjectOnScene(const std::shared_ptr<SceneObject>& obj)
{
	m_sceneObjectsLst.push_back(obj);
	m_currentSelection = obj;
}

Scene::Scene() : m_sceneObjectsLst{ }, m_currentSelection{ nullptr }
{
	createMaterials();
}

Scene::~Scene()
{
	for (const auto& obj : m_sceneObjectsLst) {
		obj->release();
	}
}

void Scene::handleSceneItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
	if (nullptr != current) {
		auto current_id = current->data(Qt::UserRole);
		m_currentSelection = std::move(getObjectByID(current_id.toUInt()));
        emit updateCamera(m_currentSelection->getBoundingBoxLength());
        emit redrawRenderer();
	}
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

void Scene::createMaterials()
{
	MaterialProperties metal_material = MaterialProperties("Metal", QVector3D(0.6f, 0.6f, 0.6f), 0.2f, 0.5f, 128.0f);
	MaterialProperties  bone_material = MaterialProperties("Skelet", QVector3D(0.9f, 0.85f, 0.7f), 0.1f, 0.1f, 32.0f);
	m_sceneMaterialsLst.push_back(bone_material);
	m_sceneMaterialsLst.push_back(metal_material);

	m_currentMaterial = metal_material;
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

void Scene::setCurrentObjVisibility(int state)
{
	const auto& current_obj = std::move(getCurrentObjSelection());
	if (nullptr == current_obj) {
		return;
	}
	current_obj->setVisible(state);
	emit redrawRenderer();
}

void Scene::setCurrentMaterial(const QString& str)
{
	for (const auto& material : m_sceneMaterialsLst) {
		if (material.name == str) {
			m_currentMaterial = material;
			break;
		}
	}
	emit redrawRenderer();
}

void Scene::updateObjDetails(const std::shared_ptr<SceneObject>& obj) const
{
	if (nullptr != obj) {
		emit nameUpdated(obj->getName());
		emit verticesUpdated(QString::number(obj->getNumberOfVertices()));
		emit facesUpdated(QString::number(obj->getNumberOfFaces()));
		emit edgesUpdated(QString::number(obj->getNumberOfEdges()));
		emit IdUpdated(QString::number(obj->getID()));
		emit widthUpdated(QString::number(obj->getWidth(), 'f', 2) + " cm");
		emit heightUpdated(QString::number(obj->getHeight(), 'f', 2) + " cm");
		emit lengthUpdated(QString::number(obj->getLength(), 'f', 2) + " cm");
	}
	else {
		emit nameUpdated("Unknown");
		emit verticesUpdated("0");
		emit facesUpdated("0");
		emit edgesUpdated("0");
		emit IdUpdated("N/A");
		emit widthUpdated("0.0");
		emit heightUpdated("0.0");
		emit lengthUpdated("0.0");
	}
}