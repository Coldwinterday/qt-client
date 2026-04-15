#ifndef CAROUSELWIDGET_H
#define CAROUSELWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "DataModels.h"

/**
 * @brief 轮播图控件
 *
 * 功能：
 * 1. 自动定时切换图片（3秒）
 * 2. 手动切换（左右箭头按钮）
 * 3. 淡入淡出动画效果
 * 4. 显示图片标题
 * 5. 支持上传自定义图片
 */
class CarouselWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CarouselWidget(QWidget *parent = nullptr);
    ~CarouselWidget();

    /**
     * @brief 设置轮播图数据
     * @param items 轮播图数据列表
     */
    void setItems(const QVector<CarouselItem> &items);

private slots:
    /**
     * @brief 自动播放槽函数
     */
    void autoPlay();

    /**
     * @brief 切换到上一张
     */
    void onPrevClicked();

    /**
     * @brief 切换到下一张
     */
    void onNextClicked();

    /**
     * @brief 上传图片
     */
    void onUploadClicked();

private:
    /**
     * @brief 初始化UI
     */
    void setupUI();

    /**
     * @brief 切换图片
     * @param index 目标索引
     */
    void switchToImage(int index);

    /**
     * @brief 创建占位图片
     * @param text 文本内容
     * @return QPixmap
     */
    QPixmap createPlaceholderImage(const QString &text);

    /**
     * @brief 更新指示器
     */
    void updateIndicators();

private:
    QVector<CarouselItem> m_items;        // 轮播图数据
    int m_currentIndex;                    // 当前索引
    QLabel *m_imageLabel;                  // 图片显示标签
    QLabel *m_titleLabel;                  // 标题标签
    QPushButton *m_prevBtn;                // 上一张按钮
    QPushButton *m_nextBtn;                // 下一张按钮
    QPushButton *m_uploadBtn;              // 上传按钮
    QTimer *m_autoTimer;                   // 自动播放定时器
    QGraphicsOpacityEffect *m_opacityEffect; // 透明度效果
    bool m_isAnimating;                    // 是否正在动画中
    QHBoxLayout *m_indicatorLayout;        // 指示器布局
    QVector<QLabel*> m_indicators;         // 指示器列表
};

#endif // CAROUSELWIDGET_H
