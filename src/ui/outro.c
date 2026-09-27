#include "outro.h"

void OutroDraw(HDC hdc,int width,int height,DWORD elapsed)
{
    HBRUSH b=CreateSolidBrush(RGB(5,5,5));
    RECT r={0,0,width,height};
    HFONT f;
    if(!b)return;
    FillRect(hdc,&r,b);DeleteObject(b);
    f=CreateFontA(34,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH,"Segoe UI");
    if(f)
    {
        HFONT old=(HFONT)SelectObject(hdc,f);
        SetBkMode(hdc,TRANSPARENT);SetTextColor(hdc,RGB(235,235,235));
        r.top=height/2-40;r.bottom=r.top+60;DrawTextA(hdc,"CSafeMEMZ finished",-1,&r,DT_CENTER|DT_SINGLELINE);
        SelectObject(hdc,old);DeleteObject(f);
    }
    (void)elapsed;
}
