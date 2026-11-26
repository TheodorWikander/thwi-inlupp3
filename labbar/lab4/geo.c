#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../utils.h"

struct point {
  int x;
  int y;
};

typedef struct point point_t;

struct rectangle {
    point_t upper_l;
    point_t lower_r;
};

typedef struct rectangle rectangle_t;

void translate(point_t *p1, point_t *p2)
{
  p1->x += p2->x;
  p1->y += p2->y;
}

int max(int a, int b) { return (a > b) ? a : b; }
int min(int a, int b) { return (a < b) ? a : b; }

void print_point(point_t *point) {
    printf("(%d, %d)", point->x, point->y);
}

point_t make_point(int x, int y) {
    point_t p = {.x = x, .y = y};
    return p;
}

void print_rect(rectangle_t *rect) {
    printf("rectangle(upper_left=");
    print_point(&rect->upper_l);
    printf("lower_right=");
    print_point(&rect->lower_r);
    printf(")\n");
}

rectangle_t make_rect(int x1, int y1, int x2, int y2) {
    point_t p1 = make_point(x1, y1);
    point_t p2 = make_point(x2, y2);
    rectangle_t rect = {.upper_l = p1, .lower_r = p2};
    return rect;
}

int area_rect(rectangle_t *rect) {
    int width = abs(rect->lower_r.x - rect->upper_l.x);
    int height = abs(rect->upper_l.y - rect->lower_r.y);
    int area = (width*height);
    return area;
}

bool intersect_rect(rectangle_t *rect1, rectangle_t *rect2) {

    //Ligger rect1 helt till vänster om rect2? Ingen skärning returnera false
    if (rect1->lower_r.x < rect2->upper_l.x) return false;
    // Ligger rect2 helt till vänster om rect1? Ingen skärning isåfall
    if (rect2->lower_r.x < rect1->upper_l.x) return false;
    //Ligger rect1 helt under rect2?
    if (rect1->upper_l.y < rect2->lower_r.y) return false;
    //Ligger rect2 helt under rect1?
    if (rect2->upper_l.y < rect1->lower_r.y) return false;

    return true;
}

rectangle_t intersection_rect(rectangle_t *r1, rectangle_t *r2) {
    if (!intersect_rect(r1, r2)) {
        return make_rect(0, 0, 0, 0);
    }

    int left = min(r1->upper_l.x, r2->upper_l.x);
    int right = min(r1->lower_r.x, r2->lower_r.x);
    int top = min(r1->upper_l.y, r2->upper_l.y);
    int bottom = max(r1->lower_r.y, r2->lower_r.y);

    return make_rect(left, top, right, bottom);
}

//dålig main men den funkar
int main(void) {
    rectangle_t rect = make_rect (1,3,5,7);
    int area = (area_rect(&rect));
    printf("%d\n", area);

    printf("Test Intersect\n");
    rectangle_t r1 = make_rect(0, 10, 10, 0);
    rectangle_t r2 = make_rect(5, 5, 15, -5);
    rectangle_t r3 = make_rect(20, 20, 30, 10);

    if (intersect_rect(&r1, &r2) && !intersect_rect(&r1, &r3)) {
        printf("Koden funkar!\n");
    } else {
        printf("Funkar inte :(\n");
    }

    rectangle_t interrect = intersection_rect(&r1, &r2);
    print_rect(&interrect);

    return 0;
}