/*
    Raydium - CQFD Corp.
    http://raydium.org/
    vfw extra definitions
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

typedef struct tagCapDriverCaps {
    UINT        wDeviceIndex;               
    BOOL        fHasOverlay;                
    BOOL        fHasDlgVideoSource;         
    BOOL        fHasDlgVideoFormat;         
    BOOL        fHasDlgVideoDisplay;        
    BOOL        fCaptureInitialized;        
    BOOL        fDriverSuppliesPalettes;    
    HANDLE      hVideoIn;                   
    HANDLE      hVideoOut;                  
    HANDLE      hVideoExtIn;                
    HANDLE      hVideoExtOut;               
} CAPDRIVERCAPS, *PCAPDRIVERCAPS, FAR *LPCAPDRIVERCAPS;

typedef struct tagCapStatus {
    UINT        uiImageWidth;               
    UINT        uiImageHeight;              
    BOOL        fLiveWindow;                
    BOOL        fOverlayWindow;             
    BOOL        fScale;                     
    POINT       ptScroll;                   
    BOOL        fUsingDefaultPalette;       
    BOOL        fAudioHardware;             
    BOOL        fCapFileExists;             
    DWORD       dwCurrentVideoFrame;        
    DWORD       dwCurrentVideoFramesDropped;
    DWORD       dwCurrentWaveSamples;       
    DWORD       dwCurrentTimeElapsedMS;     
    HPALETTE    hPalCurrent;                
    BOOL        fCapturingNow;              
    DWORD       dwReturn;                   
    UINT        wNumVideoAllocated;         
    UINT        wNumAudioAllocated;         
} CAPSTATUS, *PCAPSTATUS, FAR *LPCAPSTATUS;

typedef struct tagCaptureParms {
    DWORD       dwRequestMicroSecPerFrame;  
    BOOL        fMakeUserHitOKToCapture;    
    UINT        wPercentDropForError;       
    BOOL        fYield;                    
    DWORD       dwIndexSize;                
    UINT        wChunkGranularity;          
    BOOL        fUsingDOSMemory;            
    UINT        wNumVideoRequested;         
    BOOL        fCaptureAudio;              
    UINT        wNumAudioRequested;         
    UINT        vKeyAbort;                  
    BOOL        fAbortLeftMouse;            
    BOOL        fAbortRightMouse;           
    BOOL        fLimitEnabled;              
    UINT        wTimeLimit;                 
    BOOL        fMCIControl;                
    BOOL        fStepMCIDevice;             
    DWORD       dwMCIStartTime;             
    DWORD       dwMCIStopTime;              
    BOOL        fStepCaptureAt2x;           
    UINT        wStepCaptureAverageFrames;  
    DWORD       dwAudioBufferSize;          
    BOOL        fDisableWriteCache;         
    UINT        AVStreamMaster;             
} CAPTUREPARMS, *PCAPTUREPARMS, FAR *LPCAPTUREPARMS;

typedef struct videohdr_tag {
    LPBYTE      lpData;                 
    DWORD       dwBufferLength;         
    DWORD       dwBytesUsed;            
    DWORD       dwTimeCaptured;         
    DWORD       dwUser;                 
    DWORD       dwFlags;                
    DWORD       dwReserved[4];          
} VIDEOHDR, NEAR *PVIDEOHDR, FAR * LPVIDEOHDR;

#define WM_CAP_START                    WM_USER

#define AVICapSM(hwnd,m,w,l) ( (IsWindow(hwnd)) ?   SendMessage(hwnd,m,w,l) : 0)

#define WM_CAP_DRIVER_CONNECT           (WM_CAP_START+  10)
#define capDriverConnect(hwnd, i)                  ((BOOL)AVICapSM(hwnd, WM_CAP_DRIVER_CONNECT, (WPARAM)(i), 0L))

#define WM_CAP_DRIVER_DISCONNECT        (WM_CAP_START+  11)
#define capDriverDisconnect(hwnd)                  ((BOOL)AVICapSM(hwnd, WM_CAP_DRIVER_DISCONNECT, (WPARAM)0, 0L))

#define WM_CAP_GET_STATUS               (WM_CAP_START+  54)
#define capGetStatus(hwnd, s, wSize)               ((BOOL)AVICapSM(hwnd, WM_CAP_GET_STATUS, (WPARAM)(wSize), (LPARAM)(LPVOID)(LPCAPSTATUS)(s)))

#define WM_CAP_DLG_VIDEOFORMAT          (WM_CAP_START+  41)
#define capDlgVideoFormat(hwnd)                    ((BOOL)AVICapSM(hwnd, WM_CAP_DLG_VIDEOFORMAT, 0, 0L))

#define WM_CAP_GET_VIDEOFORMAT          (WM_CAP_START+  44)
#define capGetVideoFormat(hwnd, s, wSize)          ((DWORD)AVICapSM(hwnd, WM_CAP_GET_VIDEOFORMAT, (WPARAM)(wSize), (LPARAM)(LPVOID)(s)))

#define WM_CAP_SET_VIDEOFORMAT          (WM_CAP_START+  45)
#define capSetVideoFormat(hwnd, s, wSize)          ((BOOL)AVICapSM(hwnd, WM_CAP_SET_VIDEOFORMAT, (WPARAM)(wSize), (LPARAM)(LPVOID)(s)))

#define WM_CAP_SET_SEQUENCE_SETUP       (WM_CAP_START+  64)
#define capCaptureSetSetup(hwnd, s, wSize)         ((BOOL)AVICapSM(hwnd, WM_CAP_SET_SEQUENCE_SETUP, (WPARAM)(wSize), (LPARAM)(LPVOID)(LPCAPTUREPARMS)(s)))

#define WM_CAP_SET_CALLBACK_VIDEOSTREAM (WM_CAP_START+  6)
#define capSetCallbackOnVideoStream(hwnd, fpProc)  ((BOOL)AVICapSM(hwnd, WM_CAP_SET_CALLBACK_VIDEOSTREAM, 0, (LPARAM)(LPVOID)(fpProc)))

#define WM_CAP_SEQUENCE_NOFILE          (WM_CAP_START+  63)
#define capCaptureSequenceNoFile(hwnd)             ((BOOL)AVICapSM(hwnd, WM_CAP_SEQUENCE_NOFILE, (WPARAM)0, (LPARAM)0L))

#define WM_CAP_DRIVER_GET_CAPS          (WM_CAP_START+  14)
#define capDriverGetCaps(hwnd, s, wSize)           ((BOOL)AVICapSM(hwnd, WM_CAP_DRIVER_GET_CAPS, (WPARAM)(wSize), (LPARAM)(LPVOID)(LPCAPDRIVERCAPS)(s)))

#define WM_CAP_GET_SEQUENCE_SETUP       (WM_CAP_START+  65)
#define capCaptureGetSetup(hwnd, s, wSize)         ((BOOL)AVICapSM(hwnd, WM_CAP_GET_SEQUENCE_SETUP, (WPARAM)(wSize), (LPARAM)(LPVOID)(LPCAPTUREPARMS)(s)))

#define WM_CAP_GET_USER_DATA		(WM_CAP_START+  8)
#define capGetUserData(hwnd)               (AVICapSM(hwnd, WM_CAP_GET_USER_DATA, 0, 0))

#define WM_CAP_SET_USER_DATA		(WM_CAP_START+  9)
#define capSetUserData(hwnd, lUser)        ((BOOL)AVICapSM(hwnd, WM_CAP_SET_USER_DATA, 0, (LPARAM)lUser))

