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

#include "about_widget.hpp"
#include "build_info.hpp"
#include "version.hpp"
#include "DroneCANProtocol.h"


AboutWidget::AboutWidget(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);

    setWindowTitle(tr("DroneCAN Viewer"));

    setAttribute(Qt::WA_DeleteOnClose, true);

    // Format the build date
    QString bd = QString::number(DCV_BUILD_YEAR) + "-" +
                 QString::number(DCV_BUILD_MONTH) + "-" +
                 QString::number(DCV_BUILD_DAY);

    ui.buildDate->setText(bd);

    ui.commitHash->setText(DCV_BUILD_COMMIT_HASH);

    ui.version->setText(DroneCAN::Version::version);

    ui.qtVersion->setText(qVersion());
    ui.protocolVersion->setText(getDroneCANVersion());

    // Set the URL links
    ui.githubLabel->setTextFormat(Qt::RichText);
    ui.githubLabel->setOpenExternalLinks(true);
    ui.githubLabel->setText("<a href='https://github.com/dronecan'>github.com/dronecan</a>");

    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    connect(ui.showReleaseNotes, SIGNAL(released()), this, SLOT(showReleaseNotes()));
}


void AboutWidget::showReleaseNotes()
{
    // TODO - Show the release notes (open in external editor?)
}
