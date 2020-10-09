#include "TXlib.h"
#include <stdlib.h>
#include <string>
#include <cmath>


struct point {
	double x, y;
};

class CoordSys {
private:
	point coords0_, coords1_;
	double scaleX_, scaleY_;

public:
	void draw_point(point coords);
	void draw_line(point coords0, point coords1);
	void draw_window();
	void draw_grid();
	void draw_axis();
	double* to_pixels(point coords);

	CoordSys(point coords0, point coords1, double scaleX, double scaleY) {
		coords0_ = coords0;
		coords1_ = coords1;
		scaleX_ = scaleX;
		scaleY_ = scaleY;
	}
};

class Vector {
private:
	void normalization();
	double length();

public:
	point coords_;
	void turn_vector(double angle);
	void draw_vector(point start_coords, CoordSys& vector_space);

	Vector(point coords) {
		coords_.x = coords.x;
		coords_.y = coords.y;
	}
};

Vector operator + (Vector a, Vector b) {
	point coords_of_new_vector = { a.coords_.x + b.coords_.x, a.coords_.y + b.coords_.y };
	return Vector(coords_of_new_vector);
}

Vector operator * (double koef_of_length, Vector a) {
	point coords_of_new_vector = { koef_of_length * a.coords_.x, koef_of_length * a.coords_.y };
	return Vector(coords_of_new_vector);
}

double operator * (Vector a, Vector b) {
	return (a.coords_.x * b.coords_.x + a.coords_.y * b.coords_.y);
}

double Vector::length() {
	return sqrt(coords_.x * coords_.x + coords_.y * coords_.y);
}

void Vector::turn_vector(double angle) {
	point new_coords = { 0, 0 };

	new_coords.x = coords_.x * cos(angle) - coords_.y * sin(angle);
	new_coords.y = coords_.x * sin(angle) + coords_.y * cos(angle);

	coords_ = new_coords;
}

void Vector::normalization() {
	Vector temp_vector = *this;
	double length = temp_vector.length();

	temp_vector = 20 / length * temp_vector;
	coords_.x = temp_vector.coords_.x;
	coords_.y = temp_vector.coords_.y;
}

void Vector::draw_vector(point start_coords, CoordSys& vector_space) {
	point end_coords = { coords_.x + start_coords.x, coords_.y + start_coords.y };
		
	vector_space.draw_line(start_coords, end_coords);
	vector_space.draw_point(start_coords);

	Vector vector_for_arrow1({ -coords_.y ,  coords_.x });
	Vector vector_for_arrow2({  coords_.y , -coords_.x });
	Vector reverse = -1 * *this;

	reverse.normalization();
	vector_for_arrow1.normalization();
	vector_for_arrow2.normalization();

	Vector arrow1 = reverse + vector_for_arrow1;
	Vector arrow2 = reverse + vector_for_arrow2;

	arrow1.normalization();
	arrow2.normalization();

	point end_of_arrow1 = { arrow1.coords_.x + end_coords.x, arrow1.coords_.y + end_coords.y };
	vector_space.draw_line(end_coords, end_of_arrow1);

	point end_of_arrow2 = { arrow2.coords_.x + end_coords.x, arrow2.coords_.y + end_coords.y };
	vector_space.draw_line(end_coords, end_of_arrow2);
}

double* CoordSys::to_pixels(point coords) {
	point start_of_coord = { coords1_.x / 2, coords1_.y / 2 };
	double* rec_coord = new double[2];

	rec_coord[0] = coords.x * scaleX_ + start_of_coord.x;
	rec_coord[1] = start_of_coord.y - coords.y * scaleY_;

	return rec_coord;
}

void CoordSys::draw_point(point coords) {
	txSetColor(TX_BLACK);
	txSetFillColor(TX_BLACK);

	double* rec_coord = to_pixels(coords);

	if (rec_coord[1] >= coords0_.y &&
		rec_coord[1] <= coords1_.y &&
		rec_coord[0] <= coords1_.x &&
		rec_coord[0] >= coords0_.x)
		txCircle(rec_coord[0], rec_coord[1], 2);

	delete[] rec_coord;
}

void CoordSys::draw_line(point coords0, point coords1) {
	txSetColor(TX_BLUE);

	double* rec_coord0 = to_pixels(coords0);
	double* rec_coord1 = to_pixels(coords1);

	if (rec_coord0[1] >= coords0_.y &&
		rec_coord0[1] <= coords1_.y &&
		rec_coord0[0] <= coords1_.x &&
		rec_coord0[0] >= coords0_.x &&
		rec_coord1[1] >= coords0_.y &&
		rec_coord1[1] <= coords1_.y && 
		rec_coord1[0] <= coords1_.x &&
		rec_coord1[0] >= coords0_.x) 
		txLine(rec_coord0[0], rec_coord0[1], rec_coord1[0], rec_coord1[1]);

	delete[] rec_coord0;
	delete[] rec_coord1;
}

void CoordSys::draw_window() {
	txRectangle(coords0_.x, coords0_.y, coords1_.x, coords1_.y);
}

void CoordSys::draw_grid() {
	txSetColor(TX_LIGHTGRAY);

	for (int line = coords0_.x; line < coords1_.x; line += 10) txLine(line, coords0_.y, line, coords1_.y);
	for (int line = coords0_.y; line < coords1_.y; line += 10) txLine(coords0_.x, line, coords1_.x, line);
}

void CoordSys::draw_axis() {
	txSetColor(TX_BLACK);

	txLine(coords1_.x / 2, coords0_.y, coords1_.x / 2, coords1_.y);
	txLine(coords0_.x, coords1_.y / 2, coords1_.x, coords1_.y / 2);

	txLine(coords1_.x / 2, coords0_.y, coords1_.x / 2 + 5, coords0_.y + 5);
	txLine(coords1_.x / 2, coords0_.y, coords1_.x / 2 - 5, coords0_.y + 5);

	txLine(coords1_.x - 5, coords1_.y / 2 + 5, coords1_.x, coords1_.y / 2);
	txLine(coords1_.x - 5, coords1_.y / 2 - 5, coords1_.x, coords1_.y / 2);
}

void create_working_space(CoordSys& vector_space) {
	txSetFillColor(TX_WHITE);
	txClear();

	vector_space.draw_window();
	vector_space.draw_grid();
	vector_space.draw_axis();
}

int main() {
	txCreateWindow(900, 600);

	CoordSys vector_space({ 20, 20 }, { 880, 580 }, 1, 1);

	create_working_space(vector_space);
	txSetColor(TX_WHITE);

	Vector a({ -100, -200 });
	Vector b({ -150, -225 });

	a.draw_vector({ 0, 0 }, vector_space);
	b.draw_vector({ -70, 50 }, vector_space);

	a.turn_vector(2.1 * txPI / 2);
	b.turn_vector(txPI / 2);

	a.draw_vector({ 0, 0 }, vector_space);
	b.draw_vector({ -70, 50 }, vector_space);

	point start_of_vector = { 0, 0 };
	while (true) {
		if (txMouseButtons() == 1) {
			while (txMouseButtons() == 1) txSleep(100);

			Vector c({ -440 + txMouseX() - start_of_vector.x, 290 - txMouseY() - start_of_vector.y });

			c.draw_vector(start_of_vector, vector_space);
		}

		if (txMouseButtons() == 2) {
			while (txMouseButtons() == 2) txSleep(100);

			create_working_space(vector_space);
			start_of_vector.x = -440 + txMouseX();
			start_of_vector.y = 290 - txMouseY();

			txSetColor(TX_BLACK);
			txSetFillColor(TX_BLACK);
			txCircle(440 + start_of_vector.x, 290 - start_of_vector.y, 2);
		}
	}
}