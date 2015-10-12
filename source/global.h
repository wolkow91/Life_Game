struct vertex
{
	double x;
	double y;
};

struct color
{
	double R;
	double G;
	double B;
};

struct field
{
	vertex left_high;
	vertex right_high;
	vertex left_low;
	vertex right_low;
	field* friends[8]; 
	/* 0 - верхний левый, 1 - верхний, 2 - верхний правый, 
	3 - левый, 4 - правый,
	5 - нижний левый, 6 - нижний, 7 - правый нижний
	*/
	bool white;
	color rgb;
};

//параметры экрана
const int size_x = 800;
const int size_y = 600;
double red, green, blue;
//координаты вершин
const int x_val=81, y_val=61;
const double step_x = 1.0/(x_val-1);
const double step_y = 1.0/(y_val-1);

//прочие глобальные переменные
bool stop_key, mode_bw = true; //стоп-ключ и черно-белый режим
int speed_x = 0;


//глобальный массив структур field - наше игровое поле
field array_field[x_val-1][y_val-1];
//копия массива для промежуточных вычислений
bool time_array_field[x_val-1][y_val-1];
color time_array_color[x_val-1][y_val-1];

int mx, my; //координаты мыши
