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


/**
 * @file
 * @brief The MainWindow base class that's shared between QtWidgets and QtQuick stack
 *
 * @author Sérgio Martins \<sergio.martins@kdab.com\>
 */

#ifndef KD_MAINWINDOW_BASE_H
#define KD_MAINWINDOW_BASE_H

#include "docks_export.h"
#include "KDDockWidgets.h"
#include "QWidgetAdapter.h"
#include "LayoutSaver_p.h"

#include <QVector>

namespace KDDockWidgets {

class DockWidgetBase;
class Frame;
class DropArea;
class MultiSplitterLayout;
class DropAreaWithCentralFrame;

class DOCKS_EXPORT MainWindowBase : public QMainWindowOrQuick
{
    Q_OBJECT
public:
    typedef QVector<MainWindowBase*> List;
    explicit MainWindowBase(const QString &uniqueName, MainWindowOptions options = MainWindowOption_HasCentralFrame,
                            QWidgetOrQuick *parent = nullptr, Qt::WindowFlags flags = {});

    ~MainWindowBase() override;
    void addDockWidgetAsTab(DockWidgetBase *);
    void addDockWidget(DockWidgetBase *, KDDockWidgets::Location, DockWidgetBase *relativeTo = nullptr, AddingOption = {});

    QString uniqueName() const;
    MainWindowOptions options() const;

    ///@internal
    ///@brief returns the drop area.
    virtual DropAreaWithCentralFrame *dropArea() const = 0;

    ///@internal
    ///@brief returns the MultiSplitterLayout.
    MultiSplitterLayout* multiSplitterLayout() const;

protected:
    void setUniqueName(const QString &uniqueName);

Q_SIGNALS:
    void uniqueNameChanged();

private:
    class Private;
    Private *const d;

    friend class LayoutSaver;
    bool deserialize(const LayoutSaver::MainWindow &);
    LayoutSaver::MainWindow serialize() const;
};

}

#endif
