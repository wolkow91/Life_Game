#include <GL/glut.h>
#include <time.h>

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


void init_my_fields(void)
{	
	for (int i = 0; i < x_val-1; i++)
	{
		//инициализация указателей на соседей

		for (int j = 0; j < y_val-1; j++)
		{
			
			array_field[i][j].left_high.x=i*step_x;
			array_field[i][j].left_high.y=j*step_y;

			array_field[i][j].right_high.x=i*step_x;
			array_field[i][j].right_high.y=(j+1)*step_y;

			array_field[i][j].left_low.x=(i+1)*step_x;
			array_field[i][j].left_low.y=j*step_y;

			array_field[i][j].right_low.x=(i+1)*step_x;
			array_field[i][j].right_low.y=(j+1)*step_y;

			if (i==0) //верxние для первой строки
			{
				array_field[i][j].friends[1] = &array_field[x_val-2][j];
			}
			else{  //для всех остальных
				array_field[i][j].friends[1] = &array_field[i-1][j];
			}	

			if (i==x_val-2) //нижние для нижней строки
			{
				array_field[i][j].friends[6] = &array_field[1][j];
			}
			else{  //для всех остальных
				array_field[i][j].friends[6] = &array_field[i+1][j];
			}

			if (j==0) // левые для первого столбца
			{
				array_field[i][j].friends[3] = &array_field[i][y_val-2];
			}
			else{  //для всех остальных
				array_field[i][j].friends[3] = &array_field[i][j-1];
			}

			if (j==y_val-2) //правые для крайнего справа
			{
				array_field[i][j].friends[4] = &array_field[i][0];
			}
			else{  //для всех остальных
				array_field[i][j].friends[4] = &array_field[i][j+1];
			}	

			if (i==0 && j==0) //верхний левый для а11
			{
				array_field[i][j].friends[0] = &array_field[x_val-2][y_val-2];
			}
			else{
				if (i==0){ 		// верхняя строка
					array_field[i][j].friends[0] = &array_field[x_val-2][j-1]; 
				}else{
					if (j==0) //первый столбец
					{
						array_field[i][j].friends[0] = &array_field[i-1][y_val-2];
					}else{		//остальные строки
						array_field[i][j].friends[0] = &array_field[i-1][j-1];
					}	
				}
			}

			if (i==x_val-2 && j==0) //нижний левый для аn1
			{
				array_field[i][j].friends[5] = &array_field[0][y_val-2];
			}
			else{
				if (i==x_val-2){ 		// нижняя строка
					array_field[i][j].friends[5] = &array_field[0][j-1]; 
				}else{
					if (j==0) //первый столбец
					{
						array_field[i][j].friends[5] = &array_field[i+1][y_val-2];
					}else{		//остальные строки
						array_field[i][j].friends[5] = &array_field[i+1][j-1];
					}
				}
			}

			if (i==x_val-2 && j==y_val-2) //нижний правый для аnm
			{
				array_field[i][j].friends[7] = &array_field[0][0];
			}
			else{
				if (i==x_val-2){ 		// нижняя строка
					array_field[i][j].friends[7] = &array_field[0][j+1]; 
				}else{
					if (j==y_val-2) //последний столбец
					{
						array_field[i][j].friends[7] = &array_field[i+1][0];
					}else{		//остальные строки
						array_field[i][j].friends[7] = &array_field[i+1][j+1];
					}
				}
			}

			if (i==0 && j==y_val-2) //верхний правый для а1m
			{
				array_field[i][j].friends[2] = &array_field[x_val-2][0];
			}
			else{
				if (i==0){ 		// верхняя строка
					array_field[i][j].friends[2] = &array_field[x_val-2][j+1]; 
				}else{
					if (j==y_val-2) //последний столбец
					{
						array_field[i][j].friends[2] = &array_field[i-1][0];
					}else{		//остальные строки
						array_field[i][j].friends[2] = &array_field[i-1][j+1];
					}
				}
			}
		//конец инициализации указателей на соседей	
		}
	}
	//начальные условия тут
	for (int i = 0; i < x_val-2; i++)
	{
		for (int j = 0; j < y_val-2; j++)
		{
		//	if (j==0 || j== 13)
		//	{
		//		if(i!=1 || i!=15){
		//			array_field[i][j].white = true;
		//		}else{
		//			array_field[i][j].white = false;
		//		}
		//	}
		//	else{
					array_field[i][j].white = false;
		//		}
		}
	}
}

void my_draw(void){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	double white_now;

	glBegin(GL_QUADS);
		
		for (int i = 0; i < x_val-1; i++)
		{

			for (int j = 0; j < y_val-1; j++)
			{
				if(array_field[i][j].white)
				{
					white_now = 0.0;
				}else{
					white_now = 1.0;
				}
				
				if (array_field[i][j].white && mode_bw)
				{
					glColor3f(array_field[i][j].rgb.R, array_field[i][j].rgb.G, array_field[i][j].rgb.B);
				}else{
					glColor3f(white_now, white_now, white_now);
				}

				glVertex2d(array_field[i][j].left_high.x, array_field[i][j].left_high.y);
				glVertex2d(array_field[i][j].left_low.x, array_field[i][j].left_low.y);
				glVertex2d(array_field[i][j].right_low.x, array_field[i][j].right_low.y);
				glVertex2d(array_field[i][j].right_high.x, array_field[i][j].right_high.y);
			}
		}

	glEnd();

	//сетка
	glBegin(GL_LINES);

		glColor3f(0.0, 0.0, 0.0);

		for (int i = 0; i < x_val-1; i++)
		{
			glVertex2d(array_field[i][0].left_high.x, array_field[i][0].left_high.y);
			glVertex2d(array_field[i][y_val-2].right_high.x, array_field[i][y_val-2].right_high.y);
		}

		for (int j = 0; j < y_val-1; j++)
		{
			glVertex2d(array_field[0][j].left_high.x, array_field[0][j].left_high.y);
			glVertex2d(array_field[x_val-2][j].left_low.x, array_field[x_val-2][j].left_low.y);
		}
	glEnd();	

}

//преобразование массива полей
void transform(void){

	int alive;
	//наполнение временного массива
	for (int i = 0; i < x_val-1; i++)
	{
		for (int j = 0; j < y_val-1; j++)
		{
			alive = 0;

			for (int ka = 0; ka < 8; ka++)
			{
				if( (*array_field[i][j].friends[ka]).white){
					alive += 1;
				}
			}	

			if(!(array_field[i][j].white)){  //если поле неживое
				if (alive == 3)  // если рядом ровно 3 живые клетки
				{
					time_array_field[i][j] = true; // она оживает
					time_array_color[i][j].R = 0.0;
					time_array_color[i][j].G = 1.0;
					time_array_color[i][j].B = 0.0;
  				}else{
					time_array_field[i][j] = false; // иначе нет
				}
			}else{  //если живое
				if (alive == 2 || alive == 3) 
				{
					time_array_field[i][j] = true; //продолжает жить
					time_array_color[i][j].G = 0.0;
					time_array_color[i][j].B = 1.0;
				}else{
					time_array_field[i][j] = false; // иначе нет
				}
			}
		}
	}

	//изменение массива array_field
	for (int i = 0; i < x_val-1; i++)
	{
		for (int j = 0; j < y_val-1; j++)
		{
			array_field[i][j].white = time_array_field[i][j];
			array_field[i][j].rgb = time_array_color[i][j];
		}
	}	
}

void new_occupant(void){
	int str_ix, col_ix; //координаты нового жителя
	str_ix = rand() % (x_val-1);
	col_ix = rand() % (y_val-1);
	array_field[str_ix][col_ix].white = true;
	array_field[str_ix][col_ix].rgb.R = 1.0;
	array_field[str_ix][col_ix].rgb.G = 0.0;
	array_field[str_ix][col_ix].rgb.B = 0.0;

}

void kill_someone(void){
	int str_ix, col_ix; //координаты жителя
	str_ix = rand() % (x_val-1);
	col_ix = rand() % (y_val-1);
	array_field[str_ix][col_ix].white = false;
}

void kill_alls(void){
	for (int im = 0; im < x_val-1; im++)
	{
		for (int jm = 0; jm < y_val-1; jm++)
		{
			array_field[im][jm].rgb.R = 1.0;
			array_field[im][jm].rgb.G = 1.0;
			array_field[im][jm].rgb.B = 1.0;
			array_field[im][jm].white = false;
		}
	}
}

void click_mouse(bool kill){
	int str_ix, col_ix; //координаты жителя

	str_ix = (int)(mx*(x_val-1)/size_x);
	col_ix = (y_val-2)-(int)(my*(y_val-1)/size_y);

	switch (kill) {
		case true:
			array_field[str_ix][col_ix].white = false;
			array_field[str_ix][col_ix].rgb.R = 0.0;
			array_field[str_ix][col_ix].rgb.B = 0.0;
			array_field[str_ix][col_ix].rgb.G = 0.0;
			break;
		case false:
			array_field[str_ix][col_ix].white = true;
			array_field[str_ix][col_ix].rgb.R = 1.0;
			array_field[str_ix][col_ix].rgb.B = 0.0;
			array_field[str_ix][col_ix].rgb.G = 0.0;
			break;	
	}
}

void processNormalKeys(unsigned char key, int x, int y) {
	switch (key) { 
		case 27: //клавиша ESC
			exit(0);
		case 97: // клавиша a
			stop_key = !stop_key;
			break;
		case 119: //клавиша w
			speed_x++;
			break;
		case 101: //клавиша e
			speed_x--;
			break;	
		case 100: //клавиша d
			new_occupant();
			break;		
		case 102: //клавиша f
			kill_someone();
			break;	
		case 99: //клавиша с
			kill_alls();
			break;
		case 115:
			mode_bw = !mode_bw;
			break;		
		}	
}

void processStopKeys(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_F1:
				stop_key = false;
				break;
		case GLUT_KEY_F2:
				stop_key = true;
				break;
		}
}

void Mouse(int button, int state, int x, int y){ //обработка щелчков мыши
	if(button == GLUT_LEFT_BUTTON){
		switch (state) {
			case GLUT_DOWN: //если нажата
				mx=x; //запомнить координаты
				my=y;
				click_mouse(false);
				break;
		}
	}
	if(button == GLUT_RIGHT_BUTTON){
		switch (state) {
			case GLUT_DOWN: //если нажата
				mx=x;
				my=y;
				click_mouse(true);
				break;
		}
	}
}

void display(void) {
	my_draw();
	glutSwapBuffers();
}

void Timer(int value){

	if(!stop_key){
		transform();
	}	
	glutPostRedisplay();
	glutTimerFunc(100+100*speed_x,Timer,1);
}



void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv){
	stop_key = true;
	speed_x = 0;
	init_my_fields();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(size_x, size_y);
	glutInitWindowPosition(100,100);
	glutCreateWindow("LIFE_1.1");
	init();
	glutDisplayFunc(display);
	glutSpecialFunc(processStopKeys);
	glutKeyboardFunc(processNormalKeys);
	glutTimerFunc(100+100*speed_x, Timer, 1);
	glutMouseFunc(Mouse);

	glutMainLoop();

	return 0;
}
