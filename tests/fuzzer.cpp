/*
  This file is part of KDDockWidgets.

  Copyright (C) 2018-2019 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Sérgio Martins <sergio.martins@kdab.com>
  Author: Laurent Montel <laurent.montel@kdab.com>

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

#include "DockWidgetBase.h"
#include "MainWindow.h"
#include "DropArea_p.h"
#include "utils.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QTextEdit>

#include <iostream>

#ifdef Q_OS_WIN
# include <Windows.h>
#endif

using namespace KDDockWidgets;
using namespace KDDockWidgets::Tests;

static QtMessageHandler s_original = nullptr;

void fatalWarningsMessageHandler(QtMsgType t, const QMessageLogContext &context, const QString &msg)
{
    if (shouldBlacklistWarning(msg, QLatin1String(context.category)))
        return;

    s_original(t, context, msg);
    if (t == QtWarningMsg) {
        if (!qEnvironmentVariableIsSet("NO_FATAL"))
            qFatal("Got a warning, category=%s", context.category);
    }
}

int main(int argc, char **argv)
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("KDAB"));
    app.setApplicationName(QStringLiteral("Test random app"));

    QCommandLineParser parser;
    parser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("duration"), QStringLiteral("application duration in seconds"), QStringLiteral("second")));
    parser.process(app);
    int duration = -1;
    if (parser.isSet(QStringLiteral("duration"))) {
        duration = parser.value(QStringLiteral("duration")).toInt();
    }

    qDebug() << " Duration " << duration;
    if (duration > 0) {
        //QTimer::singleShot(duration, this, []() {quit();});
    }

    auto window = createMainWindow();
    std::cout << "auto window = createMainWindow();" << std::endl;
    s_original = qInstallMessageHandler(fatalWarningsMessageHandler);

    const int number = 20;
    qsrand(QDateTime::currentMSecsSinceEpoch());
    const int randomValue = (qrand() % number + 1);
    //qDebug() << " randomValue" << randomValue;
    QList<DockWidget *> listDockWidget;
    listDockWidget.reserve(randomValue);
    //Create DockWidget
    std::cout << "QList<DockWidget *> listDockWidget;" << std::endl;
    for (int i = 0; i < randomValue; ++i) {
        const QString name = QStringLiteral("foo-%1").arg(i);
        auto dock = new DockWidget(name);
        dock->setWidget(new QTextEdit(dock));
        listDockWidget.append(dock);
        std::cout << "{" << std::endl;
        std::cout << QString(QStringLiteral("   auto dock = new DockWidget(QStringLiteral(\"%1\"));").arg(name)).toLocal8Bit().constData() << std::endl;
        std::cout << "   dock->setWidget(new QTextEdit(dock));" << std::endl;
        std::cout << "   listDockWidget.append(dock);"<<std::endl;
        std::cout << "}" << std::endl;
    }

    //Add log !
    DropArea *dropArea = window->dropArea();

    std::cout << "auto dropArea = window->dropArea();" << std::endl;

    // Add dockWidget
    for (int i = 0; i < listDockWidget.count(); ++i) {
        //Don't use Location_None position
        const int position = (qrand() % 4 ) + 1;
        if (i == 0) {
            std::cout << QString(QStringLiteral("window->addDockWidget(listDockWidget.at(%1), static_cast<Location>(%2));").arg(i).arg(position)).toLocal8Bit().constData() << std::endl;
            window->addDockWidget(listDockWidget.at(i), static_cast<Location>(position));
        } else {
            const int attachAsTab = (qrand() % 3 ) ;
            if (attachAsTab == 0) {
                std::cout << QString(QStringLiteral("listDockWidget.at(%1 - 1)->addDockWidgetAsTab(listDockWidget.at(%1));").arg(i)).toLocal8Bit().constData() << std::endl;
                listDockWidget.at(i - 1)->addDockWidgetAsTab(listDockWidget.at(i));
            } else if (attachAsTab == 1) {
                std::cout << QString(QStringLiteral("window->addDockWidget(listDockWidget.at(%1), static_cast<Location>(%2));").arg(i).arg(position)).toLocal8Bit().constData() << std::endl;
                window->addDockWidget(listDockWidget.at(i), static_cast<Location>(position));
            } else {
                const int addingOption = (qrand() % 2 );
                std::cout << QString(QStringLiteral("window->addDockWidget(listDockWidget.at(%1-1), static_cast<Location>(%3), listDockWidget.at(%1), static_cast<AddingOption>(%2));").arg(i).arg(addingOption).arg(position)).toLocal8Bit().constData() << std::endl;
                window->addDockWidget(listDockWidget.at(i-1), static_cast<Location>(position), listDockWidget.at(i), static_cast<AddingOption>(addingOption));
            }
        }
        dropArea->checkSanity();
        std::cout << "dropArea->checkSanity();" << std::endl << std::endl;
    }

    window->show();


    return app.exec();
}
