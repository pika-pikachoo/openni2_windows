#include "stdafx.h"
#include "Ni2CameraMatrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_STR_SIZE 128

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
            double fx, fy, cx, cy;
            char strDepBuff[MAX_STR_SIZE] = {0};
            char strImgBuff[MAX_STR_SIZE] = {0};
			if ( STATUS_OK != OpenNI::initialize() ) {
                cout << "After initialization: " << OpenNI::getExtendedError() << endl;
                nRetCode = 1;
            } else {
                Device devDevice;
                if ( STATUS_OK != devDevice.open( ANY_DEVICE) ) {
                    cout << "Cannot open device: " << OpenNI::getExtendedError() << endl;
                    nRetCode = 1;
                } else {
                    VideoStream vsDepth;
                    if ( STATUS_OK != vsDepth.create( devDevice, SENSOR_DEPTH ) ) {
                        cout << "Cannot create depth stream on device: " << OpenNI::getExtendedError() << endl;
                        nRetCode = 1;
                    } else {
                        vsDepth.getProperty(LIPS_STREAM_PROPERTY_FOCAL_LENGTH_X, &fx);
                        vsDepth.getProperty(LIPS_STREAM_PROPERTY_FOCAL_LENGTH_Y, &fy);
                        vsDepth.getProperty(LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_X, &cx);
                        vsDepth.getProperty(LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_Y, &cy);
                        sprintf_s(strDepBuff, "fx=%f, fy=%f, cx=%f, cy=%f\n", fx, fy, cx, cy);
                        vsDepth.destroy();
                    }
                    VideoStream vsColor;
                    if ( STATUS_OK != vsColor.create( devDevice, SENSOR_COLOR ) ) {
                        cout << "Cannot create color stream on device: " << OpenNI::getExtendedError() << endl;
                        nRetCode = 1;
                    } else {
                        vsColor.getProperty(LIPS_STREAM_PROPERTY_FOCAL_LENGTH_X, &fx);
                        vsColor.getProperty(LIPS_STREAM_PROPERTY_FOCAL_LENGTH_Y, &fy);
                        vsColor.getProperty(LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_X, &cx);
                        vsColor.getProperty(LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_Y, &cy);
                        sprintf_s(strImgBuff, "fx=%f, fy=%f, cx=%f, cy=%f\n", fx, fy, cx, cy);
                        vsColor.destroy();
                    }
                }
                CString cstr = CString("[Depth camera]\n") + CString(strDepBuff) + CString("\n[Image camera]\n") + CString(strImgBuff);
                MessageBox( nullptr, cstr, TEXT( "Camera matrix" ), MB_OK );
                
                devDevice.close();
                OpenNI::shutdown();
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
