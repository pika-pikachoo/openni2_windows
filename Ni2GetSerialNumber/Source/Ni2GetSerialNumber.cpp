#include <OpenNI.h>
#include <cstdio>
#include <iostream>

using namespace std;
using namespace openni;

int main( int argc, char* argv[] )
{
    int nRetCode = 0;

    if ( STATUS_OK != OpenNI::initialize() )
    {
        cout << "After initialization: " << OpenNI::getExtendedError() << endl;
        nRetCode = 1;
    }
    else
    {
        Device devDevice;
        if ( STATUS_OK != devDevice.open( ANY_DEVICE ) )
        {
            cout << "Cannot open device: " << OpenNI::getExtendedError() << endl;
            nRetCode = 1;
        }
        else
        {
            char serialNumberBuffer[1024] = {0};
            devDevice.getProperty( ONI_DEVICE_PROPERTY_SERIAL_NUMBER, &serialNumberBuffer );

            cout << endl << "==================================================" << endl;
            cout << "Serial Number: " << serialNumberBuffer << endl;
            cout << "==================================================" << endl << endl;

            cout << "Press ENTER to leave...";
            char key = getchar();
        }
        devDevice.close();
        OpenNI::shutdown();
    }

    return nRetCode;
}
