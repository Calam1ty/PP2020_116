#include <windows.h>
#include <iostream>
#define WINVER 0x0500
using namespace std;

int main(){
    while (true)
{
Sleep(50);
    if (GetAsyncKeyState(VK_NUMPAD4)) {
        return 0;
        }

    /*
    int x,y;
    cout << "x, y" << endl;
    cin >> x;
    cin >> y;
    //перемещение курсора на координаты (x,y)
    SetCursorPos(x, y);
    */

    INPUT ip;

    Sleep(5000);

    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    ip.ki.wVk = 0x34;
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));

    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(5000);

    //SetCursorPos(x, y);

    //мышка
    INPUT iNPUT = {0};
    iNPUT.type = INPUT_MOUSE;
    iNPUT.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1,&iNPUT, sizeof(iNPUT));
    ZeroMemory(&iNPUT, sizeof(iNPUT));
    iNPUT.type = INPUT_MOUSE;
    iNPUT.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1,&iNPUT, sizeof(iNPUT));
}
}
