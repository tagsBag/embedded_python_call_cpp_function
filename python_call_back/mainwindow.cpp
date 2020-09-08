#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    centralwidget = new QWidget(this);
    gridLayout = new QGridLayout();
    horizontalLayout = new QHBoxLayout();
    apply_btn = new QPushButton("apply");
    fn_textbox = new QPlainTextEdit();

    setCentralWidget(centralwidget);
    centralwidget->setLayout(gridLayout);
    gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);
    horizontalLayout->addWidget(apply_btn);
    gridLayout->addWidget(fn_textbox);

    m_strategy = std::make_shared<Strategy>(2);


    fn_textbox->setPlainText(m_strategy->get_script().c_str());

    connect(this->apply_btn,&QPushButton::clicked,[=]
    {
        {   ///mid 编辑后重新获取 文本 并 设置到 strategy,strategy 保存 策略 到文件
            QString fn = fn_textbox->toPlainText();
            m_strategy->set_script(fn.toStdString());
        }

        m_strategy->write_log_string("call write_log from c++");
        m_strategy->run_python();
    });


}

MainWindow::~MainWindow()
{
}

