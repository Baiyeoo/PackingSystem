#include "carstore.h"
#include <QFile>
#include <QSaveFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>


static QJsonArray cartoarray(const std::vector<parkingsystem::Car>& cars) {
    QJsonArray arr;
    for (const auto& car : cars) arr.append(car.toJson());
    return arr;
}
static QJsonArray cartoarray(const std::deque<parkingsystem::Car>& cars) {
    QJsonArray arr;
    for (const auto& car : cars) arr.append(car.toJson());
    return arr;
}

bool carstore::exportto(const parkingsystem& sys, const QString& filepath) {
    QJsonObject root;
    root["version"] = 1;
    root["activecar"] = cartoarray(sys.getactivecar());
    root["waitingcar"] = cartoarray(sys.getactivecar());
    root["waitingcar"] = cartoarray(sys.getwaitingcar());
    root["historycar"] = cartoarray(sys.gethistorycar());

    QSaveFile file(filepath);
    if(!file.open(QIODevice::WriteOnly)) {
        qWarning() <<"carsotre: couldn't writein"<<filepath;
        return false;

    }
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return file.commit();
}
bool carstore::importfrom(parkingsystem& sys, const QString& filepath) {
    QFile file(filepath);
    if (!file.exists()) return false;
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning() << "carstore: 无法打开" << filepath;
        return false;
    }
    QJsonParseError err;
    const auto doc = QJsonDocument::fromJson(file.readAll(), &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        qWarning() << "carstore: JSON 解析失败" << err.errorString();
        return false;
    }
    const auto root = doc.object();

    std::vector<parkingsystem::Car> active;
    for (const auto& v : root["activecar"].toArray())
        active.push_back(parkingsystem::Car::fromJson(v.toObject()));

    std::deque<parkingsystem::Car> waiting;
    for (const auto& v : root["waitingcar"].toArray())
        waiting.push_back(parkingsystem::Car::fromJson(v.toObject()));

    std::vector<parkingsystem::Car> history;
    for (const auto& v : root["historycar"].toArray())
        history.push_back(parkingsystem::Car::fromJson(v.toObject()));

    sys.setactivecar(std::move(active));
    sys.setwaitingcar(std::move(waiting));
    sys.sethistorycar(std::move(history));
    return true;
}