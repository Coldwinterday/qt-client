#include "SplashWindow.h"

SplashWindow::SplashWindow(QWidget *parent)
    : QWidget(parent)
    , m_currentProgress(0)
{
    setupUI();

    // 创建定时器
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SplashWindow::updateProgress);
}

SplashWindow::~SplashWindow()
{
}

void SplashWindow::setupUI()
{
    // 设置窗口标题和大小
    setWindowTitle("启动中...");
    setFixedSize(500, 300);

    // 创建标题标签
    m_titleLabel = new QLabel("易道云编程", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);

    // 创建状态标签
    m_statusLabel = new QLabel("初始化主界面... 0%", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    QFont statusFont = m_statusLabel->font();
    statusFont.setPointSize(12);
    m_statusLabel->setFont(statusFont);

    // 创建进度条
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(false);  // 不显示内置文本，使用自定义标签
    m_progressBar->setFixedHeight(25);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addSpacing(40);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_progressBar);
    mainLayout->addStretch();

    // 设置边距
    mainLayout->setContentsMargins(50, 50, 50, 50);

    setLayout(mainLayout);

    // 设置样式
    setStyleSheet(
        "QWidget { "
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "       stop:0 #667eea, stop:1 #764ba2); "
        "}"
        "QLabel { "
        "   color: white; "
        "}"
        "QProgressBar { "
        "   border: 2px solid white; "
        "   border-radius: 5px; "
        "   background-color: rgba(255, 255, 255, 0.3); "
        "}"
        "QProgressBar::chunk { "
        "   background-color: white; "
        "   border-radius: 3px; "
        "}"
    );
}

void SplashWindow::startProgress()
{
    // 重置进度
    m_currentProgress = 0;
    m_progressBar->setValue(0);
    m_statusLabel->setText("初始化主界面... 0%");

    // 启动定时器，每50ms更新一次
    m_timer->start(50);
}

void SplashWindow::updateProgress()
{
    // 每次增加2%
    m_currentProgress += 2;

    // 更新进度条
    m_progressBar->setValue(m_currentProgress);

    // 更新状态文本
    m_statusLabel->setText(QString("初始化主界面... %1%").arg(m_currentProgress));

    // 进度达到100%时停止定时器并发送完成信号
    if (m_currentProgress >= 100) {
        m_timer->stop();
        m_statusLabel->setText("加载完成！");

        // 延迟500ms后发送完成信号，让用户看到100%
        QTimer::singleShot(500, this, [this]() {
            emit loadComplete();
        });
    }
}
