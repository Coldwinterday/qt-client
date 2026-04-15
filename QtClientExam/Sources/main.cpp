#include <QApplication>
#include "LoginWindow.h"
#include "SplashWindow.h"
#include "MainWindow.h"

/**
 * @brief 程序入口
 *
 * 窗口跳转流程：
 * LoginWindow（登录） → SplashWindow（启动页） → MainWindow（主界面）
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 创建三个窗口
    LoginWindow *loginWindow = new LoginWindow();
    SplashWindow *splashWindow = new SplashWindow();
    MainWindow *mainWindow = new MainWindow();

    // 连接信号槽：登录成功 → 显示启动页
    QObject::connect(loginWindow, &LoginWindow::loginSuccess, [&]() {
        loginWindow->hide();
        splashWindow->show();
        splashWindow->startProgress();  // 开始进度加载
    });

    // 连接信号槽：启动完成 → 显示主界面
    QObject::connect(splashWindow, &SplashWindow::loadComplete, [&]() {
        splashWindow->hide();
        mainWindow->show();
    });

    // 显示登录窗口
    loginWindow->show();

    return app.exec();
}
