#ifndef DATATABLEWIDGET_H
#define DATATABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>

/**
 * @brief 数据表格组件
 *
 * 功能：
 * 1. QTableWidget显示用户数据
 * 2. 列：ID/姓名/年龄/邮箱/状态
 * 3. 支持排序
 * 4. 支持添加数据
 * 5. 支持批量更新状态
 */
class DataTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataTableWidget(QWidget *parent = nullptr);
    ~DataTableWidget();

private slots:
    void onAddDataClicked();
    void onBatchUpdateClicked();
    void onDeleteClicked();

private:
    void setupUI();
    void loadTableData();
    int getNextId();

private:
    QTableWidget *m_tableWidget;
    QPushButton *m_addBtn;
    QPushButton *m_batchUpdateBtn;
    QPushButton *m_deleteBtn;
};

#endif // DATATABLEWIDGET_H
