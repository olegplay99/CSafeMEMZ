#include "effects.h"

#include <stdlib.h>
#include <math.h>

static HWND g_window = NULL;

void EffectsInit(HWND hwnd) { g_window = hwnd; }
void EffectsShutdown(void) { g_window = NULL; }

static int ClampInt(int v, int lo, int hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static void BlitOffset(HDC hdc, int width, int height, int dx, int dy)
{
    int sx = dx < 0 ? -dx : 0;
    int sy = dy < 0 ? -dy : 0;
    int w = width - abs(dx);
    int h = height - abs(dy);
    if (w <= 0 || h <= 0) return;
    BitBlt(hdc, dx, dy, w, h, hdc, sx, sy, SRCCOPY);
}

void EffectGlitch(HDC hdc, int width, int height, DWORD elapsed)
{
    int offset = (int)((elapsed / 35) % 41) - 20;
    BlitOffset(hdc, width, height, offset, 0);
}

void EffectShake(HDC hdc, int width, int height, DWORD elapsed)
{
    int x = (int)((elapsed / 11) % 31) - 15;
    int y = (int)((elapsed / 17) % 25) - 12;
    BlitOffset(hdc, width, height, x, y);
}

void EffectTear(HDC hdc, int width, int height, DWORD elapsed)
{
    int i;
    srand((unsigned int)(elapsed / 31));
    for (i = 0; i < 24; ++i)
    {
        int y = rand() % (height > 1 ? height : 1);
        int h = 2 + rand() % 36;
        int dx = (rand() % 161) - 80;
        if (y + h > height) h = height - y;
        if (h <= 0) continue;
        if (dx >= 0)
            BitBlt(hdc, dx, y, width - dx, h, hdc, 0, y, SRCCOPY);
        else
            BitBlt(hdc, 0, y, width + dx, h, hdc, -dx, y, SRCCOPY);
    }
}

void EffectColorShift(HDC hdc, int width, int height, DWORD elapsed)
{
    int offset = (int)((elapsed / 18) % 31) - 15;
    HPEN red = CreatePen(PS_SOLID, 2, RGB(255,0,0));
    HPEN cyan = CreatePen(PS_SOLID, 2, RGB(0,255,255));
    HPEN old;
    int y;
    if (!red || !cyan)
    {
        if (red) DeleteObject(red);
        if (cyan) DeleteObject(cyan);
        return;
    }
    old = (HPEN)SelectObject(hdc, red);
    for (y = 0; y < height; y += 24)
    {
        MoveToEx(hdc, offset, y, NULL);
        LineTo(hdc, width, y);
    }
    SelectObject(hdc, cyan);
    for (y = 12; y < height; y += 24)
    {
        MoveToEx(hdc, -offset, y, NULL);
        LineTo(hdc, width, y);
    }
    SelectObject(hdc, old);
    DeleteObject(red);
    DeleteObject(cyan);
}

void EffectRects(HDC hdc, int width, int height, DWORD elapsed)
{
    int i;
    srand((unsigned int)(elapsed / 42));
    for (i = 0; i < 38; ++i)
    {
        int x = rand() % (width > 1 ? width : 1);
        int y = rand() % (height > 1 ? height : 1);
        int w = 8 + rand() % 220;
        int h = 3 + rand() % 48;
        HBRUSH brush;
        RECT r;
        if (x + w > width) w = width - x;
        if (y + h > height) h = height - y;
        if (w <= 0 || h <= 0) continue;
        brush = CreateSolidBrush(RGB(rand()%256, rand()%256, rand()%256));
        if (!brush) continue;
        r.left=x; r.top=y; r.right=x+w; r.bottom=y+h;
        FillRect(hdc, &r, brush);
        DeleteObject(brush);
    }
}

void EffectFlash(HDC hdc, int width, int height, DWORD elapsed)
{
    HBRUSH brush;
    RECT r;
    if ((elapsed % 1200) >= 55) return;
    brush = CreateSolidBrush(RGB(185,185,185));
    if (!brush) return;
    r.left=0; r.top=0; r.right=width; r.bottom=height;
    FillRect(hdc, &r, brush);
    DeleteObject(brush);
}

void EffectSwirl(HDC hdc, int width, int height, DWORD elapsed)
{
    int y;
    const int band = 18;
    int shift = (int)((elapsed / 15) % 181) - 90;
    for (y = 0; y < height; y += band)
    {
        int h = band;
        int offset = (((y / band) & 1) ? shift : -shift);
        if (y + h > height) h = height - y;
        if (h <= 0) continue;
        if (offset >= 0) BitBlt(hdc, offset, y, width-offset, h, hdc, 0, y, SRCCOPY);
        else BitBlt(hdc, 0, y, width+offset, h, hdc, -offset, y, SRCCOPY);
    }
}

void EffectScanlines(HDC hdc, int width, int height, DWORD elapsed)
{
    int y;
    int offset = (int)(elapsed % 6);
    HBRUSH b = CreateSolidBrush(RGB(0,0,0));
    RECT r;
    if (!b) return;
    for (y = offset; y < height; y += 6)
    {
        r.left=0; r.top=y; r.right=width; r.bottom=ClampInt(y+2,0,height);
        FillRect(hdc, &r, b);
    }
    DeleteObject(b);
}

void EffectPixelate(HDC hdc, int width, int height, DWORD elapsed)
{
    int block = 5 + (int)((elapsed / 80) % 18);
    HDC tmp = CreateCompatibleDC(hdc);
    HBITMAP bmp;
    HBITMAP old;
    int x,y;
    if (!tmp) return;
    bmp = CreateCompatibleBitmap(hdc, width, height);
    if (!bmp) { DeleteDC(tmp); return; }
    old = (HBITMAP)SelectObject(tmp, bmp);
    BitBlt(tmp,0,0,width,height,hdc,0,0,SRCCOPY);
    SetStretchBltMode(hdc, COLORONCOLOR);
    for (y=0;y<height;y+=block)
        for (x=0;x<width;x+=block)
        {
            int w=block,h=block;
            if (x+w>width) w=width-x;
            if (y+h>height) h=height-y;
            if (w>0 && h>0)
                StretchBlt(hdc,x,y,w,h,tmp,x,y,1,1,SRCCOPY);
        }
    SelectObject(tmp,old);
    DeleteObject(bmp);
    DeleteDC(tmp);
}

void EffectMirror(HDC hdc, int width, int height, DWORD elapsed)
{
    HDC tmp = CreateCompatibleDC(hdc);
    HBITMAP bmp;
    HBITMAP old;
    (void)elapsed;
    if (!tmp) return;
    bmp = CreateCompatibleBitmap(hdc,width,height);
    if (!bmp) { DeleteDC(tmp); return; }
    old=(HBITMAP)SelectObject(tmp,bmp);
    BitBlt(tmp,0,0,width,height,hdc,0,0,SRCCOPY);
    StretchBlt(hdc,0,0,width,height,tmp,width,0,-width,height,SRCCOPY);
    SelectObject(tmp,old); DeleteObject(bmp); DeleteDC(tmp);
}

void EffectRGBSplit(HDC hdc, int width, int height, DWORD elapsed)
{
    int i;
    int offset=(int)((elapsed/24)%31)-15;
    HPEN red=CreatePen(PS_SOLID,2,RGB(255,0,0));
    HPEN blue=CreatePen(PS_SOLID,2,RGB(0,100,255));
    HPEN old;
    if(!red||!blue){if(red)DeleteObject(red);if(blue)DeleteObject(blue);return;}
    old=(HPEN)SelectObject(hdc,red);
    for(i=0;i<height;i+=18){MoveToEx(hdc,offset,i,NULL);LineTo(hdc,width/2,i);}
    SelectObject(hdc,blue);
    for(i=9;i<height;i+=18){MoveToEx(hdc,width/2-offset,i,NULL);LineTo(hdc,width,i);}
    SelectObject(hdc,old);DeleteObject(red);DeleteObject(blue);
}

void EffectWave(HDC hdc, int width, int height, DWORD elapsed)
{
    const int band=16;
    int y;
    for(y=0;y<height;y+=band)
    {
        int phase=(int)((y*7 + elapsed*2) % 240);
        int offset=(int)(sin(phase*3.14159265/120.0)*80.0);
        int w=width-abs(offset);
        if(y+band>height) { if(w<=0) continue; }
        if(w>0) BitBlt(hdc,offset,y,w,band,hdc,offset<0?-offset:0,y,SRCCOPY);
    }
}

void EffectInvert(HDC hdc, int width, int height, DWORD elapsed)
{
    (void)elapsed;
    BitBlt(hdc,0,0,width,height,hdc,0,0,NOTSRCCOPY);
}

void EffectVerticalTear(HDC hdc, int width, int height, DWORD elapsed)
{
    int i;
    srand((unsigned int)(elapsed/28));
    for(i=0;i<20;++i)
    {
        int x=rand()%(width>1?width:1);
        int w=4+rand()%70;
        int dy=(rand()%161)-80;
        int h=height-abs(dy);
        if(x+w>width)w=width-x;
        if(w>0&&h>0)BitBlt(hdc,x,dy,w,h,hdc,x,dy<0?-dy:0,SRCCOPY);
    }
}

void EffectBars(HDC hdc, int width, int height, DWORD elapsed)
{
    int y;
    int offset=(int)((elapsed/7)%100);
    HBRUSH b=CreateSolidBrush(RGB(0,0,0));
    RECT r;
    if(!b)return;
    for(y=offset;y<height;y+=100){r.left=0;r.top=y;r.right=width;r.bottom=ClampInt(y+14,0,height);FillRect(hdc,&r,b);}
    DeleteObject(b);
}

void EffectChecker(HDC hdc, int width, int height, DWORD elapsed)
{
    int size=32+(int)((elapsed/90)%34);
    int x,y;
    HBRUSH a=CreateSolidBrush(RGB(255,255,255));
    HBRUSH b=CreateSolidBrush(RGB(0,0,0));
    if(!a||!b){if(a)DeleteObject(a);if(b)DeleteObject(b);return;}
    for(y=0;y<height;y+=size)for(x=0;x<width;x+=size)
    {
        RECT r={x,y,ClampInt(x+size,0,width),ClampInt(y+size,0,height)};
        FillRect(hdc,&r,(((x/size)+(y/size))&1)?a:b);
    }
    DeleteObject(a);DeleteObject(b);
}

void EffectZoom(HDC hdc, int width, int height, DWORD elapsed)
{
    int phase=(int)((elapsed/18)%120);
    int zoom=100+(phase<60?phase:120-phase);
    int nw=width*zoom/100;
    int nh=height*zoom/100;
    int x=(width-nw)/2;
    int y=(height-nh)/2;
    SetStretchBltMode(hdc,HALFTONE);
    StretchBlt(hdc,x,y,nw,nh,hdc,0,0,width,height,SRCCOPY);
}

void EffectNoise(HDC hdc, int width, int height, DWORD elapsed)
{
    int i;
    srand((unsigned int)(elapsed/18));
    for(i=0;i<260;++i)
    {
        int x=rand()%(width>1?width:1),y=rand()%(height>1?height:1);
        int w=1+rand()%26,h=1+rand()%13;
        HBRUSH b=CreateSolidBrush(RGB(rand()%256,rand()%256,rand()%256));
        RECT r={x,y,ClampInt(x+w,0,width),ClampInt(y+h,0,height)};
        if(b){FillRect(hdc,&r,b);DeleteObject(b);}
    }
}

void EffectWarp(HDC hdc, int width, int height, DWORD elapsed)
{
    const int band=24;
    int y;
    for(y=0;y<height;y+=band)
    {
        int phase=(y/band)*31;
        int off=(int)((elapsed/9+phase)%180)-90;
        int w=width-abs(off);
        if(w>0)BitBlt(hdc,off,y,w,band,hdc,off<0?-off:0,y,SRCCOPY);
    }
}

void EffectColorBands(HDC hdc, int width, int height, DWORD elapsed)
{
    int y;
    const int band=10;
    for(y=0;y<height;y+=band)
    {
        int r=(y+(int)(elapsed/5))&255;
        int g=(y*2+(int)(elapsed/7))&255;
        int b=(y*3+(int)(elapsed/9))&255;
        HBRUSH br=CreateSolidBrush(RGB(r,g,b));
        RECT rc={0,y,width,ClampInt(y+band,0,height)};
        if(br){FillRect(hdc,&rc,br);DeleteObject(br);}
    }
}

void EffectSpiral(HDC hdc, int width, int height, DWORD elapsed)
{
    int cx=width/2,cy=height/2;
    int max=(width>height?width:height)/2;
    int radius;
    for(radius=max;radius>80;radius-=90)
    {
        int off=(int)((elapsed/7+radius)%140)-70;
        int left=cx-radius+off;
        int top=cy-radius;
        int size=radius*2;
        BitBlt(hdc,left,top,size,size,hdc,cx-radius,cy-radius,SRCCOPY);
    }
}

void EffectFlicker(HDC hdc, int width, int height, DWORD elapsed)
{
    int phase=(int)((elapsed/45)%8);
    if(phase==0||phase==3||phase==7)
    {
        HBRUSH b=CreateSolidBrush((phase&1)?RGB(15,15,15):RGB(220,220,220));
        RECT r={0,0,width,height};
        if(b){FillRect(hdc,&r,b);DeleteObject(b);}
    }
}

void EffectVignette(HDC hdc, int width, int height, DWORD elapsed)
{
    int step=80;
    int i;
    int strength=(int)((elapsed/25)%80)+30;
    for(i=0;i<8;++i)
    {
        int alpha=ClampInt(strength+i*12,0,255);
        int shade=255-alpha;
        HPEN p=CreatePen(PS_SOLID,step,RGB(shade,shade,shade));
        HPEN old;
        if(!p)continue;
        old=(HPEN)SelectObject(hdc,p);
        Rectangle(hdc,i*step/2,i*step/2,width-i*step/2,height-i*step/2);
        SelectObject(hdc,old);DeleteObject(p);
    }
}

void EffectMosaic(HDC hdc, int width, int height, DWORD elapsed)
{
    int i;
    int tile=18+(int)((elapsed/60)%20);
    for(i=0;i<120;++i)
    {
        int x=rand()%(width>1?width:1);
        int y=rand()%(height>1?height:1);
        int w=tile+(rand()%tile);
        int h=tile+(rand()%tile);
        int dx=(rand()%(tile*2+1))-tile;
        int dy=(rand()%(tile*2+1))-tile;
        int cw=width-x, ch=height-y;
        if(w>cw) w=cw;
        if(h>ch) h=ch;
        if(w>0&&h>0)BitBlt(hdc,x+dx,y+dy,w,h,hdc,x,y,SRCCOPY);
    }
}

void EffectCenterSplit(HDC hdc, int width, int height, DWORD elapsed)
{
    int shift=(int)((elapsed/12)%160)-80;
    int half=width/2;
    int w=half-abs(shift);
    if(w<=0)return;
    BitBlt(hdc,shift,0,w,height,hdc,0,0,SRCCOPY);
    BitBlt(hdc,half-shift,0,w,height,hdc,half,0,SRCCOPY);
}

void EffectRandomLines(HDC hdc, int width, int height, DWORD elapsed)
{
    int i;
    srand((unsigned int)(elapsed/20));
    for(i=0;i<70;++i)
    {
        HPEN p=CreatePen(PS_SOLID,1+rand()%4,RGB(rand()%256,rand()%256,rand()%256));
        HPEN old;
        if(!p)continue;
        old=(HPEN)SelectObject(hdc,p);
        MoveToEx(hdc,rand()%(width>1?width:1),rand()%(height>1?height:1),NULL);
        LineTo(hdc,rand()%(width>1?width:1),rand()%(height>1?height:1));
        SelectObject(hdc,old);DeleteObject(p);
    }
}

void EffectFinalChaos(HDC hdc, int width, int height, DWORD elapsed, int intensity)
{
    intensity=ClampInt(intensity,0,100);
    EffectGlitch(hdc,width,height,elapsed);
    if(intensity>=15)EffectShake(hdc,width,height,elapsed);
    if(intensity>=25)EffectTear(hdc,width,height,elapsed);
    if(intensity>=35)EffectRGBSplit(hdc,width,height,elapsed);
    if(intensity>=45)EffectWave(hdc,width,height,elapsed);
    if(intensity>=55)EffectNoise(hdc,width,height,elapsed);
    if(intensity>=65)EffectRects(hdc,width,height,elapsed);
    if(intensity>=75)EffectRandomLines(hdc,width,height,elapsed);
    if(intensity>=85)EffectScanlines(hdc,width,height,elapsed);
    if(intensity>=92 && ((elapsed/100)%5)==0)EffectFlash(hdc,width,height,elapsed);
}
