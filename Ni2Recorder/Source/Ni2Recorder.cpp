#include <OpenNI.h>
#include <iostream>

//#define DEVICE_M3

#define RECORDING_FILE_NAME "recording.oni"
#define RECORDING_FRAME 100
#define RECORDING_FPS 30

#ifdef DEVICE_M3
#define RECORDING_WIDTH 80
#define RECORDING_HEIGHT 60
#else
#define RECORDING_WIDTH 640
#define RECORDING_HEIGHT 480
#endif

using namespace std;
using namespace openni;

int main( int argc, char* argv[] )
{
    if ( STATUS_OK != OpenNI::initialize() )
    {
        cout << "After initialization: " << OpenNI::getExtendedError() << endl;
        return 1;
    }

    Device devDevice;
    if ( STATUS_OK != devDevice.open( ANY_DEVICE) )
    {
        cout << "Cannot open device: " << OpenNI::getExtendedError() << endl;
        return 1;
    }

    VideoMode mode;
    mode.setResolution( RECORDING_WIDTH, RECORDING_HEIGHT );
    mode.setFps( RECORDING_FPS );

    VideoStream vsDepth;
    if ( STATUS_OK != vsDepth.create( devDevice, SENSOR_DEPTH ) )
    {
        cout << "Cannot create depth stream on device: " << OpenNI::getExtendedError() << endl;
        return 1;
    }
    else
    {
        mode.setPixelFormat( PIXEL_FORMAT_DEPTH_1_MM );
        if ( STATUS_OK != vsDepth.setVideoMode( mode ) )
        {
            cout << "Cannot set video mode: " << OpenNI::getExtendedError() << endl;
            return 1;
        }
    }

#ifndef DEVICE_M3
    VideoStream vsColor;
    if ( STATUS_OK != vsColor.create( devDevice, SENSOR_COLOR ) )
    {
        cout << "Cannot create color stream on device: " << OpenNI::getExtendedError() << endl;
        return 1;
    }
    else
    {
        mode.setPixelFormat( PIXEL_FORMAT_RGB888 );
        if ( STATUS_OK != vsColor.setVideoMode( mode ) )
        {
            cout << "Cannot set color mode: " << OpenNI::getExtendedError() << endl;
            return 1;
        }
    }
#endif

    Recorder recRecorder;
    VideoFrameRef depth_frame;
    int frameStartIndex = 0;
    if ( STATUS_OK != recRecorder.create( RECORDING_FILE_NAME ) )
    {
        cout << "Cannot create recoder: " << OpenNI::getExtendedError() << endl;
    }
    else
    {
        recRecorder.attach( vsDepth );
#ifndef DEVICE_M3
        recRecorder.attach( vsColor );
#endif
        vsDepth.start();
#ifndef DEVICE_M3
        vsColor.start();
#endif
        recRecorder.start();
        while ( true )
        {
            if ( STATUS_OK != vsDepth.readFrame( &depth_frame ) )
            {
                cout << "Cannot read depth frame: " << OpenNI::getExtendedError() << endl;
                break;
            }
            else
            {
                if ( frameStartIndex == 0 )
                {
                    frameStartIndex = depth_frame.getFrameIndex();
                    cout << "Start index: " << frameStartIndex << endl;
                }
                else
                {
                    if ( ( depth_frame.getFrameIndex() - frameStartIndex ) > RECORDING_FRAME )
                    {
                        recRecorder.stop();
                        vsDepth.stop();
#ifndef DEVICE_M3
                        vsColor.stop();
#endif
                        break;
                    }
                    else
                    {
                        cout << "Current index: " << depth_frame.getFrameIndex() << endl; 
                    }
                }
            }
        }
        recRecorder.destroy();
        vsDepth.destroy();
#ifndef DEVICE_M3
        vsColor.destroy();
#endif
    }
    devDevice.close();
    OpenNI::shutdown();

    return 0;
}
