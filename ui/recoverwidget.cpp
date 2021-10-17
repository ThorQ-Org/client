#include "recoverwidget.h"

#include "namedlineedit.h"
#include "validators.h"

#include <QPushButton>
#include <QVBoxLayout>

ThorQ::UI::RecoverWidget::RecoverWidget(QWidget* parent)
    : QWidget(parent)
    , m_layout(new QVBoxLayout(this))
    , m_emailInput(new NamedLineEdit(this))
    , m_recoverButton(new QPushButton(this))
{
    // Email input
    m_emailInput->setName(tr("Email"));
    m_emailInput->setEchoMode(QLineEdit::EchoMode::Normal);
    m_emailInput->setSimpleText(true);
    m_emailInput->setInputValidator(ThorQ::Validators::EmailValidator);
    QObject::connect(m_emailInput, &ThorQ::UI::NamedLineEdit::textChanged, this, &ThorQ::UI::RecoverWidget::handleEmailInputChanged);
    QObject::connect(m_emailInput, &ThorQ::UI::NamedLineEdit::isInputValidChanged, m_recoverButton, &QPushButton::setEnabled);

    // Recover button
    m_recoverButton->setText(tr("Recover"));
    m_recoverButton->setCursor(Qt::PointingHandCursor);
    m_recoverButton->setEnabled(false);
    QObject::connect(m_recoverButton, &QPushButton::clicked, this, &ThorQ::UI::RecoverWidget::handleRecoverClicked);

    // Layout
    m_layout->setContentsMargins(12, 12, 12, 12);
    m_layout->addWidget(m_emailInput);
    m_layout->addWidget(m_recoverButton);
}

ThorQ::UI::RecoverWidget::~RecoverWidget()
{
}

void ThorQ::UI::RecoverWidget::invalidateEmail(const QString& reason)
{
    m_emailInput->showError(reason);
}

void ThorQ::UI::RecoverWidget::clearForms()
{
    m_emailInput->clearText();
}

void ThorQ::UI::RecoverWidget::handleEmailInputChanged()
{
    emit emailChanged(m_emailInput->text());
}

void ThorQ::UI::RecoverWidget::handleRecoverClicked()
{
    emit recoverClicked();
}
