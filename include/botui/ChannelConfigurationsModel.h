#ifndef _CHANNELCONFIGURATIONSMODEL_H_
#define _CHANNELCONFIGURATIONSMODEL_H_

#include <QFileSystemModel>

class ChannelConfigurationsModel : public QFileSystemModel
{
Q_OBJECT
public:
        ChannelConfigurationsModel(QObject *parent = 0);
        ~ChannelConfigurationsModel();
        
        // needed for modern compiler to generate vtable
        virtual int __dummy_vtable_fix_fn(int);

        QModelIndex defaultConfiguration() const;
private:
        QFileIconProvider *m_iconProvider;
};

#endif
