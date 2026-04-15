#ifndef APPLISTWIDGET_H
#define APPLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

/**
 * @brief 应用列表组件
 *
 * 功能：
 * 1. 显示应用列表（图标+名称+版本+大小+星标评分）
 * 2. 支持安装按钮和信号
 * 3. 支持滚动
 */
class AppListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AppListWidget(QWidget *parent = nullptr);
    ~AppListWidget();

signals:
    void installClicked(const QString &appName);

private slots:
    void onInstallClicked(const QString &appName);

private:
    void setupUI();
    void loadAppData();
    QString getStarString(double rating);

private:
    QListWidget *m_listWidget;
};

#endif // APPLISTWIDGET_H
