int chance = 1000; //шанс мутации
bool stohastic_key;

void stohastic(void){
	for (int i = 0; i < x_val-1; i++)
	{
		for (int j = 0; j < y_val-1; j++)
		{
			if(rand()%chance == 0){
				array_field[i][j].white = !array_field[i][j].white;
				array_field[i][j].rgb.R = 1.0;
				array_field[i][j].rgb.B = 0.0;
				array_field[i][j].rgb.G = 0.0;
			}
 		}
	}	
}
