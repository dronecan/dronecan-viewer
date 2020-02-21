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

#include "about_widget.hpp"
#include "can_monitor_widget.hpp"
#include "device_list_widget.hpp"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setDockNestingEnabled(true);

    initMenus();
    initWidgets();
    initSignalsSlots();

    setWindowTitle(tr("DroneCAN Viewer"));

    // Attempt to load workspace settings
    loadWorkspaceSettings(DroneCan::Directory::defaultWorkspaceFile());

    Debug(1, "MainWindow created");
}


MainWindow::~MainWindow()
{
    // TODO - Improve this debug message
    Debug(1, "~MainWindow destroyed");

    // Save the workpace settings
    saveWorkspaceSettings(DroneCan::Directory::defaultWorkspaceFile());

    delete ui;
}


void MainWindow::onClose()
{
    // TODO - Improve this debug message
    Debug(1, "MainWindow closed");

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


/**
 * @brief MainWindow::initSignalsSlots - Initialize and connect various signals / slots
 */
void MainWindow::initSignalsSlots()
{
    connect(ui->actionE_xit, SIGNAL(triggered()), this, SLOT(onClose()));

    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(showAboutInfo()));

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
                                                DroneCan::Directory::workspaceDirectory(),
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
                                                DroneCan::Directory::workspaceDirectory(),
                                                "(*.wsf)");

        // Still empty?
        if (filename.isEmpty())
        {
            return false;
        }
    }

    QSettings workspace(filename, QSettings::IniFormat);

    workspace.clear();

    workspace.beginGroup("application");
    // TODO - Set application version, etc
    workspace.endGroup(); // "application"

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


bool MainWindow::addDockedWidget(DockManager *mgr, QAction *action)
{
    if (!mgr)
    {
        // TODO - Fix this debug msg
        qDebug() << "null ptr supplied";

        return false;
    }

    QString name = mgr->objectName();

    // As widgets use name-based lookup, ensure there are no duplicates
    for (auto dock : dockedWidgets)
    {
        if (!dock) continue;

        if (dock->objectName() == name)
        {
            // TODO - Fix debug msg
            qDebug() << "Docked widget already exists with name" << name;
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
