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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "debug.hpp"
#include "adapter.hpp"
#include "directory.hpp"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initMenus();
    initSignalsSlots();

    setWindowTitle(tr("DroneCAN Viewer"));

    // Attempt to load workspace settings
    loadWorkspaceSettings(DroneCan::Directory::defaultWorkspaceFile());
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


/**
 * @brief MainWindow::initSignalsSlots - Initialize and connect various signals / slots
 */
void MainWindow::initSignalsSlots()
{
    connect(ui->actionE_xit, SIGNAL(triggered()), this, SLOT(onClose()));

    connect(ui->action_Load_Workspace, SIGNAL(triggered()), this, SLOT(loadWorkspace()));
    connect(ui->action_Save_Workspace, SIGNAL(triggered()), this, SLOT(saveWorkspace()));
}


/**
 * @brief MainWindow::showAboutInfo - Display a dialog box containing application version info
 */
void MainWindow::showAboutInfo()
{

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

    // TODO - Set application version, etc

    workspace.beginGroup("workspace");

        workspace.setValue("geometry", saveGeometry());
        workspace.setValue("state", saveState());

    workspace.endGroup(); // "workspace"

    return true;
}
