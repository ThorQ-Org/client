#ifndef THORQ_NAMEDLINEEDIT_H
#define THORQ_NAMEDLINEEDIT_H

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QByteArray>

#include "validators.h"

class QLabel;
class QVBoxLayout;

namespace ThorQ::UI {

class ErrorLabel;

class NamedLineEdit : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(NamedLineEdit)
public:
    NamedLineEdit(QWidget* parent = nullptr);
    ~NamedLineEdit();

    QString text() const;
    bool isInputValid() const;
signals:
    void textChanged();
    void editingFinished();
    void isInputValidChanged(bool isValid);
public slots:
    void setName(const QString& name);

    void setText(const QString& text);
    void clearText();

    void showError(const QString& error);
    void showError();
    void hideError();

    void setEchoMode(QLineEdit::EchoMode);

    void setSimpleText(bool isSimple);
    void setInputValidator(ThorQ::Validators::FuncType inputValidator);
private slots:
    bool validateInput(const QString& text);
    void handleTextEdited();
private:
    QLabel* m_labelTitle;
    QLineEdit* m_lineEdit;
    ErrorLabel* m_labelError;
    QVBoxLayout* m_layout;

    bool m_simpleText;
    bool m_isInputValid;
    ThorQ::Validators::FuncType m_inputValidator;

    int m_lastValueLength;
    QByteArray m_lastValueHash;
};

}

#endif // THORQ_NAMEDLINEEDIT_H
