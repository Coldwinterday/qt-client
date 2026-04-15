#include "PanelWidget.h"
#include <QMouseEvent>

// ==================== PanelItemWidget 实现 ====================

PanelItemWidget::PanelItemWidget(const QString &icon, const QString &title, QWidget *parent)
    : QFrame(parent)
    , m_appName(title)
{
    setFrameShape(QFrame::Box);
    setStyleSheet(
        "PanelItemWidget { "
        "   background-color: white; "
        "   border: 2px solid #e0e0e0; "
        "   border-radius: 10px; "
        "}"
        "PanelItemWidget:hover { "
        "   border: 2px solid #2196f3; "
        "   background-color: #f5f5f5; "
        "}"
    );
    setMinimumSize(250, 180);
    setCursor(Qt::PointingHandCursor);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    // 图标
    m_iconLabel = new QLabel(icon, this);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->setStyleSheet("QLabel { font-size: 48px; border: none; }");

    // 标题
    m_titleLabel = new QLabel(title, this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #333; border: none; }");

    // 下载按钮
    m_downloadBtn = new QPushButton("下载", this);
    m_downloadBtn->setStyleSheet(
        "QPushButton { "
        "   background-color: #4CAF50; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 5px; "
        "   padding: 8px 20px; "
        "   font-size: 14px; "
        "}"
        "QPushButton:hover { "
        "   background-color: #45a049; "
        "}"
        "QPushButton:pressed { "
        "   background-color: #3d8b40; "
        "}"
    );
    m_downloadBtn->setCursor(Qt::PointingHandCursor);

    // 连接下载按钮信号
    connect(m_downloadBtn, &QPushButton::clicked, [this]() {
        emit downloadClicked(m_appName);
    });

    layout->addWidget(m_iconLabel);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_downloadBtn);
}

void PanelItemWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit panelClicked(m_appName);
    }
    QFrame::mousePressEvent(event);
}

// ==================== PanelWidget 实现 ====================

PanelWidget::PanelWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadPanelData();
}

PanelWidget::~PanelWidget()
{
}

void PanelWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // 标题
    QLabel *titleLabel = new QLabel("应用面板", this);
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #333; }");

    mainLayout->addWidget(titleLabel);
}

void PanelWidget::loadPanelData()
{
    // 获取主布局
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());

    // 创建网格布局（2列）
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(20);
    gridLayout->setContentsMargins(0, 10, 0, 10);

    // 应用数据
    struct AppData {
        QString icon;
        QString name;
    };

    QVector<AppData> apps = {
        {"📱", "微信"},
        {"🎵", "QQ音乐"},
        {"📺", "抖音"},
        {"🛒", "淘宝"},
        {"🍔", "美团"},
        {"🚗", "高德地图"},
        {"💬", "钉钉"},
        {"📰", "今日头条"},
        {"🎮", "王者荣耀"},
        {"📷", "美图秀秀"}
    };

    // 创建面板（2列布局）
    for (int i = 0; i < apps.size(); ++i) {
        PanelItemWidget *panel = new PanelItemWidget(apps[i].icon, apps[i].name, this);

        // 连接信号
        connect(panel, &PanelItemWidget::panelClicked, this, &PanelWidget::onPanelClicked);
        connect(panel, &PanelItemWidget::downloadClicked, this, &PanelWidget::onDownloadClicked);

        int row = i / 2;
        int col = i % 2;
        gridLayout->addWidget(panel, row, col);
    }

    mainLayout->addLayout(gridLayout);
    mainLayout->addStretch();
}

void PanelWidget::onPanelClicked(const QString &appName)
{
    QMessageBox::information(this, "面板点击", QString("你点击了：%1").arg(appName));
}

void PanelWidget::onDownloadClicked(const QString &appName)
{
    QMessageBox::information(this, "下载", QString("开始下载：%1").arg(appName));
}
