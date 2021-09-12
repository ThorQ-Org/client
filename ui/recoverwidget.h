#ifndef THORQ_RECOVERWIDGET_H
#define THORQ_RECOVERWIDGET_H

#include <QWidget>

class QPushButton;
class QVBoxLayout;

namespace ThorQ::UI {

class NamedLineEdit;

class RecoverWidget : public QWidget
{
     Q_OBJECT
    Q_DISABLE_COPY(RecoverWidget)
public:
    RecoverWidget(QWidget* parent = nullptr);
    ~RecoverWidget();
signals:
    void emailChanged(const QString& email);
    void recoverClicked();
public slots:
    void invalidateEmail(const QString& reason);
    void clearForms();
private slots:
    void handleEmailInputChanged();
    void handleRecoverClicked();
private:
    QVBoxLayout* m_layout;

    NamedLineEdit* m_emailInput;
    QPushButton* m_recoverButton;
};

}

#endif // THORQ_RECOVERWIDGET_H
