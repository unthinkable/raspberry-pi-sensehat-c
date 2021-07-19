// =================================================================================================
//
//  sensehat_example.c
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
#include "unthink_utils.h"
#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// =================================================================================================
//  Constants
// =================================================================================================

//  Command line strings
#define COLOR_CYCLE_EXAMPLE_CMD "color-cycle-example"
#define COMPASS_EXAMPLE_CMD     "compass"
#define ENVIRONMENT_CMD         "environment"
#define FLASH_CMD               "flash"
#define HEADING_CMD             "heading"
#define HELP_CMD                "help"
#define LOAD_IMAGE_CMD          "load-image"
#define RAINBOW_EXAMPLE_CMD     "rainbow-example"
#define ROTATION_EXAMPLE_CMD    "rotation-example"
#define SHOW_LETTER_CMD         "show-letter"
#define SHOW_MESSAGE_CMD        "show-message"
#define TEXT_SCROLL_EXAMPLE_CMD "text-scroll-example"
#define VERSION_CMD             "version"
#define WAIT_FOR_EVENT_CMD      "wait-for-event"

//  Command line options
#define COLOR_CYCLE_EXAMPLE_OPTION  0
#define COMPASS_EXAMPLE_OPTION      1
#define ENVIRONMENT_OPTION          2
#define FLASH_OPTION                3
#define HEADING_OPTION              4
#define HELP_OPTION                 5
#define LOAD_IMAGE_OPTION           6
#define RAINBOW_EXAMPLE_OPTION      7
#define ROTATION_EXAMPLE_OPTION     8
#define SHOW_LETTER_OPTION          9
#define SHOW_MESSAGE_OPTION         10
#define TEXT_SCROLL_EXAMPLE_OPTION  11
#define VERSION_OPTION              12
#define WAIT_FOR_EVENT_OPTION       13

//  Short command line strings
#define COLOR_CYCLE_EXAMPLE_SHORT_CMD   'c'
#define COMPASS_EXAMPLE_SHORT_CMD       'C'
#define ENVIRONMENT_SHORT_CMD           'e'
#define FLASH_SHORT_CMD                 'f'
#define HEADING_SHORT_CMD               'H'
#define HELP_SHORT_CMD                  'h'
#define LOAD_IMAGE_SHORT_CMD            'i'
#define RAINBOW_EXAMPLE_SHORT_CMD       'r'
#define ROTATION_EXAMPLE_SHORT_CMD      'R'
#define SHOW_LETTER_SHORT_CMD           's'
#define SHOW_MESSAGE_SHORT_CMD          'S'
#define TEXT_SCROLL_EXAMPLE_SHORT_CMD   't'
#define VERSION_SHORT_CMD               'v'
#define WAIT_FOR_EVENT_SHORT_CMD        'w'
#define UNKNOWN_SHORT_CMD               '?'

//  Options array
static const char kShortOptions[] = { COLOR_CYCLE_EXAMPLE_SHORT_CMD,
                                      COMPASS_EXAMPLE_SHORT_CMD,
                                      ENVIRONMENT_SHORT_CMD,
                                      FLASH_SHORT_CMD,
                                      HEADING_SHORT_CMD,
                                      HELP_SHORT_CMD,
                                      LOAD_IMAGE_SHORT_CMD, ':',
                                      RAINBOW_EXAMPLE_SHORT_CMD,
                                      ROTATION_EXAMPLE_SHORT_CMD,
                                      SHOW_LETTER_SHORT_CMD, ':',
                                      SHOW_MESSAGE_SHORT_CMD, ':',
                                      TEXT_SCROLL_EXAMPLE_SHORT_CMD,
                                      VERSION_SHORT_CMD,
                                      WAIT_FOR_EVENT_SHORT_CMD, ':',
                                      0x00 };

// Interactive commands
#define HELP_CMD_NUM                1
#define VERSION_CMD_NUM             2
#define COLOR_CYCLE_EXAMPLE_CMD_NUM 3
#define COMPASS_EXAMPLE_CMD_NUM     4
#define RAINBOW_EXAMPLE_CMD_NUM     5
#define ROTATION_EXAMPLE_CMD_NUM    6
#define TEXT_SCROLL_EXAMPLE_CMD_NUM 7
#define FLASH_CMD_NUM               8
#define LOAD_IMAGE_CMD_NUM          9
#define ROTATE_CMD_NUM              10
#define SHOW_LETTER_CMD_NUM         11
#define SHOW_MESSAGE_CMD_NUM        12
#define FLIP_HORIZONTAL_CMD_NUM     13
#define FLIP_VERTICAL_CMD_NUM       14
#define HEADING_CMD_NUM             15
#define ENVIRONMENT_CMD_NUM         16
#define WAIT_FOR_EVENT_CMD_NUM      17
#define QUIT_CMD_NUM                18

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
static void PrintCmdLineHelp (const char* programName);
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
        case SIGINT:    // Interrupt
        {
            fprintf(stdout, "\nSIGINT received\n");
            gDone = true;
            break;
        }
        case SIGQUIT:   // Quit
        {
            fprintf(stdout, "\nSIGQUIT received\n");
            gDone = true;
            break;
        }
        case SIGABRT:   // abort()
        {
            fprintf(stdout, "\nSIGABRT received\n");
            gAbort = true;
            break;
        }
        case SIGTERM:   // Software termination signal from kill
        {
            fprintf(stdout, "\nSIGTERM received\n");
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
void PrintCmdLineHelp (const char* programName)
{
    fprintf(stdout, "\nUsage: %s <arguments>\n\n", basename((char*)programName));
    fprintf(stdout, "\tAvailable command line arguments are:\n\n");
    fprintf(stdout, "\t-%c, --%s\t\t\t\t\tRun the color cycle example.\n", 
           COLOR_CYCLE_EXAMPLE_SHORT_CMD, 
           COLOR_CYCLE_EXAMPLE_CMD);
    fprintf(stdout, "\t-%c, --%s\t\t\t\t\tRun the compass example.\n",
           COMPASS_EXAMPLE_SHORT_CMD,
           COMPASS_EXAMPLE_CMD);
    fprintf(stdout, "\t-%c, --%s\t\t\t\tGet the current environmental conditions.\n",
           ENVIRONMENT_SHORT_CMD,
           ENVIRONMENT_CMD);
    fprintf(stdout, "\t-%c, --%s\t\t\t\t\tFlash the LEDs.\n",
           FLASH_SHORT_CMD,
           FLASH_CMD);
    fprintf(stdout, "\t-%c, --%s\t\t\t\t\tGet the compass heading in degrees.\n",
           HEADING_SHORT_CMD, 
           HEADING_CMD);
    fprintf(stdout, "\t-%c, --%s\t\t\t\t\tPrints this usage notice.\n",
           HELP_SHORT_CMD,
           HELP_CMD);
    fprintf(stdout, "\t-%c, --%s <path>\t\t\t\tLoad an image from <path>.\n",
           LOAD_IMAGE_SHORT_CMD,
           LOAD_IMAGE_CMD);
    fprintf(stdout, "\t-%c, --%s\t\t\t\tRun the rainbow example.\n",
           RAINBOW_EXAMPLE_SHORT_CMD,
           RAINBOW_EXAMPLE_CMD);
    fprintf(stdout, "\t-%c, --%s\t\t\t\tRun the rotation example.\n",
           ROTATION_EXAMPLE_SHORT_CMD,
           ROTATION_EXAMPLE_CMD);
    fprintf(stdout, "\t-%c, --%s <letter>\t\t\tShow a letter.\n",
           SHOW_LETTER_SHORT_CMD,
           SHOW_LETTER_CMD);
    fprintf(stdout, "\t-%c, --%s <letter>\t\t\tShow a message.\n",
           SHOW_MESSAGE_SHORT_CMD,
           SHOW_MESSAGE_CMD);
    fprintf(stdout, "\t-%c, --%s\t\t\tRun the text scroll example.\n",
           TEXT_SCROLL_EXAMPLE_SHORT_CMD,
           TEXT_SCROLL_EXAMPLE_CMD);
    fprintf(stdout, "\t-%c, --%s\t\t\t\t\tPrint the version of the SenseHAT C library.\n",
           VERSION_SHORT_CMD,
           VERSION_CMD);
    fprintf(stdout, "\t-%c, --%s <up|down|left|right|push>\tWait for an event.\n\n",
           WAIT_FOR_EVENT_SHORT_CMD,
           WAIT_FOR_EVENT_CMD);
    return;
}

// =================================================================================================
//  PrintCmdNumHelp
// =================================================================================================
void PrintCmdNumHelp (void)
{
    fprintf(stdout, "\nChoose one of the following commands:\n\n");
    fprintf(stdout, "\t%d - Prints this help notice.\n", HELP_CMD_NUM);
    fprintf(stdout, "\t%d - Get C library version.\n", VERSION_CMD_NUM);
    fprintf(stdout, "\t%d - Color cycle example.\n", COLOR_CYCLE_EXAMPLE_CMD_NUM);
    fprintf(stdout, "\t%d - Compass example.\n", COMPASS_EXAMPLE_CMD_NUM);
    fprintf(stdout, "\t%d - Rainbow example.\n", RAINBOW_EXAMPLE_CMD_NUM);
    fprintf(stdout, "\t%d - Rotation example.\n", ROTATION_EXAMPLE_CMD_NUM);
    fprintf(stdout, "\t%d - Text scroll example.\n", TEXT_SCROLL_EXAMPLE_CMD_NUM);
    fprintf(stdout, "\t%d - Flash LEDs.\n", FLASH_CMD_NUM);
    fprintf(stdout, "\t%d - Load image.\n", LOAD_IMAGE_CMD_NUM);
    fprintf(stdout, "\t%d - Rotate LEDs.\n", ROTATE_CMD_NUM);
    fprintf(stdout, "\t%d - Show letter.\n", SHOW_LETTER_CMD_NUM);
    fprintf(stdout, "\t%d - Show message.\n", SHOW_MESSAGE_CMD_NUM);
    fprintf(stdout, "\t%d - Flip LEDs horizontally.\n", FLIP_HORIZONTAL_CMD_NUM);
    fprintf(stdout, "\t%d - Flip LEDs vertically.\n", FLIP_VERTICAL_CMD_NUM);
    fprintf(stdout, "\t%d - Get compass heading in degrees.\n", HEADING_CMD_NUM);
    fprintf(stdout, "\t%d - Get environmental conditions.\n", ENVIRONMENT_CMD_NUM);
    fprintf(stdout, "\t%d - Wait for event.\n", WAIT_FOR_EVENT_CMD_NUM);
    fprintf(stdout, "\t%d - Quit (default).\n", QUIT_CMD_NUM);
    fprintf(stdout, "\nEnter your choice: ");
}

// =================================================================================================
//  main
// =================================================================================================
int main(int argc, const char * argv[])
{
    int retValue = 0;

    // Setup signal handler for interesting signals
    if (signal(SIGINT, SimpleSignalHandler) == SIG_ERR)
        fprintf(stdout, "Can't catch SIGINT.\n");
    
    if (signal(SIGABRT, SimpleSignalHandler) == SIG_ERR)
        fprintf(stdout, "Can't catch SIGABRT.\n");
    
    if (signal(SIGTERM, SimpleSignalHandler) == SIG_ERR)
        fprintf(stdout, "Can't catch SIGABRT.\n");
    
    if (signal(SIGQUIT, SimpleSignalHandler) == SIG_ERR)
        fprintf(stdout, "Can't catch SIGABRT.\n");

    // Print banner
    fprintf(stdout, "\n************************************************\n");
    fprintf(stdout, "*** Raspberry Pi Sense HAT C Library Example ***\n");
    fprintf(stdout, "************************************************\n");

    // Open an instance
    int32_t result = SenseHAT_Open(&gInstance);
    if (result == UNTHINK_SUCCESS)
    {
        // Check for arguments
        if (argc != 1)
        {
            int32_t cmdLineOption = 0;
            int32_t optionIndex = 0;

            // Define command line options
            static struct option longOptions[] =
            {
                { COLOR_CYCLE_EXAMPLE_CMD,  no_argument,        0, 0 }, // Run the color cycle example
                { COMPASS_EXAMPLE_CMD,      no_argument,        0, 0 }, // Run the compass example
                { ENVIRONMENT_CMD,          no_argument,        0, 0 }, // Get the current environmental conditions
                { FLASH_CMD,                no_argument,        0, 0 }, // Flash the LEDs
                { HEADING_CMD,              no_argument,        0, 0 }, // Get the compass heading in degrees
                { HELP_CMD,                 no_argument,        0, 0 }, // Prints this usage notice
                { LOAD_IMAGE_CMD,           required_argument,  0, 0 }, // Load an image from <path>
                { RAINBOW_EXAMPLE_CMD,      no_argument,        0, 0 }, // Run the rainbow example
                { ROTATION_EXAMPLE_CMD,     no_argument,        0, 0 }, // Run the rotation example
                { SHOW_LETTER_CMD,          required_argument,  0, 0 }, // Show a letter
                { SHOW_MESSAGE_CMD,         required_argument,  0, 0 }, // Show a message
                { TEXT_SCROLL_EXAMPLE_CMD,  no_argument,        0, 0 }, // Run the text scroll example
                { VERSION_CMD,              no_argument,        0, 0 }, // Print the version of the SenseHAT C library
                { WAIT_FOR_EVENT_CMD,       required_argument,  0, 0 }, // Wait for an event
                { 0,                        0,                  0, 0 }
            };

            while (cmdLineOption != -1)
            {
                cmdLineOption = getopt_long(argc, 
                                            (char *const *)argv, 
                                            kShortOptions, 
                                            longOptions, 
                                            &optionIndex);

                // Case on command line option
                switch (cmdLineOption)
                {
                    case 0: // Long option found

                        // Color cycle example
                        if (optionIndex == COLOR_CYCLE_EXAMPLE_OPTION)
                            ColorCycleExample();

                        // Compass example
                        else if (optionIndex == COMPASS_EXAMPLE_OPTION)
                            CompassExample();

                        // Environment 
                        else if (optionIndex == ENVIRONMENT_OPTION)
                            GetEnvironment();

                        // Flash
                        else if (optionIndex == FLASH_OPTION)
                            Flash();

                        // Heading
                        else if (optionIndex == HEADING_OPTION)
                            GetHeading();

                        // Help
                        else if (optionIndex == HELP_OPTION)
                            PrintCmdLineHelp(argv[0]);

                        // Load image
                        else if (optionIndex == LOAD_IMAGE_OPTION)
                            LoadImage(optarg);

                        // Rainbow example
                        else if (optionIndex == RAINBOW_EXAMPLE_OPTION)
                            RainbowExample();

                        // Rotation example
                        else if (optionIndex == ROTATION_EXAMPLE_OPTION)
                            RotationExample();

                        // Show letter
                        else if (optionIndex == SHOW_LETTER_OPTION)
                            ShowLetter(optarg);

                        // Show message
                        else if (optionIndex == SHOW_MESSAGE_OPTION)
                            ShowMessage(optarg);

                        // Text scroll example
                        else if (optionIndex == TEXT_SCROLL_EXAMPLE_OPTION)
                            TextScrollExample();

                        // Version
                        else if (optionIndex == VERSION_OPTION)
                            GetVersion();
                        
                        // Wait for event
                        else if (optionIndex == WAIT_FOR_EVENT_OPTION)
                        {
                            if (strncmp(optarg, kUpEvent, strlen(kUpEvent)) == 0)
                                WaitForEvent(eSenseHAT_JoystickDirectionUp);
                            else if (strncmp(optarg, kDownEvent, strlen(kDownEvent)) == 0)
                                WaitForEvent(eSenseHAT_JoystickDirectionDown);
                            else if (strncmp(optarg, kLeftEvent, strlen(kLeftEvent)) == 0)
                                WaitForEvent(eSenseHAT_JoystickDirectionLeft);
                            else if (strncmp(optarg, kRightEvent, strlen(kRightEvent)) == 0)
                                WaitForEvent(eSenseHAT_JoystickDirectionRight);
                            else if (strncmp(optarg, kPushEvent, strlen(kPushEvent)) == 0)
                                WaitForEvent(eSenseHAT_JoystickDirectionPush);
                            else
                                fprintf(stdout, "Invalid command line argument: %s\n", optarg);
                        }

                        // Unknown
                        else
                        {
                            PrintCmdLineHelp(argv[0]);
                            fprintf(stdout, "\n");
                            gDone = true;
                        }
                        break;

                    case COLOR_CYCLE_EXAMPLE_SHORT_CMD:
                        ColorCycleExample();
                        break;

                    case COMPASS_EXAMPLE_SHORT_CMD:
                        CompassExample();
                        break;

                    case ENVIRONMENT_SHORT_CMD:
                        GetEnvironment();
                        break;

                    case FLASH_SHORT_CMD:
                        Flash();
                        break;

                    case HEADING_SHORT_CMD:
                        GetHeading();
                        break;

                    case HELP_SHORT_CMD:
                        PrintCmdNumHelp();
                        break;

                    case LOAD_IMAGE_SHORT_CMD:
                        LoadImage(optarg);
                        break;

                    case RAINBOW_EXAMPLE_SHORT_CMD:
                        RainbowExample();
                        break;

                    case ROTATION_EXAMPLE_SHORT_CMD:
                        RotationExample();
                        break;

                    case SHOW_LETTER_SHORT_CMD:
                        ShowLetter(optarg);
                        break;

                    case SHOW_MESSAGE_SHORT_CMD:
                        ShowMessage(optarg);
                        break;

                    case TEXT_SCROLL_EXAMPLE_SHORT_CMD:
                        TextScrollExample();
                        break;

                    case VERSION_SHORT_CMD:
                        GetVersion();
                        break;

                    case WAIT_FOR_EVENT_SHORT_CMD:
                        if (strncmp(optarg, kUpEvent, strlen(kUpEvent)) == 0)
                            WaitForEvent(eSenseHAT_JoystickDirectionUp);
                        else if (strncmp(optarg, kDownEvent, strlen(kDownEvent)) == 0)
                            WaitForEvent(eSenseHAT_JoystickDirectionDown);
                        else if (strncmp(optarg, kLeftEvent, strlen(kLeftEvent)) == 0)
                            WaitForEvent(eSenseHAT_JoystickDirectionLeft);
                        else if (strncmp(optarg, kRightEvent, strlen(kRightEvent)) == 0)
                            WaitForEvent(eSenseHAT_JoystickDirectionRight);
                        else if (strncmp(optarg, kPushEvent, strlen(kPushEvent)) == 0)
                            WaitForEvent(eSenseHAT_JoystickDirectionPush);
                        else
                            fprintf(stdout, "Invalid command line argument: %s\n", optarg);
                        break;

                    // Unknown
                    case UNKNOWN_SHORT_CMD:
                    {
                        PrintCmdLineHelp(argv[0]);
                        fprintf(stdout, "\n");
                        break;
                    }
                    default:
                        break;
                }
            }
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
                fprintf(stdout, "\n");
                if (scanResult == EOF)
                {
                    done = true;
                    fprintf(stdout, "\nQuitting...\n\n");
                }
                else if (scanResult == 0)
                    fprintf(stdout, "Invalid command.\n\n");

                else
                {
                    switch (cmdNum)
                    {
                        case HELP_CMD_NUM:
                            break;

                        case VERSION_CMD_NUM:
                            GetVersion();
                            break;

                        case FLASH_CMD_NUM:
                            Flash();
                            break;

                        case COLOR_CYCLE_EXAMPLE_CMD_NUM:
                            ColorCycleExample();
                            break;

                        case COMPASS_EXAMPLE_CMD_NUM:
                            CompassExample();
                            break;

                        case RAINBOW_EXAMPLE_CMD_NUM:
                            RainbowExample();
                            break;

                        case ROTATION_EXAMPLE_CMD_NUM:
                            RotationExample();
                            break;

                        case TEXT_SCROLL_EXAMPLE_CMD_NUM:
                            TextScrollExample();
                            break;

                        case LOAD_IMAGE_CMD_NUM:
                        {
                            // Prompt for file path
                            memset(&(cmdBuf[0]), 0, 128);
                            fprintf(stdout, "Enter image file path: ");
                            fgets(cmdBuf, sizeof(cmdBuf), stdin);
                            scanResult = sscanf(cmdBuf, "%s", cmdBuf);
                            result = UNTHINK_CHECK_CONDITION((scanResult == 1), EINVAL);
                            if (result == UNTHINK_SUCCESS)
                                LoadImage(cmdBuf);
                            break;
                        }
                        case ROTATE_CMD_NUM:
                        {
                            // Prompt for degrees of rotation
                            memset(&(cmdBuf[0]), 0, 128);
                            fprintf(stdout, "Available rotations:\n");
                            fprintf(stdout, "1 - 0\n");
                            fprintf(stdout, "2 - 90\n");
                            fprintf(stdout, "3 - 180\n");
                            fprintf(stdout, "4 - 270\n");
                            fprintf(stdout, "Enter rotation: ");
                            fgets(cmdBuf, sizeof(cmdBuf), stdin);
                            scanResult = sscanf(cmdBuf, "%d", &cmdNum);
                            if ((scanResult == EOF) || (scanResult == 0))
                                fprintf(stdout, "Invalid rotation!\n\n");
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
                                        fprintf(stdout, "Invalid rotation!\n");
                                        break;
                                }
                            }
                            break;
                        }
                        case SHOW_LETTER_CMD_NUM:
                        {
                            // Prompt for letter
                            memset(&(cmdBuf[0]), 0, 128);
                            fprintf(stdout, "Enter letter: ");
                            fgets(cmdBuf, sizeof(cmdBuf), stdin);
                            scanResult = sscanf(cmdBuf, "%s", cmdBuf);
                            if (scanResult == 1)
                                ShowLetter(cmdBuf);
                            else
                                fprintf(stdout, "Invalid input!\n");
                            break;
                        }
                        case SHOW_MESSAGE_CMD_NUM:
                        {
                            // Prompt for message
                            memset(&(cmdBuf[0]), 0, 128);
                            fprintf(stdout, "Enter message: ");
                            fgets(cmdBuf, sizeof(cmdBuf), stdin);
                            ShowMessage(cmdBuf);
                            break;
                        }
                        case FLIP_HORIZONTAL_CMD_NUM:
                            FlipHorizontal(true);
                            break;

                        case FLIP_VERTICAL_CMD_NUM:
                            FlipVertical(true);
                            break;

                        case HEADING_CMD_NUM:
                            GetHeading();
                            break;

                        case ENVIRONMENT_CMD_NUM:
                            GetEnvironment();
                            break;

                        case WAIT_FOR_EVENT_CMD_NUM:
                        {
                            // Prompt for event
                            memset(&(cmdBuf[0]), 0, 128);
                            fprintf(stdout, "Available events:\n");
                            fprintf(stdout, "1 - Up\n");
                            fprintf(stdout, "2 - Down\n");
                            fprintf(stdout, "3 - Left\n");
                            fprintf(stdout, "4 - Right\n");
                            fprintf(stdout, "5 - Push\n");
                            fprintf(stdout, "Enter event: ");
                            fgets(cmdBuf, sizeof(cmdBuf), stdin);
                            scanResult = sscanf(cmdBuf, "%d", &cmdNum);
                            if ((scanResult == EOF) || (scanResult == 0))
                                fprintf(stdout, "Invalid event.\n\n");

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
                                        fprintf(stdout, "Invalid event.\n\n");
                                        break;
                                }
                            }
                            break;
                        }
                        case QUIT_CMD_NUM:
                            done = true;
                            fprintf(stdout, "\nQuitting...\n\n");
                            break;

                        default:
                            fprintf(stdout, "Invalid command.\n\n");
                            break;
                    }
                }
            }
        }

        // Clear the LEDs 
        (void)SenseHAT_LEDClear(gInstance, &kBackColor);

        // Close the instance
        (void)SenseHAT_Close(&gInstance);
    }
    else    // SenseHAT_Open failed
        fprintf(stdout, "SenseHAT_Open failed!\n");

    return retValue;
}

// =================================================================================================
//  GetVersion
// =================================================================================================
void GetVersion (void)
{
    uint32_t version = SenseHAT_Version();
    fprintf(stdout, "Raspberry Pi Sense HAT C library version is %d.%d.%d.\n", 
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
    int32_t result = UNTHINK_SUCCESS;
    fprintf(stdout, "Flashing LEDs... Enter ctrl-c to stop.\n");

    while (!gDone)
    {
        result = SenseHAT_LEDSetPixels(gInstance, kRedArray);
        if (result == UNTHINK_SUCCESS)
        {
            sleep(1);
            result = SenseHAT_LEDSetPixels(gInstance, kGreenArray);
            if (result == UNTHINK_SUCCESS)
            {
                sleep(1);
                result = SenseHAT_LEDSetPixels(gInstance, kBlueArray);
                if (result == UNTHINK_SUCCESS)
                {
                    sleep(1);
                    result = SenseHAT_LEDClear(gInstance, &kBackColor);
                    if (result != UNTHINK_SUCCESS)
                    {
                        fprintf(stdout, "SenseHAT_LEDClear failed!\n");
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

    if (result == UNTHINK_SUCCESS)
        (void)SenseHAT_LEDClear(gInstance, NULL);

    gDone = false;

    return;
}

// =================================================================================================
//  ColorCycleExample
// =================================================================================================
void ColorCycleExample (void)
{
    int32_t result = UNTHINK_SUCCESS;
    fprintf(stdout, "Color cycling LEDs... Enter ctrl-c to stop.\n");
    result = SenseHAT_LEDClear(gInstance, NULL);
    if (result == UNTHINK_SUCCESS)
    {
        useconds_t sleepTime = 2;
        tSenseHAT_LEDPixel color = {255, 0, 0};

        while (!gDone)
        {
            result = SenseHAT_LEDClear(gInstance, &color);
            if (result == UNTHINK_SUCCESS)
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
                fprintf(stdout, "SenseHAT_LEDClear failed!\n");
                gDone = true;
            }
        }

        if (result == UNTHINK_SUCCESS)
            (void)SenseHAT_LEDClear(gInstance, NULL);

        gDone = false;
    }
    else
        fprintf(stdout, "SenseHAT_LEDClear failed!\n");

    return;
}

// =================================================================================================
//  RainbowExample
// =================================================================================================
void RainbowExample (void)
{
    int32_t result = UNTHINK_SUCCESS;
    fprintf(stdout, "Animating rainbow on LEDs... Enter ctrl-c to stop.\n");
    result = SenseHAT_LEDClear(gInstance, NULL);
    if (result == UNTHINK_SUCCESS)
    {
        useconds_t sleepTime = 2;
        tSenseHAT_LEDPixelArray pixels;
        uint32_t index = 0;
        int32_t red = 0;
        int32_t green = 0;
        int32_t blue = 0;

        memcpy((void*)pixels, (void*)kRainbowSeedArray, sizeof(tSenseHAT_LEDPixelArray));

        result = SenseHAT_LEDSetPixels(gInstance, pixels);
        if (result == UNTHINK_SUCCESS)
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

            if (result == UNTHINK_SUCCESS)
                (void)SenseHAT_LEDClear(gInstance, NULL);

            gDone = false;
        }
        else    // SenseHAT_LEDSetPixels failed
            fprintf(stdout, "SenseHAT_LEDSetPixels failed!\n");
    }
    else    // SenseHAT_LEDClear failed
        fprintf(stdout, "SenseHAT_LEDClear failed!\n");

    return;
}

// =================================================================================================
//  RotationExample
// =================================================================================================
void RotationExample (void)
{
    int32_t result = UNTHINK_SUCCESS;

    result = SenseHAT_LEDSetPixels(gInstance, kQuestionMarkArray);
    if (result == UNTHINK_SUCCESS)
    {
        tSenseHAT_LEDPixel redColor = kRedColor;
        result = SenseHAT_LEDSetPixel(gInstance, 0, 0, &redColor);
        if (result == UNTHINK_SUCCESS)
        {
            tSenseHAT_LEDPixel blueColor = kBlueColor;
            result = SenseHAT_LEDSetPixel(gInstance, 0, 7, &blueColor);
            if (result == UNTHINK_SUCCESS)
            {
                tSenseHAT_LEDPixel greenColor = kGreenColor;
                result = SenseHAT_LEDSetPixel(gInstance, 7, 0, &greenColor);
                if (result == UNTHINK_SUCCESS)
                {
                    tSenseHAT_LEDPixel purpleColor = kPurpleColor;
                    result = SenseHAT_LEDSetPixel(gInstance, 7, 7, &purpleColor);
                    if (result == UNTHINK_SUCCESS)
                    {
                        uint32_t i = 0;
                        fprintf(stdout, "Rotating... Enter ctrl-c to stop.\n");
                        while (!gDone)
                        {
                            result = SenseHAT_LEDSetRotation(gInstance, (tSenseHAT_LEDRotation)i, true);
                            if (result == UNTHINK_SUCCESS)
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
                                fprintf(stdout, "SenseHAT_LEDSetRotation failed!\n");
                                gDone = true;
                            }
                        }
                    }
                    else
                        fprintf(stdout, "SenseHAT_LEDSetPixel failed!\n");
                }
                else
                    fprintf(stdout, "SenseHAT_LEDSetPixel failed!\n");
            }
            else
                fprintf(stdout, "SenseHAT_LEDSetPixel failed!\n");
        }
        else
            fprintf(stdout, "SenseHAT_LEDSetPixel failed!\n");
    }
    else
        fprintf(stdout, "SenseHAT_LEDSetPixels failed!\n");

    if (result == UNTHINK_SUCCESS)
        (void)SenseHAT_LEDClear(gInstance, NULL);

    gDone = false;

    return;
}

// =================================================================================================
//  LoadImage
// =================================================================================================
void LoadImage (char* imageFilePath)
{
    int32_t result = UNTHINK_SUCCESS;
    fprintf(stdout, "Loading image...\n");
    result = SenseHAT_LEDLoadImage(gInstance, imageFilePath, true, NULL);
    if (result != UNTHINK_SUCCESS)
        fprintf(stdout, "SenseHAT_LEDLoadImage failed!\n");

    return;
}

// =================================================================================================
//  Rotate
// =================================================================================================
void Rotate (tSenseHAT_LEDRotation rotation)
{
    int32_t result = UNTHINK_SUCCESS;
    fprintf(stdout, "Rotating...\n");
    result = SenseHAT_LEDSetRotation(gInstance, rotation, true);
    if (result != UNTHINK_SUCCESS)
        fprintf(stdout, "SenseHAT_LEDSetRotation failed!\n");

    return;
}

// =================================================================================================
//  ShowLetter
// =================================================================================================
void ShowLetter (char* letter)
{
    int32_t result = UNTHINK_SUCCESS;
    fprintf(stdout, "Showing letter...\n");
    result = SenseHAT_LEDShowLetter(gInstance, letter, &kTextColor, &kBackColor);
    if (result == UNTHINK_SUCCESS)
        sleep(1);
    else
        fprintf(stdout, "SenseHAT_LEDShowLetter failed!\n");

    return;
}

// =================================================================================================
//  ShowMessage
// =================================================================================================
void ShowMessage (char* message)
{
    int32_t result = UNTHINK_SUCCESS;
    fprintf(stdout, "Showing message...\n");
    result = SenseHAT_LEDShowMessage(gInstance, message, 0.05, &kTextColor, &kBackColor);
    if (result == UNTHINK_SUCCESS)
        sleep(1);
    else
        fprintf(stdout, "SenseHAT_LEDShowMessage failed!\n");

    return;
}

// =================================================================================================
//  FlipHorizontal
// =================================================================================================
void FlipHorizontal (bool flip)
{
    int32_t result = UNTHINK_SUCCESS;
    fprintf(stdout, "Flipping horizontally...\n");
    result = SenseHAT_LEDFlipHorizontal(gInstance, flip, NULL);
    if (result != UNTHINK_SUCCESS)
        fprintf(stdout, "SenseHAT_LEDFlipHorizontal failed!\n");

    return;
}

// =================================================================================================
//  FlipVertical
// =================================================================================================
void FlipVertical (bool flip)
{
    int32_t result = UNTHINK_SUCCESS;
    fprintf(stdout, "Flipping vertically...\n");
    result = SenseHAT_LEDFlipVertical(gInstance, flip, NULL);
    if (result != UNTHINK_SUCCESS)
        fprintf(stdout, "SenseHAT_LEDFlipVertical failed!\n");

    return;
}

// =================================================================================================
//  GetHeading
// =================================================================================================
void GetHeading (void)
{
    double heading = 0.0;
    int32_t result = UNTHINK_SUCCESS;
    result = SenseHAT_GetCompass(gInstance, &heading);
    if (result == UNTHINK_SUCCESS)
    {
        char msg[128];
        memset((void*)msg, 0, 128);
        sprintf(msg, "Compass heading is %.2f degrees.\n", heading);
        fprintf(stdout, msg);
        (void)ShowMessage(msg);
    }
    else
        fprintf(stdout, "SenseHAT_GetCompass failed!\n");

    return;
}

// =================================================================================================
//  GetEnvironment
// =================================================================================================
void GetEnvironment (void)
{
    double humidity = 0.0;
    int32_t result = UNTHINK_SUCCESS;
    result = SenseHAT_GetHumidity(gInstance, &humidity);
    if (result == 0)
    {
        double pressure = 0.0;
        char msg[128];
        memset((void*)msg, 0, 128);
        sprintf(msg, "Humidity is %.2f percent relative humidity.\n", humidity);
        fprintf(stdout, msg);

        result = SenseHAT_GetPressure(gInstance, &pressure);
        if (result == UNTHINK_SUCCESS)
        {
            double temperature = 0.0;

            memset((void*)msg, 0, 128);
            sprintf(msg, "Pressure is %.2f millibars.\n", pressure);
            fprintf(stdout, msg);

            result = SenseHAT_GetTemperature(gInstance, &temperature);
            if (result == UNTHINK_SUCCESS)
            {
                double tempF = ((temperature * 9.0) / 5.0) + 32.0;
                char msg[128];
                memset((void*)msg, 0, 128);
                sprintf(msg, "Temperature is %.2f degrees Celsius, %.2f degrees Fahrenheit.\n", temperature, tempF);
                fprintf(stdout, msg);
            }
            else
                printf("SenseHAT_GetTemperature failed!\n");
        }
        else
            fprintf(stdout, "SenseHAT_GetPressure failed!\n");
    }
    else
        fprintf(stdout, "SenseHAT_GetHumidity failed!\n");

    return;
}

// =================================================================================================
//  WaitForEvent
// =================================================================================================
void WaitForEvent (tSenseHAT_JoystickDirection direction)
{
    tSenseHAT_JoystickEvent event;
    bool gotEvent = false;
    int32_t result = UNTHINK_SUCCESS;

    fprintf(stdout, "Waiting for event... Enter ctrl-c to stop.\n");
    while (!gotEvent && !gDone)
    {
        result = SenseHAT_WaitForEvent(gInstance, true, &event);
        if (result == UNTHINK_SUCCESS)
        {
            if (event.direction == direction)
            {
                gotEvent = true;
                break;
            }
        }
        else
        {
            fprintf(stdout, "SenseHAT_WaitForEvent failed!\n");
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
    int32_t result = UNTHINK_SUCCESS;

    result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation0, true);
    if (result == UNTHINK_SUCCESS)
    {
        result = SenseHAT_LEDClear(gInstance, NULL);
        if (result == UNTHINK_SUCCESS)
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

            fprintf(stdout, "Tracking compass... Enter ctrl-c to stop.\n");
            while (!gDone)
            {
                result = SenseHAT_GetCompass(gInstance, &degrees);
                if (result == UNTHINK_SUCCESS)
                {
                    degreesInverted = 360 - degrees;
                    ledIndex = (int32_t)(ledDegreeRatio * degreesInverted);
                    offset = ledLoop[ledIndex];

                    y = offset / 8;
                    x = offset % 8;

                    if ((x != prevX) || (y != prevY))
                    {
                        result = SenseHAT_LEDSetPixel(gInstance, prevX, prevY, NULL);
                        if (result != UNTHINK_SUCCESS)
                        {
                            fprintf(stdout, "SenseHAT_LEDSetPixel failed!\n");
                            gDone = true;
                        }
                    }

                    if (!gDone)
                    {
                        result = SenseHAT_LEDSetPixel(gInstance, x, y, &color);
                        if (result == UNTHINK_SUCCESS)
                        {   
                            prevX = x; 
                            prevY = y;
                        }
                        else
                        {
                            fprintf(stdout, "SenseHAT_LEDSetPixel failed!\n");
                            gDone = true;
                        }
                    }
                }
            }
        }
        else
            fprintf(stdout, "SenseHAT_LEDClear failed!\n");
    }
    else
        fprintf(stdout, "SenseHAT_LEDSetRotation failed!\n");

    gDone = false;

    return;
}

// =================================================================================================
//  TextScrollExample
// =================================================================================================
void TextScrollExample (void)
{
    int32_t result = UNTHINK_SUCCESS;

    result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation180, true);
    if (result == UNTHINK_SUCCESS)
    {
        tSenseHAT_LEDPixel color = kRedColor;

        result = SenseHAT_LEDShowMessage(gInstance,
                                         "One small step for Pi!",
                                         0.05,
                                         &color,
                                         NULL);
        if (result == UNTHINK_SUCCESS)
        {
            result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation0, true);
            if (result != UNTHINK_SUCCESS)
                fprintf(stdout, "SenseHAT_LEDSetRotation failed!\n");
        }
        else
            fprintf(stdout, "SenseHAT_LEDShowMessage failed!\n");
    }
    else 
        fprintf(stdout, "SenseHAT_LEDSetRotation failed!\n");

    return;
}
// =================================================================================================
