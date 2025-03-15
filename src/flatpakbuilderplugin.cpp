/**
 * @file flatpakbuilderplugin.cpp
 * @brief Implementacja głównej klasy wtyczki Flatpak Builder
 * @author Twoje Imię <twój@email.com>
 * @license GPL
 */

#include "flatpakbuilderplugin.h"
#include "flatpakbuilderconfig.h"
#include "flatpakmanifestmanager.h"
#include "flatpakbuilderjob.h"

#include <interfaces/icore.h>
#include <interfaces/iuicontroller.h>
#include <interfaces/iproject.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/idocumentcontroller.h>
#include <project/projectmodel.h>

#include <KPluginFactory>
#include <KLocalizedString>
#include <KActionCollection>
#include <KMessageBox>

#include <QAction>
#include <QUrl>

K_PLUGIN_FACTORY_WITH_JSON(FlatpakBuilderFactory, "kdevflatpakbuilder.json", registerPlugin<FlatpakBuilderPlugin>();)

FlatpakBuilderPlugin::FlatpakBuilderPlugin(QObject* parent, const QVariantList& args)
    : KDevelop::IPlugin("kdevflatpakbuilder", parent)
    , m_config(new FlatpakBuilderConfig(this))
    , m_manifestManager(new FlatpakManifestManager(this))
{
    Q_UNUSED(args);
    
    setXMLFile("kdevflatpakbuilder.rc");
    
    setupActions();
}

FlatpakBuilderPlugin::~FlatpakBuilderPlugin()
{
}

QString FlatpakBuilderPlugin::name() const
{
    return i18n("Flatpak Builder");
}

void FlatpakBuilderPlugin::setupActions()
{
    // Akcja Build Flatpak
    m_buildAction = new QAction(QIcon::fromTheme("flatpak-build"), i18n("Build Flatpak"), this);
    connect(m_buildAction, &QAction::triggered, this, &FlatpakBuilderPlugin::slotBuildFlatpak);
    actionCollection()->addAction("flatpak_build", m_buildAction);
    
    // Akcja Install Flatpak
    m_installAction = new QAction(QIcon::fromTheme("flatpak-install"), i18n("Install Flatpak"), this);
    connect(m_installAction, &QAction::triggered, this, &FlatpakBuilderPlugin::slotInstallFlatpak);
    actionCollection()->addAction("flatpak_install", m_installAction);
    
    // Akcja Export Bundle
    m_exportBundleAction = new QAction(QIcon::fromTheme("flatpak-export"), i18n("Export Bundle"), this);
    connect(m_exportBundleAction, &QAction::triggered, this, &FlatpakBuilderPlugin::slotExportBundle);
    actionCollection()->addAction("flatpak_export_bundle", m_exportBundleAction);
    
    // Akcja Create Manifest
    m_createManifestAction = new QAction(QIcon::fromTheme("document-new"), i18n("Create Manifest"), this);
    connect(m_createManifestAction, &QAction::triggered, this, &FlatpakBuilderPlugin::slotCreateManifest);
    actionCollection()->addAction("flatpak_create_manifest", m_createManifestAction);
    
    // Akcja Edit Manifest
    m_editManifestAction = new QAction(QIcon::fromTheme("document-edit"), i18n("Edit Manifest"), this);
    connect(m_editManifestAction, &QAction::triggered, this, &FlatpakBuilderPlugin::slotEditManifest);
    actionCollection()->addAction("flatpak_edit_manifest", m_editManifestAction);
}

bool FlatpakBuilderPlugin::hasManifest(KDevelop::IProject* project) const
{
    return m_manifestManager->hasManifest(project);
}

void FlatpakBuilderPlugin::createManifest(KDevelop::IProject* project)
{
    m_manifestManager->createManifest(project);
}

void FlatpakBuilderPlugin::editManifest(KDevelop::IProject* project)
{
    QUrl manifestUrl = m_manifestManager->manifestUrl(project);
    if (manifestUrl.isValid()) {
        core()->documentController()->openDocument(manifestUrl);
    } else {
        KMessageBox::error(core()->uiController()->activeMainWindow(),
                          i18n("Could not find Flatpak manifest file for this project."),
                          i18n("Flatpak Builder"));
    }
}

KJob* FlatpakBuilderPlugin::build(KDevelop::ProjectBaseItem* item)
{
    KDevelop::IProject* project = item->project();
    
    if (!hasManifest(project)) {
        int response = KMessageBox::questionYesNo(
            core()->uiController()->activeMainWindow(),
            i18n("No Flatpak manifest found for this project. Would you like to create one?"),
            i18n("Flatpak Builder")
        );
        
        if (response == KMessageBox::Yes) {
            createManifest(project);
            return nullptr; // Wróć po utworzeniu manifestu
        } else {
            return nullptr;
        }
    }
    
    QString manifestPath = m_manifestManager->manifestUrl(project).toLocalFile();
    
    FlatpakBuilderJob* job = new FlatpakBuilderJob(this, project, FlatpakBuilderJob::BuildOperation);
    job->setManifestPath(manifestPath);
    
    return job;
}

KJob* FlatpakBuilderPlugin::install(KDevelop::ProjectBaseItem* item)
{
    KDevelop::IProject* project = item->project();
    
    if (!hasManifest(project)) {
        KMessageBox::error(
            core()->uiController()->activeMainWindow(),
            i18n("No Flatpak manifest found for this project. Build the project first."),
            i18n("Flatpak Builder")
        );
        return nullptr;
    }
    
    QString manifestPath = m_manifestManager->manifestUrl(project).toLocalFile();
    
    FlatpakBuilderJob* job = new FlatpakBuilderJob(this, project, FlatpakBuilderJob::InstallOperation);
    job->setManifestPath(manifestPath);
    
    return job;
}

KJob* FlatpakBuilderPlugin::exportBundle(KDevelop::IProject* project)
{
    if (!hasManifest(project)) {
        KMessageBox::error(
            core()->uiController()->activeMainWindow(),
            i18n("No Flatpak manifest found for this project. Build the project first."),
            i18n("Flatpak Builder")
        );
        return nullptr;
    }
    
    QString manifestPath = m_manifestManager->manifestUrl(project).toLocalFile();
    
    FlatpakBuilderJob* job = new FlatpakBuilderJob(this, project, FlatpakBuilderJob::ExportOperation);
    job->setManifestPath(manifestPath);
    
    return job;
}

FlatpakBuilderConfig* FlatpakBuilderPlugin::config() const
{
    return m_config;
}

void FlatpakBuilderPlugin::slotBuildFlatpak()
{
    KDevelop::IProject* project = core()->projectController()->activeProject();
    if (!project) {
        return;
    }
    
    auto job = build(project->projectItem());
    if (job) {
        job->start();
    }
}

void FlatpakBuilderPlugin::slotInstallFlatpak()
{
    KDevelop::IProject* project = core()->projectController()->activeProject();
    if (!project) {
        return;
    }
    
    auto job = install(project->projectItem());
    if (job) {
        job->start();
    }
}

void FlatpakBuilderPlugin::slotExportBundle()
{
    KDevelop::IProject* project = core()->projectController()->activeProject();
    if (!project) {
        return;
    }
    
    auto job = exportBundle(project);
    if (job) {
        job->start();
    }
}

void FlatpakBuilderPlugin::slotCreateManifest()
{
    KDevelop::IProject* project = core()->projectController()->activeProject();
    if (project) {
        createManifest(project);
    }
}

void FlatpakBuilderPlugin::slotEditManifest()
{
    KDevelop::IProject* project = core()->projectController()->activeProject();
    if (project) {
        editManifest(project);
    }
}

#include "flatpakbuilderplugin.moc"