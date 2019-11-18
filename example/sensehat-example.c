// =================================================================================================
//
//  sensehat-example.c
//
//  Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
//
//  Supported host operating systems:
//      Raspbian Stretch or later
//
//  Description:
//      This program demonstrates the basics of using the Raspberry Pi Sense HAT C library.
//
//  Notes:
//      1)  Requires ANSI C99 (or better) compliant compilers.
//
// =================================================================================================
//  Includes
// =================================================================================================
#include "sensehat.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// =================================================================================================
//  Constants
// =================================================================================================

static const char* kColorCycleExampleCmd    = "--color-cycle-example";
static const char* kCompassExampleCmd       = "--compass-example";
static const char* kEnvironmentCmd          = "--environment";
static const char* kFlashCmd                = "--flash";
static const char* kHeadingCmd              = "--heading";
static const char* kHelpCmd                 = "--help";
static const char* kLoadImageCmd            = "--load-image";
static const char* kRainbowExampleCmd       = "--rainbow-example";
static const char* kRotationExampleCmd      = "--rotation-example";
static const char* kShowLetterCmd           = "--show-letter";
static const char* kShowMessageCmd          = "--show-message";
static const char* kTextScrollExampleCmd    = "--text-scroll-example";
static const char* kVersionCmd              = "--version";
static const char* kWaitForEventCmd         = "--wait-for-event";

#define kHelpCmdNum                 1
#define kVersionCmdNum              2
#define kColorCycleExampleCmdNum    3
#define kCompassExampleCmdNum       4
#define kRainbowExampleCmdNum       5
#define kRotationExampleCmdNum      6
#define kTextScrollExampleCmdNum    7
#define kFlashCmdNum                8
#define kLoadImageCmdNum            9
#define kRotateCmdNum               10
#define kShowLetterCmdNum           11
#define kShowMessageCmdNum          12
#define kFlipHorizontalCmdNum       13
#define kFlipVerticalCmdNum         14
#define kHeadingCmdNum              15
#define kEnvironmentCmdNum          16
#define kWaitForEventCmdNum         17
#define kQuitCmdNum                 18

static const char* kUpEvent     = "up";
static const char* kDownEvent   = "down";
static const char* kLeftEvent   = "left";
static const char* kRightEvent  = "right";
static const char* kPushEvent   = "push";

static const tSenseHAT_LEDPixel kTextColor  = {255,0,0};
static const tSenseHAT_LEDPixel kBackColor  = {0,0,0};

#define kRedColor       {255,0,0}
#define kGreenColor     {0,255,0}
#define kBlueColor      {0,0,255}
#define kPurpleColor    {255,0,255}

static const tSenseHAT_LEDPixelArray kRedArray = {
	kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor,
	kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor,
	kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor,
	kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor,
	kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor,
	kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor,
	kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor,
	kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor, kRedColor
};

static const tSenseHAT_LEDPixelArray kGreenArray = {
    kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, 
    kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, 
    kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, 
    kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, 
    kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, 
    kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, 
    kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, 
    kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor, kGreenColor
};

static const tSenseHAT_LEDPixelArray kBlueArray = {
    kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, 
    kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, 
    kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, 
    kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, 
    kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, 
    kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, 
    kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, 
    kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor, kBlueColor
};

static const tSenseHAT_LEDPixelArray kRainbowSeedArray = {
    {255,0,0}, {255,0,0}, {255,87,0}, {255,196,0}, {205,255,0}, {95,255,0}, {0,255,13}, {0,255,122},
    {255,0,0}, {255,96,0}, {255,205,0}, {196,255,0}, {87,255,0}, {0,255,22}, {0,255,131}, {0,255,240},
    {255,105,0}, {255,214,0}, {187,255,0}, {78,255,0}, {0,255,30}, {0,255,140}, {0,255,248}, {0,152,255},
    {255,233,0}, {178,255,0}, {70,255,0}, {0,255,40}, {0,255,148}, {0,253,255}, {0,144,255}, {0,34,255},
    {170,255,0}, {61,255,0}, {0,255,48}, {0,255,157}, {0,243, 255}, {0,134,255}, {0,26,255}, {83,0,255},
    {52,255,0}, {0,255,57}, {0,255,166}, {0,235,255}, {0,126,255}, {0,17,255}, {92,0,255}, {201,0,255},
    {0,255,66}, {0,255,174}, {0,226,255}, {0,117,255}, {0,8,255}, {100,0,255}, {210,0,255}, {255,0,192},
    {0,255,183}, {0,217,255}, {0,109,255}, {0,0,255}, {110,0,255}, {218,0,255}, {255,0,183}, {255,0,74}
};

static const tSenseHAT_LEDPixelArray kQuestionMarkArray = {
    {255,255,255}, {255,255,255}, {255,255,255}, {255,0,0}, {255,0,0}, {255,255,255}, {255,255,255}, {255,255,255},
    {255,255,255}, {255,255,255}, {255,0,0}, {255,255,255}, {255,255,255}, {255,0,0}, {255,255,255}, {255,255,255},
    {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,0,0}, {255,255,255}, {255,255,255},
    {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,0,0}, {255,255,255}, {255,255,255}, {255,255,255},
    {255,255,255}, {255,255,255}, {255,255,255}, {255,0,0}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255},
    {255,255,255}, {255,255,255}, {255,255,255}, {255,0,0}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255},
    {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255},
    {255,255,255}, {255,255,255}, {255,255,255}, {255,0,0}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}
};

// =================================================================================================
//  Globals
// =================================================================================================
static bool gDone = false;
static bool gAbort = false;
static tSenseHAT_Instance gInstance = NULL;

// =================================================================================================
//  Private prototypes
// =================================================================================================

static void SimpleSignalHandler (int signo);
static void PrintCmdLineHelp (void);
static void PrintCmdNumHelp (void);
static void GetVersion (void);
static void Flash (void);
static void ColorCycleExample (void);
static void CompassExample (void);
static void RainbowExample (void);
static void RotationExample (void);
static void TextScrollExample (void);
static void LoadImage (char* imageFilePath);
static void Rotate (tSenseHAT_LEDRotation rotation);
static void ShowLetter (char* letter);
static void ShowMessage (char* message);
static void FlipHorizontal (bool flip);
static void FlipVertical (bool flip);
static void GetHeading (void);
static void GetEnvironment (void);
static void WaitForEvent (tSenseHAT_JoystickDirection direction);

// =================================================================================================
//  SimpleSignalHandler
// =================================================================================================
void SimpleSignalHandler (int signo)
{
    switch (signo)
    {
        case SIGINT:    // Interrupt (ctrl-c)
        {
            gDone = true;
            break;
        }
        case SIGQUIT:   // Quit
        {
            printf("\n\n*** SIGQUIT received! ***\n\n");
            break;
        }
        case SIGABRT:   // abort()
        {
            printf("\n\n*** SIGABRT received! ***\n\n");
            gAbort = true;
            break;
        }
        case SIGTERM:   // Software termination signal from kill
        {
            printf("\n\n*** SIGTERM received! ***\n\n");
            gAbort = true;
            break;
        }
        default:
            break;
    }
}

// =================================================================================================
//  PrintCmdLineHelp
// =================================================================================================
void PrintCmdLineHelp (void)
{
    printf("Available command line arguments are:\n\n");
    printf("--color-cycle-example                       Color cycle example.\n");
    printf("--compass-example                           Compass example.\n");
    printf("--environment                               Get environmental conditions.\n");
    printf("--flash                                     Flash LEDs.\n");
    printf("--heading                                   Get compass heading in degrees.\n");
    printf("--help                                      Prints this usage notice.\n");
    printf("--load-image=<path>                         Load image in file <path>.\n");
    printf("--rainbow-example                           Rainbow example.\n");
    printf("--rotation-example                          Rotation example.\n");
    printf("--show-letter=<letter>                      Show letter.\n");
    printf("--show-message=<message>                    Show message.\n");
    printf("--text-scroll-example                       Text scroll example.\n");
    printf("--version                                   Returns C library version.\n");
    printf("--wait-for-event=<up|down|left|right|push>  Wait for event.\n");
}

// =================================================================================================
//  PrintCmdNumHelp
// =================================================================================================
void PrintCmdNumHelp (void)
{
    printf("\nChoose one of the following commands:\n\n");
    printf(" 1 - Prints this help notice.\n");
    printf(" 2 - Get C library version.\n");
    printf(" 3 - Color cycle example.\n");
    printf(" 4 - Compass example.\n");
    printf(" 5 - Rainbow example.\n");
    printf(" 6 - Rotation example.\n");
    printf(" 7 - Text scroll example.\n");
    printf(" 8 - Flash LEDs.\n");
    printf(" 9 - Load image.\n");
    printf("10 - Rotate LEDs.\n");
    printf("11 - Show letter.\n");
    printf("12 - Show message.\n");
    printf("13 - Flip LEDs horizontally.\n");
    printf("14 - Flip LEDs vertically.\n");
    printf("15 - Get compass heading in degrees.\n");
    printf("16 - Get environmental conditions.\n");
    printf("17 - Wait for event.\n");
    printf("18 - Quit (default).\n");
    printf("\nEnter your choice: ");
}

// =================================================================================================
//  main
// =================================================================================================
int main(int argc, const char * argv[])
{
    int retValue = 0;
    size_t optionLen = 0;
    char* option = NULL;

    // Setup signal handler for interesting signals
    if (signal(SIGINT, SimpleSignalHandler) == SIG_ERR)
        printf("Can't catch SIGINT!\n\n");
    
    if (signal(SIGABRT, SimpleSignalHandler) == SIG_ERR)
        printf("Can't catch SIGABRT!\n\n");
    
    if (signal(SIGTERM, SimpleSignalHandler) == SIG_ERR)
        printf("Can't catch SIGABRT!\n\n");
    
    if (signal(SIGQUIT, SimpleSignalHandler) == SIG_ERR)
        printf("Can't catch SIGABRT!\n\n");

    // Print banner
    printf("\n ************************************************\n");
    printf(" *** Raspberry Pi Sense HAT C Library Example ***\n");
    printf(" ************************************************\n");

    // Open an instance
    int32_t result = SenseHAT_Open(&gInstance);
    if (result == 0)
    {
        // Check for arguments
        if (argc != 1)
        {
            size_t cmdLen = strlen(argv[1]);

            printf("\n");

            // Check command line arguments
            if (strncmp(argv[1], kHeadingCmd, (cmdLen > strlen(kHeadingCmd) ? strlen(kHeadingCmd) : cmdLen)) == 0)
            {
                GetHeading();
            }
            else if (strncmp(argv[1], kEnvironmentCmd, (cmdLen > strlen(kEnvironmentCmd) ? strlen(kEnvironmentCmd) : cmdLen)) == 0)
            {
                GetEnvironment();
            }
            else if (strncmp(argv[1], kFlashCmd, (cmdLen > strlen(kFlashCmd) ? strlen(kFlashCmd) : cmdLen)) == 0)
            {
                Flash();
            }
            else if (strncmp(argv[1], kColorCycleExampleCmd, (cmdLen > strlen(kColorCycleExampleCmd) ? strlen(kColorCycleExampleCmd) : cmdLen)) == 0)
            {
                ColorCycleExample();
            }
            else if (strncmp(argv[1], kCompassExampleCmd, (cmdLen > strlen(kCompassExampleCmd) ? strlen(kCompassExampleCmd) : cmdLen)) == 0)
            {
                CompassExample();
            }
            else if (strncmp(argv[1], kRainbowExampleCmd, (cmdLen > strlen(kRainbowExampleCmd) ? strlen(kRainbowExampleCmd) : cmdLen)) == 0)
            {
                RainbowExample();
            }
            else if (strncmp(argv[1], kRotationExampleCmd, (cmdLen > strlen(kRotationExampleCmd) ? strlen(kRotationExampleCmd) : cmdLen)) == 0)
            {
                RotationExample();
            }
            else if (strncmp(argv[1], kTextScrollExampleCmd, (cmdLen > strlen(kTextScrollExampleCmd) ? strlen(kTextScrollExampleCmd) : cmdLen)) == 0)
            {
                TextScrollExample();
            }
            else if (strncmp(argv[1], kHelpCmd, (cmdLen > strlen(kHelpCmd) ? strlen(kHelpCmd) : cmdLen)) == 0)
            {
                PrintCmdLineHelp();
            }
            else if (strncmp(argv[1], kLoadImageCmd, (cmdLen > strlen(kLoadImageCmd) ? strlen(kLoadImageCmd) : cmdLen)) == 0)
            {
                optionLen = strlen(kLoadImageCmd) + 1;
                option = (char*)&((argv[1])[optionLen]);
                if (strlen(option) > 0)
                {
                    LoadImage(option);
                }
            }
            else if (strncmp(argv[1], kShowLetterCmd, (cmdLen > strlen(kShowLetterCmd) ? strlen(kShowLetterCmd) : cmdLen)) == 0)
            {
                optionLen = strlen(kShowLetterCmd) + 1;
                option = (char*)&((argv[1])[optionLen]);
                if (strlen(option) == 1)
                {
                    ShowLetter(option);
                }
                else    // Not a letter
                {
                    printf("Argument is not a single letter!\n");
                }
            }
            else if (strncmp(argv[1], kShowMessageCmd, (cmdLen > strlen(kShowMessageCmd) ? strlen(kShowMessageCmd) : cmdLen)) == 0)
            {
                optionLen = strlen(kShowMessageCmd) + 1;
                option = (char*)&((argv[1])[optionLen]);
                if (strlen(option) > 0)
                {
                    ShowMessage(option);
                }
                else
                {
                    printf("Argument is a zero-length string!\n");
                }
            }
            else if (strncmp(argv[1], kVersionCmd, (cmdLen > strlen(kVersionCmd) ? strlen(kVersionCmd) : cmdLen)) == 0)
            {
                GetVersion();
            }
            else if (strncmp(argv[1], kWaitForEventCmd, (cmdLen > strlen(kWaitForEventCmd) ? strlen(kWaitForEventCmd) : cmdLen)) == 0)
            {
                optionLen = strlen(kWaitForEventCmd) + 1;
                option = (char*)&((argv[1])[optionLen]);
                if (strncmp(option, kUpEvent, strlen(kUpEvent)) == 0)
                {
                    WaitForEvent(eSenseHAT_JoystickDirectionUp);
                }
                else if (strncmp(option, kDownEvent, strlen(kDownEvent)) == 0)
                {
                    WaitForEvent(eSenseHAT_JoystickDirectionDown);
                }
                else if (strncmp(option, kLeftEvent, strlen(kLeftEvent)) == 0)
                {
                    WaitForEvent(eSenseHAT_JoystickDirectionLeft);
                }
                else if (strncmp(option, kRightEvent, strlen(kRightEvent)) == 0)
                {
                    WaitForEvent(eSenseHAT_JoystickDirectionRight);
                }
                else if (strncmp(option, kPushEvent, strlen(kPushEvent)) == 0)
                {
                    WaitForEvent(eSenseHAT_JoystickDirectionPush);
                }
                else
                {
                    printf("Invalid command line argument!\n");
                }
            }
            else    // Invalid command
            {
                printf("Invalid command line argument!\n");
            }

            printf("\nQuitting...\n\n");
        }
        else
        {
            // Enter interactive command processor
            bool done = false;
            uint32_t cmdNum = 0;
            char cmdBuf[128];
            int scanResult = 0;

            while (!gAbort && !done)
            {
                memset(&(cmdBuf[0]), 0, 128);
                PrintCmdNumHelp();
                fgets(cmdBuf, sizeof(cmdBuf), stdin);
                scanResult = sscanf(cmdBuf, "%d", &cmdNum);
                printf("\n");
                if (scanResult == EOF)
                {
                    done = true;
                    printf("\nQuitting...\n\n");
                }
                else if (scanResult == 0)
                {
                    printf("Invalid command.\n\n");
                }
                else
                {
                    switch (cmdNum)
                    {
                        case kHelpCmdNum:
                        {
                            break;
                        }
                        case kVersionCmdNum:
                        {
                            GetVersion();
                            break;
                        }
                        case kFlashCmdNum:
                        {
                            Flash();
                            break;
                        }
                        case kColorCycleExampleCmdNum:
                        {
                            ColorCycleExample();
                            break;
                        }
                        case kCompassExampleCmdNum:
                        {
                            CompassExample();
                            break;
                        }
                        case kRainbowExampleCmdNum:
                        {
                            RainbowExample();
                            break;
                        }
                        case kRotationExampleCmdNum:
                        {
                            RotationExample();
                            break;
                        }
                        case kTextScrollExampleCmdNum:
                        {
                            TextScrollExample();
                            break;
                        }
                        case kLoadImageCmdNum:
                        {
                            // Prompt for file path
                            memset(&(cmdBuf[0]), 0, 128);
                            printf("Enter image file path: ");
                            fgets(cmdBuf, sizeof(cmdBuf), stdin);
                            scanResult = sscanf(cmdBuf, "%s", cmdBuf);
                            LoadImage(cmdBuf);
                            break;
                        }
                        case kRotateCmdNum:
                        {
                            // Prompt for degrees of rotation
                            memset(&(cmdBuf[0]), 0, 128);
                            printf("Available rotations:\n");
                            printf("1 - 0\n");
                            printf("2 - 90\n");
                            printf("3 - 180\n");
                            printf("4 - 270\n");
                            printf("Enter rotation: ");
                            fgets(cmdBuf, sizeof(cmdBuf), stdin);
                            scanResult = sscanf(cmdBuf, "%d", &cmdNum);
                            if ((scanResult == EOF) || (scanResult == 0))
                            {
                                printf("Invalid rotation!\n\n");
                            }
                            else
                            {
                                switch (cmdNum)
                                {
                                    case 1:
                                        Rotate(eSenseHAT_LEDRotation0);
                                        break;
                                    case 2:
                                        Rotate(eSenseHAT_LEDRotation90);
                                        break;
                                    case 3:
                                        Rotate(eSenseHAT_LEDRotation180);
                                        break;
                                    case 4:
                                        Rotate(eSenseHAT_LEDRotation270);
                                        break;
                                    default:
                                        printf("Invalid rotation!\n");
                                        break;
                                }
                            }
                            break;
                        }
                        case kShowLetterCmdNum:
                        {
                            // Prompt for letter
                            memset(&(cmdBuf[0]), 0, 128);
                            printf("Enter letter: ");
                            fgets(cmdBuf, sizeof(cmdBuf), stdin);
                            scanResult = sscanf(cmdBuf, "%s", cmdBuf);
                            if (scanResult == 1)
                            {
                                ShowLetter(cmdBuf);
                            }
                            else
                            {
                                printf("Invalid input!\n");
                            }
                            break;
                        }
                        case kShowMessageCmdNum:
                        {
                            // Prompt for message
                            memset(&(cmdBuf[0]), 0, 128);
                            printf("Enter message: ");
                            fgets(cmdBuf, sizeof(cmdBuf), stdin);
                            ShowMessage(cmdBuf);
                            break;
                        }
                        case kFlipHorizontalCmdNum:
                        {
                            FlipHorizontal(true);
                            break;
                        }
                        case kFlipVerticalCmdNum:
                        {
                            FlipVertical(true);
                            break;
                        }
                        case kHeadingCmdNum:
                        {
                            GetHeading();
                            break;
                        }
                        case kEnvironmentCmdNum:
                        {
                            GetEnvironment();
                            break;
                        }
                        case kWaitForEventCmdNum:
                        {
                            // Prompt for event
                            memset(&(cmdBuf[0]), 0, 128);
                            printf("Available events:\n");
                            printf("1 - Up\n");
                            printf("2 - Down\n");
                            printf("3 - Left\n");
                            printf("4 - Right\n");
                            printf("5 - Push\n");
                            printf("Enter event: ");
                            fgets(cmdBuf, sizeof(cmdBuf), stdin);
                            scanResult = sscanf(cmdBuf, "%d", &cmdNum);
                            if ((scanResult == EOF) || (scanResult == 0))
                            {
                                printf("Invalid event.\n\n");
                            }
                            else
                            {
                                switch (cmdNum)
                                {
                                    case 1:
                                        WaitForEvent(eSenseHAT_JoystickDirectionUp);
                                        break;
                                    case 2:
                                        WaitForEvent(eSenseHAT_JoystickDirectionDown);
                                        break;
                                    case 3:
                                        WaitForEvent(eSenseHAT_JoystickDirectionLeft);
                                        break;
                                    case 4:
                                        WaitForEvent(eSenseHAT_JoystickDirectionRight);
                                        break;
                                    case 5:
                                        WaitForEvent(eSenseHAT_JoystickDirectionPush);
                                        break;
                                    default:
                                        printf("Invalid event.\n\n");
                                        break;
                                }
                            }
                            break;
                        }
                        case kQuitCmdNum:
                        {
                            done = true;
                            printf("\nQuitting...\n\n");
                            break;
                        }
                        default:
                        {
                            printf("Invalid command.\n\n");
                            break;
                        }
                    }
                }
            }
        }

        // Clear the LEDs 
        result = SenseHAT_LEDClear(gInstance, &kBackColor);

        // Close the instance
        result = SenseHAT_Close(&gInstance);
    }
    else    // SenseHAT_Open failed
    {
        printf("SenseHAT_Open failed!\n");
    } 
    return retValue;
}

// =================================================================================================
//  GetVersion
// =================================================================================================
void GetVersion (void)
{
    uint32_t version = SenseHAT_Version();
    printf("Raspberry Pi Sense HAT C library version is %d.%d.%d.\n", 
        ((version & 0xFFFF0000) >> 16),
        ((version & 0x0000FF00) >> 8),
        (version & 0x000000FF)); 
    return;
}

// =================================================================================================
//  Flash
// =================================================================================================
void Flash (void)
{
    int32_t result = 0;
    printf("Flashing LEDs... Enter ctrl-c to stop.\n");

    while (!gDone)
    {
        result = SenseHAT_LEDSetPixels(gInstance, kRedArray);
        if (result == 0)
        {
            sleep(1);
            result = SenseHAT_LEDSetPixels(gInstance, kGreenArray);
            if (result == 0)
            {
                sleep(1);
                result = SenseHAT_LEDSetPixels(gInstance, kBlueArray);
                if (result == 0)
                {
                    sleep(1);
                    result = SenseHAT_LEDClear(gInstance, &kBackColor);
                    if (result != 0)
                    {
                        printf("SenseHAT_LEDClear failed!\n");
                        gDone = true;
                    }
                }
                else
                {
                    printf ("SenseHAT_LEDSetPixels failed!\n");
                    gDone = true;
                }
            }
            else
            {
                printf ("SenseHAT_LEDSetPixels failed!\n");
                gDone = true;
            }
        }
        else
        {
            printf ("SenseHAT_LEDSetPixels failed!\n");
            gDone = true;
        }  
    }

    if (result == 0)
    {
        result = SenseHAT_LEDClear(gInstance, NULL);
    }

    gDone = false;

    return;
}

// =================================================================================================
//  ColorCycleExample
// =================================================================================================
void ColorCycleExample (void)
{
    int32_t result = 0;
    printf("Color cycling LEDs... Enter ctrl-c to stop.\n");
    result = SenseHAT_LEDClear(gInstance, NULL);
    if (result == 0)
    {
        useconds_t sleepTime = 2;
        tSenseHAT_LEDPixel color = {255, 0, 0};

        while (!gDone)
        {
            result = SenseHAT_LEDClear(gInstance, &color);
            if (result == 0)
            {
                if ((color.red == 255) && (color.green < 255) && (color.blue == 0))
                    color.green += 1;
                if ((color.green == 255) && (color.red > 0) && (color.blue == 0))
                    color.red -= 1;
                if ((color.green == 255) && (color.blue < 255) && (color.red == 0))
                    color.blue += 1;
                if ((color.blue == 255) && (color.green > 0) && (color.red == 0))
                    color.green -= 1;
                if ((color.blue == 255) && (color.red < 255) && (color.green == 0))
                    color.red += 1;
                if ((color.red == 255) && (color.blue > 0) && (color.green == 0))
                    color.blue -= 1;
                usleep(sleepTime);
            }
            else
            {
                printf("SenseHAT_LEDClear failed!\n");
                gDone = true;
            }
        }

        if (result == 0)
        {
            result = SenseHAT_LEDClear(gInstance, NULL);
        }

        gDone = false;
    }
    else
    {
        printf("SenseHAT_LEDClear failed!\n");
    }
    return;
}

// =================================================================================================
//  RainbowExample
// =================================================================================================
void RainbowExample (void)
{
    int32_t result = 0;
    printf("Animating rainbow on LEDs... Enter ctrl-c to stop.\n");
    result = SenseHAT_LEDClear(gInstance, NULL);
    if (result == 0)
    {
        useconds_t sleepTime = 2;
        tSenseHAT_LEDPixelArray pixels;
        uint32_t index = 0;
        int32_t red = 0;
        int32_t green = 0;
        int32_t blue = 0;

        memcpy((void*)pixels, (void*)kRainbowSeedArray, sizeof(tSenseHAT_LEDPixelArray));

        result = SenseHAT_LEDSetPixels(gInstance, pixels);
        if (result == 0)
        {
            while (!gDone)
            {
                for (index = 0; index < 64; index++)
                {
                    red = pixels[index].red;
                    green = pixels[index].green;
                    blue = pixels[index].blue;

                    if ((red == 255) && (green < 255) && (blue == 0))
                        green += 1;
                    if ((green == 255) && (red > 0) && (blue == 0))
                        red -= 1;
                    if ((green == 255) && (blue < 255) && (red == 0))
                        blue += 1;
                    if ((blue == 255) && (green > 0) && (red == 0))
                        green -= 1;
                    if ((blue == 255) && (red < 255) && (green == 0))
                        red += 1;
                    if ((red == 255) && (blue > 0) && (green == 0))
                        blue -= 1;

                    pixels[index].red = red;
                    pixels[index].green = green;
                    pixels[index].blue = blue;
                }
                result = SenseHAT_LEDSetPixels(gInstance, pixels);
                usleep(sleepTime);
            }

            if (result == 0)
            {
                result = SenseHAT_LEDClear(gInstance, NULL);
            }

            gDone = false;
        }
        else    // SenseHAT_LEDSetPixels failed
        {
            printf("SenseHAT_LEDSetPixels failed!\n");
        }
    }
    else    // SenseHAT_LEDClear failed
    {
        printf("SenseHAT_LEDClear failed!\n");
    }
    return;
}

// =================================================================================================
//  RotationExample
// =================================================================================================
void RotationExample (void)
{
    int32_t result = 0;

    result = SenseHAT_LEDSetPixels(gInstance, kQuestionMarkArray);
    if (result == 0)
    {
        tSenseHAT_LEDPixel redColor = kRedColor;
        result = SenseHAT_LEDSetPixel(gInstance, 0, 0, &redColor);
        if (result == 0)
        {
            tSenseHAT_LEDPixel blueColor = kBlueColor;
            result = SenseHAT_LEDSetPixel(gInstance, 0, 7, &blueColor);
            if (result == 0)
            {
                tSenseHAT_LEDPixel greenColor = kGreenColor;
                result = SenseHAT_LEDSetPixel(gInstance, 7, 0, &greenColor);
                if (result == 0)
                {
                    tSenseHAT_LEDPixel purpleColor = kPurpleColor;
                    result = SenseHAT_LEDSetPixel(gInstance, 7, 7, &purpleColor);
                    if (result == 0)
                    {
                        uint32_t i = 0;
                        printf("Rotating... Enter ctrl-c to stop.\n");
                        while (!gDone)
                        {
                            result = SenseHAT_LEDSetRotation(gInstance, (tSenseHAT_LEDRotation)i, true);
                            if (result == 0)
                            {
                                i += 90;
                                if (i > 270)
                                {
                                    i = 0;
                                }
                                sleep(1);
                            }
                            else
                            {
                                printf("SenseHAT_LEDSetRotation failed!\n");
                                gDone = true;
                            }
                        }
                    }
                    else
                    {
                        printf("SenseHAT_LEDSetPixel failed!\n");
                    }
                }
                else
                {
                    printf("SenseHAT_LEDSetPixel failed!\n");
                }
            }
            else
            {
                printf("SenseHAT_LEDSetPixel failed!\n");
            }
        }
        else
        {
            printf("SenseHAT_LEDSetPixel failed!\n");
        }
    }
    else
    {
        printf("SenseHAT_LEDSetPixels failed!\n");
    }

    if (result == 0)
    {
        result = SenseHAT_LEDClear(gInstance, NULL);
    }

    gDone = false;

    return;
}

// =================================================================================================
//  LoadImage
// =================================================================================================
void LoadImage (char* imageFilePath)
{
    int32_t result = 0;
    printf("Loading image...\n");
    result = SenseHAT_LEDLoadImage(gInstance, imageFilePath, true, NULL);
    if (result != 0)
    {
        printf("SenseHAT_LEDLoadImage failed!\n");
    }
    return;
}

// =================================================================================================
//  Rotate
// =================================================================================================
void Rotate (tSenseHAT_LEDRotation rotation)
{
    int32_t result = 0;
    printf("Rotating...\n");
    result = SenseHAT_LEDSetRotation(gInstance, rotation, true);
    if (result != 0)
    {
        printf("SenseHAT_LEDSetRotation failed!\n");
    }
    return;
}

// =================================================================================================
//  ShowLetter
// =================================================================================================
void ShowLetter (char* letter)
{
    int32_t result = 0;
    printf("Showing letter...\n");
    result = SenseHAT_LEDShowLetter(gInstance, letter, &kTextColor, &kBackColor);
    if (result == 0)
    {
        sleep(1);
    }
    else
    {
        printf("SenseHAT_LEDShowLetter failed!\n");
    }
    return;
}

// =================================================================================================
//  ShowMessage
// =================================================================================================
void ShowMessage (char* message)
{
    int32_t result = 0;
    printf("Showing message...\n");
    result = SenseHAT_LEDShowMessage(gInstance, message, 0.05, &kTextColor, &kBackColor);
    if (result == 0)
    {
        sleep(1);
    }
    else
    {
        printf("SenseHAT_LEDShowMessage failed!\n");
    }    
    return;
}

// =================================================================================================
//  FlipHorizontal
// =================================================================================================
void FlipHorizontal (bool flip)
{
    int32_t result = 0;
    printf("Flipping horizontally...\n");
    result = SenseHAT_LEDFlipHorizontal(gInstance, flip, NULL);
    if (result != 0)
    {
        printf("SenseHAT_LEDFlipHorizontal failed!\n");
    }
    return;
}

// =================================================================================================
//  FlipVertical
// =================================================================================================
void FlipVertical (bool flip)
{
    int32_t result = 0;
    printf("Flipping vertically...\n");
    result = SenseHAT_LEDFlipVertical(gInstance, flip, NULL);
    if (result != 0)
    {
        printf("SenseHAT_LEDFlipVertical failed!\n");
    }
    return;
}

// =================================================================================================
//  GetHeading
// =================================================================================================
void GetHeading (void)
{
    double heading = 0.0;
    int32_t result = 0;
    result = SenseHAT_GetCompass(gInstance, &heading);
    if (result == 0)
    {
        char msg[128];
        memset((void*)msg, 0, 128);
        sprintf(msg, "Compass heading is %.2f degrees.\n", heading);
        printf(msg);
        (void)ShowMessage(msg);
    }
    else
    {
        printf("SenseHAT_GetCompass failed!\n");
    }
    return;
}

// =================================================================================================
//  GetEnvironment
// =================================================================================================
void GetEnvironment (void)
{
    double humidity = 0.0;
    int32_t result = 0;
    result = SenseHAT_GetHumidity(gInstance, &humidity);
    if (result == 0)
    {
        double pressure = 0.0;
        char msg[128];
        memset((void*)msg, 0, 128);
        sprintf(msg, "Humidity is %.2f percent relative humidity.\n", humidity);
        printf(msg);

        result = SenseHAT_GetPressure(gInstance, &pressure);
        if (result == 0)
        {
            double temperature = 0.0;

            memset((void*)msg, 0, 128);
            sprintf(msg, "Pressure is %.2f millibars.\n", pressure);
            printf(msg);

            result = SenseHAT_GetTemperature(gInstance, &temperature);
            if (result == 0)
            {
                double tempF = ((temperature * 9.0) / 5.0) + 32.0;
                char msg[128];
                memset((void*)msg, 0, 128);
                sprintf(msg, "Temperature is %.2f degrees Celsius, %.2f degrees Fahrenheit.\n", temperature, tempF);
                printf(msg);
            }
            else
            {
                printf("SenseHAT_GetTemperature failed!\n");
            }
        }
        else
        {
            printf("SenseHAT_GetPressure failed!\n");
        }
    }
    else
    {
        printf("SenseHAT_GetHumidity failed!\n");
    }
    return;
}

// =================================================================================================
//  WaitForEvent
// =================================================================================================
void WaitForEvent (tSenseHAT_JoystickDirection direction)
{
    tSenseHAT_JoystickEvent event;
    bool gotEvent = false;
    int32_t result = 0;

    printf("Waiting for event... Enter ctrl-c to stop.\n");
    while (!gotEvent && !gDone)
    {
        result = SenseHAT_WaitForEvent(gInstance, true, &event);
        if (result == 0)
        {
            if (event.direction == direction)
            {
                gotEvent = true;
                break;
            }
        }
        else
        {
            printf("SenseHAT_WaitForEvent failed!\n");
            break;
        }
    }   
    return;
}

// =================================================================================================
//  CompassExample
// =================================================================================================
void CompassExample (void)
{
    int32_t ledLoop[28] = {4, 5, 6, 7, 15, 23, 31, 39, 47, 55, 63, 62, 61, 60, 59, 58, 57, 56, 48, 40, 32, 24, 16, 8, 0, 1, 2, 3};
    int32_t result = 0;

    result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation0, true);
    if (result == 0)
    {
        result = SenseHAT_LEDClear(gInstance, NULL);
        if (result == 0)
        {
            int32_t prevX = 0;
            int32_t prevY = 0;
            double ledDegreeRatio = 28.0/360.0;
            double degrees = 0.0;
            double degreesInverted = 0.0;
            int32_t ledIndex = 0;
            int32_t offset = 0;
            int32_t x = 0;
            int32_t y = 0;
            tSenseHAT_LEDPixel color = kBlueColor;

            printf("Tracking compass... Enter ctrl-c to stop.\n");
            while (!gDone)
            {
                result = SenseHAT_GetCompass(gInstance, &degrees);
                if (result == 0)
                {
                    degreesInverted = 360 - degrees;
                    ledIndex = (int32_t)(ledDegreeRatio * degreesInverted);
                    offset = ledLoop[ledIndex];

                    y = offset / 8;
                    x = offset % 8;

                    if ((x != prevX) || (y != prevY))
                    {
                        result = SenseHAT_LEDSetPixel(gInstance, prevX, prevY, NULL);
                        if (result != 0)
                        {
                            printf("SenseHAT_LEDSetPixel failed!\n");
                            gDone = true;
                        }
                    }

                    if (!gDone)
                    {
                        result = SenseHAT_LEDSetPixel(gInstance, x, y, &color);
                        if (result == 0)
                        {   
                            prevX = x; 
                            prevY = y;
                        }
                        else
                        {
                            printf("SenseHAT_LEDSetPixel failed!\n");
                            gDone = true;
                        }
                    }
                }
            }
        }
        else
        {
            printf("SenseHAT_LEDClear failed!\n");
        }
    }
    else
    {
        printf("SenseHAT_LEDSetRotation failed!\n");
    }

    gDone = false;

    return;
}

// =================================================================================================
//  TextScrollExample
// =================================================================================================
void TextScrollExample (void)
{
    int32_t result = 0;

    result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation180, true);
    if (result == 0)
    {
        tSenseHAT_LEDPixel color = kRedColor;

        result = SenseHAT_LEDShowMessage(gInstance,
                                         "One small step for Pi!",
                                         0.05,
                                         &color,
                                         NULL);
        if (result == 0)
        {
            result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation0, true);
            if (result != 0)
            {
                printf("SenseHAT_LEDSetRotation failed!\n");
            }
        }
        else
        {
            printf("SenseHAT_LEDShowMessage failed!\n");
        }
    }
    else 
    {
        printf("SenseHAT_LEDSetRotation failed!\n");
    }
    return;
}
// =================================================================================================
