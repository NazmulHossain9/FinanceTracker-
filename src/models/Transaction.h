#pragma once
#include <QString>
#include <QDate>

enum class TransactionType { INCOME, EXPENSE };

QString typeToString(TransactionType type);

class Transaction {
public:
    Transaction(int id, const QString& title, double amount,
                TransactionType type, const QString& category, const QDate& date);
    Transaction(const QString& title, double amount,
                TransactionType type, const QString& category, const QDate& date);

    int             getId()       const { return m_id; }
    QString         getTitle()    const { return m_title; }
    double          getAmount()   const { return m_amount; }
    TransactionType getType()     const { return m_type; }
    QString         getCategory() const { return m_category; }
    QDate           getDate()     const { return m_date; }

    bool    isExpense()       const { return m_type == TransactionType::EXPENSE; }
    QString formattedAmount() const;

private:
    int             m_id;
    QString         m_title;
    double          m_amount;
    TransactionType m_type;
    QString         m_category;
    QDate           m_date;
};