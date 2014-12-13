#include <QSettings>

#include "QGCCore.h"
#include "QGCSettingsWidget.h"
#include "MainWindow.h"
#include "ui_QGCSettingsWidget.h"

#include "LinkManager.h"
#include "MAVLinkProtocol.h"
#include "MAVLinkSettingsWidget.h"
#include "GAudioOutput.h"

//, Qt::WindowFlags flags

QGCSettingsWidget::QGCSettingsWidget(QWidget *parent, Qt::WindowFlags flags) :
    QDialog(parent, flags),
    ui(new Ui::QGCSettingsWidget)
{
    ui->setupUi(this);

    // Add all protocols
    QList<ProtocolInterface*> protocols = LinkManager::instance()->getProtocols();
    foreach (ProtocolInterface* protocol, protocols) {
        MAVLinkProtocol* mavlink = dynamic_cast<MAVLinkProtocol*>(protocol);
        if (mavlink) {
            MAVLinkSettingsWidget* msettings = new MAVLinkSettingsWidget(mavlink, this);
            ui->tabWidget->addTab(msettings, "MAVLink");
        }
    }

    this->window()->setWindowTitle(tr("QGroundControl Settings"));


    QString langPath = QGCCore::getLangFilePath();
    foreach (QString lang, QGCCore::availableLanguages()) {
        QLocale locale(lang);
        QString name = QLocale::languageToString(locale.language());
        QIcon ico(QString("%1/flags/%2.png").arg(langPath).arg(lang));
        ui->comboBox_language->addItem(ico, name, lang);
    }

    // Audio preferences
    ui->audioMuteCheckBox->setChecked(GAudioOutput::instance()->isMuted());
    connect(ui->audioMuteCheckBox, SIGNAL(toggled(bool)), GAudioOutput::instance(), SLOT(mute(bool)));
    connect(GAudioOutput::instance(), SIGNAL(mutedChanged(bool)), ui->audioMuteCheckBox, SLOT(setChecked(bool)));

    // Reconnect
    ui->reconnectCheckBox->setChecked(MainWindow::instance()->autoReconnectEnabled());
    connect(ui->reconnectCheckBox, SIGNAL(clicked(bool)), MainWindow::instance(), SLOT(enableAutoReconnect(bool)));

    // Low power mode
    ui->lowPowerCheckBox->setChecked(MainWindow::instance()->lowPowerModeEnabled());
    connect(ui->lowPowerCheckBox, SIGNAL(clicked(bool)), MainWindow::instance(), SLOT(enableLowPowerMode(bool)));

    // Language
    ui->comboBox_language->setCurrentIndex(ui->comboBox_language->findData(MainWindow::instance()->getCurrentLanguage()));
    connect(ui->comboBox_language, SIGNAL(currentIndexChanged(int)), this, SLOT(loadLanguage(int)));

    // Style
    ui->comboBox_style->addItems(MainWindow::instance()->getAvailableStyles());
    ui->comboBox_style->setCurrentIndex(ui->comboBox_style->findText(MainWindow::instance()->getStyleName()));
    connect(ui->comboBox_style, SIGNAL(currentIndexChanged(QString)), this, SLOT(loadStyle(QString)));

    // Close / destroy
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(deleteLater()));

    // Set layout options
    ui->generalPaneGridLayout->setAlignment(Qt::AlignTop);
}

QGCSettingsWidget::~QGCSettingsWidget()
{
    delete ui;
}

void QGCSettingsWidget::loadLanguage(int idx) {
    MainWindow::instance()->loadLanguage(ui->comboBox_language->itemData(idx).toString());
}

void QGCSettingsWidget::loadStyle(QString style) {
    MainWindow::instance()->loadStyleByName(style);
    if (MainWindow::instance()->getStyleIdByName(style) == MainWindow::QGC_MAINWINDOW_STYLE_NATIVE)
        MainWindow::instance()->showInfoMessage(tr("Please restart QGroundControl"), tr("Please restart QGroundControl to switch to fully native look and feel. Currently you have loaded Qt's default Plastique style."));
}
