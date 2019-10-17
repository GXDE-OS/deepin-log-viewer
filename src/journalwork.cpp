#include "journalwork.h"
#include <DApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>

DWIDGET_USE_NAMESPACE

journalWork::journalWork(QStringList arg, QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<QList<LOG_MSG_JOURNAL> >("QList<LOG_MSG_JOURNAL>");

    initMap();

    m_arg.append("-o");
    m_arg.append("json");
    if (!arg.isEmpty())
        m_arg.append(arg);
}

journalWork::~journalWork()
{
    logList.clear();
    m_map.clear();
}

void journalWork::doWork()
{
    logList.clear();
    QProcess proc;

    //! by time: --since="xxxx-xx-xx" --until="xxxx-xx-xx" exclude U
    //! by priority: journalctl PRIORITY=x
    proc.start("journalctl", m_arg);
    proc.waitForFinished();

    QByteArray output = proc.readAllStandardOutput();
    proc.close();

    for (QByteArray data : output.split('\n')) {
        if (data.isEmpty())
            continue;
        //        QFile fi("/home/archermind/jour.json");
        //        if (fi.open(QIODevice::ReadWrite | QIODevice::Append)) {
        //            fi.write(QString::number(logList.count()).toLatin1());
        //            fi.write(data);
        //            fi.write("\n");
        //        }

        LOG_MSG_JOURNAL logMsg;

        QJsonParseError erro;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &erro));

        if (erro.error != QJsonParseError::NoError) {
            qDebug() << "erro" << erro.error << erro.errorString();
            continue;
        }
        QJsonObject jsonObj = jsonDoc.object();
        // fill field
        QString dt = jsonObj.value("_SOURCE_REALTIME_TIMESTAMP").toString();
        if (dt.isEmpty())
            dt = jsonObj.value("__REALTIME_TIMESTAMP").toString();
        logMsg.dateTime = getDateTimeFromStamp(dt);
        logMsg.hostName = jsonObj.value("_HOSTNAME").toString();
        logMsg.daemonName = jsonObj.value("_COMM").toString();
        logMsg.daemonId = jsonObj.value("_PID").toString();
        logMsg.msg = jsonObj.value("MESSAGE").toString();
        logMsg.level = i2str(jsonObj.value("PRIORITY").toString().toInt());
        logList.append(logMsg);
    }

    emit journalFinished(logList);
}

QString journalWork::getDateTimeFromStamp(QString str)
{
    QString ret = "";
    QString ums = str.right(6);
    QString dtstr = str.left(str.length() - 6);
    QDateTime dt = QDateTime::fromTime_t(dtstr.toUInt());
    ret = dt.toString("yyyy-MM-dd hh:mm:ss") + QString(".%1").arg(ums);
    return ret;
}

void journalWork::initMap()
{
    m_map.clear();
    m_map.insert(0, DApplication::translate("Level", "Emer"));
    m_map.insert(1, DApplication::translate("Level", "Alert"));
    m_map.insert(2, DApplication::translate("Level", "Critical"));
    m_map.insert(3, DApplication::translate("Level", "Error"));
    m_map.insert(4, DApplication::translate("Level", "Warning"));
    m_map.insert(5, DApplication::translate("Level", "Notice"));
    m_map.insert(6, DApplication::translate("Level", "Info"));
    m_map.insert(7, DApplication::translate("Level", "Debug"));
}

QString journalWork::i2str(int prio)
{
    return m_map.value(prio);
}