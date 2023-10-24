#pragma once
#include <QListWidget>

#include "../include/SceneObject.h"

class Scene : public QObject{
	Q_OBJECT
public:
	Scene() = default;
	~Scene() = default;
public slots:
	void addObjectOnScene(const std::shared_ptr<SceneObject>&);
	void handleSceneItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
public:
	QVector<std::shared_ptr<SceneObject>> getObjectsLst() const { return this->m_sceneObjectsLst; };
	QVector<unsigned int> getActiveObjects();
	std::shared_ptr<SceneObject> getObjectByID(unsigned int id) const;
	inline std::shared_ptr<SceneObject> getCurrentObjSelection() const { return this->m_currentSelection; };
private:
	QVector<std::shared_ptr<SceneObject>> m_sceneObjectsLst;
	std::shared_ptr<SceneObject> m_currentSelection;
};