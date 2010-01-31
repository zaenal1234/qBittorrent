#ifndef ADVANCEDSETTINGS_H
#define ADVANCEDSETTINGS_H

#include <QTableWidget>
#include <QHeaderView>
#include <QSpinBox>
#include <QCheckBox>
#include "preferences.h"

enum AdvSettingsCols {PROPERTY, VALUE};
enum AdvSettingsRows {DISK_CACHE, OUTGOING_PORT_MIN, OUTGOING_PORT_MAX, IGNORE_LIMIT_LAN, COUNT_OVERHEAD, RECHECK_COMPLETED, LIST_REFRESH };
#define ROW_COUNT 7

class AdvancedSettings: public QTableWidget {
  Q_OBJECT

private:
  QSpinBox *spin_cache, *outgoing_ports_min, *outgoing_ports_max, *spin_list_refresh;
  QCheckBox *cb_ignore_limits_lan, *cb_count_overhead, *cb_recheck_completed;

public:
  AdvancedSettings(QWidget *parent=0): QTableWidget(parent) {
    // Set visual appearance
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setColumnCount(2);
    QStringList header;
    header << tr("Property") << tr("Value");
    setHorizontalHeaderLabels(header);
    setColumnWidth(0, width()/2);
    horizontalHeader()->setStretchLastSection(true);
    setRowCount(ROW_COUNT);
    // Load settings
    loadAdvancedSettings();
  }

  ~AdvancedSettings() {
    delete spin_cache;
    delete outgoing_ports_min;
    delete outgoing_ports_max;
    delete cb_ignore_limits_lan;
    delete cb_count_overhead;
    delete cb_recheck_completed;
    delete spin_list_refresh;
  }

public slots:
  void saveAdvancedSettings() {
    // Disk write cache
    Preferences::setDiskCacheSize(spin_cache->value());
    // Outgoing ports
    Preferences::setOutgoingPortsMin(outgoing_ports_min->value());
    Preferences::setOutgoingPortsMax(outgoing_ports_max->value());
    // Ignore limits on LAN
    Preferences::ignoreLimitsOnLAN(cb_ignore_limits_lan->isChecked());
    // Include protocol overhead in transfer limits
    Preferences::includeOverheadInLimits(cb_count_overhead->isChecked());
    // Recheck torrents on completion
    Preferences::recheckTorrentsOnCompletion(cb_recheck_completed->isChecked());
    // Transfer list refresh interval
    Preferences::setRefreshInterval(spin_list_refresh->value());
  }

protected slots:
  void loadAdvancedSettings() {
    // Disk write cache
    setItem(DISK_CACHE, PROPERTY, new QTableWidgetItem(tr("Disk write cache size (MiB)")));
    spin_cache = new QSpinBox();
    connect(spin_cache, SIGNAL(valueChanged(int)), this, SLOT(emitSettingsChanged()));
    spin_cache->setMinimum(1);
    spin_cache->setMaximum(200);
    spin_cache->setValue(Preferences::diskCacheSize());
    setCellWidget(DISK_CACHE, VALUE, spin_cache);
    // Outgoing port Min
    setItem(OUTGOING_PORT_MIN, PROPERTY, new QTableWidgetItem(tr("Outgoing ports (Min)")));
    outgoing_ports_min = new QSpinBox();
    connect(outgoing_ports_min, SIGNAL(valueChanged(int)), this, SLOT(emitSettingsChanged()));
    outgoing_ports_min->setMinimum(0);
    outgoing_ports_min->setMaximum(65535);
    outgoing_ports_min->setValue(Preferences::outgoingPortsMin());
    setCellWidget(OUTGOING_PORT_MIN, VALUE, outgoing_ports_min);
    // Outgoing port Min
    setItem(OUTGOING_PORT_MAX, PROPERTY, new QTableWidgetItem(tr("Outgoing ports (Max)")));
    outgoing_ports_max = new QSpinBox();
    connect(outgoing_ports_max, SIGNAL(valueChanged(int)), this, SLOT(emitSettingsChanged()));
    outgoing_ports_max->setMinimum(0);
    outgoing_ports_max->setMaximum(65535);
    outgoing_ports_max->setValue(Preferences::outgoingPortsMax());
    setCellWidget(OUTGOING_PORT_MAX, VALUE, outgoing_ports_max);
    // Ignore transfer limits on local network
    setItem(IGNORE_LIMIT_LAN, PROPERTY, new QTableWidgetItem(tr("Ignore transfer limits on local network")));
    cb_ignore_limits_lan = new QCheckBox();
    connect(cb_ignore_limits_lan, SIGNAL(toggled(bool)), this, SLOT(emitSettingsChanged()));
    cb_ignore_limits_lan->setChecked(Preferences::ignoreLimitsOnLAN());
    setCellWidget(IGNORE_LIMIT_LAN, VALUE, cb_ignore_limits_lan);
    // Consider protocol overhead in transfer limits
    setItem(COUNT_OVERHEAD, PROPERTY, new QTableWidgetItem(tr("Include TCP/IP overhead in transfer limits")));
    cb_count_overhead = new QCheckBox();
    connect(cb_count_overhead, SIGNAL(toggled(bool)), this, SLOT(emitSettingsChanged()));
    cb_count_overhead->setChecked(Preferences::includeOverheadInLimits());
    setCellWidget(COUNT_OVERHEAD, VALUE, cb_count_overhead);
    // Recheck completed torrents
    setItem(RECHECK_COMPLETED, PROPERTY, new QTableWidgetItem(tr("Recheck torrents on completion")));
    cb_recheck_completed = new QCheckBox();
    connect(cb_recheck_completed, SIGNAL(toggled(bool)), this, SLOT(emitSettingsChanged()));
    cb_recheck_completed->setChecked(Preferences::recheckTorrentsOnCompletion());
    setCellWidget(RECHECK_COMPLETED, VALUE, cb_recheck_completed);
    // Transfer list refresh interval
    setItem(LIST_REFRESH, PROPERTY, new QTableWidgetItem(tr("Transfer list refresh interval (ms)")));
    spin_list_refresh = new QSpinBox();
    connect(spin_list_refresh, SIGNAL(valueChanged(int)), this, SLOT(emitSettingsChanged()));
    spin_list_refresh->setMinimum(30);
    spin_list_refresh->setMaximum(99999);
    spin_list_refresh->setValue(Preferences::getRefreshInterval());
    setCellWidget(LIST_REFRESH, VALUE, spin_list_refresh);
  }

  void emitSettingsChanged() {
    emit settingsChanged();
  }

signals:
  void settingsChanged();
};

#endif // ADVANCEDSETTINGS_H