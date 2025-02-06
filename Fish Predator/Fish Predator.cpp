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

ID2D1Bitmap* bmpRIP{ nullptr };
ID2D1Bitmap* bmpFood{ nullptr };
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

dll::Object Hero{ nullptr };
float hero_targ_x = 0;
float hero_targ_y = 0;
bool hero_moving = false;
bool hero_killed = false;
float RIP_x = 0;
float RIP_y = 0;

std::vector<dll::Object> vAssets;
std::vector<dll::Object> vEvils;

dll::PROTON* Food = nullptr;

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

    if (!ClearMem(&bmpRIP))ErrLog(L"Error releasing bmpRIP !");
    if (!ClearMem(&bmpFood))ErrLog(L"Error releasing bmpFood !");

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

    ClearMem(&Hero);
    Hero = dll::ObjectFactory(hero, scr_width / 2, (float)(RandMachine(60, (int)(ground - 75.0f))));
    hero_moving = false;

    if (Food)
    {
        delete Food;
        Food = nullptr;
    }

    if (!vAssets.empty())
        for (int i = 0; i < vAssets.size(); ++i)ClearMem(&vAssets[i]);
    vAssets.clear();

    if (!vEvils.empty())
        for (int i = 0; i < vEvils.size(); ++i)ClearMem(&vEvils[i]);
    vEvils.clear();
}
void LevelUp()
{
    Draw->EndDraw();

    
    int bonus = 0;
    int txt_size = 0;

    while (bonus <= enemies_killed * 2 * level)
    {
        wchar_t bon_txt[25] = L"БОНУС: ";
        wchar_t add[5] = L"\0";
        txt_size = 0;
        wsprintf(add, L"%d", bonus);
        wcscat_s(bon_txt, add);
        for (int i = 0; i < 25; ++i)
        {
            if (bon_txt[i] != '\0')++txt_size;
            else break;
        }

        Draw->BeginDraw();
        Draw->DrawBitmap(bmpIntro[0], D2D1::RectF(0, 0, scr_width, scr_height));
        if (bigText && hgltBrush)Draw->DrawTextW(bon_txt, txt_size, bigText, D2D1::RectF(300.0f, scr_height / 2 - 100.0f, 
            scr_width, scr_height), hgltBrush);
        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
        Draw->EndDraw();
        Sleep(80);
        
        ++bonus;
    }
    if (bonus > 0)Sleep(2000);
    
    score += bonus;

    Draw->BeginDraw();
    Draw->DrawBitmap(bmpIntro[0], D2D1::RectF(0, 0, scr_width, scr_height));
    if (bigText && hgltBrush)Draw->DrawTextW(L"НИВОТО ПРЕМИНАТО !", 19, bigText, D2D1::RectF(200.0f, scr_height / 2 - 100.0f, 
        scr_width, scr_height), hgltBrush);
    Draw->EndDraw();
    if (sound)
    {
        PlaySound(NULL, NULL, NULL);
        PlaySound(L".\\res\\snd\\levelup.wav", NULL, SND_SYNC);
        PlaySound(snd_file, NULL, SND_ASYNC | SND_LOOP);
    }
    else Sleep(2000);

    ++level;
    secs = 180 + level * 10;
    enemies_killed = 0;
    if (Food)
    {
        delete Food;
        Food = nullptr;
    }

    ClearMem(&Hero);
    Hero = dll::ObjectFactory(hero, scr_width / 2, (float)(RandMachine(60, (int)(ground - 75.0f))));
    hero_moving = false;
    Hero->strenght += level + 1;


    if (!vAssets.empty())
        for (int i = 0; i < vAssets.size(); ++i)ClearMem(&vAssets[i]);
    vAssets.clear();

    if (!vEvils.empty())
        for (int i = 0; i < vEvils.size(); ++i)ClearMem(&vEvils[i]);
    vEvils.clear();

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
        if (secs <= 0)LevelUp();
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

        case mLvl:
            pause = true;
            if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
            if (MessageBox(hwnd, L"Ако прескочиш нивото, губиш бонуса !\n\nНаистина ли прескачаш ?",
                L"Следващо ниво !", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
            {
                pause = false;
                break;
            }
            enemies_killed = 0;
            LevelUp();
            break;

        case mExit:
            SendMessage(hwnd, WM_CLOSE, NULL, NULL);
            break;


        }
        break;

    case WM_LBUTTONDOWN:
        if (HIWORD(lParam) < 50)
        {

        }
        else
        {
            hero_targ_x = (float)(LOWORD(lParam));
            hero_targ_y = (float)(HIWORD(lParam));
            hero_moving = true;
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

            bmpRIP = Load(L".\\res\\img\\rip.png", Draw);
            bmpFood = Load(L".\\res\\img\\field\\food.png", Draw);

            
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
                DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 20.0f, L"", &midText);
            hr = iWriteFactory->CreateTextFormat(L"SEGOE PRINT", NULL, DWRITE_FONT_WEIGHT_ULTRA_BLACK,
                DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"", &bigText);
            hr = iWriteFactory->CreateTextFormat(L"SEGOE PRINT", NULL, DWRITE_FONT_WEIGHT_ULTRA_BLACK,
                DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"", &fishText);

            if (hr != S_OK)
            {
                ErrLog(L"Error creating iWriteFactory text formats !");
                ErrExit(eD2D);
            }
        }
    }

    if (Draw && bigText && hgltBrush)
    {
        for (int i = 0; i < 150; ++i)
        {
            Draw->BeginDraw();
            Draw->DrawBitmap(bmpIntro[intro_frame], D2D1::RectF(0, 0, scr_width, scr_height));
            ++intro_frame;
            if (intro_frame > 13)intro_frame = 0;

            if (RandMachine(0, 6) == 2)
            {
                Draw->DrawTextW(L"ГЛАДЕН ХИЩНИК !\n\n      dev. Daniel !", 37, bigText, D2D1::RectF(100.0f, 200.0f, scr_width,
                    scr_height), hgltBrush);
                Draw->EndDraw();
                mciSendString(L"play .\\res\\snd\\buzz.wav", NULL, NULL, NULL);
                Sleep(80);
            }
            else Draw->EndDraw();
        }

        Draw->BeginDraw();
        Draw->DrawBitmap(bmpIntro[intro_frame], D2D1::RectF(0, 0, scr_width, scr_height));
        Draw->DrawTextW(L"ГЛАДЕН ХИЩНИК !\n\n      dev. Daniel !", 37, bigText, D2D1::RectF(100.0f, 200.0f, scr_width,
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
            Draw->DrawTextW(L"ПАУЗА", 6, bigText, D2D1::RectF(scr_width / 2 - 150.0f, scr_height / 2 - 80.0f, scr_width,
                scr_height), hgltBrush);
            Draw->EndDraw();
            continue;
        }

        ////////////////////////////////

        //HERO & EVILS ************************
        
        if (Hero)
        {
            if (hero_moving)
            {
                if (!Hero->Move((float)(level), hero_targ_x, hero_targ_y)) hero_moving = false;
            }
        }
        
        if (vEvils.size() <= 5 + level && RandMachine(0, 5) == 2)
        {
            int evil_type = RandMachine(0, 8);
            int evil_dir = RandMachine(0, 1);

            switch (evil_type)
            {
            case 0:
                if (evil_dir == 0)
                {
                    vEvils.push_back(dll::ObjectFactory(fish1, scr_width, (float)(RandMachine(80, (int)(ground-80.0f)))));
                    vEvils.back()->dir = dirs::left;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                else if (evil_dir == 1)
                {
                    vEvils.push_back(dll::ObjectFactory(fish1, 0, (float)(RandMachine(80, (int)(ground-80.0f)))));
                    vEvils.back()->dir = dirs::right;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                break;

            case 1:
                if (evil_dir == 0)
                {
                    vEvils.push_back(dll::ObjectFactory(fish2, scr_width, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::left;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                else if (evil_dir == 1)
                {
                    vEvils.push_back(dll::ObjectFactory(fish2, 0, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::right;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                break;

            case 2:
                if (evil_dir == 0)
                {
                    vEvils.push_back(dll::ObjectFactory(fish3, scr_width, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::left;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                else if (evil_dir == 1)
                {
                    vEvils.push_back(dll::ObjectFactory(fish3, 0, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::right;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                break;

            case 3:
                if (evil_dir == 0)
                {
                    vEvils.push_back(dll::ObjectFactory(fish4, scr_width, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::left;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                else if (evil_dir == 1)
                {
                    vEvils.push_back(dll::ObjectFactory(fish4, 0, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::right;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                break;

            case 4:
                if (evil_dir == 0)
                {
                    vEvils.push_back(dll::ObjectFactory(fish5, scr_width, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::left;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                else if (evil_dir == 1)
                {
                    vEvils.push_back(dll::ObjectFactory(fish5, 0, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::right;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                break;

            case 5:
                if (evil_dir == 0)
                {
                    vEvils.push_back(dll::ObjectFactory(fish6, scr_width, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::left;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                else if (evil_dir == 1)
                {
                    vEvils.push_back(dll::ObjectFactory(fish6, 0, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::right;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                break;

            case 6:
                if (evil_dir == 0)
                {
                    vEvils.push_back(dll::ObjectFactory(fish7, scr_width, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::left;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                else if (evil_dir == 1)
                {
                    vEvils.push_back(dll::ObjectFactory(fish7, 0, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::right;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                break;

            case 7:
                if (evil_dir == 0)
                {
                    vEvils.push_back(dll::ObjectFactory(fish8, scr_width, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::left;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                else if (evil_dir == 1)
                {
                    vEvils.push_back(dll::ObjectFactory(fish8, 0, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::right;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                break;

            case 8:
                if (evil_dir == 0)
                {
                    vEvils.push_back(dll::ObjectFactory(fish9, scr_width, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::left;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                else if (evil_dir == 1)
                {
                    vEvils.push_back(dll::ObjectFactory(fish9, 0, (float)(RandMachine(80, (int)(ground - 80.0f)))));
                    vEvils.back()->dir = dirs::right;
                    vEvils.back()->strenght += RandMachine(0, level + 3);
                    break;
                }
                break;
            }
        }

        if (!vEvils.empty() && Hero)
        {
            for (std::vector<dll::Object>::iterator evil = vEvils.begin(); evil < vEvils.end(); ++evil)
            {
                FPOINT going_to = (*evil)->AINextMove(FPOINT(Hero->start.x, Hero->start.y));
                (*evil)->Move((float)(level), going_to.x, going_to.y);
            }
        }

        if (!vEvils.empty() && Hero)
        {
            for (std::vector<dll::Object>::iterator evil = vEvils.begin(); evil < vEvils.end(); evil++)
            {
                float current_distance = Hero->Distance(Hero->center, (*evil)->center);
                if (current_distance <= Hero->GetWidth() / 2 + (*evil)->GetWidth() / 2)
                {
                    if (Hero->strenght >= (*evil)->strenght)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\eaten.wav", NULL, NULL, NULL);
                        Hero->strenght += (*evil)->strenght / 3;
                        (*evil)->Release();
                        vEvils.erase(evil);
                        ++enemies_killed;
                        score += level;
                        break;
                    }
                    else
                    {
                        RIP_x = Hero->center.x;
                        RIP_y = Hero->center.y;
                        hero_killed = true;
                        ClearMem(&Hero);
                        break;
                    }
                }
            }
        }

        // ASSETS **********************

        if (vAssets.size() < 10 && RandMachine(0, 10) == 5)
        {
            int chance = RandMachine(0, 3);
            int rand_dir = RandMachine(0, 3);

            switch (chance)
            {
            case 0:
                vAssets.push_back(dll::ObjectFactory(bubbles, (float)(RandMachine(100, (int)(scr_width - 100))),
                    ground - 70.0f));
                vAssets.back()->dir = dirs::up;
                break;

            case 1:
                if (rand_dir == 0 || rand_dir == 2)
                {
                    vAssets.push_back(dll::ObjectFactory(grass, scr_width, ground - 157.0f));
                    vAssets.back()->dir = dirs::left;
                }
                else
                {
                    vAssets.push_back(dll::ObjectFactory(grass, 0, ground - 157.0f));
                    vAssets.back()->dir = dirs::right;
                }
                break;

            case 2:
                if (rand_dir == 0)
                {
                    vAssets.push_back(dll::ObjectFactory(jelly1, -52.0f, (float)(RandMachine(50, 650))));
                    vAssets.back()->dir = dirs::right;
                    break;
                }
                else if (rand_dir == 1)
                {
                    vAssets.push_back(dll::ObjectFactory(jelly1, scr_width + 52.0f, (float)(RandMachine(50, 650))));
                    vAssets.back()->dir = dirs::left;
                    break;
                }
                else if (rand_dir == 2)
                {
                    vAssets.push_back(dll::ObjectFactory(jelly1, (float)(RandMachine(150, 900)), sky));
                    vAssets.back()->dir = dirs::down;
                    break;
                }
                else if (rand_dir == 3)
                {
                    vAssets.push_back(dll::ObjectFactory(jelly1, (float)(RandMachine(150, 900)), ground));
                    vAssets.back()->dir = dirs::up;
                    break;
                }
                break;

            case 3:
                if (rand_dir == 0)
                {
                    vAssets.push_back(dll::ObjectFactory(jelly2, -70.0f, (float)(RandMachine(50, 650))));
                    vAssets.back()->dir = dirs::right;
                    break;
                }
                else if (rand_dir == 1)
                {
                    vAssets.push_back(dll::ObjectFactory(jelly2, scr_width + 70.0f, (float)(RandMachine(50, 650))));
                    vAssets.back()->dir = dirs::left;
                    break;
                }
                else if (rand_dir == 2)
                {
                    vAssets.push_back(dll::ObjectFactory(jelly2, (float)(RandMachine(150, 900)), sky));
                    vAssets.back()->dir = dirs::down;
                    break;
                }
                else if (rand_dir == 3)
                {
                    vAssets.push_back(dll::ObjectFactory(jelly2, (float)(RandMachine(150, 900)), ground));
                    vAssets.back()->dir = dirs::up;
                    break;
                }
                break;
            }
        }

        if (!vAssets.empty())
        {
            for (std::vector<dll::Object>::iterator asset = vAssets.begin(); asset < vAssets.end(); asset++)
            {
                if (!(*asset)->Move((float)(level)))
                {
                    (*asset)->Release();
                    vAssets.erase(asset);
                    break;
                }
            }
        }

        if (!Food && RandMachine(0, 1000) == 666)
        {
            Food = new dll::PROTON((float)(RandMachine(70, 900)), sky, 70.0f, 71.0f);
        }

        if (Food && Hero)
        {
            Food->start.y += level;
            Food->SetEdges();
            
            float dist = Hero->Distance(FPOINT(Food->start.x + 35.0f, Food->start.y + 30.0f), Hero->center);
            if (dist <= Hero->GetWidth() / 2 + 35.0f)
            {
                if (sound)mciSendString(L"play .\\res\\snd\\eaten.wav", NULL, NULL, NULL);
                Hero->strenght += level;
                delete Food;
                Food = nullptr;
            }
            else if (Food->end.y > ground)
            {
                delete Food;
                Food = nullptr;
            }
        }

        ////////////////////////////////

        ////////////////////////////////////////
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

        wchar_t stat_txt[150] = L"ХИЩНИК: ";
        wchar_t stat_add[5] = L"\0";
        int stat_size = 0;

        wcscat_s(stat_txt, current_player);
        
        wcscat_s(stat_txt, L", НИВО: ");
        wsprintf(stat_add, L"%d", level);
        wcscat_s(stat_txt, stat_add);

        wcscat_s(stat_txt, L", ТОЧКИ: ");
        wsprintf(stat_add, L"%d", score);
        wcscat_s(stat_txt, stat_add);

        wcscat_s(stat_txt, L", ВРЕМЕ: ");
        if (mins < 10)wcscat_s(stat_txt, L"0");
        wsprintf(stat_add, L"%d", mins);
        wcscat_s(stat_txt, stat_add);
        wcscat_s(stat_txt, L" : ");
        if (secs - mins * 60 < 10)wcscat_s(stat_txt, L"0");
        wsprintf(stat_add, L"%d", secs - mins * 60);
        wcscat_s(stat_txt, stat_add);

        for (int i = 0; i < 150; i++)
        {
            if (stat_txt[i] != '\0')stat_size++;
            else break;
        }

        if (midText && hgltBrush)
            Draw->DrawTextW(stat_txt, stat_size, midText, D2D1::RectF(10.0f, ground + 5.0f, scr_width, scr_height), hgltBrush);


        ////////////////////////////////////////

        // HERO KILLED ************************

        if (hero_killed)
        {
            Draw->DrawBitmap(bmpRIP, D2D1::RectF(RIP_x, RIP_y, RIP_x + 80.0f, RIP_y + 94.0f));
            Draw->EndDraw();
            if (sound)
            {
                PlaySound(NULL, NULL, NULL);
                PlaySound(L".\\res\\snd\\killed.wav", NULL, SND_SYNC);
            }
            else Sleep(3000);
            GameOver();
        }

        // DRAW HERO **************************

        if (Hero)
        {
            if (Hero->strenght > 150)
            {
                int hero_frame = Hero->GetFrame();

                if (Hero->dir == dirs::right)
                    Draw->DrawBitmap(bmpBigHeroR[hero_frame], Resizer(bmpBigHeroR[hero_frame], Hero->start.x, Hero->start.y));
                else if (Hero->dir == dirs::left)
                    Draw->DrawBitmap(bmpBigHeroL[hero_frame], Resizer(bmpBigHeroL[hero_frame], Hero->start.x, Hero->start.y));
            }
            else if (Hero->strenght > 75)
            {
                int hero_frame = Hero->GetFrame();

                if (Hero->dir == dirs::right)
                    Draw->DrawBitmap(bmpMidHeroR[hero_frame], Resizer(bmpMidHeroR[hero_frame], Hero->start.x, Hero->start.y));
                else if (Hero->dir == dirs::left)
                    Draw->DrawBitmap(bmpMidHeroL[hero_frame], Resizer(bmpMidHeroL[hero_frame], Hero->start.x, Hero->start.y));
            }
            else
            {
                int hero_frame = Hero->GetFrame();

                if (Hero->dir == dirs::right)
                    Draw->DrawBitmap(bmpSmallHeroR[hero_frame], Resizer(bmpSmallHeroR[hero_frame], Hero->start.x, Hero->start.y));
                else if (Hero->dir == dirs::left)
                    Draw->DrawBitmap(bmpSmallHeroL[hero_frame], Resizer(bmpSmallHeroL[hero_frame], Hero->start.x, Hero->start.y));
            }

            wchar_t strenght_txt[5] = { 0 };
            int size = 0;

            wsprintf(strenght_txt, L"%d", Hero->strenght);
            for (int i = 0; i < 5; ++i)
            {
                if (strenght_txt[i] != '\0')++size;
                else break;
            }
            if (fishText && hgltBrush)
                Draw->DrawTextW(strenght_txt, size, fishText, D2D1::RectF(Hero->start.x + 20.0f, Hero->end.y + 5.0f,
                    Hero->start.x + 85.0f, Hero->end.y + 30.0f), hgltBrush);
        }

        // DRAW ASSETS ************************

        if (!vAssets.empty())
        {
            for (std::vector<dll::Object>::iterator asset = vAssets.begin(); asset < vAssets.end(); ++asset)
            {
                int asset_frame = (*asset)->GetFrame();

                switch ((*asset)->GetType())
                {
                case bubbles:
                    Draw->DrawBitmap(bmpBubbles[asset_frame], Resizer(bmpBubbles[asset_frame], (*asset)->start.x,
                        (*asset)->start.y));
                    break;

                case grass:
                    Draw->DrawBitmap(bmpGrass[asset_frame], Resizer(bmpGrass[asset_frame], (*asset)->start.x,
                        (*asset)->start.y));
                    break;

                case jelly1:
                    Draw->DrawBitmap(bmpJelly1[asset_frame], Resizer(bmpJelly1[asset_frame], (*asset)->start.x,
                        (*asset)->start.y));
                    break;

                case jelly2:
                    Draw->DrawBitmap(bmpJelly2[asset_frame], Resizer(bmpJelly2[asset_frame], (*asset)->start.x,
                        (*asset)->start.y));
                    break;
                }
            }
        }

        if (Food)Draw->DrawBitmap(bmpFood, D2D1::RectF(Food->start.x, Food->start.y, Food->end.x, Food->end.y));

        ///////////////////////////////////////

        // DRAW EVILS *************************
        
        if (!vEvils.empty())
        {
            for (std::vector<dll::Object>::iterator evil = vEvils.begin(); evil < vEvils.end(); ++evil)
            {
                int evil_frame = (*evil)->GetFrame();

                switch ((*evil)->GetType())
                {
                case fish1:
                    if ((*evil)->dir == dirs::left)
                        Draw->DrawBitmap(bmpFish1L[evil_frame], Resizer(bmpFish1L[evil_frame], (*evil)->start.x, 
                            (*evil)->start.y));
                    else
                        Draw->DrawBitmap(bmpFish1R[evil_frame], Resizer(bmpFish1R[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    break;

                case fish2:
                    if ((*evil)->dir == dirs::left)
                        Draw->DrawBitmap(bmpFish2L[evil_frame], Resizer(bmpFish2L[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    else
                        Draw->DrawBitmap(bmpFish2R[evil_frame], Resizer(bmpFish2R[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    break;

                case fish3:
                    if ((*evil)->dir == dirs::left)
                        Draw->DrawBitmap(bmpFish3L[evil_frame], Resizer(bmpFish3L[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    else
                        Draw->DrawBitmap(bmpFish3R[evil_frame], Resizer(bmpFish3R[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    break;

                case fish4:
                    if ((*evil)->dir == dirs::left)
                        Draw->DrawBitmap(bmpFish4L[evil_frame], Resizer(bmpFish4L[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    else
                        Draw->DrawBitmap(bmpFish4R[evil_frame], Resizer(bmpFish4R[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    break;

                case fish5:
                    if ((*evil)->dir == dirs::left)
                        Draw->DrawBitmap(bmpFish5L[evil_frame], Resizer(bmpFish5L[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    else
                        Draw->DrawBitmap(bmpFish5R[evil_frame], Resizer(bmpFish5R[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    break;

                case fish6:
                    if ((*evil)->dir == dirs::left)
                        Draw->DrawBitmap(bmpFish6L[evil_frame], Resizer(bmpFish6L[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    else
                        Draw->DrawBitmap(bmpFish6R[evil_frame], Resizer(bmpFish6R[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    break;

                case fish7:
                    if ((*evil)->dir == dirs::left)
                        Draw->DrawBitmap(bmpFish7L[evil_frame], Resizer(bmpFish7L[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    else
                        Draw->DrawBitmap(bmpFish7R[evil_frame], Resizer(bmpFish7R[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    break;

                case fish8:
                    if ((*evil)->dir == dirs::left)
                        Draw->DrawBitmap(bmpFish8L[evil_frame], Resizer(bmpFish8L[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    else
                        Draw->DrawBitmap(bmpFish8R[evil_frame], Resizer(bmpFish8R[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    break;

                case fish9:
                    if ((*evil)->dir == dirs::left)
                        Draw->DrawBitmap(bmpFish9L[evil_frame], Resizer(bmpFish9L[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    else
                        Draw->DrawBitmap(bmpFish9R[evil_frame], Resizer(bmpFish9R[evil_frame], (*evil)->start.x,
                            (*evil)->start.y));
                    break;
                }

                wchar_t strenght_txt[5] = { 0 };
                int size = 0;

                wsprintf(strenght_txt, L"%d", (*evil)->strenght);
                for (int i = 0; i < 5; ++i)
                {
                    if (strenght_txt[i] != '\0')++size;
                    else break;
                }
                if (fishText && hgltBrush)
                    Draw->DrawTextW(strenght_txt, size, fishText, D2D1::RectF((*evil)->start.x + 20.0f, (*evil)->end.y + 5.0f,
                        (*evil)->start.x + 85.0f, (*evil)->end.y + 30.0f), hgltBrush);
            }
        }
        
        ////////////////////////////////////////
        
        Draw->EndDraw();

    }

    ClearResources();
    std::remove(tmp_file);

    return (int) bMsg.wParam;
}