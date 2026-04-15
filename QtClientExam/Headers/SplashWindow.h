#ifndef SPLASHWINDOW_H
#define SPLASHWINDOW_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>

/**
 * @brief 启动页窗口类
 *
 * 功能：
 * 1. 显示启动图案和标题
 * 2. 通过进度条展示加载进度（0→100%）
 * 3. 加载完成后发送 loadComplete 信号
 */
class SplashWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SplashWindow(QWidget *parent = nullptr);
    ~SplashWindow();

    /**
     * @brief 开始进度加载
     * 启动定时器，模拟加载过程
     */
    void startProgress();

signals:
    /**
     * @brief 加载完成信号
     * 用于通知主程序切换到主界面
     */
    void loadComplete();

private slots:
    /**
     * @brief 更新进度槽函数
     * 定时器触发，更新进度条和百分比显示
     */
    void updateProgress();

private:
    /**
     * @brief 初始化UI界面
     */
    void setupUI();

private:
    QProgressBar *m_progressBar;   // 进度条
    QLabel *m_titleLabel;          // 标题标签
    QLabel *m_statusLabel;         // 状态标签（显示百分比）
    QTimer *m_timer;               // 定时器
    int m_currentProgress;         // 当前进度值（0-100）
};

#endif // SPLASHWINDOW_H
