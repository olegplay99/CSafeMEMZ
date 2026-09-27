#include "intro.h"

static void CenterText(HDC hdc, int width, int y, const char* text, int size, COLORREF color)
{
    HFONT f=CreateFontA(size,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH,"Segoe UI");
    RECT r={0,y,width,y+size+20};
    if(!f)return;
    {HFONT old=(HFONT)SelectObject(hdc,f);SetBkMode(hdc,TRANSPARENT);SetTextColor(hdc,color);DrawTextA(hdc,text,-1,&r,DT_CENTER|DT_SINGLELINE);SelectObject(hdc,old);}
    DeleteObject(f);
}

void IntroDraw(HDC hdc,int width,int height,DWORD elapsed)
{
    HBRUSH b=CreateSolidBrush(RGB(8,8,8));
    RECT r={0,0,width,height};
    if(!b)return;
    FillRect(hdc,&r,b);DeleteObject(b);
    CenterText(hdc,width,height/2-80,"CSafeMEMZ",52,RGB(240,240,240));
    CenterText(hdc,width,height/2,"SAFE VISUAL SIMULATION",20,RGB(180,180,180));
    if(elapsed>1600)CenterText(hdc,width,height/2+45,"press ESC to exit",15,RGB(110,110,110));
}
