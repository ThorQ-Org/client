#include "validators.h"

#include "constants.h"

ThorQ::Validators::RetType ThorQ::Validators::UsernameValidator(ThorQ::Validators::ArgType username)
{
    if (username.length() < ThorQ::USERNAME_LEN_MIN) {
        return ThorQ::Validators::RetType(false, "Too short username");
    }

    if (username.length() > ThorQ::USERNAME_LEN_MAX) {
        return ThorQ::Validators::RetType(false, "Too long username");
    }

    return ThorQ::Validators::DefaultValue;
}

ThorQ::Validators::RetType ThorQ::Validators::PasswordValidator(ThorQ::Validators::ArgType password)
{
    if (password.length() < ThorQ::PASSWORD_LEN_MIN) {
        return ThorQ::Validators::RetType(false, "Too short password");
    }

    if (password.length() > ThorQ::PASSWORD_LEN_MAX) {
        return ThorQ::Validators::RetType(false, "Too long password");
    }

    return ThorQ::Validators::DefaultValue;
}


constexpr bool IsAlhaNumericChar(QChar c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9');
}
constexpr bool IsValidLocalChar(QChar  c)
{
    return IsAlhaNumericChar(c) ||
           c == '!'  ||
           c == '#'  ||
           c == '$'  ||
           c == '%'  ||
           c == '&'  ||
           c == '\'' ||
           c == '*'  ||
           c == '+'  ||
           c == '/'  ||
           c == '='  ||
           c == '?'  ||
           c == '^'  ||
           c == '_'  ||
           c == '`'  ||
           c == '{'  ||
           c == '|'  ||
           c == '}'  ||
           c == '~'  ||
           c == '-';
}
constexpr bool IsValidLocalPart(const QChar*& it)
{
    const QChar* startIt = it;
    do {
        if (!IsValidLocalChar(*it++)) {
            return false;
        }

        while (IsValidLocalChar(*it)) { it++; }
    }
    while (*it++ == '.');

    int localLen = (it - 1) - startIt;

    return *(it - 1) == '@' && localLen <= 64;
}
constexpr bool IsValidDomainPart(const QChar*& it)
{
    QChar c = 0;
    unsigned int n = 0;
    do {
        const QChar* dnsLabelStartIt = it;
        if (!IsAlhaNumericChar(*it++)) {
            return false;
        }

        while (IsAlhaNumericChar(*it)) { it++; }

        int dnsLabelLen = it - dnsLabelStartIt;
        if (dnsLabelLen > 63) {
            return false;
        }

        c = *it++;
        n++;
    }
    while (c == '.');

    it--;

    return c == 0 && n > 1;
}

ThorQ::Validators::RetType ThorQ::Validators::EmailValidator(ThorQ::Validators::ArgType email)
{
    const QChar* begin = email.data();
    const QChar* it = begin;

    // Check email size
    if (email.length() < ThorQ::EMAIL_LEN_MIN) {
        return ThorQ::Validators::RetType(false, "Too short email");
    }
    if (email.length() > ThorQ::EMAIL_LEN_MAX) {
        return ThorQ::Validators::RetType(false, "Too long email");
    }

    // Verify that email recepient section is valid
    if (!IsValidLocalPart(it)) {
        return ThorQ::Validators::RetType(false, "Invalid email (local part)");
    }

    // Validate domain validity
    if (!IsValidDomainPart(it)) {
        return ThorQ::Validators::RetType(false, "Invalid email (domain part)");
    }

    // Check that the entire email has been checked
    int validatedLength = it - begin;

    if (validatedLength != email.length()) {
        return ThorQ::Validators::RetType(false, "Invalid email");
    }

    return ThorQ::Validators::DefaultValue;
}
