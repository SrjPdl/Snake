#include <dos.h>
#include <graphics.h>
#include <stdio.h>
#include <windows.h>

int EXIT = 0;
enum flag
{
    none,
    left,
    right,
    up,
    down
};
int area_x1 = 50, area_y1 = 80, area_x2 = 450, area_y2 = 480;
int factor = 10;
int pl_x1, pl_y1, pl_x2, pl_y2;
int tailx1[200] = {0};
int taily1[200] = {0};
int tailno = 0;

static int x1, y1;

void get_input(char c);

void play_area()
{
    setfillstyle(SOLID_FILL, BLACK);
    bar(area_x1, area_y1, area_x2, area_y2);
}
void player()
{
    pl_x1 = (area_x2 + area_x1) / 2;
    pl_x1 /= factor;
    pl_x1 *= factor;
    pl_y1 = (area_y2 + area_y1) / 2;
    pl_y1 /= factor;
    pl_y1 *= factor;
    pl_x2 = pl_x1 + factor;
    pl_y2 = pl_y1 + factor;
    setfillstyle(SOLID_FILL, WHITE);
    bar(pl_x1, pl_y1, pl_x2, pl_y2);
}
void food()
{
    SYSTEMTIME currtime;
    GetSystemTime(&currtime);
    setfillstyle(SOLID_FILL, RED);
    do
    {
        x1 = (currtime.wMilliseconds * rand()) % area_x2;
        y1 = (currtime.wMilliseconds * rand()) % area_y2;
        x1 /= factor;
        x1 *= factor;
        y1 /= factor;
        y1 *= factor;
    } while (x1 < area_x1 || x1 > area_x2 || y1 < area_y1 || y1 > area_y2);
    bar(x1, y1, x1 + factor, y1 + factor);
}
void cl(int x1, int y1)
{
    setfillstyle(SOLID_FILL, BLACK);
    bar(x1, y1, x1 + factor, y1 + factor);
}
void tail()
{
    int i;
    int prevx1 = tailx1[0];
    int prevy1 = taily1[0];
    int prevx2, prevy2;
    tailx1[0] = pl_x1;
    taily1[0] = pl_y1;
    for (i = 1; i <= tailno; i++)
    {
        if (tailx1[tailno] != 0 && taily1[tailno] != 0)
            cl(tailx1[tailno], taily1[tailno]);
        prevx2 = tailx1[i];
        prevy2 = taily1[i];
        tailx1[i] = prevx1;
        taily1[i] = prevy1;
        prevx1 = prevx2;
        prevy1 = prevy2;
        setfillstyle(SOLID_FILL, WHITE);
        if (tailx1[i] != 0 && taily1[i] != 0)
            bar(tailx1[i], taily1[i], tailx1[i] + factor, taily1[i] + factor);
        if (tailx1[0] == prevx2 && taily1[0] == prevy2)
        {
            EXIT = 1;
            break;
        }
    }
}
void control(flag ctrl)
{
    int i;
    char ch;
    int x = 110;
    char buffer[20];
    static int score;
    float speed_rev = 100;

    switch (ctrl)
    {
    case left:
        for (i = pl_x1; i >= area_x1; i -= factor)
        {
            if (tailno == 0)
                cl(pl_x1, pl_y1);
            pl_x1 -= factor;
            pl_x2 = pl_x1 + factor;
            setfillstyle(SOLID_FILL, WHITE);
            bar(pl_x1, pl_y1, pl_x2, pl_y2);
            if (pl_x1 < area_x1)
            {
                setfillstyle(SOLID_FILL, CYAN);
                bar(pl_x1, pl_y1, pl_x2, pl_y2);
                EXIT = 1;
            }
            if (pl_x1 == x1 && pl_y1 == y1)
            {

                score += 5;
                sprintf(buffer, "%d", score);
                outtextxy(x, 20, buffer);
                food();
                Beep(700, 80);
                tailno++;
            }
            if (tailno >= 1)
            {
                tail();
            }
            if (EXIT == 1)
                return;
            delay(speed_rev);
            if (kbhit())
            {
                ch = getch();
                if (tailno >= 1 && (ch == 'd' || ch == 'D'))
                    ch = 'a';
                break;
            }
        }
        get_input(ch);
        break;
    case right:
        for (i = pl_x2; i <= area_x2; i += factor)
        {
            cl(pl_x1, pl_y1);
            pl_x1 += factor;
            pl_x2 = pl_x1 + factor;
            setfillstyle(SOLID_FILL, WHITE);
            bar(pl_x1, pl_y1, pl_x2, pl_y2);
            if (pl_x2 > area_x2)
            {
                setfillstyle(SOLID_FILL, CYAN);
                bar(pl_x1, pl_y1, pl_x2, pl_y2);
                EXIT = 1;
            }
            if (pl_x1 == x1 && pl_y1 == y1)
            {

                score += 5;
                sprintf(buffer, "%d", score);
                outtextxy(x, 20, buffer);
                tailno++;
                food();
                Beep(700, 80);
            }
            if (tailno >= 1)
            {
                tail();
            }
            if (EXIT == 1)
                return;
            delay(speed_rev);
            if (kbhit())
            {
                ch = getch();
                if (tailno >= 1 && (ch == 'a' || ch == 'A'))
                    ch = 'd';
                break;
            }
        }
        get_input(ch);
        break;
    case up:
        for (i = pl_y1; i >= area_y1; i -= factor)
        {
            cl(pl_x1, pl_y1);
            pl_y1 -= factor;
            pl_y2 = pl_y1 + factor;
            setfillstyle(SOLID_FILL, WHITE);
            bar(pl_x1, pl_y1, pl_x2, pl_y2);
            if (pl_y1 < area_y1)
            {
                setfillstyle(SOLID_FILL, CYAN);
                bar(pl_x1, pl_y1, pl_x2, pl_y2);
                EXIT = 1;
                break;
            }
            if (pl_y1 == y1 && pl_x1 == x1)
            {

                score += 5;
                sprintf(buffer, "%d", score);
                outtextxy(x, 20, buffer);
                tailno++;
                food();
                Beep(700, 80);
            }
            if (tailno >= 1)
            {
                tail();
            }
            if (EXIT == 1)
                return;
            delay(speed_rev);
            if (kbhit())
            {
                ch = getch();
                if (tailno >= 1 && (ch == 's' || ch == 'S'))
                    ch = 'w';
                break;
            }
        }
        get_input(ch);
        break;
    case down:
        for (i = pl_y2; i <= area_y2; i += factor)
        {
            cl(pl_x1, pl_y1);
            pl_y1 += factor;
            pl_y2 = pl_y1 + factor;
            setfillstyle(SOLID_FILL, WHITE);
            bar(pl_x1, pl_y1, pl_x2, pl_y2);
            if (pl_y2 > area_y2)
            {
                setfillstyle(SOLID_FILL, CYAN);
                bar(pl_x1, pl_y1, pl_x2, pl_y2);
                EXIT = 1;
            }
            if (pl_y1 == y1 && pl_x1 == x1)
            {
                score += 5;
                sprintf(buffer, "%d", score);
                outtextxy(x, 20, buffer);
                tailno++;
                food();
                Beep(700, 80);
            }
            if (tailno >= 1)
            {
                tail();
            }
            if (EXIT == 1)
                return;
            delay(speed_rev);
            if (kbhit())
            {
                ch = getch();
                if (tailno >= 1 && (ch == 'w' || ch == 'W'))
                    ch = 's';
                break;
            }
        }
        get_input(ch);
        break;
    case none:
        break;
    }
}
void get_input(char c)
{
    flag ctrl;
    switch (c)
    {
    case 'a':
    case 'A':
        ctrl = left;
        break;
    case 'd':
    case 'D':
        ctrl = right;
        break;
    case 'w':
    case 'W':
        ctrl = up;
        break;
    case 's':
    case 'S':
        ctrl = down;
        break;
    default:
        break;
    }
    control(ctrl);
}
void exit_window()
{
    cleardevice();
}
int main()
{
    char c;
    initwindow(500, 500, "SNAKE", 100, 100, false, true);
    setbkcolor(CYAN);
start:
    cleardevice();
    outtextxy(200, 20, "Made by SURAJ POUDEL");
    play_area();
    food();
    player();
    outtextxy(50, 20, "SCORE:  0");
    while (!EXIT)
    {

        if (kbhit())
        {
            c = getch();
            get_input(c);
        }
    }
    outtextxy(((area_x1 + area_x2) - 10) / 2, (area_y1 + area_y2) / 2, "GAME OVER");
    Beep(670, 100);
    delay(5);
    Beep(700, 50);
    outtextxy(((area_x1 + area_x2) - 10) / 2, (area_y1 + area_y2) / 2, "GAME OVER");
    outtextxy(((area_x1 + area_x2) - 10) / 2 - 100, (area_y1 + area_y2) / 2 + 15, "Press R to play again. Any other key to exit");
    c = getch();
    if (c == 'R' || c == 'r')
    {
        EXIT = 0;
        tailno = 0;
        tailx1[200] = {0};
        taily1[200] = {0};
        goto start;
    }
    else
        return 0;
}
