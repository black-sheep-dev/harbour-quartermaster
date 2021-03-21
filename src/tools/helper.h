#ifndef HELPER_H
#define HELPER_H

class QByteArray;

class Helper
{
public:
    static QByteArray gunzip( const QByteArray &data);
};

#endif // HELPER_H
