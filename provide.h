#ifndef PROVIDE_H
#define PROVIDE_H
#include <QString>
#include <QDateTime>
#include <vector>
#include <deque>
class parkingsystem {
public:
    bool carin(QString plate);
    bool carout(QString plate);

    struct Car {
        QString plate;
        QDateTime intime;
        QDateTime outtime;
        int dequenumber;
        QDateTime indequetime;
        //int cost;
        bool operator==(const Car& other) const { return plate == other.plate; }
    };//提供可识别引用类型vector,deque<Car>里的Car类型
    const std::vector<Car>& getactivecar() const { return activecar; };
    const std::deque<Car>& getwaitingcar() const { return waitingcar; };
    const std::vector<Car>& gethistorycar() const {return historycar; };//暴露不可变引用数据层
    void generatemockdata();

private:
    std::vector<Car> activecar;
    std::deque<Car> waitingcar;
    std::vector<Car> historycar;

    class parkingsystemconfig {
    private:
        int parknum = 15;
        int firsthourcost = 10;
        int laterhourcost = 5;

    public:
        int cfg_parknum() { return parknum; };
        int cfg_firsthourcost() { return firsthourcost; };
        int cfg_laterhourcost() { return laterhourcost; };

    };

public:
    parkingsystemconfig parkcfg;
    int anticipatesettle(QDateTime intime, QDateTime outtime_ifh);
};


#endif
