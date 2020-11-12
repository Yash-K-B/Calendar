
#include <iostream>
#include <ctime>
#include <array>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "Source.h"

using namespace std;
using namespace sf;

array<string,7> days = { "SUN","MON","TUE","WED","THU","FRI","SAT" };
array<string,7> days_full = { "SUNDAY","MONDAY","TUESDAY","WEDNESDAY","THURSDAY","FRIDAY","SATURDAY" };
array<string,12> months = { "JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE","JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER" };
int days_next = 0;
const int window_height = 720, window_width = 1280;

vector<Text> dates;
RectangleShape today;
Font date_font,desc_font;
int date_font_size = window_height/27;
RectangleShape rect;
Text desc;

int main() {
	
	RenderWindow window(VideoMode(window_width, window_height), "Calender");

	time_t t = time(0);
	tm* now = new tm();
	localtime_s(now, &t);

	Font title_font;
	title_font.loadFromFile("fonts/VastShadow-Regular.ttf");

	string title = "CALENDAR";
	Text title_text(title,title_font,static_cast<int>(date_font_size*2.5f));
	float width = title_text.getGlobalBounds().width;
	title_text.setPosition(window_width / 2 - width / 2, 10);

	float date_base_x = window_width * 0.175f, date_base_y = window_height*0.4f;
	float x_scale = (window_width*0.7f)/7.0f, y_scale = (window_height)/13.5f;
	date_font.loadFromFile("fonts/MiriamLibre-Regular.ttf");
	desc_font.loadFromFile("fonts/ModernAntiqua-Regular.ttf");


	int number_of_days = getDaysOfMonth(now->tm_mon + 1, now->tm_year + 1900);
	int shift = get_day(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900), day_count = 1;
	
	refresh(days_next, date_base_x, date_base_y, x_scale, y_scale);

	Image bg_image;
	bg_image.loadFromFile("images/bg_img.jpg");
	Texture texture;
	texture.loadFromImage(bg_image);
	Sprite sprite;
	sprite.setTexture(texture, true);
	//cout << bg_image.getSize().x<<"\t"<<window_height / texture.getSize().y << endl;
	sprite.scale(window_width * 1.0f / bg_image.getSize().x, window_height * 1.0f / bg_image.getSize().y);


	while (window.isOpen()) {
		Event event;
		if (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Right)
				{
					days_next++;
					refresh(days_next, date_base_x, date_base_y, x_scale, y_scale);

				}

				if (event.key.code == Keyboard::Key::Left)
				{
					days_next--;
					refresh(days_next, date_base_x, date_base_y, x_scale, y_scale);

				}
			}
			
		}

		window.clear();
		window.draw(sprite);
		window.draw(today);
		window.draw(title_text);
		window.draw(rect);
		for (auto& d : dates) window.draw(d);
		window.draw(desc);
		window.display();
	}

	return 0;
}



int getDaysOfMonth(int m, int year)
{
	switch (m) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;

	case 4:
	case 6:
	case 9:
	case 11:
		return 30;

	case 2:
		return 28 + is_leap_year(year);
	default:
		return 0;
	}
}

int is_leap_year(int year) {
	if (year % 100 == 0)
		if (year % 400 == 0) return 1;
		else return 0;
	else if (year % 4 == 0) return 1;
	else return 0;

}


int get_day(int date, int month, int year) {
	int month_code = 6, year_code = 1;
	for (int i = 2013; i <= year; i++)
		year_code = (year_code + is_leap_year(i) + 1) % 7;
	month_code -= is_leap_year(year);
	for (int j = 2; j <= month; j++)
		month_code = (month_code + getDaysOfMonth(j - 1, year)) % 7;
	return (date + month_code + year_code) % 7;
}

string to_camel_case(string s) {
	for (unsigned int i = 0; i <= s.size(); i++) {
		if (i == 0) s[i] = s[i] >= 'a' && s[i] <= 'z' ? s[i] - 32 : s[i];
		else if (i != 0 && s[i - 1] == ' ')
			s[i] = s[i] >= 'a' && s[i] <= 'z' ? s[i] - 32 : s[i];
		else s[i] = s[i] >= 'A' && s[i] <= 'Z' ? s[i] + 32 : s[i];
	}
	return s;
}


void refresh(int next_month,float date_base_x,float date_base_y,float x_scale,float y_scale) {


	time_t t = time(0);
	tm* now = new tm();
	localtime_s(now, &t);
	dates.clear();

	int month = (now->tm_mon + next_month) >= 0?(now->tm_mon + next_month) % 12 + 1: (now->tm_mon + next_month+1)%12+12;
	int year = (now->tm_mon + next_month) >= 0 ? (now->tm_mon + next_month)/12 + now->tm_year+1900: (now->tm_mon + next_month-12) / 12 + now->tm_year + 1900;

	//cout << "year - " << (now->tm_mon + next_month - 12) / 12 << endl;
	//cout << month << " ---- " << year << endl;
	
	for (int i = 0; i < 7; i++) {
		Text t(days[i], date_font, static_cast<int>(date_font_size*1.2f));
		t.setPosition(date_base_x + ((i % 7) * x_scale) - t.getLocalBounds().width * 0.2f, date_base_y *0.7f);
		t.setFillColor(i == 0 ? Color::Red : (i == 6) ? Color::Magenta : Color::Green);
		dates.push_back(t);
	}


	int number_of_days = getDaysOfMonth(month,year);
	int shift = get_day(1, month,year), day_count = 1;

	for (int i = 0; i < number_of_days + shift; i++) {
		if (i < shift) continue;
		Text t(to_string(day_count++), date_font, date_font_size);
		if (i % 7 == 0)t.setFillColor(Color::Red);
		if (i % 7 == 6) t.setFillColor(Color::Magenta);
		t.setPosition(date_base_x + ((i % 7) * x_scale), date_base_y*0.95f + ((i / 7) * y_scale));
		dates.push_back(t);

		if (day_count - 1 == now->tm_mday) {
			if ((now->tm_mon != (now->tm_mon + next_month) % 12 ) || now->tm_year !=year-1900) {
				today.setPosition(-30, -30);
				today.setSize(Vector2f(0,0));
				continue;
			}
			today.setSize(Vector2f(t.getLocalBounds().width * 1.5f, t.getLocalBounds().height * 1.5f));
			today.setOutlineColor(i % 7 == 0 ? Color(255, 0, 0, 255) : Color(255, 255, 255, 255));
			today.setOutlineThickness(2.0f);
			//cout << t.getLocalBounds().width<<" ---- "<< t.getLocalBounds().height << endl;
			today.setPosition(date_base_x + ((i % 7) * x_scale) - (t.getLocalBounds().width * 0.17f), date_base_y*0.95f + ((i / 7) * y_scale) + (t.getLocalBounds().height * 0.1f));
			today.setFillColor(Color(0, 0, 200, 0));
		}
	}
	
	rect.setSize(Vector2f(window_width * 0.76f, (((number_of_days + shift - 1) / 7) + 3) * y_scale));
	rect.setPosition(window_width * 0.12f, window_height*0.24f);
	rect.setFillColor(Color(0, 0, 0, 100));


	desc.setString(months[month-1] + " - " + to_string(year));
	desc.setFont(desc_font);
	desc.setCharacterSize(static_cast<int>(date_font_size*1.25f));

	desc.setPosition((window_width - desc.getGlobalBounds().width)/2.0f, date_base_y+(6)*y_scale);

}
