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

#include <OpenSR/libRangerQt.h>

#include "OpenSR/SoundManager.h"
#include "OpenSR/ResourceManager.h"
#include "OpenSR/Sound.h"
#include "OpenSR/PluginInterface.h"
#include "OpenSR/DATTranslator.h"

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
#include <QPluginLoader>
#include <QString>
#include <QJSEngine>

namespace OpenSR
{
namespace
{
static const QString SETTINGS_ORGANIZATION = "OpenSR";
static const QString SETTINGS_APPLICATION = "OpenSR";

void mergeMap(QVariantMap &source, const QVariantMap &append)
{
    auto end = append.end();
    for (auto i = append.begin(); i != end; ++i)
    {
        auto target = source.find(i.key());
        if (target == source.end())
            source.insert(i.key(), i.value());
        else
            *target = append;
    }
}
}
Engine::Engine(int argc, char** argv): QApplication(argc, argv)
{
    QApplication::setOrganizationName(SETTINGS_ORGANIZATION);
    QApplication::setApplicationName(SETTINGS_APPLICATION);

    installTranslator(new DATTranslator(this));

    m_sound = new SoundManager(this);
    m_resources = new ResourceManager(this);

    m_qmlView = new QQuickView();
    m_qmlView->setResizeMode(QQuickView::SizeRootObjectToView);
    m_qmlEngine = m_qmlView->engine();
    m_qmlEngine->addImportPath(":/");
    //m_qmlEngine->rootContext()->setContextProperty("engine", this);
    //m_qmlEngine->rootContext()->setContextProperty("QM", new QMLQMExporter(this));

    // "QQmlContext does not take ownership of value." they said....
    // QTBUG-18732
    //m_qmlEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);

    m_qmlEngine->setNetworkAccessManagerFactory(m_resources->qmlNAMFactory());

    m_scriptEngine = new QJSEngine(this);
    m_scriptEngine->globalObject().setProperty("engine", m_scriptEngine->newQObject(this));
    m_scriptEngine->globalObject().setProperty("console", m_scriptEngine->newQObject(new JSConsole(this)));
}

Engine::~Engine()
{
    qDebug() << "~Engine";
}

int Engine::run()
{
    QSettings settings;

    m_resources->addFileSystemPath(settings.value("engine/mainDataDir", QDir::current().absolutePath() + "/data").toString());

    QUrl qmlMainUrl(settings.value("engine/mainQML", "qrc:/OpenSR/ScreenLoader.qml").toString());
    m_qmlView->setSource(qmlMainUrl);

    int width = settings.value("graphics/width", 1024).toInt();
    int height = settings.value("graphics/height", 768).toInt();

    m_qmlView->setWidth(width);
    m_qmlView->setHeight(height);

    QString scriptPath = settings.value("engine/startupScript", "res:/startup.qs").toString();
    execScript(scriptPath);

    m_qmlView->show();

    m_sound->start();

    return exec();
}

void Engine::addRCCArchive(const QString& path)
{
    QResource::registerResource(path);
}

void Engine::showQMLComponent(const QString& url)
{
    if (!m_qmlView->rootObject())
        return;

    QMetaObject::invokeMethod(m_qmlView->rootObject(), "changeScreen", Q_ARG(QVariant, QUrl(url)), Q_ARG(QVariant, QVariantMap()));
}

SoundManager* Engine::sound() const
{
    return m_sound;
}

ResourceManager* Engine::resources() const
{
    return m_resources;
}

QQmlEngine* Engine::qmlEngine()
{
    return m_qmlEngine;
}

QJSEngine* Engine::scriptEngine()
{
    return m_scriptEngine;
}

void Engine::addDATFile(const QString& url)
{
    QIODevice *dev = m_resources->getIODevice(QUrl(url));
    if (!dev || !dev->isOpen())
        return;
    QVariantMap dat = loadDAT(dev);
    mergeMap(m_datRoot, dat);
}

QVariantMap Engine::datRoot() const
{
    return m_datRoot;
}

QVariant Engine::datValue(const QString& path) const
{
    QList<QString> pathes = path.split('.', QString::SkipEmptyParts);
    QVariantMap current = m_datRoot;
    QVariant result;

    for (const QString& p : pathes)
    {
        auto it = current.find(p);
        if (it == current.end())
        {
            qWarning() << "No such DAT record: " << path;
            return QVariant();
        }
        result = it.value();
        if (result.type() != QVariant::Map)
            break;
        current = result.toMap();
    }
    return result;
}

void Engine::loadPlugin(const QString& name)
{
    QPluginLoader loader(name);
    if (!loader.load())
    {
        qWarning().noquote() << QString("Cannot load plugin \"%1\": %2").arg(name, loader.errorString());
        return;
    }
    //TODO: Unloading
    PluginInterface *plugin = qobject_cast<PluginInterface*>(loader.instance());
    if (!plugin)
    {
        qWarning().noquote() << QString("Cannot load plugin \"%1\": %2").arg(name, tr("seems not a plugin."));
        loader.unload();
        return;
    }
    if (!plugin->initPlugin(this))
    {
        qWarning().noquote() << QString("Cannot load plugin \"%1\"").arg(name);
        loader.unload();
        return;
    }
    loader.instance()->setParent(this);
}

void Engine::execScript(const QUrl& url)
{
    QIODevice *dev = resources()->getIODevice(url);
    if (!dev)
    {
        qWarning().noquote() << QString("Cannot exec script %1: file not found.").arg(url.toString());
        return;
    }

    QString script = QString::fromUtf8(dev->readAll());
    dev->close();
    delete dev;

    QJSValue result = m_scriptEngine->evaluate(script, url.toString());
    if (result.isError())
    {
        qWarning().noquote() << QString("%1:%2: %3").arg(url.toString(),
                             result.property("lineNumber").toString(), result.toString());
    }
}

JSConsole::JSConsole(QObject *parent) :
    QObject(parent)
{
}

void JSConsole::log(QString msg)
{
    qDebug().noquote() << msg;
}
}

#include "Engine.moc"
