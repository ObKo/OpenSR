/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPENSR_WORLD_WORLDOBJECT_H
#define OPENSR_WORLD_WORLDOBJECT_H

#include "World.h"

#include <QObject>
#include <QQmlListProperty>

class QJSEngine;

namespace OpenSR
{
namespace World
{
class OPENSR_WORLD_API WorldObject: public QObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

    Q_PROPERTY(quint32 id READ id STORED false)
    Q_PROPERTY(quint32 typeId READ typeId STORED false)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString namePrefix READ namePrefix STORED false)
    //TODO: Use WorldObject instead of QObject
    Q_PROPERTY(QQmlListProperty<QObject> children READ getChildren STORED false)

public: static const quint32 m_StationKindStaticTypeId;
public:
    Q_INVOKABLE WorldObject(WorldObject *parent = 0, quint32 id = 0);
    virtual ~WorldObject();

    quint32 id() const;
    virtual quint32 typeId() const;
    QString name() const;
    virtual QString namePrefix() const;

    QQmlListProperty<QObject> getChildren();

    void setName(const QString& name);

    /*! This function called before world saving */
    virtual void prepareSave();
    /*! This function called during world save after all storable properties saved */
    virtual bool save(QDataStream &stream) const;
    /*! This function called during world load after all storable properties loaded */
    virtual bool load(QDataStream &stream, const QMap<quint32, WorldObject*>& objects);

    /*! This function is called when new turn started */
    virtual void startTurn();
    /*! Process turn */
    virtual void processTurn(float time);
    /*! This function is called when turn finished */
    virtual void finishTurn();

    static quint32 typeIdFromClassName(const QString& className);

    /*!
     * @brief Function to register class in meta & QML systems.
     * @note All subclasses should provide specialized version of this function.
     */
    template<class T>
    static void registerType(QQmlEngine *qml, QJSEngine *script);
    /*!
     * @brief Function to create object of class T.
     * Used in object factory for JavaScript & QML.
     * @note All subclasses should provide specialized version of this function.
     */
    template<class T>
    static T* createObject(WorldObject* parent = 0, quint32 id = 0);
    /*!
     * @brief Function to get type id
     * Used in world serialization.
     * @note All subclasses should provide specialized version of this function.
     */
    template<class T>
    static quint32 staticTypeId();
    /*!
     * @brief Function to get type meta object
     * Used in world serialization.
     * @note All subclasses should provide specialized version of this function.
     */
    template<class T>
    static const QMetaObject* staticTypeMeta();

Q_SIGNALS:
    void nameChanged();

private:
    quint32 m_id;
    QString m_name;
};
}
}

#endif // OPENSR_WORLD_WORLDOBJECT_H
