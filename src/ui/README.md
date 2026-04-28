# UI

All Qt widgets. Each class calls `DatabaseManager::instance()` directly — no intermediate service layer.

## MainWindow

Top-level `QMainWindow`. Owns the transaction `QTableWidget` and the summary label bar.

- `refreshTable()` — reloads all transactions from DB and rebuilds table rows. The transaction DB `id` is stored in column 0's `Qt::UserRole` so delete can look it up without a separate query.
- `updateSummary()` — recomputes income/expense/balance labels from DB aggregates.
- Both methods must be called together after any write operation.

Menu bar: **File → Export CSV**, **View → Dashboard**, **Help → About**.

## AddTransactionDialog

Modal `QDialog` opened by `MainWindow::onAddTransaction()`. Validates and writes directly to `DatabaseManager` on accept via `onAccept()`. Returns `QDialog::Accepted` on success so the caller knows to refresh.

Fields: title, amount, type (Income/Expense), category, date.

## DashboardWidget

`QWidget` embedded in a `QDialog` by `MainWindow::onShowDashboard()`.

- `createExpensePieChart()` — pie chart of expenses by category, sourced from `DatabaseManager::getSpentByCategory`.
- `createMonthlyBarChart()` — bar chart of income vs. expenses per month for the current year.
- `refresh()` — rebuilds both charts from fresh DB data; call this if the widget needs to stay open while transactions change.
