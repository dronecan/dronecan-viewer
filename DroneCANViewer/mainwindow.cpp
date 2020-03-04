/**

Copyright (c) 2020 DroneCAN

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

**/

#include <qfiledialog.h>
#include <qsettings.h>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "debug.hpp"
#include "adapter.hpp"
#include "directory.hpp"

#include "version.hpp"
#include "build_info.hpp"


#include "msg_box.hpp"
#include "can_connect_widget.hpp"
#include "about_widget.hpp"
#include "can_monitor_widget.hpp"
#include "device_list_widget.hpp"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setDockNestingEnabled(true);

    initCANInterface();

    initMenus();
    initWidgets();
    initSignalsSlots();
    initTimers();

    setWindowTitle(tr("DroneCAN Viewer"));

    // Load global settings
    loadGlobalSettings();

    // Attempt to load workspace settings
    loadWorkspaceSettings(DroneCAN::Directory::defaultWorkspaceFile());

    DCDebug << "MainWindow created";
}


MainWindow::~MainWindow()
{
    DCDebug << "~MainWindow destroyed";

    // Save the workpace settings
    saveWorkspaceSettings(DroneCAN::Directory::defaultWorkspaceFile());

    // Save global settings file
    saveGlobalSettings();

    // Stop the CAN interface
    canInterface->stop();
    canInterface->wait();

    delete ui;
}


void MainWindow::onClose()
{
    DCDebug << "MainWindow closed";

    close();
}


/**
 * @brief MainWindow::initMenus - Initialize menubar and menu items
 */
void MainWindow::initMenus()
{

}


void MainWindow::initWidgets()
{
    addDockedWidget(new CANMonitorWidget(this), ui->action_canviewer);

    addDockedWidget(new DeviceListWidget(this), ui->actionDevice_List);
}


void MainWindow::initTimers()
{
    // 4Hz generic update for all widgets
    widgetUpdateTimer = new QTimer(this);
    connect(widgetUpdateTimer, SIGNAL(timeout()), this, SLOT(updateDisplay()));
    widgetUpdateTimer->start(250);
}


void MainWindow::initCANInterface()
{
    canInterface = new DroneCANInterface(this);

    canInterface->start();
}


/**
 * @brief MainWindow::initSignalsSlots - Initialize and connect various signals / slots
 */
void MainWindow::initSignalsSlots()
{
    connect(ui->actionE_xit, SIGNAL(triggered()), this, SLOT(onClose()));

    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(showAboutInfo()));

    connect(ui->action_Connect, SIGNAL(triggered()), this, SLOT(connectCAN()));

    connect(ui->action_Load_Workspace, SIGNAL(triggered()), this, SLOT(loadWorkspace()));
    connect(ui->action_Save_Workspace, SIGNAL(triggered()), this, SLOT(saveWorkspace()));
}


/**
 * @brief MainWindow::showAboutInfo - Display a dialog box containing application version info
 */
void MainWindow::showAboutInfo()
{
    AboutWidget *about = new AboutWidget(this);

    about->setWindowModality(Qt::ApplicationModal);

    about->exec();
}


void MainWindow::connectCAN()
{
    if (!canInterface) return;

    if (!canInterface->isOpen())
    {
        CANConnectDialog dlg(this);

        if (dlg.exec() == QDialog::Accepted)
        {
            QString driver = dlg.getDriverName();
            QString device = dlg.getDeviceName();

            bool result = canInterface->open(driver, device);

            if (result)
            {

            }
            else
            {
                QString title = tr("Connection Error");
                QString msg = tr("Error connecting to CAN device") + QString(":\n");

                msg += canInterface->getErrorString();

                InfoBox(title, msg, this);
            }
        }
    }
    else
    {
        canInterface->close();
    }
}


void MainWindow::loadWorkspace()
{
    loadWorkspaceSettings();
}


/**
 * @brief MainWindow::loadWorkspaceSettings - Load workspace settings from external file
 * @param filename - If no filename is provided, user is presented with QFileDialog
 */
bool MainWindow::loadWorkspaceSettings(QString filename)
{
    if (filename.isEmpty())
    {
        // TODO - Better filtering
        filename = QFileDialog::getOpenFileName(this,
                                                tr("Load Workspace File"),
                                                DroneCAN::Directory::workspaceDirectory(),
                                                "(*.wsf)");
        // Still empty?
        if (filename.isEmpty())
        {
            return false;
        }
    }

    // Does the file exist?
    QFile f(filename);

    if (!f.exists())
    {
        return false;
    }

    QSettings workspace(filename, QSettings::IniFormat);

    workspace.beginGroup("workspace");

    // Extract workspace state
    if (workspace.contains("geometry"))
    {
        QVariant geom = workspace.value("geometry");
        restoreGeometry(geom.toByteArray());
    }

    if (workspace.contains("state"))
    {
        QVariant state = workspace.value("state");
        restoreState(state.toByteArray());
    }

    workspace.endGroup(); // "workspace"

    workspace.beginGroup("windows");

    for (auto *dock : dockedWidgets)
    {
        if (dock)
        {
            dock->loadSettings(workspace);

            dock->updateActionState();
        }
    }

    workspace.endGroup(); // "windows"

    return true;
}


/**
 * @brief MainWindow::saveBuildInfo - Encode application version to a settings file
 * @param settings
 */
void MainWindow::saveBuildInfo(QSettings &settings)
{
    settings.beginGroup("application");

    settings.setValue("version", DroneCAN::Version::version);

    settings.setValue("commit", DCV_BUILD_COMMIT_HASH);

    settings.endGroup();
}


void MainWindow::saveWorkspace()
{
    saveWorkspaceSettings();
}


/**
 * @brief MainWindow::saveWorkspaceSettings - Save workspace settings to external file
 * @param filename - If no filename is provided, user is presented with QFileDialog
 */
bool MainWindow::saveWorkspaceSettings(QString filename)
{
    if (filename.isEmpty())
    {
        filename = QFileDialog::getSaveFileName(this,
                                                tr("Save Workspace File"),
                                                DroneCAN::Directory::workspaceDirectory(),
                                                "(*.wsf)");

        // Still empty?
        if (filename.isEmpty())
        {
            return false;
        }
    }

    QSettings workspace(filename, QSettings::IniFormat);

    workspace.clear();

    saveBuildInfo(workspace);

    workspace.beginGroup("workspace");

        workspace.setValue("geometry", saveGeometry());
        workspace.setValue("state", saveState());

    workspace.endGroup(); // "workspace"

    workspace.beginGroup("windows");

    for (auto *dock : dockedWidgets)
    {
        if (dock)
        {
            dock->saveSettings(workspace);
        }
    }

    workspace.endGroup(); // "windows"

    return true;
}


void MainWindow::loadGlobalSettings()
{
    QSettings settings(DroneCAN::Directory::globalSettingsFile(), QSettings::IniFormat);

    if (canInterface)
    {
        canInterface->loadSettings(settings);
    }
}


void MainWindow::saveGlobalSettings()
{
    QSettings settings(DroneCAN::Directory::globalSettingsFile(), QSettings::IniFormat);

    settings.clear();

    saveBuildInfo(settings);

    if (canInterface)
    {
        canInterface->saveSettings(settings);
    }
}


bool MainWindow::addDockedWidget(DockManager *mgr, QAction *action)
{
    if (!mgr)
    {
        DCWarning << "nullptr supplied to addDockedWidget";

        return false;
    }

    QString name = mgr->objectName();

    // As widgets use name-based lookup, ensure there are no duplicates
    for (auto dock : dockedWidgets)
    {
        if (!dock) continue;

        if (dock->objectName() == name)
        {
            DCWarning << "Docked widget already exists with name" << name;
            return false;
        }
    }

    if (action)
    {
        if (!action->isCheckable())
        {
            action->setCheckable(true);
        }

        mgr->setToggleAction(action);
    }

    dockedWidgets.append(mgr);
    addDockWidget(Qt::LeftDockWidgetArea, mgr);

    mgr->updateActionState();

    return true;
}


bool MainWindow::addDockedWidget(QWidget *widget, QAction *action)
{
    auto* mgr = new DockManager(widget->windowTitle(), widget, this);

    return addDockedWidget(mgr, action);
}


bool MainWindow::removeDockedWidget(QString title)
{
    for (int i = 0; i < dockedWidgets.count(); i++)
    {
        if (dockedWidgets.at(i)->objectName() == title)
        {
            auto* mgr = dockedWidgets.at(i);

            dockedWidgets.removeAt(i);

            delete mgr;

            return true;
        }
    }

    return false;
}


void MainWindow::updateDisplay()
{
    // Update the window title
    QString title = "DroneCAN - " + canInterface->connectionString();

    setWindowTitle(title);

    if (canInterface->isOpen())
    {
        ui->action_Connect->setText(tr("Disconnect"));
    }
    else
    {
        ui->action_Connect->setText(tr("Connect"));
    }

    updateWidgets();
}


void MainWindow::updateWidgets()
{
    for (auto* dock : dockedWidgets)
    {
        if (!dock) continue;

        dock->refresh();
    }
}
