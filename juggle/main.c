#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "constants.h"

static void handle_input(void);
static void tick(int signum);
static void move_balls(void);
static void redraw(void);
static void game_over(void);

static int ball_positions[] = {
	4, 9, 5, 7, 7, 6, 10, 4, 12, 4, 15, 6, 17, 7, 18, 9, -1, -1, -1, -1, -1, -1, -1, -1,
	2, 9, 2, 7, 3, 5, 5, 3, 9, 2, 13, 2, 17, 3, 19, 5, 20, 7, 20, 9, -1, -1, -1, -1,
	0, 9, 0, 6, 1, 4, 3, 2, 6, 1, 9, 0, 13, 0, 16, 1, 19, 2, 21, 4, 22, 6, 22, 9
};
static int ball_indices[] = { 6, 12, 12 };
static int ball_speeds[] = { -2, 2, -2 };
static int ball_lengths[] = { 14, 18, 22 };
static int ball_anim = 1;
static char *playfield;
static int score = 0, dying = 0, ply_anim = 1;
extern char *player_l;
extern char *player_r;
extern char *player_m;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	initscr();
	if (has_colors() == FALSE) {
		endwin();
		fprintf(stderr, "Your terminal does not support color\n");
		exit(1);
	}
	start_color();
	nonl();
	cbreak();
	noecho();
	keypad(stdscr, true);
	init_pair(CANVAS_PEN, COLOR_BLACK, COLOR_WHITE);
	init_pair(MAN_PEN, COLOR_BLACK, COLOR_YELLOW);
	init_pair(BALL_PEN, COLOR_BLACK, COLOR_GREEN);
	clear();
	curs_set(0);

	playfield = calloc(SCREENW + 1, sizeof(char));
	if (playfield == NULL) {
		perror("Failed to allocate playfield");
		exit(1);
	}

	memset(playfield, ' ', SCREENW);
	refresh();

	struct sigaction action;
	action.sa_handler = &tick;
	action.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &action, NULL);

	struct itimerval timer;
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 1000000 / FPS;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 1000000 / FPS;
	setitimer(ITIMER_REAL, &timer, NULL);

	for EVER {
		if (dying) {
			continue;
		}
		handle_input();
	}

	game_over();
}

static void handle_input(void)
{
	int c = getch();
	if (c == KEY_LEFT && ply_anim > 0) {
		ply_anim--;
	} else if (c == KEY_RIGHT && ply_anim < 2) {
		ply_anim++;
	}
}

static void tick(int signum)
{
	move_balls();
	redraw();
}

static void move_balls(void)
{
	ball_anim--;
	if (ball_anim > 0) {
		return;
	}

	ball_anim = 7;
	for (int i = 0; i < BALLS; i++) {
		ball_indices[i] += ball_speeds[i];
		if (ball_indices[i] < 0) {
			ball_indices[i] = 0;
			ball_speeds[i] = -ball_speeds[i];
		} else if (ball_indices[i] > ball_lengths[i]) {
			ball_indices[i] = ball_lengths[i];
			ball_speeds[i] = -ball_speeds[i];
		}
	}
}

static void redraw(void)
{
	/*
	if (dying > 0) {
		if (dying > 200) {
			game_over();
			return;
		}

		dying++;
	} else {
		animate_bomb();
	}
	*/

	attron(COLOR_PAIR(CANVAS_PEN));
	for (int y = 0; y < SCREENH; y++) {
		mvprintw(y, 0, "%s", playfield);
	}
	mvprintw(0, 0, "Score: %d", score);
	attroff(COLOR_PAIR(CANVAS_PEN));

	attron(COLOR_PAIR(MAN_PEN));
	int offset_x = (SCREENW - PLAYER_W) / 2;
	int y = SCREENH - PLAYER_H;
	char *player = ply_anim == 0 ? player_l : (ply_anim == 1 ? player_m : player_r);
	for (int i = 0; i < PLAYER_H; i++) {
		for (int j = offset_x; j < PLAYER_W + offset_x; j++) {
			if (*player == '*') {
				mvprintw(y, j, "%s", " ");
			}
			player++;
		}
		y++;
	}
	attroff(COLOR_PAIR(MAN_PEN));

	attron(COLOR_PAIR(BALL_PEN));
	offset_x++;
	for (int i = 0; i < BALLS; i++) {
		int *p = ball_positions + (i * 24) + ball_indices[i];
		int x = offset_x + *p++;
		int y = *p;
		mvprintw(y + 3, x, "%s", " ");
	}
	attroff(COLOR_PAIR(BALL_PEN));
	/*
	if (dying > 0) {
		int d = dying / 20;
		char background = *(playfield + (player_y * (SCREENW + 1)) + player_x);
		mvaddch(player_y, player_x, d % 2 ? background : 'V');
	} else {
		mvaddch(player_y, player_x, 'V');
	}

	*/
	refresh();
}

static void game_over(void)
{
	endwin();
	printf("You scored: %d\n\n", score);
	exit(0);
}
