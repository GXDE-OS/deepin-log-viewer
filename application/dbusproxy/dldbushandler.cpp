// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dldbushandler.h"
#include <QDebug>
#include <QStandardPaths>

DLDBusHandler *DLDBusHandler::m_statichandeler = nullptr;

DLDBusHandler *DLDBusHandler::instance(QObject *parent)
{
    if (parent != nullptr && m_statichandeler == nullptr) {
        m_statichandeler = new DLDBusHandler(parent);
    }
    return m_statichandeler;
}

DLDBusHandler::~DLDBusHandler()
{
    quit();
}

DLDBusHandler::DLDBusHandler(QObject *parent)
    : QObject(parent)
{
    m_dbus = new DeepinLogviewerInterface("com.deepin.logviewer",
                                          "/com/deepin/logviewer",
                                          QDBusConnection::systemBus(),
                                          this);
    //Note: when dealing with remote objects, it is not always possible to determine if it exists when creating a QDBusInterface.
    if (!m_dbus->isValid() && !m_dbus->lastError().message().isEmpty()) {
        qDebug() << "m_dbus isValid false error:" << m_dbus->lastError() << m_dbus->lastError().message();
    }
    qDebug() << "m_dbus isValid true";
}

/*!
 * \~chinese \brief DLDBusHandler::readLog 读取日志文件
 * \~chinese \param filePath 文件路径
 * \~chinese \return 读取的日志
 */
QString DLDBusHandler::readLog(const QString &filePath)
{
    return m_dbus->readLog(filePath);
}

QString DLDBusHandler::openLogStream(const QString &filePath)
{
    return m_dbus->openLogStream(filePath);
}

QString DLDBusHandler::readLogInStream(const QString &token)
{
    return m_dbus->readLogInStream(token);
}

/*!
 * \~chinese \brief DLDBusHandler::exitCode 返回进程状态
 * \~chinese \return 进程返回值
 */
int DLDBusHandler::exitCode()
{
    return m_dbus->exitCode();
}

/*!
 * \~chinese \brief DLDBusHandler::quit 退出服务端程序
 */
void DLDBusHandler::quit()
{
    m_dbus->quit();
}

QStringList DLDBusHandler::getFileInfo(const QString &flag, bool unzip)
{
    QDBusPendingReply<QStringList> reply = m_dbus->getFileInfo(flag, unzip);
    reply.waitForFinished();
    if (reply.isError()) {
        qDebug() << reply.error().message();
    } else {
        filePath = reply.value();
    }
    return filePath;
}

QStringList DLDBusHandler::getOtherFileInfo(const QString &flag, bool unzip)
{
    QDBusPendingReply<QStringList> reply = m_dbus->getOtherFileInfo(flag, unzip);
    reply.waitForFinished();
    QStringList filePathList;
    if (reply.isError()) {
        qDebug() << reply.error().message();
    } else {
        filePathList = reply.value();
    }
    return filePathList;
}


bool DLDBusHandler::exportLog(const QString &outDir, const QString &in, bool isFile)
{
    return m_dbus->exportLog(outDir, in, isFile);
}
