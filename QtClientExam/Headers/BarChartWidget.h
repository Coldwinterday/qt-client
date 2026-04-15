#ifndef BARCHARTWIDGET_H
#define BARCHARTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QMouseEvent>
#include <QTimer>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

/**
 * @brief 柱状图组件
 *
 * 功能：
 * 1. 显示6个月销售数据
 * 2. 柱状图绘制
 * 3. 鼠标悬停显示数值
 * 4. 动态数据更新
 * 5. 开始/暂停控制
 */
class BarChartWidget : public QWidget
{
    Q_OBJECT


public:
    explicit BarChartWidget(QWidget *parent = nullptr);
    ~BarChartWidget();

public:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onStartClicked();
    void onPauseClicked();
    void updateData();

private:
    void setupUI();
    void initData();
    int getBarIndexAtPos(const QPoint &pos);

private:
    struct BarData {
        QString month;
        int value;
    };

    QVector<BarData> m_data;
    int m_hoveredIndex;  // 鼠标悬停的柱子索引

    QTimer *m_updateTimer;
    QPushButton *m_startBtn;
    QPushButton *m_pauseBtn;
    QWidget *m_chartArea;  // 图表绘制区域
};

#endif // BARCHARTWIDGET_H
