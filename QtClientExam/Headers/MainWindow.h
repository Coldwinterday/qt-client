#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

// 引入5个展示组件
#include "CarouselWidget.h"
#include "BarChartWidget.h"
#include "PanelWidget.h"
#include "AppListWidget.h"
#include "DataTableWidget.h"

/**
 * @brief 主界面窗口类
 *
 * 功能：
 * 1. 左侧按钮导航栏（轮播图/柱状图/面板/列表/表格）
 * 2. 右侧内容展示区域（QStackedWidget）
 * 3. 切换时带淡入淡出动画
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /**
     * @brief 导航按钮点击槽函数
     * @param index 目标页面索引
     */
    void onNavButtonClicked(int index);

private:
    /**
     * @brief 初始化UI界面
     */
    void setupUI();

    /**
     * @brief 带动画切换页面
     * @param index 目标页面索引
     */
    void switchPageWithAnimation(int index);

private:
    QWidget *m_navWidget;              // 左侧导航区域
    QStackedWidget *m_stackedWidget;   // 右侧内容区
    QButtonGroup *m_buttonGroup;       // 按钮组
    QGraphicsOpacityEffect *m_opacityEffect;  // 透明度效果

    // 5个展示组件
    CarouselWidget *m_carouselWidget;
    BarChartWidget *m_barChartWidget;
    PanelWidget *m_panelWidget;
    AppListWidget *m_appListWidget;
    DataTableWidget *m_dataTableWidget;

    int m_currentIndex;  // 当前页面索引
};

#endif // MAINWINDOW_H
