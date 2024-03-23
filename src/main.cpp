// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "app_environment.h"
#include "import_qml_components_plugins.h"
#include "import_qml_plugins.h"
#include "squeezeserver.h"

int main(int argc, char *argv[])
{
    set_qt_environment();

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qt/qml/Main/main.qml"_qs);
    QObject::connect(
                &engine, &QQmlApplicationEngine::objectCreated, &app,
                [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);

    SqueezeServer *squeezeServer = new SqueezeServer(&app, &engine, url, "192.168.1.50");
    qmlRegisterSingletonInstance("com.georgegalt.SqueezeServer", 1, 0, "SqueezeServer", squeezeServer);
    qmlRegisterSingletonInstance("com.georgegalt.Slimp3CLI", 1, 0, "Slimp3Cli", squeezeServer->slimp3CLI());
    qmlRegisterSingletonInstance("com.georgegalt.Slimp3JSON", 1, 0, "Slimp3JSON", squeezeServer->slimp3JSON());
    qmlRegisterSingletonInstance("com.georgegalt.PlayerModel", 1, 0, "PlayerModel", squeezeServer->PlayerModel());
    squeezeServer->Init();

    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    engine.addImportPath(":/");

    // engine.load(url);

    // if (engine.rootObjects().isEmpty()) {
    //     return -1;
    // }

    return app.exec();
}
