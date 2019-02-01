#include "Ni2CameraMatrix.h"

#define MAX_STR_SIZE 256
//#define DEVICE_M5

using namespace std;
using namespace openni;

int main( int argc, char* argv[] )
{
    int nRetCode = 0;

    double fx = 0, fy = 0, cx = 0, cy = 0;
    char strDepBuff[MAX_STR_SIZE] = {0};
    char strImgBuff[MAX_STR_SIZE] = {0};

    if ( STATUS_OK != OpenNI::initialize() )
    {
        cout << "After initialization: " << OpenNI::getExtendedError() << endl;
        nRetCode = 1;
    }
    else
    {
        stringstream streamIntrinsics;

        Device devDevice;
        if ( STATUS_OK != devDevice.open( ANY_DEVICE ) )
        {
            cout << "Cannot open device: " << OpenNI::getExtendedError() << endl;
             nRetCode = 1;
        }
        else
        {
            VideoStream vsDepth;
            if ( STATUS_OK != vsDepth.create( devDevice, SENSOR_DEPTH ) )
            {
                cout << "Cannot create depth stream on device: " << OpenNI::getExtendedError() << endl;
                nRetCode = 1;
            }
            else
            {
                streamIntrinsics << "[Depth Camera]" << endl;
#ifdef DEVICE_M5
                // Right Camera
                vsDepth.getProperty( LIPS_STREAM_PROPERTY_FOCAL_LENGTH_X_R, &fx );
                vsDepth.getProperty( LIPS_STREAM_PROPERTY_FOCAL_LENGTH_Y_R, &fy );
                vsDepth.getProperty( LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_X_R, &cx );
                vsDepth.getProperty( LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_Y_R, &cy );
                streamIntrinsics << "fxR=" << fx << ", fyR=" << fy << ", cxR=" << cx << ", cyR=" << cy << endl;

                // Left Camera
                vsDepth.getProperty( LIPS_STREAM_PROPERTY_FOCAL_LENGTH_X_L, &fx );
                vsDepth.getProperty( LIPS_STREAM_PROPERTY_FOCAL_LENGTH_Y_L, &fy );
                vsDepth.getProperty( LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_X_L, &cx );
                vsDepth.getProperty( LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_Y_L, &cy );
                streamIntrinsics << "fxL=" << fx << ", fyL=" << fy << ", cxL=" << cx << ", cyL=" << cy << endl;
#else
                vsDepth.getProperty( LIPS_STREAM_PROPERTY_FOCAL_LENGTH_X, &fx );
                vsDepth.getProperty( LIPS_STREAM_PROPERTY_FOCAL_LENGTH_Y, &fy );
                vsDepth.getProperty( LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_X, &cx );
                vsDepth.getProperty( LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_Y, &cy );
                streamIntrinsics << "fx=" << fx << ", fy=" << fy << ", cx=" << cx << ", cy=" << cy << endl;
#endif
                vsDepth.destroy();
            }

            VideoStream vsColor;
            if ( STATUS_OK != vsColor.create( devDevice, SENSOR_COLOR ) )
            {
                cout << "Cannot create color stream on device: " << OpenNI::getExtendedError() << endl;
                nRetCode = 1;
            }
            else
            {
                streamIntrinsics << "[Color Camera]" << endl;
                vsColor.getProperty( LIPS_STREAM_PROPERTY_FOCAL_LENGTH_X, &fx );
                vsColor.getProperty( LIPS_STREAM_PROPERTY_FOCAL_LENGTH_Y, &fy );
                vsColor.getProperty( LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_X, &cx );
                vsColor.getProperty( LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_Y, &cy );
                streamIntrinsics << "fx=" << fx << ", fy=" << fy << ", cx=" << cx << ", cy=" << cy << endl;
                vsColor.destroy();
            }
        }

        cout << endl << "==================================================" << endl;
        cout << streamIntrinsics.str();
        cout << "==================================================" << endl << endl;

        cout << "Press ENTER to leave...";
        char key = getchar();

        devDevice.close();
        OpenNI::shutdown();
    }

    return nRetCode;
}
