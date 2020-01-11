#include "CameraLiveWidget.h"
#include "kipr/camera.h"

#include <QDebug>
#include <QStringListModel>

#include "ui_CameraLiveWidget.h"
#include "Device.h"
#include "ChannelConfigurationsModel.h"
#include "SettingsProvider.h"

CameraLiveWidget::CameraLiveWidget(Device *device, QWidget *parent)
  : StandardWidget(device, parent),
  ui(new Ui::CameraLiveWidget),
  m_model(new ChannelConfigurationsModel(this))
{
  ui->setupUi(this);
  performStandardSetup(tr("Camera"), false);
  
  ui->config->setModel(m_model);
  ui->config->setRootModelIndex(m_model->index(m_model->rootPath()));
  ui->config->setCurrentIndex(m_model->defaultConfiguration().row());
  
  ui->comboBox->addItem("0");
  ui->comboBox->addItem("1");
  ui->comboBox->addItem("2");
  ui->comboBox->addItem("3");

  
  connect(ui->config, SIGNAL(currentIndexChanged(int)), SLOT(currentIndexChanged(int)));
  
  const SettingsProvider *const settingsProvider = device->settingsProvider();
  if(settingsProvider) {
    const bool showBbox = settingsProvider->value("bounding_box", true).toBool();
    const int numLabels = settingsProvider->value("num_blob_labels", 0).toInt();
    ui->camera->setShowBbox(showBbox);
    ui->camera->setNumBlobLabels(numLabels);
  }

  QTimer *timer = new QTimer(this);
  ui->ValueUpdates->connect(timer, SIGNAL(timeout()), SLOT(update()));
  timer->start(200);
}

CameraLiveWidget::~CameraLiveWidget()
{
  delete ui;
}

void CameraLiveWidget::update()
{
	int value = 0;
	value = ui->comboBox->currentIndex();
	QString values = QString::number(value);
  	ui->ValueUpdates->setText(values);
	ui->ValueUpdates->show();
}

void CameraLiveWidget::currentIndexChanged(const int &index)
{
  const QString path = m_model->filePath(m_model->index(index,
    0, ui->config->rootModelIndex()));
  Config *conf = Config::load(path.toStdString());
  ui->camera->setConfig(conf);
  delete conf;
}
