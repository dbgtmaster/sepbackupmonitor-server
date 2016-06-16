#include "Registry.h"

QHash<QByteArray, QVariant> Registry::_registry;

Registry::Registry()
{
}

QVariant Registry::get(const QByteArray &key, bool *ok) {

    if (ok != 0) {
        QVariant value( _registry.value(key) );
        *ok = value.isValid();
        return value;
    }
    else {
        return _registry.value(key);
    }
}
