/*
	Copyright 2010-2014 David "Alemarius Nexus" Lerch

	This file is part of gtatools-gui.

	gtatools-gui is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	gtatools-gui is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with gtatools-gui.  If not, see <http://www.gnu.org/licenses/>.

	Additional permissions are granted, which are listed in the file
	GPLADDITIONS.
 */

#ifndef PVSDIALOG_H_
#define PVSDIALOG_H_

#include <QDialog>
#include <ui_PVSDialog.h>
#include <nxcommon/ProgressObserver.h>
#include "../Task.h"


class PVSDialog : public QDialog, public ProgressObserver {
	Q_OBJECT

public:
	PVSDialog(QWidget* parent);
	virtual void progressChanged(int value, int max);

private slots:
	void buttonClicked(QAbstractButton* button);

private:
	Ui_PVSDialog ui;
	Task* task;
};

#endif /* PVSDIALOG_H_ */
