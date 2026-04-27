bool DatabaseManager::createTables() {
    QSqlQuery query;
    return query.exec(R"(
        CREATE TABLE IF NOT EXISTS transactions (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            title       TEXT NOT NULL,
            amount      REAL NOT NULL,
            type        TEXT NOT NULL,
            category    TEXT,
            date        TEXT NOT NULL
        )
    )");
}

bool DatabaseManager::addTransaction(const Transaction& t) {
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO transactions (title, amount, type, category, date)
        VALUES (:title, :amount, :type, :category, :date)
    )");
    query.bindValue(":title",    t.getTitle());
    query.bindValue(":amount",   t.getAmount());
    query.bindValue(":type",     t.isExpense() ? "EXPENSE" : "INCOME");
    query.bindValue(":category", t.getCategory());
    query.bindValue(":date",     t.getDate().toString(Qt::ISODate));
    return query.exec();
}

QVector<Transaction> DatabaseManager::getAllTransactions() {
    QVector<Transaction> list;
    QSqlQuery query("SELECT * FROM transactions ORDER BY date DESC");
    while (query.next()) {
        TransactionType type = (query.value("type").toString() == "EXPENSE")
            ? TransactionType::EXPENSE : TransactionType::INCOME;

        list.append(Transaction(
            query.value("id").toInt(),
            query.value("title").toString(),
            query.value("amount").toDouble(),
            type,
            query.value("category").toString(),
            QDate::fromString(query.value("date").toString(), Qt::ISODate)
        ));
    }
    return list;
}