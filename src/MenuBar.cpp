#include "MenuBar.h"
#include "RootController.h"

#include "UiStandards.h"
#include "Program.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include "StopButtonWidget.h"
#include "NetworkSettingsWidget.h"

#include <QTimer>

MenuBar::MenuBar(Program *program, QWidget *parent)
        : QWidget(parent)
        , m_title(new QLabel("", 0))
        , m_program(program)
        , m_stopButton(new StopButtonWidget(m_program))
        , m_layout(new QHBoxLayout(this))
{
        init();
}

MenuBar::MenuBar(QWidget *parent)
        : QWidget(parent)
        , m_title(new QLabel("", 0))
        , m_program(Program::instance())
        , m_stopButton(new StopButtonWidget(m_program))
        , m_layout(new QHBoxLayout(this))
{
        init();
}

MenuBar::~MenuBar()
{
        delete m_title;
}

void MenuBar::setTitle(const QString& title, const QPixmap& icon)
{
        m_title->setText(title + "  ");
        if(!icon.isNull()) m_title->setPixmap(icon);
}

QString MenuBar::title() const
{
        return m_title->text();
}

void MenuBar::addHomeAndBackButtons()
{
        clear();
        if(RootController::ref().depth() > 0) {
                 QAction *homeAction = addAction(UiStandards::homeString());
                 connect(homeAction, SIGNAL(triggered()), RootController::ptr(), SLOT(dismissAllWidgets()));
        }

        if(RootController::ref().depth() > 1) {
                QAction *backAction = addAction(UiStandards::backString());
                connect(backAction, SIGNAL(triggered()),
                        RootController::ptr(), SLOT(dismissWidget()));
        }
}

void MenuBar::init()
{
        QPalette pal = palette();

        // set black background
        switch (NetworkSettingsWidget::mode())
        {
                case NetworkSettingsWidget::Normal:
                        pal.setColor(QPalette::Background, QColor(0, 0, 0));
                        break;
                case NetworkSettingsWidget::Tournament:
                        pal.setColor(QPalette::Background, QColor(255, 0, 0));
                        break;
        }

        setAutoFillBackground(true);
        setPalette(pal);

        setLayout(m_layout);



        // setNativeMenuBar(false);
        //setMinimumHeight(65);
        m_title->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
        //m_title->setMinimumSize(50, 21);
        QPalette palette = m_title->palette();
        palette.setColor(m_title->foregroundRole(), Qt::white);
        m_title->setPalette(palette);
        // setCornerWidget(m_title);
        //m_stopButton->setMinimumSize(10, 22);
        //m_stopButton->resize(50, 22);
        //m_stopButton->setMaximumSize(100, 22);
        setStyleSheet("QPushButton { height:35px;font-size: 30px; }");//Delete this if error
        //QAction *stopButton = addAction(UiStandards::stopString());
        m_program->connect(m_stopButton, SIGNAL(clicked()), SLOT(stop()));
        // setTitle("Untitled")
        
        QTimer *timer = new QTimer(this);
        timer->setInterval(5000);
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeOut()));
        timer->start();
}

void MenuBar::addAction(QAction *const action)
{
        QPushButton *const button = new QPushButton(action->icon(), action->text(), this);

        connect(button, SIGNAL(clicked()), action, SLOT(trigger()));
        m_layout->addWidget(button);
}

QAction *MenuBar::addAction(const QString &str)
{
        QAction *const action = new QAction(str, this);
        addAction(action);
        return action;
}

void MenuBar::timeOut()
{
        QPalette pal = palette();

        // set black background
        switch (NetworkSettingsWidget::mode())
        {
                case NetworkSettingsWidget::Normal:
                        pal.setColor(QPalette::Background, QColor(0, 0, 0));
                        break;
                case NetworkSettingsWidget::Tournament:
                        pal.setColor(QPalette::Background, QColor(255, 0, 0));
                        break;
        }

        setAutoFillBackground(true);
        setPalette(pal);
}

void MenuBar::clear()
{
        for(int i = 0; i < m_layout->count(); ++i)
        {
                m_layout->removeItem(m_layout->itemAt(i));
        }
}
