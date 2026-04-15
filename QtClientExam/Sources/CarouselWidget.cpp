#include "CarouselWidget.h"
#include <QPainter>
#include <QFileInfo>
#include <QDir>
#include <QFile>

CarouselWidget::CarouselWidget(QWidget *parent)
    : QWidget(parent)
    , m_currentIndex(0)
    , m_isAnimating(false)
{
    setupUI();

    // 创建自动播放定时器（3秒）
    m_autoTimer = new QTimer(this);
    connect(m_autoTimer, &QTimer::timeout, this, &CarouselWidget::autoPlay);
    m_autoTimer->start(3000);

    // 连接按钮信号
    connect(m_prevBtn, &QPushButton::clicked, this, &CarouselWidget::onPrevClicked);
    connect(m_nextBtn, &QPushButton::clicked, this, &CarouselWidget::onNextClicked);

    // 初始化默认数据
    QVector<CarouselItem> defaultItems;
    defaultItems.append(CarouselItem("", "轮播图 1"));
    defaultItems.append(CarouselItem("", "轮播图 2"));
    defaultItems.append(CarouselItem("", "轮播图 3"));
    setItems(defaultItems);
}

CarouselWidget::~CarouselWidget()
{
}

void CarouselWidget::setupUI()
{
    // 创建图片显示标签
    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setMinimumSize(800, 400);
    m_imageLabel->setStyleSheet("background-color: #e0e0e0; border-radius: 10px;");

    // 创建透明度效果
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(1.0);
    m_imageLabel->setGraphicsEffect(m_opacityEffect);

    // 标题和上传按钮布局
    QHBoxLayout *headerLayout = new QHBoxLayout();

    m_titleLabel = new QLabel("轮播图", this);
    m_titleLabel->setAlignment(Qt::AlignLeft);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setStyleSheet("color: #333; margin: 10px;");

    // 上传按钮
    m_uploadBtn = new QPushButton("上传图片", this);
    m_uploadBtn->setFixedSize(100, 35);
    m_uploadBtn->setCursor(Qt::PointingHandCursor);
    m_uploadBtn->setStyleSheet(
        "QPushButton { "
        "   background-color: #3498db; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 5px; "
        "   font-size: 14px; "
        "}"
        "QPushButton:hover { "
        "   background-color: #2980b9; "
        "}"
        "QPushButton:pressed { "
        "   background-color: #21618c; "
        "}"
    );
    connect(m_uploadBtn, &QPushButton::clicked, this, &CarouselWidget::onUploadClicked);

    headerLayout->addWidget(m_titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(m_uploadBtn);

    // 创建左右按钮
    m_prevBtn = new QPushButton("◀", this);
    m_nextBtn = new QPushButton("▶", this);
    m_prevBtn->setFixedSize(50, 50);
    m_nextBtn->setFixedSize(50, 50);
    m_prevBtn->setCursor(Qt::PointingHandCursor);
    m_nextBtn->setCursor(Qt::PointingHandCursor);

    QString btnStyle =
        "QPushButton { "
        "   background-color: rgba(0, 0, 0, 0.5); "
        "   color: white; "
        "   border: none; "
        "   border-radius: 25px; "
        "   font-size: 20px; "
        "}"
        "QPushButton:hover { "
        "   background-color: rgba(0, 0, 0, 0.7); "
        "}"
        "QPushButton:pressed { "
        "   background-color: rgba(0, 0, 0, 0.9); "
        "}";
    m_prevBtn->setStyleSheet(btnStyle);
    m_nextBtn->setStyleSheet(btnStyle);

    // 按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_prevBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(m_nextBtn);

    // 指示器布局
    m_indicatorLayout = new QHBoxLayout();
    m_indicatorLayout->setSpacing(10);
    m_indicatorLayout->addStretch();
    // 指示器会在setItems时动态创建
    m_indicatorLayout->addStretch();

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(m_imageLabel);
    mainLayout->addLayout(btnLayout);
    mainLayout->addLayout(m_indicatorLayout);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    setLayout(mainLayout);
}

void CarouselWidget::setItems(const QVector<CarouselItem> &items)
{
    m_items = items;

    // 清除旧指示器
    for (QLabel *indicator : m_indicators) {
        m_indicatorLayout->removeWidget(indicator);
        indicator->deleteLater();
    }
    m_indicators.clear();

    // 创建新指示器
    for (int i = 0; i < m_items.size(); ++i) {
        QLabel *indicator = new QLabel(this);
        indicator->setFixedSize(12, 12);
        indicator->setStyleSheet("QLabel { background-color: #bdbdbd; border-radius: 6px; }");
        m_indicators.append(indicator);
        m_indicatorLayout->insertWidget(m_indicatorLayout->count() - 1, indicator);
    }

    if (!m_items.isEmpty()) {
        m_currentIndex = 0;
        switchToImage(0);
    }
}

void CarouselWidget::updateIndicators()
{
    for (int i = 0; i < m_indicators.size(); ++i) {
        if (i == m_currentIndex) {
            m_indicators[i]->setStyleSheet("QLabel { background-color: #3498db; border-radius: 6px; }");
        } else {
            m_indicators[i]->setStyleSheet("QLabel { background-color: #bdbdbd; border-radius: 6px; }");
        }
    }
}

void CarouselWidget::switchToImage(int index)
{
    if (m_items.isEmpty() || m_isAnimating) {
        return;
    }

    m_isAnimating = true;
    m_currentIndex = index;

    // 获取当前项
    const CarouselItem &item = m_items[m_currentIndex];

    // 淡出动画
    QPropertyAnimation *fadeOut = new QPropertyAnimation(m_opacityEffect, "opacity");
    fadeOut->setDuration(300);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);

    // 淡入动画
    QPropertyAnimation *fadeIn = new QPropertyAnimation(m_opacityEffect, "opacity");
    fadeIn->setDuration(300);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);

    // 淡出完成后更新图片并淡入
    connect(fadeOut, &QPropertyAnimation::finished, [this, item, fadeIn]() {
        // 更新图片和标题
        QPixmap pixmap;
        if (!item.imagePath.isEmpty() && QFile::exists(item.imagePath)) {
            // 加载真实图片
            pixmap.load(item.imagePath);
        } else {
            // 创建占位图片
            pixmap = createPlaceholderImage(item.title);
        }

        m_imageLabel->setPixmap(pixmap.scaled(m_imageLabel->size(),
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation));
        m_titleLabel->setText(item.title);

        // 更新指示器
        updateIndicators();

        // 开始淡入
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    });

    // 淡入完成后重置动画状态
    connect(fadeIn, &QPropertyAnimation::finished, [this]() {
        m_isAnimating = false;
    });

    // 开始淡出
    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
}

QPixmap CarouselWidget::createPlaceholderImage(const QString &text)
{
    // 创建占位图片
    QPixmap pixmap(800, 400);
    pixmap.fill(QColor(100 + (m_currentIndex * 30) % 155,
                       150 + (m_currentIndex * 40) % 105,
                       200 + (m_currentIndex * 20) % 55));

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制文字
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(48);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, text);

    return pixmap;
}

void CarouselWidget::autoPlay()
{
    if (m_items.isEmpty()) {
        return;
    }

    // 切换到下一张
    int nextIndex = (m_currentIndex + 1) % m_items.size();
    switchToImage(nextIndex);
}

void CarouselWidget::onPrevClicked()
{
    if (m_items.isEmpty()) {
        return;
    }

    // 切换到上一张
    int prevIndex = (m_currentIndex - 1 + m_items.size()) % m_items.size();
    switchToImage(prevIndex);

    // 重置自动播放定时器
    m_autoTimer->start(3000);
}

void CarouselWidget::onNextClicked()
{
    if (m_items.isEmpty()) {
        return;
    }

    // 切换到下一张
    int nextIndex = (m_currentIndex + 1) % m_items.size();
    switchToImage(nextIndex);

    // 重置自动播放定时器
    m_autoTimer->start(3000);
}

void CarouselWidget::onUploadClicked()
{
    // 打开文件对话框
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "选择图片",
        QDir::homePath(),
        "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)"
    );

    if (filePath.isEmpty()) {
        return;
    }

    // 验证图片
    QPixmap pixmap(filePath);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, "错误", "无效的图片文件！");
        return;
    }

    // 添加到轮播列表
    QString fileName = QFileInfo(filePath).fileName();
    QString title = QString("自定义图片 - %1").arg(fileName);
    CarouselItem newItem(filePath, title);
    m_items.append(newItem);

    // 创建新指示器
    QLabel *indicator = new QLabel(this);
    indicator->setFixedSize(12, 12);
    indicator->setStyleSheet("QLabel { background-color: #bdbdbd; border-radius: 6px; }");
    m_indicators.append(indicator);
    m_indicatorLayout->insertWidget(m_indicatorLayout->count() - 1, indicator);

    // 切换到新图片
    switchToImage(m_items.size() - 1);

    // 重置自动播放定时器
    m_autoTimer->start(3000);

    QMessageBox::information(this, "成功", QString("图片已添加：%1").arg(fileName));
}
