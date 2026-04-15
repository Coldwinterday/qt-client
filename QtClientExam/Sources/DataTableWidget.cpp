#include "DataTableWidget.h"
#include <QHeaderView>
#include <QLabel>

DataTableWidget::DataTableWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadTableData();
}

DataTableWidget::~DataTableWidget()
{
}

void DataTableWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // 标题和按钮行
    QHBoxLayout *headerLayout = new QHBoxLayout();
    QLabel *titleLabel = new QLabel("用户数据表", this);
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #333; }");

    m_addBtn = new QPushButton("添加数据", this);
    m_batchUpdateBtn = new QPushButton("批量更新", this);
    m_deleteBtn = new QPushButton("删除选中", this);

    QString btnStyle =
        "QPushButton { "
        "   background-color: #2196f3; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 5px; "
        "   padding: 5px 15px; "
        "}"
        "QPushButton:hover { "
        "   background-color: #1976d2; "
        "}"
        "QPushButton:pressed { "
        "   background-color: #0d47a1; "
        "}";

    m_addBtn->setStyleSheet(btnStyle);
    m_batchUpdateBtn->setStyleSheet(btnStyle);
    m_deleteBtn->setStyleSheet(btnStyle.replace("#2196f3", "#f44336")
                                       .replace("#1976d2", "#d32f2f")
                                       .replace("#0d47a1", "#b71c1c"));

    connect(m_addBtn, &QPushButton::clicked, this, &DataTableWidget::onAddDataClicked);
    connect(m_batchUpdateBtn, &QPushButton::clicked, this, &DataTableWidget::onBatchUpdateClicked);
    connect(m_deleteBtn, &QPushButton::clicked, this, &DataTableWidget::onDeleteClicked);

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(m_addBtn);
    headerLayout->addWidget(m_batchUpdateBtn);
    headerLayout->addWidget(m_deleteBtn);

    // 表格
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(5);
    m_tableWidget->setHorizontalHeaderLabels({"ID", "姓名", "年龄", "邮箱", "状态"});

    // 设置表格样式
    m_tableWidget->setStyleSheet("QTableWidget { border: 1px solid #ddd; border-radius: 5px; "
                                 "gridline-color: #e0e0e0; }");
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->setAlternatingRowColors(true);
    m_tableWidget->setSortingEnabled(true);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);  // 多选模式

    // 调整列宽
    m_tableWidget->setColumnWidth(0, 80);
    m_tableWidget->setColumnWidth(1, 100);
    m_tableWidget->setColumnWidth(2, 80);
    m_tableWidget->setColumnWidth(3, 200);

    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(m_tableWidget);
}

void DataTableWidget::loadTableData()
{
    // 模拟用户数据
    struct UserData {
        int id;
        QString name;
        int age;
        QString email;
        QString status;
    };

    QVector<UserData> users = {
        {1001, "张三", 28, "zhangsan@example.com", "在线"},
        {1002, "李四", 32, "lisi@example.com", "离线"},
        {1003, "王五", 25, "wangwu@example.com", "在线"},
        {1004, "赵六", 35, "zhaoliu@example.com", "忙碌"},
        {1005, "钱七", 29, "qianqi@example.com", "在线"},
        {1006, "孙八", 41, "sunba@example.com", "离线"},
        {1007, "周九", 27, "zhoujiu@example.com", "在线"},
        {1008, "吴十", 33, "wushi@example.com", "忙碌"},
        {1009, "郑十一", 30, "zhengshiyi@example.com", "在线"},
        {1010, "王十二", 26, "wangshier@example.com", "离线"}
    };

    m_tableWidget->setRowCount(users.size());

    for (int i = 0; i < users.size(); ++i) {
        m_tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(users[i].id)));
        m_tableWidget->setItem(i, 1, new QTableWidgetItem(users[i].name));
        m_tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(users[i].age)));
        m_tableWidget->setItem(i, 3, new QTableWidgetItem(users[i].email));

        QTableWidgetItem *statusItem = new QTableWidgetItem(users[i].status);
        if (users[i].status == "在线") {
            statusItem->setForeground(QBrush(QColor(76, 175, 80))); // 绿色
        } else if (users[i].status == "离线") {
            statusItem->setForeground(QBrush(QColor(158, 158, 158))); // 灰色
        } else {
            statusItem->setForeground(QBrush(QColor(255, 152, 0))); // 橙色
        }
        m_tableWidget->setItem(i, 4, statusItem);
    }
}

int DataTableWidget::getNextId()
{
    int maxId = 1000;
    for (int i = 0; i < m_tableWidget->rowCount(); ++i) {
        int id = m_tableWidget->item(i, 0)->text().toInt();
        if (id > maxId) {
            maxId = id;
        }
    }
    return maxId + 1;
}

void DataTableWidget::onAddDataClicked()
{
    // 输入姓名
    bool ok;
    QString name = QInputDialog::getText(this, "添加数据", "姓名:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    // 输入年龄
    int age = QInputDialog::getInt(this, "添加数据", "年龄:", 25, 1, 120, 1, &ok);
    if (!ok) return;

    // 输入邮箱
    QString email = QInputDialog::getText(this, "添加数据", "邮箱:", QLineEdit::Normal, "", &ok);
    if (!ok || email.isEmpty()) return;

    // 关键：添加前禁用排序，防止崩溃
    m_tableWidget->setSortingEnabled(false);

    // 添加到表格
    int row = m_tableWidget->rowCount();
    m_tableWidget->insertRow(row);

    // 创建并设置每个item
    QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(getNextId()));
    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    QTableWidgetItem *ageItem = new QTableWidgetItem(QString::number(age));
    QTableWidgetItem *emailItem = new QTableWidgetItem(email);
    QTableWidgetItem *statusItem = new QTableWidgetItem("在线");

    // 设置对齐方式
    idItem->setTextAlignment(Qt::AlignCenter);
    ageItem->setTextAlignment(Qt::AlignCenter);
    statusItem->setTextAlignment(Qt::AlignCenter);

    // 设置状态颜色
    statusItem->setForeground(QBrush(QColor(76, 175, 80)));

    // 插入所有item
    m_tableWidget->setItem(row, 0, idItem);
    m_tableWidget->setItem(row, 1, nameItem);
    m_tableWidget->setItem(row, 2, ageItem);
    m_tableWidget->setItem(row, 3, emailItem);
    m_tableWidget->setItem(row, 4, statusItem);

    // 重新启用排序
    m_tableWidget->setSortingEnabled(true);

    QMessageBox::information(this, "成功", "数据添加成功！");
}

void DataTableWidget::onBatchUpdateClicked()
{
    // 获取选中的行
    QList<QTableWidgetItem*> selectedItems = m_tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选中要更新的行！");
        return;
    }

    // 选择新状态
    QStringList statusOptions = {"在线", "离线", "忙碌"};
    bool ok;
    QString newStatus = QInputDialog::getItem(this, "批量更新状态",
                                              "选择新状态:",
                                              statusOptions, 0, false, &ok);
    if (!ok) return;

    // 获取选中的行号（去重）
    QSet<int> selectedRows;
    for (auto *item : selectedItems) {
        selectedRows.insert(item->row());
    }

    // 批量更新
    for (int row : selectedRows) {
        QTableWidgetItem *statusItem = new QTableWidgetItem(newStatus);

        if (newStatus == "在线") {
            statusItem->setForeground(QBrush(QColor(76, 175, 80)));
        } else if (newStatus == "离线") {
            statusItem->setForeground(QBrush(QColor(158, 158, 158)));
        } else {
            statusItem->setForeground(QBrush(QColor(255, 152, 0)));
        }

        m_tableWidget->setItem(row, 4, statusItem);
    }

    QMessageBox::information(this, "成功",
                            QString("已更新 %1 条数据的状态为：%2")
                            .arg(selectedRows.size()).arg(newStatus));
}

void DataTableWidget::onDeleteClicked()
{
    // 获取选中的行
    QList<QTableWidgetItem*> selectedItems = m_tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选中要删除的行！");
        return;
    }

    // 获取选中的行号（去重并排序）
    QSet<int> selectedRows;
    for (auto *item : selectedItems) {
        selectedRows.insert(item->row());
    }

    // 确认删除
    int ret = QMessageBox::question(this, "确认删除",
                                    QString("确定要删除选中的 %1 条数据吗？")
                                    .arg(selectedRows.size()),
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) return;

    // 从后往前删除（避免索引变化）
    QList<int> rowList = selectedRows.values();
    std::sort(rowList.begin(), rowList.end(), std::greater<int>());

    for (int row : rowList) {
        m_tableWidget->removeRow(row);
    }

    QMessageBox::information(this, "成功", "删除成功！");
}

