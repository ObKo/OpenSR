/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 - 2017 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_ENGINE_H
#define OPENSR_ENGINE_H

#include <QApplication>
#include <OpenSR/OpenSR.h>

class QQuickView;
class QQmlEngine;
class QJSEngine;

namespace OpenSR
{
class SoundManager;
class ResourceManager;
class ENGINE_API Engine: public QApplication
{
    Q_OBJECT
public:
    Engine(int& argc, char **argv);
    virtual ~Engine();

    int run();

    SoundManager *sound() const;
    ResourceManager *resources() const;

    QQmlEngine *qmlEngine();
    QJSEngine *scriptEngine();

    Q_INVOKABLE QVariantMap datRoot() const;
    Q_INVOKABLE QVariant datValue(const QString& path) const;

    Q_INVOKABLE void execScript(const QUrl& url);

public Q_SLOTS:
    void addRCCArchive(const QString& source);
    void showQMLComponent(const QString& url);
    void addDATFile(const QString& url, bool isCache = false);
    void loadPlugin(const QString& name);

private:
    QQuickView *m_qmlView;
    QQmlEngine *m_qmlEngine;
    QJSEngine *m_scriptEngine;
    SoundManager *m_sound;
    ResourceManager *m_resources;
    QVariantMap m_datRoot;

    Q_DISABLE_COPY(Engine)
};

}

#endif
