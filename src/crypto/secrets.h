#ifndef SECRETS_H
#define SECRETS_H

#include <QDataStream>

struct Secrets {
    QString cloudhookUrl;
    QString remoteUiUrl;
    QString secret;
    QString token;
    QString webhookId;

    bool operator==(const Secrets &other) {
        return cloudhookUrl == other.cloudhookUrl
                && remoteUiUrl == other.remoteUiUrl
                && secret == other.secret
                && token == other.token
                && webhookId == other.webhookId;
    }

    bool operator!=(const Secrets &other) {
        return !(cloudhookUrl == other.cloudhookUrl
                 && remoteUiUrl == other.remoteUiUrl
                 && secret == other.secret
                 && token == other.token
                 && webhookId == other.webhookId);
    }

    friend QDataStream &operator<<(QDataStream &out, const Secrets &secrets) {
        out << secrets.cloudhookUrl;
        out << secrets.remoteUiUrl;
        out << secrets.secret;
        out << secrets.token;
        out << secrets.webhookId;

        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, Secrets &secrets) {
        in >> secrets.cloudhookUrl;
        in >> secrets.remoteUiUrl;
        in >> secrets.secret;
        in >> secrets.token;
        in >> secrets.webhookId;

        return in;
    }
};
Q_DECLARE_METATYPE(Secrets)

#endif // WEBHOOKSECRETS_H
