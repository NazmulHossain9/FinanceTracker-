# Utils

Stateless helper classes. No database access, no Qt parent hierarchy.

## CSVExporter

`exportTransactions(transactions, filePath)` writes a vector of `Transaction` objects to a CSV file.

- Returns `true` on success, `false` if the file cannot be opened.
- Column order: Date, Title, Category, Type, Amount.
- Caller is responsible for fetching transactions from `DatabaseManager` before passing them in.

To add another export format (JSON, OFX, etc.), add a new static class here following the same pattern.
