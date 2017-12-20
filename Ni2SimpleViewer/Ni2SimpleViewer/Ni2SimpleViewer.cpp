// Ni2SimpleViewer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
using namespace cv;

#if OPENNI_VER==1
using namespace xn;
#else
using namespace openni;
#endif

#ifndef FPS_AVG_COUNT
#define FPS_AVG_COUNT 120
#endif
#ifndef XN_VGA_X_RES
#define XN_VGA_X_RES 640
#endif
#ifndef XN_VGA_Y_RES
#define XN_VGA_Y_RES 480
#endif
#ifndef XN_QVGA_X_RES
#define XN_QVGA_X_RES 320
#endif
#ifndef XN_QVGA_Y_RES
#define XN_QVGA_Y_RES 240
#endif
#ifndef XN_QQVGA_X_RES
#define XN_QQVGA_X_RES 160
#endif
#ifndef XN_QQVGA_Y_RES
#define XN_QQVGA_Y_RES 120
#endif
#ifndef XN_1080P_X_RES
#define XN_1080P_X_RES 1920
#endif
#ifndef XN_1080P_Y_RES
#define XN_1080P_Y_RES 1080
#endif
#ifndef XN_720P_X_RES
#define XN_720P_X_RES 1280
#endif
#ifndef XN_720P_Y_RES
#define XN_720P_Y_RES 720
#endif

enum showOp
{
    DEPTH = 1,
    IMAGE = 2,
    IR = 4
};

void onMouse( int Event, int x, int y, int flags, void* param );
int Xres = 0;
int Yres = 0;
UINT16 Zres = 0;

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
    system( "cls" );
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
            break;
        case 2:
            return IMAGE;
            break;
        case 3:
            return IR;
            break;
        case 4:
            return ( DEPTH + IMAGE );
            break;
        case 5:
            return ( DEPTH + IR );
            break;
        case 6:
            return ( IMAGE + IR );
            break;
        case 7:
            return ( DEPTH + IMAGE + IR );
            break;
        case 0:
            return -1;
            break;
        default:
            return getUserInput();
            break;
    };
}

#if OPENNI_VER==2
double calcFPS( int64 startTick, uint64 count )
{
    double fps = 0.0;
    if ( count < FPS_AVG_COUNT )
    {
        return 0.0f;
    } else {
        double execute_time = (double)(cv::getTickCount() - startTick)/cv::getTickFrequency();
        return count/execute_time;
    }
    return fps;
}
#endif

#if OPENNI_VER==1
void showResolution ( XnMapOutputMode* mapMode )
#else
void showResolution ( const openni::VideoMode* mapMode )
#endif
{
#if OPENNI_VER==1
    uint32_t xRes = mapMode->nXRes;
    uint32_t yRes = mapMode->nYRes;
    uint32_t fps = mapMode->nFPS;
#else
    int xRes = mapMode->getResolutionX();
    int yRes = mapMode->getResolutionY();
    int fps = mapMode->getFps();
#endif
    if ( XN_VGA_X_RES == xRes && XN_VGA_Y_RES == yRes )
    {
        std::cout << "VGA ( 640 x 480 ), FPS = " << fps << endl;
    }
    else if ( XN_QVGA_X_RES == xRes && XN_QVGA_Y_RES == yRes )
    {
        std::cout << "QVGA ( 320 x 240 ), FPS = " << fps << endl;
    }
    else if ( XN_QQVGA_X_RES == xRes && XN_QQVGA_Y_RES == yRes )
    {
        std::cout << "QQVGA ( 160 x 120 ), FPS = " << fps << endl;
    }
    else if ( 80 == xRes && 60 ==yRes )
    {
        std::cout << "QQQVGA ( 80 x 60 ), FPS = " <<fps << endl;
    }
    else if ( XN_1080P_X_RES == xRes && XN_1080P_Y_RES == yRes )
    {
        std::cout << "1080P ( 1920 x 1080 ), FPS = " << fps << endl;
    }
    else if ( XN_720P_X_RES == xRes && XN_720P_Y_RES == yRes )
    {
        std::cout << "720P ( 1280 x 720 ), FPS = " << fps << endl;
    }
    else if ( 2592 == xRes && 1944 == yRes )
    {
        std::cout << "FHD ( 2592 x 1944 ), FPS = " << fps << endl;
    }
    else if ( 1280 ==xRes && 960 == yRes )
    {
        std::cout << "960P ( 1280 x 960 ), FPS = " << fps << endl;
    }
    else
    {
        std::cout << "Unknown ( " << xRes << " x " << yRes << " ), FPS = " <<fps << endl;
    }
}

void getResolutionSetting(
#if OPENNI_VER==1
    int query_option, DepthGenerator* depthData, ImageGenerator* imageData, IRGenerator* irData,
    XnMapOutputMode* depthOutputMode, XnMapOutputMode* imageOutputMode, XnMapOutputMode* irOutputMode
#else
    int query_option, openni::VideoStream* depthData, openni::VideoStream* imageData, openni::VideoStream* irData,
    VideoMode* depthOutputMode, VideoMode* imageOutputMode, VideoMode* irOutputMode, Device *device
#endif
)
{
    unsigned int depthModeCount = 0;
    unsigned int imageModeCount = 0;
    unsigned int irModeCount = 0;

    system( "cls" );
#if OPENNI_VER==1
    XnStatus status;
#endif
#if OPENNI_VER==1
    if ( query_option & DEPTH )
    {
        depthModeCount = depthData->GetSupportedMapOutputModesCount();
        if ( depthModeCount < 1 )
        {
            cout << "[Depth] Device doesn't support depth" << endl;
        }
        else
        {
            XnMapOutputMode* depthModes = new XnMapOutputMode[depthModeCount];
            status = depthData->GetSupportedMapOutputModes( depthModes, depthModeCount );
            if ( XN_STATUS_OK != status )
            {
                cout << "[Depth] GetSupportedMapOutputModes fail" << endl;
            }
            else
            {
                unsigned int answer = 1;
                unsigned int i;
                cout << endl << endl;
                cout << "Available options for DEPTH : " << endl;
                for ( i = 0; i < depthModeCount; i++ )
                {
                    cout << i + 1 << ") ";
                    showResolution ( &depthModes[i] );
                }
                cout << ++i << ") Use SDK default " << endl;
                cout << "Please select resolution and FPS for Depth : ";
                cin >> answer;
                if ( answer > depthModeCount )
                {
                    answer = 1;
                }
                depthOutputMode->nXRes = depthModes[answer - 1].nXRes;
                depthOutputMode->nYRes = depthModes[answer - 1].nYRes;
                depthOutputMode->nFPS = depthModes[answer - 1].nFPS;
            }
            delete [] depthModes;
        }
    }
    if ( query_option & IMAGE )
    {
        imageModeCount = imageData->GetSupportedMapOutputModesCount();
        if ( imageModeCount < 1 )
        {
            cout << "[Image] Device doesn't support RGB" << endl;
        }
        else
        {
            XnMapOutputMode* imageModes;
            imageModes = new XnMapOutputMode[imageModeCount];
            status = imageData->GetSupportedMapOutputModes( imageModes, imageModeCount );
            if ( XN_STATUS_OK != status )
            {
                cout << "[Image] GetSupportedMapOutputModes fail" << endl;
            }
            else
            {
                unsigned int answer = 1;
                unsigned int i;
                cout << endl << endl;
                cout << "Available options for IMAGE : " << endl;
                for ( i = 0; i < imageModeCount; i++ )
                {
                    cout << i + 1 << ") ";
                    showResolution ( &imageModes[i] );
                }
                cout << ++i << ") Use SDK default " << endl;
                cout << "Please select resolution and FPS for IMAGE : ";
                cin >> answer;
                if ( answer > imageModeCount )
                {
                    answer = 1;
                }
                imageOutputMode->nXRes = imageModes[answer - 1].nXRes;
                imageOutputMode->nYRes = imageModes[answer - 1].nYRes;
                imageOutputMode->nFPS = imageModes[answer - 1].nFPS;
            }
            delete [] imageModes;
        }
    }
    if ( query_option & IR )
    {
        irModeCount = irData->GetSupportedMapOutputModesCount();
        if ( irModeCount < 1 )
        {
            cout << "[IR] Device doesn't support IR" << endl;
        }
        else
        {
            XnMapOutputMode* irModes;
            irModes = new XnMapOutputMode[irModeCount];
            status = irData->GetSupportedMapOutputModes( irModes, irModeCount );
            if ( XN_STATUS_OK != status )
            {
                cout << "[IR] GetSupportedMapOutputModes fail" << endl;
            }
            else
            {
                unsigned int answer = 1;
                unsigned int i;
                cout << endl << endl;
                cout << "Available options for IR : " << endl;
                for ( i = 0; i < irModeCount; i++ )
                {
                    cout << i + 1 << ") ";
                    showResolution ( &irModes[i] );
                }
                cout << ++i << ") Use SDK default " << endl;
                cout << "Please select resolution and FPS for IR : ";
                cin >> answer;
                if ( answer > irModeCount )
                {
                    answer = 1;
                }
                irOutputMode->nXRes = irModes[answer - 1].nXRes;
                irOutputMode->nYRes = irModes[answer - 1].nYRes;
                irOutputMode->nFPS = irModes[answer - 1].nFPS;
            }
            delete [] irModes;
        }
    }
#else
    if ( query_option & DEPTH )
    {
        const openni::SensorInfo* sinfo = device->getSensorInfo( openni::SENSOR_DEPTH );
        const openni::Array< openni::VideoMode >& modesDepth = sinfo->getSupportedVideoModes();
        unsigned int answer = 1;
        system( "CLS" );
        if ( 1 > modesDepth.getSize() )
        {
            std::cout << "[Device] This device do not support Depth." << endl;
        }
        else
        {
            std::cout << "Available options for Depth : " << endl;
            for ( int i = 0; i < modesDepth.getSize(); i++ )
            {
                std::cout << i + 1 << ") ";
                showResolution( &modesDepth[i] );
            }
            std::cin >> answer;
            if ( answer > modesDepth.getSize() )
            {
                std::cout << "User input wrong parameter, using default video output mode." << endl;
                answer = 1;
            }
            depthOutputMode->setFps( modesDepth[answer - 1 ].getFps() );
            depthOutputMode->setResolution( modesDepth[answer - 1 ].getResolutionX(), modesDepth[answer - 1 ].getResolutionY() );
            depthOutputMode->setPixelFormat( modesDepth[answer - 1].getPixelFormat() );
        }
    }
    if ( query_option & IMAGE )
    {
        const openni::SensorInfo* sinfo = device->getSensorInfo( openni::SENSOR_COLOR );
        const openni::Array< openni::VideoMode>& modesImage = sinfo->getSupportedVideoModes();
        unsigned int answer = 1;
        system( "cls" );
        if ( 1 > modesImage.getSize() )
        {
            std::cout << "[Device] This device do not support Image." << endl;
        }
        else
        {
            std::cout << "Available options for IMAGE : " << endl;
            for ( int i = 0; i < modesImage.getSize(); i++ )
            {
                std::cout << i + 1 << ") ";
                showResolution( &modesImage[i] );
            }
            std::cin >> answer;
            if ( answer > modesImage.getSize() )
            {
                std::cout << "User input wrong parameter, using default video output mode." << endl;
                answer = 1;
            }
            imageOutputMode->setFps( modesImage[answer - 1 ].getFps() );
            imageOutputMode->setResolution( modesImage[answer - 1].getResolutionX(), modesImage[answer - 1].getResolutionY() );
            imageOutputMode->setPixelFormat( modesImage[answer - 1].getPixelFormat() );
        }
    }
    if ( query_option & IR )
    {
        const openni::SensorInfo* sinfo = device->getSensorInfo( openni::SENSOR_IR );
        const openni::Array< openni::VideoMode>& modesIR = sinfo->getSupportedVideoModes();
        unsigned int answer = 1;
        system( "cls" );
        if ( 1 > modesIR.getSize() )
        {
            std::cout << "[Device] This device do not support Image." << endl;
        }
        else
        {
            std::cout << "Available options for IR : " << endl;
            for ( int i = 0; i < modesIR.getSize(); i++ )
            {
                std::cout << i + 1 << ") ";
                showResolution( &modesIR[i] );
            }
            std::cin >> answer;
            if ( answer > modesIR.getSize() )
            {
                std::cout << "User input wrong parameter, using default video output mode." << endl;
                answer = 1;
            }
            irOutputMode->setFps( modesIR[answer - 1 ].getFps() );
            irOutputMode->setResolution( modesIR[answer - 1].getResolutionX(), modesIR[answer - 1].getResolutionY() );
            irOutputMode->setPixelFormat( modesIR[answer - 1].getPixelFormat() );
        }
    }
#endif
}

int _tmain( int argc, _TCHAR * argv[] )
{
    int option = getUserInput();

    if ( -1 == option )
    {
        std::cout << "Exit program!" << endl;
        return 0;
    }

#if OPENNI_VER==1
    Context mContext;

    DepthGenerator vsDepth;
    ImageGenerator vsColor;
    IRGenerator vsIr;

    DepthMetaData depth_frame;
    ImageMetaData color_frame;
    IRMetaData ir_frame;

    XnFPSData xnDepthFPS, xnColorFPS, xnIrFPS;

    XnMapOutputMode mapDepthOutputMode;
    XnMapOutputMode mapImageOutputMode;
    XnMapOutputMode mapIrOutputMode;

    mContext.Init();
#else
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
#endif


#if OPENNI_VER==1
    getResolutionSetting( option, &vsDepth, &vsColor, &vsIr, &mapDepthOutputMode, &mapImageOutputMode, &mapIrOutputMode );

    if ( option & DEPTH )
    {
        vsDepth.Create( mContext );
        xnFPSInit( &xnDepthFPS, FPS_AVG_COUNT );
        vsDepth.SetMapOutputMode( mapDepthOutputMode );
    }
    if ( option & IMAGE )
    {
        vsColor.Create( mContext );
        xnFPSInit( &xnColorFPS, FPS_AVG_COUNT );
        vsColor.SetMapOutputMode( mapImageOutputMode );
    }
    if ( option & IR )
    {
        vsIr.Create( mContext );
        xnFPSInit( &xnIrFPS, FPS_AVG_COUNT );
        vsIr.SetMapOutputMode( mapIrOutputMode );
    }
    mContext.StartGeneratingAll();
#else
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
        std::cout << "Image video mode - FPS=" << vsColor.getVideoMode().getFps() << ", X=" << vsColor.getVideoMode().getResolutionX() << ", Y=" << vsColor.getVideoMode().getResolutionY() << endl;
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
        std::cout << "IR video mode - FPS=" << mapIrOutputMode.getFps() << ", X=" << mapIrOutputMode.getResolutionX() << ", Y=" << mapIrOutputMode.getResolutionY() << endl;
        vsIR.start();
    }
#endif
    char fpsstr[7];
    char xyzstr[128];
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
#if OPENNI_VER==1
        mContext.WaitAndUpdateAll();
#endif
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
#if OPENNI_VER==1
            vsDepth.GetMetaData( depth_frame );
            Mat imgDepth( depth_frame.FullYRes(), depth_frame.FullXRes(), CV_16UC1, ( void* )depth_frame.Data() );
            Mat img8bitDepth;
            Mat img8bit3ChDepth;
            Mat img8bit3ChMask = Mat( depth_frame.FullYRes(), depth_frame.FullXRes(), CV_8UC3, Scalar( 0, 255, 255 ) );
            imgDepth.convertTo( img8bitDepth, CV_8U, 255.0 / 10000 );
            cvtColor( img8bitDepth, img8bit3ChDepth, CV_GRAY2BGR );
            bitwise_and( img8bit3ChDepth, img8bit3ChMask, img8bit3ChDepth );
            xnFPSMarkFrame( &xnDepthFPS );
            sprintf_s( fpsstr, sizeof( fpsstr ), "%.2f", xnFPSCalc( &xnDepthFPS ) );
            if ( mirror )
            {
                flip( img8bitDepth, img8bitDepth, 1 );
            }
            if ( showText )
            {
                putText( img8bit3ChDepth, string( "FPS:" ) + fpsstr, Point( 5, 20 ), FONT_HERSHEY_DUPLEX, ( depth_frame.FullXRes() > 320 ) ? 1.0 : 0.5, Scalar( 255, 255, 255 ) );
            }
            Zres = depth_frame( Xres, Yres );
            sprintf_s( xyzstr, sizeof( xyzstr ), "X : %d, Y : %d, Depth : %u", Xres, Yres, Zres );
            if ( showText )
            {
                putText( img8bit3ChDepth, xyzstr, Point( 5, 50 ), FONT_HERSHEY_DUPLEX, ( depth_frame.FullXRes() > 320 ) ? 1.0 : 0.5, Scalar( 255, 255, 255 ) );
            }
            imshow( "Depth view", img8bit3ChDepth );
            if ( capture )
            {
                imwrite( "depth_" + std::to_string( depth_frame.FrameID() ) + ".png", img8bit3ChDepth, quality );
            }
            cv::setMouseCallback( "Depth view", onMouse, NULL );
#else
            if ( vsDepth.isValid() )
            {
                if ( STATUS_OK == vsDepth.readFrame( &depth_frame ) )
                {
                    Mat imgDepth ( depth_frame.getHeight(), depth_frame.getWidth(), CV_16UC1, ( void* )depth_frame.getData() );
                    Mat img8bitDepth;
                    Mat img8bit3ChDepth;
                    Mat img8bit3ChMask = Mat( depth_frame.getHeight(), depth_frame.getWidth(), CV_8UC3, Scalar( 0, 255, 255 ) );
                    imgDepth.convertTo( img8bitDepth, CV_8U, 255.0 / 10000 );
                    cv::cvtColor( img8bitDepth, img8bit3ChDepth, CV_GRAY2BGR );
                    cv::bitwise_and( img8bit3ChDepth, img8bit3ChMask, img8bit3ChDepth );
                    if ( mirror )
                    {
                        flip( img8bit3ChDepth, img8bit3ChDepth, 1 );
                    }
                    // Show FPS
                    /*
                    float fps = 0.0f;
                    if ( depth_frame.getFrameIndex() > FPG_AVG_COUNT )
                    {
                        cout << "frame index: " << depth_frame.getFrameIndex() << endl;
                        float delta_time =  1000.0f * ( getTickCount() - tStart ) / getTickFrequency();
                        fps = 1000.0f * depth_frame.getFrameIndex() / delta_time;
                    }*/
                    double fps = calcFPS( tStart, ++depth_frame_count );
                    if ( showText )
                    {
                        stringstream stream;
                        stream << "FPS: " << fixed << setprecision( 2 ) << fps;
                        putText( img8bit3ChDepth, stream.str(), Point( 5, 20 ), FONT_HERSHEY_DUPLEX, ( depth_frame.getWidth() > 320 ) ? 1.0 : 0.5, Scalar( 255, 255, 255 ) );
                    }

                    // Show X, Y, Depth value
                    if ( showText )
                    {
                        Zres = imgDepth.at<ushort>( Yres, Xres );
                        sprintf_s( xyzstr, sizeof( xyzstr ), "X : %d, Y : %d, Depth : %u", Xres, Yres, Zres );
                        putText( img8bit3ChDepth, xyzstr, Point( 5, 50 ), FONT_HERSHEY_DUPLEX, ( depth_frame.getWidth() > 320 ) ? 1.0 : 0.5, Scalar( 255, 255, 255 ) );
                    }
                    imshow( "Depth view", img8bit3ChDepth );
                    if ( capture )
                    {
                        imwrite( "depth_" + std::to_string( depth_frame.getFrameIndex() ) + ".png", img8bit3ChDepth, quality );
                    }
                }
                cv::setMouseCallback( "Depth view", onMouse, NULL );
            }
#endif
        }

        if ( option & IMAGE )
        {
#if OPENNI_VER==1
            vsColor.GetMetaData( color_frame );
            Mat imgColor( color_frame.FullYRes(), color_frame.FullXRes(), CV_8UC3, ( void* )color_frame.Data() );
            Mat imgBGRColor;
            cvtColor( imgColor, imgBGRColor, CV_RGB2BGR );
            xnFPSMarkFrame( &xnColorFPS );
            sprintf_s( fpsstr, sizeof( fpsstr ), "%.2f", xnFPSCalc( &xnColorFPS ) );
            if ( showText )
            {
                putText( imgBGRColor, string( "FPS:" ) + fpsstr, Point( 5, 20 ), FONT_HERSHEY_DUPLEX, ( color_frame.FullXRes() > 320 ) ? 1.0 : 0.5, Scalar( 200, 0, 0 ) );
            }
            imshow( "Color view", imgBGRColor );
            if ( capture )
            {
                imwrite( "image_" + std::to_string( color_frame.FrameID() ) + ".png", imgBGRColor, quality );
            }
#else
            if ( vsColor.isValid() )
            {
                if ( STATUS_OK == vsColor.readFrame( &color_frame ) )
                {
                    Mat imgColor( color_frame.getHeight(), color_frame.getWidth(), CV_8UC3, ( void* )color_frame.getData() );
                    Mat imgBGRColor;
                    cv::cvtColor( imgColor, imgBGRColor, CV_RGB2BGR );
                    if ( mirror )
                    {
                        flip( imgBGRColor, imgBGRColor, 1 );
                    }
                    // Show FPS
                    if ( showText )
                    {
                        /*float fps = 0.0f;
                        if ( color_frame.getFrameIndex() > FPG_AVG_COUNT )
                        {
                            float delta_time =  1000.0f * ( getTickCount() - tStart ) / getTickFrequency();
                            fps = 1000.0f * color_frame.getFrameIndex() / delta_time;
                        }*/
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
#endif
        }

        if ( option & IR )
        {
#if OPENNI_VER==1
            vsIr.GetMetaData( ir_frame );
            Mat imgIR( ir_frame.FullYRes(), ir_frame.FullXRes(), CV_16UC1, ( void * )ir_frame.Data() );
            Mat img8bitIR;
            imgIR.convertTo( img8bitIR, CV_8U, 255.0 / 4096 );
            xnFPSMarkFrame( &xnIrFPS );
            sprintf_s( fpsstr, sizeof( fpsstr ), "%.2f", xnFPSCalc( &xnIrFPS ) );
            if ( showText )
            {
                putText( img8bitIR, string( "FPS:" ) + fpsstr, Point( 5, 20 ), FONT_HERSHEY_DUPLEX, ( ir_frame.FullXRes() > 320 ) ? 1.0 : 0.5, Scalar( 200, 0, 0 ) );
            }
            imshow( "IR view", img8bitIR );
            if ( capture )
            {
                imwrite( "ir_" + std::to_string( ir_frame.FrameID() ) + ".png", img8bitIR, quality );
            }
#else
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
                        /*float fps = 0.0f;
                        if ( ir_frame.getFrameIndex() > FPG_AVG_COUNT )
                        {
                            float delta_time =  1000.0f * ( getTickCount() - tStart ) / getTickFrequency();
                            fps = 1000.0f * ir_frame.getFrameIndex() / delta_time;
                        }*/
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
#endif
        }
        int keyInput = waitKey( 1 );
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
#if OPENNI_VER==1
    mContext.StopGeneratingAll();
#endif
    if ( option & DEPTH )
    {
#if OPENNI_VER==1
        vsDepth.Release();
#else
        vsDepth.destroy();
#endif
    }
    if ( option & IMAGE )
    {
#if OPENNI_VER==1
        vsColor.Release();
#else
        vsColor.destroy();
#endif
    }
    if ( option & IR )
    {
#if OPENNI_VER==1
        vsIr.Release();
#else
        vsDepth.destroy();
        vsIR.destroy();
#endif
    }

#if OPENNI_VER==1
    mContext.Release();
    destroyAllWindows();
#else
    devDevice.close();
    OpenNI::shutdown();
#endif

    return 0;
}

