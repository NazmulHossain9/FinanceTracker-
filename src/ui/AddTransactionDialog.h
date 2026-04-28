#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDateEdit>

class AddTransactionDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddTransactionDialog(QWidget* parent = nullptr);

private slots:
    void onAccept();

private:
    QLineEdit*      m_titleEdit;
    QDoubleSpinBox* m_amountSpin;
    QComboBox*      m_typeCombo;
    QComboBox*      m_categoryCombo;
    QDateEdit*      m_dateEdit;
};