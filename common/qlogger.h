#ifndef QLOGGER_H
#define QLOGGER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QDateTime>

/**
 * @class QLogger
 * @brief 线程安全的单例日志记录器，运行于独立线程
 *
 * 本类提供跨线程的日志记录能力，通过Qt信号槽机制实现线程间通信，
 * 确保日志操作在专用线程中执行。支持扩展文件日志、网络日志等耗时操作。
 */
class QLogger : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     * @return QLogger* 单例实例指针
     * @note 首次调用时会自动创建实例和日志线程
     */
    static QLogger* instance();

    /**
     * @brief 记录日志的静态接口
     * @param[in] message 要记录的日志消息
     * @note 此方法线程安全，可在任何线程调用
     * @warning 消息内容不应包含换行符，由日志系统统一处理格式
     */
    static void log(const QString& message);

private:
    explicit QLogger(QObject *parent = nullptr);
    ~QLogger();

    // 禁止拷贝构造和赋值操作
    QLogger(const QLogger&) = delete;
    QLogger& operator=(const QLogger&) = delete;

    /// @brief 单例实例指针
    static QLogger* m_instance;

    /// @brief 专用日志线程
    static QThread* m_thread;

    /// @brief 实例创建互斥锁
    static QMutex m_mutex;

private slots:
    /**
     * @brief 实际处理日志的槽函数
     * @param[in] message 日志消息
     * @note 此方法在日志线程中执行，可安全执行I/O操作
     */
    void handleLog(const QString& message);

signals:
    /**
     * @brief 日志记录请求信号
     * @param message 要记录的日志消息
     * @note 通过此信号将日志请求从任意线程转发到日志线程
     */
    void logRequested(const QString& message);
};

#endif // QLOGGER_H
