#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

enum KEYS { 
	UP, 
	LEFT, 
	RIGHT 
};

int main(void)
{
	int width = 640;
	int height = 480;

	bool done = false;
	bool redraw = true;

	int pos_x = width / 2;
	int pos_y = height / 2;

	int FPS = 59;

	bool keys[4] = { false, false, false, false };

	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;

	if (!al_init())										
		return -1;

	display = al_create_display(width, height);			

	if (!display)										
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_start_timer(timer);
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			pos_y -= keys[UP] * 25;
			pos_x -= keys[LEFT] * 25;
			pos_x += keys[RIGHT] * 25;

			redraw = true;
		}

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, al_map_rgb(255, 0, 0));
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}

	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);						

	return 0;
}