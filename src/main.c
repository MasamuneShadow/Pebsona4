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
TextLayer 	layerWord_DATE; //white
BitmapLayer layerTime_WHITE;
BitmapLayer layerTime_BLACK;
BitmapLayer layerDate_WHITE;
BitmapLayer layerDate_BLACK;
BitmapLayer layerWord_WHITE;
BitmapLayer layerWord_BLACK;
BitmapLayer layerWeather;
//BitmapLayer layerWeather_WHITE;
//BitmapLayer layerWeather_BLACK;

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

BmpContainer background_image;

#define TOTAL_TIME_DIGITS 4
/*static BmpContainer imgTime_BLACK[TOTAL_TIME_DIGITS];
static int imgTimeResourceIds_BLACK[TOTAL_TIME_DIGITS];

static BmpContainer imgTime_WHITE[TOTAL_TIME_DIGITS];
static int imgTimeResourceIds_WHITE[TOTAL_TIME_DIGITS];*/

//also uses 4 digits
/*static BmpContainer imgDate_BLACK[TOTAL_TIME_DIGITS];
static int imgDateResourceIds_BLACK[TOTAL_TIME_DIGITS];

static BmpContainer imgDate_WHITE[TOTAL_TIME_DIGITS];
static int imgDateResourceIds_WHITE[TOTAL_TIME_DIGITS];*/

BmpContainer imgWord_BLACK;
BmpContainer imgWord_WHITE;
BmpContainer imgWeather;
//BmpContainer imgWeather_BLACK;
//BmpContainer imgWeather_WHITE;
BmpContainer slash;
BmpContainer colon;

/*=========Bools============*/
bool isDayTransitioning = false;
bool isWordTransitioning = false;
//bool isTransitioning = false;
//bool isWordImageOnly = false;

int current_word;
int previous_word;
int previous_hour;
int previous_day;
int current_weather;
int previous_weather;

bool getWeather = true;
GRect wordFrame;
GRect weatherFrame;
GRect timeFrame; //ha!
GRect dateFrame;
GRect wordDateFrame;

GFont font_date;
ResHandle res_d;

#define DATE "Date"
#define TIME "Time"
#define WORD "Word"
#define WEATHER "Weather"
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
	if (current_word == 0)//midnight
	{
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_MIDNIGHT,&imgWord_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_MIDNIGHT_INVERT,&imgWord_WHITE);
	}		
	else if (current_word == 1) //early morning
	{
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_EARLY_MORNING,&imgWord_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_EARLY_MORNING_INVERT,&imgWord_WHITE);
	}
	else if(current_word == 2) //morning
	{	
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_MORNING,&imgWord_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_MORNING_INVERT,&imgWord_WHITE);
	}
	else if(current_word == 3)//lunchtime
	{
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_LUNCHTIME,&imgWord_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_LUNCHTIME_INVERT,&imgWord_WHITE);
		}
	else if(current_word == 4)//afternoon
	{
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_AFTERNOON,&imgWord_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_AFTERNOON_INVERT,&imgWord_WHITE);
	}
	else if(current_word == 5)//evening
	{
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_EVENING,&imgWord_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_EVENING_INVERT,&imgWord_WHITE);
	}
	else if(current_word == 6)//night
	{
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_NIGHT,&imgWord_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_NIGHT_INVERT,&imgWord_WHITE);
	}
	else if(current_word == 7)//Unknown
	{
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_BLANK,&imgWord_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_TIME_BLANK,&imgWord_WHITE);
	}
}

void SetWeatherImage()
{	
	if (current_weather == 0)//unknown
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_UNKNOWN,&imgWeather);
		//bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_UNKNOWN,&imgWeather_BLACK);
		//bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_UNKNOWN_INVERT,&imgWeather_WHITE);
	}		
	else if (current_weather == 1) //sun
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SUN,&imgWeather);
		//bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SUN,&imgWeather_BLACK);
		//bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SUN_INVERT,&imgWeather_WHITE);
	}
	else if(current_weather == 2) //rain
	{	
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_RAIN,&imgWeather);
		//bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_RAIN,&imgWeather_BLACK);
		//bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_RAIN_INVERT,&imgWeather_WHITE);
	}
	else if(current_weather == 3)//cloud
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_CLOUD,&imgWeather);
		//bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_CLOUD,&imgWeather_BLACK);
		//bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_CLOUD_INVERT,&imgWeather_WHITE);
	}
	else if(current_weather == 4)//snow
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SNOW,&imgWeather);
		//bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SNOW,&imgWeather_BLACK);
		//bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SNOW_INVERT,&imgWeather_WHITE);
	}
}

/*void SetDateImage()
{	
	if (current_weather == 0)//unknown
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_UNKNOWN,&imgWeather_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_UNKNOWN_INVERT,&imgWeather_WHITE);
	}		
	else if (current_weather == 1) //sun
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SUN,&imgWeather_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SUN_INVERT,&imgWeather_WHITE);
	}
	else if(current_weather == 2) //rain
	{	
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_RAIN,&imgWeather_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_RAIN_INVERT,&imgWeather_WHITE);
	}
	else if(current_weather == 3)//cloud
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_CLOUD,&imgWeather_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_CLOUD_INVERT,&imgWeather_WHITE);
		}
	else if(current_weather == 4)//snow
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SNOW,&imgWeather_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SNOW_INVERT,&imgWeather_WHITE);
	}
}

void SetTimeImage()
{	
	if (current_weather == 0)//unknown
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_UNKNOWN,&imgWeather_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_UNKNOWN_INVERT,&imgWeather_WHITE);
	}		
	else if (current_weather == 1) //sun
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SUN,&imgWeather_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SUN_INVERT,&imgWeather_WHITE);
	}
	else if(current_weather == 2) //rain
	{	
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_RAIN,&imgWeather_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_RAIN_INVERT,&imgWeather_WHITE);
	}
	else if(current_weather == 3)//cloud
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_CLOUD,&imgWeather_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_CLOUD_INVERT,&imgWeather_WHITE);
		}
	else if(current_weather == 4)//snow
	{
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SNOW,&imgWeather_BLACK);
		bmp_init_container(RESOURCE_ID_IMAGE_WEATHER_SNOW_INVERT,&imgWeather_WHITE);
	}
}*/


void RemoveAndDeIntBmp(char* bitmap)
{
	if (strcmp(bitmap,WORD) == 0)
	{
		layer_remove_from_parent(&layerWord_WHITE.layer);
		layer_remove_from_parent(&layerWord_BLACK.layer);
		bmp_deinit_container(&imgWord_WHITE);
		bmp_deinit_container(&imgWord_BLACK);
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
		/*bitmap_layer_init(&layerWeather_WHITE,weatherFrame);
		bitmap_layer_set_bitmap(&layerWeather_WHITE,&imgWeather_WHITE.bmp);
		bitmap_layer_set_background_color(&layerWeather_WHITE,GColorClear);
		bitmap_layer_set_compositing_mode(&layerWeather_WHITE, GCompOpClear);
		layer_add_child(&window.layer, &layerWeather_WHITE.layer);
		layer_mark_dirty(&layerWeather_WHITE.layer);*/

		/*// takes the white and doesn't draw it if it itercets with a black?
		bitmap_layer_init(&layerWeather_BLACK,WeatherFrame);
		bitmap_layer_set_bitmap(&layerWeather_BLACK, &imgWeather_BLACK.bmp); //or .layer?
		layer_add_child(&window.layer, &layerWeather_BLACK.layer);
		bitmap_layer_set_background_color(&layerWeather_BLACK,GColorClear);
		bitmap_layer_set_compositing_mode(&layerWeather_BLACK, GCompOpOr);
		layer_mark_dirty(&layerWeather_BLACK.layer);*/
	}
	else if (strcmp(bitmap,DATE) == 0)
	{
		//do 4-5 (slash) times for each digit set
		/*SetDateImage();			
		bitmap_layer_init(&layerDate_WHITE,DateFrame);
		bitmap_layer_set_bitmap(&layerDate_WHITE,&imgDate_WHITE.bmp);
		bitmap_layer_set_background_color(&layerDate_WHITE,GColorClear);
		bitmap_layer_set_compositing_mode(&layerDate_WHITE, GCompOpClear);
		layer_add_child(&window.layer, &layerDate_WHITE.layer);
		layer_mark_dirty(&layerDate_WHITE.layer);	

		// takes the white and doesn't draw it if it itercets with a black?
		bitmap_layer_init(&layerDate_BLACK,DateFrame);
		bitmap_layer_set_bitmap(&layerDate_BLACK, &imgDate_BLACK.bmp); //or .layer?
		layer_add_child(&window.layer, &layerDate_BLACK.layer);
		bitmap_layer_set_background_color(&layerDate_BLACK,GColorClear);
		bitmap_layer_set_compositing_mode(&layerDate_BLACK, GCompOpOr);
		layer_mark_dirty(&layerDate_BLACK.layer);*/	
	}
	else if (strcmp(bitmap,TIME) == 0)
	{
		/*SetTimeImage();			
		//do 4 times (1 for each digit)
		bitmap_layer_init(&layerTime_WHITE,TimeFrame);
		bitmap_layer_set_bitmap(&layerTime_WHITE,&imgTime_WHITE.bmp);
		bitmap_layer_set_background_color(&layerTime_WHITE,GColorClear);
		bitmap_layer_set_compositing_mode(&layerTime_WHITE, GCompOpClear);
		layer_add_child(&window.layer, &layerTime_WHITE.layer);
		layer_mark_dirty(&layerTime_WHITE.layer);	

		// takes the white and doesn't draw it if it itercets with a black?
		bitmap_layer_init(&layerTime_BLACK,TimeFrame);
		bitmap_layer_set_bitmap(&layerTime_BLACK, &imgTime_BLACK.bmp); //or .layer?
		layer_add_child(&window.layer, &layerTime_BLACK.layer);
		bitmap_layer_set_background_color(&layerTime_BLACK,GColorClear);
		bitmap_layer_set_compositing_mode(&layerTime_BLACK, GCompOpOr);
		layer_mark_dirty(&layerTime_BLACK.layer);*/
	}
}
/*
void SetTime()
{
	//set_container_image(&date_digits_images[3], &date_layer, date_resource_ids[3], MED_DIGIT_IMAGE_RESOURCE_IDS[month%10], GPoint(0, 84));
}
void GetAndSetTime()
{}*/

void GetAndSetWeather()
{
    //get info from phone,
	getWeather = false;
	current_weather = 1;
    if (current_weather != previous_weather)
	{
		SetBitmap(WEATHER);
		previous_weather = current_weather;
	}
}

void GetAndSetCurrentWord(PblTm* current_time)
{
    switch (current_time->tm_hour)
    {
        case 0:
            current_word = 0; //midnight
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            current_word = 1; //early morning
            break;
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            current_word = 2; //morning
            break;
        case 11:
		case 12:
		case 13:
            current_word = 3;//lunchtime
            break;
        case 14:
        case 15:
        case 16:
		case 17:
            current_word = 4;//afternoon
            break;
        case 18:
        case 19:
		case 20:
            current_word = 5;//evening
            break;
        case 21:
        case 22:
        case 23:
        case 24:
            current_word = 6;//night
            break;
    }
    
    
    if (previous_word != current_word)
    {
		SetBitmap(WORD);
		previous_word = current_word;
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
	if (current_word != previous_word)
        isWordTransitioning = true;
	isWordTransitioning = false; //DEBUG -- for time being
}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;
  (void)ctx;
	
	static char date_text[] = "Xxx";
	string_format_time(date_text, sizeof(date_text), "%a", t->tick_time);
	text_layer_set_text(&layerWord_DATE, date_text);
	  
	static char time_text[] = "00:00";
	char *time_format;
	time_format = clock_is_24h_style() ? "%R" : "%I:%M";
	string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);
	//CheckTransitionTime();
	//if (!isDayTransition && !isWordTransition)
	//{
		// Kludge to handle lack of non-padded hour format string
		// for twelve hour clock.
		if (!clock_is_24h_style() && (time_text[0] == '0'))
		{
			memmove(time_text, &time_text[1], sizeof(time_text) - 1);
		}
		
		// update everything accordingly
		//static char date_text[] = "00 00";
		//string_format_time(date_text, sizeof(date_text), "%m　%d", t->tick_time);
		//SetBitmap("Date",date_text);
		//SetBitmap("Time", time_text);
		
		if (previous_hour != time_text[1])
		{
			previous_hour = time_text[1];
			GetAndSetCurrentWord(t->tick_time);
			//if within check for weather()
			if (getWeather)
			{GetAndSetWeather();}
		}
	//}
}

void RefreshAll()
{
	//SetBitmap(DATE);
    //SetBitmap(TIME);
	SetBitmap(WORD);
	//SetBitmap(WEATHER);
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
	res_d = resource_get_handle(RESOURCE_ID_FONT_DAYS_21);
	font_date = fonts_load_custom_font(res_d);
	text_layer_init(&layerWord_DATE, window.layer.frame);
    text_layer_set_text_color(&layerWord_DATE, GColorWhite);
    text_layer_set_background_color(&layerWord_DATE, GColorClear);
    layer_set_frame(&layerWord_DATE.layer,wordDateFrame);
	//text_layer_set_text_alignment(&layerWord_DATE,GTextAlignmentRight);
    text_layer_set_font(&layerWord_DATE, font_date);
    layer_add_child(&window.layer, &layerWord_DATE.layer);
	
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
	
		
    //font stuff

    
    //SETUP INIT STUFF
    previous_hour = 99;
    previous_word = 99;
    current_word = 1;
	previous_weather = 99;
	current_weather = 0; //uNKNOWN
	wordDateFrame = GRect(0,80,64,40);
	wordFrame = GRect(0,40,144,21);
	weatherFrame = GRect(53,87,75,75);
	timeFrame = GRect(0,66,144,30);
	dateFrame = GRect(0,5,100,30);

	//run into transition	
	Watchface();
	//DayTransition(Leads into BG Display)	
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
        }
    };

    app_event_loop(params, &handlers);
}

void handle_deinit(AppContextRef ctx) {
	(void)ctx;
		bmp_deinit_container(&background_image);
		bmp_deinit_container(&imgWord_BLACK);
		bmp_deinit_container(&imgWord_WHITE);
		bmp_deinit_container(&imgWeather);
		//bmp_deinit_container(&imgWeather_BLACK);
		//bmp_deinit_container(&imgWeather_WHITE);
		bmp_deinit_container(&slash);
		bmp_deinit_container(&colon);
		
		/*for (int i = 0; i < TOTAL_TIME_DIGITS; i++) {
			bmp_deinit_container(&imgTime[i]);
		}
		
		for (int i = 0; i < TOTAL_TIME_DIGITS; i++) {
			bmp_deinit_container(&imgDate[i]);
		}		*/
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