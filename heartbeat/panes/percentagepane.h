/****************************************
 *
 *   theHeartbeat - System Monitor
 *   Copyright (C) 2019 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#ifndef PERCENTAGEPANE_H
#define PERCENTAGEPANE_H

#include <QWidget>
#include"sidepane.h"

namespace Ui {
    class PercentagePane;
}

class PercentagePane : public SidePane
{
        Q_OBJECT

    public:
        explicit PercentagePane(QWidget *parent = nullptr);
        ~PercentagePane();

        void setTitle(QString title);
        void setPercentage(double percentage);
        void setExpanded(bool expanded);
        void setExpandable(bool expandable);

    signals:
        void toggleExpand();

    private slots:
        void on_expandButton_clicked();

    private:
        Ui::PercentagePane *ui;
};

#endif // PERCENTAGEPANE_H
