#ifndef PANELWIDGET_H
#define PANELWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

/**
 * @brief 单个应用面板项
 */
class PanelItemWidget : public QFrame
{
    Q_OBJECT

public:
    explicit PanelItemWidget(const QString &icon, const QString &title, QWidget *parent = nullptr);

signals:
    void panelClicked(const QString &appName);
    void downloadClicked(const QString &appName);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString m_appName;
    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QPushButton *m_downloadBtn;
};

/**
 * @brief 面板网格组件
 *
 * 功能：
 * 1. 2列N行网格布局
 * 2. 每个面板显示图标+标题+下载按钮
 * 3. 支持点击事件和下载信号
 */
class PanelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PanelWidget(QWidget *parent = nullptr);
    ~PanelWidget();

private slots:
    void onPanelClicked(const QString &appName);
    void onDownloadClicked(const QString &appName);

private:
    void setupUI();
    void loadPanelData();
};

#endif // PANELWIDGET_H
