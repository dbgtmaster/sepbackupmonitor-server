#ifndef UTILDATASTREAM_H
#define UTILDATASTREAM_H

#include <QString>
#include <QDataStream>

#include "TcpCommandsHelper.h"

class QDataStream;

namespace UtilDataStream {

// Liest Daten von einem Stream, und wirft eine Exception, wenn beim lesen ein Fehler aufgetreten ist..
// stream: DataStream, von welchem gelesen werden soll.
// var: Variable, in welcher der Inhalt vom Datenstream geschrieben wird...
template <typename DATA_TYPE>
void read(QDataStream& stream, DATA_TYPE& var);

// Schreibt Daten in einem Stream, und wirft eine Exception, wenn beim schreiben ein Fehler aufgetreten ist..
// stream: DataStream, in welchem geschrieben werden soll.
// var: Variable, von welcher der Inhalt gelesen werden soll...
template <typename DATA_TYPE>
void write(QDataStream& stream, const DATA_TYPE& var);

}

// Exception, wenn ein Fehler beim lesen des DataStreams aufgetreten ist...
class UtilDataStreamReadException {
private:
    QString _errorMsg;

public:
    UtilDataStreamReadException(const QString &errorMsg) : _errorMsg(errorMsg) {
    }

    inline const QString& error() const {
        return _errorMsg;
    }
};

// Exception, wenn ein Fehler beim schreiben des DataStreams aufgetreten ist...
class UtilDataStreamWriteException {
private:
    QString _errorMsg;

public:
    UtilDataStreamWriteException(const QString &errorMsg) : _errorMsg(errorMsg) {
    }

    inline const QString& error() const {
        return _errorMsg;
    }
};

/* ****************************************************************************************************************** */

/**
  * Template Deklartions...
  */

// Schreibt Daten von einem QDataStream in die Variable "var"
template<typename DATA_TYPE>
void UtilDataStream::read(QDataStream &stream, DATA_TYPE &var) {

    stream >> var;

    // Status überprüfen:
    if (stream.status() != QDataStream::Ok) {
        /**
          * Es ist ein Fehler beim lesen der Daten aufgetreten, wir werden eine Exception..
          */
        QString errorMsg;
        switch (stream.status()) {
        case QDataStream::ReadPastEnd:
            errorMsg = "The data stream has read past the end of the data in the underlying device.";
            break;
        case QDataStream::ReadCorruptData:
            errorMsg = "The data stream has read corrupt data.";
            break;
        default:
            errorMsg = "Unknown error message.";
        }

        throw UtilDataStreamReadException(errorMsg);
    }
}


// Liest Daten von "var" und schreibt diese in den QDataStream
template<typename DATA_TYPE>
void UtilDataStream::write(QDataStream &stream, const DATA_TYPE &var) {

    stream << var;

    // Status überprüfen:
    if (stream.status() != QDataStream::Ok) {
        /**
          * Es ist ein Fehler beim lesen der Daten aufgetreten, wir werden eine Exception..
          */
        QString errorMsg;
        switch (stream.status()) {
        case QDataStream::ReadPastEnd:
            errorMsg = "The data stream has read past the end of the data in the underlying device.";
            break;
        case QDataStream::ReadCorruptData:
            errorMsg = "The data stream has read corrupt data.";
            break;
        default:
            errorMsg = "Unknown error message.";
        }

        throw UtilDataStreamWriteException(errorMsg);
    }
}

#endif // UTILDATASTREAM_H
