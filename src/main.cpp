#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <gl/GL.h>
#include <cmath>
#include <algorithm>

#pragma comment(lib, "opengl32.lib")

namespace {
constexpr int WIDTH = 1280, HEIGHT = 720;
HWND g_window = nullptr;
HDC g_dc = nullptr;
HGLRC g_gl = nullptr;
bool g_keys[256]{};
bool g_mouseCaptured = false;
bool g_firstMouse = true;
POINT g_center{};
float g_x = 0.0f, g_z = 5.0f, g_yaw = 0.0f, g_pitch = 0.0f;
float g_time = 0.0f;
LARGE_INTEGER g_frequency{}, g_previous{};

void perspective(float fov, float aspect, float nearPlane, float farPlane) {
    const float top = nearPlane * std::tan(fov * 3.14159265f / 360.0f);
    const float right = top * aspect;
    glFrustum(-right, right, -top, top, nearPlane, farPlane);
}

void cube(float x, float y, float z, float sx, float sy, float sz,
          float r, float g, float b) {
    const float x0=x-sx/2, x1=x+sx/2, y0=y-sy/2, y1=y+sy/2, z0=z-sz/2, z1=z+sz/2;
    glColor3f(r,g,b);
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z1); glVertex3f(x1,y0,z1); glVertex3f(x1,y1,z1); glVertex3f(x0,y1,z1);
    glVertex3f(x1,y0,z0); glVertex3f(x0,y0,z0); glVertex3f(x0,y1,z0); glVertex3f(x1,y1,z0);
    glVertex3f(x0,y0,z0); glVertex3f(x0,y0,z1); glVertex3f(x0,y1,z1); glVertex3f(x0,y1,z0);
    glVertex3f(x1,y0,z1); glVertex3f(x1,y0,z0); glVertex3f(x1,y1,z0); glVertex3f(x1,y1,z1);
    glVertex3f(x0,y1,z1); glVertex3f(x1,y1,z1); glVertex3f(x1,y1,z0); glVertex3f(x0,y1,z0);
    glVertex3f(x0,y0,z0); glVertex3f(x1,y0,z0); glVertex3f(x1,y0,z1); glVertex3f(x0,y0,z1);
    glEnd();
}

void tree(float x, float z, float scale) {
    cube(x, 1.0f*scale, z, .42f*scale, 2.0f*scale, .42f*scale, .16f,.10f,.06f);
    cube(x, 2.45f*scale, z, 1.7f*scale, 1.7f*scale, 1.7f*scale, .035f,.12f,.055f);
    cube(x, 3.15f*scale, z, 1.15f*scale, 1.2f*scale, 1.15f*scale, .025f,.085f,.04f);
}

void renderScene() {
    RECT rc; GetClientRect(g_window, &rc);
    const int w = std::max(1L, rc.right-rc.left), h = std::max(1L, rc.bottom-rc.top);
    glViewport(0,0,w,h);
    glClearColor(.012f,.018f,.022f,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    perspective(72.0f, float(w)/float(h), .08f, 180.0f);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glRotatef(-g_pitch,1,0,0);
    glRotatef(-g_yaw,0,1,0);
    glTranslatef(-g_x,-1.65f,-g_z);

    // Moonlit clearing
    glColor3f(.055f,.065f,.065f);
    glBegin(GL_QUADS);
    glVertex3f(-70,0,-70); glVertex3f(70,0,-70); glVertex3f(70,0,70); glVertex3f(-70,0,70);
    glEnd();

    // Distant forest perimeter
    for(int i=-8;i<=8;i++) {
        tree(float(i)*5.5f,-25.0f,1.15f);
        tree(float(i)*5.5f, 25.0f,1.15f);
        if(i>-8 && i<8) { tree(-42.0f,float(i)*5.5f,1.1f); tree(42.0f,float(i)*5.5f,1.1f); }
    }
    // Scattered trees in the clearing
    tree(-12,-9,1.0f); tree(13,-12,1.2f); tree(-17,10,1.15f);
    tree(18,9,1.0f); tree(8,18,.9f); tree(-8,19,1.0f);

    // A dim structure in the distance
    cube(0,1.7f,-30,8,3.4f,1.0f,.055f,.06f,.065f);
    cube(0,3.5f,-30,9,.25f,1.2f,.025f,.03f,.035f);

    // Small pool of light near the spawn point
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); glOrtho(0,w,h,0,-1,1);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glColor4f(.75f,.72f,.48f,.16f);
    glBegin(GL_TRIANGLE_FAN); glVertex2f(w*.5f,h*.5f);
    for(int i=0;i<=40;i++){ float a=i*6.2831853f/40; glVertex2f(w*.5f+std::cos(a)*4,h*.5f+std::sin(a)*4); }
    glEnd();
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void update(float dt) {
    g_time += dt;
    const float speed = (g_keys[VK_SHIFT] ? 7.0f : 4.2f) * dt;
    const float yaw = g_yaw * 3.14159265f / 180.0f;
    float forward = 0, side = 0;
    if(g_keys['W']) forward += 1; if(g_keys['S']) forward -= 1;
    if(g_keys['D']) side += 1; if(g_keys['A']) side -= 1;
    const float len = std::sqrt(forward*forward+side*side);
    if(len > 0) { forward/=len; side/=len; }
    g_x += (std::sin(yaw)*forward + std::cos(yaw)*side) * speed;
    g_z += (-std::cos(yaw)*forward + std::sin(yaw)*side) * speed;
    g_x = std::clamp(g_x,-38.0f,38.0f);
    g_z = std::clamp(g_z,-22.0f,22.0f);
}

void captureMouse() {
    if(g_mouseCaptured) return;
    g_mouseCaptured = true; g_firstMouse = true;
    RECT r; GetClientRect(g_window,&r);
    POINT p{(r.right-r.left)/2,(r.bottom-r.top)/2};
    ClientToScreen(g_window,&p); g_center=p;
    SetCursorPos(g_center.x,g_center.y);
    ShowCursor(FALSE);
}

void releaseMouse() {
    if(!g_mouseCaptured) return;
    g_mouseCaptured=false; ShowCursor(TRUE); ClipCursor(nullptr);
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch(msg) {
    case WM_LBUTTONDOWN: captureMouse(); return 0;
    case WM_KEYDOWN:
        if(wp<256) g_keys[wp]=true;
        if(wp==VK_ESCAPE) { releaseMouse(); }
        return 0;
    case WM_KEYUP: if(wp<256) g_keys[wp]=false; return 0;
    case WM_MOUSEMOVE:
        if(g_mouseCaptured) {
            POINT p{GET_X_LPARAM(lp),GET_Y_LPARAM(lp)}; ClientToScreen(hwnd,&p);
            if(g_firstMouse) { g_firstMouse=false; g_center=p; }
            else {
                g_yaw += (p.x-g_center.x)*.12f;
                g_pitch = std::clamp(g_pitch+(p.y-g_center.y)*.12f,-85.0f,85.0f);
                SetCursorPos(g_center.x,g_center.y);
            }
        }
        return 0;
    case WM_SIZE: return 0;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProc(hwnd,msg,wp,lp);
}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int show) {
    WNDCLASSA wc{}; wc.style=CS_OWNDC; wc.lpfnWndProc=windowProc;
    wc.hInstance=instance; wc.lpszClassName="GP4WindowsWindow";
    wc.hCursor=LoadCursor(nullptr,IDC_ARROW); RegisterClassA(&wc);
    g_window=CreateWindowA(wc.lpszClassName,"GP 4 - Windows Prototype",
        WS_OVERLAPPEDWINDOW|WS_VISIBLE,CW_USEDEFAULT,CW_USEDEFAULT,WIDTH,HEIGHT,
        nullptr,nullptr,instance,nullptr);
    if(!g_window) return 1;
    g_dc=GetDC(g_window);
    PIXELFORMATDESCRIPTOR pfd{}; pfd.nSize=sizeof(pfd); pfd.nVersion=1;
    pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
    pfd.iPixelType=PFD_TYPE_RGBA; pfd.cColorBits=32; pfd.cDepthBits=24; pfd.iLayerType=PFD_MAIN_PLANE;
    int format=ChoosePixelFormat(g_dc,&pfd);
    if(!format || !SetPixelFormat(g_dc,format,&pfd)) return 2;
    g_gl=wglCreateContext(g_dc); if(!g_gl || !wglMakeCurrent(g_dc,g_gl)) return 3;
    ShowWindow(g_window,show); QueryPerformanceFrequency(&g_frequency); QueryPerformanceCounter(&g_previous);
    MSG msg{}; bool running=true;
    while(running) {
        while(PeekMessage(&msg,nullptr,0,0,PM_REMOVE)) {
            if(msg.message==WM_QUIT) running=false;
            TranslateMessage(&msg); DispatchMessage(&msg);
        }
        LARGE_INTEGER now; QueryPerformanceCounter(&now);
        float dt=float(now.QuadPart-g_previous.QuadPart)/float(g_frequency.QuadPart);
        g_previous=now; dt=std::min(dt,.05f);
        update(dt); renderScene(); SwapBuffers(g_dc);
    }
    releaseMouse(); wglMakeCurrent(nullptr,nullptr); wglDeleteContext(g_gl);
    ReleaseDC(g_window,g_dc); return 0;
}
