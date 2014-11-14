#include "createinvoicedialog.h"
#include "ui_createinvoicedialog.h"


#include "publics.h"

CreateInvoiceDialog::CreateInvoiceDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CreateInvoiceDialog)
{
	ui->setupUi(this);
}

CreateInvoiceDialog::~CreateInvoiceDialog()
{
	delete ui;
}

void CreateInvoiceDialog::startNew()
{
	QString unitSql = "SELECT * FROM unit WHERE Occupied = 'Yes'";
	Publics::loadQueryToCombo(unitSql, "UnitNo", ui->cboUnits);
}
