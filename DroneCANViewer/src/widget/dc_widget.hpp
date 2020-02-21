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

#ifndef DC_WIDGET_HPP
#define DC_WIDGET_HPP

#include <qwidget.h>
#include <qsettings.h>
#include <qdockwidget.h>
#include <QCloseEvent>
#include <QAction>


/**
 * @brief The DCWidget class is a wrapper around QWidget that provides some common functions
 */
class DCWidget : public QWidget
{
    Q_OBJECT

public:
    DCWidget(QString name, QWidget *parent = nullptr) : QWidget(parent)
    {
        setObjectName(name);
    }

    virtual ~DCWidget() {}

    virtual void loadSettings(QSettings& settings);
    virtual void saveSettings(QSettings& settings);

public slots:
    void refresh();

protected:
    virtual void updateDisplay();
};


/**
 * @brief The DockManager class is a wrapper class for a QDockWidget which provides extra functionality
 */
class DockManager : public QDockWidget
{
    Q_OBJECT

public:
    DockManager(const QString& title, QWidget* widget, QWidget* parent, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DockManager();

    void saveSettings(QSettings& settings);
    void loadSettings(QSettings& settings);

    void setToggleAction(QAction* action);

public slots:
    void refresh();
    void updateActionState();

protected:
    void closeEvent(QCloseEvent* event) override;

    QAction* toggleAction = nullptr;

    DCWidget* getDCWidget();
};


#endif // DC_WIDGET_HPP
