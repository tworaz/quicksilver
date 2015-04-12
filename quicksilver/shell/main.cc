// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <QGuiApplication>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QTouchDevice>
#include <Init.h>

#if defined(OS_SAILFISH)
#include <QQuickView>
#include <sailfishapp.h>
#endif

int main(int argc, char* argv[]) {
  Q_INIT_RESOURCE(quicksilver_shell);

  QVariant url;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      url = QString(argv[i]);
      break;
    }
  }

#if defined(OS_SAILFISH)
  QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
  QScopedPointer<QQuickView> view(SailfishApp::createView());

  if (!QuickSilver::Initialize(argc, argv)) {
    qCritical() << "Failed to initialize QuickSilver!";
    return 1;
  }

  view->rootContext()->setContextProperty("InitialURL", url);
  view->setSource(QUrl("qrc:/quicksilver_shell_sf.qml"));
  view->showFullScreen();

  return app->exec();
#else
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  QQmlComponent component(&engine);

  qDebug() << "Number of touch devices: " << QTouchDevice::devices().size();
  Q_FOREACH (const QTouchDevice* d, QTouchDevice::devices()) {
    qDebug() << "Device: " << d->name();
  }

  engine.rootContext()->setContextProperty("InitialURL", url);

  if (!QuickSilver::Initialize(argc, argv)) {
    qCritical() << "Failed to initialize QuickSilver!";
    return 1;
  }

  component.loadUrl(QUrl("qrc:/quicksilver_shell.qml"));
  if (!component.isReady()) {
    qCritical() << "Failed to load QML component: "
                << qPrintable(component.errorString());
    return 1;
  }
  component.create();

  return app.exec();
#endif
};

