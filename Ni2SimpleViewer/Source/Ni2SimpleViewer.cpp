// STL Headers
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
// OpenNI2 Headers
#include <OpenNI.h>
// OpenCV Headers
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace openni;
using namespace cv;

#define FPS_AVG_COUNT 120

#define RES_VGA_X 640
#define RES_VGA_Y 480

#define RES_QVGA_X 320
#define RES_QVGA_Y 240

#define RES_QQVGA_X 160
#define RES_QQVGA_Y 120

#define RES_QQQVGA_X 80
#define RES_QQQVGA_Y 60

#define RES_1080P_X 1920
#define RES_1080P_Y 1080

#define RES_720P_X 1280
#define RES_720P_Y 720

#define RES_FHD_X 2592
#define RES_FHD_Y 1944

#define RES_960P_X 1280
#define RES_960P_Y 960

enum showOp
{
    DEPTH = 1,
    IMAGE = 2,
    IR = 4
};

int Xres = 0;
int Yres = 0;
unsigned short Zres = 0;

void onMouse( int Event, int x, int y, int flags, void* param )
{
    if ( Event == EVENT_MOUSEMOVE )
    {
        Xres = x;
        Yres = y;
    }
}

int getUserInput()
{
    int option = 0;
    std::cout << endl;
    std::cout << "1) Depth only" << endl;
    std::cout << "2) Image only" << endl;
    std::cout << "3) IR only" << endl;
    std::cout << "4) Depth and Image" << endl;
    std::cout << "5) Depth and IR" << endl;
    std::cout << "6) Image and IR" << endl;
    std::cout << "7) All" << endl;
    std::cout << "0) Exit" << endl;
    std::cout << "Please input your choice : ";
    std::cin >> option;
    switch ( option )
    {
        case 1:
            return DEPTH;
        case 2:
            return IMAGE;
        case 3:
            return IR;
        case 4:
            return ( DEPTH + IMAGE );
        case 5:
            return ( DEPTH + IR );
        case 6:
            return ( IMAGE + IR );
        case 7:
            return ( DEPTH + IMAGE + IR );
        case 0:
            return -1;
        default:
            return getUserInput();
    };
}

double calcFPS( int64 startTick, uint64 count )
{
    if ( count < FPS_AVG_COUNT )
    {
        return 0.0f;
    }
    else
    {
        double execute_time = ( double )( cv::getTickCount() - startTick ) / cv::getTickFrequency();
        return ( count / execute_time );
    }
}

void showResolution ( const VideoMode* mapMode )
{
    int xRes = mapMode->getResolutionX();
    int yRes = mapMode->getResolutionY();
    int fps = mapMode->getFps();

    if ( RES_VGA_X == xRes && RES_VGA_Y == yRes )
    {
        std::cout << "VGA ( 640 x 480 ), FPS = " << fps << endl;
    }
    else if ( RES_QVGA_X == xRes && RES_QVGA_Y == yRes )
    {
        std::cout << "QVGA ( 320 x 240 ), FPS = " << fps << endl;
    }
    else if ( RES_QQVGA_X == xRes && RES_QQVGA_Y == yRes )
    {
        std::cout << "QQVGA ( 160 x 120 ), FPS = " << fps << endl;
    }
    else if ( RES_QQQVGA_X == xRes && RES_QQQVGA_Y == yRes )
    {
        std::cout << "QQQVGA ( 80 x 60 ), FPS = " << fps << endl;
    }
    else if ( RES_1080P_X == xRes && RES_1080P_Y == yRes )
    {
        std::cout << "1080P ( 1920 x 1080 ), FPS = " << fps << endl;
    }
    else if ( RES_720P_X == xRes && RES_720P_Y == yRes )
    {
        std::cout << "720P ( 1280 x 720 ), FPS = " << fps << endl;
    }
    else if ( RES_FHD_X == xRes && RES_FHD_Y == yRes )
    {
        std::cout << "FHD ( 2592 x 1944 ), FPS = " << fps << endl;
    }
    else if ( RES_960P_X == xRes && RES_960P_Y == yRes )
    {
        std::cout << "960P ( 1280 x 960 ), FPS = " << fps << endl;
    }
    else
    {
        std::cout << "Unknown ( " << xRes << " x " << yRes << " ), FPS = " << fps << endl;
    }
}

void getResolutionSetting( int query_option, VideoStream* depthData, VideoStream* imageData, VideoStream* irData,
                           VideoMode* depthOutputMode, VideoMode* imageOutputMode, VideoMode* irOutputMode, Device *device )
{
    unsigned int depthModeCount = 0;
    unsigned int imageModeCount = 0;
    unsigned int irModeCount = 0;

    if ( query_option & DEPTH )
    {
        const SensorInfo* sinfo = device->getSensorInfo( SENSOR_DEPTH );
        const Array<VideoMode>& modesDepth = sinfo->getSupportedVideoModes();
        unsigned int answer = 1;
        std::cout << endl;
        if ( 1 > modesDepth.getSize() )
        {
            std::cout << "[Device] This device does not support Depth." << endl;
        }
        else
        {
            std::cout << "Available options for Depth :" << endl;
            for ( int i = 0; i < modesDepth.getSize(); i++ )
            {
                std::cout << i + 1 << ") ";
                showResolution( &modesDepth[i] );
            }
            cout << "Please select resolution and FPS for Depth: ";
            std::cin >> answer;
            if ( answer > modesDepth.getSize() )
            {
                std::cout << "User input wrong parameter, using default video output mode." << endl;
                answer = 1;
            }
            depthOutputMode->setFps( modesDepth[answer - 1].getFps() );
            depthOutputMode->setResolution( modesDepth[answer - 1].getResolutionX(), modesDepth[answer - 1].getResolutionY() );
            depthOutputMode->setPixelFormat( modesDepth[answer - 1].getPixelFormat() );
        }
    }
    if ( query_option & IMAGE )
    {
        const SensorInfo* sinfo = device->getSensorInfo( SENSOR_COLOR );
        const Array<VideoMode>& modesImage = sinfo->getSupportedVideoModes();
        unsigned int answer = 1;
        std::cout << endl;
        if ( 1 > modesImage.getSize() )
        {
            std::cout << "[Device] This device does not support Color." << endl;
        }
        else
        {
            std::cout << "Available options for COLOR :" << endl;
            for ( int i = 0; i < modesImage.getSize(); i++ )
            {
                std::cout << i + 1 << ") ";
                showResolution( &modesImage[i] );
            }
            cout << "Please select resolution and FPS for COLOR: ";
            std::cin >> answer;
            if ( answer > modesImage.getSize() )
            {
                std::cout << "User input wrong parameter, using default video output mode." << endl;
                answer = 1;
            }
            imageOutputMode->setFps( modesImage[answer - 1].getFps() );
            imageOutputMode->setResolution( modesImage[answer - 1].getResolutionX(), modesImage[answer - 1].getResolutionY() );
            imageOutputMode->setPixelFormat( modesImage[answer - 1].getPixelFormat() );
        }
    }
    if ( query_option & IR )
    {
        const SensorInfo* sinfo = device->getSensorInfo( SENSOR_IR );
        const Array<VideoMode>& modesIR = sinfo->getSupportedVideoModes();
        unsigned int answer = 1;
        std::cout << endl;
        if ( 1 > modesIR.getSize() )
        {
            std::cout << "[Device] This device do not support IR." << endl;
        }
        else
        {
            std::cout << "Available options for IR :" << endl;
            for ( int i = 0; i < modesIR.getSize(); i++ )
            {
                std::cout << i + 1 << ") ";
                showResolution( &modesIR[i] );
            }
            cout << "Please select resolution and FPS for IR: ";
            std::cin >> answer;
            if ( answer > modesIR.getSize() )
            {
                std::cout << "User input wrong parameter, using default video output mode." << endl;
                answer = 1;
            }
            irOutputMode->setFps( modesIR[answer - 1].getFps() );
            irOutputMode->setResolution( modesIR[answer - 1].getResolutionX(), modesIR[answer - 1].getResolutionY() );
            irOutputMode->setPixelFormat( modesIR[answer - 1].getPixelFormat() );
        }
    }
}

int main( int argc, char* argv[] )
{
    int option = getUserInput();

    if ( -1 == option )
    {
        std::cout << "Exit program!" << endl;
        return 0;
    }

    if ( STATUS_OK != OpenNI::initialize() )
    {
        std::cout << "After initialization: " << OpenNI::getExtendedError() << endl;
        return 1;
    }

    Device devDevice;
    if ( STATUS_OK != devDevice.open( ANY_DEVICE ) )
    {
        std::cout << "Cannot open device: " << OpenNI::getExtendedError() << endl;
        return 1;
    }

    VideoMode mode, mapDepthOutputMode, mapImageOutputMode, mapIrOutputMode;
    VideoStream vsDepth;
    VideoStream vsColor;
    VideoStream vsIR;

    VideoFrameRef depth_frame;
    VideoFrameRef color_frame;
    VideoFrameRef ir_frame;

    uint64 depth_frame_count = 0;
    uint64 color_frame_count = 0;
    uint64 ir_frame_count = 0;

    getResolutionSetting( option, &vsDepth, &vsColor, &vsIR, &mapDepthOutputMode, &mapImageOutputMode, &mapIrOutputMode, &devDevice );
    if ( option & DEPTH )
    {
        if ( STATUS_OK != vsDepth.create( devDevice, SENSOR_DEPTH ) )
        {
            std::cout << "Cannot create depth stream on device: " << OpenNI::getExtendedError() << endl;
            return 1;
        }
        vsDepth.setVideoMode( mapDepthOutputMode );
        std::cout << "Depth video mode - FPS=" << vsDepth.getVideoMode().getFps() << ", X=" << vsDepth.getVideoMode().getResolutionX() << ", Y=" << vsDepth.getVideoMode().getResolutionY() << endl;
        vsDepth.start();
    }
    if ( option & IMAGE )
    {
        if ( STATUS_OK != vsColor.create( devDevice, SENSOR_COLOR ) )
        {
            std::cout << "Cannot create color stream on device: " << OpenNI::getExtendedError() << endl;
            return 1;
        }
        vsColor.setVideoMode( mapImageOutputMode );
        std::cout << "Color video mode - FPS=" << vsColor.getVideoMode().getFps() << ", X=" << vsColor.getVideoMode().getResolutionX() << ", Y=" << vsColor.getVideoMode().getResolutionY() << endl;
        vsColor.start();
    }
    if ( option & IR )
    {
        if ( STATUS_OK != vsIR.create( devDevice, SENSOR_IR ) )
        {
            std::cout << "Cannot create IR stream on device: " << OpenNI::getExtendedError() << endl;
            return 1;
        }
        vsIR.setVideoMode( mapIrOutputMode );
        std::cout << "IR video mode - FPS=" << vsIR.getVideoMode().getFps() << ", X=" << vsIR.getVideoMode().getResolutionX() << ", Y=" << vsIR.getVideoMode().getResolutionY() << endl;
        vsIR.start();
    }

    bool quit = false;
    bool capture = false;
    bool showText = true;
    bool mirror = false;
    bool fullScreen = false;
    vector<int> quality;
    quality.push_back( CV_IMWRITE_PNG_COMPRESSION );
    quality.push_back( 0 );
    int64 tStart = cv::getTickCount();

    if ( option & DEPTH )
    {
        namedWindow( "Depth view", CV_WINDOW_NORMAL );
    }
    if ( option & IMAGE )
    {
        namedWindow( "Color view", CV_WINDOW_NORMAL );
    }
    if ( option & IR )
    {
        namedWindow( "IR view", CV_WINDOW_NORMAL );
    }

    while ( true )
    {
        if ( fullScreen )
        {
            if ( option & DEPTH )
            {
                setWindowProperty( "Depth view", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN );
            }
            if ( option & IMAGE )
            {
                setWindowProperty( "Color view", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN );
            }
            if ( option & IR )
            {
                setWindowProperty( "IR view", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN );
            }
        }
        else
        {
            if ( option & DEPTH )
            {
                setWindowProperty( "Depth view", CV_WND_PROP_FULLSCREEN, 0 );
            }
            if ( option & IMAGE )
            {
                setWindowProperty( "Color view", CV_WND_PROP_FULLSCREEN, 0 );
            }
            if ( option & IR )
            {
                setWindowProperty( "IR view", CV_WND_PROP_FULLSCREEN, 0 );
            }
        }

        if ( option & DEPTH )
        {
            if ( vsDepth.isValid() )
            {
                if ( STATUS_OK == vsDepth.readFrame( &depth_frame ) )
                {
                    Mat imgDepth ( depth_frame.getHeight(), depth_frame.getWidth(), CV_16UC1, ( void* )depth_frame.getData() );
                    Mat img8bitDepth;
                    Mat img8bit3ChDepth;
                    Mat img8bit3ChMask = Mat( depth_frame.getHeight(), depth_frame.getWidth(), CV_8UC3, Scalar( 0, 255, 255 ) );
                    imgDepth.convertTo( img8bitDepth, CV_8U, 255.0 / 10000 );
                    cvtColor( img8bitDepth, img8bit3ChDepth, CV_GRAY2BGR );
                    cv::bitwise_and( img8bit3ChDepth, img8bit3ChMask, img8bit3ChDepth );
                    if ( mirror )
                    {
                        flip( img8bit3ChDepth, img8bit3ChDepth, 1 );
                    }

                    // Show FPS, X, Y, Depth value
                    if ( showText )
                    {
                        double fps = calcFPS( tStart, ++depth_frame_count );
                        stringstream streamFPS;
                        streamFPS << "FPS: " << fixed << setprecision( 2 ) << fps;
                        putText( img8bit3ChDepth, streamFPS.str(), Point( 5, 20 ), FONT_HERSHEY_DUPLEX, ( depth_frame.getWidth() > 320 ) ? 1.0 : 0.5, Scalar( 255, 255, 255 ) );

                        Zres = imgDepth.at<ushort>( Yres, Xres );
                        stringstream streamXYZ;
                        streamXYZ << "X : " << Xres << ", Y : " << Yres << ", Depth : " << Zres;
                        putText( img8bit3ChDepth, streamXYZ.str(), Point( 5, 50 ), FONT_HERSHEY_DUPLEX, ( depth_frame.getWidth() > 320 ) ? 1.0 : 0.5, Scalar( 255, 255, 255 ) );
                    }

                    imshow( "Depth view", img8bit3ChDepth );
                    if ( capture )
                    {
                        imwrite( "depth_" + std::to_string( depth_frame.getFrameIndex() ) + ".png", img8bit3ChDepth, quality );
                    }
                }
                setMouseCallback( "Depth view", onMouse, NULL );
            }
        }

        if ( option & IMAGE )
        {
            if ( vsColor.isValid() )
            {
                if ( STATUS_OK == vsColor.readFrame( &color_frame ) )
                {
                    Mat imgColor( color_frame.getHeight(), color_frame.getWidth(), CV_8UC3, ( void* )color_frame.getData() );
                    Mat imgBGRColor;
                    cvtColor( imgColor, imgBGRColor, CV_RGB2BGR );
                    if ( mirror )
                    {
                        flip( imgBGRColor, imgBGRColor, 1 );
                    }

                    // Show FPS
                    if ( showText )
                    {
                        double fps = calcFPS( tStart, ++color_frame_count );
                        stringstream stream;
                        stream << "FPS: " << fixed << setprecision( 2 ) << fps;
                        putText( imgBGRColor, stream.str(), Point( 5, 20 ), FONT_HERSHEY_DUPLEX, ( color_frame.getWidth() > 320 ) ? 1.0 : 0.5, Scalar( 255, 255, 255 ) );
                    }

                    imshow( "Color view", imgBGRColor );
                    if ( capture )
                    {
                        imwrite( "image_" + std::to_string( color_frame.getFrameIndex() ) + ".png", imgBGRColor, quality );
                    }
                }
            }
        }

        if ( option & IR )
        {
            if ( vsIR.isValid() )
            {
                if ( STATUS_OK == vsIR.readFrame( &ir_frame ) )
                {
                    Mat imgIR ( ir_frame.getHeight(), ir_frame.getWidth(), CV_16UC1, ( void* )ir_frame.getData() );
                    Mat img8bitIR;
                    imgIR.convertTo( img8bitIR, CV_8U, 255.0 / 4096 );
                    if ( mirror )
                    {
                        flip( img8bitIR, img8bitIR, 1 );
                    }

                    // Show FPS
                    if ( showText )
                    {
                        double fps = calcFPS( tStart, ++ir_frame_count );
                        stringstream stream;
                        stream << "FPS: " << fixed << setprecision( 2 ) << fps;
                        putText( img8bitIR, stream.str(), Point( 5, 20 ), FONT_HERSHEY_DUPLEX, ( ir_frame.getWidth() > 320 ) ? 1.0 : 0.5, Scalar( 255, 255, 255 ) );
                    }

                    imshow( "IR view", img8bitIR );
                    if ( capture )
                    {
                        imwrite( "ir_" + std::to_string( ir_frame.getFrameIndex() ) + ".png", img8bitIR, quality );
                    }
                }
            }
        }

        char keyInput = waitKey( 1 );
        if ( keyInput != -1 )
        {
            switch ( keyInput )
            {
                case 'Q': // Q = 81
                case 'q': // q = 113
                    //q for exit
                    quit = true;
                    break;
                case 'C': // C = 67
                case 'c': // c = 99
                    // depth
                    capture = true;
                    break;
                case 'F': // F = 70
                case 'f': // f = 102
                    showText = ( showText ) ? false : true;
                    break;
                case 'W':
                case 'w':
                    fullScreen = ( fullScreen ) ? false : true;
                    break;
                default:
                    break;
            }
        }
        else
        {
            capture = false;
        }

        if ( quit )
        {
            break;
        }
    }

    if ( option & DEPTH )
    {
        vsDepth.destroy();
    }
    if ( option & IMAGE )
    {
        vsColor.destroy();
    }
    if ( option & IR )
    {
        vsIR.destroy();
    }

    devDevice.close();
    OpenNI::shutdown();
    destroyAllWindows();

    return 0;
}
