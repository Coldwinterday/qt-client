#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

/**
 * @brief 登录窗口类
 *
 * 功能：
 * 1. 提供用户名和密码输入框
 * 2. 硬编码校验（admin/123456）
 * 3. 登录成功后发送 loginSuccess 信号
 */
class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    /**
     * @brief 登录成功信号
     * 用于通知主程序切换到启动页
     */
    void loginSuccess();

private slots:
    /**
     * @brief 登录按钮点击槽函数
     * 验证用户名和密码，成功则发送 loginSuccess 信号
     */
    void onLoginClicked();

private:
    /**
     * @brief 初始化UI界面
     */
    void setupUI();

private:
    QLineEdit *m_usernameEdit;    // 用户名输入框
    QLineEdit *m_passwordEdit;    // 密码输入框
    QPushButton *m_loginBtn;      // 登录按钮
    QLabel *m_titleLabel;         // 标题标签
};

#endif // LOGINWINDOW_H
