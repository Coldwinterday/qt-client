#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_currentIndex(0)
{
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // 设置窗口标题和大小
    setWindowTitle("EDO - 易道云编程");
    resize(1200, 700);

    // 创建左侧导航区域
    m_navWidget = new QWidget(this);
    m_navWidget->setFixedWidth(150);
    m_navWidget->setStyleSheet("QWidget { background-color: #2c3e50; }");

    QVBoxLayout *navLayout = new QVBoxLayout(m_navWidget);
    navLayout->setContentsMargins(0, 20, 0, 20);
    navLayout->setSpacing(5);

    // 标题
    QLabel *titleLabel = new QLabel("EDO", m_navWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel { "
        "   color: white; "
        "   font-size: 24px; "
        "   font-weight: bold; "
        "   padding: 20px 0; "
        "}"
    );
    navLayout->addWidget(titleLabel);
    navLayout->addSpacing(20);

    // 创建按钮组
    m_buttonGroup = new QButtonGroup(this);
    QStringList navItems = {"轮播图", "柱状图", "面板", "列表", "表格"};

    QString btnStyle =
        "QPushButton { "
        "   background-color: transparent; "
        "   color: #bdc3c7; "
        "   border: none; "
        "   border-left: 3px solid transparent; "
        "   padding: 15px 20px; "
        "   text-align: left; "
        "   font-size: 14px; "
        "}"
        "QPushButton:hover { "
        "   background-color: #34495e; "
        "   color: white; "
        "}"
        "QPushButton:checked { "
        "   background-color: #34495e; "
        "   color: white; "
        "   border-left: 3px solid #3498db; "
        "   font-weight: bold; "
        "}";

    for (int i = 0; i < navItems.size(); ++i) {
        QPushButton *btn = new QPushButton(navItems[i], m_navWidget);
        btn->setCheckable(true);
        btn->setStyleSheet(btnStyle);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedHeight(50);

        m_buttonGroup->addButton(btn, i);
        navLayout->addWidget(btn);

        connect(btn, &QPushButton::clicked, [this, i]() {
            onNavButtonClicked(i);
        });
    }

    // 默认选中第一个
    m_buttonGroup->button(0)->setChecked(true);

    navLayout->addStretch();

    // 创建右侧内容区
    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->setStyleSheet(
        "QStackedWidget { "
        "   background-color: #ecf0f1; "
        "   border: none; "
        "}"
    );

    // 创建透明度效果
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(1.0);
    m_stackedWidget->setGraphicsEffect(m_opacityEffect);

    // 创建5个展示组件
    m_carouselWidget = new CarouselWidget(this);
    m_barChartWidget = new BarChartWidget(this);
    m_panelWidget = new PanelWidget(this);
    m_appListWidget = new AppListWidget(this);
    m_dataTableWidget = new DataTableWidget(this);

    // 添加到StackedWidget
    m_stackedWidget->addWidget(m_carouselWidget);
    m_stackedWidget->addWidget(m_barChartWidget);
    m_stackedWidget->addWidget(m_panelWidget);
    m_stackedWidget->addWidget(m_appListWidget);
    m_stackedWidget->addWidget(m_dataTableWidget);

    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_navWidget);
    mainLayout->addWidget(m_stackedWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    setLayout(mainLayout);

    // 设置整体背景
    setStyleSheet("MainWindow { background-color: #ecf0f1; }");
}

void MainWindow::onNavButtonClicked(int index)
{
    if (index == m_currentIndex) {
        return;  // 已经是当前页面，不需要切换
    }

    switchPageWithAnimation(index);
    m_currentIndex = index;
}

void MainWindow::switchPageWithAnimation(int index)
{
    // 淡出动画
    QPropertyAnimation *fadeOut = new QPropertyAnimation(m_opacityEffect, "opacity");
    fadeOut->setDuration(150);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);

    connect(fadeOut, &QPropertyAnimation::finished, [this, index, fadeOut]() {
        // 切换页面
        m_stackedWidget->setCurrentIndex(index);

        // 淡入动画
        QPropertyAnimation *fadeIn = new QPropertyAnimation(m_opacityEffect, "opacity");
        fadeIn->setDuration(150);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);

        fadeOut->deleteLater();
    });

    fadeOut->start();
}
