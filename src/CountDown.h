#ifndef __COUNT_DOWN_H
#define __COUNT_DOWN_H
#include <Arduino.h>

class CountDown 
{
private:
    uint8_t _hour;     // 时 (0-23)
    uint8_t _minute;   // 分 (0-59)
    uint8_t _second;   // 秒 (0-59)

    uint8_t total_hour;     // 时 (0-23)
    uint8_t total_minute;   // 分 (0-59)
    uint8_t total_second;   // 秒 (0-59)

    bool _isRunning;   // 计时状态
    unsigned long _lastTick;  // 上次心跳时间戳

    // 内部：将时间转为总秒数（新增----xi）
    unsigned long toSeconds(uint8_t h, uint8_t m, uint8_t s) const
    {
        return h * 3600UL + m * 60UL + s;
    }

public:
    /* 原始程序 */
    // // 构造函数：初始化为00:00:00或指定时间
    // CountDown(uint8_t h = 0, uint8_t m = 0, uint8_t s = 0)
    //     : _hour(h), _minute(m), _second(s), _isRunning(false), _lastTick(0) {}

    //构造函数：将时间初始化为0（修改----xi）
    CountDown(uint8_t h = 0, uint8_t m = 0, uint8_t s = 0)
        : _hour(h % 24), _minute(m % 60), _second(s % 60),
          total_hour(h % 24), total_minute(m % 60), total_second(s % 60),
          _isRunning(false), _lastTick(0) {}
    // 设置时间
    void setTime(uint8_t h, uint8_t m, uint8_t s) {
        _hour = h % 24;
        _minute = m % 60;
        _second = s % 60;
    }

      // 设置时间
    void setTotalTime(uint8_t h, uint8_t m, uint8_t s) {
        total_hour = h % 24;
        total_minute = m % 60;
        total_second = s % 60;
    }

    // 获取时间
    void getTime(uint8_t& h, uint8_t& m, uint8_t& s) const {
        h = _hour;
        m = _minute;
        s = _second;
    }
    /* 原始程序 */
    // // 获取格式化时间字符串 (HH:MM:SS)
    // int getPercent(void) const 
    // {
    //     float total_min_current = _hour*60+_minute+(_second/60.0);
    //     float total_min_target =  total_hour*60+total_minute+(total_second/60.0);
    //     return (int)(100*total_min_current/total_min_target);
    // }
 
    // 获取倒计时进度百分比（已过去时间占比）修改----xi
    int getPercent() const {
        unsigned long totalSec = toSeconds(total_hour, total_minute, total_second);
        if (totalSec == 0) return 100;

        unsigned long currentSec = toSeconds(_hour, _minute, _second);
        unsigned long elapsedSec = totalSec - currentSec;

        return (int)((elapsedSec * 100.0) / totalSec);
    }

    // 开始计时
    void start() {
        _isRunning = true;
        _lastTick = millis();
    }

    // 停止计时
    void stop() {
        _isRunning = false;
    }

    // 检查计时状态
    bool isRunning() const {
        return _isRunning;
    }

    // 心跳函数：需在主循环中定期调用
    void tick() 
    {
        //修改----xi
        if (!_isRunning) return;

        unsigned long now = millis();
        if (now - _lastTick >= 1000)
        {
            _lastTick = now;
            decrementSecond();  // 每隔1秒减1秒
        }

        /* 原始程序 */
        // if (!_isRunning) return;
        // else
        // {
        //     decrementSecond();
        //     if(_hour==0&_minute==0&_second==0)
        //     {
        //         Serial.print("触发泄压\n");
        //     }
        // }
    }

private:
    // 增加1秒（内部使用）
    void decrementSecond() 
    {
        if (_second > 0)
        {
            _second--;
        }
        else
        {
            if (_minute > 0)
            {
                _minute--;
                _second = 59;
            }
            else
            {
                if (_hour > 0)
                {
                    _hour--;
                    _minute = 59;
                    _second = 59;
                }
                else
                {
                    // 倒计时结束
                    _second = 0;
                    _isRunning = false;
                    Serial.println("触发泄压");
                }
            }
        }

        /* 原始程序 */
        // if (_second ==0) 
        // {
        //     if (_minute ==0) 
        //     {
        //         if(_hour==0)
        //         {
                    
        //         }
        //         else
        //         {
        //             _minute = 60;
        //             _hour = (_hour-1);
        //         }
        //     }
        //     else
        //     {
        //         _second = 60;
        //         _minute--;
        //     }
        // }
        // else
        // {
        //      _second--;
        // }
    }
};

extern CountDown count_down_time;
void vTimerCallback(TimerHandle_t xTimer);

#endif


