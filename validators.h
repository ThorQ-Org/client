#ifndef THORQ_VALIDATORS_H
#define THORQ_VALIDATORS_H

#include <QString>
#include <QStringView>

#include <functional>
#include <utility>

namespace ThorQ::Validators {

using ArgType = QStringView;
using RetType = std::pair<bool, QString>;
using FuncType = std::function<RetType(ArgType)>;

const RetType DefaultValue = RetType{ true, QString{} };
const FuncType FunctionPlaceholder = [](ArgType){ return DefaultValue; };

RetType UsernameValidator(ArgType username);
RetType PasswordValidator(ArgType password);
RetType EmailValidator(ArgType email);

}

#endif // THORQ_VALIDATORS_H
