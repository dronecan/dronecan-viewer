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

#include <qdebug.h>

#include "dc_widget.hpp"


void DCWidget::loadSettings(QSettings &settings)
{
    Q_UNUSED(settings);
}


void DCWidget::saveSettings(QSettings &settings)
{
    Q_UNUSED(settings);
}


void DCWidget::updateDisplay()
{

}


void DCWidget::refresh()
{
    if (isVisible())
    {
        updateDisplay();
    }
}


DockManager::DockManager(const QString &title, QWidget *widget, QWidget *parent, Qt::WindowFlags flags) :
    QDockWidget(title, parent, flags)
{
    setObjectName(title);
    setWidget(widget);

    // Prevent deletion when the widget closes
    setAttribute(Qt::WA_DeleteOnClose, false);

    DCDebug << "creating new DockManager -" << title;
}


DockManager::~DockManager()
{
    DCDebug << "destroying DockManager -" << objectName();
}


DCWidget* DockManager::getDCWidget()
{
    return dynamic_cast<DCWidget*>(widget());
}


void DockManager::refresh()
{
    if (isVisible())
    {
        auto *w = getDCWidget();

        if (w)
        {
            w->refresh();
        }
    }
}


void DockManager::saveSettings(QSettings &settings)
{
    auto *w = getDCWidget();

    if (w)
    {
        w->saveSettings(settings);
    }
}


void DockManager::loadSettings(QSettings &settings)
{
    auto *w = getDCWidget();

    if (w)
    {
        w->loadSettings(settings);
    }
}


/**
 * @brief DockManager::closeEvent - Called when the dock is 'closed'. Instead of closing, simply hide it
 * @param event
 */
void DockManager::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    if (toggleAction && toggleAction->isCheckable())
    {
        toggleAction->blockSignals(true);
        toggleAction->setChecked(false);
        toggleAction->blockSignals(false);
    }
}


void DockManager::setToggleAction(QAction *action)
{
    // If an action is already assigned, disconnect it
    if (toggleAction)
    {
        disconnect(toggleAction, SIGNAL(toggled(bool)), this, SLOT(setVisible(bool)));
    }

    if (action && action->isCheckable())
    {
        toggleAction = action;
        connect(action, SIGNAL(toggled(bool)), this, SLOT(setVisible(bool)));

        setVisible(action->isChecked());

        updateActionState();
    }
}


void DockManager::updateActionState()
{
    if (toggleAction)
    {
        toggleAction->blockSignals(true);
        toggleAction->setChecked(isVisible());
        toggleAction->blockSignals(false);
    }
}
