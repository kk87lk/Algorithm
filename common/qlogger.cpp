#include "qlogger.h"
#include <QMutexLocker>
#include <QDebug>

QLogger* QLogger::m_instance = nullptr;
QThread* QLogger::m_thread = nullptr;
QMutex QLogger::m_mutex;

QLogger* QLogger::instance()
{
    QMutexLocker locker(&m_mutex);
    if (!m_instance) {
        // 创建实例和线程
        m_instance = new QLogger();
        m_thread = new QThread();

        // 将实例移动到新线程
        m_instance->moveToThread(m_thread);

        // 连接线程结束信号到清理操作
        QObject::connect(m_thread, &QThread::finished, m_instance, &QObject::deleteLater);
        QObject::connect(m_thread, &QThread::finished, []() {
            m_thread->deleteLater();
            m_thread = nullptr;
            m_instance = nullptr;
        });

        // 连接日志信号
        QObject::connect(m_instance, &QLogger::logRequested, m_instance, &QLogger::handleLog);

        m_thread->start();
    }
    return m_instance;
}

QLogger::QLogger(QObject *parent) : QObject(parent)
{
    // 初始化代码（如打开日志文件）
}

QLogger::~QLogger()
{
    // 确保线程正确退出
    if (m_thread && m_thread->isRunning()) {
        m_thread->quit();
        m_thread->wait();
    }
}

void QLogger::log(const QString& message)
{
    // 通过信号跨线程调用
    Q_EMIT instance()->logRequested(message);
}

void QLogger::handleLog(const QString& message)
{
    // 实际处理日志（在对象所属线程执行）
    qDebug().noquote() << QDateTime::currentDateTime().toString()
                       << "[" << QThread::currentThreadId() << "]"
                       << message;

    // 这里可以添加文件写入等操作
    // QFile file("log.txt");
    // if (file.open(QIODevice::Append)) {
    //     file.write(message.toUtf8() + "\n");
    // }
}
