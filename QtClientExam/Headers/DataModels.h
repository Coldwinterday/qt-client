#ifndef DATAMODELS_H
#define DATAMODELS_H

#include <QString>
#include <QVector>

/**
 * @brief 轮播图数据项
 */
struct CarouselItem {
    QString imagePath;   // 图片路径
    QString title;       // 标题

    CarouselItem(const QString &path = "", const QString &t = "")
        : imagePath(path), title(t) {}
};

/**
 * @brief 应用列表数据项
 */
struct AppItem {
    QString name;        // 应用名称
    QString iconPath;    // 图标路径
    double rating;       // 评分（0-5）
    int stars;           // 星标数

    AppItem(const QString &n = "", const QString &icon = "",
            double r = 0.0, int s = 0)
        : name(n), iconPath(icon), rating(r), stars(s) {}
};

/**
 * @brief 面板数据项
 */
struct PanelItem {
    QString title;       // 应用标题
    QString iconPath;    // 图标路径

    PanelItem(const QString &t = "", const QString &icon = "")
        : title(t), iconPath(icon) {}
};

/**
 * @brief 表格数据项
 */
struct TableRowData {
    int id;              // ID
    QString name;        // 名称
    QString target;      // 目标
    int position;        // 位置

    TableRowData(int i = 0, const QString &n = "",
                 const QString &t = "", int p = 0)
        : id(i), name(n), target(t), position(p) {}
};

/**
 * @brief 柱状图数据项
 */
struct BarChartData {
    QString label;       // 标签
    double value;        // 值（0-100）

    BarChartData(const QString &l = "", double v = 0.0)
        : label(l), value(v) {}
};

#endif // DATAMODELS_H
