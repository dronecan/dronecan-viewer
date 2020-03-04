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


#include <qmessagebox.h>

#include "msg_box.hpp"


/**
 * @brief InfoBox - Display a simple information box
 * @param title - Window title
 * @param message - Explanatory text
 * @param parent - parent object
 */
void InfoBox(QString title, QString message, QWidget *parent)
{
    QMessageBox box(parent);

    box.setWindowTitle(title);
    box.setText(message);

    box.setStandardButtons(QMessageBox::StandardButton::Ok);

    box.exec();
}


/**
 * @brief QuestionBox - Present a simple question box to the user
 * @param title - Window title
 * @param message - Explanatory text
 * @param parent - parent object
 * @return - true if the "OK" button was pressed
 */
bool QuestionBox(QString title, QString message, QWidget *parent)
{
    QMessageBox box(parent);

    box.setWindowTitle(title);
    box.setText(message);

    box.setStandardButtons(QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel);

    return box.exec() == QMessageBox::Ok;
}
