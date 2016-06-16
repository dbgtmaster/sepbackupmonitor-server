#ifndef REGISTRY_H
#define REGISTRY_H

#include <QHash>
#include <QVariant>

class Registry
{
    static QHash<QByteArray, QVariant> _registry;
public:
    Registry();

    static QVariant get(const QByteArray &, bool *ok = 0);

    static inline void set(const QByteArray &key, const QVariant &value) {
        _registry[key] = value;
    }
};

#endif // REGISTRY_H
