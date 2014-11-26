#ifndef RENTMANAGERMAINWINDOW_H
#define RENTMANAGERMAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtXml>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>

class QTreeWidgetItem;

class QActionGroup;
class CompaniesDialog;
class PropertiesDialog;
class TenantsDialog;
class AssignUnitToTenantDialog;
class PaymentsDialog;
class MyPrinter;
class CreateInvoiceDialog;
class CreateMultiInvoice;
class SmartPaymentDialog;
class QPrinter;
class QListWidgetItem;
class DepositDialog;
class CustomerAccountDialog;

namespace Ui {
class RentManagerMainWindow;
}

class RentManagerMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit RentManagerMainWindow(QWidget *parent = 0);
	~RentManagerMainWindow();
	static RentManagerMainWindow *instance();

	void showQueryError(QSqlQuery qu, QString title = "Error", QString textBefore = "Ther following error has occured:", QString textAfter = "Please contact your administrator.");

	//Dialogs
	CompaniesDialog *m_companiesDialog;
	PropertiesDialog *m_propertiesDialog;
	TenantsDialog *m_tenants;
	AssignUnitToTenantDialog *m_assign;
	PaymentsDialog *m_payDiag;
	CreateInvoiceDialog *m_singleInvoice;
	CreateMultiInvoice *m_multiInvoice;
	SmartPaymentDialog *m_smartPayment;
	DepositDialog	 *m_deposits;
	CustomerAccountDialog *m_custAccount;

	MyPrinter *m_printer;
public slots:
	void startOpenFile();
	void closeFile();
	void reloadBrowser();
private:
	Ui::RentManagerMainWindow *ui;
	QSqlDatabase db;
	QString currentFilePath;
	void loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	QString curFile;
	QString monthName;
	QString yearName;
	void updateRecentFileActions();
	enum { MaxRecentFiles = 5 };
	QAction *recentFileActs[MaxRecentFiles];
	QActionGroup *actionsToDisable;
	bool initializeCompanyFile();
	static RentManagerMainWindow *m_instance;
	QString currentProperty;

	QPrinter *printer;
	QString m_reportName;
	QString rptName;
	QString m_xml;
	QStringList m_paramsToReplace;
	QStringList m_paramReplacements;
	QDateEdit *rpt_dtpReportQuery;
	QComboBox *rpt_cboCompany;
	QComboBox *rpt_cboProperty;
	QComboBox *rpt_cboTenant;
	QPushButton   *rpt_cmdSelectUnit;
	QPushButton *rpt_refresh;
protected:
	QDomDocument m_doc;
private slots:
	void openRecentFile();
	void startNewFile();

	void on_actionCompanies_triggered();
	void on_actionProperties_triggered();
	void on_actionTenants_triggered();
	void on_actionAssign_Unit_To_Tenant_triggered();
	void on_actionReceive_Payments_triggered();
	void on_trvBrowser_itemClicked(QTreeWidgetItem *item, int column);
	void on_actionView_Payments_triggered();
	void on_actionAll_Units_triggered();
	void aboutQt();
	void singleInvoice();
	void on_actionMulti_Invoicing_triggered();

	void reloadCalendar();
	void on_cboYear_currentIndexChanged(int index);
	void on_actionContact_List_triggered();
	void on_actionAbout_triggered();
	void on_actionQuit_triggered();
	void on_lstReports_itemClicked(QListWidgetItem *item);

	void showReportPreview();
	void previewRequested(QPrinter *p);
	void on_actionPrint_triggered();
	void on_actionExport_To_Excel_triggered();
	void on_actionChange_Password_triggered();


	void on_actionZoom_In_triggered();
	void on_actionZoom_Out_triggered();

	void fitPage();
	void fitWidth();
	void on_actionDeposit_List_triggered();
	void on_actionView_Tenant_Accounts_triggered();
};

#endif // RENTMANAGERMAINWINDOW_H
