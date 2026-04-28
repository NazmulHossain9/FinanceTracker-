#include "MainWindow.h"
#include "AddTransactionDialog.h"
#include "DashboardWidget.h"
#include "database/DatabaseManager.h"
#include "utils/CSVExporter.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QDialog>
#include <QMenuBar>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    setupMenuBar();
    refreshTable();
    updateSummary();
}

void MainWindow::setupUI() {
    setWindowTitle("Finance Tracker");
    setMinimumSize(900, 600);

    auto* central    = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(central);

    auto* summaryLayout = new QHBoxLayout();
    m_incomeLabel  = new QLabel("Income: $0.00");
    m_expenseLabel = new QLabel("Expenses: $0.00");
    m_balanceLabel = new QLabel("Balance: $0.00");
    m_balanceLabel->setStyleSheet("font-weight: bold; font-size: 16px;");

    summaryLayout->addWidget(m_incomeLabel);
    summaryLayout->addWidget(m_expenseLabel);
    summaryLayout->addStretch();
    summaryLayout->addWidget(m_balanceLabel);

    m_table = new QTableWidget();
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels({"Date", "Title", "Category", "Type", "Amount"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);

    auto* btnLayout = new QHBoxLayout();
    m_addBtn    = new QPushButton("Add Transaction");
    m_deleteBtn = new QPushButton("Delete Selected");
    m_addBtn->setMinimumHeight(32);
    m_deleteBtn->setMinimumHeight(32);
    btnLayout->addWidget(m_addBtn);
    btnLayout->addWidget(m_deleteBtn);
    btnLayout->addStretch();

    connect(m_addBtn,    &QPushButton::clicked, this, &MainWindow::onAddTransaction);
    connect(m_deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteTransaction);

    mainLayout->addLayout(summaryLayout);
    mainLayout->addWidget(m_table);
    mainLayout->addLayout(btnLayout);
    setCentralWidget(central);
}

void MainWindow::setupMenuBar() {
    auto* fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("&Export CSV...", this, &MainWindow::onExportCSV);
    fileMenu->addSeparator();
    fileMenu->addAction("&Quit", qApp, &QApplication::quit, QKeySequence::Quit);

    auto* viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction("&Dashboard", this, &MainWindow::onShowDashboard);

    auto* helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("&About", this, [this]() {
        QMessageBox::about(this, "Finance Tracker",
            "<b>Finance Tracker v1.0</b><br>"
            "Personal finance management app.<br><br>"
            "Built with C++17 &amp; Qt6 &amp; SQLite.");
    });
}

void MainWindow::onAddTransaction() {
    AddTransactionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshTable();
        updateSummary();
    }
}

void MainWindow::onDeleteTransaction() {
    int row = m_table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Delete", "Please select a transaction to delete.");
        return;
    }

    int     id    = m_table->item(row, 0)->data(Qt::UserRole).toInt();
    QString title = m_table->item(row, 1)->text();

    auto reply = QMessageBox::question(this, "Confirm Delete",
        QString("Delete \"%1\"?").arg(title),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (DatabaseManager::instance().deleteTransaction(id)) {
            refreshTable();
            updateSummary();
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete the transaction.");
        }
    }
}

void MainWindow::onExportCSV() {
    QString path = QFileDialog::getSaveFileName(
        this, "Export Transactions",
        QDir::homePath() + "/transactions.csv",
        "CSV files (*.csv)");
    if (path.isEmpty()) return;

    auto txns = DatabaseManager::instance().getAllTransactions();
    if (CSVExporter::exportTransactions(txns, path))
        QMessageBox::information(this, "Export",
            "Exported " + QString::number(txns.size()) + " transactions to:\n" + path);
    else
        QMessageBox::critical(this, "Export Failed", "Could not write to the selected file.");
}

void MainWindow::onShowDashboard() {
    auto* dialog = new QDialog(this);
    dialog->setWindowTitle("Dashboard");
    dialog->resize(1000, 520);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    auto* layout    = new QVBoxLayout(dialog);
    auto* dashboard = new DashboardWidget(dialog);
    layout->addWidget(dashboard);
    dialog->exec();
}

void MainWindow::refreshTable() {
    auto transactions = DatabaseManager::instance().getAllTransactions();
    m_table->setRowCount(transactions.size());

    for (int i = 0; i < transactions.size(); ++i) {
        const auto& t = transactions[i];

        auto* dateItem = new QTableWidgetItem(t.getDate().toString("yyyy-MM-dd"));
        dateItem->setData(Qt::UserRole, t.getId());   // store ID for delete
        m_table->setItem(i, 0, dateItem);
        m_table->setItem(i, 1, new QTableWidgetItem(t.getTitle()));
        m_table->setItem(i, 2, new QTableWidgetItem(t.getCategory()));
        m_table->setItem(i, 3, new QTableWidgetItem(t.isExpense() ? "Expense" : "Income"));

        auto* amountItem = new QTableWidgetItem(t.formattedAmount());
        amountItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        amountItem->setForeground(t.isExpense() ? QColor(Qt::red) : QColor(Qt::darkGreen));
        m_table->setItem(i, 4, amountItem);
    }
}

void MainWindow::updateSummary() {
    auto& db = DatabaseManager::instance();
    double income   = db.getTotalIncome();
    double expenses = db.getTotalExpenses();
    double balance  = income - expenses;

    m_incomeLabel ->setText(QString("Income:  $%1").arg(income,   0, 'f', 2));
    m_expenseLabel->setText(QString("Expenses: $%1").arg(expenses, 0, 'f', 2));
    m_balanceLabel->setText(QString("Balance: $%1").arg(balance,   0, 'f', 2));
    m_balanceLabel->setStyleSheet(
        balance >= 0 ? "color: green; font-weight: bold; font-size: 16px;"
                     : "color: red;   font-weight: bold; font-size: 16px;");
}
