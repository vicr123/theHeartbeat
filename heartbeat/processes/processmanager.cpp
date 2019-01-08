/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
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
#include "processmanager.h"

#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include <QHash>
#include <QDebug>
#include <QProcess>
#include <QTimer>
#include <tpromise.h>
#include <QMutexLocker>
#include "process.h"

struct ProcessManagerPrivate {
    QHash<int, Process*> processes;
    QMutex processesLocker;
};

ProcessManager::ProcessManager(QObject *parent) : QObject(parent) {
    d = new ProcessManagerPrivate();
    checkProcesses();

    QTimer* timer = new QTimer();
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &ProcessManager::checkProcesses);
    timer->start();
}

ProcessManager::~ProcessManager() {
    delete d;
}

void ProcessManager::checkProcesses() {
    new tPromise<void>([=](QString& error) {
        QDir procDir("/proc");
        QStringList procDirs = procDir.entryList(QDir::Dirs);

        QRegularExpression numberMatch("\\d+");
        for (QString process : procDirs) {
            bool isAnInteger;
            int pid = process.toInt(&isAnInteger);
            if (isAnInteger && !d->processes.contains(pid)) {
                Process* p = new Process(pid);
                connect(p, &Process::processGone, [=] {
                    //Process will delete itself
                    QMutexLocker locker(&d->processesLocker);
                    d->processes.remove(pid);
                });

                d->processesLocker.lock();
                d->processes.insert(pid, p);
                d->processesLocker.unlock();

                emit newPid(pid);
            }
        }

        for (int pid : d->processes.keys()) {
            d->processes.value(pid)->performUpdate();
        }
    });
}


Process* ProcessManager::processByPid(int pid) {
    QMutexLocker locker(&d->processesLocker);
    if (d->processes.contains(pid)) {
        return d->processes.value(pid);
    } else {
        return nullptr;
    }
}

QList<int> ProcessManager::availablePids() {
    QMutexLocker locker(&d->processesLocker);
    return d->processes.keys();
}