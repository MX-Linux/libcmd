/**********************************************************************
 *  cmd.h
 **********************************************************************
 * Copyright (C) 2017 MX Authors
 *
 * Authors: Adrian
 *          MX Linux <http://forum.mxlinux.org>
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this package. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/


#ifndef CMD_H
#define CMD_H

#include <QFile>
#include <QFileSystemWatcher>
#include <QProcess>
#include <QTimer>
#include <QTextStream>

#include "cmd_global.h"

class CMDSHARED_EXPORT Cmd: public QObject
{
    Q_OBJECT
public:
    explicit Cmd(QObject *parent = 0);
    ~Cmd();

    bool isRunning() const;
    bool connectFifo(const QString &file_name);
    int getExitCode(bool quiet = false) const;
    int run(const QString &cmd_str, const QStringList &options = QStringList(""), int est_duration = 10); // with optional estimated time of completion
    void disconnectFifo();

    QString getError() const;
    QString getOutput() const;
    QString getOutput(const QString &cmd_str, const QStringList &options = QStringList(""), int est_duration = 10);


signals:
    void finished(int exit_code, QProcess::ExitStatus exit_status);
    void fifoChangeAvailable(const QString &out);
    void errorAvailable(const QString &err);
    void outputAvailable(const QString &out);
    void runTime(int, int); // runtime counter with estimated time
    void started();

public slots:
    bool kill();
    bool pause();
    bool resume();
    bool terminate();
    void writeToProc(const QString &str);
    void writeToFifo(const QString &str);

private slots:
    void fifoChanged();
    void onStdoutAvailable();
    void onStderrAvailable();
    void tick();      // slot called by timer

private:
    int elapsed_time; // elapsed running time
    int est_duration; // estimated completion time
    QFile fifo;       // named pipe used for interprocess communication
    QFileSystemWatcher file_watch;
    QString out, err;
    QString line_out, line_err;
    QTextStream buffer_out, buffer_err;
    QProcess *proc;
    QTimer *timer;

};

#endif // CMD_H
