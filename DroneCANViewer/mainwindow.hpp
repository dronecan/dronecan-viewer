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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qmainwindow.h>
#include <qlist.h>
#include <qtimer.h>


#include "adapter.hpp"
#include "dc_widget.hpp"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onClose();

    void loadWorkspace();
    void saveWorkspace();

    void showAboutInfo();

    void connectCAN();

protected slots:
    void updateWidgets();

protected:
    Ui::MainWindow *ui = nullptr;

    void initMenus();
    void initSignalsSlots();
    void initWidgets();
    void initTimers();
    void initCANInterface();

    void saveBuildInfo(QSettings &settings);

    //! Settings particular to a particular workspace layout
    bool loadWorkspaceSettings(QString filename = QString());
    bool saveWorkspaceSettings(QString filename = QString());

    //! Global settings (independent of workspace)
    void loadGlobalSettings(void);
    void saveGlobalSettings(void);

    bool addDockedWidget(DockManager *mgr, QAction *action = nullptr);
    bool addDockedWidget(QWidget *widget, QAction *action = nullptr);

    bool removeDockedWidget(QString title);

    QList<DockManager*> dockedWidgets;

    QTimer *widgetUpdateTimer = nullptr;

    DroneCANInterface* canInterface = nullptr;
};

#endif // MAINWINDOW_H
