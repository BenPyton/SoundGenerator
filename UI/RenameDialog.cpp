/*
 * Copyright 2021 Benoit Pelletier
 *
 * This file is part of Sound Generator.
 *
 * Sound Generator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sound Generator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sound Generator.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "RenameDialog.h"
#include "ui_renamedialog.h"

RenameDialog::RenameDialog(QString _oldName, QWidget* _parent) :
    QDialog(_parent),
    ui(new Ui::RenameDialog)
{
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->setupUi(this);
    ui->led_newname->setText(_oldName);

    QPushButton* applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    assert(applyButton != nullptr);
    applyButton->setAutoDefault(false);
    applyButton->setDefault(false);
    connect(applyButton, &QPushButton::clicked, this, &RenameDialog::onApply);

    QPushButton* cancelButton = ui->buttonBox->button(QDialogButtonBox::Cancel);
    assert(cancelButton != nullptr);
    cancelButton->setAutoDefault(false);
    cancelButton->setDefault(false);
    connect(cancelButton, &QPushButton::clicked, this, &RenameDialog::onCancel);
}

RenameDialog::~RenameDialog()
{
    delete ui;
}

QString RenameDialog::getNewName()
{
    return ui->led_newname->text();
}

void RenameDialog::onApply()
{
    done(ReturnCode::APPLY);
}

void RenameDialog::onCancel()
{
    done(ReturnCode::CANCEL);
}

