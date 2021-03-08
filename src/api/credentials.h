#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <QDataStream>

struct Credentials {
    QString cloudhookUrl;
    QString remoteUiUrl;
    QString secret;
    QString token;
    QString webhookId;

    Credentials() = default;
    Credentials(const Credentials &other) = default;

    bool isValid() const {
        return !token.isEmpty();
    }

    bool operator==(const Credentials &other) {
        return cloudhookUrl == other.cloudhookUrl
                && remoteUiUrl == other.remoteUiUrl
                && secret == other.secret
                && token == other.token
                && webhookId == other.webhookId;
    }
    bool operator!=(const Credentials &other) {
        return !(*this == other);
    }

    friend QDataStream &operator<<(QDataStream &out, const Credentials &credentials) {
        out << credentials.cloudhookUrl;
        out << credentials.remoteUiUrl;
        out << credentials.secret;
        out << credentials.token;
        out << credentials.webhookId;

        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, Credentials &credentials) {
        in >> credentials.cloudhookUrl;
        in >> credentials.remoteUiUrl;
        in >> credentials.secret;
        in >> credentials.token;
        in >> credentials.webhookId;

        return in;
    }
};
Q_DECLARE_METATYPE(Credentials)

#endif // CREDENTIALS_H
