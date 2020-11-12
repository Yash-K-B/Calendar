
#pragma once

#ifndef SOURCE_H
#define SOURCE_H


int getDaysOfMonth(int m, int year);

int is_leap_year(int year);

int get_day(int date, int month, int year);

void refresh(int next_month, float date_base_x, float date_base_y, float x_scale, float y_scale);


#endif // !SOURCE_H

