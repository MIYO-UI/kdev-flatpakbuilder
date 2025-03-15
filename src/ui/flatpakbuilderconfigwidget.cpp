/**
 * @file flatpakbuilderconfigwidget.cpp
 * @brief Implementacja widgetu konfiguracyjnego dla wtyczki Flatpak Builder
 * @author Twoje Imię <twój@email.com>
 * @license GPL
 */

#include "flatpakbuilderconfigwidget.h"
#include "ui_flatpakbuilderconfigwidget.h"
#include "flatpakbuilderplugin.h"
#include "flatpakbuilderconfig.h"

#include <KLocalizedString>

#include <QFileDialog>
#include <QStandardPaths>

FlatpakBuilderConfigWidget::FlatpakBuilderConfigWidget(FlatpakBuilderPlugin* plugin, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FlatpakBuilderConfigWidget)
    , m_plugin(plugin)
    , m_config(plugin->config())
{
    ui->setupUi(this);
    
    // Połącz sygnały i sloty
    connect(ui->btnBrowseFlatpakBuilder, &QPushButton::clicked, 
            this, &FlatpakBuilderConfigWidget::slotBrowseFlatpakBuilder);
    
    connect(ui->btnBrowseFlatpak, &QPushButton::clicked, 
            this, &FlatpakBuilderConfigWidget::slotBrowseFlatpak);
    
    connect(ui->btnBrowseBuildDir, &QPushButton::clicked, 
            this, &FlatpakBuilderConfigWidget::slotBrowseBuildDir);
    
    // Inicjalizuj widget
    load();
}

FlatpakBuilderConfigWidget::~FlatpakBuilderConfigWidget()
{
    delete ui;
}

void FlatpakBuilderConfigWidget::save()
{
    // Zapisz ścieżki do narzędzi
    m_config->setFlatpakBuilderPath(ui->txtFlatpakBuilder->text());
    m_config->setFlatpakPath(ui->txtFlatpak->text());
    m_config->setDefaultBuildDir(ui->txtBuildDir->text());
    
    // Zapisz konfigurację
    m_config->save();
}

void FlatpakBuilderConfigWidget::load()
{
    // Wczytaj konfigurację
    m_config->load();
    
    // Ustaw pola formularza
    ui->txtFlatpakBuilder->setText(m_config->flatpakBuilderPath());
    ui->txtFlatpak->setText(m_config->flatpakPath());
    ui->txtBuildDir->setText(m_config->defaultBuildDir());
}

void FlatpakBuilderConfigWidget::defaults()
{
    // Znajdź domyślne ścieżki do narzędzi
    ui->txtFlatpakBuilder->setText(QStandardPaths::findExecutable("flatpak-builder"));
    ui->txtFlatpak->setText(QStandardPaths::findExecutable("flatpak"));
    ui->txtBuildDir->setText(QDir::homePath() + "/.cache/flatpak-builder");
}

void FlatpakBuilderConfigWidget::slotBrowseFlatpakBuilder()
{
    QString path = QFileDialog::getOpenFileName(this, 
                                              i18n("Select flatpak-builder executable"),
                                              QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).first(),
                                              i18n("All Files (*)"));
    
    if (!path.isEmpty()) {
        ui->txtFlatpakBuilder->setText(path);
    }
}

void FlatpakBuilderConfigWidget::slotBrowseFlatpak()
{
    QString path = QFileDialog::getOpenFileName(this, 
                                              i18n("Select flatpak executable"),
                                              QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).first(),
                                              i18n("All Files (*)"));
    
    if (!path.isEmpty()) {
        ui->txtFlatpak->setText(path);
    }
}

void FlatpakBuilderConfigWidget::slotBrowseBuildDir()
{
    QString path = QFileDialog::getExistingDirectory(this, 
                                                   i18n("Select build directory"),
                                                   ui->txtBuildDir->text());
    
    if (!path.isEmpty()) {
        ui->txtBuildDir->setText(path);
    }
}