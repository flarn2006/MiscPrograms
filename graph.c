/* Created by flarn2006 <flarn2006@gmail.com> */
/* Compile with libmatheval (http://www.gnu.org/software/libmatheval/) */
/* Also requires ncurses (Google it.) */
/* To compile without libmatheval, use -DNOLIBMATHEVAL. */
/* (Edit defaultFunction to change which function to graph then.) */

#include <curses.h>
#include <form.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef NOLIBMATHEVAL
#include <matheval.h>
#endif

#define XMIN -2*M_PI
#define XMAX 2*M_PI
#define YMIN -M_PI
#define YMAX M_PI
#define XSCL 1
#define YSCL 1

typedef struct _viewwin viewwin;
typedef struct _khdata khdata;
typedef double (*yfunction)(double x);
typedef void (*key_handler)(int key, khdata *data);
typedef enum {MODE_GRAPH, MODE_TRACE} oper_mode;

struct _viewwin {
	double xmin, xmax;
	double ymin, ymax;
	double xscl, yscl;
};

struct _khdata {
	/* Struct for pointers to data that key handlers may need to access */
	viewwin *view;
	oper_mode *mode;
	double *trace;
};

#ifndef NOLIBMATHEVAL
void *eval = NULL;
#endif

void defaultKeyHandler(int key, khdata *data);

int enableSlopeChars = 1;
key_handler handleKey = defaultKeyHandler;

double defaultFunction(double x)
{
	return sin(x);
}

double estimateSlope(yfunction func, double x, double accuracy)
{
	double y1 = func(x - accuracy);
	double y2 = func(x + accuracy);
	return (y2 - y1) / (2 * accuracy);
}

double scale(double value, double omin, double omax, double nmin, double nmax)
{
	/* Useful function to scale a value in one range to a different range.
	   omin/omax - old range
	   nmin/nmax - new range
	*/
	double x = (value - omin) / (omax - omin);
	return x * (nmax - nmin) + nmin;
}

void plotPoint(WINDOW *win, const viewwin *view, double x, double y, int ch, int *scrY, int *scrX)
{
	/* Displays a point on the screen at a location determined by graph coordinates.
	   win       - ncurses window for drawing (can be NULL to only set scrY and scrX w/o drawing)
	   view      - view parameters structure
	   x/y       - graph coordinates for point
	   ch        - character to display
	   scrY/scrX - screen coordinates where point was drawn are saved here if not NULL
	*/
	int xm, ym; getmaxyx(win, ym, xm);
	int xp = scale(x, view->xmin, view->xmax, 0, xm);
	int yp = scale(y, view->ymin, view->ymax, ym, 0);
	
	if (scrX) *scrX = xp;
	if (scrY) *scrY = yp;

	if (win) mvwaddch(win, yp, xp, ch);
}

char slopeChar(double slope)
{
	/* Gets the character to display at a point in the graph with a given slope. */
	
	double a = fabs(slope);
	if (a < 1.5)        return '=';
	else if (a < 5.0)   return slope>0 ? '/' : '\\';
	else                return '|';
}

#define FIELD_MAX_CHARS 32

int editViewWindow(viewwin *view)
{
	/* Figure out where to put the window */
	int wwidth = 21, wheight = 11;
	int ym, xm; getmaxyx(stdscr, ym, xm);
	int wy = ym/2 - wheight/2;
	int wx = xm/2 - wwidth/2;

	/* Create a new window and draw the form */
	WINDOW *fwin = newwin(wheight, wwidth, wy, wx);
	keypad(fwin, TRUE);
	werase(fwin);
	
	wattron(fwin, A_BOLD);
	box(fwin, 0, 0);
	mvwprintw(fwin, 0, 4, " VIEW WINDOW ");
	mvwprintw(fwin, 2, 3, "Xmin =");
	mvwprintw(fwin, 3, 3, "Xmax =");
	mvwprintw(fwin, 4, 3, "Xscl =");
	mvwprintw(fwin, 5, 3, "Ymin =");
	mvwprintw(fwin, 6, 3, "Ymax =");
	mvwprintw(fwin, 7, 3, "Yscl =");
	wattroff(fwin, A_BOLD);

	mvwprintw(fwin, 9, 6, "[SPC] OK");

	/* Create the form fields */
	FIELD *fields[7];
	int i; for (i=0; i<6; i++) {
		fields[i] = new_field(1, 8, i, 0, 0, 0);
		set_field_back(fields[i], A_REVERSE | A_UNDERLINE);
		set_field_type(fields[i], TYPE_NUMERIC, 6, 0.0, 0.0);
		field_opts_off(fields[i], O_AUTOSKIP | O_STATIC);
		set_max_field(fields[i], FIELD_MAX_CHARS);
	}
	fields[6] = NULL;

	/* Fill the form fields with initial values */
	char printbuf[FIELD_MAX_CHARS+1];
	snprintf(printbuf, FIELD_MAX_CHARS+1, "%.5lf", view->xmin);
	set_field_buffer(fields[0], 0, printbuf);
	snprintf(printbuf, FIELD_MAX_CHARS+1, "%.5lf", view->xmax);
	set_field_buffer(fields[1], 0, printbuf);
	snprintf(printbuf, FIELD_MAX_CHARS+1, "%.5lf", view->xscl);
	set_field_buffer(fields[2], 0, printbuf);
	snprintf(printbuf, FIELD_MAX_CHARS+1, "%.5lf", view->ymin);
	set_field_buffer(fields[3], 0, printbuf);
	snprintf(printbuf, FIELD_MAX_CHARS+1, "%.5lf", view->ymax);
	set_field_buffer(fields[4], 0, printbuf);
	snprintf(printbuf, FIELD_MAX_CHARS+1, "%.5lf", view->yscl);
	set_field_buffer(fields[5], 0, printbuf);

	/* Create a subwindow for the form fields */
	WINDOW *fsub = derwin(fwin, 6, 8, 2, 10);
	keypad(fsub, TRUE);
	
	/* Create the actual form */
	FORM *f = new_form(fields);
	set_form_win(f, fwin);
	set_form_sub(f, fsub);
	post_form(f);
	wrefresh(fwin);
	wrefresh(fsub);
	curs_set(1);

	/* Handle input */
	int savewin = 1;
	int exitloop = 0;
	int ch; while (!exitloop) {
		switch (ch = wgetch(fwin)) {
			case 'j': case '\n': case KEY_DOWN:
				form_driver(f, REQ_NEXT_FIELD);
				break;
			case 'k': case KEY_UP:
				form_driver(f, REQ_PREV_FIELD);
				break;
			case KEY_BACKSPACE:
				form_driver(f, REQ_DEL_PREV);
				break;
			case ' ':
				exitloop = 1;
				break;
			case 'q':
				savewin = 0;
				exitloop = 1;
				break;
			default:
				form_driver(f, ch);
				break;
		}
	}

	/* Save the window parameters if necessary */
	viewwin view_temp;  /* Storage for pre-validation values */

	if (savewin) {
		sscanf(field_buffer(fields[0], 0), "%lf", &view_temp.xmin);
		sscanf(field_buffer(fields[1], 0), "%lf", &view_temp.xmax);
		sscanf(field_buffer(fields[2], 0), "%lf", &view_temp.xscl);
		sscanf(field_buffer(fields[3], 0), "%lf", &view_temp.ymin);
		sscanf(field_buffer(fields[4], 0), "%lf", &view_temp.ymax);
		sscanf(field_buffer(fields[5], 0), "%lf", &view_temp.yscl);

		if (view_temp.xmin >= view_temp.xmax || view_temp.ymin >= view_temp.ymax) savewin = 0;
	}

	if (savewin) {
		/* Copy contents of view_temp to view */
		memcpy(view, &view_temp, sizeof(viewwin));
	}

	/* Clean up */
	curs_set(0);
	unpost_form(f);
	free_form(f);
	for (i=0; i<6; i++) free_field(fields[i]);
	delwin(fsub);
	delwin(fwin);
	refresh();

	return savewin;
}

void getViewStep(WINDOW *win, const viewwin *view, double *xstep, double *ystep)
{
	/* Gets the 'value' of one character on either or both axes. */

	int xm, ym; getmaxyx(win, ym, xm);
	if (xstep) *xstep = (view->xmax - view->xmin) / (xm + 1);
	if (ystep) *ystep = (view->ymax - view->ymin) / (ym + 1);
}

void drawAxes(WINDOW *win, const viewwin *view)
{
	/* This function is what draws the axes on the screen. */
	
	int xm, ym; getmaxyx(win, ym, xm);
	double x0 = scale(0, view->xmin, view->xmax, 0, xm);
	double y0 = scale(0, view->ymin, view->ymax, ym, 0);

	double xstep, ystep; getViewStep(win, view, &xstep, &ystep);
	
	int i; for (i=0; i<=xm; i++) {
		double plotx = view->xmin + xstep * i;
		int tick = fabs(fmod(plotx, view->xscl)) < xstep;
		mvwaddch(win, y0, i, tick ? ACS_PLUS:ACS_HLINE);
	}
	for (i=0; i<=ym; i++) {
		double ploty = view->ymin + ystep * i;
		int tick = fabs(fmod(ploty, view->yscl)) < ystep;
		mvwaddch(win, i, x0, tick ? ACS_PLUS:ACS_VLINE);
	}
	
	mvwaddch(win, y0, x0, ACS_PLUS);
}

void drawGraph(WINDOW *win, const viewwin *view, yfunction yfunc, int enableSlopeChars)
{
	/* Draws a graph on the screen without axes.
	   win              - ncurses window for drawing
	   view             - view parameters structure
	   yfunc            - function to graph (function pointer)
	   enableSlopeChars - whether or not to call slopeChar to determine characters
	*/
	int xm, ym; getmaxyx(win, ym, xm);
	double step; getViewStep(win, view, &step, NULL);
	double x; for (x = view->xmin; x <= view->xmax; x += step)
	{
		double y = yfunc(x);
		char ch = '#';
		if (enableSlopeChars) {
			double deriv = estimateSlope(yfunc, x, step/2);
			double view_ratio = (view->ymax - view->ymin) / (view->xmax - view->xmin);
			ch = slopeChar(deriv / view_ratio);
		}
		plotPoint(win, view, x, y, ch, NULL, NULL);
	}
}

#ifndef NOLIBMATHEVAL
double performEval(double x)
{
	return evaluator_evaluate_x(eval, x);
}
#endif

void zoomOut(viewwin *view, float factor)
{
	float centerX = (view->xmin + view->ymax) / 2;
	float centerY = (view->ymin + view->ymax) / 2;
	view->xmin = scale(factor, 1.0f, 0.0f, view->xmin, centerX);
	view->xmax = scale(factor, 1.0f, 0.0f, view->xmax, centerX);
	view->ymin = scale(factor, 1.0f, 0.0f, view->ymin, centerY);
	view->ymax = scale(factor, 1.0f, 0.0f, view->ymax, centerY);
}

void traceKeyHandler(int key, khdata *data)
{
	/* Keyboard handling function for trace mode. */
	
	double step; getViewStep(stdscr, data->view, &step, NULL);

	switch (key) {
		case 'h': case KEY_LEFT:  *data->trace -= step; break;
		case 'l': case KEY_RIGHT: *data->trace += step; break;
	}

	if (*data->trace < data->view->xmin) *data->trace = data->view->xmin;
	if (*data->trace > data->view->xmax) *data->trace = data->view->xmax;

	if (key == 't') {
		handleKey = defaultKeyHandler;
		*data->mode = MODE_GRAPH;
	}

	if (key == '\n') {
		move(0, 3);
		clrtoeol();
		attron(COLOR_PAIR(2));
		refresh();
		curs_set(1); echo();
		scanw("%lf", data->trace);
		curs_set(0); noecho();
	}
}

void defaultKeyHandler(int key, khdata *data)
{
	/* Default keyboard handling function. */
	
	viewwin *view = data->view;
	double xshift = 0, yshift = 0;
	
	switch (key) {
		case 'k': case KEY_UP:    yshift = 1; break;
		case 'j': case KEY_DOWN:  yshift = -1; break;
		case 'h': case KEY_LEFT:  xshift = -1; break;
		case 'l': case KEY_RIGHT: xshift = 1; break;
	}
	
	xshift *= (view->xmax - view->xmin) / 16;
	yshift *= (view->ymax - view->ymin) / 16;
	
	view->xmin += xshift; view->xmax += xshift;
	view->ymin += yshift; view->ymax += yshift;

	if (key == '-') {
		zoomOut(view, 1.03f);
	}

	if (key == '=') {
		zoomOut(view, 1.0f/1.03f); /* actually zooms in, because 1/factor */
	}

	if (key == 's') enableSlopeChars = !enableSlopeChars;

	if (key == 't') {
		handleKey = traceKeyHandler;
		*data->mode = MODE_TRACE;
	}

	if (key == 'w') editViewWindow(view);

	if (key == 'r') {
		view->xmin = XMIN; view->xmax = XMAX;
		view->ymin = YMIN; view->ymax = YMAX;
		view->xscl = XSCL; view->yscl = YSCL;
	}
}

void drawTrace(WINDOW *win, viewwin *view, yfunction yfunc, double x)
{
	/* Draws the trace cursor on the screen.
	   win   - ncurses window for drawing
	   view  - view parameters structure
	   yfunc - function to call to determine y coordinate
	   x     - graph coordinate for x position
	*/
	double y = yfunc(x);
	int yp, xp;
	attron(COLOR_PAIR(2));
	plotPoint(win, view, x, y, ACS_PLUS, &yp, &xp);
	mvwaddch(win, yp-1, xp, ACS_VLINE); mvwaddch(win, yp+1, xp, ACS_VLINE);
	mvwaddch(win, yp, xp-2, ACS_HLINE); mvwaddch(win, yp, xp-1, ACS_HLINE);
	mvwaddch(win, yp, xp+1, ACS_HLINE); mvwaddch(win, yp, xp+2, ACS_HLINE);
	mvwprintw(win, 0, 0, "X: %.5lf", x);
	mvwprintw(win, 1, 0, "Y: %.5lf", y);
	attroff(COLOR_PAIR(2));
}

int main(int argc, char *argv[])
{
	viewwin view;
	int key = 0;
	yfunction yfunc = defaultFunction;

	view.xmin = XMIN; view.xmax = XMAX;
	view.ymin = YMIN; view.ymax = YMAX;
	view.xscl = XSCL; view.yscl = YSCL;

#ifndef NOLIBMATHEVAL
	if (argc > 1) {
		if (strncmp(argv[1], "y=", 2) == 0) argv[1] += 2;
		eval = evaluator_create(argv[1]);
		
		if (!eval) {
			fprintf(stderr, "Error in expression!\n");
			return 1;
		}
		
		yfunc = performEval;
	}
#endif

	/* ncurses initialization */
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);

	while (key != (int)'q') {	
		static oper_mode mode = MODE_GRAPH;
		static double trace = 0.0;
		erase();
	
		/* perform drawing */
		attron(COLOR_PAIR(1));
		drawAxes(stdscr, &view);
		attroff(COLOR_PAIR(1));
		drawGraph(stdscr, &view, yfunc, enableSlopeChars);
		if (mode == MODE_TRACE) drawTrace(stdscr, &view, yfunc, trace);
		refresh();

		/* wait for a key and call keyboard handler */
		khdata khd;
		khd.view = &view;
		khd.mode = &mode;
		khd.trace = &trace;
		key = getch(); handleKey(key, &khd);
	}

	endwin();

#ifndef NOLIBMATHEVAL
	if (eval) evaluator_destroy(eval);
#endif

	return 0;
}
