#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <dirent.h>

typedef struct {
    float R, G, B;
} fRGB;

typedef struct {
    int R, G, B;
} iRGB;
typedef struct {
    float getX, getY;
} fPoint;
typedef struct  {
    int getX, getY;
} iPoint;
typedef struct  {
    int family, style,angle, size;
} Font;

typedef struct {
    int idx, dsl, r, c, sel, type;
} options;
typedef struct {
	int shape, x, y, rot;
	iPoint lpx;
	char *txt, *fnt;
    int flag;//1 - Set, 0 - Empty
    options opt;
}shapeX2;
typedef struct {
    int num;
    char file[100][50];
    int type[100];//0 - file, 1 Directory
}filesList;
typedef struct {
    int num;
    char txtFile[50][200];
}textFilesList;

//Prototyping
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
//-----------------------------------------------------
void drawHandle(HDC hDC);

void drawAll();
void drawTriangle();
void drawTriangle3d();
void drawTriangleDemo();
void drawSquare();
void drawCube();
void DrawCircle(float cx, float cy, float r, int num_segments);
void fillCircle(float cx, float cy, float r, int num_segments);

void drawLine(float x1, float y1, float x2, float y2);
void drawRectangle(float x1, float y1, float x2, float y2);
void drawText(float x, float y, void *font, char* string);
void fillRectangle(float x1, float y1, float x2, float y2);
void drawBitmap(float x, float y, void *font, char* string);
void loadFile();

//Absolute locations on the screen
float getX(int x);
float getY(int y);
float txtX (int row);
float txtY (int col);
float getPixH(float px);
float getPixV(float px);

fPoint p1, p2;
iPoint lp1, lp2;
iPoint p3;
int flip = 0;
fRGB fColor;//Text
fRGB bColor;//Text

//Menus / DropDowns
void printMenus();
void drawRectMenus(int y, int x, int w, float cl, int ct);
void selectMenuItem();
//-------------Global variables---------
const int VIDEO_W = 640;//Video Width
const int VIDEO_H = 480;//Video Height
const int TXT_ROWS = 25;
const int TXT_COLS = 80;

int mouseV=4;//Velocity
int mouseX=320;
int mouseY=240;

int flagMouseDown = 0;
int flagFill = 0;
int selItem = 0;
int flagP1 = 0;
int flagPaint = 0;

float theta = 0.0f;
BOOL bQuit = 0;
int shape = 0;//Selected Shape
int shape_lmt = 17; // Shape Limit
float penSize = 1.0f, pen = 1.0f;

int ctrText = 0;
char listText[1000][50];
fRGB g1 [600][400]; //600 x 400 //Bitmap

unsigned char g2 [600*360*3]; //600 x 400 //Bitmap
shapeX2 currShape;
//unsigned char g2 [590][360][3]; //600 x 400 //Bitmap


//--------- Screen Array ------------
int shapeCtr;
int shapeLMT = 1000;
//------------------------------------
int exMenu = 0;

char xData[245*900];
int iHeader = 180; // Bitmap Header
int menuSelected[10] = {0,0,0,0,1,1,1,1,0,0};
int menu_lmt = 20;

int size, sz_w, sz_h; //Size Variables
int menuSel = 0;


int pos_x = 0;//x to the Print routines - Max 80
int pos_y = 0;//y to the Print routines - Max 25

char dText[50] = "This is a testing Text";
// ============ InputBox and MessageBox ==============
char title[80];
char msg[80*15];
int flagInputBox = 0;
int flagMsgBox = 0;
char input[80];
int iBox=0;
int inpBoxCTR = 0;
int flagCap = 0;
int flagColorPicker = 0;
int selectedColor = 0;
int flagFontPicker = 0;

int selectedFont[4] = {1, 1, 1, 6};
int selectFontCTR = 0;
int lf_font[4];
fRGB dColor;
Font dFont;

int flagShape = 0;
int selectedShape = -1;
int selectMode = 0;
int flagCut = 0, flagCopy = 0, flagPaste=0, flagDelete=0, flagMove=0;
int flagTest = 0;
//====================================================
char currPath[50] = "c:\\temp\\bpaint\\";
char currFile[20] = "cautocad1.txt";
//====================================================

    // AutoCad Project Variables
    //ArrayList<shapeX> compList;
    shapeX2 compList[1001];
    int rotation = 0;
    int flagSelect = 0;
    float zoom = 1.0f, zoomTrack = 1.0f;
    int flagZoom = 0;
    int IC_Start = 30;
    char strx[256][10] = {0};//Used to Split
    int ctrComp = 0;
    int compListsize = 0;
    filesList fList[100];
    textFilesList fTxtList[100];

    int ctrFiles = 0, ctrLink = 0, ctrTxtFile=0, ctrRestore=0, wind_Pos=100;
    int flagWindResize = 0, flagWindScroll=0, flagWindSelFile=0;
    filesList taskList;


    //Goal to Create the Menus: File, Edit, Help and drop-downs: Shape, Size, PenSize;
    //Windows Manager Menu
    char *menuStart[]= {"Start", "Add Link", "Remove All", "Task Manager", "Help", "About", "Close Windows", "Exit"};
    char *menuLink[]= {"Link", "Open(Explorer)", "Open(Text)", "----------","Move", "Copy", "Delete"};
    char *menuWindow[]= {"Window", "Close", "Move", "Select File", "Resize", "Maximize"};



    //char *ddShapes[]= {"Shapes", "Drawing", "Rectangle (p1, p2)", "Circle (p1, p2)", "Square", "Rectangle (H)", "Rectangle (V)", "Circle", "Line (p1, p2)", "Text"};
    char *ddShapes[]= {"Shapes", "---", "Line", "Rectangle", "Circle", "Text", "***************", "Wire", "Resistor", "Capacitor", "Electrolytic Capacitor", "Diode", "LED", "LDR",
                "Transistor NPN", "Transistor PNP", "Variable Resistor", "Variable Capacitor", "Positive", "Negative", "Ground", "Battery", "Speaker", "Connector", "Dot", "Inductor 2 pins",
                "Inductor 3 pins", "Small Inductor", "Relay", "Transformer", "RF Coil", "***************", "IC 2 pins", "IC 4 pins", "IC 6 pins", "IC 8 pins", "IC 10 pins", "IC 12 pins", "IC 14 pins",
                "IC 16 pins", "IC 18 pins", "IC 20 pins", "IC 22 pins", "IC 24 pins", "IC 26 pins", "IC 28 pins", "IC 30 pins", "IC 32 pins", "IC 34 pins", "IC 36 pins", "IC 38 pins", "IC 40 pins", "IC 42 pins",
                "IC 44 pins", "IC 46 pins", "IC 48 pins", "IC 50 pins", "***************"};

    //char *ddRotate[]= {"Rotate", "0-Right", "1-Down", "2-Left", "3-Up"};
    //char *ddPenSizes[]= {"PenSizes", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "12", "14", "16", "18", "20", "25", "30"};

    char *fontFamily[] = {"Family","Font #1"};
    char *fontStyle[] = {"Style", "normal", "italic", "bold", "bold-italic"};
    char *fontInclination[] = {"Angle", "  0"," 45", " 90", "135", "180", "225", "270", "315"};
    char *fontSize[] = {"Size","5", "6", "7", "8", "9", "10", "12", "14", "16", "18", "20", "24", "28", "32", "38", "44", "48", "54", "60", "66", "72", "80", "90","100"};


char charCode[128][8] =  {
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},
{0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},
{0x18, 0x7E, 0x03, 0x3E, 0x60, 0x3F, 0x18, 0x00},
{0x00, 0x63, 0x30, 0x18, 0x0C, 0x06, 0x63, 0x00},
{0x1C, 0x36, 0x1C, 0x6E, 0x33, 0x33, 0x6E, 0x00},
{0x18, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},
{0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},
{0x00, 0x77, 0x3E, 0x7F, 0x3E, 0x77, 0x00, 0x00},
{0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x0C, 0x00},
{0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x00},
{0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},
{0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},
{0x18, 0x1E, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00},
{0x3E, 0x63, 0x30, 0x18, 0x0C, 0x66, 0x7F, 0x00},
{0x3E, 0x63, 0x60, 0x3C, 0x60, 0x63, 0x3E, 0x00},
{0x30, 0x38, 0x3C, 0x36, 0x7F, 0x30, 0x30, 0x00},
{0x7F, 0x03, 0x3F, 0x60, 0x60, 0x63, 0x3E, 0x00},
{0x3E, 0x63, 0x03, 0x3F, 0x63, 0x63, 0x3E, 0x00},
{0x7F, 0x63, 0x60, 0x30, 0x18, 0x18, 0x18, 0x00},
{0x3E, 0x63, 0x63, 0x3E, 0x63, 0x63, 0x3E, 0x00},
{0x3E, 0x63, 0x63, 0x7E, 0x60, 0x63, 0x3E, 0x00},
{0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00},
{0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0C},
{0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x00},
{0x00, 0x00, 0x7F, 0x00, 0x00, 0x7F, 0x00, 0x00},
{0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},
{0x3E, 0x63, 0x60, 0x30, 0x18, 0x00, 0x18, 0x00},
{0x3E, 0x63, 0x63, 0x7B, 0x3B, 0x03, 0x7E, 0x00},
{0x1C, 0x36, 0x63, 0x63, 0x7F, 0x63, 0x63, 0x00},
{0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},
{0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},
{0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},
{0x7F, 0x43, 0x03, 0x1F, 0x03, 0x43, 0x7F, 0x00},
{0x7F, 0x46, 0x06, 0x3E, 0x06, 0x06, 0x0F, 0x00},
{0x3E, 0x63, 0x03, 0x03, 0x7B, 0x63, 0x3E, 0x00},
{0x63, 0x63, 0x63, 0x7F, 0x63, 0x63, 0x63, 0x00},
{0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00},
{0x3C, 0x18, 0x18, 0x18, 0x1B, 0x1B, 0x0E, 0x00},
{0x63, 0x33, 0x1B, 0x0F, 0x1B, 0x33, 0x63, 0x00},
{0x0F, 0x06, 0x06, 0x06, 0x06, 0x46, 0x7F, 0x00},
{0x63, 0x77, 0x7F, 0x6B, 0x6B, 0x63, 0x63, 0x00},
{0x63, 0x67, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x00},
{0x3E, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00},
{0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},
{0x3E, 0x63, 0x63, 0x63, 0x63, 0x6B, 0x3E, 0x60},
{0x3F, 0x63, 0x63, 0x3F, 0x1B, 0x33, 0x63, 0x00},
{0x3E, 0x63, 0x03, 0x3E, 0x60, 0x63, 0x3E, 0x00},
{0x7E, 0x5A, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00},
{0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00},
{0x63, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x08, 0x00},
{0x63, 0x63, 0x6B, 0x6B, 0x7F, 0x77, 0x63, 0x00},
{0x63, 0x36, 0x1C, 0x1C, 0x1C, 0x36, 0x63, 0x00},
{0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x3C, 0x00},
{0x7F, 0x61, 0x30, 0x18, 0x0C, 0x46, 0x7F, 0x00},
{0x3E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x3E, 0x00},
{0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},
{0x3E, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3E, 0x00},
{0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
{0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x7E, 0x00},
{0x07, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x3F, 0x00},
{0x00, 0x00, 0x3E, 0x63, 0x03, 0x63, 0x3E, 0x00},
{0x38, 0x30, 0x3E, 0x33, 0x33, 0x33, 0x7E, 0x00},
{0x00, 0x00, 0x3E, 0x63, 0x7F, 0x03, 0x3E, 0x00},
{0x38, 0x6C, 0x0C, 0x3F, 0x0C, 0x0C, 0x1E, 0x00},
{0x00, 0x00, 0x6E, 0x73, 0x63, 0x7E, 0x60, 0x3E},
{0x07, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x67, 0x00},
{0x18, 0x00, 0x1C, 0x18, 0x18, 0x18, 0x3C, 0x00},
{0x30, 0x00, 0x38, 0x30, 0x30, 0x30, 0x33, 0x1E},
{0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},
{0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x38, 0x00},
{0x00, 0x00, 0x36, 0x7F, 0x6B, 0x6B, 0x63, 0x00},
{0x00, 0x00, 0x3B, 0x66, 0x66, 0x66, 0x66, 0x00},
{0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x3E, 0x00},
{0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},
{0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},
{0x00, 0x00, 0x3B, 0x66, 0x06, 0x06, 0x0F, 0x00},
{0x00, 0x00, 0x3E, 0x03, 0x3E, 0x60, 0x3E, 0x00},
{0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x6C, 0x38, 0x00},
{0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},
{0x00, 0x00, 0x63, 0x63, 0x36, 0x1C, 0x08, 0x00},
{0x00, 0x00, 0x63, 0x63, 0x6B, 0x7F, 0x36, 0x00},
{0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},
{0x00, 0x00, 0x63, 0x63, 0x73, 0x6E, 0x60, 0x3E},
{0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},
{0x70, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x70, 0x00},
{0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},
{0x0E, 0x18, 0x18, 0x70, 0x18, 0x18, 0x0E, 0x00},
{0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

//--------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;

    if (!RegisterClassEx(&wcex))
        return 0;
    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "Basic Windows Manager Application [OpenGL]",
                          WS_OVERLAPPEDWINDOW,//WS_POPUPWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          VIDEO_W,
                          VIDEO_H,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    start();
    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            drawHandle(hDC);

        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            keyboardCheck(wParam);
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);

    //glEnable(GL_DEPTH_TEST);//Tutorial 5 - Z-Buffer
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);//Tutorial 5 - Z-Buffer
    //glDepthMask(GL_FALSE);

}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

//========================== NEW CODE =========================================
void drawHandle(HDC hDC) {
            /* OpenGL animation code goes here */
            //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

            //glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//Tutorial 5
            glPushMatrix();

            //glRotatef(theta, 0.0f, 0.0f, 1.0f);
            //Cube
            if (shape== 4) glRotatef(theta, 0.0f, 1.0f, 1.0f);//Tutorial 6 - A cube
            drawAll();//Draw all the shapes
            glPopMatrix();
            SwapBuffers(hDC);
            if (shape== 4) theta += 0.5f;//Rotation velocity
            //Sleep (1);//Pause
}
//---------------------------------------------------------------------------------
void drawAll() {
            //Centrallized Variables
            loop();
            glColor3f(0.0f,0.0f, 1.0f);//Color to Draw;
            glLineWidth(0.5f);
            //-----------------------------------------
}

//============================ STANDARD SHAPES ==============================
void drawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);//output vertex
        glVertex2f(x2, y2);//output vertex
    glEnd();
}
void drawRectangle(float x1, float y1, float x2, float y2)
{
    drawLine(x1, y1, x1, y2);
    drawLine(x1, y2, x2, y2);

    drawLine(x2, y2, x2, y1);
    drawLine(x2, y1, x1, y1);
}
void fillRectangle(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x1, y2);
        glVertex2f(x2, y2);
        glVertex2f(x2, y1);
    glEnd();
}
//===========================================================================
//*********************** DRAW BITMAP FILES ****************************
void drawBitmap(float x, float y, void *font, char* string) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)

        int i, j, c, r;
        float cx = 0.00f;
        float fct = (float) 1/200;
        float rx = 0.0f;

        for (c = 0; c<=200; c++) {
            cx = cx+fct;
            rx = 0.0f;
            for (r=0; r<=200;r++) {
                    rx = rx + fct;
                    fillRectangle(rx, cx, rx+fct, cx+fct);//Scalable dot
            }
        }

}
void drawBitmapFonts() {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
        int i, j, c, r;
        float cx = 0.00f;
        int scl = 900;
        float fct = (float) 2/scl;
        float rx = -1.0f;
        int ctx = 0;
        char ch;
        float px = 0;

        glLineWidth(0.5f);
        for (c = 0; c<245; c++) {
            cx = cx+fct;
            rx = -1.0f;
            for (r=0; r<scl;r++) {
                    rx = rx + fct;
                    ctx = (c*900)+r;
                    ch = xData[ctx];

                    px = (float) 255/ch;
                    px = 1.0f - px;


                    glColor3f(px, px, px);

                    fillRectangle(rx, cx, rx+fct, cx+fct);//Scalable dot
            }
        }

}


//****************** Draw Text / Print Text *******************************
const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "X" : " ");
    }

    return b;
}

void drawCharCode() {
    int i = 0;
    int ch = 0;
    for (i=0; i<7; i++) {
        ch = (int) charCode[65+3][i];
        printf("\n%s", byte_to_binary(ch));
    }
}

void drawDot(float x, float y) {
    glBegin(GL_POINTS);
        //glVertex3f(x, y, -0.25f);
        glVertex2f(x, y);
    glEnd( );
}
void drawChar(char str, float x, float y, float fct_x, float fct_y) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
       //----------- Printing the Character on the Screen ----------
        float rx, cx;
        int r, c;
        glLineWidth(0.5f);

        int ch = 0; int ctx = (int) str;
        char bt[9];
        char ch1;

        rx = y;
        for (r = 0; r<8; r++) {
            cx = x;
            ch = (int) charCode[ctx][7-r];
            strcpy(bt, byte_to_binary(ch));
            for (c=0; c<8;c++) {
                    if (bt[7-c]==' ' ) {
                        //glColor3f(1.0f, 1.0f, 1.f);//BackGround
                        setColorRGB(bColor);
                    } else {
                        glColor3f(0.0f, 0.0f, 0.f);//ForeColor
                        setColorRGB(fColor);
                    }
                    cx = cx + fct_x;
                    fillRectangle(cx, rx, cx+fct_x, rx+fct_y);//Scalable dot

            }
            rx = rx+fct_y;
        }
        //---------------------------------------------------------

}
void drawCharF(char str, float x, float y, float fct_x, float fct_y, Font f, fRGB xColor) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
       //----------- Printing the Character on the Screen ----------
        float rx, cx;
        int r, c;
        glLineWidth(0.5f);

        int ch = 0; int ctx = (int) str;
        char bt[9];
        char ch1;
        float dsl_x = 0.00f;

        rx = y;
        for (r = 0; r<8; r++) {
            cx = x+dsl_x;
            ch = (int) charCode[ctx][7-r];
            strcpy(bt, byte_to_binary(ch));
            for (c=0; c<8;c++) {
                    cx = cx + fct_x;
                    if (bt[7-c]==' ' ) {
                    } else {
                        glColor3f(0.0f, 0.0f, 0.f);//ForeColor
                        setColorRGB(xColor);
                        fillRectangle(cx, rx, cx+fct_x, rx+fct_y);//Scalable dot
                        //fontStyle[] = {"Style", "normal", "italic", "bold", "bold-italic"};
                        if (f.style==3 || f.style == 4) {//Bold
                            fillRectangle(cx, rx, cx+fct_x*1.25, rx+fct_y*1.25);//Scalable dot
                        }
                    }

            }
            rx = rx+fct_y;
                        if (f.style==2 || f.style == 4) {dsl_x= dsl_x+fct_x*1.25f;}//Italic
        }
        //---------------------------------------------------------
}

void drawStringF(char * text, float x, float y, Font f, fRGB xColor) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
        //fontInclination[] = {"Angle", "  0"," 45", " 90", "135", "180", "225", "270", "315"};
        float fct_x = (float) (2.0f/VIDEO_W * f.size/12.0f);
        float fct_y = (float) (2.0f/VIDEO_H * f.size/12.0f);

        int i = 0;  char ch = 0;
        for (i = 0; i <strlen(text); i++ )
        {
            ch = text[i];
            drawCharF(ch, x, y, fct_x, fct_y*2, f, xColor);
            if (f.angle==1) {x = x + 8*fct_x;}//0dg
            if (f.angle==5) {x = x - 8*fct_x;}//180dg

            //Font Inclination
            if (f.angle==2) {x = x + 8*fct_x; y = y + 8*fct_y*1;}//45dg
            if (f.angle==8) {x = x + 8*fct_x; y = y - 8*fct_y*1;}//315dg

            if (f.angle==3) {y = y + 8*fct_y*2;}//90dg
            if (f.angle==7) {y = y - 8*fct_y*2;}//270dg

            if (f.angle==4) {x = x - 8*fct_x; y = y + 8*fct_y*1;}//135dg
            if (f.angle==6) {x = x - 8*fct_x;y = y - 8*fct_y*1;}//270dg
        }

}
void drawTextX(char * text, float x, float y) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
        float scl_w = (float) 2/VIDEO_W;
        float scl_h = (float) 2/VIDEO_H;

        int i = 0;  char ch = 0;
        for (i = 0; i <strlen(text); i++ )
        {
            ch = text[i];
            drawChar(ch, x, y, scl_w, scl_h);
            x = x + 8*scl_w;
        }
}
void drawString(char * text, float x, float y, int sz_x) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
        float fct = (float) (2.0f/VIDEO_H * sz_x/100.0f);
        int i = 0;  char ch = 0;
        for (i = 0; i <strlen(text); i++ )
        {
            ch = text[i];
            drawChar(ch, x, y, fct, fct*2);
            x = x + 8*fct;
        }
}
void printRow(char * text, int fixCol, int maxCol) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
        float scl_x = (float) (2.0f/VIDEO_W);
        float scl_y = (float) (2.0f/VIDEO_H);

        float x, y;

        int i = 0;  char ch = 0;
        for (i = 0; i <strlen(text); i++ )
        {
            x = -1.0f + ((float)pos_x/TXT_COLS*2);
            y = 1.0f  -  ((float)pos_y/TXT_ROWS*2);
            ch = text[i];
            if (ch==0) {break;}

            if (ch==10 || ch==13) {
                pos_x = fixCol-1; pos_y++;
            } else {
                drawChar(ch, x, y, scl_x, scl_y*1.5f);
            }
            //----- Incrementing the Text Position on the Screen -----
            pos_x++;if (pos_x>=maxCol) {pos_x = fixCol;pos_y++;}

            if (pos_y>=25) {pos_y = 0;}
            //----------------------------------------------------------
        }
}

void printText(char *string) {
    //Prints Text
    printRow(string, 0, 80);
}
void printTextNL(char *string) {
    //Prints Text and adds a NewLine at the end
    printRow(string, 0, 80);
    //New Line
    pos_y++; pos_x = 0;
    if (pos_y>=25) {pos_y = 0;}
}

void printTextRC(int row, int col, char *string) {
    pos_x = col; pos_y = row;
    printRow(string, col, 80);
}
void printTextRCmax(int row, int col, int maxCol, char *string) {
    pos_x = col; pos_y = row;
    printRow(string, col, maxCol);
}

//****************** Load BitMap File *******************************
void loadFile() {
    char cFile[]= "C:\\TEMP\\FONT4.BMP" ; //245 x 900
    drawCharCode();

    int h = 245;
    int w = 900;

    int i, ii, j, jj;
    char ch;

    FILE *file2 = fopen(cFile, "r"); //Open File


    for (i=0; i<iHeader;i++) {//Read the Header
        fscanf(file2, "%c", &ch);
    }

    int ctx = 0;
    for (i=0;i<(h*w);i++) {
            fscanf(file2, "%c", &ch);
            xData[ctx] = ch; ctx++;
    }

    fclose(file2);//Close File

    shape = 14; //ShowBitmap
}
//****************************************************************

//========================== PAINT PROJECT ==================================
//=============== Essential Translations  and Variables =====================

float getX(int x) {
    float r = -1.0f + (((float) x/ VIDEO_W)*2.0f);
    return r;
}
float getY(int y) {
    float r = 1.0f - (((float) y/ VIDEO_H)*2.0f);
    return r;
}

float txtX (int row) {
    float r = -1.0f + ((float)row/TXT_COLS*2);
    return r;
}
float txtY (int col) {
    float r = 1.0f  -  ((float)col/TXT_ROWS*2);
    return r;
}
float getPixH(float px) {
    float r = (px/VIDEO_W)*2.0f;
    return r;
}
float getPixV(float px) {
    float r = (px/ VIDEO_H)*2.0f;
    return r;
}

fRGB setfRGB(float r, float g, float b) {
    fRGB t;
    t.R = r;
    t.G = g;
    t.B = b;
    return t;
}
void setColorRGB(fRGB color) {
    glColor3f(color.R, color.G, color.B);
}
fPoint setfPoint(float x, float y) {
    fPoint t;
    t.getX = x;
    t.getY = y;
    return t;
}
iPoint setiPoint(int x, int y) {
    iPoint t;
    t.getX = x;
    t.getY = y;
    return t;
}
//===========================================================================
void mousePointer() {
    if (mouseX <=0) mouseX = 0;
    if (mouseX >=VIDEO_W) mouseX = VIDEO_W;
    if (mouseY <=0) mouseY = 0;
    if (mouseY >=VIDEO_H) mouseY = VIDEO_H;

    float x0 = getX(mouseX);
    float y0 = getY(mouseY);
    float dh = getPixH(6.0f);
    float dv = getPixV(4.0f);

    float x1 = x0+dh;
    float y1 = y0+dv;
    float x2 = x0-dh;
    float y2 = y0-dv;

    glColor3f(0.0f, 0.0f, 1.0f); //blue;
    drawLine(x0, y0+dh, x0, y0-dh);
    drawLine(x0+dv, y0, x0-dv, y0);
    drawRectangle(x1, y1, x2, y2);
}
void updateStatus() {
    printTextRC(24,3, "Shape: ");

    if (flagCopy==1 || flagCut==1 || flagPaste==1 || flagDelete==1) {
        if (flagCopy==1) printText("Copy Shape");
        if (flagCut==1) printText("Move Shape");
        if (flagPaste==1) printText("Paste Shape");
        if (flagDelete==1) printText("Delete Shape");
    } else {
        printText(ddShapes[menuSelected[4]]);
    }

    if (selectedShape==-1) {
        printText(",[Selected Item: Nothing]");
    } else {
        printText(",[Selected Item: ");
        switch (compList[selectedShape].shape) {
            case 10: printText(",Disk - ");break;
            case 11: printText(",File - ");break;
            case 12: printText(",Link - ");break;
            case 13: printText(",Folder - ");break;
            case 14: printText(",Window - ");break;
            default: break;
        }
        printText(compList[selectedShape].txt);
        printText("] ");
    }

    char str1[15];
    sprintf(str1, "%i", mouseX);
    char str2[15];
    sprintf(str2, "%i", mouseY);

    printText (", P(");
    printText (str1);
    printText (",");
    printText (str2);
    printText (")");

    printText(" Sel[");
    char str3[15];
    sprintf(str3, "%i", shape);
    printText (str3);
    printText ("]");

    //printTextRC(24,3, "Press: F1 - Menu, Down + Space To Select, F2 - Mouse, +/- Other Screens");
}

//===========================================================================
void clearScreen() {
    int i = 0;
    compListsize = 0;
    shapeCtr = 0; ctrText = 0; ctrTxtFile=0; ctrFiles=0; ctrLink = 0;
}
//============================[Message Box and Input Box]===============================================

void drawBox(float x1, float y1, float x2, float y2, float cl1, float cl2, float bd) {
    float clx;

    clx = cl2;
    glColor3f(clx,clx, clx);//Color to Draw;
    fillRectangle(x1, y1, x2, y2);

    clx = cl1;
    glLineWidth(bd);
    glColor3f(clx,clx, clx);//Color to Draw;
    drawRectangle(x1,y1, x2, y2);//Border

}
void msgBox() {
    pos_x = 20;pos_y = 7;

    char txt[65] = {0};
    char ch;
    int i = 0, rw =1, cl = 0, w = 0, h =0;
    //Get number of rows and columns
    int len = strlen(msg);
    rw = 1;
    for (i=0; i<len; i++) {
       ch = msg[i];
       if (ch==13 || ch == 10 || cl >=60) {
            rw++; cl = 0;
       }
       cl++; if (cl>w) w = cl;//max
    }


    //Print the Box
    float x1 = txtX(19);
    float x2 = txtX(20+w);
    float y1 = txtY(6)-getPixH(2.0);
    float y2 = txtY(9+rw+1) -getPixH(2.0);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 0.5f);

    //Print Title
    bColor = setfRGB(0.82f, 0.82f, 0.82f);

    int mid = w/2;
    printTextRC(7, 20+w/2-strlen(title)/2, title);
    printTextRCmax(9, 20, 70, msg);
    //Print Close
    printTextRC(9+rw+1, 20+w/2-5,"[Close F4]" );

    bColor = setfRGB(1.0f, 1.0f, 1.0f);
    //Border


}

void inputBox() {
    pos_x = 20;pos_y = 7;
    float x1, x2, y1, y2;

    char txt[65] = {0};
    char ch;
    int i = 0, rw =1, cl = 0, w = 0, h =0;
    //Get number of rows and columns
    int len = strlen(msg);
    rw = 1;
    for (i=0; i<len; i++) {
       ch = msg[i];
       if (ch==13 || ch == 10 || cl >=60) {
            rw++; cl = 0;
       }
       cl++; if (cl>w) w = cl;//max
    }

    //--------------------------------------
    x1 = txtX(19);  x2 = txtX(20+w+1); y1 = txtY(6)-getPixH(2.0);
    y2 = txtY(9+rw+3) -getPixH(2.0);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 0.5f);
    //----------------------------------
    x1 = txtX(20);   x2 = txtX(20+w-1);
    y1 = txtY(9+rw)-getPixH(2.0); y2 = txtY(9+rw+1) -getPixH(2.0);
    drawBox(x1, y1, x2, y2, 0.0f, 1.00f, 0.2f);
    //----------------------------------

    //Print Title
    bColor = setfRGB(0.82f, 0.82f, 0.82f);

    int mid = w/2;
    printTextRC(7, 20+w/2-strlen(title)/2, title);
    printTextRCmax(9, 20, 70, msg);
    //Print Close

    printTextRC(9+rw+3, 20+w/2-5,"[ENTER]" );
    bColor = setfRGB(1.0f, 1.0f, 1.0f);
    //Border
    //Input Box
    printTextRC(9+rw+1, 22,input);

}

void getInput(unsigned char key) {
    if (inpBoxCTR>=50) return;
    if (key==27) {iBox = 0; flagInputBox = 0;}
    if (key==13 || key == 10) {CloseBox();}
    if (key==VK_SHIFT) {if (flagCap==1) {flagCap=0;} else {flagCap=1;}}
    if (key==VK_BACK) {input[inpBoxCTR]=0; inpBoxCTR--; if (inpBoxCTR<0) inpBoxCTR=0;};

    if (key==VK_DECIMAL) key = '.';
    if (key==VK_ADD) key = '+';
    if (key==VK_SUBTRACT) key = '-';
    if (key==VK_MULTIPLY) key = '*';
    if (key==VK_DIVIDE) key = '/';

    if (key >= 32 && key <= 127) {
        if (flagCap==0) {
            if (key>='A' && key<= 'Z') key = key+32;
        }
        input[inpBoxCTR]=key;
        inpBoxCTR++;
        input[inpBoxCTR]=0;
    }

}
void about() {
 char *t = "About";
 char *m = "Basic Windows Manager Application \nMade as Part of a Training Program \nBy P. Ramos - Aug/2016";
 showMsgBox(t, m);
}

void help() {
 char *t = "Help";
 char *m = " Basic Windows Manager Application"
            "\n [F1] - Access the Menus"
            "\n [F2] - Cursor Mover (mouse)"
            "\n [F3] - Clear the Screen"
            "\n [F4] - Close DialogBoxes"
            "\n [F5] - Open Link"
            "\n Menu Navigation: [F1] [up/down] [left/right]"
            "\n Menu Selection: with '>>' pointed press [SPACE]"
            "\n *** Made by Paulo Ramos @2016 ***"
            ;
showMsgBox(t, m);
}

void showMsgBox(char *t, char *m) {
 strcpy(title, t);
 strcpy(msg, m);
 flagInputBox=0;
 flagMsgBox = 1;
}

void showInputBox(char *t, char *m) {
 strcpy(title, t);
 strcpy(msg, m);
 inpBoxCTR = 0;
 input[0]=0; input[1]=0;
 flagMsgBox = 0;
 flagInputBox=1;
}
void setText() {
 char *t = "Set Text";
 char *m = "Enter the Text To be Displayed ";
  iBox = 101; //Set Text Code
 showInputBox(t, m);
}
//================== Change Font and Change Color =================================
void changeColor() {
    int c1, c2, c3;
    int c=0, r=0;

    printTextRC(6, 20,  "[       Select a Color           ]");
    printTextRC(18, 20, "[Selected Color]        [Close F4]");

    float x = 160;
    float y = 120;
    float dsl_x = getX(10);
    float dsl_y = getY(8);
    float x1, y1, x2, y2;
    int ctx =0;

    fRGB xColor;
    for (c1=0; c1<=3; c1++) {
        for (c2=0; c2<=3; c2++) {
            for (c3=0; c3<=3; c3++) {

                x1 = getX(x); y1 = getY(y);
                x2 = getX(x+25); y2 = getY(y+20);


                if (ctx == selectedColor) {
                    dColor = setfRGB((float)c1/3,(float)c2/3,(float) c3/3);
                    glColor3f(1.0f - (float)c1/3,1.0f -(float)c2/3,1.0f -(float) c3/3);
                    drawLine(x1, y1, x2, y2);
                    drawLine(x1, y2, x2, y1);
                }
                ctx ++;

                glColor3f((float)c1/3,(float)c2/3,(float) c3/3);
                fillRectangle(x1, y1, x2, y2);

                glLineWidth(2.0f);
                glColor3f(1.0f - (float)c1/3,1.0f -(float)c2/3,1.0f -(float) c3/3);
                drawRectangle(x1, y1, x2, y2);

                x = x+35;
                c++;
                if (c>=8) {
                    x = 160; y = y +25;
                    c = 0;
                }

            }
        }
    }

                x=160; y = 350;
                x1 = getX(x); y1 = getY(y);
                x2 = getX(x+130); y2 = getY(y+20);

                setColorRGB (dColor);
                fillRectangle(x1, y1, x2, y2);

                glColor3f(0.0f, 0.0f, 0.0f);
                drawRectangle(x1, y1, x2, y2);


                x=145; y = 95;
                x1 = getX(x); y1 = getY(y);
                x2 = getX(x+300); y2 = getY(y+285);

                glColor3f(0.0f, 0.0f, 0.0f);
                drawRectangle(x1, y1, x2, y2);
}
void msgBoxKeys(unsigned char key) {
    if (key==VK_F4 || key==13 || key==VK_SPACE) CloseBox();
}
void changeColorKeys(unsigned char key) {
    if (key==VK_F4  || key==13 || key==VK_SPACE) CloseBox();
    if (key==VK_RIGHT) selectedColor++;
    if (key==VK_LEFT) selectedColor--;
    if (key==VK_UP) selectedColor = selectedColor-8;
    if (key==VK_DOWN) selectedColor = selectedColor+8;
    if (selectedColor <0) selectedColor = 0;
    if (selectedColor >64) selectedColor = 64;
}

void drawFontListBox(int idx, int row, int col, int w) {
    float x1, y1, x2, y2;
    int i, lf, r, dsl, dslx;
    //----------------------------------------------------
    lf = lf_font[idx];
    r = row;
    dslx = 0;
    if (selectedFont[idx]> 6) dslx = selectedFont[idx]-6;
    for (i=0; i<lf; i++) {
        if (i==0) {bColor= setfRGB(1.0f, 1.0f, 1.0f);} else {bColor= setfRGB(0.82f, 0.82f, 0.82f);}
        pos_y = r; pos_x = col;
        if (i==0 && selectFontCTR==idx) printText(">>");
        if (i>0) {
                if ((i+dsl)==selectedFont[idx]) {
                    printText("(*)");
                }else {
                    printText("( )");
                }
        }

        dsl = dslx;
        if (i==0) dsl = 0;
        if (idx==0) printText(fontFamily[i+dsl]);
        if (idx==1) printText(fontStyle[i+dsl]);
        if (idx==2) printText(fontInclination[i+dsl]);
        if (idx==3) printText(fontSize[i+dsl]);

        r++;
        if (i>=6) break;
    }
    if (lf>6) {//Scrolling

        x1 = txtX(col+w-1); x2 = txtX(col+w); y1 = txtY(row); y2 = txtY(row+6);
        float px = (y2-y1)/lf, rp = ((float)selectedFont[idx]/lf)*(y2-y1);
        x1 = txtX(col+w-1); x2 = txtX(col+w); y1 = txtY(row)+rp; y2 = txtY(row)+rp+px;
        drawBox(x1, y1, x2, y2, 0.0f, 0.0f, 2.0f);

        x1 = txtX(col+w-1); x2 = txtX(col+w); y1 = txtY(row); y2 = txtY(row+6);
        drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);
    }
    x1 = txtX(col); x2 = txtX(col+w); y1 = txtY(5); y2 = txtY(5+6);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);
    //----------------------------------------------------

}
Font setFont(int family, int style, int angle, int size) {
    Font t;
    t.family = family;
    t.style = style;
    t.angle = angle;
    t.size = size;
    return t;
}
void changeFont() {
    int i, j, lf, r, c, st_r=6;
    float x1, y1, x2, y2;

    bColor = setfRGB(0.82f, 0.82f, 0.82f);

    printTextRC(4, 20,  "             Select the Font           ");
    x1 = txtX(20); x2 = txtX(67); y1 = txtY(3); y2 = txtY(4);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);

    printTextRC(18, 20, "[Selected Font]            [Close F4]");
    x1 = txtX(20); x2 = txtX(67); y1 = txtY(17); y2 = txtY(18);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);
    //-------------Draw Font List Box ---------------------
    drawFontListBox(0, 5, 20, 14);//Font Family
    drawFontListBox(1, 5, 35, 14);//Font Style
    drawFontListBox(2, 5, 50, 8);//Font
    drawFontListBox(3, 5, 59, 8);//Font Size
    //----------------------------------------------------
    bColor = setfRGB(1.0f, 1.0f, 1.0f);
    //-------------------------------------------
    int ff, ft, fa, fs;
    ff = selectedFont[0];//Font Family
    ft = selectedFont[1];//Font Style
    fa = selectedFont[2];//Font Angle
    fs = atoi(fontSize[selectedFont[3]]);//Font Size
    Font fx = setFont(ff, ft, fa, fs);
    x1 = txtX(22);   y1 = txtY(15);
    //drawString("Font Preview", x1, y1, 100);
    drawStringF("Font Preview", x1, y1, fx, fColor);
    dFont = fx;
    //-------------------------------------------
    printTextRC(12, 20, "Preview:");
    x1 = txtX(20); x2 = txtX(67); y1 = txtY(12); y2 = txtY(17);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);
    //-----------------------------------------------
    x1 = txtX(19); x2 = txtX(68); y1 = txtY(2); y2 = txtY(19);
    drawBox(x1, y1, x2, y2, 0.0f, 1.0f, 2.0f);
}

void changeFontKeys(unsigned char key) {
    //int selectedFont[4] = {1, 1, 1, 1};
    //int selectFontCTR = 0;
    if (key==VK_F4  || key==13 || key==VK_SPACE) CloseBox();
    if (key==VK_RIGHT) selectFontCTR++;
    if (key==VK_LEFT) selectFontCTR--;
    if (key==VK_UP) selectedFont[selectFontCTR]--;
    if (key==VK_DOWN) selectedFont[selectFontCTR]++;

    if (selectFontCTR <0) selectedColor = 2;
    if (selectFontCTR >2) selectedColor = 0;
    if (selectedFont[selectFontCTR]<=1) selectedFont[selectFontCTR] = 1;
    if (selectedFont[selectFontCTR]>= lf_font[selectFontCTR]) selectedFont[selectFontCTR] = lf_font[selectFontCTR]-1;//Family
}
//===================================== Open and Save File Dialogs ================================
void saveFileDialog() {
    if (strlen(currFile)>0) {
        char cFile[100];
        strcpy(cFile, currPath);
        strcat(cFile, currFile);
        saveFile(cFile);

    } else {
        saveFileAsDialog();
    }
}
void saveFileAsDialog() {
    printf("\n Inform the new name for the file: %s ", currFile);

    char m [300];
    char *t = "Save File As";
    char *m1 = "Press. Enter to Open the File [";
    char *m2 = "]\nor Inform the name of the new File to be saved:";

    strcpy(m, m1);
    strcat(m, currFile);
    strcat(m, m2);

    iBox = 201;
    showInputBox(t, m);
}
void openFileDialog() {
    char m [300];
    char *t = "Open File";
    char *m1 = "Press. Enter to Open The File [";
    char *m2 = "]\nor Inform the name of the new File to be opened:";

    strcpy(m, m1);
    strcat(m, currFile);
    strcat(m, m2);

    iBox = 202;
    showInputBox(t, m);
}

void CloseBox() {
    flagInputBox = 0 ;
    flagMsgBox = 0;
    flagColorPicker = 0;
    flagFontPicker = 0;
    flagTest = 0;
    if (iBox==101) {if (strlen(input)>0) {strcpy(dText, input);}}//setText;
    if (iBox==201) {//Save File as
        if (strlen(input)>0) {
                strcpy(currFile, input);
        }
        if (strlen(currFile)>0) {
            char cFile[100];
            strcpy(cFile, currPath);
            strcat(cFile, currFile);
            saveFile(cFile);
        }
    }
    if (iBox==202) {//OpenFile
        if (strlen(input)>0) {
                strcpy(currFile, input);
        }
        char cFile[100];
        strcpy(cFile, currPath);
        strcat(cFile, currFile);
        if (strlen(currFile)>0) {
            openFile(cFile);
        }
    }
    if (iBox==301) {
        if (strlen(input)>0) {
                char tText[100] = {0};
                char tText2[100] = {0};

                int type = 0, i=0, ln=0;
                strcpy(tText, input);
                ln = strlen(tText);
                for (i=2; i<ln;i++) {tText2[i-2] = tText[i];}
                type = tText[0]-48;
                execAddLink(type, tText2);
            }

    }
    iBox = 0;
}

//===========================================================================
//==========================[Mouse Emulation]================================
void mouseClick() {
    addShapeX(mouseX, mouseY, 0);//Testing
}
void mouseMove() {
    showShapeX(mouseX, mouseY);
}
void mouseDown() {//Shift Down
}
void mouseUp() {//Shift Up
}
//=================================================================================
//============================ AutoCad Routines ===================================
//=================================================================================
void intDrawCircle(int x, int y, float rx, float ry, int num_segments)
{
    float cx, cy;
    cx = getX(x)+rx; cy = getY(y)-(ry);
    glBegin(GL_LINE_LOOP);
    int ii = 0;
    float cyx = getPixV(1.0f)/getPixH(1.0f);
    for(ii = 0; ii < num_segments; ii++)
    {
        float th = 2.0f * 3.1415926f * (float) ii / (float) num_segments;//get the current angle
        float x = rx * cosf(th);//calculate the x component
        float y = (ry * sinf(th));//calculate the y component
        glVertex2f(x + cx, y + cy);//output vertex
    }
    glEnd();
}

void intDrawArc(int x, int y, float rx, float ry, int num_segments, int a1, int a2)
{
    //a1 - Starting Angle, a2 = Total Angle

    float cx, cy;
    float fx1, fx2, fy1, fy2;
    cx = getX(x)+rx; cy = getY(y)-(ry);
    int ii = 0;
    fx1 = 0.0f; fy1 = 0.0f;
    for(ii = a1; ii < a1+a2; ii++)
    {
        float th = 2.0f * 3.1415926f * (float) ii / (float) num_segments;//get the current angle
        float x = rx * cosf(th);//calculate the x component
        float y = (ry * sinf(th));//*cyx;//calculate the y component
        fx2 = x+cx;  fy2 = y + cy;
        if (fx1!=0.0f && fy2!=0.0f) {
            glBegin(GL_LINE_LOOP);
                glVertex2f(fx1, fy1);//output vertex
                glVertex2f(fx2, fy2);//output vertex
            glEnd();
        }
        fx1 = fx2; fy1 = fy2;
    }
}

void intDrawLine(int x1, int y1, int x2, int y2)
{
    float fx1, fy1, fx2, fy2;
    fx1 = getX(x1); fy1 = getY(y1);
    fx2 = getX(x2); fy2 = getY(y2);
    glBegin(GL_LINE_LOOP);
        glVertex2f(fx1, fy1);//output vertex
        glVertex2f(fx2, fy2);//output vertex
    glEnd();
}
void intDrawRectangle(int x1, int y1, int x2, int y2)
{
    float fx1, fy1, fx2, fy2;
    fx1 = getX(x1); fy1 = getY(y1);
    fx2 = getX(x2); fy2 = getY(y2);
    drawLine(fx1, fy1, fx1, fy2);
    drawLine(fx1, fy2, fx2, fy2);
    drawLine(fx2, fy2, fx2, fy1);
    drawLine(fx2, fy1, fx1, fy1);
}

char *string_replace(char *str, char rpl, char by) {
    // Replace Space(' ') by Nothing('') and Splits the String according to the informed Splitter (spt);
    //------------ String Replace ' ' by '' and Split ';' -------------------
	//String [] strx = str.replace(" ", "").split(";");
    int i, i2, len, cti;

    char tmp1[256] = {0};
    len = strlen(str); i2 = 0;
    for (i=0;i <len; i++) {
        if (by==0){
            if (str[i]!=rpl) {tmp1[i2]=str[i]; i2++;}
        } else {
            if (str[i]==rpl) {
                tmp1[i]=by;
            } else {
                tmp1[i]=str[i];
            }
        }
    }; tmp1[len]=0;//Replace ' ' by '';
    return tmp1;
    //-------------------------------------------------------------------------
}
void string_split(char *str, char spt) {
    // Replace Space(' ') by Nothing('') and Splits the String according to the informed Splitter (spt);
    //------------ String Replace ' ' by '' and Split ';' -------------------
	//String [] strx = str.replace(" ", "").split(";");
    int i, i2, len, cti;

    char tmp2[256] = {0};
    //---- Resetting Global Variable strx[256][10] -----
    for (i=0;i<256;i++) {
        for (i2=0;i2<10;i2++) {
            strx[i][i2]=0;
        }
    }
    //--------------------------------------------------

    len = strlen(str); i2 = 0; cti = 0;
    for (i=0;i <len; i++) {
            if (str[i]==spt) {
                tmp2[i2]=0;
                strcpy(strx[cti], tmp2);
                cti++;
                i2=0;tmp2[0]=0;
            } else {
                tmp2[i2] = str[i];i2++;
            }
    };
    //-------------------------------------------------------------------------
}
void string_repl_split(char *str, char spt) {
    char *tmpx1 = string_replace(str, ' ', 0);
    char tmp1[512] = {0};
    strcpy(tmp1, tmpx1);

    string_split(tmp1, ';');
}
void drawComponentX(char *str, int x, int y, int rot) {

	int dx1, dy1, dx2, dy2, x1, y1, x2, y2, n, ctr, i, a1, a2;
	iPoint px1, px2;
    float rx, ry;
	char sp[10] = "L";

	//g.setColor(dColor);//Pen and SolidBrush Color
	//g.setStroke(new BasicStroke(penSize));iPoint
    setColorRGB(dColor);
    glLineWidth(pen);

	//String [] strx = str.replace(" ", "").split(";");
    string_repl_split(str, ';');

	n = atoi(strx[0]);
	ctr = 1;

	for (i=0; i<n;i++) {

		//---------------- Reading the String -------------------------------
		//sp = strx[ctr+0];
		strcpy(sp, strx[ctr+0]);
		dx1 = atoi(strx[ctr+1]); dy1 = atoi(strx[ctr+2]);
		dx2 = atoi(strx[ctr+3]); dy2 = atoi(strx[ctr+4]);
		ctr = ctr + 5;
		//---------------- Rotation -----------------------------------------
		x1 = x + dx1; y1 = y + dy1; x2 = x + dx2; y2 = y + dy2;a1 = 270; a2=90;//default
		if (rot==1) {x1 = x + dy1; y1 = y + dx1; x2 = x + dy2; y2 = y + dx2;a1=180; a2=0;}
		if (rot==2) {x1 = x - dx1; y1 = y - dy1; x2 = x - dx2; y2 = y - dy2;a1=90; a2 = 270;}
		if (rot==3) {x1 = x - dy1; y1 = y - dx1; x2 = x - dy2; y2 = y - dx2;a1=0; a2 = 180;}
		//--------------- px2 always > px1 ----------------------------------
		px2 = setiPoint(x2, y2); px1 = setiPoint(x1, y1);//Default
		if (y2>y1 && x2 > x1) {px2 = setiPoint(x2, y2); px1 = setiPoint(x1, y1);}
		if (y2>y1 && x1 > x2) {px2 = setiPoint(x1, y2); px1 = setiPoint(x2, y1);}
		if (y1>y2 && x1 > x2) {px2 = setiPoint(x1, y1); px1 = setiPoint(x2, y2);}
		if (y1>y2 && x2 > x1) {px2 = setiPoint(x2, y1); px1 = setiPoint(x1, y2);}
		if (strcmp(sp, "L")==0) {px2 = setiPoint(x2, y2); px1 = setiPoint(x1, y1);}


		//--------------- Basic Shapes ---------------------------------------
        if (strcmp(sp, "L")==0) {intDrawLine(px1.getX, px1.getY, px2.getX, px2.getY);}//Line
        if (strcmp(sp, "R")==0) {intDrawRectangle(px1.getX, px1.getY, px2.getX, px2.getY);}//Rectangle
        rx = (abs(px1.getX - px2.getX)/640.0f)/1.0f;
        ry = (abs(px1.getY - px2.getY)/480.0f)/1.0f;
        if (strcmp(sp, "C")==0) {intDrawCircle(px1.getX, px1.getY, rx, ry, 360);}//Circle
        if (strcmp(sp, "A")==0) {intDrawArc(px1.getX, px1.getY, rx, ry, 360, a1, 180);}//Circle
        if (strcmp(sp, "IA")==0) {intDrawArc(px1.getX, px1.getY, rx, ry, 360, a2, 180);}//Circle
		//--------------------------------------------------------------------

	}
}
void drawComponent(int comp, int x, int y, int rot, iPoint lpz, int flagPt, char *lText, options opt) {
	char *str; str = "";
	int dx, dy;
	if (comp==10) {str = "10; R;0;0;48;52; C;4;4;44;44; C;14;14;34;34; C;22;22;26;26; R;2;2;4;4; R;44;2;46;4; R;2;48;4;50; L;24;36;44;44;  L;24;36;44;36; L;44;36;44;44; ";}  //Disk
	if (comp==11) {str = "4; R;4;4;40;48; L;8;18;36;18;  L;8;28;36;28; L;8;38;36;38; ";}  //File
	if (comp==12) {str = "4; R;4;14;34;44; L;19;29;40;4; L;32;4;40;4;  L;40;4;40;12; ";}  //Link
	if (comp==13) {str = "3; R;4;20;44;48; R;4;12;44;20; R;4;9;20;12; ";}  //Folder
	if (comp==14) {str = "";drawWindow(x, y, lpz.getX, lpz.getY, lText, opt);}  //Window


	if (comp==3 && strlen(lText) > 0) {
            float fx1, fy1;
            fx1 = getX(x); fy1 = getY(y);
            drawStringF(lText, fx1, fy1, dFont, dColor);
	}
	if (strlen(str) > 0) {
            float fx1 = getX(x-18), fx2 = getX(x+52), fy1 = getY(y),fy2 = getY(y+70);
            drawBox(fx1, fy1, fx2, fy2, 1.0f, 1.0f, 0.2f);
            drawComponentX(str, x, y, rot);
    }
    if (comp>=10 && comp <=13) {drawLinkText(lText, x+24, y+70, 14);}

}


 void updateScreen() {
	//clearPanel();
	shapeX2 t, w;
	iPoint bklp1 = lp1;//backup
	int i =0, lx, ly;
	for (i = 0; i < compListsize;i++) {
		t = compList[i];
        if (t.flag ==1)	drawComponent(t.shape, t.x, t.y, t.rot, t.lpx, 0, t.txt, t.opt);
	}
	//MoveGraphics();
	lp1 = bklp1;//Restore
}

 void showShapeX(int x, int y) {
	int comp = ctrComp, dx, dy;
	if (flagCopy == 1 || flagDelete == 1 || flagCut == 1) return;
	if (flagPaste=1 && selectedShape != -1) {
			shapeX2 t;
			t = currShape;
			dColor = setfRGB(0.0f, 0.0f, 1.0f);//Blue
    		t.lpx = currShape.lpx;  t.txt = currShape.txt;

			drawComponent(t.shape, x, y, t.rot, t.lpx, 0, t.txt, t.opt);
			dColor = setfRGB(0.0f, 0.0f, 0.0f);//Black
	} else {
	    if (comp==0) return;
	    options opt;
	    opt.idx = 0; opt.dsl=0; opt.c = 0; opt.r=0;
		drawComponent(comp, x, y, rotation, lp1, flagP1, dText, opt);
	}
}

shapeX2 new_shapeX(int shape, int x, int y, int rot) {
    shapeX2 t;
    t.shape = shape; t.x = x, t.y = y; t.rot = rot;
    t.flag = 1;
    t.opt.idx = 0;t.opt.dsl=0;t.opt.c = 0;t.opt.r = 0; t.opt.sel=-1; t.opt.type =0;
    return t;
}
 void addShapeX(int x, int y, int wt) {
	int comp = ctrComp, dx, dy;
	if (flagPaste==1 && selectedShape != -1) {
    		shapeX2 t = new_shapeX(currShape.shape, x, y, currShape.rot);
    		t.lpx = currShape.lpx;  t.txt = currShape.txt;
    		t.opt = currShape.opt;

    		if (t.shape!=0) {
                compList[compListsize]=t;compListsize++;
    		}
            flagPaste=0;
            update_TaskList();
            pasteShape();
            setShape();
            ctrComp = 0;
            return;
		}
	if (flagSelect==1) {SelectShape(x, y);}
	if (flagCopy==1 && selectedShape != -1) {copyShape();return;}
	if (flagDelete==1 && selectedShape != -1) {delShape();return;}
	if (flagCut==1 && selectedShape != -1) {cutShape();return;}

	//----------------- Adding a New Shape -----------------------
		shapeX2 t = new_shapeX(comp, x, y, rotation);
		if ((comp>=10 && comp<=14)) {
		    if (comp==14) {t.lpx = lp1;t.opt.idx = ctrFiles;t.opt.type=wt;
                if (wt==1) t.opt.idx = ctrTxtFile;
		    }
            strcpy(listText[ctrText], dText);
            t.txt= listText[ctrText];
            ctrText++;
            compList[compListsize]=t;compListsize++;
            update_TaskList();
        }
    //-------------------------------------------------------------
}
//************************Edit Options: Copy, Cut and Paste **************************
void SelectShape(int x, int y) {
	shapeX2 t;
	int rx1, rx2, ry1, ry2, comp, rg, i;
	selectedShape = -1;
	for (i = compListsize-1; i >= 0; i--) {
		t = compList[i];
		comp = t.shape;
		if (t.flag==1) {
            t.lpx.getX = t.x + 50; t.lpx.getY = t.y + 50;
            if (t.x > t.lpx.getX) {rx2 = t.x;rx1 = t.lpx.getX;} else {rx1 = t.x;rx2 = t.lpx.getX;}
            if (t.y > t.lpx.getY) {ry2 = t.y;ry1 = t.lpx.getY;} else {ry1 = t.y;ry2 = t.lpx.getY;}
            rg = 8;
            if (x>=rx1-rg && x <=rx2+rg && y >= ry1-rg && y <= ry2+rg) {selectedShape = i;currShape = compList[i]; break;}
		}
	}
	showSelectedShape();
	if (selectedShape != -1) flagSelect = 0;
}
void showSelectedShape() {
	if (selectedShape != -1) {
			shapeX2 t;
			t = compList[selectedShape];
			dColor = setfRGB(0.0f, 0.0f, 1.0f);
			drawComponent(t.shape, t.x, t.y, t.rot, t.lpx, 0, t.txt, t.opt);
			dColor = setfRGB(0.0f, 0.0f, 0.0f);
	}
}
void copyShape() {
	if (selectedShape != -1) {
		currShape = compList[selectedShape];
		updateScreen();
		setEdit("Paste");
	}
}
void cutShape() {
	if (selectedShape != -1) {
		currShape = compList[selectedShape];
		compList[selectedShape].flag=0;
		updateScreen();
		setEdit("Paste");
	}
}
void moveShape() {
	if (selectedShape != -1) {
		currShape = compList[selectedShape];
		compList[selectedShape].flag=0;
		updateScreen();
		setEdit("Paste");
	}
}
void delShape() {
	if (selectedShape != -1) {
		compList[selectedShape].flag=0;
		updateScreen();
		setEdit("Delete");
	}
}
void pasteShape() {
	setShape();
	selectedShape=-1;
	setEdit("");
	flagMove=1; flagCut=1; flagPaste=0;
}
 void setEdit(char *opt) {
    shape=16;//Mouse move
	flagPaste = 0; flagDelete=0; flagCut = 0; flagCopy = 0; flagSelect = 0, flagMove = 0;

	if (strcmp(opt,"Cut")==0) {
        flagCut = 1; flagSelect = 1; selectedShape = -1;
	}
	if (strcmp(opt,"Move")==0) {
        flagCut = 1;flagSelect = 1; selectedShape = -1;
	}
	if (strcmp(opt,"Copy")==0) {
        flagCopy = 1; flagSelect = 1; selectedShape = -1;
	}
	if (strcmp(opt,"Paste")==0 && selectedShape != -1) {
        flagPaste = 1; flagSelect = 0;
	}
	if (strcmp(opt,"Delete")==0) {
        flagDelete = 1; flagSelect = 1;
	}
    flagP1 = 1;
	//l1.setText("Selected Shape: " + selShape);
	updateStatus();

}
//************************** Zoom IN and Zoom OUT *****************************************
 void showZoom() {
}
 void zoomIN() {
    flagZoom = 1;
	zoom += 0.25f;
	glScalef(zoom, zoom, 1);
	zoomTrack= zoomTrack * zoom;
	updateScreen();

}
 void zoomOut() {
	flagZoom = 0;
	glScalef(1/zoomTrack, 1/zoomTrack, 1);
    updateScreen();
	zoom = 1.0f;
	zoomTrack = 1.0f;
}
void keyZoom() {

}
//==========================[SAVE FILE / OPEN FILE] =========================

void openFile(char *cFile) {
    char tmpf[200];
    clearScreen();
    //****************** Opening the File ******************
    int r, c, a, b, ch;
    float f;
    if (strlen(cFile) <=0) {
         char *t2 = "Error Saving File" ;
         char *m = "Inform a valid name to the File!";
         showMsgBox(t2, m);
    }


    FILE *file2 = fopen(cFile, "r"); //

    fscanf(file2,"%[^\n]%*c",&tmpf);
    shapeCtr = 0;ctrText = 0;

    int flag, shape, x, y, w, h;


    while (tmpf!=EOF) {
        //printf("%s\n", tmpf);//testing
        if (strcmp(tmpf,"Data:")==0) {
            shapeX2 t = new_shapeX(0,0,0,0);
            t.flag = 1;
            fscanf(file2, " %i %i %i %i %i %i \n", &t.shape, &t.x, &t.y, &t.rot, &t.lpx.getX, &t.lpx.getY);
            compList[compListsize] = t; compListsize++;
        }
        if (strcmp(tmpf,"Text:")==0) {
            fscanf(file2,"%[^\n]%*c",&tmpf);
            strcpy(listText[ctrText], tmpf);
            compList[compListsize-1].txt = listText[ctrText];
            ctrText ++;
        }
        if (strcmp(tmpf,"End:")==0) {
                char *t2 = "Opening File" ;
                char *m = "File Opened Successfully!";
                showMsgBox(t2, m);
                break;
        }
        fscanf(file2,"%[^\n]%*c",&tmpf);
    }
    fclose(file2);

}
void saveFile(char *cFile) {
    if (strlen(cFile) <=0) {
         char *t = "Error Saving File" ;
         char *m = "Inform a valid name to the File!";
         showMsgBox(t, m);
         return;
    }

    FILE *file1 = fopen(cFile, "w"); //
    int i, j, r, c;

    //------------- Print Data to the File --------------------
        fprintf(file1, "AutoCad-ProjectFile:\n");
		shapeX2 t;
		for (i = 0; i < compListsize;i++) {
			t = compList[i];
			if (t.flag==1) {
                //file1.write("Data:\r\n "+ t.shape + " " +  t.x + " " +  t.y + " " +  t.rot + " " +  t.lpx.X + " " +  t.lpx.Y + " \r\n" );
                fprintf(file1, "Data:\n");
                fprintf(file1, " %i %i %i %i %i %i \n", t.shape, t.x, t.y, t.rot, t.lpx.getX, t.lpx.getY);
                if (t.shape==3 && strlen(t.txt)>0) {
                    //file1.write("Text:\r\n %s\r\r", t.txt);
                    fprintf(file1,"Text:\n");
                    char lTXT[100] = {0};
                    strcpy(lTXT, t.txt);
                    fprintf(file1,"%s\n", lTXT);
                }
			}
		}
    fprintf(file1, "End:\n");
    //----------------------------------------------------------
         char *t2 = "Saving File" ;
         char *m = "File Saved Successfully!";
         showMsgBox(t2, m);
    fclose(file1);
}
//=================================================================================
//===================== Windows Manager Application Routines ======================
//=================================================================================
//========================== [ Menus ] ============================================
void printMenus() {
    //ExpandMenu
    if (exMenu>=8) {exMenu=0;}
    if (exMenu<0) {exMenu=7;}
    int lf = 0, i = 0, ix = 0, iy = 0;

    //PrintMenus
    printTextRC(1,3, "Start");
    printTextRC(1,12, "Link");
    printTextRC(1,22, "Window");

    //Expand Menus
    switch (exMenu) {
        case 1://menuStart
             lf = sizeof(menuStart)/sizeof(menuStart[0]);
             ix = 3;pos_y=1;pos_x= ix; for (i=0; i<lf; i++) {drawRectMenus(pos_y,pos_x,14,0.82f, i);printTextNL(menuStart[i]);pos_x = ix;}
            break;
        case 2://menuLink
             lf = sizeof(menuLink)/sizeof(menuLink[0]);
             ix = 12;pos_y=1;pos_x= ix; for (i=0; i<lf; i++) {drawRectMenus(pos_y,pos_x,15,0.82f, i);printTextNL(menuLink[i]);pos_x = ix;}
            break;
        case 3://menuWindow
             lf = sizeof(menuWindow)/sizeof(menuWindow[0]);
             ix = 22;pos_y=1;pos_x= ix; for (i=0; i<lf; i++) {drawRectMenus(pos_y,pos_x,16,0.82f, i);printTextNL(menuWindow[i]);pos_x = ix;}
            break;
        default:
            exMenu = 0;
            break;
    }
}

void drawRectMenus(int y, int x, int w, float cl, int ct) {
    if (ct ==0) return;
    if (menuSel==ct) {printTextRC(y, x, ">>"); w = w +2;}
    float x1 = txtX(x);
    float x2 = txtX(x+w);

    float y1 = txtY(y-1)-getPixH(2.0);
    float y2 = txtY(y) -getPixH(2.0);

    cl = 1.0f;
    glColor3f(cl,cl, cl);//Color to Draw;
    glLineWidth(0.5f);
    //fillRectangle(x1, y1, x2, y2);
    drawBox(x1, y1, x2, y2, 0.82f, 1.0f, 0.2f);
    cl = 0.0f;
    glColor3f(cl,cl, cl);//Color to Draw;
    drawRectangle(x1,y1, x2, y2);//Border
}
void selectMenuItem() {
    menuSelected[exMenu] = menuSel;
    exMenu = 0;//Exit Menu
    setShape();
    //-------------- Menu Start -----------------------
    //char *menuStart[]= {"Start", "Add Link", "Remove All", "Task Manager", "Help", "About", "Close Windows", "Exit"};

    if (menuSelected[1]==1) {addLink();} //Add Link;
    if (menuSelected[1]==2) {clearScreen();} //Remove Links;
    if (menuSelected[1]==3) {task_list(100,100);} //Task Manager;
    if (menuSelected[1]==4) {help();} //Help;
    if (menuSelected[1]==5) {about();} //About;
    if (menuSelected[1]==6) {closeWindows();} //Close Windows
    if (menuSelected[1]==7) {bQuit = TRUE;} //Exit;
    menuSelected[1] = 0;//Resets
    //-------------- Menu Link -----------------------
    //char *menuLink[]= {"Link", "Open(Explorer)", "Open(Text)", "----------","Move", "Copy", "Delete"};
    if (menuSelected[2]==1) {openLink();} //Open
    if (menuSelected[2]==2) {openText();} //Text Viewer
    if (menuSelected[2]==3) {} //-------
    if (menuSelected[2]==4) {setEdit("Move");} //Move
    if (menuSelected[2]==5) {setEdit("Copy");} //Copy
    if (menuSelected[2]==6) {setEdit("Delete");;} //Delete
    menuSelected[2] = 0;//Resets
    //-------------- Menu Window -----------------------
    //char *menuWindow[]= {"Window", "Close", "Move", "Select File", "Resize", "Maximize"};
    if (menuSelected[3]==1) {wind_close();} //Close
    if (menuSelected[3]==2) {wind_move();} //Move
    if (menuSelected[3]==3) {wind_sel_file();} //select file
    if (menuSelected[3]==4) {wind_resize();}//Resize
    if (menuSelected[3]==5) {wind_max();}//Maximize
    menuSelected[3] = 0;//Resets
}

//=================================================================================
void drawScreen() {
    int x1, x2, y1, y2;
    glColor3f(0.0f, 0.0f, 0.0f); //blue;
    x1 = 0; y1=0; x2 = 640; y2 = 480;
    intDrawRectangle(x1, y1, x2, y2);

    x1 = 0; y1=0; x2 = 640; y2 = 20;
    intDrawRectangle(x1, y1, x2, y2);

    x1 = 0; y1=445; x2 = 640; y2 = 480;
    intDrawRectangle(x1, y1, x2, y2);
    updateScreen();
    updateStatus();
}
//================================[ Listing Files ]================================
int isDir(char *path, char *fileN) {
  char file[256] = {0};
  DIR *ck;
  struct dirent *ck_d;
        strcpy(file, path);
        int ln = strlen(path);
        if(path[ln-1]!='/') strcat(file, "/");

        strcat(file, fileN);
        ck = opendir(file);

        if ((ck_d = readdir(ck))==NULL) {
          return 0;
        } else {
          return 1;
        }
}
void drawLinkText(char *text, int x, int y, int lmt) {
    //Draw Link Description
        if (strlen(text) > 0){
            Font lFont = setFont(1, 1, 1, 7);
            int i=0, ctx=0, ln, lnx;
            float fx1, fy1;

            char str1[20]={0};
            char str2[20]={0};
            ln = strlen(text);//Size
               for (i=0; i<ln; i++) {
                    if (i<lmt) str1[i]=text[i];
                    if (i>=lmt) str2[i-lmt]=text[i];
                    ctx++;
                    if(ctx>=ln) break;
                    if (ctx==lmt*2) {
                            strcat(str2, "...");
                            break;
                        }
               }
                lnx = strlen(str1);
                fx1 = getX(x-lnx*3); fy1 = getY(y);
                drawStringF(str1, fx1, fy1, lFont, dColor);

                lnx = strlen(str2);
                fx1 = getX(x-lnx*3); fy1 = getY(y+12);
                drawStringF(str2, fx1, fy1, lFont, dColor);
        }
}

void drawWindowLine(char *text, int x, int y, int lmt) {
        if (strlen(text) > 0){
            int lm = (int) (lmt-x)/5.5f; int ln = strlen(text);
            if (ln>lm) text[lm]=0;

            Font lFont = setFont(1, 1, 1, 8);
            float fx1, fy1;
            fx1 = getX(x); fy1 = getY(y);
            drawStringF(text, fx1, fy1, lFont, dColor);
        }
}

void drawWindow(int x1, int y1, int dx, int dy, char * title, options opt) {
    int x2, y2, i, tp, n, x, y, r, c, c1, r1, dsl, idx, r0, c0, lf;
    x2 = x1+dx; y2 = y1+dy;
    if (dx<60) x2 = x1+60; if (dy<60) y2 = y1+60;

    setColorRGB(dColor);glLineWidth(pen);
    //--------  Basic Structure of the Screen  ----------------
    float fx1 = getX(x1), fx2 = getX(x2), fy1 = getY(y1),fy2 = getY(y2);
    drawBox(fx1, fy1, fx2, fy2, 0.0f, 1.0f, 0.2f);

    intDrawRectangle(x1, y1, x2, y2);//Window
    intDrawRectangle(x1, y1, x2, y1+15);//Title
    drawWindowLine(title, x1, y1+13, x2-10);
    intDrawRectangle(x2-10, y1, x2, y2);//Scroll Right
    //intDrawRectangle(x1, y2-10, x2, y2);//Scroll Bottom
    //---------------------------------------------------------

    // List Files
    char lText[50] = {0};
    iPoint lpz = setiPoint(0,0);
    x = 20; y = 30;
    c = (int) (abs(x2 - x1)/70);
    r = (int) (abs(y2 - y1)/90);

    dsl = 0; i=0;
    idx = opt.idx;//Index inside the Array
    r0= opt.r;  c0 = opt.c;

    //dsl = (c0*r)+(r0*c);
    dsl = (r0*c);

    fRGB bk_color = dColor;
    //------------------------------------------------------------
    if (opt.type==0) {//List Files
        lf = fList[idx].num;
        y = y-90;
        for (r1=0; r1<r; r1++)  {//Rows
            x = 20; y = y +90;
            for (c1=0; c1<c; c1++) {//Columns
                if ((i+dsl)>=fList[idx].num-1) break;
                strcpy(lText, fList[idx].file[i+dsl]);
                if (fList[idx].type[i+dsl]==1) {tp = 13;} else {tp = 11;}
                dColor = setfRGB(0.0f, 0.0f, 0.0f);
                if (opt.sel==i+dsl) {dColor = setfRGB(1.0f, 0.0f, 0.0f);}//selected;
                drawComponent(tp, x1+x, y1+y, 0, lpz, 0, lText, opt);//
                i++;x = x+70;
            }
                if ((i+dsl)>=fList[idx].num-1) break;
        }
    } else if (opt.type==1) {//Text Viewer
        r = (int) ((abs(y2 - y1)-10)/20); //List of Itens per Window
        lf = fTxtList[idx].num;
        y = y -20;
        dsl = 0; i=0;
        if (opt.r>(r-1)) dsl = opt.r-(r-1);
        for (r1=0; r1<r; r1++)  {//Rows
                x = 5; y = y +20;
                if ((i+dsl)>lf) break;
                strcpy(lText, fTxtList[idx].txtFile[i+dsl]);
                dColor = setfRGB(0.0f, 0.0f, 0.0f);
                if (opt.r==i+dsl) {dColor = setfRGB(1.0f, 0.0f, 0.0f);}//selected;
                drawWindowLine(lText,x1+x, y1+y, x2-10);
                //intDrawLine(x1, y1+y+2, x2-10, y1+y+2);//Scroll Right
                i++;
        }

    } else if (opt.type==2) {//Image Viewer
    } else if (opt.type==3) {//Task Manager
        r = (int) (abs(y2 - y1)/20); //List of Itens per Window
        lf = taskList.num-1;
        y = y -20;
        dsl = 0; i=0;
        if (opt.r>r) dsl = opt.r-r;
        for (r1=0; r1<r; r1++)  {//Rows
                x = 20; y = y +20;
                if ((i+dsl)>lf) break;
                strcpy(lText, taskList.file[i+dsl]);
                dColor = setfRGB(0.0f, 0.0f, 0.0f);
                if (opt.r==i+dsl) {dColor = setfRGB(1.0f, 0.0f, 0.0f);}//selected;
                drawWindowLine(lText,x1+x, y1+y, x2-10);
                intDrawLine(x1, y1+y+2, x2-10, y1+y+2);//Scroll Right
                i++;
        }
    }
    dColor = bk_color;
    //------------------ Scrolling Marker ----------------------
    int dlt = abs(y2 - y1), sz = 20, yp = 0; float px;
    if(lf>1) {
        px = (dsl*1.0f)/(lf*1.0f)*100.0f;
        if (px>=100.0f) px = 100.0f;
        yp = (int) (px*dlt)/100+15;
        if (yp+y1>y2) yp = y2-y1-20;
        if (yp+y1 <y1) yp = 0;
    }
    intDrawRectangle(x2-10, y1+yp, x2, y1+yp+sz);//Scroll Right
    dColor = setfRGB(0.0f, 0.0f, 0.0f);
 }

void listFiles (char *path, int x, int y)
{
  int type, n;
  DIR  *d;
  struct dirent *dir;
  d = opendir(path);

  n = 0;

  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
        if (isDir(path, dir->d_name)==0) {
          fList[ctrFiles].type[n] = 0;//File
        } else {
          fList[ctrFiles].type[n] = 1;//Directory
        }
        //printf("%s\n", dir->d_name);
        strcpy(dText, dir->d_name);
        strcpy(fList[ctrFiles].file[n], dir->d_name);
        n++;
    }
    fList[ctrFiles].num = n;
    closedir(d);
  }
        ctrComp = 14;//Window
        lp1 = setiPoint(300, 300);
        strcpy(dText, path);
  		addShapeX(x, y,0);//File/Folder
  		ctrFiles++;

  }
//=================================================================================
void addLink() {
 char *t = "Add Link";
 char *m = "Type: 1-Link, 2-Disk, 3-Folder, 4-File, 5-List files\n[T] [Title/Path]:";
    iBox = 301; //Set Text Code
    showInputBox(t, m);
}
void execAddLink(int type, char *lText) {
    if (type==1) ctrComp = 12;//Link
    if (type==2) ctrComp = 10;//Disk
    if (type==3) ctrComp = 13;//Folder
    if (type==4) ctrComp = 11;//File
    if (type==5) {listFiles(lText, mouseX, mouseY);}
    if (type <=4) {
        strcpy(dText, lText);
        int r, c;
        c = (int) ctrLink/4;
        r = ctrLink - (c*4);
        addShapeX((c*90)+20, (r*90)+30, 0);
        ctrLink++;
        mouseX = (c*90)+20; mouseY = (r*90)+30;
    }
    setEdit("Move");
}

void openLink() {
    setEdit("Copy");
    char path[100] = {0};
    SelectShape(mouseX, mouseY);
    if (selectedShape!=-1) {
        strcpy(path, compList[selectedShape].txt);
        if(compList[selectedShape].shape==14) {//window
            int ln = strlen(path);
            int idx = compList[selectedShape].opt.idx;
            int sel= compList[selectedShape].opt.sel;
            if (path[ln-1]!='/') strcat(path, "/");
            strcat(path, fList[idx].file[sel]);
        }
        setEdit("");
        mouseX = wind_Pos; mouseY=wind_Pos;
        listFiles(path, wind_Pos,wind_Pos);
        setEdit("Move");
    }

}
void openText() {
    setEdit("Copy");
    char path[100] = {0};
    SelectShape(mouseX, mouseY);
    if (selectedShape!=-1) {
        strcpy(path, compList[selectedShape].txt);
        if(compList[selectedShape].shape==14) {//window
            int ln = strlen(path);
            int idx = compList[selectedShape].opt.idx;
            int sel= compList[selectedShape].opt.sel;
            if (path[ln-1]!='/') strcat(path, "/");
            strcat(path, fList[idx].file[sel]);
        }
        setEdit("");
        mouseX = 100; mouseY=100;
        window_text(path, 100, 100);
        setEdit("Move");
    }

}

void closeWindows() {
	shapeX2 t, w;
    ctrTxtFile=0; ctrFiles=0;
	int i =0, lx, ly;
	for (i = 0; i < compListsize;i++) {
		if (compList[i].shape==14) compList[i].flag=0;
	}
}
//========================[Task Manager, Text and Image Viewer]==================
void close_Task() {
        if(currShape.shape==14 && currShape.opt.type==3) {//window & task manager
            int sel = currShape.opt.r;
            int idx = taskList.type[sel];
            compList[idx].flag = 0;//Close
            update_TaskList();
        }
}
void update_TaskList() {
	shapeX2 t;
	int i =0, ix=0;
	for (i = 0; i < compListsize;i++) {
		t = compList[i];
        if (t.flag ==1)	{
            char str[5]={0};
            itoa(i,str,5);
            strcpy(taskList.file[ix], str);
            switch(t.opt.type) {
                case 0:
                    if (t.shape==10) strcat(taskList.file[ix], ":[Disk] - ");
                    if (t.shape==11) strcat(taskList.file[ix], ":[File] - ");
                    if (t.shape==12) strcat(taskList.file[ix], ":[Link] - ");
                    if (t.shape==13) strcat(taskList.file[ix], ":[Folder] - ");
                    if (t.shape==14) strcat(taskList.file[ix], ":[Files List] - ");

                    break;

                case 1: strcat(taskList.file[ix], ":[Text Viewer] - ");break;
                case 2: strcat(taskList.file[ix], ":[Image Viewer] - ");break;
                case 3: strcat(taskList.file[ix], ":[Task List] - ");break;
            }
            strcat(taskList.file[ix], t.txt);
            taskList.type[ix] = i; // Index
            ix++;
        }
	}
	taskList.num = ix;
}
void task_list(int x, int y) {
    update_TaskList();
    setEdit("");
    mouseX = x, mouseY = y;
    ctrComp=14;
    lp1 = setiPoint(300, 300);
    strcpy(dText, "Task Manager");
    addShapeX(x,y, 3);
    ctrComp=0;
    setEdit("Move");
 }
void loadTextFile(char *cFile, int max_rows) {
    char tmpf[100]= {0};
    int i=0, j=0,i2=0, ch=0, lmt=65;
    //****************** Opening the File ******************
    if (strlen(cFile) <=0) return;

    FILE *file2 = fopen(cFile, "r"); //
    while (ch!=EOF) {
        ch = getc(file2);
        if (ch==EOF) break;
        if (ch>=32) {tmpf[j]=ch;j++;}
        if (ch==10 || j>=lmt-1) {
            tmpf[j]=0;
            for (i2=0; i2<lmt;i2++) {
                fTxtList[ctrTxtFile].txtFile[i][i2]=tmpf[i2];
                if (i2>=j) break;
            }
            fTxtList[ctrTxtFile].txtFile[i][lmt]=0;
            i++;j=0;
            //printf("%s\n", tmpf);
        }
        if (i>=max_rows-1) break;
    }
    fTxtList[ctrTxtFile].num=i;
    fclose(file2);
}

void window_text(char *fileX, int x, int y) {
    loadTextFile(fileX, 200);
    ctrComp=14;
    lp1 = setiPoint(300, 300);
    strcpy(dText, fileX);
    addShapeX(x,y, 1);
    ctrTxtFile++;
}
//================================[Window Options]================================
void wind_close() {
    SelectShape(mouseX, mouseY);
    if (selectedShape==-1) return;
    if (compList[selectedShape].shape==14) {
        compList[selectedShape].flag=0;//Close
        setEdit("Move");
    }
}
void wind_move() {
  setEdit("Move");
}
void wind_resize() {
    setEdit("Move");
    SelectShape(mouseX, mouseY);
    if (selectedShape==-1) return;
    if (compList[selectedShape].shape==14) {
        compList[selectedShape].flag=0;//Close
        flagWindResize=1;
    }
}
void wind_resize_keys(int wParam) {
    int dx=0, dy=0;
    switch (wParam) {
        case VK_UP: dy=-4;break;
        case VK_DOWN: dy=+4;break;
        case VK_LEFT: dx=-4;break;
        case VK_RIGHT: dx=+4; break;
        case VK_SPACE: flagWindResize=0; break;
        case 13: flagWindResize=0; break;
        case VK_F4: flagWindResize=0; break;
    }
    currShape.lpx.getX+=dx;
    currShape.lpx.getY+=dy;
}
void wind_max() {
    setEdit("Move");
    SelectShape(mouseX, mouseY);
    if (selectedShape==-1) return;
    if (compList[selectedShape].shape==14) {
        compList[selectedShape].flag=0;//Close
        //currShape.x=20; currShape.y=30;
        mouseX=20;mouseY=30;
        currShape.lpx.getX=600;
        currShape.lpx.getY=400;
        flagPaste=1; addShapeX(mouseX, mouseY,0);
    }
}

void wind_scroll() {
    setEdit("Move");
    SelectShape(mouseX, mouseY);
    if (selectedShape==-1) return;
    if (compList[selectedShape].shape==14) {
        compList[selectedShape].flag=0;//Close
        flagWindScroll=1;
    }
}
void wind_scroll_keys(int wParam) {
    int dx=0, dy=0;
    switch (wParam) {
        case VK_UP: dy=-1;break;
        case VK_DOWN: dy=+1;break;
        case VK_LEFT: dx=-1;break;
        case VK_RIGHT: dx=+1; break;
        case VK_SPACE: flagWindScroll=0; break;
        case 13: flagWindScroll=0; break;
        case VK_F4: flagWindScroll=0; break;
    }
    currShape.opt.c+=dx;
    currShape.opt.r+=dy;
    if (currShape.opt.c<0) currShape.opt.c=0;
    if (currShape.opt.r<0) currShape.opt.r=0;

}
void wind_sel_file() {
    setEdit("Move");
    SelectShape(mouseX, mouseY);
    if (selectedShape==-1) return;
    if (compList[selectedShape].shape==14) {
        compList[selectedShape].flag=0;//Close
        flagWindSelFile=1;
    }
}
void wind_selFile_keys(int wParam) {
    int dx=0, dy=0;
    switch (wParam) {
        case VK_UP: dy=-1;break;
        case VK_DOWN: dy=+1;break;
        case VK_LEFT: dx=-1;break;
        case VK_RIGHT: dx=+1; break;
        case VK_DELETE: close_Task(); break;
        case VK_SPACE: flagWindSelFile=0; break;
        case 13: flagWindSelFile=0; break;
        case VK_F4: flagWindSelFile=0; break;
    }
    int c = (int) (abs(currShape.lpx.getX)/70);
    //int r = (int) (abs(currShape.lpx.getY)/90);
    currShape.opt.sel += (dy*c)+dx;
    int sel = currShape.opt.sel;
    int rx = (int) sel/c;
    if (rx<0) rx = 0;
    currShape.opt.r = rx;
}

//=================================================================================
void Test_Routine() {
}
void loopTest() {
}

//============================ Basic Sequence =====================================
void setShape() {
    ctrComp = 0;
    flagSelect = 0;
    setEdit("");
    return;
}

void start() {

    bColor = setfRGB(1.0f, 1.0f, 1.0f);
    fColor = setfRGB(0.0f, 0.0f, 0.0f);
    dColor = setfRGB(0.0f, 0.0f, 0.0f);
    wind_Pos = 100;

    lf_font[0] = sizeof(fontFamily)/sizeof(fontFamily[0]);;
    lf_font[1] = sizeof(fontStyle)/sizeof(fontStyle[0]);;
    lf_font[2] = sizeof(fontInclination)/sizeof(fontInclination[0]);;
    lf_font[3] = sizeof(fontSize)/sizeof(fontSize[0]);;

    int ff, ft, fa, fs;
    ff = selectedFont[0];//Font Family
    ft = selectedFont[1];//Font Style
    fa = selectedFont[2];//Font Angle
    fs = atoi(fontSize[selectedFont[3]]);//Font Size
    dFont = setFont(ff, ft, fa, fs);

    shape = 16;exMenu = 0;
    clearScreen();
    menuSelected[4]=1; //drawing
    setShape();
    setEdit("");
    execAddLink(2, "/");
    execAddLink(3, "/TEMP");
    execAddLink(3, "/TEMP/BPAINT");

    execAddLink(4, "/temp/analogtv.txt");
    execAddLink(4, "/temp/adjectives.txt");
    setEdit("");

    /*listFiles("/", 150,50);
    window_text("/temp/analogtv.txt", 200,75);
    listFiles("/TEMP", 250,125);
    window_text("/temp/adjectives.txt", 300,150);
    task_list(350, 175);
    //*/
    setEdit("Move");
    closeWindows();



}
//================================ Keyboard Interface ==================================
void keys_mousePointer(int wParam) {
        switch(wParam) {
            case VK_DOWN: mouseY+=mouseV; break;
            case VK_UP: mouseY-=mouseV; break;
            case VK_RIGHT:mouseX+=mouseV;break;
            case VK_LEFT: mouseX-=mouseV;break;
            case VK_SPACE: mouseClick();break;
            default:break;
        }
    mousePointer();
}
void keys_menuSelection(int wParam) {
        switch(wParam) {
            case VK_DOWN: menuSel ++; break;
            case VK_UP: menuSel --; break;
            case VK_RIGHT: exMenu++;menuSel = 0; break;
            case VK_LEFT: exMenu--;menuSel = 0; break;
            case VK_SPACE: selectMenuItem(); break;
            case 13: selectMenuItem(); break;
            default: break;
        }
}
void keyBoard_Generic(int wParam) {
        switch (wParam)
        {
            case VK_ESCAPE: bQuit = GL_TRUE; break;
            case VK_F1: exMenu ++; shape = 15;int i = 0;menuSel = 0;flagP1 = 1; break;
            case VK_F2: shape = 16;exMenu = 0;flagP1 = 1; break;
            case VK_F3: clearScreen(); break;
            case VK_F4: CloseBox(); break;
            case VK_F5: openLink(); break;
            case VK_F6: Test_Routine(); break;
        }
}
void keyboardCheck(int wParam) {
            if (wParam==VK_ESCAPE) {bQuit = GL_TRUE;return;}
            if (flagInputBox==1) {getInput(wParam);return;}
            if (flagColorPicker==1) {changeColorKeys(wParam);return;}
            if (flagFontPicker==1) {changeFontKeys(wParam);return;}
            if (flagMsgBox==1) {msgBoxKeys(wParam);return;}
            if (flagWindResize==1) {wind_resize_keys(wParam);return;}
            if (flagWindScroll==1) {wind_scroll_keys(wParam);return;}
            if (flagWindSelFile==1) {wind_selFile_keys(wParam);return;}
            if (shape==15) {keys_menuSelection(wParam);}
            if (shape==16) {keys_mousePointer(wParam);}
            keyBoard_Generic(wParam);
}
//=================================== Main Loop  =======================================
void loop() {
    drawScreen();
    if (flagMsgBox==1 || flagInputBox==1 || flagColorPicker==1 || flagFontPicker==1) {
            if (flagMsgBox==1) msgBox();
            if (flagInputBox==1)inputBox();
            if (flagColorPicker==1) changeColor();
            if (flagFontPicker==1) changeFont();
    } else {
        mousePointer();
        mouseMove();
    }
    if (flagTest==1) loopTest();
    printMenus();
}
//=======================================================================================
