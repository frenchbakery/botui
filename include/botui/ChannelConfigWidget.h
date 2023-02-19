#ifndef _CHANNELCONFIGWIDGET_H_
#define _CHANNELCONFIGWIDGET_H_

#include <QWidget>

#ifdef WALLABY
#include <wallaby/config.hpp>
#else
#include <kovan/config.hpp>
#endif


#include <QModelIndex>

class ChannelConfigWidget : public QWidget
{
Q_OBJECT
public:
	ChannelConfigWidget(const QModelIndex &index, QWidget *parent = 0);
	virtual ~ChannelConfigWidget();
	
	// needed for modern g++ to generate a vtable
	virtual int __dummy_vtable_fix_fn(int);

	Q_PROPERTY(Config config READ config WRITE setConfig NOTIFY configChanged)
	void setConfig(const Config &config);
	const Config &config() const;
	
	virtual void refresh() = 0;
	
signals:
	void configChanged(const QModelIndex &index, const Config &config);
	
private:
	Config m_config;
	QModelIndex m_index;
};

#endif
