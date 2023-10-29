#pragma once
#include <QListWidget>

#include "../include/SceneObject.h"

class Scene : public QObject {
	Q_OBJECT
	class MaterialProperties;
public:
	static constexpr auto TRANSLATION_SPEED = 0.02f;
	static constexpr auto ANGLE_ROTATION_SCALE = 7.0f;

	Scene();
	~Scene();

signals:
	void verticesUpdated(QString) const;
	void edgesUpdated(QString) const;
	void facesUpdated(QString) const;
	void IdUpdated(QString) const;
	void widthUpdated(QString) const;
	void lengthUpdated(QString) const;
	void heightUpdated(QString) const;
	void nameUpdated(QString) const;

public slots:
	void addObjectOnScene(const std::shared_ptr<SceneObject>&);
	void handleSceneItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void removeCurrentObjSelection();
	void setCurrentObjVisibility(int state);
	void setCurrentMaterial(const QString& str);

public:
	inline QVector<std::shared_ptr<SceneObject>> getObjectsLst() const { return this->m_sceneObjectsLst; };
	inline std::shared_ptr<SceneObject> getCurrentObjSelection() const { return this->m_currentSelection; };
	inline MaterialProperties getCurrentMaterial()				 const { return this->m_currentMaterial; }
	void updateObjDetails(const std::shared_ptr<SceneObject>& obj) const;
	std::shared_ptr<SceneObject> getObjectByID(unsigned int id) const;

private:
	void createMaterials();

	struct MaterialProperties {
		MaterialProperties(
			const QString& name,
			const QVector3D& color,
			float ambientStrength,
			float specularStrength,
			float shininess) :
			name{ name },
			objectColor{ color },
			ambientStrength{ ambientStrength },
			specularStrength{ specularStrength },
			shininess{ shininess }
		{
		}
		QString name;
		QVector3D objectColor;
		float ambientStrength;
		float specularStrength;
		float shininess;

		MaterialProperties() = default;
	};

	QVector<std::shared_ptr<SceneObject>> m_sceneObjectsLst;
	QVector<MaterialProperties> m_sceneMaterialsLst;
	std::shared_ptr<SceneObject> m_currentSelection;
	MaterialProperties m_currentMaterial;
};