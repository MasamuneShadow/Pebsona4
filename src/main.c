#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "http.h"
#include "util.h"
#include "config.h"
#include <time.h>

	
//#define MY_UUID { 0x01, 0x6E, 0x11, 0x1F, 0xB2, 0x60, 0x4E, 0x80, 0xA7, 0xCD, 0x78, 0x35, 0xA0, 0x31, 0xE3, 0xF3 } //OG PEBSONA 4
#define MY_UUID { 0x91, 0x41, 0xB6, 0x28, 0xBC, 0x89, 0x49, 0x8E, 0xB1, 0x47, 0x04, 0x9F, 0x49, 0xC0, 0x99, 0xAD }
//find a way to have this work without using this id!
PBL_APP_INFO(MY_UUID,
             "Pebsona4", "Masamune_Shadow",
             1, 8, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

// POST variables
#define WEATHER_KEY_LATITUDE 1
#define WEATHER_KEY_LONGITUDE 2

// Received variables
#define WEATHER_KEY_ICON 1
		
#define WEATHER_HTTP_COOKIE 1949327671
#define TIME_HTTP_COOKIE 1131038282
	
#define DATE "Date"
#define TIME "Time"
#define WORD "Word"
#define WEATHER "Weather"
#define ALL "All"

#define timeWidth 19
#define timeHeight 19
#define timePosY 55
	
#define	dateFrame  (GRect(0,2,100,26))
#define	wordDateFrame  (GRect(90,10,50,26))
#define	wordFrame  (GRect(0,32,144,21))
#define	weatherFrame  (GRect(53,87,75,75))

#define	timeFrame  (GRect(0,timePosY,144,25))
#define	timeHourTensFrame  (GRect(0,timePosY,timeWidth,timeHeight))
#define	timeHourOnesFrame  (GRect(20,timePosY,timeWidth,timeHeight))
#define	timeColonFrame  (GRect(40,timePosY,8,timeHeight))
#define	timeMinuteTensFrame  (GRect(48,timePosY,timeWidth,timeHeight))
#define	timeMinuteOnesFrame  (GRect(68,timePosY,timeWidth, timeHeight))

	
//Once weather support gets introduced, the "Sunny" icon will change to reflect said weather.
//ingame there are:
/*
Sunny		=	1
Umbrella	=	2
Raindrops	
Cloud		=	3 
Snowman		=	4
Snow
Thunder
Fog
(All are a combo of these)
*/

/* Word Time Enums
Midnight	=	0
EarlyMorning=	1
Morning		=	2
Lunchtime	=	3 
Afternoon	=	4
Evening		=	5
Night		=	6
*/



Window window;
/*=========Layers============*/
TextLayer 	layerDate; //white
TextLayer 	layerDateWord; //white
BitmapLayer layerTimeH1_WHITE;
BitmapLayer layerTimeH2_WHITE;
BitmapLayer layerTimeM1_WHITE;
BitmapLayer layerTimeM2_WHITE;
BitmapLayer layerTimeH1_BLACK;
BitmapLayer layerTimeH2_BLACK;
BitmapLayer layerTimeM1_BLACK;
BitmapLayer layerTimeM2_BLACK;

BitmapLayer layerColon_WHITE;
BitmapLayer layerColon_BLACK;

BitmapLayer layerWord_WHITE;
BitmapLayer layerWord_BLACK;
BitmapLayer layerWeather;
	#define INTRO_DAY1_ICON 1
	#define INTRO_DAY2_ICON 2
	#define INTRO_DAY3_ICON 3
	#define INTRO_DAY4_ICON 4
/*
#define INTRO_DAY1_DATE 1
	#define INTRO_DAY1_WORD_DAY 2
	#define INTRO_DAY1_ICON 3
	#define INTRO_DAY2_DATE 4
	#define INTRO_DAY2_WORD_DAY 5
	#define INTRO_DAY2_ICON 6
	#define INTRO_DAY3_DATE 7
	#define INTRO_DAY3_WORD_DAY 8
	#define INTRO_DAY3_ICON 9
	#define INTRO_DAY4_DATE 10
	#define INTRO_DAY4_WORD_DAY 11
	#define INTRO_DAY4_ICON 12


About the Day to Day Transitions...
God damn call is too slow and I don't think there is anything I can do about it without causing an inexcusable power drain.
So commenting out for now, then going to work on the W2W Transition & Calls from my website.
So sorry about this.
	*/
//structs added later, don't want to fix what isn't broken, so I won't be going back and updating the previous code.
// Into Trans Defines
//Animation Defines
#define openAnimationDuration 500
#define closingAnimationDuration 500
#define slideAnimationDuration 600
#define openAnimationDelay 500
#define closingAnimationDelay 500
#define slideAnimationDelay 250
#define instantDuration 1

#define INTRO_START_RECT (GRect(72,0,0,168))
#define INTRO_END_RECT (GRect(46,0,52,168))

//all of the images will be of the same size as the square, but thanks to the inverts, it'll work out w/ trans.
#define day0PosX -50
#define day1PosX 0
#define day2PosX 52
#define day3PosX 100
#define day4PosX 147

#define dayImgPosY 119
#define dayImgRecX 40
#define dayImgRecY 38
#define dayWordPosY 95
#define dayWordRecX 40
#define dayWordRecY 18
#define dayDayPosY 78
#define dayDayRecX 42
#define dayDayRecY 25

#define introMonth0PosX 46		
#define introMonth1PosX 98		
#define introMonthPosY 0
#define introMonthRecX 52
#define introMonthRecY 42
		
#define introYear0PosX 46		
#define introYear1PosX 98		
#define introYearPosY 50
#define introYearRecX 52
#define introYearRecY 26

#define INTRO_MONTH_0_FRAME (GRect(introMonth0PosX,introMonthPosY,introMonthRecX,introMonthRecY))
#define INTRO_YEAR_0_FRAME (GRect(introYear0PosX,introYearPosY,introYearRecX,introYearRecY))
		
#define INTRO_MONTH_1_FRAME (GRect(introMonth1PosX,introMonthPosY,introMonthRecX,introMonthRecY))
#define INTRO_YEAR_1_FRAME (GRect(introYear1PosX,introYearPosY,introYearRecX,introYearRecY))

#define DAY_0_IMAGE_FRAME (GRect(day0PosX,dayImgPosY,0,dayImgRecY))
#define DAY_1_IMAGE_FRAME (GRect(day1PosX,dayImgPosY,dayImgRecX,dayImgRecY))
#define DAY_2_IMAGE_FRAME (GRect(day2PosX,dayImgPosY,dayImgRecX,dayImgRecY))//highlighed frame
#define	DAY_3_IMAGE_FRAME (GRect(day3PosX,dayImgPosY,dayImgRecX,dayImgRecY))
#define DAY_4_IMAGE_FRAME (GRect(day4PosX,dayImgPosY,dayImgRecX,dayImgRecY))
#define DAY_0_WORD_FRAME (GRect(day0PosX,dayWordPosY,0,dayWordRecY))
#define DAY_1_WORD_FRAME (GRect(day1PosX,dayWordPosY,dayWordRecX,dayWordRecY))
#define DAY_2_WORD_FRAME (GRect(day2PosX,dayWordPosY,dayWordRecX,dayWordRecY))
#define DAY_3_WORD_FRAME (GRect(day3PosX,dayWordPosY,dayWordRecX,dayWordRecY))
#define DAY_4_WORD_FRAME (GRect(day4PosX,dayWordPosY,dayWordRecX,dayWordRecY))	
#define DAY_0_DAY_FRAME  (GRect(day0PosX,dayDayPosY,0,dayDayRecY))
#define DAY_1_DAY_FRAME  (GRect(day1PosX,dayDayPosY,dayDayRecX,dayDayRecY))
#define DAY_2_DAY_FRAME  (GRect(day2PosX,dayDayPosY,dayDayRecX,dayDayRecY))
#define DAY_3_DAY_FRAME  (GRect(day3PosX,dayDayPosY,dayDayRecX,dayDayRecY))
#define DAY_4_DAY_FRAME  (GRect(day4PosX,dayDayPosY,dayDayRecX,dayDayRecY))

typedef struct dayTransDay{
	TextLayer 		layerWord;
	TextLayer 		layerDay;
	BitmapLayer 	layerWeather;
	BitmapLayer 	layerWeatherSplitTop_WHITE;
	BitmapLayer 	layerWeatherSplitTop_BLACK;
	BitmapLayer 	layerWeatherSplitBottom_WHITE;
	BitmapLayer 	layerWeatherSplitBottom_BLACK;
	BmpContainer 	imgWeather;
	BmpContainer 	imgWeatherSplitTop_WHITE;
	BmpContainer 	imgWeatherSplitTop_BLACK;
	BmpContainer 	imgWeatherSplitBottom_WHITE;
	BmpContainer 	imgWeatherSplitBottom_BLACK;
	PropertyAnimation slideLeftWordAnimation;
	PropertyAnimation slideLeftDayAnimation;
	PropertyAnimation slideLeftImgAnimation;
	GRect wordRectStart;
	GRect dayRectStart;
	GRect imgRectStart;
	GRect wordRectEnd;
	GRect dayRectEnd;
	GRect imgRectEnd;
	bool isSplit;
	int previousWeather;
	int previousWeatherSplitTop;
	int previousWeatherSplitBottom;
	int currentWeather;
	int currentWeatherSplitTop;
	int currentWeatherSplitBottom;
	char date[3];
	char wordDay[3];
}dayTransDay;
/*typedef struct introHeader{
	TextLayer Month;
	TextLayer Year[]
}introHeader;*/
#define NUMBER_OF_DAYS 4
typedef struct dayTrans{
	BitmapLayer layerDayTrans;
	BitmapLayer layerBackground;
	InverterLayer layerInvert;
	BitmapLayer layerClosingLeft;
	BitmapLayer layerClosingRight;
	TextLayer introMonth0;
	TextLayer introYear0;
	GRect introMonth0Frame;
	GRect introYear0Frame;
	TextLayer introMonth1;
	TextLayer introYear1;
	GRect introMonth1Frame;
	GRect introYear1Frame;
	GRect startRect;
	GRect endRect;
	dayTransDay day[NUMBER_OF_DAYS]; //4
	PblTm tm;
	PebbleTickEvent tick;
	PropertyAnimation openingAnimation;
	PropertyAnimation closingAnimationLeft;	
	PropertyAnimation closingAnimationRight;
	PropertyAnimation holdAnimation;	
}dayTrans;
dayTrans dayTransition;

//Word Transitions
//Animation Defines
#define startAnimationDuration 500
#define exitAnimationDuration 500
#define flipAnimationDuration 600
#define startAnimationDelay 250
#define exitAnimationDelay 250
#define flipAnimationDelay 250
//#define LAYER_TEXT_YEAR_FRAME (GRect(46,50,52,26))
#define WORD_TRANSITION_START_RECT (GRect(144,0,52,168))
#define WORD_TRANSITION_END_RECT (GRect(-46,0,52,168))

//all of the images will be of the same size as the square, but thanks to the inverts, it'll work out w/ trans.
//fix the defines, make them app. for this transition.
#define squarePosX 145
#define squarePosY 18
#define squareRecX 108
#define squareRecY 108
#define	wordFrame  (GRect(0,32,144,21))
#define WORD_TEXT_FRAME (GRect(squarePosX,61,144,21))
//#define WORD_SQUARE_FRAME_START (GRect(squarePosX,squarePosY,squareRecX,squareRecY))
	#define WORD_SQUARE_FRAME_START (GRect(15,15,15,15))
#define WORD_SQUARE_FRAME_HOLD (GRect(72,squarePosY,squareRecX,squareRecY))
#define WORD_SQUARE_FRAME_END (GRect(-108,squarePosY,squareRecX,squareRecY))
#define	BACKGROUND_FRAME (GRect(0,0,144,168)) //the size of the window.
	
typedef struct wordTrans{
	BitmapLayer layerBackground;
	BitmapLayer layerSquare;
	//InverterLayer layerIn;
	BitmapLayer layerWord_WHITE;
	BitmapLayer layerWord_BLACK;
	BmpContainer square;
	GRect layerWordFrame;
	GRect startRect;
	GRect holdRect;
	//GRect holdHalfRect;
	GRect endRect;	
	GRect backgroundRect;
	PropertyAnimation wordAnimationStart_WHITE;
	PropertyAnimation wordAnimationStart_BLACK;
	PropertyAnimation wordAnimationHold_WHITE;
	PropertyAnimation wordAnimationHold_BLACK;
	PropertyAnimation wordAnimationExit_WHITE;
	PropertyAnimation wordAnimationExit_BLACK;
	PropertyAnimation startAnimation;	
	PropertyAnimation exitAnimation;
	PropertyAnimation flipAnimationFirstHalf;
	PropertyAnimation flipAnimationSecondHalf;
	PropertyAnimation holdAnimation;	
	int previousWordHolder;
	int currentWordHolder;	
}wordTrans;
wordTrans wordTransition;
	
static const int DIGIT_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DIGIT_0,
  RESOURCE_ID_IMAGE_DIGIT_1,
  RESOURCE_ID_IMAGE_DIGIT_2,
  RESOURCE_ID_IMAGE_DIGIT_3,
  RESOURCE_ID_IMAGE_DIGIT_4,
  RESOURCE_ID_IMAGE_DIGIT_5,
  RESOURCE_ID_IMAGE_DIGIT_6,
  RESOURCE_ID_IMAGE_DIGIT_7,
  RESOURCE_ID_IMAGE_DIGIT_8,
  RESOURCE_ID_IMAGE_DIGIT_9
};

static const int DIGIT_IMAGE_RESOURCE_IDS_INVERT[] = {
  RESOURCE_ID_IMAGE_DIGIT_0_INVERT,
  RESOURCE_ID_IMAGE_DIGIT_1_INVERT,
  RESOURCE_ID_IMAGE_DIGIT_2_INVERT,
  RESOURCE_ID_IMAGE_DIGIT_3_INVERT,
  RESOURCE_ID_IMAGE_DIGIT_4_INVERT,
  RESOURCE_ID_IMAGE_DIGIT_5_INVERT,
  RESOURCE_ID_IMAGE_DIGIT_6_INVERT,
  RESOURCE_ID_IMAGE_DIGIT_7_INVERT,
  RESOURCE_ID_IMAGE_DIGIT_8_INVERT,
  RESOURCE_ID_IMAGE_DIGIT_9_INVERT
};

BmpContainer background_image;

#define TOTAL_TIME_DIGITS 4
static BmpContainer imgTime_BLACK[TOTAL_TIME_DIGITS];
static int imgTimeResourceIds_BLACK[TOTAL_TIME_DIGITS];

static BmpContainer imgTime_WHITE[TOTAL_TIME_DIGITS];
static int imgTimeResourceIds_WHITE[TOTAL_TIME_DIGITS];

BmpContainer imgWord_BLACK;
BmpContainer imgWord_WHITE;
BmpContainer imgColon_BLACK;
BmpContainer imgColon_WHITE;
BmpContainer imgWeather;

int currentWord;
//int currentMinute;
int currentM1;
int currentM2;
//int currentHour;
int currentH1;
int currentH2;
int currentDay;
int currentWeather;

int previousWord;
//int previousMinute;
int previousHour;
int previousDay;
int previousWeather;

GFont fontMonth; 
GFont fontYear;	
GFont fontDate;
GFont fontAbbr;
//GFont font Size 12;
ResHandle resDate;
ResHandle resAbbr;
ResHandle resMonth;	
ResHandle resYear;	
//ResHandle res Font 12;

/*=========Bools============*/
bool isTVPingTransition = false;
bool isDayTransition = false;
bool isWordTransition = false;
bool isTransitioning = false;
bool watchfaceStarted = false;
bool fIntroWeatherGet = false;
bool fAbleToPlaceSuccessfully = false;
bool Initializing = true;
	
//Weather Stuff
static int our_latitude, our_longitude;
static bool located = false;
int monthMaxDay[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
void WordTransition(AppContextRef ctx);

void SetWordImage()
{	
	if (!isTransitioning)
	{
		if (currentWord == 0)//midnight
		{
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_MIDNIGHT,&imgWord_BLACK);
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_MIDNIGHT_INVERT,&imgWord_WHITE);
		}		
		else if (currentWord == 1) //early morning
		{
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_EARLY_MORNING,&imgWord_BLACK);
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_EARLY_MORNING_INVERT,&imgWord_WHITE);
		}
		else if(currentWord == 2) //morning
		{	
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_MORNING,&imgWord_BLACK);
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_MORNING_INVERT,&imgWord_WHITE);
		}
		else if(currentWord == 3)//lunchtime
		{
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_LUNCHTIME,&imgWord_BLACK);
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_LUNCHTIME_INVERT,&imgWord_WHITE);
		}
		else if(currentWord == 4)//afternoon
		{
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_AFTERNOON,&imgWord_BLACK);
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_AFTERNOON_INVERT,&imgWord_WHITE);
		}
		else if(currentWord == 5)//evening
		{
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_EVENING,&imgWord_BLACK);
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_EVENING_INVERT,&imgWord_WHITE);
		}
		else if(currentWord == 6)//night
		{
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_NIGHT,&imgWord_BLACK);
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_NIGHT_INVERT,&imgWord_WHITE);
		}
		else if(currentWord == 7)//Unknown
		{
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_BLANK,&imgWord_BLACK);
			bmp_init_container(RESOURCE_ID_IMAGE_TIME_BLANK,&imgWord_WHITE);
		}
	}
}

void SetWeatherImage()
{	
	if (!isTransitioning)
	{
		if (currentWeather == 0) //sun
		{
			bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SUN,&imgWeather);
		}
		else if(currentWeather == 1)//cloud
		{
			bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_CLOUD,&imgWeather);
		}
		else if(currentWeather == 2) //rain
		{	
			bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_RAIN,&imgWeather);
		}
		else if(currentWeather == 3 )//snow
		{
			bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SNOW,&imgWeather);
		}
		else if (currentWeather >= 4)
		{
			bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_UNKNOWN,&imgWeather);
		}
	}
	else
	{
	//decypher the dates, but for now just set them all to sunny
		for (int i = 0; i < 4; i++)
		{
			fIntroWeatherGet = true;
			if (!dayTransition.day[i].isSplit)
			{
				if (dayTransition.day[i].currentWeather == 0)//sunny
				{	
					bmp_init_container(RESOURCE_ID_IMAGE_SQUARE_SUN,&dayTransition.day[i].imgWeather);
				}
				else if (dayTransition.day[i].currentWeather == 1) //clouds	
				{
					bmp_init_container(RESOURCE_ID_IMAGE_SQUARE_CLOUD,&dayTransition.day[i].imgWeather);
				}
				else if(dayTransition.day[i].currentWeather == 2) //rain
				{
					bmp_init_container(RESOURCE_ID_IMAGE_SQUARE_RAIN,&dayTransition.day[i].imgWeather);
				}
				else if (dayTransition.day[i].currentWeather == 3) //snow				
				{
					bmp_init_container(RESOURCE_ID_IMAGE_SQUARE_SNOW,&dayTransition.day[i].imgWeather);
				}
				else if(dayTransition.day[i].currentWeather >= 4) //unknown
				{
					bmp_init_container(RESOURCE_ID_IMAGE_SQUARE_UNKNOWN,&dayTransition.day[i].imgWeather);
				}
			}
			
			else
			{
				//tops
				bmp_init_container(RESOURCE_ID_IMAGE_SQUARE_SUN_INVERT,&dayTransition.day[i].imgWeatherSplitTop_WHITE);
				bmp_init_container(RESOURCE_ID_IMAGE_SQUARE_SUN,&dayTransition.day[i].imgWeatherSplitTop_BLACK);
								
				//bottom
				bmp_init_container(RESOURCE_ID_IMAGE_SQUARE_SUN_INVERT,&dayTransition.day[i].imgWeatherSplitBottom_WHITE);
				bmp_init_container(RESOURCE_ID_IMAGE_SQUARE_SUN,&dayTransition.day[i].imgWeatherSplitBottom_BLACK);			
			}
        }
	}
}


void SetTimeImage()
{	
	bmp_init_container(DIGIT_IMAGE_RESOURCE_IDS[currentH1],&imgTime_BLACK[0]);
	bmp_init_container(DIGIT_IMAGE_RESOURCE_IDS[currentH2],&imgTime_BLACK[1]);
	bmp_init_container(DIGIT_IMAGE_RESOURCE_IDS[currentM1],&imgTime_BLACK[2]);
	bmp_init_container(DIGIT_IMAGE_RESOURCE_IDS[currentM2],&imgTime_BLACK[3]);

	bmp_init_container(DIGIT_IMAGE_RESOURCE_IDS_INVERT[currentH1],&imgTime_WHITE[0]);
	bmp_init_container(DIGIT_IMAGE_RESOURCE_IDS_INVERT[currentH2],&imgTime_WHITE[1]);
	bmp_init_container(DIGIT_IMAGE_RESOURCE_IDS_INVERT[currentM1],&imgTime_WHITE[2]);
	bmp_init_container(DIGIT_IMAGE_RESOURCE_IDS_INVERT[currentM2],&imgTime_WHITE[3]);

	bmp_init_container(RESOURCE_ID_IMAGE_DIGIT_COLON,&imgColon_BLACK);
	bmp_init_container(RESOURCE_ID_IMAGE_DIGIT_COLON_INVERT,&imgColon_WHITE);
	
}

void RemoveAndDeIntBmp(char* bitmap)
{
	
	if (strcmp(bitmap,WORD) == 0 || strcmp(bitmap,ALL) == 0)
	{
		if (!isTransitioning)
		{
			layer_remove_from_parent(&layerWord_WHITE.layer);
			layer_remove_from_parent(&layerWord_BLACK.layer);
		}
		else if (isWordTransition)
		{
			layer_remove_from_parent(&wordTransition.layerWord_WHITE.layer);
			layer_remove_from_parent(&wordTransition.layerWord_BLACK.layer);
		}
		bmp_deinit_container(&imgWord_WHITE);
		bmp_deinit_container(&imgWord_BLACK);
	}
	
	if (strcmp (bitmap,WEATHER) == 0 || strcmp(bitmap,ALL) == 0)
	{
		if (!isTransitioning)
		{
			layer_remove_from_parent(&layerWeather.layer);
			bmp_deinit_container(&imgWeather);
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if (!dayTransition.day[i].isSplit)
				{
					layer_remove_from_parent(&dayTransition.day[i].layerWeather.layer);
					bmp_deinit_container(&dayTransition.day[i].imgWeather);
				}
				else
				{
					layer_remove_from_parent(&dayTransition.day[i].layerWeatherSplitTop_WHITE.layer);
					bmp_deinit_container(&dayTransition.day[i].imgWeatherSplitTop_WHITE);
					layer_remove_from_parent(&dayTransition.day[i].layerWeatherSplitBottom_WHITE.layer);
					bmp_deinit_container(&dayTransition.day[i].imgWeatherSplitBottom_WHITE);
					layer_remove_from_parent(&dayTransition.day[i].layerWeatherSplitTop_BLACK.layer);
					bmp_deinit_container(&dayTransition.day[i].imgWeatherSplitTop_BLACK);
					layer_remove_from_parent(&dayTransition.day[i].layerWeatherSplitBottom_BLACK.layer);
					bmp_deinit_container(&dayTransition.day[i].imgWeatherSplitBottom_BLACK);
				}
			}
		}
	}

	if (strcmp (bitmap,TIME) == 0 || strcmp(bitmap,ALL) == 0)
	{
		layer_remove_from_parent(&layerTimeH1_WHITE.layer);
		layer_remove_from_parent(&layerTimeH2_WHITE.layer);
		layer_remove_from_parent(&layerTimeM1_WHITE.layer);
		layer_remove_from_parent(&layerTimeM2_WHITE.layer);

		layer_remove_from_parent(&layerColon_WHITE.layer);
		layer_remove_from_parent(&layerColon_BLACK.layer);

		layer_remove_from_parent(&layerTimeH1_BLACK.layer);
		layer_remove_from_parent(&layerTimeH2_BLACK.layer);
		layer_remove_from_parent(&layerTimeM1_BLACK.layer);
		layer_remove_from_parent(&layerTimeM2_BLACK.layer);


		bmp_deinit_container(&imgTime_WHITE[0]);
		bmp_deinit_container(&imgTime_WHITE[1]);
		bmp_deinit_container(&imgTime_WHITE[2]);
		bmp_deinit_container(&imgTime_WHITE[3]);

		bmp_deinit_container(&imgColon_WHITE);
		bmp_deinit_container(&imgColon_BLACK);

		bmp_deinit_container(&imgTime_BLACK[0]);
		bmp_deinit_container(&imgTime_BLACK[1]);
		bmp_deinit_container(&imgTime_BLACK[2]);
		bmp_deinit_container(&imgTime_BLACK[3]);
	}
}

//for bitmap only (black and white)
//bitmap = layer that we want to manipulate
void SetBitmap(char* bitmap)
{
	if (!Initializing && !isTransitioning)
	{
		RemoveAndDeIntBmp(bitmap);
	}
	if (strcmp(bitmap,WORD) == 0)
	{    
		SetWordImage();			
		//black background, takes the white and makes them black, all else are not drawn
		if (isTransitioning)
		{
			bitmap_layer_init(&wordTransition.layerWord_WHITE,wordTransition.layerWordFrame);
			bitmap_layer_set_bitmap(&wordTransition.layerWord_WHITE,&imgWord_WHITE.bmp);
			bitmap_layer_set_background_color(&wordTransition.layerWord_WHITE,GColorClear);
			bitmap_layer_set_compositing_mode(&wordTransition.layerWord_WHITE, GCompOpClear);
			layer_add_child(&window.layer, &wordTransition.layerWord_WHITE.layer);
			layer_mark_dirty(&wordTransition.layerWord_WHITE.layer);
		}
		else
		{
			bitmap_layer_init(&layerWord_WHITE,wordFrame);
			bitmap_layer_set_bitmap(&layerWord_WHITE,&imgWord_WHITE.bmp);
			bitmap_layer_set_background_color(&layerWord_WHITE,GColorClear);
			bitmap_layer_set_compositing_mode(&layerWord_WHITE, GCompOpClear);
			layer_add_child(&window.layer, &layerWord_WHITE.layer);
			layer_mark_dirty(&layerWord_WHITE.layer);
		}
		
		if (isTransitioning)
		{
			bitmap_layer_init(&layerWord_BLACK,wordFrame);
			bitmap_layer_set_bitmap(&layerWord_BLACK, &imgWord_BLACK.bmp); //or .layer?
			bitmap_layer_set_background_color(&layerWord_BLACK,GColorClear);
			bitmap_layer_set_compositing_mode(&layerWord_BLACK, GCompOpOr);
			layer_add_child(&window.layer, &layerWord_BLACK.layer);
			layer_mark_dirty(&layerWord_BLACK.layer);	
		}
		else
		{
			bitmap_layer_init(&layerWord_BLACK,wordFrame);
			bitmap_layer_set_bitmap(&layerWord_BLACK, &imgWord_BLACK.bmp); //or .layer?
			bitmap_layer_set_background_color(&layerWord_BLACK,GColorClear);
			bitmap_layer_set_compositing_mode(&layerWord_BLACK, GCompOpOr);
			layer_add_child(&window.layer, &layerWord_BLACK.layer);
			layer_mark_dirty(&layerWord_BLACK.layer);	
		}
	
	}
	else if (strcmp(bitmap,WEATHER) == 0)
	{
		SetWeatherImage();
		if(!isTransitioning)
		{
			bitmap_layer_init(&layerWeather,weatherFrame);
			bitmap_layer_set_bitmap(&layerWeather,&imgWeather.bmp);
			bitmap_layer_set_background_color(&layerWeather,GColorClear);
			bitmap_layer_set_compositing_mode(&layerWeather, GCompOpOr);
			layer_add_child(&window.layer, &layerWeather.layer);
			layer_mark_dirty(&layerWeather.layer);		
			if (previousWeather != currentWeather)
			{
				previousWeather = currentWeather;
			}
		}
		else
		{
			if (fAbleToPlaceSuccessfully && fIntroWeatherGet)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!dayTransition.day[i].isSplit)
					{
						
						bitmap_layer_init(&dayTransition.day[i].layerWeather, dayTransition.day[i].imgRectStart);
						bitmap_layer_set_bitmap(&dayTransition.day[i].layerWeather,&dayTransition.day[i].imgWeather.bmp);
						bitmap_layer_set_background_color(&dayTransition.day[i].layerWeather,GColorClear);
						bitmap_layer_set_compositing_mode(&dayTransition.day[i].layerWeather, GCompOpAssign);
						layer_add_child(&window.layer, &dayTransition.day[i].layerWeather.layer);
						layer_mark_dirty(&dayTransition.day[i].layerWeather.layer);		
						if (dayTransition.day[i].previousWeather != dayTransition.day[i].currentWeather)
						{
							dayTransition.day[i].previousWeather = dayTransition.day[i].currentWeather;
						}
					}
					else
					{
					//TOP WHITE
						bitmap_layer_init(&dayTransition.day[i].layerWeatherSplitTop_WHITE, dayTransition.day[i].imgRectStart);
						bitmap_layer_set_bitmap(&dayTransition.day[i].layerWeatherSplitTop_WHITE,&dayTransition.day[i].imgWeatherSplitTop_WHITE.bmp);
						bitmap_layer_set_background_color(&dayTransition.day[i].layerWeatherSplitTop_WHITE,GColorClear);
						bitmap_layer_set_compositing_mode(&dayTransition.day[i].layerWeatherSplitTop_WHITE, GCompOpClear);
						layer_add_child(&window.layer, &dayTransition.day[i].layerWeatherSplitTop_WHITE.layer);
						layer_mark_dirty(&dayTransition.day[i].layerWeatherSplitTop_WHITE.layer);
					//TOP BLACK
						bitmap_layer_init(&dayTransition.day[i].layerWeatherSplitTop_BLACK, dayTransition.day[i].imgRectStart);
						bitmap_layer_set_bitmap(&dayTransition.day[i].layerWeatherSplitTop_BLACK,&dayTransition.day[i].imgWeatherSplitTop_BLACK.bmp);
						bitmap_layer_set_background_color(&dayTransition.day[i].layerWeatherSplitTop_BLACK,GColorClear);
						bitmap_layer_set_compositing_mode(&dayTransition.day[i].layerWeatherSplitTop_BLACK, GCompOpOr);
						layer_add_child(&window.layer, &dayTransition.day[i].layerWeatherSplitTop_BLACK.layer);
						layer_mark_dirty(&dayTransition.day[i].layerWeatherSplitTop_BLACK.layer);	
					//TOP PREV/CUR
						if (dayTransition.day[i].previousWeatherSplitTop != dayTransition.day[i].currentWeatherSplitTop)
						{
							dayTransition.day[i].previousWeatherSplitTop = dayTransition.day[i].currentWeatherSplitTop;
						}
					//BOTTOM WHITE
						bitmap_layer_init(&dayTransition.day[i].layerWeatherSplitBottom_WHITE, dayTransition.day[i].imgRectStart);
						bitmap_layer_set_bitmap(&dayTransition.day[i].layerWeatherSplitBottom_WHITE,&dayTransition.day[i].imgWeatherSplitBottom_WHITE.bmp);
						bitmap_layer_set_background_color(&dayTransition.day[i].layerWeatherSplitBottom_WHITE,GColorClear);
						bitmap_layer_set_compositing_mode(&dayTransition.day[i].layerWeatherSplitBottom_WHITE, GCompOpClear);
						layer_add_child(&window.layer, &dayTransition.day[i].layerWeatherSplitBottom_WHITE.layer);
						layer_mark_dirty(&dayTransition.day[i].layerWeatherSplitBottom_WHITE.layer);
					//BOTTOM BLACK
						bitmap_layer_init(&dayTransition.day[i].layerWeatherSplitBottom_BLACK, dayTransition.day[i].imgRectStart);
						bitmap_layer_set_bitmap(&dayTransition.day[i].layerWeatherSplitBottom_BLACK,&dayTransition.day[i].imgWeatherSplitBottom_BLACK.bmp);
						bitmap_layer_set_background_color(&dayTransition.day[i].layerWeatherSplitBottom_BLACK,GColorClear);
						bitmap_layer_set_compositing_mode(&dayTransition.day[i].layerWeatherSplitBottom_BLACK, GCompOpOr);
						layer_add_child(&window.layer, &dayTransition.day[i].layerWeatherSplitBottom_BLACK.layer);
						layer_mark_dirty(&dayTransition.day[i].layerWeatherSplitBottom_BLACK.layer);		
					//BOTTOM PREV/CUR
						if (dayTransition.day[i].previousWeatherSplitBottom != dayTransition.day[i].currentWeatherSplitBottom)
						{
							dayTransition.day[i].previousWeatherSplitBottom = dayTransition.day[i].currentWeatherSplitBottom;
						}
					}
				}
			}
			fIntroWeatherGet = false;
			fAbleToPlaceSuccessfully = false;
		}
	}
	else if (strcmp(bitmap,TIME) == 0)
	{
		SetTimeImage();			
		//do 2 times (1 white, 1 black)
		//do 4 times (1 for each digit) 
		//8 times total (yuck)
	//White 1
		bitmap_layer_init(&layerTimeH1_WHITE,timeHourTensFrame);
		bitmap_layer_set_bitmap(&layerTimeH1_WHITE, &imgTime_WHITE[0].bmp); //or .layer?
		bitmap_layer_set_background_color(&layerTimeH1_WHITE,GColorClear);
		bitmap_layer_set_compositing_mode(&layerTimeH1_WHITE, GCompOpClear);
		layer_add_child(&window.layer, &layerTimeH1_WHITE.layer);
		layer_mark_dirty(&layerTimeH1_WHITE.layer);
	//White 2		
		bitmap_layer_init(&layerTimeH2_WHITE,timeHourOnesFrame);
		bitmap_layer_set_bitmap(&layerTimeH2_WHITE, &imgTime_WHITE[1].bmp); //or .layer?
		bitmap_layer_set_background_color(&layerTimeH2_WHITE,GColorClear);
		bitmap_layer_set_compositing_mode(&layerTimeH2_WHITE, GCompOpClear);
		layer_add_child(&window.layer, &layerTimeH2_WHITE.layer);
		layer_mark_dirty(&layerTimeH2_WHITE.layer);
	//White 3
		bitmap_layer_init(&layerTimeM1_WHITE,timeMinuteTensFrame);
		bitmap_layer_set_bitmap(&layerTimeM1_WHITE, &imgTime_WHITE[2].bmp); //or .layer?
		bitmap_layer_set_background_color(&layerTimeM1_WHITE,GColorClear);
		bitmap_layer_set_compositing_mode(&layerTimeM1_WHITE, GCompOpClear);
		layer_add_child(&window.layer, &layerTimeM1_WHITE.layer);
		layer_mark_dirty(&layerTimeM1_WHITE.layer);
	//White 4	
		bitmap_layer_init(&layerTimeM2_WHITE,timeMinuteOnesFrame);
		bitmap_layer_set_bitmap(&layerTimeM2_WHITE, &imgTime_WHITE[3].bmp); //or .layer?
		bitmap_layer_set_background_color(&layerTimeM2_WHITE,GColorClear);
		bitmap_layer_set_compositing_mode(&layerTimeM2_WHITE, GCompOpClear);
		layer_add_child(&window.layer, &layerTimeM2_WHITE.layer);
		layer_mark_dirty(&layerTimeM2_WHITE.layer);
	//Colon White
		bitmap_layer_init(&layerColon_WHITE,timeColonFrame);
		bitmap_layer_set_bitmap(&layerColon_WHITE, &imgColon_WHITE.bmp); //or .layer?
		bitmap_layer_set_background_color(&layerColon_WHITE,GColorClear);
		bitmap_layer_set_compositing_mode(&layerColon_WHITE, GCompOpClear);
		layer_add_child(&window.layer, &layerColon_WHITE.layer);
		layer_mark_dirty(&layerColon_WHITE.layer);

	//Colon Black
		bitmap_layer_init(&layerColon_BLACK,timeColonFrame);
		bitmap_layer_set_bitmap(&layerColon_BLACK, &imgColon_BLACK.bmp); //or .layer?
		bitmap_layer_set_background_color(&layerColon_BLACK,GColorClear);
		bitmap_layer_set_compositing_mode(&layerColon_BLACK, GCompOpOr);
		layer_add_child(&window.layer, &layerColon_BLACK.layer);
		layer_mark_dirty(&layerColon_BLACK.layer);
	//Black 1		
		bitmap_layer_init(&layerTimeH1_BLACK,timeHourTensFrame);
		bitmap_layer_set_bitmap(&layerTimeH1_BLACK, &imgTime_BLACK[0].bmp); //or .layer?
		bitmap_layer_set_background_color(&layerTimeH1_BLACK,GColorClear);
		bitmap_layer_set_compositing_mode(&layerTimeH1_BLACK, GCompOpOr);
		layer_add_child(&window.layer, &layerTimeH1_BLACK.layer);
		layer_mark_dirty(&layerTimeH1_BLACK.layer);
	//Black 2
		bitmap_layer_init(&layerTimeH2_BLACK,timeHourOnesFrame);
		bitmap_layer_set_bitmap(&layerTimeH2_BLACK, &imgTime_BLACK[1].bmp); //or .layer?
		bitmap_layer_set_background_color(&layerTimeH2_BLACK,GColorClear);
		bitmap_layer_set_compositing_mode(&layerTimeH2_BLACK, GCompOpOr);
		layer_add_child(&window.layer, &layerTimeH2_BLACK.layer);
		layer_mark_dirty(&layerTimeH2_BLACK.layer);
	//Black 3	
		bitmap_layer_init(&layerTimeM1_BLACK,timeMinuteTensFrame);
		bitmap_layer_set_bitmap(&layerTimeM1_BLACK, &imgTime_BLACK[2].bmp); //or .layer?
		bitmap_layer_set_background_color(&layerTimeM1_BLACK,GColorClear);
		bitmap_layer_set_compositing_mode(&layerTimeM1_BLACK, GCompOpOr);
		layer_add_child(&window.layer, &layerTimeM1_BLACK.layer);
		layer_mark_dirty(&layerTimeM1_BLACK.layer);
	//Black 4
		bitmap_layer_init(&layerTimeM2_BLACK,timeMinuteOnesFrame);
		bitmap_layer_set_bitmap(&layerTimeM2_BLACK, &imgTime_BLACK[3].bmp); //or .layer?
		bitmap_layer_set_background_color(&layerTimeM2_BLACK,GColorClear);
		bitmap_layer_set_compositing_mode(&layerTimeM2_BLACK, GCompOpOr);
		layer_add_child(&window.layer, &layerTimeM2_BLACK.layer);
		layer_mark_dirty(&layerTimeM2_BLACK.layer);
	}
}


void request_weather();
void handle_timer(AppContextRef app_ctx, AppTimerHandle handle, uint32_t cookie);

void failed(int32_t cookie, int http_status, void* context) {
	if(cookie == 0 || cookie == WEATHER_HTTP_COOKIE) 
	{
		if (isTransitioning && isDayTransition)
		{
			fIntroWeatherGet = false;
			dayTransition.day[0].currentWeather = 11;
			dayTransition.day[1].currentWeather = 11;
			dayTransition.day[2].currentWeather = 11;
			dayTransition.day[3].currentWeather = 11;
		}
		currentWeather = 11;
		SetBitmap(WEATHER);
	}
}

void success(int32_t cookie, int http_status, DictionaryIterator* received, void* context) 
{
	if(cookie != WEATHER_HTTP_COOKIE) return;
	if (!isTransitioning)
	{
		
		Tuple* icon_tuple = dict_find(received, WEATHER_KEY_ICON);
		int icon = icon_tuple->value->int8;
		/*static char lat[20];
		mini_snprintf(lat, 20, "%d", icon);
		text_layer_set_text(&layerDate, lat);*/
		if(icon >= 0 && icon < 5)
		{
			currentWeather = icon;
		}
		else
		{
			currentWeather = 11;
		}
		SetBitmap(WEATHER);
	}
	else if (isTransitioning && isDayTransition)
	{
		
		if(cookie != WEATHER_HTTP_COOKIE) return;
		
		Tuple*	 icon  = dict_find(received,INTRO_DAY1_ICON);
		Tuple*	 icon2 = dict_find(received,INTRO_DAY2_ICON);
		Tuple*	 icon3 = dict_find(received,INTRO_DAY3_ICON);
		Tuple*	 icon4 = dict_find(received,INTRO_DAY4_ICON);
		
		dayTransition.day[0].currentWeather = icon->value->int8;
		dayTransition.day[1].currentWeather = icon2->value->int8;
		dayTransition.day[2].currentWeather = icon3->value->int8;
		dayTransition.day[3].currentWeather = icon4->value->int8;		
		
		fIntroWeatherGet = true;
		//vibes_short_pulse();
		SetBitmap(WEATHER);
	}
}
void requestIntroWeather();

void reconnect(void* context) {
	requestIntroWeather();
	request_weather();
}

void set_timer(AppContextRef ctx) {
	app_timer_send_event(ctx, 1740000, 1);
}

void location(float latitude, float longitude, float altitude, float accuracy, void* context) {
	// Fix the floats	
	our_latitude = latitude * 10000;
	our_longitude = longitude * 10000;
	
	/*static char lat[20];
	mini_snprintf(lat, 20, "%d", our_latitude);
	text_layer_set_text(&layerDate, lat);
	
	static char lon[20];
	mini_snprintf(lon, 20, "%d", our_longitude);
	text_layer_set_text(&layerDateWord, lon);*/
	
	located = true;
	requestIntroWeather();
	request_weather();
	set_timer((AppContextRef)context);
}

void handle_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie) {
	requestIntroWeather();
	request_weather();
	// Update again in fifteen minutes.
	if(cookie)
		set_timer(ctx);
}



void GetAndSetCurrentWord(PblTm* currentTime)
{
    switch (currentTime->tm_hour)
    {
        case 0:
            currentWord = 0; //midnight
            break;
        case 3:
        case 4:
        case 5:
            currentWord = 1; //early morning
            break;
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            currentWord = 2; //morning
            break;
        case 11:
		case 12:
            currentWord = 3;//lunchtime
            break;
		case 13:
        case 14:
        case 15:
        case 16:
		case 17:
            currentWord = 4;//afternoon
            break;
        case 18:
        case 19:
		case 20:
            currentWord = 5;//evening
            break;
        case 21:
        case 22:
        case 23:
        case 24:
		case 1:
		case 2:
            currentWord = 6;//night
            break;
    }    
	if (previousWord != currentWord)
	{
		SetBitmap(WORD);
		previousWord = currentWord;
	}
}

void DayTransition(AppContextRef ctx);
	
void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;
  (void)ctx;
	
	if (!isTransitioning)
	{
		static char dateText[] = "00 00";
		string_format_time(dateText, sizeof(dateText), "%m/%d", t->tick_time);
		text_layer_set_text(&layerDate, dateText);
		
		static char dateWordText[] = " Xxx";
		string_format_time(dateWordText, sizeof(dateWordText), " %a", t->tick_time);
		
		//uppercase it, should be by itself, but fuck it.
		for (int i = 0; dateWordText[i] != 0; i++) {
			if (dateWordText[i] >= 'a' && dateWordText[i] <= 'z') {
				dateWordText[i] -= 0x20;
			}
		}
		
		text_layer_set_text(&layerDateWord, dateWordText);
		
		static char timeText[] = "00:00";
		char *timeFormat;
		timeFormat = clock_is_24h_style() ? "%R" : "%I:%M";
		string_format_time(timeText, sizeof(timeText), timeFormat, t->tick_time);


		// Kludge to handle lack of non-padded hour format string
		// for twelve hour clock.
		if (!clock_is_24h_style() && (timeText[0] == '0'))
		{
			memmove(timeText, &timeText[1], sizeof(timeText) - 1);
		}
		//not right, should be by itself but don't want to pass around w/e

		int hour = t->tick_time->tm_hour;
	
		if (!clock_is_24h_style()) 
		{
			hour = hour % 12;
			if (hour == 0) 
			{
				hour = 12;
			}	
		}	
		int value = hour %100;

		currentH1 = value / 10;
		currentH2 = value % 10;
		value = t->tick_time->tm_min %100;
		currentM1 = value / 10;
		currentM2 = value % 10;

		SetBitmap(TIME);
		
		if(!located || !(t->tick_time->tm_min % 15))
		{
			//Every 15 minutes, request updated weather			
			request_weather();
			requestIntroWeather();
		}
		
		if (previousHour != hour)
		{
			//currentDay = 	t->tick_time->tm_mday;
			previousHour = hour;
			GetAndSetCurrentWord(t->tick_time);
		}
		
		if (!Initializing && currentDay != t->tick_time->tm_mday)
		{
			//will get set properly in the transition.
			DayTransition(ctx);
			//deint watchface?
		}
	}
}

void RefreshAll()
{
    SetBitmap(TIME);
	SetBitmap(WORD);
	SetBitmap(WEATHER);
}


void Watchface()//DISPLAYS THE "WATCH PART" OR, NO TRANSITIONS ARE RUNNING
{
	if (!isTransitioning)
	{		
		window_set_background_color(&window, GColorClear);
		
		layer_remove_from_parent(&background_image.layer.layer);
		bmp_deinit_container(&background_image);
		
		bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND,  &background_image);
		layer_add_child(&window.layer,&background_image.layer.layer);
			
		text_layer_init(&layerDate, window.layer.frame);
		text_layer_set_text_color(&layerDate, GColorWhite);
		text_layer_set_background_color(&layerDate, GColorClear);
		layer_set_frame(&layerDate.layer,dateFrame);
		text_layer_set_font(&layerDate, fontDate);
		layer_add_child(&window.layer, &layerDate.layer);
	
		text_layer_init(&layerDateWord, window.layer.frame);
		text_layer_set_text_color(&layerDateWord, GColorWhite);
		text_layer_set_background_color(&layerDateWord, GColorClear);
		layer_set_frame(&layerDateWord.layer,wordDateFrame);
		text_layer_set_font(&layerDateWord, fontAbbr);
		layer_add_child(&window.layer, &layerDateWord.layer);	
	
		//DIGIT_IMAGE_RESOURCE_IDS
		for (int i = 0; i < TOTAL_TIME_DIGITS; i++) {
			bmp_init_container(RESOURCE_ID_IMAGE_DIGIT_0, &imgTime_BLACK[i]);
			imgTimeResourceIds_BLACK[i] = -1;
		}
	
		for (int i = 0; i < TOTAL_TIME_DIGITS; i++) {
			bmp_init_container(RESOURCE_ID_IMAGE_DIGIT_0_INVERT, &imgTime_WHITE[i]);
			imgTimeResourceIds_WHITE[i] = -1;
		}
		
		if (!Initializing)
		{
			RefreshAll();
		}
	}
}

void StartWatchface(AppContextRef ctx)
{
	//this could very well have gone in the handle_init, but for now atleast, it's here.
	//this should only ever be called once, and only once.
	//request_weather();
	(void) ctx;
	isTransitioning = false;
	isDayTransition = false;
	//isWordTransition = false;
	Watchface(ctx);
	watchfaceStarted = true;
	PblTm tm;
    PebbleTickEvent t;
	//run into transition	
	// Refresh time
	get_time(&tm);
    t.tick_time = &tm;
    t.units_changed = SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT | DAY_UNIT;
	handle_minute_tick(ctx, &t);
	//request_weather();
	Initializing = false;
}

void ClosingAnimationStopped(Animation* animation, bool finished, void* context)
{

	layer_remove_from_parent(&dayTransition.introMonth0.layer);
	layer_remove_from_parent(&dayTransition.introYear0.layer);
	layer_remove_from_parent(&dayTransition.introMonth1.layer);
	layer_remove_from_parent(&dayTransition.introYear1.layer);
	layer_remove_from_parent(&dayTransition.layerInvert.layer);

	for (int i = 0; i < 4; i++)
	{
		{
			layer_remove_from_parent(&dayTransition.day[i].layerDay.layer);
			layer_remove_from_parent(&dayTransition.day[i].layerWord.layer);
		}
	}
	RemoveAndDeIntBmp(WEATHER);
	
	layer_remove_from_parent(&dayTransition.layerDayTrans.layer);
	layer_remove_from_parent(&dayTransition.layerClosingLeft.layer);
	layer_remove_from_parent(&dayTransition.layerClosingRight.layer);
	layer_remove_from_parent(&dayTransition.layerBackground.layer);
	fAbleToPlaceSuccessfully = false;
	if (!watchfaceStarted)
	{
		StartWatchface(context);
	}
	else
	{
		//static char wordText[20];
		//strftime(wordText,20,"%c", dayTransition.day[1].date);
		//text_layer_set_text(&layerDateWord, wordText);
		
		isTransitioning = false;
		isDayTransition = false;
		request_weather();
	}
}

void HoldAnimationStopped(Animation* animation, bool finished, void* context) 
{
	//setup closing animation
	GRect closingLeftRect = GRect(0,0,46,168);
	GRect closingEndLeftRect = GRect(0,0,98,168);
	GRect closingRightRect = GRect(98,0,98,168);	
	GRect closingEndRightRect = GRect(46,0,98,168);
	
	bitmap_layer_init(&dayTransition.layerClosingLeft,closingLeftRect);
	bitmap_layer_set_background_color(&dayTransition.layerClosingLeft, GColorBlack);
	layer_add_child(&window.layer, &dayTransition.layerClosingLeft.layer);
	bitmap_layer_init(&dayTransition.layerClosingRight,closingRightRect);
	bitmap_layer_set_background_color(&dayTransition.layerClosingRight, GColorBlack);
	layer_add_child(&window.layer, &dayTransition.layerClosingRight.layer);
	/*if (fNeedNewMonth)
	delete the old month
	if (fNeedNewYear)
	delete the old year
	//CHECK OUT HOW IT WORKS IN THE GAME/ANIME, THAT'S THE GOLDEN STANDARD (HAR HAR)*/
	property_animation_init_layer_frame(&dayTransition.closingAnimationLeft, &dayTransition.layerClosingLeft.layer,&closingLeftRect,&closingEndLeftRect);
	animation_set_duration( &dayTransition.closingAnimationLeft.animation, closingAnimationDuration);
	animation_set_delay(&dayTransition.closingAnimationLeft.animation, closingAnimationDelay);
	
	property_animation_init_layer_frame(&dayTransition.closingAnimationRight, &dayTransition.layerClosingRight.layer,&closingRightRect,&closingEndRightRect);
	animation_set_duration( &dayTransition.closingAnimationRight.animation, closingAnimationDuration);
	animation_set_delay(&dayTransition.closingAnimationRight.animation, closingAnimationDelay);
	//handlers on the right only.
	animation_set_handlers(&dayTransition.closingAnimationRight.animation, (AnimationHandlers){
	.stopped = (AnimationStoppedHandler)ClosingAnimationStopped
	}, context);
	animation_schedule(&dayTransition.closingAnimationLeft.animation);
	animation_schedule(&dayTransition.closingAnimationRight.animation);
}

void SlidingAnimationStopped(Animation* animation, bool finished, void* context) 
{
	//hold it there for a few
	property_animation_init_layer_frame(&dayTransition.holdAnimation, &dayTransition.layerDayTrans.layer,&dayTransition.endRect,&dayTransition.endRect);
	animation_set_duration( &dayTransition.holdAnimation.animation, openAnimationDuration); //500
	animation_set_handlers(&dayTransition.holdAnimation.animation, (AnimationHandlers){
	.stopped = (AnimationStoppedHandler)HoldAnimationStopped
	}, context);
	animation_schedule(&dayTransition.holdAnimation.animation);
}
//t current day #
//get current word day
	
void OffsetDays(PblTm* inputTime,int offset)
{

	//offset can be + or -, it will act the same 
	inputTime->tm_mday = inputTime->tm_mday + offset;
		
	if ((inputTime->tm_mon == 0 && inputTime->tm_mday > 30) || (inputTime->tm_mon == 1 && inputTime->tm_mday >27) || (inputTime->tm_mon == 2 && inputTime->tm_mday < 3))
	{
		//calc leap year
		if((inputTime->tm_year%400==0) || (inputTime->tm_year%4==0))
			monthMaxDay[1] = 29;
		else
			monthMaxDay[1] = 28;
	}
	
	//now for the offset Days part
	if (inputTime->tm_mday > monthMaxDay[inputTime->tm_mon])
	{
		if (inputTime->tm_mon != 11)
			inputTime->tm_mday =- monthMaxDay[inputTime->tm_mon+1];
		else
		  inputTime->tm_mday =- monthMaxDay[0];
	}
	else if (inputTime->tm_mday <= 0)
		inputTime->tm_mday = monthMaxDay[inputTime->tm_mon-1] - inputTime->tm_mday;
}

void SetWordDayOfWeek(int iOffsetDayOfWeek, int i)
{	
	while (!(iOffsetDayOfWeek >= 1 && iOffsetDayOfWeek <= 7))
	{
		if (iOffsetDayOfWeek < 1)
			iOffsetDayOfWeek += 7;
		else if (iOffsetDayOfWeek > 7)
			iOffsetDayOfWeek -= 7;
	}
	//dayTransition.day[i].wordDay = "";
	switch (iOffsetDayOfWeek)
	{
		case 1:
			strcpy(dayTransition.day[i].wordDay,(char*)"MON");
			break;
		case 2:
			strcpy(dayTransition.day[i].wordDay,(char*)"TUE");
			break;
		case 3:
			strcpy(dayTransition.day[i].wordDay,(char*)"WED");
			break;
		case 4:
			strcpy(dayTransition.day[i].wordDay,(char*)"THU");
			break;
		case 5:
			strcpy(dayTransition.day[i].wordDay,(char*)"FRI");
			break;
		case 6:
			strcpy(dayTransition.day[i].wordDay,(char*)"SAT");
			break;	
		case 7:
			strcpy(dayTransition.day[i].wordDay,(char*)"SUN");
			break;
	}
}
void SetupSlidingFrames()
{
	PblTm tm;
	int iOffset;
	for (int i = 0; i < 4; i++)
	{
		get_time(&tm);
		//static char wordDay[] = " Xxx";
		static char dayText[] = " 00";	
		static char dayOfWeek[] = " 0";
		if (i == 0)
		{
			dayTransition.day[i].wordRectStart = DAY_1_WORD_FRAME;
			dayTransition.day[i].dayRectStart = DAY_1_DAY_FRAME;
			dayTransition.day[i].imgRectStart = DAY_1_IMAGE_FRAME;
			dayTransition.day[i].wordRectEnd = DAY_0_WORD_FRAME;
			dayTransition.day[i].dayRectEnd = DAY_0_DAY_FRAME;
			dayTransition.day[i].imgRectEnd = DAY_0_IMAGE_FRAME;
			iOffset = -2;
		}	
		else if (i == 1)
		{
			dayTransition.day[i].wordRectStart = DAY_2_WORD_FRAME;
			dayTransition.day[i].dayRectStart = DAY_2_DAY_FRAME;
			dayTransition.day[i].imgRectStart = DAY_2_IMAGE_FRAME;
			dayTransition.day[i].wordRectEnd = DAY_1_WORD_FRAME;
			dayTransition.day[i].dayRectEnd = DAY_1_DAY_FRAME;
			dayTransition.day[i].imgRectEnd = DAY_1_IMAGE_FRAME;
			iOffset = -1;
		}
		else if (i == 2)
		{
			dayTransition.day[i].wordRectStart = DAY_3_WORD_FRAME;
			dayTransition.day[i].dayRectStart = DAY_3_DAY_FRAME;
			dayTransition.day[i].imgRectStart = DAY_3_IMAGE_FRAME;
			dayTransition.day[i].wordRectEnd = DAY_2_WORD_FRAME;
			dayTransition.day[i].dayRectEnd = DAY_2_DAY_FRAME;
			dayTransition.day[i].imgRectEnd = DAY_2_IMAGE_FRAME;
			iOffset = 0;
		}
		else if (i == 3)
		{
			dayTransition.day[i].wordRectStart = DAY_4_WORD_FRAME;
			dayTransition.day[i].dayRectStart = DAY_4_DAY_FRAME;
			dayTransition.day[i].imgRectStart = DAY_4_IMAGE_FRAME;
			dayTransition.day[i].wordRectEnd = DAY_3_WORD_FRAME;
			dayTransition.day[i].dayRectEnd = DAY_3_DAY_FRAME;
			dayTransition.day[i].imgRectEnd = DAY_3_IMAGE_FRAME;
			iOffset = 1;
		}
		//dayTransition.day[i].date = " 00";
		//dayTransition.day[i].wordDay = " Xxx";
		OffsetDays(&tm,iOffset);
		string_format_time(dayTransition.day[i].date, sizeof(dayText), "%d",&tm);
		string_format_time(dayOfWeek, sizeof(dayOfWeek), "%u",&tm);
		int iOffsetDayOfWeek = 0;
		
		if (dayOfWeek[0] ==  '1')
		{
			iOffsetDayOfWeek = 1;
		}
		else if (dayOfWeek[0] ==  '2')
		{
			iOffsetDayOfWeek = 2;
		}
		else if (dayOfWeek[0] ==  '3')
		{
			iOffsetDayOfWeek = 3;
		}
		else if (dayOfWeek[0] == '4')
		{
			iOffsetDayOfWeek = 4;
		}
		else if (dayOfWeek[0] ==  '5')
		{
			iOffsetDayOfWeek = 5;
		}
		else if (dayOfWeek[0] == '6')
		{
			iOffsetDayOfWeek = 6;
		}
		else if (dayOfWeek[0] ==  '7')
		{
			iOffsetDayOfWeek = 7;
		}
		
		iOffsetDayOfWeek += iOffset;
		
		SetWordDayOfWeek(iOffsetDayOfWeek, i);
		dayTransition.day[i].previousWeather = 11;
		//for now it'll be the same day on all of them, but we'll have to implement maths and offsets later
		
		//properly handle and recieve the text from intro
		
		//string_format_time(dayText, sizeof(dayText), " %d", dayTransition.tick.tick_time.t_mhours+24); //NUMBERS	
		
		//string_format_time(dayText, sizeof(dayText), " %d", dayTransition.tick.tick_time); //NUMBERS
		//string_format_time(wordText, sizeof(wordText), " %a", dayTransition.tick.tick_time); //NUMBERS
		//dayText + 864000
			
			//somehow do all of this server-side
			//if i have to, call 4 times with the dates,
		//also need to do the persistant data storage.
			
		
		//uppercase it, should be by itself, but fuck it.
		//dayText = dayTransition.day[i].date;
		//wordText = dayTransition.day[i].wordDay;
				
		/*for (int index = 0; dayTransition.day[i].wordDay[index] != 0; index++) {
			if (dayTransition.day[i].wordDay[index] >= 'a' && dayTransition.day[i].wordDay[index] <= 'z') {
				dayTransition.day[i].wordDay[index] -= 0x20;
			}
		}*/
				
		//properly offset
		
		//this stuff isn't being created, don't know why. but look into it.
		//layers init
		text_layer_init(&dayTransition.day[i].layerWord, dayTransition.day[i].wordRectStart);
		text_layer_set_text_color(&dayTransition.day[i].layerWord, GColorWhite);
		text_layer_set_background_color(&dayTransition.day[i].layerWord, GColorClear);
		text_layer_set_font(&dayTransition.day[i].layerWord, fontAbbr); //just to test
		text_layer_set_text_alignment(&dayTransition.day[i].layerWord, GTextAlignmentCenter);
		layer_add_child(&window.layer, &dayTransition.day[i].layerWord.layer);
		text_layer_set_text(&dayTransition.day[i].layerWord,dayTransition.day[i].wordDay);
		
		text_layer_init(&dayTransition.day[i].layerDay, dayTransition.day[i].dayRectStart);
		text_layer_set_text_color(&dayTransition.day[i].layerDay, GColorWhite);
		text_layer_set_background_color(&dayTransition.day[i].layerDay, GColorClear);
		text_layer_set_font(&dayTransition.day[i].layerDay, fontAbbr); //just to test
		text_layer_set_text_alignment(&dayTransition.day[i].layerDay, GTextAlignmentCenter);
		layer_add_child(&window.layer, &dayTransition.day[i].layerDay.layer);			
		text_layer_set_text(&dayTransition.day[i].layerDay,dayTransition.day[i].date);		
		dayTransition.day[i].isSplit = false;//DEBUG	
	}
}

void OpeningAnimationStopped(Animation* animation, bool finished, void* context) 
{	
	dayTransition.introMonth0Frame = INTRO_MONTH_0_FRAME;	
	//dayTransition.introMonth1Frame = INTRO_MONTH_1_FRAME;	
	//we need to remove this layer, and redraw it, but black BG. so the inverter layer will work right.
	layer_remove_from_parent(&dayTransition.layerDayTrans.layer);
	bitmap_layer_init(&dayTransition.layerDayTrans, dayTransition.endRect);
	bitmap_layer_set_background_color(&dayTransition.layerDayTrans, GColorBlack);
	layer_add_child(&window.layer, &dayTransition.layerDayTrans.layer);
	
	text_layer_init(&dayTransition.introMonth0, dayTransition.introMonth0Frame);
    text_layer_set_text_color(&dayTransition.introMonth0, GColorWhite);
    text_layer_set_background_color(&dayTransition.introMonth0, GColorClear);
    text_layer_set_font(&dayTransition.introMonth0, fontMonth);
	text_layer_set_text_alignment(&dayTransition.introMonth0, GTextAlignmentCenter);
    layer_add_child(&window.layer, &dayTransition.introMonth0.layer);
	static char month0Text[] = "00";
	string_format_time(month0Text, sizeof(month0Text), "%m", dayTransition.tick.tick_time);
	if (month0Text[0] == '1')
	{
		//need to determine how to properly get just the first digit.
		text_layer_set_text(&dayTransition.introMonth0, (month0Text));
	}
	else
	{
		//static char month = (char)(((int)'0')+month0Text[1]);
		text_layer_set_text(&dayTransition.introMonth0, &month0Text[1]);
	}
	
	/*text_layer_init(&dayTransition.introMonth1, dayTransition.introMonth1Frame);
    text_layer_set_text_color(&dayTransition.introMonth1, GColorWhite);
    text_layer_set_background_color(&dayTransition.introMonth1, GColorClear);
    text_layer_set_font(&dayTransition.introMonth1, fontMonth);
	text_layer_set_text_alignment(&dayTransition.introMonth1, GTextAlignmentCenter);
    layer_add_child(&window.layer, &dayTransition.introMonth1.layer);
	static char month1Text[] = "00";
	string_format_time(month1Text, sizeof(month1Text), "%m", dayTransition.tick.tick_time);
	if (month1Text[0] == '1')
	{
		//need to determine how to properly get just the first digit.
		text_layer_set_text(&dayTransition.introMonth1, (month1Text));
	}
	else
	{
		//static char month = (char)(((int)'0')+month1Text[1]);
		text_layer_set_text(&dayTransition.introMonth1, &month1Text0[1]);
	}*/
	
	
	
	dayTransition.introYear0Frame = INTRO_YEAR_0_FRAME;
	text_layer_init(&dayTransition.introYear0, dayTransition.introYear0Frame);
    text_layer_set_text_color(&dayTransition.introYear0, GColorWhite);
    text_layer_set_background_color(&dayTransition.introYear0, GColorClear);
    text_layer_set_font(&dayTransition.introYear0, fontYear);
	text_layer_set_text_alignment(&dayTransition.introYear0, GTextAlignmentCenter);
    layer_add_child(&window.layer, &dayTransition.introYear0.layer);
	static char year0Text[] = "0000";
	string_format_time(year0Text, sizeof(year0Text), "%Y", dayTransition.tick.tick_time);
	text_layer_set_text(&dayTransition.introYear0, year0Text);
	fAbleToPlaceSuccessfully = true;
	
	/*	dayTransition.introYear1Frame = INTRO_YEAR_0_FRAME;
	text_layer_init(&dayTransition.introYear1, dayTransition.introYear1Frame);
    text_layer_set_text_color(&dayTransition.introYear1, GColorWhite);
    text_layer_set_background_color(&dayTransition.introYear1, GColorClear);
    text_layer_set_font(&dayTransition.introYear1, fontYear);
	text_layer_set_text_alignment(&dayTransition.introYear1, GTextAlignmentCenter);
    layer_add_child(&window.layer, &dayTransition.introYear1.layer);
	static char year1Text[] = "0000";
	string_format_time(year1Text, sizeof(year1Text), "%Y", dayTransition.tick.tick_time);
	text_layer_set_text(&dayTransition.introYear1, year1Text);*/
	
	SetupSlidingFrames();
	SetBitmap(WEATHER);
	inverter_layer_init(&dayTransition.layerInvert, dayTransition.endRect);
	layer_add_child(&window.layer,&dayTransition.layerInvert.layer);

	//setup the days so that we don't have inf. transitions
	currentDay = dayTransition.tick.tick_time->tm_mday;
	previousDay = currentDay;
		
	// for some reasoon this didn't work so i had to manually set everything *whywithhands*'
	property_animation_init_layer_frame(&dayTransition.day[0].slideLeftWordAnimation, &dayTransition.day[0].layerWord.layer,&dayTransition.day[0].wordRectStart,&dayTransition.day[0].wordRectEnd);
	property_animation_init_layer_frame(&dayTransition.day[0].slideLeftDayAnimation, &dayTransition.day[0].layerDay.layer,&dayTransition.day[0].dayRectStart, &dayTransition.day[0].dayRectEnd);
	property_animation_init_layer_frame(&dayTransition.day[0].slideLeftImgAnimation, &dayTransition.day[0].layerWeather.layer,&dayTransition.day[0].imgRectStart,&dayTransition.day[0].imgRectEnd);
	animation_set_duration(&dayTransition.day[0].slideLeftWordAnimation.animation, slideAnimationDuration);
	animation_set_delay(&dayTransition.day[0].slideLeftWordAnimation.animation, slideAnimationDelay);
	animation_schedule(&dayTransition.day[0].slideLeftWordAnimation.animation);
	animation_set_duration(&dayTransition.day[0].slideLeftDayAnimation.animation, slideAnimationDuration);
	animation_set_delay(&dayTransition.day[0].slideLeftDayAnimation.animation, slideAnimationDelay);
	animation_schedule(&dayTransition.day[0].slideLeftDayAnimation.animation);
	animation_set_duration(&dayTransition.day[0].slideLeftImgAnimation.animation, slideAnimationDuration);
	animation_set_delay(&dayTransition.day[0].slideLeftImgAnimation.animation, slideAnimationDelay);
	animation_schedule(&dayTransition.day[0].slideLeftImgAnimation.animation);
	
	property_animation_init_layer_frame(&dayTransition.day[1].slideLeftWordAnimation, &dayTransition.day[1].layerWord.layer,&dayTransition.day[1].wordRectStart,&dayTransition.day[1].wordRectEnd);
	property_animation_init_layer_frame(&dayTransition.day[1].slideLeftDayAnimation, &dayTransition.day[1].layerDay.layer,&dayTransition.day[1].dayRectStart, &dayTransition.day[1].dayRectEnd);
	property_animation_init_layer_frame(&dayTransition.day[1].slideLeftImgAnimation, &dayTransition.day[1].layerWeather.layer,&dayTransition.day[1].imgRectStart,&dayTransition.day[1].imgRectEnd);
	animation_set_duration(&dayTransition.day[1].slideLeftWordAnimation.animation, slideAnimationDuration);
	animation_set_delay(&dayTransition.day[1].slideLeftWordAnimation.animation, slideAnimationDelay);
	animation_schedule(&dayTransition.day[1].slideLeftWordAnimation.animation);
	animation_set_duration(&dayTransition.day[1].slideLeftDayAnimation.animation, slideAnimationDuration);
	animation_set_delay(&dayTransition.day[1].slideLeftDayAnimation.animation, slideAnimationDelay);
	animation_schedule(&dayTransition.day[1].slideLeftDayAnimation.animation);
	animation_set_duration(&dayTransition.day[1].slideLeftImgAnimation.animation, slideAnimationDuration);
	animation_set_delay(&dayTransition.day[1].slideLeftImgAnimation.animation, slideAnimationDelay);
	animation_schedule(&dayTransition.day[1].slideLeftImgAnimation.animation);
	
	property_animation_init_layer_frame(&dayTransition.day[2].slideLeftWordAnimation, &dayTransition.day[2].layerWord.layer,&dayTransition.day[2].wordRectStart,&dayTransition.day[2].wordRectEnd);
	property_animation_init_layer_frame(&dayTransition.day[2].slideLeftDayAnimation, &dayTransition.day[2].layerDay.layer,&dayTransition.day[2].dayRectStart, &dayTransition.day[2].dayRectEnd);
	property_animation_init_layer_frame(&dayTransition.day[2].slideLeftImgAnimation, &dayTransition.day[2].layerWeather.layer,&dayTransition.day[2].imgRectStart,&dayTransition.day[2].imgRectEnd);
	animation_set_duration(&dayTransition.day[2].slideLeftWordAnimation.animation, slideAnimationDuration);
	animation_set_delay(&dayTransition.day[2].slideLeftWordAnimation.animation, slideAnimationDelay);
	animation_schedule(&dayTransition.day[2].slideLeftWordAnimation.animation);
	animation_set_duration(&dayTransition.day[2].slideLeftDayAnimation.animation, slideAnimationDuration);
	animation_set_delay(&dayTransition.day[2].slideLeftDayAnimation.animation, slideAnimationDelay);
	animation_schedule(&dayTransition.day[2].slideLeftDayAnimation.animation);
	animation_set_duration(&dayTransition.day[2].slideLeftImgAnimation.animation, slideAnimationDuration);
	animation_set_delay(&dayTransition.day[2].slideLeftImgAnimation.animation, slideAnimationDelay);
	animation_schedule(&dayTransition.day[2].slideLeftImgAnimation.animation);

	property_animation_init_layer_frame(&dayTransition.day[3].slideLeftWordAnimation, &dayTransition.day[3].layerWord.layer,&dayTransition.day[3].wordRectStart,&dayTransition.day[3].wordRectEnd);
	property_animation_init_layer_frame(&dayTransition.day[3].slideLeftDayAnimation, &dayTransition.day[3].layerDay.layer,&dayTransition.day[3].dayRectStart, &dayTransition.day[3].dayRectEnd);
	property_animation_init_layer_frame(&dayTransition.day[3].slideLeftImgAnimation, &dayTransition.day[3].layerWeather.layer,&dayTransition.day[3].imgRectStart,&dayTransition.day[3].imgRectEnd);
	animation_set_duration(&dayTransition.day[3].slideLeftWordAnimation.animation, slideAnimationDuration);
	animation_set_delay(&dayTransition.day[3].slideLeftWordAnimation.animation, slideAnimationDelay);
	animation_schedule(&dayTransition.day[3].slideLeftWordAnimation.animation);
	animation_set_duration(&dayTransition.day[3].slideLeftDayAnimation.animation, slideAnimationDuration);
	animation_set_delay(&dayTransition.day[3].slideLeftDayAnimation.animation, slideAnimationDelay);
	animation_schedule(&dayTransition.day[3].slideLeftDayAnimation.animation);
	animation_set_duration(&dayTransition.day[3].slideLeftImgAnimation.animation, slideAnimationDuration);
	animation_set_delay(&dayTransition.day[3].slideLeftImgAnimation.animation, slideAnimationDelay);
	/*create a nother layer for the year/month
add those to the animations when the following is true
(also move the previous)

if next day = 1
if month == 12
month = 1
year = year +1
else
month = month + 1*/
	/*if (fNeedNewMonth)
	{
		text_layer_init(&dayTransition.layerTextMonth, dayTransition.layerTextMonthFrame);
		text_layer_set_text_color(&dayTransition.layerTextMonth, GColorWhite);
		text_layer_set_background_color(&dayTransition.layerTextMonth, GColorClear);
		text_layer_set_font(&dayTransition.layerTextMonth, fontMonth);
		text_layer_set_text_alignment(&dayTransition.layerTextMonth, GTextAlignmentCenter);
		layer_add_child(&window.layer, &dayTransition.layerTextMonth.layer);
		static char monthText[] = "00";
		string_format_time(monthText, sizeof(monthText), "%m", dayTransition.tick.tick_time);
		if (monthText[0] == '1')
		{
			//need to determine how to properly get just the first digit.
			text_layer_set_text(&dayTransition.layerTextMonth, (monthText));
		}
		else
		{
			//static char month = (char)(((int)'0')+monthText[1]);
			text_layer_set_text(&dayTransition.layerTextMonth, &monthText[1]);
		}
		//create animation sections
		//animate the old moving out (and then delete it on the after)
		//animate the enw
	}
	if (fNeedNewYear)
		dayTransition.layerTextYearFrame = LAYER_TEXT_YEAR_FRAME;
		text_layer_init(&dayTransition.layerTextYear, dayTransition.layerTextYearFrame);
		text_layer_set_text_color(&dayTransition.layerTextYear, GColorWhite);
		text_layer_set_background_color(&dayTransition.layerTextYear, GColorClear);
		text_layer_set_font(&dayTransition.layerTextYear, fontYear);
		text_layer_set_text_alignment(&dayTransition.layerTextYear, GTextAlignmentCenter);
		layer_add_child(&window.layer, &dayTransition.layerTextYear.layer);
		static char yearText[] = "0000";
		string_format_time(yearText, sizeof(yearText), "%Y", dayTransition.tick.tick_time);
		text_layer_set_text(&dayTransition.layerTextYear, yearText);
	//create animation sections
	
	}	*/
	//only set handlers on the last frame.
	animation_set_handlers(&dayTransition.day[3].slideLeftImgAnimation.animation, (AnimationHandlers){
		.stopped = (AnimationStoppedHandler)SlidingAnimationStopped
	}, context);
	animation_schedule(&dayTransition.day[3].slideLeftImgAnimation.animation);
}

void requestIntroWeather() {
	if(!located) {
		http_location_request();
		return;
	}
	// Build the HTTP request
	DictionaryIterator *body;
	//HTTPResult result = http_out_get("http://masamuneshadow.herobo.com/api/weather_intro.php", WEATHER_HTTP_COOKIE, &body);
	HTTPResult result = http_out_get("http://masamuneshadow.herobo.com/api/weather_intro_DEBUG.php", WEATHER_HTTP_COOKIE, &body);
	
	if(result != HTTP_OK) {
		dayTransition.day[0].currentWeather = 11;
		dayTransition.day[1].currentWeather = 11;
		dayTransition.day[2].currentWeather = 11;
		dayTransition.day[3].currentWeather = 11;
		fIntroWeatherGet = false;
		SetBitmap(WEATHER);
		return;
	}
	
	dict_write_int32(body, WEATHER_KEY_LATITUDE, our_latitude);
	dict_write_int32(body, WEATHER_KEY_LONGITUDE, our_longitude);

	// Send it.
	if(http_out_send() != HTTP_OK) {
		dayTransition.day[0].currentWeather = 11;
		dayTransition.day[1].currentWeather = 11;
		dayTransition.day[2].currentWeather = 11;
		dayTransition.day[3].currentWeather = 11;
		fIntroWeatherGet = false;
		SetBitmap(WEATHER);
		return;
	}
}

void DayTransition(AppContextRef ctx)
{

	//requestIntroWeather();
	/*if none
	requestIntroWeather
	end if*/
	
	isTransitioning = true;
	isDayTransition = true;
	
	requestIntroWeather(); //this needs to trigger super fast!
	
	dayTransition.startRect = INTRO_START_RECT;
	dayTransition.endRect = INTRO_END_RECT;
	
	//create the background layer.
	bitmap_layer_init(&dayTransition.layerBackground, window.layer.frame);
	bitmap_layer_set_background_color(&dayTransition.layerBackground, GColorBlack);
	layer_add_child(&window.layer, &dayTransition.layerBackground.layer);
	
	bitmap_layer_init(&dayTransition.layerDayTrans, dayTransition.startRect);
	bitmap_layer_set_background_color(&dayTransition.layerDayTrans, GColorWhite);
	layer_add_child(&window.layer, &dayTransition.layerDayTrans.layer);
	//opening bar animation
	property_animation_init_layer_frame(&dayTransition.openingAnimation, &dayTransition.layerDayTrans.layer,&dayTransition.startRect,&dayTransition.endRect);
	animation_set_duration(&dayTransition.openingAnimation.animation, openAnimationDuration);
	animation_set_delay(&dayTransition.openingAnimation.animation, openAnimationDelay);
  	animation_set_handlers(&dayTransition.openingAnimation.animation, (AnimationHandlers){
	.stopped = (AnimationStoppedHandler)OpeningAnimationStopped
	}, (void*)ctx);
	animation_schedule(&dayTransition.openingAnimation.animation);
	get_time(&dayTransition.tm);
    dayTransition.tick.tick_time = &dayTransition.tm;
}

void WordTransitionExitAnimationStopped(Animation* animation, bool finished, void* context)
{
	
	/*layer_remove_from_parent(&dayTransition.layerTextYear.layer);
	layer_remove_from_parent(&dayTransition.layerTextMonth.layer);
	layer_remove_from_parent(&dayTransition.layerInvert.layer);

	for (int i = 0; i < 4; i++)
	{
		{
			layer_remove_from_parent(&dayTransition.day[i].layerDay.layer);
			layer_remove_from_parent(&dayTransition.day[i].layerWord.layer);
		}
	}
	RemoveAndDeIntBmp(WEATHER);
	
	layer_remove_from_parent(&dayTransition.layerDayTrans.layer);
	layer_remove_from_parent(&dayTransition.layerClosingLeft.layer);
	layer_remove_from_parent(&dayTransition.layerClosingRight.layer);*/
	if (!watchfaceStarted)
		StartWatchface(context);
}
/*
void WordTransitionFlipAnimationSecondHalfStopped(Animation* animation, bool finished, void* context) 
{	
	//should just be able to take the points and criss-cross them (left->right, right->left)
	property_animation_init_layer_frame(&wordTransition.exitAnimation.animation, &wordTransition.layerSquare.layer,&wordTransition.holdRect,&wordTransition.exitRect);
	
}

void WordTransitionFlipAnimationFirstHalfStopped(Animation* animation, bool finished, void* context) 
{	
	//should just be able to take the points and criss-cross them (left->right, right->left)
	//now with the actual current.
	wordTransition.layerWordFrame = wordTransition.holdLayerWordFrame;
	SetBitmap(WORD);
	
	
	
		
	property_animation_init_layer_frame(&wordTransition.flipAnimationSecondHalf.animation, &dayTransition.layer,&wordTransition.,&wordTransition);
	animation_set_duration(&wordTransition.flipAnimationSecondHalf.animation, flipAnimationDuration);
		animation_set_handlers(&dayTransition.day[3].slideLeftImgAnimation.animation, (AnimationHandlers){
		.stopped = (AnimationStoppedHandler)WordTransitionFlipAnimationSecondHalfStopped
	}, context);
	animation_schedule(&wordTransition.flipAnimationSecondHalf.animation);
	
	
}*/

/*void WordTransitionStartAnimationStopped(Animation* animation, bool finished, void* context) 
{	
	//start the hold flipped one.
	the flip should just be the two black closers from the previous thing.
	
		property_animation_init_layer_frame(&wordTransition.flipAnimationFirstHalf.animation, &wordTransition.layerSquare.layer,&wordTransition.holdRect,&wordTransition.holdHalfRect);
		animation_set_duration(&wordTransition.flipAnimationFirstHalf.animation, flipAnimationDuration);
		animation_set_handlers(&wordTransition.flipAnimationFirstHalf.animation, (AnimationHandlers){
		.stopped = (AnimationStoppedHandler)WordTransitionFlipAnimationFirstHalfStopped
	}, context);
	animation_schedule(&wordTransition.flipAnimationFirstHalf.animation);
}*/



void WordTransition(AppContextRef ctx)
{
	if (!isTransitioning)
	{
		//no need to de-int what's going on in the background
		//just make a new thing to go on top of it
		isWordTransition = true;
		isTransitioning = true;
		
		wordTransition.backgroundRect = BACKGROUND_FRAME;
		wordTransition.endRect = WORD_SQUARE_FRAME_END;
		wordTransition.holdRect = WORD_SQUARE_FRAME_HOLD;
		wordTransition.startRect = WORD_SQUARE_FRAME_START;
		wordTransition.layerWordFrame = WORD_TEXT_FRAME;
		
		bitmap_layer_init(&wordTransition.layerBackground, wordTransition.backgroundRect);
		bitmap_layer_set_background_color(&wordTransition.layerBackground, GColorBlack);
		layer_add_child(&window.layer, &wordTransition.layerBackground.layer);
		
		bitmap_layer_init(&wordTransition.layerSquare, wordTransition.startRect);
		bitmap_layer_set_background_color(&wordTransition.layerBackground, GColorWhite);
		layer_add_child(&window.layer, &wordTransition.layerSquare.layer);
		
		wordTransition.previousWordHolder = previousWord;
		wordTransition.currentWordHolder = currentWord;
		currentWord = previousWord;
		//initally display the "current" (but really previous) word
		SetBitmap(WORD);
		//setup the holders
		previousWord = wordTransition.previousWordHolder;
		currentWord = wordTransition.currentWordHolder;
		
			/* Need to figure out how to properly rotate a square into a diamond
		https://github.com/pebble/pebblekit/blob/master/Pebble/demos/feature_image_transparent_rotate/src/feature_image_transparent_rotate.c
		just make a square, and then use that code to rotate it 45 degrees (or however much I want)
		
		bam that easy.
		
		void square_layer_update_callback(Layer *me, GContext* ctx) {
		  (void)me;
		  graphics_context_set_stroke_color(ctx, GColorWhite); // TODO: Needed?
		  graphics_context_set_fill_color(ctx, GColorWhite);
		  graphics_fill_rect(ctx, GRect(12,0,120,50), 0, GCornersAll);
		}*/
		property_animation_init_layer_frame(&wordTransition.startAnimation, &wordTransition.layerSquare.layer, &wordTransition.startRect, &wordTransition.holdRect);
		animation_set_duration(&wordTransition.startAnimation.animation, startAnimationDuration);
		animation_set_delay(&wordTransition.startAnimation.animation, 3000);
		/*animation_set_handlers(&wordTransition.startAnimation.animation, (AnimationHandlers){
		.stopped = (AnimationStoppedHandler)WordTransitionStartAnimationStopped
		}, (void*)ctx);*/
		animation_schedule(&wordTransition.startAnimation.animation);
	}
}

void handle_init(AppContextRef ctx) {
    (void) ctx;

    window_init(&window, "PEBSONA4");
    window_stack_push(&window, true /* Animated */);
    window_set_background_color(&window, GColorBlack);

    resource_init_current_app(&APP_RESOURCES);
    bmp_init_container(RESOURCE_ID_IMAGE_TIME_BLANK,  &background_image);
	layer_add_child(&window.layer,&background_image.layer.layer);
    
	//Fonts
	resDate = resource_get_handle(RESOURCE_ID_FONT_DAYS_26);
	fontDate = fonts_load_custom_font(resDate);
	resAbbr = resource_get_handle(RESOURCE_ID_FONT_DAYS_13);
	fontAbbr = fonts_load_custom_font(resAbbr);
	resYear = resource_get_handle(RESOURCE_ID_FONT_DAYS_16);
	fontYear = fonts_load_custom_font(resYear);
	resMonth = resource_get_handle(RESOURCE_ID_FONT_DAYS_42);
	fontMonth = fonts_load_custom_font(resMonth);
    //SETUP INIT STUFF
	//previousMinute = 99;
    previousHour = 99;
    previousWord = 99;
    previousDay = 99;
	previousWeather = 99;
	//currentMinute = 88;
	//currentHour = 88;
	currentWord = 99;
	currentDay = 88;
	currentWeather = 11; //uNKNOWN

	http_register_callbacks((HTTPCallbacks){.failure=failed,.success=success,.reconnect=reconnect,.location=location}, (void*)ctx);
	located = false;
	watchfaceStarted = false;
	StartWatchface(ctx);
	//isTransitioning = true;
	//isDayTransition = true;
	//request_weather();
	//requestIntroWeather();
	//TVLoadingScreen();
	DayTransition(ctx);
}

void handle_deinit(AppContextRef ctx) 
{
	(void)ctx;
	
	animation_unschedule_all();
	bmp_deinit_container(&background_image);
	bmp_deinit_container(&imgWeather);
	bmp_deinit_container(&imgWord_BLACK);
	bmp_deinit_container(&imgWord_WHITE);
	bmp_deinit_container(&imgColon_BLACK);
	bmp_deinit_container(&imgColon_WHITE);
	for (int i = 0; i < 4; i++)
	{
		bmp_deinit_container(&dayTransition.day[i].imgWeather);
		bmp_deinit_container(&dayTransition.day[i].imgWeatherSplitTop_WHITE);
		bmp_deinit_container(&dayTransition.day[i].imgWeatherSplitBottom_WHITE);
		bmp_deinit_container(&dayTransition.day[i].imgWeatherSplitTop_BLACK);
		bmp_deinit_container(&dayTransition.day[i].imgWeatherSplitBottom_BLACK);
	}
	/*bmpdeinit_container(&dayTransition.layerDayTrans);
	bmp_deinit_container(&dayTransition.layerBackground);
	bmp_deinit_container(&wordTransition.layerBackground);
	bmp_deinit_container(&wordTransition.layerSquare);
	bmp_deinit_container(&wordTransition.layerWord_WHITE);
	bmp_deinit_container(&wordTransition.layerWord_BLACK);*/
	for (int i = 0; i < TOTAL_TIME_DIGITS; i++)
	{
		bmp_deinit_container(&imgTime_BLACK[i]);
		bmp_deinit_container(&imgTime_WHITE[i]);
	}
	fonts_unload_custom_font(fontDate);
	fonts_unload_custom_font(fontAbbr);
	fonts_unload_custom_font(fontYear);
	fonts_unload_custom_font(fontMonth);
}

void pbl_main(void *params)
{
    PebbleAppHandlers handlers =
    {
        .init_handler = &handle_init,
        .deinit_handler = &handle_deinit,
        .tick_info =
        {
            .tick_handler = &handle_minute_tick,
            .tick_units = MINUTE_UNIT
        },
			.timer_handler = handle_timer,
			.messaging_info = {
			.buffer_sizes = {
			.inbound = 124,
			.outbound = 256,
				}
			}
    };

    app_event_loop(params, &handlers);
}

void request_weather() {
	//try it without the location request.
	if(!located) {
		http_location_request();
		return;
	}
	// Build the HTTP request
	DictionaryIterator *body;
	HTTPResult result = http_out_get("http://masamuneshadow.herobo.com/api/weather_current.php", WEATHER_HTTP_COOKIE, &body);
	if(result != HTTP_OK) {
		currentWeather = 11;
		SetBitmap(WEATHER);
		return;
	}
	dict_write_int32(body, WEATHER_KEY_LATITUDE, our_latitude);
	dict_write_int32(body, WEATHER_KEY_LONGITUDE, our_longitude);
	// Send it.
	if(http_out_send() != HTTP_OK) {
		currentWeather = 11;
		SetBitmap(WEATHER);
		return;
	}
}
//get weather @ 8
//get weather at 6 pm?
//if mornweather == pmweather use non-split image.
/*
need to call the request FIRST THING, possibly get the location even sooner.

move the weather call earlier
ensure the link occurs earlier

there is currently a memory leak.

could store the data for faster use later via misbehaving
store it in a bitmap, frame = 1 "letter" in binary
black = 0 , white = 1,
1 pixel
can store 18 in one line

but i need to determine if I can even get orphaned data, and possibly how i could get the orphaned data/check of its existance when bringing it back up.

new Program structure
init
have it do the weather call/request
.setup
setup watchface
create the transition ontop of it (isTransitioning, IsDayTransition = true)
do the animation
    (after the hold, no delay)
de-int it all, and then that should leave just the watchface stuff.


maybe
(10:57:11 AM) : i think i just need to rocket out queries at the start, and I'm going to restructure it to give myself ever-so-slightly more time before the animation starts
(10:57:49 AM) : because if it takes 2.5 MS to calc and return, and I take 5ms wait before starting the animation, there shouldn't be any problem
(10:58:08 AM) : or i could extend the animation to maybe play for twice as long

use the option 3 to setup the weather as well (set the currentWeather)

in the deinit, add in the dayTrans stuff. and wordTrans stuff to DeInt.
*/
