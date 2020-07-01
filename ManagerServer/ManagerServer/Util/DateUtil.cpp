#include "DateUtil.h"
#include <ctime> // c : time.h

string DateUtil::Now()
{
    time_t timeCur;     // time.h ���� �ð����� ����� Ÿ��
    struct tm tmTemp;  // time.h ���� �ð����� ������ Ÿ��
    timeCur = time(NULL);
    localtime_s(&tmTemp, &timeCur);
    char buf[24] = { 0, };
    strftime(buf, sizeof buf, "%Y-%m-%d %X", &tmTemp);
    return string(buf);
}

string DateUtil::CustomDatetime(struct Datetime& dt)
{
    struct tm curr_tm;
    char buf[24] = { 0, };
    // �ð� ����    
    curr_tm.tm_year = dt.GetYear() - 1900; // since 1900 : 1900�� �������
    curr_tm.tm_mon = (dt.GetMonth() -1); // 0~11(1~12��)
    curr_tm.tm_mday = dt.GetDay(); // 1~31 
    curr_tm.tm_hour = dt.GetHour();
    curr_tm.tm_min = dt.GetMinute();
    curr_tm.tm_sec = dt.GetSecond();
    mktime(&curr_tm); // ptmTemp->tm_wday : ���� ����
    strftime(buf, sizeof buf, "%Y-%m-%d %X", &curr_tm);
    return string(buf);
}
