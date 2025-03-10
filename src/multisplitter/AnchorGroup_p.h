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

#ifndef KD_MULTISPLITTER_ANCHORGROUP_P_H
#define KD_MULTISPLITTER_ANCHORGROUP_P_H

#include "docks_export.h"
#include "KDDockWidgets.h"
#include "Anchor_p.h"

#include <QDebug>

namespace KDDockWidgets {

class MultiSplitterLayout;
class Anchor;
class Item;

struct DOCKS_EXPORT_FOR_UNIT_TESTS AnchorGroup
{
    ///@brief contructs an invalid group
    AnchorGroup() = default;

    explicit AnchorGroup(MultiSplitterLayout *);

    void addItem(Item *item);
    void addItem(MultiSplitterLayout *);
    void removeItem(Item *item);
    bool isValid() const { return top && left && bottom && right; }

    int width() const;
    int height() const;

    ///@brief returns whether this group contains @p anchor
    bool containsAnchor(Anchor *anchor) const;

    ///@brief returns whether this group contains @p anchor at Side @p side
    ///If side is Side1, then anchor must be equal to left or top, otherwise top or bottom
    bool containsAnchor(Anchor *anchor, Anchor::Side side) const;

    /**
     * @brief Returns the max available size in this group
     * This is the size of the widget when you push all anchors outwards
     */
    QSize availableSize() const;

    /**
     * @brief Returns the size of an item that would be inside these 4 anchors
     */
    QSize itemSize() const;

    /**
     * @brief Similar to @ref itemSize(), but returns the width if @p o is Qt::Vertical, otherwise
     * the height
     */
    int itemSize(Qt::Orientation o) const;

    /**
     * @brief Returns whether @ref availableSize is bigger or equal than @ref needed
     */
    bool hasAvailableSizeFor(QSize needed) const;

    /// Returns the group formed by the Anchors that actually have items on their outter side
    AnchorGroup outterGroup() const;

    Anchor *oppositeAnchor(Anchor*) const;
    Anchor *createAnchorFrom(KDDockWidgets::Location fromAnchorLocation, Item *relativeTo);
    void setAnchor(Anchor *a, Qt::Orientation orientation, Anchor::Side side);

    Anchor *adjacentAnchor(Anchor*) const;

    QPair<Anchor*,Anchor*> adjacentAnchors(Anchor*) const;

    Anchor *anchor(KDDockWidgets::Location) const;
    Anchor *anchorAtDirection(Anchor::Side side, Qt::Orientation orientation) const;

    Anchor *anchorAtSide(Anchor::Side side, Qt::Orientation orientation) const;

    void setAnchor(Anchor *anchor, KDDockWidgets::Location);

    bool anchorIsFollowingInwards(Anchor*) const;
    const Anchor::List anchorsFollowingInwards() const;
    const Anchor::List anchorsNotFollowingInwards() const;
    const Anchor::List anchors() const;

    Anchor::Side sideForAnchor(Anchor*) const;
    bool isStatic() const;
    bool isStaticOrFollowsStatic() const;

    void updateItemSizes();


    Anchor *top = nullptr;
    Anchor *left = nullptr;
    Anchor *bottom = nullptr;
    Anchor *right = nullptr;
    MultiSplitterLayout *layout;

    QDebug debug(QDebug d) const;
};
}

inline QDebug operator<< (QDebug d, KDDockWidgets::AnchorGroup *group)
{
    // out-of-line as it needs to include MultiSplitterLayout
    return group->debug(d);
}

#endif
