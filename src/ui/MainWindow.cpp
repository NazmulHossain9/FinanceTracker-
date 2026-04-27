#include "MainWindow.h"
#include "AddTransactionDialog.h"
#include "database/DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    refreshTable();
    updateSummary();
}

void MainWindow::setupUI() {
    setWindowTitle("💰 Finance Tracker");
    setMinimumSize(900, 600);

    auto* central = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(central);

    // --- Summary Bar ---
    auto* summaryLayout = new QHBoxLayout();
    m_incomeLabel  = new QLabel("Income: $0.00");
    m_expenseLabel = new QLabel("Expenses: $0.00");
    m_balanceLabel = new QLabel("Balance: $0.00");

    m_balanceLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    summaryLayout->addWidget(m_incomeLabel);
    summaryLayout->addWidget(m_expenseLabel);
    summaryLayout->addStretch();
    summaryLayout->addWidget(m_balanceLabel);

    // --- Table ---
    m_table = new QTableWidget();
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels({"Date","Title","Category","Type","Amount"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // --- Buttons ---
    auto* btnLayout = new QHBoxLayout();
    m_addBtn    = new QPushButton("➕ Add Transaction");
    m_deleteBtn = new QPushButton("🗑️ Delete Selected");
    btnLayout->addWidget(m_addBtn);
    btnLayout->addWidget(m_deleteBtn);
    btnLayout->addStretch();

    // --- Wire up ---
    connect(m_addBtn,    &QPushButton::clicked, this, &MainWindow::onAddTransaction);
    connect(m_deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteTransaction);

    mainLayout->addLayout(summaryLayout);
    mainLayout->addWidget(m_table);
    mainLayout->addLayout(btnLayout);
    setCentralWidget(central);
}

void MainWindow::onAddTransaction() {
    AddTransactionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshTable();
        updateSummary();
    }
}

void MainWindow::refreshTable() {
    auto transactions = DatabaseManager::instance().getAllTransactions();
    m_table->setRowCount(transactions.size());

    for (int i = 0; i < transactions.size(); ++i) {
        const auto& t = transactions[i];
        m_table->setItem(i, 0, new QTableWidgetItem(t.getDate().toString("yyyy-MM-dd")));
        m_table->setItem(i, 1, new QTableWidgetItem(t.getTitle()));
        m_table->setItem(i, 2, new QTableWidgetItem(t.getCategory()));
        m_table->setItem(i, 3, new QTableWidgetItem(t.isExpense() ? "Expense" : "Income"));

        auto* amountItem = new QTableWidgetItem(
            QString("$%1").arg(t.getAmount(), 0, 'f', 2));
        amountItem->setForeground(t.isExpense() ? Qt::red : Qt::darkGreen);
        m_table->setItem(i, 4, amountItem);
    }
}

void MainWindow::updateSummary() {
    auto& db = DatabaseManager::instance();
    double income   = db.getTotalIncome();
    double expenses = db.getTotalExpenses();
    double balance  = income - expenses;

    m_incomeLabel ->setText(QString("✅ Income: $%1").arg(income,   0,'f',2));
    m_expenseLabel->setText(QString("🔴 Expenses: $%1").arg(expenses,0,'f',2));
    m_balanceLabel->setText(QString("💰 Balance: $%1").arg(balance,  0,'f',2));
    m_balanceLabel->setStyleSheet(
        balance >= 0 ? "color: green; font-weight: bold; font-size: 16px;"
                     : "color: red;   font-weight: bold; font-size: 16px;"
    );
}