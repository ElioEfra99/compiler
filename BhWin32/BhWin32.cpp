// BhWin32.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "BhWin32.h"
#include <Richedit.h>
#include <stdio.h>
#include <iostream>
#include <commdlg.h>

#pragma comment(lib, "comctl32.lib")

#define MAX_LOADSTRING 100

// Variables globales:
HINSTANCE hInst;                                // instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal
HWND hWndToolbar;
static HWND hWndEdit;                           // Manejador de ventana
HFONT hFont;                                    // Manejador de fuente
CHARFORMAT2 cf;                                 // Formato del texto
DWORD dwEVM;                                    // Evento de captura

const int NUMBUTTONS = 4;

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Colocar código aquí.

    // Inicializar cadenas globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BHWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Realizar la inicialización de la aplicación:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BHWIN32));

    MSG msg;

    // Bucle principal de mensajes:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCIÓN: MyRegisterClass()
//
//  PROPÓSITO: Registra la clase de ventana.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BHWIN32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BHWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCIÓN: InitInstance(HINSTANCE, int)
//
//   PROPÓSITO: Guarda el identificador de instancia y crea la ventana principal
//
//   COMENTARIOS:
//
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Almacenar identificador de instancia en una variable global

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   /*Código para crear la barra de herramientas*/
   INITCOMMONCONTROLSEX InitCtrlEx;
   InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
   InitCtrlEx.dwICC = ICC_BAR_CLASSES;
   InitCommonControlsEx(&InitCtrlEx);
   TBBUTTON tbrButtons[4];
   tbrButtons[0].iBitmap = 0;
   tbrButtons[0].idCommand = ID_BTNINICIO;
   tbrButtons[0].fsState = TBSTATE_ENABLED;
   tbrButtons[0].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[0].dwData = 0L;
   tbrButtons[0].iString = (INT_PTR)L"Inicio";
   tbrButtons[1].iBitmap = 1;
   tbrButtons[1].idCommand = ID_BTNCOMPILAR;
   tbrButtons[1].fsState = TBSTATE_ENABLED;
   tbrButtons[1].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[1].dwData = 0L;
   tbrButtons[1].iString = (INT_PTR)L"Compilar";
   tbrButtons[2].iBitmap = 2;
   tbrButtons[2].idCommand = IDM_EXIT;
   tbrButtons[2].fsState = TBSTATE_ENABLED;
   tbrButtons[2].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[2].dwData = 0L;
   tbrButtons[2].iString = (INT_PTR)L"Salir";
   tbrButtons[3].iBitmap = 3;
   tbrButtons[3].idCommand = IDM_ABOUT;
   tbrButtons[3].fsState = TBSTATE_ENABLED;
   tbrButtons[3].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[3].dwData = 0L;
   tbrButtons[3].iString = (INT_PTR)L"Acerca de...";
   hWndToolbar = CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS
	   | CCS_TOP | CCS_NODIVIDER | TBSTYLE_FLAT,
	   IDB_TOOLBAR,
	   NUMBUTTONS,
	   hInstance,
	   IDB_TOOLBAR,
	   tbrButtons,
	   NUMBUTTONS,
	   0, 0, 0, 0,
	   sizeof(TBBUTTON));
   /*Se establece el tamaño de los iconos para cada botón*/
   SendMessage(hWndToolbar, TB_SETBITMAPSIZE, 0, MAKELPARAM(50, 32));
   /*Se reajusta el tamaño de la barra de herramientas*/
   SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0L);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO: Procesa mensajes de la ventana principal.
//
//  WM_COMMAND  - procesar el menú de aplicaciones
//  WM_PAINT    - Pintar la ventana principal
//  WM_DESTROY  - publicar un mensaje de salida y volver
//
//

long PopFileLength(FILE* file)
{
    int iCurrentPos, iFileLength;
    iCurrentPos = ftell(file);
    fseek(file, 0, SEEK_END);
    iFileLength = ftell(file);
    fseek(file, iCurrentPos, SEEK_SET);
    return iFileLength;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hStatus;            
    char cFile[MAX_PATH];
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Analizar las selecciones de menú:
        switch (wmId)
        {
        case IDM_ABOUT:
            MessageBox(NULL,
                (LPCWSTR)L"U need help?",
                (LPCWSTR)L"",
                MB_ICONWARNING | MB_DEFBUTTON2);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_ABRIR:
        {
            TCHAR szFile[MAX_PATH], szCaption[64 + _MAX_FNAME + _MAX_EXT];
            ZeroMemory(szFile, MAX_PATH);
            OPENFILENAME ofn;
            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
                OFN_HIDEREADONLY | OFN_CREATEPROMPT;
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = _T("Tipos de formatos soportados(*.doggo)\0 * .doggo\0Texto(*.doggo)\0\0");
            ofn.lpstrTitle = _T("Abrir archivo de Doggo");
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = MAX_PATH;
            if (IDOK == GetOpenFileName(&ofn)) {
                wsprintf(szCaption, _T("%s - %s"), szTitle, szFile[0] ? szFile :
                    _T("Sin archivo abierto"));
                SetWindowText(hWnd, szCaption);
                FILE* file;
                int iLength;
                PSTR pstrBuffer;
                TCHAR* ptchBuffer;
                wcstombs(cFile, szFile, MAX_PATH);
                if (NULL == (file = fopen(cFile, "rb"))) {
                    MessageBox(hWnd, L"Error al leer el archivo", L"Error",
                        MB_OK | MB_ICONERROR);
                }
                else {
                    iLength = PopFileLength(file);
                    if (NULL == (pstrBuffer = (PSTR)malloc
                    (sizeof(char) * (iLength + 1))) ||
                        NULL == (ptchBuffer = (TCHAR*)malloc
                        (sizeof(TCHAR) * (iLength + 1)))) {
                        fclose(file);
                        MessageBox(hWnd, L"Error al reservar memoria",
                            L"Error", MB_OK | MB_ICONERROR);
                    }
                    else {
                        fread(pstrBuffer, 1, iLength, file);
                        fclose(file);
                        pstrBuffer[iLength] = '\O';
                        mbstowcs(ptchBuffer, pstrBuffer, iLength + 1);
                        SetWindowText(hWndEdit, ptchBuffer);
                        free(pstrBuffer);
                        free(ptchBuffer);
                    }
                }
            }
            //El siguiente segmento de código da formato reemplazando texto
            //normal por texto con formato
            memset(&cf, 0, sizeof cf); //Se limpia la estructura del formato
            cf.cbSize = sizeof(CHARFORMAT2); //Se fija el tamaño de la estructura
            // Se establece la mascara para que sea posible aplicar color al texto
            cf.dwMask = CFM_COLOR; //| CFM_BACKCOLOR ;
            cf.crTextColor = RGB(255, 0, 0); //Se establece el color del texto
            // Se establece un rango de texto a seleccionar
            // SendMessage(hWndEdit, EM_SETSEL, (WPARAM)5, (LPARAM)9);
            // Se aplica el formato al rango seleccionado
            // SendMessage(hWndEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
            // Se reemplaza el rango seleccionado con el nuevo texto y formato
            // SendMessage(hWndEdit, EM_REPLACESEL, FALSE, (LPARAM)L"cara");
        }
        break;
        case IDM_GUARDAR:
        {
            TCHAR szFile[MAX_PATH];
            ZeroMemory(szFile, MAX_PATH);
            OPENFILENAME ofn;
            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
                OFN_OVERWRITEPROMPT;
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = _T("Tipos de formatos soportados(*.doggo)\0 * .doggo\0Texto(*.doggo)\0\0");
            ofn.lpstrTitle = _T("Guardar archivo de Doggo");
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = MAX_PATH;
            if (IDOK == GetSaveFileName(&ofn)) {
                FILE* file;
                int iLength;
                PSTR pstrBuffer;
                TCHAR* ptchBuffer = NULL;
                wcstombs(cFile, szFile, MAX_PATH);
                if (NULL == (file = fopen(cFile, "wb"))) {
                    MessageBox(hWnd, L"Error al crear el archivo", L"Error",
                        MB_OK | MB_ICONERROR);
                }
                else {
                    iLength = GetWindowTextLength(hWndEdit);
                    if (NULL == (pstrBuffer = (PSTR)malloc(sizeof(char) *
                    (iLength + 1))) ||
                        NULL == (ptchBuffer = (TCHAR*)malloc(sizeof(TCHAR) *
                        (iLength + 1))))
                    {
                        MessageBox(hWnd, L"Error al reservar memoria",
                            L"Error", MB_OK | MB_ICONERROR);
                        fclose(file);
                    }
                    GetWindowText(hWndEdit, ptchBuffer, iLength + 1);
                    wcstombs(pstrBuffer, ptchBuffer, iLength + 1);
                    fwrite(pstrBuffer, 1, iLength + 1, file);
                    fclose(file);
                    free(pstrBuffer);
                    free(ptchBuffer);
                }
            }
        }
        break;
        case ID_BTNCOMPILAR:
        {
            TCHAR szFile[MAX_PATH];
            ZeroMemory(szFile, MAX_PATH);
            OPENFILENAME ofn;
            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
                OFN_OVERWRITEPROMPT;
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = _T("Tipos de formatos soportados(*.doggo)\0 * .doggo\0Texto(*.doggo)\0\0");
            ofn.lpstrTitle = _T("Guardar archivo de Doggo");
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = MAX_PATH;
            if (IDOK == GetSaveFileName(&ofn)) {
                FILE* file;
                int iLength;
                PSTR pstrBuffer;
                TCHAR* ptchBuffer = NULL;
                wcstombs(cFile, szFile, MAX_PATH);
                if (NULL == (file = fopen(cFile, "wb"))) {
                    MessageBox(hWnd, L"Error al crear el archivo", L"Error",
                        MB_OK | MB_ICONERROR);
                }
                else {
                    iLength = GetWindowTextLength(hWndEdit);
                    if (NULL == (pstrBuffer = (PSTR)malloc(sizeof(char) *
                    (iLength + 1))) ||
                        NULL == (ptchBuffer = (TCHAR*)malloc(sizeof(TCHAR) *
                        (iLength + 1))))
                    {
                        MessageBox(hWnd, L"Error al reservar memoria",
                            L"Error", MB_OK | MB_ICONERROR);
                        fclose(file);
                    }
                    GetWindowText(hWndEdit, ptchBuffer, iLength + 1);
                    wcstombs(pstrBuffer, ptchBuffer, iLength + 1);
                    fwrite(pstrBuffer, 1, iLength + 1, file);
                    fclose(file);
                    free(pstrBuffer);
                    free(ptchBuffer);
                }
            }
            char compilerPath[255];
            char filePath[255];
            strcpy(compilerPath, "CompiladorDoggo.exe ");
            strcpy(filePath, cFile);
            strcat(compilerPath, filePath);
            system(compilerPath);
            // system("CompiladorDoggo.exe C:\\Users\\eliod\\Desktop\\definitivecode.doggo");
        }
        break;
        case ID_BTNINICIO:
            MessageBox(NULL,
                (LPCWSTR)L"Bienvenido!! :-}",
                (LPCWSTR)L"",
                MB_ICONWARNING | MB_DEFBUTTON2);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_NOTIFY:
        {
            NMHDR* pHdr = (NMHDR*)lParam;
            TCHAR szPosCur[MAX_LOADSTRING];
            if (pHdr->hwndFrom==hWndEdit)
            {
                if (pHdr->code==EN_SELCHANGE)
                {
                    swprintf(szPosCur, L"Linea; %d", 1 +
                    (WORD)SendMessage(hWndEdit, EM_LINEFROMCHAR, -1, 0L));
                    SendDlgItemMessage(hWnd, IDB_STATUS, SB_SETTEXT, 0,
                    (LPARAM)szPosCur);
                }
            }
        }
        break;
    
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Agregar cualquier código de dibujo que use hDC aquí...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        {
            LoadLibrary(L"riched20.dll");
            hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, RICHEDIT_CLASS, L"",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL
                | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                0, 0, 0, 0,
                hWnd, (HMENU)ID_EDITRICH, hInst, NULL);
            hFont = CreateFont(18, 0, 0, 0, 0, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
                L"Arial");
            hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, 
                                     hWnd, (HMENU)IDB_STATUS, hInst, NULL);
            SendMessage(hWndEdit, WM_SETFONT, (WPARAM)hFont, 0);
            SetFocus(hWndEdit);
        }
        break;
    case WM_SIZE:
        {
            MoveWindow(hWndEdit, 0, 50, LOWORD(lParam), HIWORD(lParam), TRUE);
            SendDlgItemMessage(hWnd, IDB_STATUS, WM_SIZE, 0, 0);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}