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
TextLayer text_time_layer;
TextLayer text_date_layer;
Layer word_layer;
Layer transition_layer;
Layer weather_layer;

BmpContainer background_image;
RotBmpPairContainer displayword;
RotBmpPairContainer weather;


int current_word;
int previous_word;
int previous_hour;

int current_weather;
int previous_weather;
bool getWeather = true;
//Layer weather_layer;
//Word Transitions
/*Layer transition_word_layer;
Layer transition_block_1_layer;
Layer transition_block_2_layer;
Layer transition_square_layer;*/

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

void layer_update_callback(Layer *me, GContext* ctx) {
  (void)me;
  (void)ctx;
	layer_remove_from_parent(&displayword.layer.layer);
	rotbmp_pair_deinit_container(&displayword);
	
	if (current_word == 0)//midnight
	{
		rotbmp_pair_init_container(RESOURCE_ID_IMAGE_TIME_MIDNIGHT_WHITE,RESOURCE_ID_IMAGE_TIME_MIDNIGHT_BLACK,&displayword);
	}
	else if (current_word == 1) //early morning
	{
		rotbmp_pair_init_container(RESOURCE_ID_IMAGE_TIME_EARLYMORNING_WHITE,RESOURCE_ID_IMAGE_TIME_EARLYMORNING_BLACK,&displayword);
	}
	else if(current_word == 2) //morning
	{
		rotbmp_pair_init_container(RESOURCE_ID_IMAGE_TIME_MORNING_WHITE,RESOURCE_ID_IMAGE_TIME_MORNING_BLACK,&displayword);
	}
	else if(current_word == 3)//lunchtime
	{
		rotbmp_pair_init_container(RESOURCE_ID_IMAGE_TIME_LUNCHTIME_WHITE,RESOURCE_ID_IMAGE_TIME_LUNCHTIME_BLACK,&displayword);
	}
	else if(current_word == 4)//afternoon
	{
		rotbmp_pair_init_container(RESOURCE_ID_IMAGE_TIME_AFTERNOON_WHITE,RESOURCE_ID_IMAGE_TIME_AFTERNOON_BLACK,&displayword);
	}
	else if(current_word == 5)//evening
	{
		rotbmp_pair_init_container(RESOURCE_ID_IMAGE_TIME_EVENING_WHITE,RESOURCE_ID_IMAGE_TIME_EVENING_BLACK,&displayword);
	}
	else if(current_word == 6)//night
	{
		rotbmp_pair_init_container(RESOURCE_ID_IMAGE_TIME_NIGHT_WHITE,RESOURCE_ID_IMAGE_TIME_NIGHT_BLACK,&displayword);
	}
        	
  // We make sure the dimensions of the GRect to draw into
  // are equal to the size of the bitmap--otherwise the image
  // will automatically tile. Which might be what *you* want.

	layer_add_child(&word_layer, &displayword.layer.layer);
}

void weather_update_callback(Layer *me, GContext* ctx) {
  (void)me;
  (void)ctx;
	//@TODO:
	//Compositite/Combine certain images.
	//Either compile the images in photoshop, or figure them out here
	layer_remove_from_parent(&weather.layer.layer);
	rotbmp_pair_deinit_container(&weather);
	
	if (current_weather == 0)//UNKNOWN/BLANK
	{
		rotbmp_pair_init_container(RESOURCE_ID_IMAGE_WEATHER_UNKNOWN_WHITE,RESOURCE_ID_IMAGE_WEATHER_UNKNOWN_BLACK,&weather);
	}
	else if (current_weather == 1) //SUNNY
	{
		rotbmp_pair_init_container(RESOURCE_ID_IMAGE_WEATHER_SUN_WHITE,RESOURCE_ID_IMAGE_WEATHER_SUN_BLACK,&weather);
	}
	else if(current_weather == 2) //RAINY
	{
		rotbmp_pair_init_container(RESOURCE_ID_IMAGE_WEATHER_RAIN_WHITE,RESOURCE_ID_IMAGE_WEATHER_RAIN_BLACK,&weather);
	}
	else if(current_weather == 3)//CLOUDY
	{
		rotbmp_pair_init_container(RESOURCE_ID_IMAGE_WEATHER_CLOUD_WHITE,RESOURCE_ID_IMAGE_WEATHER_CLOUD_BLACK,&weather);
	}
	else if(current_weather == 4)//SNOWMAN
	{
		rotbmp_pair_init_container(RESOURCE_ID_IMAGE_WEATHER_SNOW_WHITE,RESOURCE_ID_IMAGE_WEATHER_SNOW_BLACK,&weather);
	}
	        	
  // We make sure the dimensions of the GRect to draw into
  // are equal to the size of the bitmap--otherwise the image
  // will automatically tile. Which might be what *you* want.

	layer_add_child(&weather_layer, &weather.layer.layer);
  //graphics_draw_bitmap_in_rect(ctx, &displayword.bmp, destination);
}

void GetAndSetWeather()
{
    //get info from phone,
	getWeather = false;
	current_weather = 1;
    weather_layer.update_proc = &weather_update_callback;
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
		word_layer.update_proc = &layer_update_callback;
		previous_word = current_word;
    }
    
}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;
  (void)ctx;
  static char date_text[] = "00 00 Xxx";
  string_format_time(date_text, sizeof(date_text), "%m/%d %a", t->tick_time);
  text_layer_set_text(&text_date_layer, date_text);
  
  static char time_text[] = "00:00";
  char *time_format;

    time_format = clock_is_24h_style() ? "%R" : "%I:%M";
    string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);

    // Kludge to handle lack of non-padded hour format string
    // for twelve hour clock.
    if (!clock_is_24h_style() && (time_text[0] == '0'))
    {
        memmove(time_text, &time_text[1], sizeof(time_text) - 1);
    }
    
    text_layer_set_text(&text_time_layer, time_text);
    
    if (previous_hour != time_text[1])
    {
        previous_hour = time_text[1];
        GetAndSetCurrentWord(t->tick_time);
		//if within check for weather()
		if (getWeather)
		{GetAndSetWeather();}
    }
}

void handle_init(AppContextRef ctx) {
    ResHandle res_d;
    ResHandle res_t;
    GFont font_date;
    GFont font_time;
    
    (void) ctx;

    window_init(&window, "PEBSONA4");
    window_stack_push(&window, true /* Animated */);
    window_set_background_color(&window, GColorClear);

    resource_init_current_app(&APP_RESOURCES);
    //Create Containers
    bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND,  &background_image);
	rotbmp_pair_init_container(RESOURCE_ID_IMAGE_BLANK_WHITE,RESOURCE_ID_IMAGE_BLANK_BLACK,&displayword);
	rotbmp_pair_init_container(RESOURCE_ID_IMAGE_WEATHER_UNKNOWN_WHITE,RESOURCE_ID_IMAGE_WEATHER_UNKNOWN_BLACK,&weather);
	
    //font stuff
    res_t = resource_get_handle(RESOURCE_ID_FONT_DAYS_19);
    res_d = resource_get_handle(RESOURCE_ID_FONT_DAYS_22);
    font_time = fonts_load_custom_font(res_t);
    font_date = fonts_load_custom_font(res_d);
    
    previous_hour = 99;
    previous_word = 99;
    current_word = 0;
	previous_weather = 99;
	current_weather = 0; //sunny
    layer_add_child(&window.layer,&background_image.layer.layer);
        
    text_layer_init(&text_date_layer, window.layer.frame);
    text_layer_set_text_color(&text_date_layer, GColorWhite);
    text_layer_set_background_color(&text_date_layer, GColorClear);
    layer_set_frame(&text_date_layer.layer,GRect(0,5,144,40));
    text_layer_set_font(&text_date_layer, font_date);
    layer_add_child(&window.layer, &text_date_layer.layer);
        
    text_layer_init(&text_time_layer, window.layer.frame);
    text_layer_set_text_color(&text_time_layer, GColorWhite);
    text_layer_set_background_color(&text_time_layer, GColorClear);
    layer_set_frame(&text_time_layer.layer, GRect(0, 140, 80,55));
    text_layer_set_font(&text_time_layer, font_time);
    layer_add_child(&window.layer, &text_time_layer.layer);

	    //not my prefered implmentation, but until i can cut down size/get it wroking right,
    //it'll have to do

    //"MIDNIGHT" Layer White
    layer_init(&word_layer, window.layer.frame);
//	GRect word_destination = layer_get_frame(&displayword.layer.layer);	
  //word_destination.origin.x = 0;
  //word_destination.origin.y = 45;
	layer_set_frame(&word_layer, GRect(0,25,144,40));
  	word_layer.update_proc = &layer_update_callback;
  	layer_add_child(&window.layer, &word_layer);
	
	layer_init(&weather_layer, window.layer.frame);
	layer_set_frame(&weather_layer, GRect(34,68,90,90));
  	weather_layer.update_proc = &weather_update_callback;
  	layer_add_child(&window.layer, &weather_layer);
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
		rotbmp_pair_deinit_container(&displayword);
		rotbmp_pair_deinit_container(&weather);
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
