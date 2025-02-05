#include "framework.h"
#include "Fish Predator.h"
#include <mmsystem.h>
#include <d2d1.h>
#include <dwrite.h>
#include "D2BMPLOADER.h"
#include "ErrH.h"
#include "FCheck.h"
#include "gifresizer.h"
#include "ServEng.h"
#include <chrono>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2bmploader.lib")
#pragma comment(lib, "errh.lib")
#pragma comment(lib, "fcheck.lib")
#pragma comment(lib, "gifresizer.lib")
#pragma comment(lib, "serveng.lib")

constexpr wchar_t bWinClassName[]{ L"HungryShark" };

constexpr char tmp_file[]{ ".\\res\\data\\temp.dat" };
constexpr wchar_t Ltmp_file[]{ L".\\res\\data\\temp.dat" };
constexpr wchar_t snd_file[]{ L".\\res\\snd\\main.wav" };
constexpr wchar_t record_file[]{ L".\\res\\data\\record.dat" };
constexpr wchar_t save_file[]{ L".\\res\\data\\save.dat" };
constexpr wchar_t help_file[]{ L".\\res\\data\\help.dat" };

constexpr int mNew{ 1001 };
constexpr int mLvl{ 1002 }; 
constexpr int mExit{ 1003 };
constexpr int mSave{ 1004 };
constexpr int mLoad{ 1005 };
constexpr int mHoF{ 1006 };

constexpr int no_record{ 2001 };
constexpr int first_record{ 2002 };
constexpr int record{ 2003 };

WNDCLASS bWinClass{};
HINSTANCE bIns = nullptr;
HWND bHwnd = nullptr;
HMENU bBar = nullptr;
HMENU bMain = nullptr;
HMENU bStore = nullptr;
HICON Icon = nullptr;
HCURSOR mainCur = nullptr;
HCURSOR outCur = nullptr;
POINT cur_pos{};
HDC PaintDC{ nullptr };
PAINTSTRUCT bPaint{};
MSG bMsg{};
BOOL bRet{ 0 };
UINT bTimer{ 0 };

D2D1_RECT_F b1Rect{ 20.0f, 0, scr_width / 3 - 50.0f, 50.0f };
D2D1_RECT_F b2Rect{ scr_width / 3 + 20.0f, 0, scr_width * 2/ 3 - 50.0f, 50.0f };
D2D1_RECT_F b3Rect{ scr_width * 2 / 3 + 20.0f, 0, scr_width - 50.0f, 50.0f };

D2D1_RECT_F b1TextRect{ 40.0f, 5.0f, scr_width / 3 - 50.0f, 50.0f };
D2D1_RECT_F b2TextRect{ scr_width / 3 + 50.0f, 5.0f, scr_width * 2 / 3 - 50.0f, 50.0f };
D2D1_RECT_F b3TextRect{ scr_width * 2 / 3 + 40.0f, 5.0f, scr_width - 50.0f, 50.0f };

bool pause = false;
bool in_client = true;
bool show_help = false;
bool sound = true;
bool b1Hglt = false;
bool b2Hglt = false;
bool b3Hglt = false;
bool name_set = false;

int level = 1;
int score = 0;
int mins = 0;
int secs = 180;
int enemies_killed = 0;

int intro_frame = 0;
int field_frame = 0;
int field_delay = 3;

wchar_t current_player[16]{ L"ONE HUNGRY FISH" };

dll::RANDiT RandMachine{};

ID2D1Factory* iFactory{ nullptr };
ID2D1HwndRenderTarget* Draw{ nullptr };

ID2D1RadialGradientBrush* but1Bckg{ nullptr };
ID2D1RadialGradientBrush* but2Bckg{ nullptr };
ID2D1RadialGradientBrush* but3Bckg{ nullptr };
ID2D1SolidColorBrush* boxBckg{ nullptr };
ID2D1SolidColorBrush* textBrush{ nullptr };
ID2D1SolidColorBrush* hgltBrush{ nullptr };
ID2D1SolidColorBrush* inactBrush{ nullptr };

IDWriteFactory* iWriteFactory{ nullptr };
IDWriteTextFormat* nrmText{ nullptr };
IDWriteTextFormat* midText{ nullptr };
IDWriteTextFormat* bigText{ nullptr };
IDWriteTextFormat* fishText{ nullptr };

ID2D1Bitmap* bmpIntro[14]{ nullptr };
ID2D1Bitmap* bmpField[75]{ nullptr };
ID2D1Bitmap* bmpBubbles[40]{ nullptr };
ID2D1Bitmap* bmpGrass[6]{ nullptr };
ID2D1Bitmap* bmpJelly1[47]{ nullptr };
ID2D1Bitmap* bmpJelly2[5]{ nullptr };

ID2D1Bitmap* bmpSmallHeroL[25]{ nullptr };
ID2D1Bitmap* bmpSmallHeroR[25]{ nullptr };
ID2D1Bitmap* bmpMidHeroL[25]{ nullptr };
ID2D1Bitmap* bmpMidHeroR[25]{ nullptr };
ID2D1Bitmap* bmpBigHeroL[25]{ nullptr };
ID2D1Bitmap* bmpBigHeroR[25]{ nullptr };

ID2D1Bitmap* bmpFish1L[20]{ nullptr };
ID2D1Bitmap* bmpFish1R[20]{ nullptr };
ID2D1Bitmap* bmpFish2L[6]{ nullptr };
ID2D1Bitmap* bmpFish2R[6]{ nullptr };
ID2D1Bitmap* bmpFish3L[12]{ nullptr };
ID2D1Bitmap* bmpFish3R[12]{ nullptr };
ID2D1Bitmap* bmpFish4L[40]{ nullptr };
ID2D1Bitmap* bmpFish4R[40]{ nullptr };
ID2D1Bitmap* bmpFish5L[8]{ nullptr };
ID2D1Bitmap* bmpFish5R[8]{ nullptr };
ID2D1Bitmap* bmpFish6L[16]{ nullptr };
ID2D1Bitmap* bmpFish6R[16]{ nullptr };
ID2D1Bitmap* bmpFish7L[4]{ nullptr };
ID2D1Bitmap* bmpFish7R[4]{ nullptr };
ID2D1Bitmap* bmpFish8L[8]{ nullptr };
ID2D1Bitmap* bmpFish8R[8]{ nullptr };
ID2D1Bitmap* bmpFish9L[8]{ nullptr };
ID2D1Bitmap* bmpFish9R[8]{ nullptr };
////////////////////////////////////////////////

template<typename T>concept HasRelease = requires (T check)
{
    check.Release();
};
template<HasRelease U>bool ClearMem(U** what)
{
    if (*what)
    {
        (*what)->Release();
        (*what) = nullptr;
        return true;
    }

    return false;
}
void ErrLog(LPCWSTR what)
{
    std::wofstream log(L".\\res\\data\\error.log", std::ios::app);
    log << what << L" Time of occurence: " << std::chrono::system_clock::now() << std::endl;
    log.close();
}
void ClearResources()
{
    if (!ClearMem(&iFactory))ErrLog(L"Error releasing iFactory !");
    if (!ClearMem(&Draw))ErrLog(L"Error releasing Draw !");
    if (!ClearMem(&but1Bckg))ErrLog(L"Error releasing but1Bckg !");
    if (!ClearMem(&but2Bckg))ErrLog(L"Error releasing but2Bckg !");
    if (!ClearMem(&but3Bckg))ErrLog(L"Error releasing but3Bckg !");

    if (!ClearMem(&boxBckg))ErrLog(L"Error releasing boxBckg !");
    if (!ClearMem(&textBrush))ErrLog(L"Error releasing textBrush !");
    if (!ClearMem(&hgltBrush))ErrLog(L"Error releasing hgltBrush !");
    if (!ClearMem(&inactBrush))ErrLog(L"Error releasing inactBrush !");

    if (!ClearMem(&iWriteFactory))ErrLog(L"Error releasing iWrietFactory !");
    if (!ClearMem(&nrmText))ErrLog(L"Error releasing nrmText !");
    if (!ClearMem(&midText))ErrLog(L"Error releasing midText !");
    if (!ClearMem(&bigText))ErrLog(L"Error releasing bigText !");
    if (!ClearMem(&fishText))ErrLog(L"Error releasing fishText !");

    for (int i = 0; i < 14; ++i)if (!ClearMem(&bmpIntro[i]))ErrLog(L"Error releasing bmpIntro !");
    for (int i = 0; i < 75; ++i)if (!ClearMem(&bmpField[i]))ErrLog(L"Error releasing bmpField !");
    for (int i = 0; i < 40; ++i)if (!ClearMem(&bmpBubbles[i]))ErrLog(L"Error releasing bmpBubbles !");
    for (int i = 0; i < 6; ++i)if (!ClearMem(&bmpGrass[i]))ErrLog(L"Error releasing bmpGrass !");
    for (int i = 0; i < 47; ++i)if (!ClearMem(&bmpJelly1[i]))ErrLog(L"Error releasing bmpJelly1 !");
    for (int i = 0; i < 5; ++i)if (!ClearMem(&bmpJelly2[i]))ErrLog(L"Error releasing bmpJelly2 !");

    for (int i = 0; i < 25; ++i)if (!ClearMem(&bmpSmallHeroL[i]))ErrLog(L"Error releasing bmpSmallHeroL !");
    for (int i = 0; i < 25; ++i)if (!ClearMem(&bmpSmallHeroR[i]))ErrLog(L"Error releasing bmpSmallHeroR !");
    for (int i = 0; i < 25; ++i)if (!ClearMem(&bmpMidHeroL[i]))ErrLog(L"Error releasing bmpMidHeroL !");
    for (int i = 0; i < 25; ++i)if (!ClearMem(&bmpMidHeroR[i]))ErrLog(L"Error releasing bmpMidHeroR !");
    for (int i = 0; i < 25; ++i)if (!ClearMem(&bmpBigHeroL[i]))ErrLog(L"Error releasing bmpBigHeroL !");
    for (int i = 0; i < 25; ++i)if (!ClearMem(&bmpBigHeroR[i]))ErrLog(L"Error releasing bmpBigHeroR !");

    for (int i = 0; i < 20; ++i)if (!ClearMem(&bmpFish1L[i]))ErrLog(L"Error releasing bmpFish1L !");
    for (int i = 0; i < 20; ++i)if (!ClearMem(&bmpFish1R[i]))ErrLog(L"Error releasing bmpFish1R !");

    for (int i = 0; i < 6; ++i)if (!ClearMem(&bmpFish2L[i]))ErrLog(L"Error releasing bmpFish2L !");
    for (int i = 0; i < 6; ++i)if (!ClearMem(&bmpFish2R[i]))ErrLog(L"Error releasing bmpFish2R !");

    for (int i = 0; i < 12; ++i)if (!ClearMem(&bmpFish3L[i]))ErrLog(L"Error releasing bmpFish3L !");
    for (int i = 0; i < 12; ++i)if (!ClearMem(&bmpFish3R[i]))ErrLog(L"Error releasing bmpFish3R !");

    for (int i = 0; i < 40; ++i)if (!ClearMem(&bmpFish4L[i]))ErrLog(L"Error releasing bmpFish4L !");
    for (int i = 0; i < 40; ++i)if (!ClearMem(&bmpFish4R[i]))ErrLog(L"Error releasing bmpFish4R !");

    for (int i = 0; i < 8; ++i)if (!ClearMem(&bmpFish5L[i]))ErrLog(L"Error releasing bmpFish5L !");
    for (int i = 0; i < 8; ++i)if (!ClearMem(&bmpFish5R[i]))ErrLog(L"Error releasing bmpFish5R !");

    for (int i = 0; i < 16; ++i)if (!ClearMem(&bmpFish6L[i]))ErrLog(L"Error releasing bmpFish6L !");
    for (int i = 0; i < 16; ++i)if (!ClearMem(&bmpFish6R[i]))ErrLog(L"Error releasing bmpFish6R !");

    for (int i = 0; i < 4; ++i)if (!ClearMem(&bmpFish7L[i]))ErrLog(L"Error releasing bmpFish7L !");
    for (int i = 0; i < 4; ++i)if (!ClearMem(&bmpFish7R[i]))ErrLog(L"Error releasing bmpFish7R !");

    for (int i = 0; i < 8; ++i)if (!ClearMem(&bmpFish8L[i]))ErrLog(L"Error releasing bmpFish8L !");
    for (int i = 0; i < 8; ++i)if (!ClearMem(&bmpFish8R[i]))ErrLog(L"Error releasing bmpFish8R !");

    for (int i = 0; i < 8; ++i)if (!ClearMem(&bmpFish9L[i]))ErrLog(L"Error releasing bmpFish9L !");
    for (int i = 0; i < 8; ++i)if (!ClearMem(&bmpFish9R[i]))ErrLog(L"Error releasing bmpFish9R !");
}
void ErrExit(int what)
{
    MessageBeep(MB_ICONERROR);
    MessageBox(NULL, ErrHandle(what), L"Критична грешка !", MB_OK | MB_APPLMODAL | MB_ICONERROR);
    
    ClearResources();
    std::remove(tmp_file);
    exit(1);
}

void GameOver()
{
    PlaySound(NULL, NULL, NULL);
    KillTimer(bHwnd, bTimer);



    bMsg.message = WM_QUIT;
    bMsg.wParam = 0;
}
void InitGame()
{
    level = 1;
    score = 0;
    wcscpy_s(current_player, L"ONE HUNGRY FISH");
    name_set = false;
    secs = 180;
    enemies_killed = 0;






}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
    switch (ReceivedMsg)
    {
    case WM_INITDIALOG:
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (WPARAM)(Icon));
        return true;

    case WM_CLOSE:
        EndDialog(hwnd, IDCANCEL);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            break;

        case IDOK:
            if (GetDlgItemTextW(hwnd, IDC_NAME, current_player, 16) < 1)
            {
                wcscpy_s(current_player, L"ONE HUNGRY FISH");
                if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
                MessageBox(bHwnd, L"Ха, ха, ха ! Забрави си името", L"Забраватор", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
                EndDialog(hwnd, IDCANCEL);
            }
            EndDialog(hwnd, IDOK);
            break;
        }
        break;
    }

    return (INT_PTR)(FALSE);
}
LRESULT CALLBACK WinProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
    switch (ReceivedMsg)
    {
    case WM_CREATE:
        SetTimer(hwnd, bTimer, 1000, NULL);
        bBar = CreateMenu();
        bMain = CreateMenu();
        bStore = CreateMenu();
        if (bBar && bMain && bStore)
        {
            AppendMenu(bBar, MF_POPUP, (UINT_PTR)(bMain), L"Основно меню");
            AppendMenu(bBar, MF_POPUP, (UINT_PTR)(bStore), L"Меню за данни");

            AppendMenu(bMain, MF_STRING, mNew, L"Нова игра");
            AppendMenu(bMain, MF_STRING, mLvl, L"Следващо ниво");
            AppendMenu(bMain, MF_SEPARATOR, NULL, NULL);
            AppendMenu(bMain, MF_STRING, mExit, L"Изход");

            AppendMenu(bStore, MF_STRING, mSave, L"Запази игра");
            AppendMenu(bStore, MF_STRING, mLoad, L"Зареди игра");
            AppendMenu(bStore, MF_SEPARATOR, NULL, NULL);
            AppendMenu(bStore, MF_STRING, mHoF, L"Зала на славата");

            SetMenu(hwnd, bBar);
            InitGame();
        }
        break;

    case WM_CLOSE:
        pause = true;
        if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
        if (MessageBox(hwnd, L"Ако излезеш, губиш прогреса по играта !\n\nНаистина ли излизаш ?",
            L"Изход !", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
        {
            pause = false;
            break;
        }
        GameOver();
        break;

    case WM_TIMER:
        if (pause)break;
        secs--;
        mins = secs / 60;
        break;

    case WM_PAINT:
        PaintDC = BeginPaint(hwnd, &bPaint);
        FillRect(PaintDC, &bPaint.rcPaint, CreateSolidBrush(RGB(10, 10, 10)));
        EndPaint(hwnd, &bPaint);
        break;

    case WM_SETCURSOR:
        GetCursorPos(&cur_pos);
        ScreenToClient(hwnd, &cur_pos);
        if (LOWORD(lParam) == HTCLIENT)
        {
            if (!in_client)
            {
                in_client = true;
                pause = false;
            }

            if (cur_pos.y <= 50)
            {
                if (cur_pos.x >= b1Rect.left && cur_pos.x <= b1Rect.right)
                {
                    if (!b1Hglt)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                        b1Hglt = true;
                        b2Hglt = false;
                        b3Hglt = false;
                    }
                }
                if (cur_pos.x >= b2Rect.left && cur_pos.x <= b2Rect.right)
                {
                    if (!b2Hglt)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                        b1Hglt = false;
                        b2Hglt = true;
                        b3Hglt = false;
                    }
                }
                if (cur_pos.x >= b3Rect.left && cur_pos.x <= b3Rect.right)
                {
                    if (!b3Hglt)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                        b1Hglt = false;
                        b2Hglt = false;
                        b3Hglt = true;
                    }
                }

                SetCursor(outCur);
                return true;
            }
            else if (b1Hglt || b2Hglt || b3Hglt)
            {
                if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                b1Hglt = false;
                b2Hglt = false;
                b3Hglt = false;
            }

            SetCursor(mainCur);
            return true;
        }
        else
        {
            if (in_client)
            {
                in_client = false;
                pause = true;
            }

            if (b1Hglt || b2Hglt || b3Hglt)
            {
                if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                b1Hglt = false;
                b2Hglt = false;
                b3Hglt = false;
            }

            SetCursor(LoadCursor(NULL, IDC_ARROW));
            return true;
        }
        break;


    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case mNew:
            pause = true;
            if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
            if (MessageBox(hwnd, L"Ако рестартираш, губиш прогреса по играта !\n\nНаистина ли рестартираш ?",
                L"Рестарт !", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
            {
                pause = false;
                break;
            }
            InitGame();
            break;




        case mExit:
            SendMessage(hwnd, WM_CLOSE, NULL, NULL);
            break;


        }
        break;






    default: return DefWindowProc(hwnd, ReceivedMsg, wParam, lParam);
    }

    return (LRESULT)(FALSE);
}

void CreateResources()
{
    int result{ 0 };
    CheckFile(Ltmp_file, &result);
    if (result == FILE_EXIST)ErrExit(eStarted);
    else
    {
        std::wofstream start(Ltmp_file);
        start << L"Game started at: " << std::chrono::system_clock::now();
        start.close();
    }

    int win_x = GetSystemMetrics(SM_CXSCREEN) / 2 - (int)(scr_width / 2.0f);
    if (GetSystemMetrics(SM_CXSCREEN) < win_x + (int)(scr_width) || GetSystemMetrics(SM_CYSCREEN) < (int)(scr_height + 10.0f))
        ErrExit(eScreen);
    Icon = (HICON)(LoadImage(NULL, L".\\res\\main.ico", IMAGE_ICON, 255, 255, LR_LOADFROMFILE));
    if (!Icon)ErrExit(eIcon);

    mainCur = LoadCursorFromFileW(L".\\res\\main.ani");
    outCur = LoadCursorFromFileW(L".\\res\\out.ani");

    if (!mainCur || !outCur)ErrExit(eCursor);

    bWinClass.lpszClassName = bWinClassName;
    bWinClass.hInstance = bIns;
    bWinClass.lpfnWndProc = &WinProc;
    bWinClass.hbrBackground = CreateSolidBrush(RGB(10, 10, 10));
    bWinClass.hIcon = Icon;
    bWinClass.hCursor = mainCur;
    bWinClass.style = CS_DROPSHADOW;

    if (!RegisterClass(&bWinClass))ErrExit(eClass);
    
    bHwnd = CreateWindow(bWinClassName, L"ГЛАДЕН ХИЩНИК ! ver. 1.0", WS_CAPTION | WS_SYSMENU, win_x, 10, (int)(scr_width),
        (int)(scr_height), NULL, NULL, bIns, NULL);
    
    if (!bHwnd)ErrExit(eWindow);
    else
    {
        ShowWindow(bHwnd, SW_SHOWDEFAULT);

        HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &iFactory);
        if (hr != S_OK)
        {
            ErrLog(L"Error creating iFactory !");
            ErrExit(eD2D);
        }

        if (iFactory)
            hr = iFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(bHwnd,
                D2D1::SizeU((UINT32)(scr_width), (UINT32)(scr_height))), &Draw);
        if (hr != S_OK)
        {
            ErrLog(L"Error creating D2D1 HwndRenderTarget !");
            ErrExit(eD2D);
        }

        if (Draw)
        {
            D2D1_GRADIENT_STOP Stops[2]{};
            ID2D1GradientStopCollection* StColl = nullptr;

            Stops[0].position = 0;
            Stops[0].color = D2D1::ColorF(D2D1::ColorF::Aquamarine);
            Stops[1].position = 1.0f;
            Stops[1].color = D2D1::ColorF(D2D1::ColorF::DarkViolet);

            hr = Draw->CreateGradientStopCollection(Stops, 2, &StColl);
            if (hr != S_OK)
            {
                ErrLog(L"Error creating D2D1 GradientStopCollection !");
                ErrExit(eD2D);
            }
            if (StColl)
            {
                hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b1Rect.left + 100.0f, 
                    25.0f), D2D1::Point2F(0, 0), 100.0f, 25.0f), StColl, &but1Bckg);
                hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b2Rect.left + 100.0f,
                    25.0f), D2D1::Point2F(0, 0), 100.0f, 25.0f), StColl, &but2Bckg);
                hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b3Rect.left + 100.0f,
                    25.0f), D2D1::Point2F(0, 0), 100.0f, 25.0f), StColl, &but3Bckg);
                if (hr != S_OK)
                {
                    ErrLog(L"Error creating D2D1 RadialGradientBrushes !");
                    ErrExit(eD2D);
                }

                ClearMem(&StColl);
            }

            hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &textBrush);
            hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &hgltBrush);
            hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &inactBrush);
            hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkKhaki), &boxBckg);
            if (hr != S_OK)
            {
                ErrLog(L"Error creating D2D1 SolidColorBrushes !");
                ErrExit(eD2D);
            }

            for (int i = 0; i < 14; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\field\\intro\\";
                wchar_t add[5] = L"\0";
                
                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpIntro[i] = Load(name, Draw);

                if (!bmpIntro[i])
                {
                    ErrLog(L"Error loading bmpIntro !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 75; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\field\\background\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpField[i] = Load(name, Draw);

                if (!bmpField[i])
                {
                    ErrLog(L"Error loading bmpField !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 40; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\field\\bubbles\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpBubbles[i] = Load(name, Draw);

                if (!bmpBubbles[i])
                {
                    ErrLog(L"Error loading bmpBubbles !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 6; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\field\\grass\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpGrass[i] = Load(name, Draw);

                if (!bmpGrass[i])
                {
                    ErrLog(L"Error loading bmpGrass !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 47; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\field\\jelly1\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpJelly1[i] = Load(name, Draw);

                if (!bmpJelly1[i])
                {
                    ErrLog(L"Error loading bmpJelly1 !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 5; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\field\\jelly2\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpJelly2[i] = Load(name, Draw);

                if (!bmpJelly2[i])
                {
                    ErrLog(L"Error loading bmpJelly2 !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 25; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\hero\\small\\l\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpSmallHeroL[i] = Load(name, Draw);

                if (!bmpSmallHeroL[i])
                {
                    ErrLog(L"Error loading bmpSmallHeroL !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 25; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\hero\\small\\r\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpSmallHeroR[i] = Load(name, Draw);

                if (!bmpSmallHeroR[i])
                {
                    ErrLog(L"Error loading bmpSmallHeroR !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 25; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\hero\\mid\\l\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpMidHeroL[i] = Load(name, Draw);

                if (!bmpMidHeroL[i])
                {
                    ErrLog(L"Error loading bmpMidHeroL !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 25; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\hero\\mid\\r\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpMidHeroR[i] = Load(name, Draw);

                if (!bmpMidHeroR[i])
                {
                    ErrLog(L"Error loading bmpMidHeroR !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 25; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\hero\\big\\l\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpBigHeroL[i] = Load(name, Draw);

                if (!bmpBigHeroL[i])
                {
                    ErrLog(L"Error loading bmpBigHeroL !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 25; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\hero\\big\\r\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpBigHeroR[i] = Load(name, Draw);

                if (!bmpBigHeroR[i])
                {
                    ErrLog(L"Error loading bmpBigHeroR !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 20; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish1\\l\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish1L[i] = Load(name, Draw);

                if (!bmpFish1L[i])
                {
                    ErrLog(L"Error loading bmpFish1L !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 20; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish1\\r\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish1R[i] = Load(name, Draw);

                if (!bmpFish1R[i])
                {
                    ErrLog(L"Error loading bmpFish1R !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 6; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish2\\l\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish2L[i] = Load(name, Draw);

                if (!bmpFish2L[i])
                {
                    ErrLog(L"Error loading bmpFish2L !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 6; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish2\\r\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish2R[i] = Load(name, Draw);

                if (!bmpFish2R[i])
                {
                    ErrLog(L"Error loading bmpFish2R !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 12; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish3\\l\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish3L[i] = Load(name, Draw);

                if (!bmpFish3L[i])
                {
                    ErrLog(L"Error loading bmpFish3L !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 12; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish3\\r\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish3R[i] = Load(name, Draw);

                if (!bmpFish3R[i])
                {
                    ErrLog(L"Error loading bmpFish3R !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 40; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish4\\l\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish4L[i] = Load(name, Draw);

                if (!bmpFish4L[i])
                {
                    ErrLog(L"Error loading bmpFish4L !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 40; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish4\\r\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish4R[i] = Load(name, Draw);

                if (!bmpFish4R[i])
                {
                    ErrLog(L"Error loading bmpFish4R !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 8; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish5\\l\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish5L[i] = Load(name, Draw);

                if (!bmpFish5L[i])
                {
                    ErrLog(L"Error loading bmpFish1L !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 8; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish5\\r\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish5R[i] = Load(name, Draw);

                if (!bmpFish5R[i])
                {
                    ErrLog(L"Error loading bmpFish5R !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 16; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish6\\l\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish6L[i] = Load(name, Draw);

                if (!bmpFish6L[i])
                {
                    ErrLog(L"Error loading bmpFish6L !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 16; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish6\\r\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish6R[i] = Load(name, Draw);

                if (!bmpFish6R[i])
                {
                    ErrLog(L"Error loading bmpFish6R !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 4; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish7\\l\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish7L[i] = Load(name, Draw);

                if (!bmpFish7L[i])
                {
                    ErrLog(L"Error loading bmpFish1L !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 4; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish7\\r\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish7R[i] = Load(name, Draw);

                if (!bmpFish7R[i])
                {
                    ErrLog(L"Error loading bmpFish1R !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 8; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish8\\l\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish8L[i] = Load(name, Draw);

                if (!bmpFish8L[i])
                {
                    ErrLog(L"Error loading bmpFish8L !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 8; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish8\\r\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish8R[i] = Load(name, Draw);

                if (!bmpFish8R[i])
                {
                    ErrLog(L"Error loading bmpFish8R !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 8; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish9\\l\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish9L[i] = Load(name, Draw);

                if (!bmpFish9L[i])
                {
                    ErrLog(L"Error loading bmpFish9L !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 8; ++i)
            {
                wchar_t name[150] = L".\\res\\img\\fish9\\r\\";
                wchar_t add[5] = L"\0";

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFish9R[i] = Load(name, Draw);

                if (!bmpFish9R[i])
                {
                    ErrLog(L"Error loading bmpFish9R !");
                    ErrExit(eD2D);
                }
            }
        }

        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&iWriteFactory));
        if (hr != S_OK)
        {
            ErrLog(L"Error creating iWriteFactory !");
            ErrExit(eD2D);
        }

        if (iWriteFactory)
        {
            hr = iWriteFactory->CreateTextFormat(L"SEGOE PRINT", NULL, DWRITE_FONT_WEIGHT_ULTRA_BLACK,
                DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 18.0f, L"", &nrmText);
            hr = iWriteFactory->CreateTextFormat(L"SEGOE PRINT", NULL, DWRITE_FONT_WEIGHT_ULTRA_BLACK,
                DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 36.0f, L"", &midText);
            hr = iWriteFactory->CreateTextFormat(L"SEGOE PRINT", NULL, DWRITE_FONT_WEIGHT_ULTRA_BLACK,
                DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"", &bigText);
            hr = iWriteFactory->CreateTextFormat(L"SEGOE PRINT", NULL, DWRITE_FONT_WEIGHT_ULTRA_BLACK,
                DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"", &fishText);

            if (hr != S_OK)
            {
                ErrLog(L"Error creating iWriteFactory text formats !");
                ErrExit(eD2D);
            }
        }
    }

    if (Draw && bigText && hgltBrush)
    {
        for (int i = 0; i < 200; ++i)
        {
            Draw->BeginDraw();
            Draw->DrawBitmap(bmpIntro[intro_frame], D2D1::RectF(0, 0, scr_width, scr_height));
            ++intro_frame;
            if (intro_frame > 13)intro_frame = 0;

            if (RandMachine(0, 6) == 2)
            {
                Draw->DrawTextW(L"ГЛАДЕН ХИЩНИК !\n\n      dev. Daniel !", 37, bigText, D2D1::RectF(100.0f, 80.0f, scr_width,
                    scr_height), hgltBrush);
                Draw->EndDraw();
                mciSendString(L"play .\\res\\snd\\buzz.wav", NULL, NULL, NULL);
                Sleep(80);
            }
            else Draw->EndDraw();
        }

        Draw->BeginDraw();
        Draw->DrawBitmap(bmpIntro[intro_frame], D2D1::RectF(0, 0, scr_width, scr_height));
        Draw->DrawTextW(L"ГЛАДЕН ХИЩНИК !\n\n      dev. Daniel !", 37, bigText, D2D1::RectF(100.0f, 80.0f, scr_width,
            scr_height), hgltBrush);
        Draw->EndDraw();
        PlaySound(L".\\res\\snd\\boom.wav", NULL, SND_SYNC);
        Sleep(1000);
    }

}

////////////////////////////////////////////////


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    bIns = hInstance;
    if (!bIns)
    {
        ErrLog(L"Error in Windows hInstance parameter !");
        ErrExit(eClass);
    }

    CreateResources();


    while (bMsg.message != WM_QUIT)
    {
        if ((bRet = PeekMessage(&bMsg, bHwnd, NULL, NULL, PM_REMOVE)) != 0)
        {
            if (bRet == -1)ErrExit(eMsg);
            TranslateMessage(&bMsg);
            DispatchMessageW(&bMsg);
        }

        if (pause)
        {
            if (show_help) continue;
            Draw->BeginDraw();
            Draw->DrawBitmap(bmpIntro[intro_frame], D2D1::RectF(0, 0, scr_width, scr_height));
            Draw->DrawTextW(L"ПАУЗА !", 6, bigText, D2D1::RectF(scr_width / 2 - 100.0f, scr_height / 2 - 80.0f, scr_width,
                scr_height), hgltBrush);
            Draw->EndDraw();
            continue;
        }

        ////////////////////////////////








        // DRAW THINGS *************************

        Draw->BeginDraw();
        if (nrmText && textBrush && hgltBrush && inactBrush && boxBckg && but1Bckg && but2Bckg && but3Bckg)
        {
            Draw->FillRectangle(D2D1::RectF(0, 0, scr_width, 50.0f), boxBckg);
            Draw->FillRoundedRectangle(D2D1::RoundedRect(b1Rect, 10.0f, 10.0f), but1Bckg);
            Draw->FillRoundedRectangle(D2D1::RoundedRect(b2Rect, 10.0f, 10.0f), but2Bckg);
            Draw->FillRoundedRectangle(D2D1::RoundedRect(b3Rect, 10.0f, 10.0f), but3Bckg);

            if (name_set)Draw->DrawTextW(L"ИМЕ НА ГЛАДНИК", 15, nrmText, b1TextRect, inactBrush);
            else
            {
                if (!b1Hglt)Draw->DrawTextW(L"ИМЕ НА ГЛАДНИК", 15, nrmText, b1TextRect, textBrush);
                else Draw->DrawTextW(L"ИМЕ НА ГЛАДНИК", 15, nrmText, b1TextRect, hgltBrush);
            }
            if (!b2Hglt)Draw->DrawTextW(L"ЗВУЦИ ON / OFF", 15, nrmText, b2TextRect, textBrush);
            else Draw->DrawTextW(L"ЗВУЦИ ON / OFF", 15, nrmText, b2TextRect, hgltBrush);
            if (!b3Hglt)Draw->DrawTextW(L"ПОМОЩ ЗА ИГРАТА", 16, nrmText, b3TextRect, textBrush);
            else Draw->DrawTextW(L"ПОМОЩ ЗА ИГРАТА", 16, nrmText, b3TextRect, hgltBrush);
        }
        Draw->DrawBitmap(bmpField[field_frame], D2D1::RectF(0, 50.0f, scr_width, scr_height));
        --field_delay;
        if (field_delay < 0)
        {
            field_delay = 3;
            ++field_frame;
            if (field_frame > 74)field_frame = 0;
        }
        
        
        
        
        ////////////////////////////////////////
        
        Draw->EndDraw();

    }

    ClearResources();
    std::remove(tmp_file);

    return (int) bMsg.wParam;
}