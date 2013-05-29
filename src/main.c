#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x01, 0x6E, 0x11, 0x1F, 0xB2, 0x60, 0x4E, 0x80, 0xA7, 0xCD, 0x78, 0x35, 0xA0, 0x31, 0xE3, 0xF3 }
PBL_APP_INFO(MY_UUID,
             "Pebsona4", "Masamune_Shadow",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

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

Layer transition_layer;
//Word Transitions
/*Layer transition_word_layer;
Layer transition_block_1_layer;
Layer transition_block_2_layer;
Layer transition_square_layer;*/

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

/*=========Bools============*/
bool isDayTransitioning = false;
bool isWordTransitioning = false;
bool isTransitioning = false;
//bool isWordImageOnly = false;


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

bool getWeather = true;
GRect wordFrame;
GRect weatherFrame;
GRect timeFrame; //ha!
GRect timeHourTensFrame;
GRect timeHourOnesFrame;
GRect timeColonFrame;
GRect timeMinuteTensFrame;
GRect timeMinuteOnesFrame;
GRect dateFrame;
GRect wordDateFrame;

GFont fontDate;
GFont fontAbbr;
ResHandle resDate;
ResHandle resAbbr;

#define DATE "Date"
#define TIME "Time"
#define WORD "Word"
#define WEATHER "Weather"
#define ALL "All"

#define timeWidth 19
#define timeHeight 19
#define timePosY 55

//Day Transitions
/*
Layer DayGraphic
TextLayer text_numday_layer;
TextLayer text_monthday_layer;
*/
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

void SetWordImage()
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

void SetWeatherImage()
{	
	if (currentWeather == 0)//unknown
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_UNKNOWN,&imgWeather);
	}		
	else if (currentWeather == 1) //sun
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SUN,&imgWeather);
	}
	else if(currentWeather == 2) //rain
	{	
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_RAIN,&imgWeather);
	}
	else if(currentWeather == 3)//cloud
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_CLOUD,&imgWeather);
	}
	else if(currentWeather == 4)//snow
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SNOW,&imgWeather);
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
		layer_remove_from_parent(&layerWord_WHITE.layer);
		layer_remove_from_parent(&layerWord_BLACK.layer);
		bmp_deinit_container(&imgWord_WHITE);
		bmp_deinit_container(&imgWord_BLACK);
	}

	if (strcmp (bitmap,WEATHER) == 0 || strcmp(bitmap,ALL) == 0)
	{
		layer_remove_from_parent(&layerWeather.layer);
		bmp_deinit_container(&imgWeather);
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
	RemoveAndDeIntBmp(bitmap);
	if (strcmp(bitmap,WORD) == 0)
	{    
		SetWordImage();			
		//black background, takes the white and makes them black, all else are not drawn
		bitmap_layer_init(&layerWord_WHITE,wordFrame);
		bitmap_layer_set_bitmap(&layerWord_WHITE,&imgWord_WHITE.bmp);
		bitmap_layer_set_background_color(&layerWord_WHITE,GColorClear);
		bitmap_layer_set_compositing_mode(&layerWord_WHITE, GCompOpClear);
		layer_add_child(&window.layer, &layerWord_WHITE.layer);
		layer_mark_dirty(&layerWord_WHITE.layer);

		// takes the white and doesn't draw it if it itercets with a black?
		bitmap_layer_init(&layerWord_BLACK,wordFrame);
		bitmap_layer_set_bitmap(&layerWord_BLACK, &imgWord_BLACK.bmp); //or .layer?
		bitmap_layer_set_background_color(&layerWord_BLACK,GColorClear);
		bitmap_layer_set_compositing_mode(&layerWord_BLACK, GCompOpOr);
		layer_add_child(&window.layer, &layerWord_BLACK.layer);
		layer_mark_dirty(&layerWord_BLACK.layer);			
	}
	else if (strcmp(bitmap,WEATHER) == 0)
	{
		SetWeatherImage();
		bitmap_layer_init(&layerWeather,weatherFrame);
		bitmap_layer_set_bitmap(&layerWeather,&imgWeather.bmp);
		bitmap_layer_set_background_color(&layerWeather,GColorClear);
		bitmap_layer_set_compositing_mode(&layerWeather, GCompOpOr);
		layer_add_child(&window.layer, &layerWeather.layer);
		layer_mark_dirty(&layerWeather.layer);		
	}
	/*else if (strcmp(bitmap,DATE) == 0){}*/
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

void GetAndSetWeather()
{
    //get info from phone,
	getWeather = false;
	currentWeather = 1;
    if (currentWeather != previousWeather)
	{
		SetBitmap(WEATHER);
		previousWeather = currentWeather;
	}
}

void GetAndSetCurrentWord(PblTm* currentTime)
{
    switch (currentTime->tm_hour)
    {
        case 0:
            currentWord = 0; //midnight
            break;
        case 1:
        case 2:
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
            currentWord = 6;//night
            break;
    }

    if (previousWord != currentWord)
    {
		SetBitmap(WORD);
		previousWord = currentWord;
    }
    
}

/*DayTransition()
{


//end
deint
isDayTransition = false;
watchface
}

WordTransition()
{
isDayTransition
//end
deint
isWordTransition = false;
watchface
}*/

void CheckTransitionTime()
{
	if (currentWord != previousWord)
        isWordTransitioning = true;
	isWordTransitioning = false; //DEBUG -- for time being
}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;
  (void)ctx;

	static char dateText[] = "00 00";
	string_format_time(dateText, sizeof(dateText), "%m/%d", t->tick_time);
	text_layer_set_text(&layerDate, dateText);

	static char dateWordText[] = " Xxx";
	string_format_time(dateWordText, sizeof(dateWordText), " %a", t->tick_time);
	text_layer_set_text(&layerDateWord, dateWordText);

	static char timeText[] = "00:00";
	char *timeFormat;
	timeFormat = clock_is_24h_style() ? "%R" : "%I:%M";
	string_format_time(timeText, sizeof(timeText), timeFormat, t->tick_time);
	//CheckTransitionTime();
	//if (!isDayTransition && !isWordTransition)
	//{
		// Kludge to handle lack of non-padded hour format string
		// for twelve hour clock.
		if (!clock_is_24h_style() && (timeText[0] == '0'))
		{
			memmove(timeText, &timeText[1], sizeof(timeText) - 1);
		}
		//not right, should be by itself but don't want to pass around w/e
		int hour = t->tick_time->tm_hour;
		if (previousHour != hour)
		{
			currentDay = 	t->tick_time->tm_mday;
			previousHour = hour;
			GetAndSetCurrentWord(t->tick_time);
			//if within check for weather()
			if (getWeather)
			{GetAndSetWeather();}
		}

		if (previousDay != currentDay){}

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
	//}
}

void RefreshAll()
{
    SetBitmap(TIME);
	SetBitmap(WORD);
	//if within check, 
	// SetBitmap(WEATHER); //DEBUG = OFF
}


void Watchface()//DISPLAYS THE "WATCH PART" OR, NO TRANSITIONS ARE RUNNING
{
//set
//background init
//dateinit
//time init
//layer_proc_update word
//layer_proc_update_weather



	/*weatherDestination = layer_get_frame(&weather_BLACK.layer.layer);
	weatherDestination.origin.y = 75;
  	weatherDestination.origin.x = 50;*/

	//Date Text Layer init
	resDate = resource_get_handle(RESOURCE_ID_FONT_DAYS_26);
	fontDate = fonts_load_custom_font(resDate);
	resAbbr = resource_get_handle(RESOURCE_ID_FONT_DAYS_13);
	fontAbbr = fonts_load_custom_font(resAbbr);

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

	RefreshAll();
}

void handle_init(AppContextRef ctx) {
    (void) ctx;

    window_init(&window, "PEBSONA4");
    window_stack_push(&window, true /* Animated */);
    window_set_background_color(&window, GColorClear);

    resource_init_current_app(&APP_RESOURCES);
    bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND,  &background_image);

	layer_add_child(&window.layer,&background_image.layer.layer);
    
    //SETUP INIT STUFF
	previousWeather = 99;
	//previousMinute = 99;
    previousHour = 99;
    previousWord = 99;
    previousDay = 99;

	currentWeather = 0; //uNKNOWN
	//currentMinute = 88;
	//currentHour = 88;
	currentWord = 7;
	currentDay = 88;

	dateFrame = GRect(0,2,100,26);
	wordDateFrame = GRect(100,10,50,26);
	wordFrame = GRect(0,32,144,21);
	weatherFrame = GRect(53,87,75,75);

	timeFrame = GRect(0,timePosY,144,25);
	timeHourTensFrame = GRect(0,timePosY,timeWidth,timeHeight);
	timeHourOnesFrame = GRect(20,timePosY,timeWidth,timeHeight);
	timeColonFrame = GRect(40,timePosY,8,timeHeight);
	timeMinuteTensFrame = GRect(48,timePosY,timeWidth,timeHeight);
	timeMinuteOnesFrame = GRect(68,timePosY,timeWidth, timeHeight);

	//run into transition	

	Watchface();

	//DayTransition()//(Leads into BG Display)	
}


void handle_deinit(AppContextRef ctx) 
{
	(void)ctx;
		bmp_deinit_container(&background_image);
		bmp_deinit_container(&imgWeather);
		bmp_deinit_container(&imgWord_BLACK);
		bmp_deinit_container(&imgWord_WHITE);
		bmp_deinit_container(&imgColon_BLACK);
		bmp_deinit_container(&imgColon_WHITE);
		for (int i = 0; i < TOTAL_TIME_DIGITS; i++)
		{
			bmp_deinit_container(&imgTime_BLACK[i]);
			bmp_deinit_container(&imgTime_WHITE[i]);
		}
		fonts_unload_custom_font(fontDate);
		fonts_unload_custom_font(fontAbbr);
}

void pbl_main(void *params)
{
    PebbleAppHandlers handlers =
    {
        .init_handler = &handle_init,
        .tick_info =
        {
            .tick_handler = &handle_minute_tick,
            .tick_units = MINUTE_UNIT
        },
		.deinit_handler = &handle_deinit // called on close, do clean up in there
    };

    app_event_loop(params, &handlers);
}


/*
void WordTransition()
{
}
*/

/*
void DayTransition()
{
}
*/