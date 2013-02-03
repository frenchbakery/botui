#include "CombinedMotorWidget.h"
#include "ui_CombinedMotorWidget.h"

#include <QTimer>
#include <QDebug>

#include <math.h>
#include <kovan/kovan.h>

#include "NumpadDialog.h"

// #define A_KOVAN

CombinedMotorWidget::CombinedMotorWidget(Device *device, QWidget *parent)
	: StandardWidget(device, parent),
	ui(new Ui::CombinedMotorWidget)
{
	ui->setupUi(this);
	
	performStandardSetup(tr("Motors"), false);
	
	ui->pwm->setMinimumValue(-100.0);
	ui->pwm->setMaximumValue(100.0);
	ui->pwm->setValue(0.0);
	
	ui->velocity->setMinimumValue(-1500.0);
	ui->velocity->setMaximumValue(1500.0);
	ui->velocity->setValue(0.0);
	
	connect(ui->pwm, SIGNAL(valueChanged(double)),
		SLOT(pwmChanged(double)));
	connect(ui->velocity, SIGNAL(valueChanged(double)),
		SLOT(velChanged(double)));
	
	connect(ui->pwmStop, SIGNAL(clicked()), SLOT(stop()));
	connect(ui->velocityStop, SIGNAL(clicked()), SLOT(stop()));
	connect(ui->positionStop, SIGNAL(clicked()), SLOT(stop()));
	
	connect(ui->fd, SIGNAL(clicked()), SLOT(forward()));
	connect(ui->bk, SIGNAL(clicked()), SLOT(backward()));
	
	connect(ui->tabs, SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));
	
	connect(ui->go, SIGNAL(clicked()), SLOT(go()));
	
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), SLOT(update()));
	timer->start(100);
	
	ui->goalPos->setInputProvider(new NumpadDialog(tr("Goal Position"), NumpadDialog::Integer,
		std::numeric_limits<double>::min(), std::numeric_limits<double>::max(), this));
	ui->speed->setInputProvider(new NumpadDialog(tr("Speed"), NumpadDialog::Integer,
		-1500, 1500, this));
	
	ao();
#ifdef A_KOVAN
	publish();
#endif
}

CombinedMotorWidget::~CombinedMotorWidget()
{
	ao();
#ifdef A_KOVAN
	publish();
#endif
	delete ui;
}

void CombinedMotorWidget::pwmChanged(double pwm)
{
	ui->fd->setEnabled(pwm < 99.5);
	ui->bk->setEnabled(pwm > -99.5);
	ui->pwmStop->setEnabled(qAbs(pwm) > 0.5);
	motor(ui->motors->currentIndex(), pwm);
#ifdef A_KOVAN
	publish();
#endif
}

void CombinedMotorWidget::velChanged(double vel)
{
	ui->velocityStop->setEnabled(qAbs(vel) > 0.5);
	move_at_velocity(ui->motors->currentIndex(), vel);
#ifdef A_KOVAN
	publish();
#endif
}

void CombinedMotorWidget::tabChanged(int i)
{
	stop();
}

void CombinedMotorWidget::stop()
{
	ui->go->setEnabled(true);
	ui->pwm->setValue(0.0);
	ui->velocity->setValue(0.0);
	off(ui->motors->currentIndex());
	qDebug() << ui->motors->currentIndex();
#ifdef A_KOVAN
	publish();
#endif
}

void CombinedMotorWidget::go()
{
	move_to_position(ui->motors->currentIndex(),
		ui->speed->text().toInt(),
		ui->goalPos->text().toInt());
#ifdef A_KOVAN
	publish();
#endif
}

void CombinedMotorWidget::forward()
{
	ui->pwm->setValue(100.0);
}

void CombinedMotorWidget::backward()
{
	ui->pwm->setValue(-100.0);
}

void CombinedMotorWidget::update()
{
#ifdef A_KOVAN
	publish();
#endif
	int port = ui->motors->currentIndex();
	ui->position->setText(QString::number(get_motor_position_counter(port)));
	ui->positionStop->setEnabled(get_motor_done(port) ? true : false);
	ui->go->setEnabled(get_motor_done(port) ? false : true);
}