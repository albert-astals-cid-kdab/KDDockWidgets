/*
  This file is part of KDDockWidgets.

  Copyright (C) 2018-2019 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Sérgio Martins <sergio.martins@kdab.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MainWindow.h"
#include "ExampleDockableWidget.h"
#include "LayoutSaver.h"
#include "DockWidget.h"
#include "Config.h"

#include <QTextEdit>
#include <QMenu>
#include <QAction>
#include <QStyleFactory>
#include <QApplication>
#include <QDebug>
#include <QMenuBar>
#include <QVBoxLayout>

using namespace KDDockWidgets;

DockWidgetBase::Options s_dockWidgetOptions = DockWidgetBase::Option_None; // DockWidget::Option_NotClosable;

static MyWidget *newMyWidget()
{
    static int count = 0;
    count++;
    return new MyWidget();
}


class MyMainWindow : public MainWindow
{
    Q_OBJECT
public:
    MyMainWindow(MainWindowOptions options, QWidget *parent = nullptr)
        : MainWindow(QStringLiteral("MyMainWindow"), options, parent)
    {
        // qApp->installEventFilter(this);

        auto menubar = menuBar();
        auto fileMenu = new QMenu(QStringLiteral("File"));
        toggleMenu = new QMenu(QStringLiteral("Toggle"));
        menubar->addMenu(fileMenu);
        menubar->addMenu(toggleMenu);

        QAction *newAction = fileMenu->addAction(QStringLiteral("New DockWidget"));
        static int count = 0;
        count++;
        connect(newAction, &QAction::triggered, this, [] {
            auto w = newMyWidget();
            w->setGeometry(100, 100, 400, 400);
            auto dock = new DockWidget(QStringLiteral("dock %1").arg(count));
            dock->setWidget(w);
            dock->resize(400, 400);
            dock->show();
        });

        // newAction = fileMenu->addAction("Change MainWindow indicator style");

        auto saveLayoutAction = fileMenu->addAction(QStringLiteral("Save Layout"));
        connect(saveLayoutAction, &QAction::triggered, this, [] {
            LayoutSaver saver;
            const bool result = saver.saveToDisk();
            qDebug() << "Saving layout to disk. Result=" << result;
        });

        auto restoreLayoutAction = fileMenu->addAction(QStringLiteral("Restore Layout"));
        connect(restoreLayoutAction, &QAction::triggered, this, [] {
            LayoutSaver saver;
            saver.restoreFromDisk();
        });
    }

    bool eventFilter(QObject *o, QEvent *ev) override
    {
        if (ev->type() == QEvent::MouseButtonPress ||
            ev->type() == QEvent::MouseButtonRelease ||
            //ev->type() == QEvent::MouseMove ||
            ev->type() == QEvent::NonClientAreaMouseButtonPress ||
            ev->type() == QEvent::NonClientAreaMouseButtonRelease ||
            ev->type() == QEvent::NonClientAreaMouseMove)
            qDebug() << "Mouse event: " << ev->type();

        if (ev->type() == QEvent::Move)
            qDebug() << "Move event " << window()->pos();

        return KDDockWidgets::MainWindow::eventFilter(o, ev);
    }

    QMenu *toggleMenu;
};

int main(int argc, char **argv)
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    //KDDockWidgets::Config::self().setFlags( KDDockWidgets::Config::Flags() | KDDockWidgets::Config::Flag_HideTitleBarWhenTabsVisible | KDDockWidgets::Config::Flag_AlwaysShowTabs);
    //KDDockWidgets::Config::self().setFlags( KDDockWidgets::Config::Flags() | KDDockWidgets::Config::Flag_AlwaysShowTabs);
    app.setOrganizationName(QStringLiteral("KDAB"));
    app.setApplicationName(QStringLiteral("Test app"));
    qApp->setStyle(QStyleFactory::create(QStringLiteral("Fusion")));

    const bool embedded = app.arguments().contains(QStringLiteral("--embedded"));
    const bool noCentralFrame = app.arguments().contains(QStringLiteral("--no-central"));
    MainWindowOptions options = noCentralFrame ? MainWindowOption_None
                                               : MainWindowOption_HasCentralFrame;

    MyMainWindow mainWindow(options);
    QWidget *window;
    if (embedded) {
        window = new QWidget();
        auto lay = new QVBoxLayout(window);
        lay->addWidget(&mainWindow);
    } else {
        window = &mainWindow;
    }

    window->resize(1000, 800);
    window->show();

    auto example = newMyWidget();
    auto dock = new DockWidget(QStringLiteral("foo"), s_dockWidgetOptions);
    dock->setIcon(QIcon::fromTheme(QStringLiteral("mail-message")));
    dock->setWidget(example);
    dock->setTitle(QStringLiteral("foo"));
    example->winId(); // for testing native widgets too
    dock->resize(400, 400);
    dock->show();
    mainWindow.toggleMenu->addAction(dock->toggleAction());

    example = newMyWidget();
    example->setGeometry(100, 100, 400, 400);
    dock = new DockWidget(QStringLiteral("bar"), s_dockWidgetOptions);
    dock->setWidget(example);
    dock->resize(400, 400);
    dock->show();
    mainWindow.toggleMenu->addAction(dock->toggleAction());

    auto textEdit = new QTextEdit();
    textEdit->setText(QStringLiteral("Hello, this is the central document."));
    dock = new DockWidget(QStringLiteral("doc 0"), s_dockWidgetOptions);
    dock->setWidget(textEdit);
    mainWindow.toggleMenu->addAction(dock->toggleAction());
    if (!noCentralFrame)
        mainWindow.addDockWidgetAsTab(dock);

    return app.exec();
}

#include <main.moc>
