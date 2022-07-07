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
static void move_ball(int ball, int low, int high);
static int move_and_check_ball(int ball, int ply_index);
static int is_dying(void);
static void redraw(void);
static void game_over(void);

static int ball_positions[] = {
	0, 9, 0, 6, 1, 4, 3, 2, 6, 1, 9, 0, 13, 0, 16, 1, 19, 2, 21, 4, 22, 6, 22, 9,
	2, 9, 2, 7, 3, 5, 5, 3, 9, 2, 13, 2, 17, 3, 19, 5, 20, 7, 20, 9, -1, -1, -1, -1,
	4, 9, 5, 7, 7, 6, 10, 4, 12, 4, 15, 6, 17, 7, 18, 9, -1, -1, -1, -1, -1, -1, -1, -1
};
static int ball_indices[] = { 12, 12, 8 };
static int ball_speeds[] = { -2, 2, -2 };
static int ball_lengths[] = { 22, 18, 14 };
static int ball_anim = 1;
static int ball_dying[] = { 0, 0, 0 };
static char *playfield;
static int score = 0, ply_anim = 1, dying_anim = 0;
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
		handle_input();
	}

	game_over();
}

static void handle_input(void)
{
	int c = getch();
	if (is_dying()) {
		return;
	}
	if (c == KEY_LEFT && ply_anim > 0) {
		ply_anim--;
	} else if (c == KEY_RIGHT && ply_anim < 2) {
		ply_anim++;
	}
}

static void tick(int signum)
{
	if (is_dying()) {
		if (dying_anim > 200) {
			game_over();
			return;
		}

		dying_anim++;
	} else {
		move_balls();
	}
	redraw();
}

static void move_balls(void)
{
	ball_anim--;
	if (ball_anim > 0) {
		return;
	}

	ball_anim = 31;

	move_ball(BALL_O, 0, 2);
	move_ball(BALL_M, 1, 1);
	move_ball(BALL_I, 2, 0);
}

static void move_ball(int ball, int low, int high)
{
	if (ball_indices[ball] == 0)
	{
		if (move_and_check_ball(ball, low)) {
			return;
		}
	} else if (ball_indices[ball] >= ball_lengths[ball]) {
		if (move_and_check_ball(ball, high)) {
			return;
		}
	}

	ball_indices[ball] += ball_speeds[ball];
}

static int move_and_check_ball(int ball, int ply_index)
{
	if (ply_anim == ply_index) {
		ball_speeds[ball] = -ball_speeds[ball];
		score++;
		return 0;
	}

	ball_dying[ball] = 1;
	return 1;
}

static int is_dying(void)
{
	return ball_dying[BALL_O] || ball_dying[BALL_M] || ball_dying[BALL_I];
}

static void redraw(void)
{
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

	offset_x++;
	int d = is_dying() ? dying_anim / 20 : 0;
	for (int i = 0; i < BALLS; i++) {
		int *p = ball_positions + (i * 24) + ball_indices[i];
		int x = offset_x + *p++;
		int y = *p;
		int pen = COLOR_PAIR((ball_dying[i] && (d % 2)) ? CANVAS_PEN : BALL_PEN);
		attron(pen);
		mvprintw(y + 3, x, "%s", " ");
		attroff(pen);
	}

	refresh();
}

static void game_over(void)
{
	endwin();
	printf("You scored: %d\n\n", score);
	exit(0);
}
