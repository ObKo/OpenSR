/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 - 2015 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/Engine.h"

#include <OpenSR/SoundManager.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Sound.h>

#include <QQuickView>
#include <QBoxLayout>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QSettings>
#include <QResource>
#include <QDebug>
#include <QQuickItem>
#include <QDir>

namespace OpenSR
{
Engine::Engine(int argc, char** argv): QApplication(argc, argv)
{
    m_sound = new SoundManager(this);
    m_resources = new ResourceManager(this);

    m_qmlView = new QQuickView();
    m_qmlEngine = m_qmlView->engine();
    m_qmlEngine->addImportPath(":/");
    m_qmlEngine->rootContext()->setContextProperty("engine", this);

    m_qmlEngine->setNetworkAccessManagerFactory(m_resources->qmlNAMFactory());

    m_scriptEngine = new QJSEngine(this);
    m_scriptEngine->globalObject().setProperty("engine", m_scriptEngine->newQObject(this));
    m_scriptEngine->globalObject().setProperty("resources", m_scriptEngine->newQObject(m_resources));
}

Engine::~Engine()
{
}

int Engine::run()
{
    QSettings settings;

    m_resources->addFileSystemPath(settings.value("engine/mainDataDir", QDir::current().absolutePath() + "/data").toString());

    QUrl qmlMainUrl(settings.value("engine/mainQML", "qrc:/OpenSR/ScreenLoader.qml").toString());
    m_qmlView->setSource(qmlMainUrl);

    QString scriptPath = settings.value("engine/startupScript", "data/startup.qs").toString();
    QFile sf(scriptPath);
    sf.open(QIODevice::ReadOnly);
    QString script = QString::fromUtf8(sf.readAll());
    sf.close();
    m_scriptEngine->evaluate(script, scriptPath);

    int width = settings.value("graphics/width", 1024).toInt();
    int height = settings.value("graphics/height", 768).toInt();

    m_qmlView->setWidth(width);
    m_qmlView->setHeight(height);

    m_qmlView->show();

    m_sound->start();

    return exec();
}

void Engine::addRCCArchive(const QString& path)
{
    QResource::registerResource(path);
}

void Engine::showQMLComponent(const QUrl& source)
{
    if (!m_qmlView->rootObject())
        return;

    QMetaObject::invokeMethod(m_qmlView->rootObject(), "changeScreen", Q_ARG(QVariant, source));
}

SoundManager* Engine::sound() const
{
    return m_sound;
}

ResourceManager* Engine::resources() const
{
    return m_resources;
}
}
