#include "rentmanagermainwindow.h"
#include "ui_rentmanagermainwindow.h"

#include "publics.h"
#include <QActionGroup>
#include "companyinitializationdialog.h"

RentManagerMainWindow *RentManagerMainWindow::m_instance = NULL;

RentManagerMainWindow::RentManagerMainWindow(QWidget *parent) :
	QMainWindow(),
	actionsToDisable(0),
	ui(new Ui::RentManagerMainWindow)
{
	Q_ASSERT_X(m_instance == NULL, "MainWindow", "MainWindow recreated!");
	m_instance = this;

	ui->setupUi(this);

	//ActionGroup
	if (!actionsToDisable)
		actionsToDisable = new QActionGroup(this);

	actionsToDisable->addAction(ui->actionAssign_Unit_To_Tenant);
	actionsToDisable->addAction(ui->actionCompanies);
	actionsToDisable->addAction(ui->actionCreate_Invoice);
	actionsToDisable->addAction(ui->actionMulti_Invoicing);
	actionsToDisable->addAction(ui->actionProperties);
	actionsToDisable->addAction(ui->actionReceive_Payments);
	actionsToDisable->addAction(ui->actionTenants);
	actionsToDisable->addAction(ui->actionUnites);
	actionsToDisable->addAction(ui->actionView_Tenan_Accounts);
	actionsToDisable->addAction(ui->action_Save);
	actionsToDisable->addAction(ui->actionSave_As);

	actionsToDisable->setDisabled(true);
	ui->menuRecent_Files->clear();
	for (int i = 0; i < MaxRecentFiles; ++i) {
		recentFileActs[i] = new QAction(this);
		recentFileActs[i]->setVisible(false);
		connect (recentFileActs[i], SIGNAL(triggered()), SLOT(openRecentFile()));
		ui->menuRecent_Files->addAction(recentFileActs[i]);
	}
	updateRecentFileActions();

	connect (ui->action_Open, SIGNAL(triggered()), SLOT(startOpenFile()));
}

RentManagerMainWindow::~RentManagerMainWindow()
{
	delete ui;
}

RentManagerMainWindow *RentManagerMainWindow::instance()
{
	return m_instance;
}

void RentManagerMainWindow::showQueryError(QSqlQuery qu, QString title, QString textBefore, QString textAfter)
{
	QMessageBox::critical(this, title, QString("%1<br/><b>%2</b><br/>%3").arg(textBefore, qu.lastError().text(), textAfter));
}

void RentManagerMainWindow::startOpenFile()
{
	QString fileName = Publics::getOpenFile(this);
	if (fileName.length() > 0) {
		loadFile(fileName);
	}
}

void RentManagerMainWindow::closeFile()
{
	actionsToDisable->setEnabled(false);

	ui->trvBrowser->invisibleRootItem()->takeChildren();
	ui->tblUnits->clearContents();

	curFile = "";
	if (db.isOpen())
		db.close();
}

void RentManagerMainWindow::loadFile(const QString &fileName)
{
	if (fileName.isNull()) {
		startOpenFile();
		return;
	}

	closeFile();
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(fileName);
	if (!db.open()) {
		QMessageBox::critical(this, tr("Error"), tr("Database Error.\n%1").arg(db.lastError().text()));
		return;
	}

	//Database open
	initializeCompanyFile();

	curFile = fileName;
	ui->statusBar->showMessage(QString("Opened file: %1").arg(fileName));
	//refresh browser

#if QT_VERSION > 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationDisplayName());
#endif
#if QT_VERSION < 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationName());
#endif
	QStringList files = sett.value("recentFiles").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);

	while (files.size() > MaxRecentFiles)
		files.removeLast();

	sett.setValue("recentFiles", files);
	updateRecentFileActions();
	//Enable employee related widgets and actions
	actionsToDisable->setEnabled(true);
}

bool RentManagerMainWindow::saveFile(const QString &fileName)
{
	Q_UNUSED(fileName);
	return true;
}

void RentManagerMainWindow::setCurrentFile(const QString &fileName)
{
	Q_UNUSED(fileName);
}

QString RentManagerMainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void RentManagerMainWindow::updateRecentFileActions()
{
#if QT_VERSION > 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationDisplayName());
#endif
#if QT_VERSION < 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationName());
#endif
	QStringList files = sett.value("recentFiles").toStringList();

	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString fNum = QString::number(i + 1);

		if (i < 9)
			fNum.prepend(" ");

		QString text = tr("%1 %2").arg(fNum).arg(strippedName(files[i]));
		recentFileActs[i]->setText(text);
		recentFileActs[i]->setStatusTip(files[i]);
		recentFileActs[i]->setData(files[i]);
		recentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		recentFileActs[j]->setVisible(false);

}

void RentManagerMainWindow::initializeCompanyFile()
{
	CompanyInitializationDialog *init = new CompanyInitializationDialog(this);
	init->exec();
}

void RentManagerMainWindow::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		loadFile(action->data().toString());
}

void RentManagerMainWindow::startNewFile()
{

}
