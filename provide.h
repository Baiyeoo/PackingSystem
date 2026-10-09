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
        int spotposition = -1;//补全车位逻辑
        bool operator==(const Car& other) const { return plate == other.plate; }
    };//提供可识别引用类型vector,deque<Car>里的Car类型
    const std::vector<Car>& getactivecar() const { return activecar; };
    const std::deque<Car>& getwaitingcar() const { return waitingcar; };
    const std::vector<Car>& gethistorycar() const {return historycar; };//暴露不可变引用数据层
    void generatemockdata();
    //const std::vector<int>& getspotstatus() const;

private:
    std::vector<Car> activecar;
    std::deque<Car> waitingcar;
    std::vector<Car> historycar;
    //std::vector<int> spotstatus;//单独维护停车场空闲车位;AI提示说我需要往业务层塞个可供维护UI车位显示的数组，
    //我想了想这个明显是过度设计了，只要业务层维护好了Car的spotposition字段，UI显示完全可以通过activecar设计一个UI层的函数;
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
