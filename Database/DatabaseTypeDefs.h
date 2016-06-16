#ifndef DATABASE_TYPEDEFS_H
#define DATABASE_TYPEDEFS_H

    template <typename A, typename B> class QHash;
    template <typename A, typename B> class QMap;
    class QString;
    class QVariant;
    typedef QHash<QString, QVariant> hashColumns;
    typedef QMap<QString, QVariant> mapColumns;

#endif
