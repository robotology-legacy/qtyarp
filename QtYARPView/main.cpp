/*
 * Copyright (C) 2009 RobotCub Consortium, European Commission FP6 Project IST-004370
 * Author: Davide Perrone
 * Date: Feb 2014
 * email:   dperrone@aitek.it
 * website: www.aitek.it
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "config.h"

#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QtWidgets/QApplication>
#include <QQmlContext>
#include <QVariant>
#include <QDir>

/*! \brief Main method for the YARPView container.
 *
 *  \param argc
 *  \param argv
 */
int main(int argc, char *argv[])
{
    // Pack the argc and argv to a QStringList so we can pass them easily to
    // the plugin.
    // This list must be packed before creating the QApplication, because
    // QApplication will remove the known arguments, and this includes the
    // "--name <name>" argument on linux.
    QStringList params;
    for(int i=1;i<argc;i++){
        params.append(argv[i]);
    }

    QApplication app(argc, argv);
    QVariant retVal;

    // De-comment this to trace all imports
    /*QByteArray data = "1";
    qputenv("QML_IMPORT_TRACE", data);*/


    QQmlApplicationEngine engine;
#ifdef WIN32
    engine.addImportPath(QDir::cleanPath(QCoreApplication::applicationDirPath() + "\\" + PLUGINS_RELATIVE_PATH));
#else
    engine.addImportPath(QDir::cleanPath(QCoreApplication::applicationDirPath() + "/" + PLUGINS_RELATIVE_PATH));
#endif
    engine.load(QUrl("qrc:/qml/QtYARPView/main.qml"));
    QObject *topLevel = engine.rootObjects().value(0);

    if(!topLevel){
        return 0;
    }

    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    // Call the parseParameters of the qml object called YARPVideoSurface
    QObject *yarpVideoSurface = topLevel->findChild<QObject*>("YARPVideoSurface");
    QMetaObject::invokeMethod(yarpVideoSurface,"parseParameters",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(QVariant, retVal),
                              Q_ARG(QVariant,params));
    if(!retVal.toBool()){
        return 0;
    }


    window->show();

    return app.exec();

}

