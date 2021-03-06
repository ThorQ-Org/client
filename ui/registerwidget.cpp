#include "registerwidget.h"

#include "namedlineedit.h"
#include "validators.h"

#include <constants.h>

#include <QPushButton>
#include <QVBoxLayout>

ThorQ::UI::RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget(parent)
    , m_usernameInput(new NamedLineEdit(this))
    , m_emailInput(new NamedLineEdit(this))
    , m_passwordInput(new NamedLineEdit(this))
    , m_confirmPasswordInput(new NamedLineEdit(this))
    , m_registerButton(new QPushButton(this))
    , m_layout(new QVBoxLayout(this))
{
    // Username input
    m_usernameInput->setName(tr("Username"));
    m_usernameInput->setEchoMode(QLineEdit::EchoMode::Normal);
    m_usernameInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_usernameInput->setInputValidator(ThorQ::Validators::UsernameValidator);
    QObject::connect(m_usernameInput, &ThorQ::UI::NamedLineEdit::textChanged, this, &ThorQ::UI::RegisterWidget::handleUsernameInputChanged);
    QObject::connect(m_usernameInput, &ThorQ::UI::NamedLineEdit::isInputValidChanged, this, &ThorQ::UI::RegisterWidget::handleInputValidChanged);

    // Email input
    m_emailInput->setName(tr("Email"));
    m_emailInput->setEchoMode(QLineEdit::EchoMode::Normal);
    m_emailInput->setSimpleText(true);
    m_emailInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_emailInput->setInputValidator(ThorQ::Validators::EmailValidator);
    QObject::connect(m_emailInput, &ThorQ::UI::NamedLineEdit::textChanged, this, &ThorQ::UI::RegisterWidget::handleEmailInputChanged);
    QObject::connect(m_emailInput, &ThorQ::UI::NamedLineEdit::isInputValidChanged, this, &ThorQ::UI::RegisterWidget::handleInputValidChanged);

    // Password input
    m_passwordInput->setName(tr("Password"));
    m_passwordInput->setEchoMode(QLineEdit::EchoMode::Password);
    m_passwordInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_passwordInput->setInputValidator(ThorQ::Validators::PasswordValidator);
    QObject::connect(m_passwordInput, &ThorQ::UI::NamedLineEdit::textChanged, this, &ThorQ::UI::RegisterWidget::handlePasswordInputChanged);
    QObject::connect(m_passwordInput, &ThorQ::UI::NamedLineEdit::isInputValidChanged, this, &ThorQ::UI::RegisterWidget::handleInputValidChanged);

    // Confirm password input
    m_confirmPasswordInput->setName(tr("Confirm Password"));
    m_confirmPasswordInput->setEchoMode(QLineEdit::EchoMode::Password);
    m_confirmPasswordInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QObject::connect(m_confirmPasswordInput, &ThorQ::UI::NamedLineEdit::textChanged, this, &ThorQ::UI::RegisterWidget::handlePasswordInputChanged);
    QObject::connect(m_confirmPasswordInput, &ThorQ::UI::NamedLineEdit::isInputValidChanged, this, &ThorQ::UI::RegisterWidget::handleInputValidChanged);

    // Register button
    m_registerButton->setText(tr("Register"));
    m_registerButton->setCursor(Qt::PointingHandCursor);
    m_registerButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_registerButton->setEnabled(false);
    QObject::connect(m_registerButton, &QPushButton::clicked, this, &ThorQ::UI::RegisterWidget::handleRegisterClicked);

    // Layout
    m_layout->setContentsMargins(12, 12, 12, 12);
    m_layout->addWidget(m_usernameInput);
    m_layout->addWidget(m_emailInput);
    m_layout->addWidget(m_passwordInput);
    m_layout->addWidget(m_confirmPasswordInput);
    m_layout->addWidget(m_registerButton);
    setLayout(m_layout);
}

ThorQ::UI::RegisterWidget::~RegisterWidget()
{
}

QString ThorQ::UI::RegisterWidget::username() const
{
    return m_usernameInput->text();
}

QString ThorQ::UI::RegisterWidget::email() const
{
    return m_emailInput->text();
}

QString ThorQ::UI::RegisterWidget::password() const
{
    return m_passwordInput->text();
}

void ThorQ::UI::RegisterWidget::invalidateUsername(const QString& reason)
{
    m_usernameInput->showError(reason);
}

void ThorQ::UI::RegisterWidget::invalidateEmail(const QString& reason)
{
    m_emailInput->showError(reason);
}

void ThorQ::UI::RegisterWidget::invalidatePassword(const QString& reason)
{
    m_passwordInput->showError(reason);
}

void ThorQ::UI::RegisterWidget::clearForms()
{
    m_usernameInput->clearText();
    m_emailInput->clearText();
    clearPassword();
}

void ThorQ::UI::RegisterWidget::clearPassword()
{
    m_passwordInput->clearText();
    m_confirmPasswordInput->clearText();
}

void ThorQ::UI::RegisterWidget::handleUsernameInputChanged()
{
    emit usernameChanged(m_usernameInput->text());
}

void ThorQ::UI::RegisterWidget::handleEmailInputChanged()
{
    emit emailChanged(m_emailInput->text());
}

void ThorQ::UI::RegisterWidget::handlePasswordInputChanged()
{
    QString password = m_passwordInput->text();

    bool passwordsMatch = password == m_confirmPasswordInput->text();

    m_registerButton->setEnabled(passwordsMatch);

    if (passwordsMatch) {
        emit passwordChanged(password);
    } else {
        m_confirmPasswordInput->showError("Passwords do not match");
    }
}

void ThorQ::UI::RegisterWidget::handleInputValidChanged()
{
    m_registerButton->setEnabled(m_usernameInput->isInputValid() && m_emailInput->isInputValid() && m_passwordInput->isInputValid() && m_confirmPasswordInput->isInputValid());
}

void ThorQ::UI::RegisterWidget::handleRegisterClicked()
{
    emit registerClicked();
}
