#include "AppListWidget.h"
#include <QLabel>
#include <QHBoxLayout>

AppListWidget::AppListWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadAppData();
}

AppListWidget::~AppListWidget()
{
}

void AppListWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // 标题
    QLabel *titleLabel = new QLabel("应用列表", this);
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #333; }");

    m_listWidget = new QListWidget(this);
    m_listWidget->setStyleSheet("QListWidget { border: 1px solid #ddd; border-radius: 5px; }");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(m_listWidget);
}

void AppListWidget::loadAppData()
{
    // 模拟应用数据
    struct AppInfo {
        QString icon;
        QString name;
        QString version;
        QString size;
        double rating;
        bool isInstalled;
    };

    QVector<AppInfo> apps = {
        {"📱", "微信", "v8.0.32", "256 MB", 4.8, true},
        {"🎵", "QQ音乐", "v11.2.5", "128 MB", 4.5, false},
        {"📺", "抖音", "v23.5.0", "189 MB", 4.2, false},
        {"🛒", "淘宝", "v10.8.20", "312 MB", 4.6, true},
        {"🍔", "美团", "v12.3.201", "145 MB", 4.7, false},
        {"🚗", "高德地图", "v11.15.0", "234 MB", 4.9, false},
        {"💬", "钉钉", "v6.5.40", "178 MB", 4.3, true},
        {"📰", "今日头条", "v9.0.8", "156 MB", 4.1, false},
        {"🎮", "王者荣耀", "v3.82.1.8", "1.8 GB", 4.4, false},
        {"📷", "美图秀秀", "v9.2.8.0", "98 MB", 4.0, false}
    };

    for (const auto &app : apps) {
        QWidget *itemWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(itemWidget);
        layout->setContentsMargins(10, 5, 10, 5);

        // 图标
        QLabel *iconLabel = new QLabel(app.icon);
        iconLabel->setStyleSheet("QLabel { font-size: 32px; }");
        iconLabel->setFixedSize(50, 50);

        // 应用信息
        QVBoxLayout *infoLayout = new QVBoxLayout();
        QLabel *nameLabel = new QLabel(app.name);
        nameLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");

        QLabel *versionLabel = new QLabel(QString("%1 | %2").arg(app.version).arg(app.size));
        versionLabel->setStyleSheet("QLabel { font-size: 12px; color: #888; }");

        // 星标评分
        QLabel *ratingLabel = new QLabel(getStarString(app.rating));
        ratingLabel->setStyleSheet("QLabel { font-size: 12px; color: #ff9800; }");

        infoLayout->addWidget(nameLabel);
        infoLayout->addWidget(versionLabel);
        infoLayout->addWidget(ratingLabel);

        // 安装按钮
        QPushButton *installBtn = new QPushButton(app.isInstalled ? "已安装" : "安装");
        installBtn->setFixedSize(80, 30);

        if (app.isInstalled) {
            installBtn->setStyleSheet(
                "QPushButton { "
                "   background-color: #bdbdbd; "
                "   color: white; "
                "   border: none; "
                "   border-radius: 5px; "
                "}"
            );
            installBtn->setEnabled(false);
        } else {
            installBtn->setStyleSheet(
                "QPushButton { "
                "   background-color: #4CAF50; "
                "   color: white; "
                "   border: none; "
                "   border-radius: 5px; "
                "}"
                "QPushButton:hover { "
                "   background-color: #45a049; "
                "}"
                "QPushButton:pressed { "
                "   background-color: #3d8b40; "
                "}"
            );
            installBtn->setCursor(Qt::PointingHandCursor);

            // 连接信号
            QString appName = app.name;
            connect(installBtn, &QPushButton::clicked, [this, appName]() {
                onInstallClicked(appName);
            });
        }

        layout->addWidget(iconLabel);
        layout->addLayout(infoLayout);
        layout->addStretch();
        layout->addWidget(installBtn);

        QListWidgetItem *item = new QListWidgetItem(m_listWidget);
        item->setSizeHint(QSize(0, 90));
        m_listWidget->setItemWidget(item, itemWidget);
    }
}

QString AppListWidget::getStarString(double rating)
{
    int fullStars = (int)rating;
    bool hasHalf = (rating - fullStars) >= 0.5;

    QString stars;
    for (int i = 0; i < fullStars; ++i) {
        stars += "⭐";
    }
    if (hasHalf) {
        stars += "⭐";
    }

    return stars + QString(" %1分").arg(rating, 0, 'f', 1);
}

void AppListWidget::onInstallClicked(const QString &appName)
{
    emit installClicked(appName);
    QMessageBox::information(this, "安装", QString("开始安装：%1").arg(appName));
}
