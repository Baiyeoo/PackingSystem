#include "provide.h"
#include <algorithm>
QJsonObject parkingsystem::Car::toJson() const {
    QJsonObject obj;
    obj["plate"] = plate;
    obj["intime"] = intime.toString(Qt::ISODate);
    obj["outtime"] = outtime.toString(Qt::ISODate);
    obj["dequenumber"] = dequenumber;
    obj["indequetime"] = indequetime.toString(Qt::ISODate);
    obj["spotposition"] = spotposition;
    return obj;
}
parkingsystem::Car parkingsystem::Car::fromJson(const QJsonObject& obj) {
    Car car;
    car.plate = obj["plate"].toString();
    car.intime = QDateTime::fromString(obj["intime"].toString(), Qt::ISODate);
    car.outtime = QDateTime::fromString(obj["outtime"].toString(), Qt::ISODate);
    car.dequenumber = obj["dequenumber"].toInt();
    car.indequetime = QDateTime::fromString(obj["indequetime"].toString(), Qt::ISODate);
    car.spotposition = obj["spotposition"].toInt(-1);
    return car;
}
bool parkingsystem::carin(QString plate) {
    for(Car car : activecar) {
        if(car.plate == plate) return false;//重复入队
    }
    int parknum = parkcfg.cfg_parknum();
    if (parknum == activecar.size()) {
        int queuenumber = waitingcar.empty() ? 1 : waitingcar.back().dequenumber + 1;
        waitingcar.push_back({
            plate,
                              QDateTime(),
                              QDateTime(),
            queuenumber,
                              QDateTime::currentDateTime(),
        });
        //wait
        return true;//检查大小可检测是具体排队了还是入队了
    }

    int unused = qHash(plate) % parknum;//第一处不封装的车位逻辑
    QSet<int> usedcar;
    for (const auto& car : activecar) usedcar.insert(car.spotposition);
    int parknum2 = parknum;
    while (parknum2--) {
        if(!usedcar.contains(unused)) break;
        unused++;
        unused %= parknum;
    }

    activecar.push_back({
        plate,
        QDateTime::currentDateTime(),
                         QDateTime(),
        0,//车位忘记做了？这个是等待序号
                         QDateTime(),
        //0,//实际花费，其实可以不用，仅仅是预计和历史都可以通过相减逻辑实现
        unused,
    });

    return true;//入队成功
}
bool parkingsystem::carout(QString plate) {

    bool inpark = false;
    Car tempcar;
    for (Car car : activecar) {
        if (car.plate == plate) {
            inpark = true;
            tempcar = car;
            break;
        }
    }
    if (inpark) {
        activecar.erase(std::remove(activecar.begin(), activecar.end(), tempcar), activecar.end());
        historycar.push_back({
            plate,
            tempcar.intime,
            QDateTime::currentDateTime(),
            0,
            QDateTime(),
        });
    } else {
        bool inque = false;
        for (Car car : waitingcar) {
            if (car.plate == plate) {
                inque = true;
                tempcar = car;
                break;
            }
        }
        if (inque) {
            waitingcar.erase(std::remove(waitingcar.begin(), waitingcar.end(), tempcar), waitingcar.end());
            historycar.push_back({
                plate,
                QDateTime(),
                QDateTime(),
                0,
                QDateTime(),
            });
            return true;
        }
        //cannot find car messege both activecar and waitingcar;
        return false;
    }

    int parknum = parkcfg.cfg_parknum();

    if (parknum == activecar.size() + 1 && !waitingcar.empty()) {
        tempcar = waitingcar.front();
        waitingcar.pop_front();

        int unused = qHash(tempcar.plate) % parknum;//第二处不封装的车位逻辑
        QSet<int> usedcar;
        for (const auto& car : activecar) usedcar.insert(car.spotposition);
        int parknum2 = parknum;
        while (parknum2--) {
            if(!usedcar.contains(unused)) break;
            unused++;
            unused %= parknum;
        }

        activecar.push_back({
            tempcar.plate,
                             QDateTime::currentDateTime(),
                             QDateTime(),
            0,
                             QDateTime(),
            unused,
        });
        //出队的时候如果满人了，先要判断合法出队，出队成功，如果满人，如果没满人,按照单线程思路，没满人之前有carout守关，waitingcar不可能有元素
    }
    return true;
}
int parkingsystem::anticipatesettle(QDateTime intime, QDateTime outtime_ifh) {
    int firsthourcost = parkcfg.cfg_firsthourcost();
    int laterhourcost = parkcfg.cfg_laterhourcost();

    if(!intime.isValid()) return 0;//处理车辆历史的不排队记录
    QDateTime checktime = outtime_ifh.isValid() ? outtime_ifh : QDateTime::currentDateTime();
    qint64 differsecs = intime.secsTo(checktime);
    int differhours = differsecs <= 0 ? 1 : (differsecs + 3599) / 3600;//不足一小时按一小时算
    return differhours == 1 ? firsthourcost : (firsthourcost + laterhourcost * (differhours - 1));

    /*
    if (outtime_ifh.isValid()) {
        qint64 differsecs = intime.secsTo(outtime_ifh);
        int differhours = (differsecs + 3599) / 3600;//不足一小时按一小时算
        return differhours == 1 ? firsthourcost : (firsthourcost + laterhourcost * (differhours - 1));
    } else {
        qint64 differsecs = intime.secsTo(QDateTime::currentDateTime());
        int differhours = (differsecs + 3599) / 3600;//不足一小时按一小时算
        return differhours == 1 ? firsthourcost : (firsthourcost + laterhourcost * (differhours - 1));
    }
    */
}

void parkingsystem::generatemockdata() {
    const QDateTime now = QDateTime::currentDateTime();

    activecar.push_back({"粤A88888", now.addSecs(-1800), QDateTime(), 0, QDateTime(), 0});
    activecar.push_back({"粤B66666", now.addSecs(-3600), QDateTime(), 0, QDateTime(), 1});
    activecar.push_back({"粤C12345", now.addSecs(-7200), QDateTime(), 0, QDateTime(), 2});

    historycar.push_back({"粤D99999", now.addDays(-1), now.addSecs(-79200), 0, QDateTime(), 0});
    historycar.push_back({"粤E11111", now.addDays(-2), now.addSecs(-187200), 0, QDateTime(), 5});
}
/*
void parkingsystem::xx() {
    int parknum = parkcfg.cfg_parknum();
    int unused = qHash(plate) % parknum;
    QSet<int> usedcar;
    for (const auto& car : activecar) usedcar.insert(car.spotposition);
    int parknum2 = parknum;
    while (parknum2--) {
        if(!usedcar.contains(unused)) break;
        unused++;
        unused %= parknum;
    }
    //push unused

}//abandon
*/