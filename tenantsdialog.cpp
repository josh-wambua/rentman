#include "tenantsdialog.h"
#include "ui_tenantsdialog.h"

#include <QtSql>
#include <QTreeWidgetItem>
#include <QInputDialog>
#include "publics.h"

TenantsDialog::TenantsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TenantsDialog)
{
	ui->setupUi(this);

	connect (ui->cmdAddNew, SIGNAL(clicked()), SLOT(newTenant()));
	connect (ui->cmdDelete, SIGNAL(clicked()), SLOT(deleteTenant()));
	connect (ui->cmdSave, SIGNAL(clicked()), SLOT(saveChanges()));
	connect (ui->cmdExit, SIGNAL(clicked()), this, SLOT(reject()));
}

TenantsDialog::~TenantsDialog()
{
	delete ui;
}

void TenantsDialog::reloadTenants()
{
	ui->trvTenants->invisibleRootItem()->takeChildren();
	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM tenant");
	QString id;
	while (qu.next()) {
		QTreeWidgetItem *it = new QTreeWidgetItem(ui->trvTenants);
		it->setText(0, qu.record().value("Name").toString());
		it->setText(99, qu.record().value("TenantID").toString());
		id = it->text(99);
	}

	if (id.length() > 0)
	{
		m_tenantID = id;
		editTenant();
	}
}

void TenantsDialog::editTenant()
{
	Publics::clearTextBoxes(this);
	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM tenant  WHERE TenantID = '"
						     + m_tenantID + "'");
	qu.first();

	ui->txtName->setText(qu.record().value("Name").toString());
	Publics::setComboBoxText(ui->cboSex, qu.record().value("Sex").toString());
	ui->txtEmail->setText(qu.record().value("Email").toString());
	ui->txtTel->setText(qu.record().value("Tel").toString());
}

void TenantsDialog::newTenant()
{
	QString newTenantName = QInputDialog::getText(this, "New Tenant", "Enter new tenant name");
	if (newTenantName.length() > 0) {
		QSqlQuery qu2 = QSqlDatabase::database().exec("INSERT INTO tenant (Name) VALUES ('" + newTenantName + "')");
		m_tenantID = qu2.lastInsertId().toString();
		reloadTenants();
		editTenant();
	}
}

void TenantsDialog::saveChanges()
{
	QString sql = "UPDATE tenant SET "
		      "Name = '" + ui->txtName->text() + "', "
		"Sex = '" + ui->cboSex->currentText() + "', "
		"Email = '" + ui->txtEmail->text() + "', "
		"Tel = '" + ui->txtTel->text() + "' WHERE "
		"TenantID = '" + m_tenantID + "'";

	QSqlDatabase::database().exec(sql);
	reloadTenants();
}

void TenantsDialog::deleteTenant()
{
	if (QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this tenant?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
		QSqlDatabase::database().exec("DELETE FROM tenant WHERE TenantID = '" + m_tenantID + "'");
		m_tenantID = "";
		Publics::clearTextBoxes(this);
		reloadTenants();
	}
}

void TenantsDialog::tenantChanged(QTreeWidgetItem *item, int col)
{
	Q_UNUSED(col);
	if (m_tenantID.length() > 0)
		saveChanges();

	QString c_id = item->text(99);
	m_tenantID = c_id;
	editTenant();
}