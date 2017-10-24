#include "stdafx.h"
#include "Ni2GetSerialNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;

using namespace std;
using namespace openni;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(NULL);

    if (hModule != NULL)
    {
        if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
        {
            _tprintf(_T("Fatal error: MFC initialzation fails\n"));
            nRetCode = 1;
        }
        else
        {
            if ( STATUS_OK != OpenNI::initialize() ) {
                cout << "After initialization: " << OpenNI::getExtendedError() << endl;
                nRetCode = 1;
            } else {
                Device devDevice;
                if ( STATUS_OK != devDevice.open( ANY_DEVICE) ) {
                    cout << "Cannot open device: " << OpenNI::getExtendedError() << endl;
                    nRetCode = 1;
                } else {
                    char serialNumberBuffer[1024] = {0};
                    devDevice.getProperty(ONI_DEVICE_PROPERTY_SERIAL_NUMBER, &serialNumberBuffer);
                    MessageBox( nullptr, CString(serialNumberBuffer), TEXT( "Serial Number" ), MB_OK );
                }
                devDevice.close();
            }
        }
    }
    else
    {
        _tprintf(_T("Fatal error: GetModuleHandle fails\n"));
        nRetCode = 1;
    }

    return nRetCode;
}
