#ifndef SJTU_UTILITY_HPP
#define SJTU_UTILITY_HPP

#include <utility>
#include <iostream>
#include <sstream>
#include <iomanip>
namespace sjtu {

template <class T>
T* upper_bound(T* first,T* last, T key) {
    if(first==last)return first;
    last--;
    if(key>=*last)return last+1;
    while (first < last) {
        T* mid = first + (last - first) / 2;
        if (key < *mid) {
            last = mid;
        } else {
            first = mid + 1;
        }
    }
    return first;
}
template <class T>
T* lower_bound(T *first,T *last, T key) {
    if(first==last)return first;
    last--;
    if(key>*last)return last+1;
    while (first < last) {
        T* mid = first + (last - first) / 2;
        if (*mid < key) {
            first = mid + 1;
        } else {
            last = mid;
        }
    }
    return first;
}

template<typename T>
void merge_sort(T* left, T* right) {
    if (left >= right) return;

    T* middle = left + (right - left) / 2;

    merge_sort(left, middle);
    merge_sort(middle + 1, right);

    T* temp = new T[right - left + 1];
    T* p1 = left, *p2 = middle + 1, *p3 = temp;
    while (p1 <= middle && p2 <= right) {
        if (*p1 < *p2) {
            *p3++ = *p1++;
        }
        else {
            *p3++ = *p2++;
        }
    }
    while (p1 <= middle) {
        *p3++ = *p1++;
    }
    while (p2 <= right) {
        *p3++ = *p2++;
    }
    for (T* p = temp, *q = left; p3-- != temp;) {
        *q++ = *p++;
    }
    delete[] temp;
}


template<class T1, class T2>
class pair {
public:
	T1 first;
	T2 second;
	constexpr pair() : first(), second() {}
	pair(const pair &other) = default;
	pair(pair &&other) = default;
	pair(const T1 &x, const T2 &y) : first(x), second(y) {}
	template<class U1, class U2>
	pair(U1 &&x, U2 &&y) : first(x), second(y) {}
	template<class U1, class U2>
	pair(const pair<U1, U2> &other) : first(other.first), second(other.second) {}
	template<class U1, class U2>
	pair(pair<U1, U2> &&other) : first(other.first), second(other.second) {}
};

class daytime
{
public:
    int hh,mm;
    daytime(){}
    daytime(int hh_,int mm_):hh(hh_),mm(mm_){}
    daytime(const daytime &other):hh(other.hh),mm(other.mm){}
    int operator-(const daytime &other){
        int c1;
        if(other.hh<hh){
            c1=other.hh+24-hh;
            return c1*60+other.mm-mm;
        }
        else if(other.hh==hh && other.mm<mm){
            return 1440-mm+other.mm;
        }
        else return (other.hh-hh)*60+other.mm-mm;
    }
    daytime operator+=(const int &minute){
        mm+=minute;
        if(mm>=60){
            int t=mm/60;
            mm=mm%60;
            hh+=t;
            if(t>=24)t%=24;
        }
        return *this;
    }
    daytime operator-=(const int &minute){
        mm-=minute;
        if(mm<0){
            int t=-(mm+1)/60+1;
            mm+=t*60;
            hh-=t;
            if(hh<0)hh+=(-(hh+1)/24+1)*24;
        }
        return *this;
    }
};
class monthtime
{
public:
    int mm,dd;
    monthtime(){}
    monthtime(int mm_,int dd_):mm(mm_),dd(dd_){}
    monthtime(const monthtime &other):mm(other.mm),dd(other.dd){}
    // int operator- (const monthtime &other){
    //     if(other.mm-)
    // }
};
}
// class date
// {
// public:
//     sjtu::monthtime m;
//     sjtu::daytime d;
//     date(){}
//     date(sjtu::monthtime m_,sjtu::daytime d_):m(m_),d(d_){}
//     friend std::ostream & operator<<(std::ostream &os,const date &obj){
//         os<<obj.m.mm<<'-'<<obj.m.dd<<' '<<obj.d.hh<<':'<<obj.d.mm;
//         return os;
//     }
// };

class Time {
public:
    Time(){}
    Time(int month, int day, int hour, int minute)
        : month_(month), day_(day), hour_(hour), minute_(minute) {}
    Time(sjtu::monthtime m_,sjtu::daytime d_=sjtu::daytime()):month_(m_.mm),day_(m_.dd),hour_(d_.hh),minute_(d_.mm){}
    int getMonth() const { return month_; }
    int getDay() const { return day_; }
    int getHour() const { return hour_; }
    int getMinute() const { return minute_; }

    void setMonth(int month) { month_ = month; }
    void setDay(int day) { day_ = day; }
    void setHour(int hour) { hour_ = hour; }
    void setMinute(int minute) { minute_ = minute; }

    std::string toString() const {
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << month_ << "-" << std::setw(2) << std::setfill('0') << day_
            << " " << std::setw(2) << std::setfill('0') << hour_ << ":" << std::setw(2) << std::setfill('0') << minute_;
        return oss.str();
    }

    Time operator+(int minutes) const {
        Time result = *this;
        result.minute_ += minutes;

        result.hour_+=result.minute_/60;
        result.minute_ %= 60;
            
        if (result.hour_ >= 24) {
            result.day_+=result.hour_/24;
            result.hour_%=24;
            
            if (result.day_ > days_in_month(result.month_)) {
                result.day_ -= days_in_month(result.month_);
                result.month_++;
            }
        }
        
        return result;
    }
    Time operator-(int minutes) const {
        Time result(*this);

        // 计算总共需要借位的分钟数和需要借位的天数
        int borrow_minutes = minutes % (24 * 60);
        int borrow_days = minutes / (24 * 60);

        // 计算需要借位的小时数和分钟数
        int borrow_hour = borrow_minutes / 60;
        int borrow_minute = borrow_minutes % 60;

        // 依次进行借位和进位操作
        result.minute_ -= borrow_minute;
        if (result.minute_ < 0) {
            result.minute_ += 60;
            borrow_hour++;
        }

        result.hour_ -= borrow_hour;
        if (result.hour_ < 0) {
            result.hour_ += 24;
            borrow_days++;
        }

        // 处理日期借位的逻辑
        while (borrow_days > 0) {
            int days_in_prev_month = days_in_month(result.month_ - 1);
            if (result.day_ <= borrow_days) {
                borrow_days -= result.day_;
                result.month_--;
                if (result.month_ == 0) {
                    result.month_ = 12;
                    // 如果当前月份是 1 月，则需要借位到上一年的 12 月
                    result.day_ = days_in_month(result.month_) - borrow_days % days_in_prev_month;
                } else {
                    result.day_ = days_in_month(result.month_);
                }
            } else {
                result.day_ -= borrow_days;
                borrow_days = 0;
            }
        }

        return result;
    }
    int operator-(const Time& other) const {
        int days=0;
        if(month_!=other.month_){
            for(int i=other.month_+1;i<=month_-1;i++)
                days+=days_in_month(i);
            days+=days_in_month(other.month_)-other.day_;
            days+=day_;
        }
        else days=day_-other.day_;
        // 计算当前对象和另一个对象的分钟数差
        int minutes = days * 24 * 60 + (hour_ - other.hour_) * 60 + (minute_ - other.minute_);
        return minutes;
    }
    int days_to(const Time& other){
        int days=0;
        if(month_!=other.month_){
            for(int i=other.month_+1;i<=month_-1;i++)
                days+=days_in_month(i);
            days+=days_in_month(other.month_)-other.day_;
            days+=day_;
        }
        else days=day_-other.day_;
        return days;
    }
    bool operator<(const Time& other) const {
        if (month_ != other.month_) {
            return month_ < other.month_;
        } else if (day_ != other.day_) {
            return day_ < other.day_;
        } else if (hour_ != other.hour_) {
            return hour_ < other.hour_;
        } else {
            return minute_ < other.minute_;
        }
    }

    bool operator>(const Time& other) const {
        return other < *this;
    }

    bool operator==(const Time& other) const {
        return month_ == other.month_ && day_ == other.day_ && hour_ == other.hour_ && minute_ == other.minute_;
    }

    bool operator<=(const Time& other) const {
        return *this < other || *this == other;
    }

    bool operator>=(const Time& other) const {
        return *this > other || *this == other;
    }

private:
    int month_;
    int day_;
    int hour_;
    int minute_;

    int days_in_month(int month) const {
        // if (month == 2) {
        //     // 判断闰年
        //     int year = 2023;  // 当前年份
        //     if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        //         return 29;
        //     } else {
        //         return 28;
        //     }
        // } else 
        if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
        else return 31;
    }
};

#endif
