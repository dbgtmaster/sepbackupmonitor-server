#include "dbAbstractModel.h"

#include <QVariant>
#include <QSqlField>
#include <QSqlDatabase>

dbAbstractModel::dbAbstractModel() : _db(DatabasePool::get()), _isValid(false)
{

}

QVariant dbAbstractModel::getColumn(const QString &name) {

    logDebug("dbAbstractModel::getColumn(): get Column: '%1'", name);
    return _dbData[name];
}

bool dbAbstractModel::setValid(bool t) {
    _isValid = t;
    return t;
}

bool dbAbstractModel::isValid() {
    return _isValid;
}

/**
  * Exception
  */
dbModelException::dbModelException(const QString &message) : _message(message) {

}

QString& dbModelException::getMessage() {
    return _message;
}
