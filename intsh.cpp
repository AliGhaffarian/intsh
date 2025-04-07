#include <iostream>

#include <string>

#include <vector>

#include <Windows.h>



LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

using namespace std;
string banner = R"(  
  iiii                           tttt                           hhhhhhh             
 i::::i                        ttt:::t                          h:::::h             
  iiii                         t:::::t                          h:::::h             
                               t:::::t                          h:::::h             
iiiiiiinnnn  nnnnnnnn    ttttttt:::::ttttttt        ssssssssss   h::::h hhhhh       
i:::::in:::nn::::::::nn  t:::::::::::::::::t      ss::::::::::s  h::::hh:::::hhh    
 i::::in::::::::::::::nn t:::::::::::::::::t    ss:::::::::::::s h::::::::::::::hh  
 i::::inn:::::::::::::::ntttttt:::::::tttttt    s::::::ssss:::::sh:::::::hhh::::::h 
 i::::i  n:::::nnnn:::::n      t:::::t           s:::::s  ssssss h::::::h   h::::::h
 i::::i  n::::n    n::::n      t:::::t             s::::::s      h:::::h     h:::::h
 i::::i  n::::n    n::::n      t:::::t                s::::::s   h:::::h     h:::::h
 i::::i  n::::n    n::::n      t:::::t    ttttttssssss   s:::::s h:::::h     h:::::h
i::::::i n::::n    n::::n      t::::::tttt:::::ts:::::ssss::::::sh:::::h     h:::::h
i::::::i n::::n    n::::n      tt::::::::::::::ts::::::::::::::s h:::::h     h:::::h
i::::::i n::::n    n::::n        tt:::::::::::tt s:::::::::::ss  h:::::h     h:::::h
iiiiiiii nnnnnn    nnnnnn          ttttttttttt    sssssssssss    hhhhhhh     hhhhhhh
)";

string banner_delim = "====================================================================================\n";
string intsh_msg = "INTSH / SHUTDOWN STOPPER\n";
string credit_msg = "CREDIT\n";
vector<string> credit_list = {
    "SunbroHere https://github.com/AliGhaffarian",
    "slofj link http://slofj.ir"
};
string instruction_banner = "INSTRUCTIONS\n"; 
vector<string> instructions = {
    "start intsh",
    "restart/shutdown your computer",
    "intsh will stop the restarting process, wait for 10 seconds and hit cancel",
};
template <typename t>void print_list(vector<t> the_list, bool numbered = true){
    for(int i = 0 ; i < the_list.size() ; i++){
        if (numbered) cout << i + 1 << " - ";
        cout << the_list[i] << '\n';
    }
}
enum colors {
    bright_green = 0xA,
    bright_blue = 9,
    bright_red = 0xC,
};
void Color(int color=0x07)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void print_in_color(colors c, string msg){
        Color(c);
        cout << msg;
        Color();
}
void print_msg_and_delim(colors c, string msg, char delim){
    print_in_color(c, msg);
    string delim_str = string(msg.size(), delim);
    cout << delim_str << '\n';
}
void print_init_msgs(){
    cout << banner_delim;
    print_in_color(colors::bright_green, banner);
    cout << banner_delim;
    cout << '\n';
    print_msg_and_delim(colors::bright_blue, intsh_msg, '-');
    cout << '\n';
    print_msg_and_delim(colors::bright_blue, credit_msg, '-');  
    print_list(credit_list, false);
    cout << '\n';
    print_msg_and_delim(colors::bright_red, instruction_banner, '-');
    print_list(instructions);
}
int APIENTRY main( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPWSTR lpCmdLine, int nCmdShow )
{
    print_init_msgs();
    WNDCLASSEXW wx = { sizeof(wx) }; // set cbSize member and zero-initialize all other
    wx.lpfnWndProc = WndProc;
    wx.hInstance = hInstance;
    wx.lpszClassName = L"MyWindowClass";

    if( ! RegisterClassExW( &wx ) )
        return 1;  // TODO: improve error handling

    HWND hWnd = CreateWindowExW( 0, wx.lpszClassName, L"intsh", 0, 0, 0, 0, 0,
                                 NULL, NULL, NULL, NULL );
    if( ! hWnd )
        return 2;  // TODO: improve error handling


    MSG msg;
    while( GetMessage( &msg, nullptr, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

    return static_cast<int>( msg.wParam );
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch( message )
    {
        case WM_QUERYENDSESSION:
        {
            // Try to block shutdown.
            ShutdownBlockReasonCreate( hWnd, L"wait for 10 seconds, then hit cancel" );
            return FALSE;
        }
        case WM_ENDSESSION:
        {
            // TODO: Always handle this message because shutdown can be forced
            // even if we return FALSE from WM_QUERYENDSESSION!
            return 0;
        }
        default:
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}