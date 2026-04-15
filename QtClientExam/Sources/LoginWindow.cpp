#include "LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUI();

    // 连接登录按钮信号槽
    connect(m_loginBtn, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::setupUI()
{
    // 设置窗口标题和大小
    setWindowTitle("用户登录");
    setFixedSize(400, 300);

    // 创建标题标签
    m_titleLabel = new QLabel("易道云编程 - 用户登录", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);

    // 创建用户名输入框
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("请输入用户名");
    m_usernameEdit->setFixedHeight(35);

    // 创建密码输入框
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("请输入密码");
    m_passwordEdit->setEchoMode(QLineEdit::Password);  // 密码模式
    m_passwordEdit->setFixedHeight(35);

    // 创建登录按钮
    m_loginBtn = new QPushButton("登录", this);
    m_loginBtn->setFixedHeight(40);
    m_loginBtn->setCursor(Qt::PointingHandCursor);

    // 创建用户名标签和输入框布局
    QLabel *usernameLabel = new QLabel("用户名:", this);
    QHBoxLayout *usernameLayout = new QHBoxLayout();
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(m_usernameEdit);

    // 创建密码标签和输入框布局
    QLabel *passwordLabel = new QLabel("密码:", this);
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(m_passwordEdit);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(usernameLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(passwordLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_loginBtn);
    mainLayout->addStretch();

    setLayout(mainLayout);

    // 设置简单样式
    setStyleSheet(
        "QWidget { background-color: #f5f5f5; }"
        "QLineEdit { "
        "   border: 1px solid #ddd; "
        "   border-radius: 5px; "
        "   padding: 5px; "
        "   background-color: white; "
        "}"
        "QPushButton { "
        "   background-color: #4CAF50; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 5px; "
        "   font-size: 14px; "
        "   font-weight: bold; "
        "}"
        "QPushButton:hover { "
        "   background-color: #45a049; "
        "}"
        "QPushButton:pressed { "
        "   background-color: #3d8b40; "
        "}"
        "QLabel { "
        "   color: #333; "
        "}"
    );
}

void LoginWindow::onLoginClicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text().trimmed();

    // 硬编码校验：admin/123456
    if (username == "admin" && password == "123456") {
        // 登录成功，发送信号
        emit loginSuccess();
    } else {
        // 登录失败，显示错误提示
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！\n\n提示：用户名 admin，密码 123456");
        m_passwordEdit->clear();
        m_passwordEdit->setFocus();
    }
}
