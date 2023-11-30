#include "ultimate.h"

#define WELCOME "Welcome to AAUCS"
#define _GROUP_ "      NO.13     "
#define SETTING_NUM 6

extern uchar DC[];

extern uchar str1[];
extern uchar str2[];
extern uchar str3[];
extern uchar str5[];

// 第一页变量
extern char upperLimit;
extern char lowerLimit;
extern float temp;
extern char fanGear;
extern uchar fanGearStep;

// 第二页变量
extern float highest;
extern float lowest;

// 第三页变量

extern uchar hms, hs, hm; // 开机后 超过温度上限 时间
extern uchar lms, ls, lm; // 开机后 低于温度下限 时间

extern uint SHOW_WAIT;

extern uchar key, pressKey;
extern uchar page;
extern bit pageChange;

extern uchar dsr;

extern uchar ringtone;

extern uchar volume;

extern uchar option;

extern changeCount;

extern _nop_(void);

void Delay1ms(uint t) // 12MHz
{                     // 软件延迟 参数用uchar 比uint 精准
    uchar i;
    while (t--) // mov dec mov jnz orl jz : 8us
    {
        i = 142;    // mov : 1us
        while (--i) // dec mov jz : 4us * 142
        {
            _nop_();
        } // nop jmp : 3us * 141
    }     // 每当低位为 0 会多1us处理高位(dec) 忽略
} // (8+1+7*142-3)us * t + 6us 约 t ms

// ============== LCD1602 ==============

void LCD1602_ShowString(uchar* s)
{
    while (*s)
    {
        LCD1602_WriteData(*s++);
        Delay1ms(SHOW_WAIT);
    }
}

void LCD1602_ScreenShiftRight(uchar num)
{
    while (num--)
    {
        LCD1602_WriteCmd(Shift_ScreenRight); // 命令5
        Delay1ms(SHOW_WAIT);
    }
}

void LCD1602_Action(void)
{
    uchar i;
    // 开启 LCD1602 显示 (initial)
    LCD1602_WriteCmd(Set_8bit_2line_5x7);   // 命令6
    LCD1602_WriteCmd(Show_CursorOff);       // 命令4
    LCD1602_WriteCmd(Mode_CursorRightMove); // 命令3
    LCD1602_WriteCmd(Clear_Screen);         // 命令1

    // // 开机界面
    // LCD1602_WriteCmd(Move_Cursor_Row1_Col(16)); // 命令8 设置光标在显示屏之外
    // LCD1602_ShowString(WELCOME);
    // SHOW_WAIT = 100;
    // LCD1602_WriteCmd(Move_Cursor_Row2_Col(16)); // 命令8
    // LCD1602_WriteCmd(Mode_ScreenRightMove);     // 命令3
    // LCD1602_ShowString(_GROUP_); // 一边输出第二行 一边移动屏幕
    // LCD1602_WriteCmd(Mode_CursorRightMove); // 命令3  恢复光标自增

    // i = 3;
    // do // 闪烁三次
    // {
    //     LCD1602_WriteCmd(Show_ScreenOff); // 命令4
    //     Delay1ms(SHOW_WAIT * 2);
    //     LCD1602_WriteCmd(Show_CursorOff); // 命令4
    //     Delay1ms(SHOW_WAIT * 2);
    // } while (--i);

    // LCD1602_ScreenShiftRight(16);   // 字体移出屏幕
    LCD1602_WriteCmd(Clear_Screen); // 命令1 清屏
    SHOW_WAIT = 0;
} // LCD1602 Action

void ViewPage_1(void)
{
    // 第一行 温度上限 温度下限
    LCD1602_WriteCmd(Clear_Screen); // 命令1 清屏
    LCD1602_ShowString("H:");
    Int8ToString(upperLimit, str3, 3);
    LCD1602_ShowString(str3);
    LCD1602_ShowString(DC);
    LCD1602_ShowString("  L:");
    Int8ToString(lowerLimit, str3, 3);
    LCD1602_ShowString(str3);
    LCD1602_ShowString(DC);
    // 第二行 温度 风扇档位
    LCD1602_WriteCmd(Move_Cursor_Row2_Col(0));
    LCD1602_ShowString("T:");
    FloatToString(temp, str5, 5, 1);
    LCD1602_ShowString(str5);
    LCD1602_ShowString(DC);
    LCD1602_ShowString("  FAN:");
    Int8ToString(fanGear, str1, 1);
    LCD1602_ShowString(str1);
}

void ViewPage_2(void)
{
    // 第一行 自开机后的最高温
    LCD1602_WriteCmd(Clear_Screen); // 命令1 清屏
    LCD1602_ShowString("Highest: ");
    FloatToString(highest, str5, 5, 1);
    LCD1602_ShowString(str5);
    LCD1602_ShowString(DC);
    // 第二行 自开机后的最低温
    LCD1602_WriteCmd(Move_Cursor_Row2_Col(0));
    LCD1602_ShowString("Lowest:  ");
    FloatToString(lowest, str5, 5, 1);
    LCD1602_ShowString(str5);
    LCD1602_ShowString(DC);
}

void ViewPage_3(void)
{
    // 第一行 超过温度上限时长
    LCD1602_WriteCmd(Clear_Screen); // 命令1 清屏
    LCD1602_ShowString("H Time: ");
    Int8ToString(hm, str2, 2);
    LCD1602_ShowString(str2);
    LCD1602_ShowString("m");
    Int8ToString(hs, str2, 2);
    LCD1602_ShowString(str2);
    LCD1602_ShowString(".");
    Int8ToString(hms, str1, 1);
    LCD1602_ShowString(str1);
    LCD1602_ShowString("s");
    // 第二行 低于温度下限时长
    LCD1602_WriteCmd(Move_Cursor_Row2_Col(0));
    LCD1602_ShowString("L Time: ");
    Int8ToString(lm, str2, 2);
    LCD1602_ShowString(str2);
    LCD1602_ShowString("m");
    Int8ToString(ls, str2, 2);
    LCD1602_ShowString(str2);
    LCD1602_ShowString(".");
    Int8ToString(lms, str1, 1);
    LCD1602_ShowString(str1);
    LCD1602_ShowString("s");
}

void ViewPage_4(void)
{
    LCD1602_WriteCmd(Clear_Screen); // 命令1 清屏
    // // 第一行 温感分辨率 风扇档位步长 开机音乐
    // LCD1602_WriteCmd(Clear_Screen); // 命令1 清屏
    // LCD1602_ShowString("R:");
    // LCD1602_ShowString();
    // LCD1602_ShowString("  FG:");
    // LCD1602_ShowString();
    // LCD1602_ShowString(DC);
    // LCD1602_ShowString("  M:");
    // LCD1602_ShowString();
    // // 第二行 音量
    // LCD1602_WriteCmd(Move_Cursor_Row2_Col(0));
    // LCD1602_ShowString("Volume: ");
    // LCD1602_ShowString();
}

void Settings(uchar opt)
{
    char i;
    uchar sgl[2][2] = {{0x7e, 0x7f}, {' ', ' '}};
    if (opt == option)
        return;
    if ((opt >> 1) != (option >> 1))
    {
        LCD1602_WriteCmd(Clear_Screen); // 命令1 清屏
        switch (opt >> 1)
        {
        case 0:
            // 0 温度上限
            LCD1602_WriteCmd(Move_Cursor_Row1_Col(1));
            LCD1602_ShowString("H Limit: ");
            Int8ToString(upperLimit, str3, 3);
            LCD1602_ShowString(str3);
            LCD1602_ShowString(DC);
            // 1 温度下限
            LCD1602_WriteCmd(Move_Cursor_Row2_Col(1));
            LCD1602_ShowString("L Limit: ");
            Int8ToString(lowerLimit, str3, 3);
            LCD1602_ShowString(str3);
            LCD1602_ShowString(DC);
            break;
        case 1:
            // 2 温感分辨率
            LCD1602_WriteCmd(Move_Cursor_Row1_Col(1));
            LCD1602_ShowString("TResolution: ");
            LCD1602_WriteData('0' + dsr);
            // 3 风扇档位步长
            LCD1602_WriteCmd(Move_Cursor_Row2_Col(1));
            LCD1602_ShowString("FGear' Step: ");
            LCD1602_WriteData('0' + fanGearStep);
            break;
        case 2:
            // 4 开机铃声
            LCD1602_WriteCmd(Move_Cursor_Row1_Col(2));
            LCD1602_ShowString("Ringtone:  ");
            LCD1602_WriteData('0' + ringtone);
            // 5 音量
            LCD1602_WriteCmd(Move_Cursor_Row2_Col(1));
            LCD1602_ShowString("Volume:");
            i = 0;
            while (i < volume)
            {
                LCD1602_WriteData(0xff);
                ++i;
            }
            break;
        }
    }
    i = opt & 1;
    LCD1602_WriteCmd(Move_Cursor_Row1_Col(0));
    LCD1602_WriteData(sgl[i][0]);
    LCD1602_WriteCmd(Move_Cursor_Row1_Col(15));
    LCD1602_WriteData(sgl[i][1]);
    i = !i;
    LCD1602_WriteCmd(Move_Cursor_Row2_Col(0));
    LCD1602_WriteData(sgl[i][0]);
    LCD1602_WriteCmd(Move_Cursor_Row2_Col(15));
    LCD1602_WriteData(sgl[i][1]);
    option = opt;
}

// void SettingChoose(bit row)
// {
//     uchar s[2][2] = {{0x7e, 0x7f}, {' ', ' '}};
//     LCD1602_WriteCmd(Move_Cursor_Row1_Col(0));
//     LCD1602_WriteData(s[row][0]);
//     LCD1602_WriteCmd(Move_Cursor_Row1_Col(15));
//     LCD1602_WriteData(s[row][1]);
//     row = !row;
//     LCD1602_WriteCmd(Move_Cursor_Row2_Col(0));
//     LCD1602_WriteData(s[row][0]);
//     LCD1602_WriteCmd(Move_Cursor_Row2_Col(15));
//     LCD1602_WriteData(s[row][1]);
// }

char KeysSystem_3(void);

void ChangeSetting(void)
{
    char i, j;
    uchar cunt = 24;
    bit flag = 0;
    switch (option)
    {
    case 0: // 这一段差异性和共性都多，为了节约一点内存，三目运算符 时间换空间
    case 1: // 这一段逻辑复杂，只用知道是增加/减少上限下限的
        i = option ? lowerLimit : upperLimit;
        while (1)
        {
            if (!--cunt)
            {
                cunt = 24;
                flag = !flag;
            }
            LCD1602_WriteCmd((option ? 0xc0 : 0x80) | 10);
            if (flag)
                LCD1602_ShowString("   ");
            else
            {
                Int8ToString(i, str3, 3);
                LCD1602_ShowString(str3);
            }
            switch (KeysSystem_3())
            {
            case 2:
                if (option)
                    lowerLimit = i;
                else
                    upperLimit = i;
            case -2: // 可能会因为时序造成空白 所以要
                LCD1602_WriteCmd((option ? 0xc0 : 0x80) | 10);
                Int8ToString(option ? lowerLimit : upperLimit, str3, 3);
                LCD1602_ShowString(str3); // 刷新一下数据
                return;
            case -1:
                if (i > -55 && ((i > lowerLimit + 1) || option))
                    --i;
                break;
            case 1:
                if (i < 127 && ((i < upperLimit - 1) || !option))
                    ++i;
                break;
            }
        }
    case 4: // 这一部分代码几乎一样，就复用了用来增大/减小选项值
        i = ringtone; // 0 - 3
    case 3:
        if (option == 3)
            i = fanGearStep; // 0 - 7
    case 2:
        if (option == 2)
            i = dsr; // 0 - 3
        LCD1602_WriteCmd((option & 1 ? 0xc0 : 0x80) | (option == 4 ? 14 : 15));
        while (1)
        {
            if (!--cunt)
            {
                cunt = 24;
                flag = !flag;
            }
            if (flag)
                j = ' ';
            else
                j = '0' + i;
            LCD1602_WriteCmd(Shift_CursorLeft);
            LCD1602_WriteData(j);
            switch (KeysSystem_3())
            {
            case 2:
                if (option == 4)
                    ringtone = i;
                else if (option == 3)
                    fanGearStep = i;
                else if (option == 2)
                    dsr = i;
            case -2:
                j = option == 4 ? ringtone : (option == 3 ? fanGearStep : dsr);
                LCD1602_WriteCmd(Shift_CursorLeft);
                LCD1602_WriteData(j + '0');
                return;
            case -1:
                if (i > 0)
                    --i;
                break;
            case 1:
                if (i < 3 + option == 3 ? 4 : 0)
                    ++i;
                break;
            }
        }
    case 5:
        break;
    }
}

// -------------------------------------

bit CheckKeysInvalid()
{
    uchar old = key;
    Delay1ms(10);
    key = 0x0f;
    key |= KEYS;
    return key != old;
}

void KeysSystem_1(void)
{
    KEYS = 0xff;
    key = 0x0f; // 排除 P33 P32 P31 P30
    key |= KEYS;
    switch (key)
    {
    case 0x7f: // P37
    case 0xbf: // P36
    case 0xdf: // P35
    case 0xef: // P34
        if (page == key || CheckKeysInvalid())
            return;
        pressKey = key;
        break;
    case 0xff: // 松开 上升沿触发
        if (pressKey == 0xff || CheckKeysInvalid())
            return;
        pageChange = 1;
        page = pressKey;
    default:
        pressKey = 0xff;
    }
}

void KeysSystem_2(void)
{
    uchar opt;
    KEYS = 0xff;
    key = 0x0f; // 排除 P33 P32 P31 P30
    key |= KEYS;
    switch (key)
    {
    case 0x7f: // P37
    case 0xbf: // P36
    case 0xdf: // P35
    case 0xef: // P34
        if (CheckKeysInvalid())
            return;
        pressKey = key;
        break;
    case 0xff: // 松开 上升沿触发
        if (pressKey == 0xff || CheckKeysInvalid())
            return;
        // 执行按键功能
        switch (pressKey)
        {
        case 0x7f: // P37 进入设置
            EA = 0;
            pressKey = 0xff;
            ChangeSetting();
            pressKey = 0xff;
            EA = 1;
            return;
        case 0xbf: // P36 下一条
            opt = option + 1;
            if (opt == SETTING_NUM)
                opt = 0;
            break;
        case 0xdf: // P35 上一条
            if (!option)
                opt = SETTING_NUM - 1;
            else
                opt = option - 1;
            break;
        case 0xef: // P34 首条
            opt = 0;
        }
        Settings(opt);
    default:
        pressKey = 0xff;
    }
}

char KeysSystem_3(void)
{
    KEYS = 0xff;
    key = 0x0f; // 排除 P33 P32 P31 P30
    key |= KEYS;
    if (CheckKeysInvalid())
        return 0; // 为了稳定的时序，所以在这里消抖
    switch (key)
    {
    // 低电平触发，长按加快变化
    case 0xbf: // P36 增加
    case 0xdf: // P35 减少
        if (pressKey == key)
        {
            if (!changeCount)
            {
                ++changeCount;
                return key == 0xbf ? 1 : -1;
            }
            if (++changeCount - 80 > 0 && !(changeCount & 0x0f))
            {
                return key == 0xbf ? 1 : -1;
            }
            return 0;
        }
        else
        {
            changeCount = 0;
        }
    // 上升沿触发
    case 0x7f: // P37 取消
    case 0xef: // P34 确定
        pressKey = key;
        break;
    case 0xff: // 松开 上升沿触发
        if (pressKey == 0xff)
            return 0;
        // 执行按键功能
        switch (pressKey)
        {
        case 0x7f: // P37 取消
            pressKey = 0xff;
            return -2;
        // case 0xbf: // P36 增加
        //     pressKey = 0xff;
        //     return 1;
        // case 0xdf: // P35 减少
        //     pressKey = 0xff;
        //     return -1;
        case 0xef: // P34 确定
            pressKey = 0xff;
            return 2;
        }
    default:
        pressKey = 0xff;
    }
    return 0;
}

// void Clock(void)
// {
//     uchar us, ms, s, m;
//     if (++us == 180)
//     {
//         us = 0;
//         if (++ms == 20)
//         {
//             ms = 0;
//             if (++s == 60)
//             {
//                 s = 0;
//                 if (++m == 60)
//                 {
//                     m = 0;
//                 }
//             }
//         }
//     }
// }