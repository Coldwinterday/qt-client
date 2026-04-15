#include "BarChartWidget.h"
#include <QPainter>
#include <QDebug>
#include <QtMath>
#include <cstdlib>
#include <ctime>
#include <QWidget>

// 图表绘制区域类
class ChartArea : public QWidget
{
public:
    ChartArea(BarChartWidget *parent) : QWidget(parent), m_parent(parent) {
        setMouseTracking(true);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        m_parent->paintEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        m_parent->mouseMoveEvent(event);
    }

private:
    BarChartWidget *m_parent;
};

BarChartWidget::BarChartWidget(QWidget *parent)
    : QWidget(parent)
    , m_hoveredIndex(-1)
{
    srand(time(nullptr));
    setupUI();
    initData();
}

BarChartWidget::~BarChartWidget()
{
}

void BarChartWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // 标题和控制按钮
    QHBoxLayout *headerLayout = new QHBoxLayout();
    QLabel *titleLabel = new QLabel("销售数据柱状图", this);
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #333; }");

    m_startBtn = new QPushButton("开始更新", this);
    m_pauseBtn = new QPushButton("暂停", this);
    m_pauseBtn->setEnabled(false);

    QString btnStyle =
        "QPushButton { "
        "   background-color: #4CAF50; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 5px; "
        "   padding: 5px 15px; "
        "}"
        "QPushButton:hover { "
        "   background-color: #45a049; "
        "}"
        "QPushButton:pressed { "
        "   background-color: #3d8b40; "
        "}"
        "QPushButton:disabled { "
        "   background-color: #bdbdbd; "
        "}";

    m_startBtn->setStyleSheet(btnStyle);
    m_pauseBtn->setStyleSheet(btnStyle.replace("#4CAF50", "#ff9800")
                                      .replace("#45a049", "#fb8c00")
                                      .replace("#3d8b40", "#f57c00"));

    connect(m_startBtn, &QPushButton::clicked, this, &BarChartWidget::onStartClicked);
    connect(m_pauseBtn, &QPushButton::clicked, this, &BarChartWidget::onPauseClicked);

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(m_startBtn);
    headerLayout->addWidget(m_pauseBtn);

    // 图表区域
    m_chartArea = new ChartArea(this);
    m_chartArea->setMinimumSize(600, 400);

    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(m_chartArea);

    // 创建定时器
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &BarChartWidget::updateData);
}

void BarChartWidget::initData()
{
    // 初始化6个月的销售数据
    m_data.append({"1月", 120});
    m_data.append({"2月", 200});
    m_data.append({"3月", 150});
    m_data.append({"4月", 280});
    m_data.append({"5月", 230});
    m_data.append({"6月", 310});
}

void BarChartWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(m_chartArea);
    painter.setRenderHint(QPainter::Antialiasing);

    int margin = 50;
    int chartWidth = m_chartArea->width() - 2 * margin;
    int chartHeight = m_chartArea->height() - 2 * margin;

    // 绘制坐标轴
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(margin, m_chartArea->height() - margin, m_chartArea->width() - margin, m_chartArea->height() - margin); // X轴
    painter.drawLine(margin, margin, margin, m_chartArea->height() - margin); // Y轴

    // 计算最大值
    int maxValue = 0;
    for (const auto &data : m_data) {
        if (data.value > maxValue) maxValue = data.value;
    }
    maxValue = qMax(maxValue, 100);  // 至少100

    // 绘制柱状图
    int barCount = m_data.size();
    int barWidth = chartWidth / barCount * 0.6;
    int spacing = chartWidth / barCount;

    for (int i = 0; i < barCount; ++i) {
        int barHeight = (m_data[i].value * chartHeight) / maxValue;
        int x = margin + i * spacing + (spacing - barWidth) / 2;
        int y = m_chartArea->height() - margin - barHeight;

        // 悬停时高亮
        if (i == m_hoveredIndex) {
            painter.setBrush(QBrush(QColor(255, 152, 0))); // 橙色
        } else {
            painter.setBrush(QBrush(QColor(33, 150, 243))); // 蓝色
        }

        painter.drawRect(x, y, barWidth, barHeight);

        // 绘制月份标签
        painter.setPen(Qt::black);
        painter.drawText(x, m_chartArea->height() - margin + 20, barWidth, 20,
                        Qt::AlignCenter, m_data[i].month);

        // 悬停时显示数值
        if (i == m_hoveredIndex) {
            painter.drawText(x, y - 10, barWidth, 20,
                           Qt::AlignCenter, QString::number(m_data[i].value));
        }
    }

    // 绘制Y轴刻度
    painter.setPen(Qt::black);
    for (int i = 0; i <= 5; ++i) {
        int value = maxValue * i / 5;
        int y = m_chartArea->height() - margin - (chartHeight * i / 5);
        painter.drawText(5, y - 10, 40, 20, Qt::AlignRight, QString::number(value));
        painter.drawLine(margin - 5, y, margin, y);
    }
}

void BarChartWidget::mouseMoveEvent(QMouseEvent *event)
{
    int newIndex = getBarIndexAtPos(event->pos());
    if (newIndex != m_hoveredIndex) {
        m_hoveredIndex = newIndex;
        m_chartArea->update();
    }
}

int BarChartWidget::getBarIndexAtPos(const QPoint &pos)
{
    int margin = 50;
    int chartWidth = m_chartArea->width() - 2 * margin;
    int spacing = chartWidth / m_data.size();

    if (pos.x() < margin || pos.x() > m_chartArea->width() - margin) {
        return -1;
    }

    int index = (pos.x() - margin) / spacing;
    if (index >= 0 && index < m_data.size()) {
        return index;
    }

    return -1;
}

void BarChartWidget::onStartClicked()
{
    m_updateTimer->start(2000);  // 每2秒更新一次
    m_startBtn->setEnabled(false);
    m_pauseBtn->setEnabled(true);
}

void BarChartWidget::onPauseClicked()
{
    m_updateTimer->stop();
    m_startBtn->setEnabled(true);
    m_pauseBtn->setEnabled(false);
}

void BarChartWidget::updateData()
{
    // 随机更新每个柱子的值（±20）
    for (auto &data : m_data) {
        int delta = (rand() % 41) - 20;  // -20 到 +20
        data.value = qBound(50, data.value + delta, 350);  // 限制在50-350之间
    }
    m_chartArea->update();
}
