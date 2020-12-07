// #include "htmlayout\htmlayout.dll"
// #include <htmlayout.h>

#pragma comment(lib, "HTMLayout.lib")

#include <tchar.h> 
#include <Windows.h>   
#pragma comment ( lib, "HTMLayout.lib" )   

LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );   

struct DOMEventsHandlerType: htmlayout::event_handler {       
	DOMEventsHandlerType(): event_handler( 0xFFFFFFFF ) {}       
	virtual BOOL handle_event ( HELEMENT he, BEHAVIOR_EVENT_PARAMS& params )       {         
		switch( params.cmd )         {         
		case BUTTON_CLICK:         {
			// click on button                     
			htmlayout::dom::element el = params.heTarget;             
			HWND hWnd = el.get_element_hwnd( true );             
			if ( !wcscmp( el.get_attribute( "id" ), _T( "myButton" )))                 MessageBox( hWnd, _T( "You click on the button" ), _T( "Message" ), MB_OK );             
			break;         
		}           
		case BUTTON_PRESS: break;// mouse down or key down in button         
		case BUTTON_STATE_CHANGED: break;         
		case EDIT_VALUE_CHANGING: break;// before text change         
		case EDIT_VALUE_CHANGED: break;//after text change         
		case SELECT_SELECTION_CHANGED:  break;// selection in <select> changed         
		case SELECT_STATE_CHANGED: break;// node in select expanded/collapsed, heTarget is the node         
		case POPUP_REQUEST: break;// request to show popup just received,                                               //     here DOM of popup element can be modifed.         
		case POPUP_READY: break;// popup element has been measured and ready to be shown on screen,                                               //     here you can use functions like ScrollToView.         
		case POPUP_DISMISSED: break;// popup element is closed,                                               //     here DOM of popup element can be modifed again - e.g. some items can be removed                                               //     to free memory.         
		case MENU_ITEM_ACTIVE: break; // menu item activated by mouse hover or by keyboard                       
		case MENU_ITEM_CLICK: break;                // menu item click                               // "grey" event codes  - notfications from behaviors from this SDK         
		case HYPERLINK_CLICK: break;// hyperlink click         
		case TABLE_HEADER_CLICK:  break;// click on some cell in table header,                                               //     target = the cell,                                               //     reason = index of the cell (column number, 0..n)         
		case TABLE_ROW_CLICK: break;// click on data row in the table, target is the row                                               //     target = the row,                                               //     reason = index of the row (fixed_rows..n)         
		case TABLE_ROW_DBL_CLICK: break;// mouse dbl click on data row in the table, target is the row                                               //     target = the row,                                               //     reason = index of the row (fixed_rows..n)           
		case ELEMENT_COLLAPSED: break;// element was collapsed, so far only behavior:tabs is sending these two to the panels         
		case ELEMENT_EXPANDED: break;// element was expanded,           
		}      
		
		return FALSE;       
	}   
} DOMEventsHandler;       

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {     
	WNDCLASSEX mainWindow;       
	memset( &mainWindow, 0, sizeof( WNDCLASSEX ));       
	mainWindow.cbSize        = sizeof( WNDCLASSEX );    
	mainWindow.hbrBackground = ( HBRUSH )GetStockObject( WHITE_BRUSH );     
	mainWindow.hCursor       = LoadCursor( NULL, IDC_ARROW );     
	mainWindow.hInstance     = hInstance;     
	mainWindow.lpfnWndProc   = WindowProc;     
	mainWindow.lpszClassName = _T( "Example" );       
	RegisterClassEx( &mainWindow );       
	HWND hWnd = CreateWindowEx( NULL, mainWindow.lpszClassName, _T( "Example" ), WS_VISIBLE | WS_OVERLAPPEDWINDOW, \                                 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, \                                 HWND_DESKTOP, NULL, hInstance, 0 );       
	if ( NULL == hWnd )     {         
		 MessageBox( NULL, _T( "Error creating window" ), _T( "Error" ), MB_OK );         
		return EXIT_FAILURE;     
	}   
	MSG uMsg;     
	BOOL bRet;    
	while( 0 != ( bRet = GetMessage( &uMsg, NULL, 0, 0 ))) {         	if ( -1 == bRet )         {             
		return EXIT_FAILURE;         
		}         else            {             
		TranslateMessage( &uMsg );            
		DispatchMessage( &uMsg );         
		}    
	}         
	return uMsg.wParam; 
}   

LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {     
	BOOL bHandled;     
	LRESULT lResult = HTMLayoutProcND( hWnd, uMsg, wParam, lParam, &bHandled );     
	if ( 0 != bHandled ) return lResult;       
	
	switch( uMsg )     {        
		case WM_CREATE:             
			htmlayout::attach_event_handler(hWnd, &DOMEventsHandler);               
			HTMLayoutLoadFile( hWnd, _T( "D:\\Projects\\C++\\Lessons\\HTMLayout\\Example\\index.html" ));             
		return 0;           
		case WM_DESTROY:             
			PostQuitMessage( 0 );             
		return 0;     
	}     
	return DefWindowProc( hWnd, uMsg, wParam, lParam ); 
}