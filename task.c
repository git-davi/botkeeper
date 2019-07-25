#include <ptask.h>

#include "task.h"
#include "util.h"


BITMAP *bground_b; 
BITMAP *portiere_b;
BITMAP *palla_b;
BITMAP *porta_b;
BITMAP *indicatore_b;
BITMAP *barra_b;
BITMAP *freccia_b;

palla_t palla;
portiere_t portiere;
campo_t campo;
freccia_t freccia;

int shot = 0;

int low_x, up_x, low_y, up_y;
int user_id;


void task_portiere(void){
	// aggiorna la sua posizione in base alla 
	// traiettoria della palla
	int tmp_dir;
	int next_pos_palla;	// calcola la posizione futura della palla
						// per anticiparla 

	while(1){
		pthread_mutex_lock(&portiere.m);
		// trovo la direzione del portiere 
		// -> verrà usata come vettore velocità (scalandola)
		next_pos_palla = portiere.pos.x;

		pthread_mutex_lock(&palla.m);
		if(palla.v.y != 0)
			next_pos_palla = (int)(	(double)palla.pos.x + 
									(double)palla.v.x/BALL_SCALING_FACTOR * 									POR_ANTICIPATION_MS);
		else if(palla.v.x != 0)
			next_pos_palla = (int)(	(double)palla.pos.x + 
									(double)palla.v.x/BALL_SCALING_FACTOR * 									POR_ANTICIPATION_MS);

		pthread_mutex_unlock(&palla.m);
		
		tmp_dir = next_pos_palla - portiere.pos.x;

		// porto a norma 1 la direzione in x, voglio che si muova 
		// alla stessa velocità a seconda della posizione della palla
		if(tmp_dir != 0)
			tmp_dir = tmp_dir/abs(tmp_dir);

		// calcolo nuova posizione del portiere
		portiere.pos.x = (int)((double)portiere.pos.x + 
								(double)tmp_dir/POR_SCALING_FACTOR * T);

		// movimento solo davanti alla porta
		if(portiere.pos.x >= portiere.border.up)
			portiere.pos.x = portiere.border.up;
		if(portiere.pos.x <= portiere.border.low)
			portiere.pos.x = portiere.border.low;	
		pthread_mutex_unlock(&portiere.m);

		ptask_wait_for_period();
	}
	
}

void task_palla(void){
	double ratio;
	// aggiorno posizione della palla
	/*int i;
	i = ptask_get_index();*/
	while(1){
		pthread_mutex_lock(&palla.m);
		palla.pos.x = (int)((double)palla.pos.x + 
							(double)palla.v.x/BALL_SCALING_FACTOR * T);
		palla.pos.y = (int)((double)palla.pos.y + 
							(double)-palla.v.y/BALL_SCALING_FACTOR * T);
		// NBBBB ----> 	siccome allegro ha l'asse y invertito
		//				per far salire la palla devo assegnare
		//				un valore negativo alla componente della y
		//				affinchè il vettore abbia senso

		// attrito rallenta la palla
		/*
		questa versione decresce esponenzialmente FRICTION_FACTOR^N
		palla.v.x = (int)((double)palla.v.x * FRICTION_FACTOR);
		palla.v.y = (int)((double)palla.v.y * FRICTION_FACTOR);
		*/
		if(palla.v.x == 0)
			ratio = (double)abs(palla.v.y) / 1;	
		else
			ratio = (double)abs(palla.v.y) / (double)abs(palla.v.x);
		if(palla.v.x != 0)
			palla.v.x = sign(palla.v.x) * (abs(palla.v.x) - FRICTION_FACTOR); 
		if(palla.v.y != 0)
			palla.v.y = sign(palla.v.y) * (abs(palla.v.y) - 
													FRICTION_FACTOR * ratio);	
		

		if(palla.v.x == 0 && palla.v.y == 0)
			ptask_activate(user_id);

		pthread_mutex_unlock(&palla.m);

		ptask_wait_for_period();
	}
	
}


void task_grafico(void){
	int palla_x;
	int palla_y;
	int portiere_x;
    int portiere_y;
	int barra_x, barra_y;
	int indicatore_x, indicatore_y;
	int freccia_x, freccia_y;
	
	/*int i;
	i = ptask_get_index();*/
	while(1){	
		leggi_coordinate(&palla_x, &palla_y, 
						&portiere_x, &portiere_y,
						&barra_x, &barra_y,
						&indicatore_x, &indicatore_y,
						&freccia_x, &freccia_y);

		//disegno campo,portiere,palla
		blit(bground_b, screen, 0, 0, 
				campo.border_x.low, campo.border_y.low,
				SCREEN_W, SCREEN_H);
		draw_sprite(screen, portiere_b, portiere_x, portiere_y);
		draw_sprite(screen, palla_b, palla_x, palla_y);


		if(palla.v.x == 0 && palla.v.y == 0) {
			draw_sprite(screen, barra_b, barra_x, barra_y);
			draw_sprite(screen, indicatore_b, indicatore_x, 
												indicatore_y);
			line(screen, palla_x + palla_b->w/2, palla_y + palla_b->h/2, 
					palla_x + freccia_x * 10 , 
					palla_y - freccia_y * 10, 15);
		}

		pthread_mutex_lock(&palla.m);
		pthread_mutex_lock(&portiere.m);
		collisione_bordi();
		collisione_portiere();
		pthread_mutex_unlock(&portiere.m);
		pthread_mutex_unlock(&palla.m);

		ptask_wait_for_period();
	}
	
}


void task_user(void){
	double tmp_v_x, tmp_v_y;
	int pot;

	// potenza tiro massima lungo ogni componente è 300
	ptask_wait_for_activation();
	while(1){		

		while(!key[KEY_SPACE]){ }	
		
		pthread_mutex_lock(&freccia.m);
		tmp_v_x = freccia.x;
		tmp_v_y = freccia.y;
		freccia.dir_chosen = 1;
		pthread_mutex_unlock(&freccia.m);
	
		key[KEY_SPACE] = 0;
		while(!key[KEY_SPACE]){ }
		
		pthread_mutex_lock(&freccia.m);
		pot = freccia.pot / POWER_SCALER;
		pthread_mutex_unlock(&freccia.m);

		set_velocita_palla((int)tmp_v_x*pot, (int)tmp_v_y*pot);

		ptask_wait_for_activation();
	}

}

void task_freccia(void){
	double adder = SPEED_ARROW;
	int raggio = 10;
	
	while(1){
		
		pthread_mutex_lock(&freccia.m);
		if(freccia.dir_chosen == 0) {
			if(freccia.x <= -raggio) 
				adder = SPEED_ARROW;
			if(freccia.x >= raggio) 
				adder = -SPEED_ARROW;
			freccia.x += adder;
			freccia.y = sqrt(raggio * raggio - (freccia.x * freccia.x));
		}
		pthread_mutex_unlock(&freccia.m);
		
		ptask_wait_for_period();

	}
		
}


void task_potenza(void){
	int adder = 1;

	while(1){

		pthread_mutex_lock(&freccia.m);
		if(freccia.pot >= barra_b->h - 45)
			adder = -SPEED_INDICATOR;
		if(freccia.pot <= 0)
			adder = SPEED_INDICATOR;
		freccia.pot += adder;
		freccia.indicatore.y -= adder;
		pthread_mutex_unlock(&freccia.m);
	
		ptask_wait_for_period();

	}

}

void init_tasks(void){
	ptask_init(SCHED_OTHER, GLOBAL, NO_PROTOCOL);
	user_id = ptask_create_prio(task_user, T, PRIO, NOW);
	ptask_create_edf(task_palla, T, C, T, NOW);
	ptask_create_edf(task_portiere, T, C, T, NOW);
	ptask_create_edf(task_grafico, T, C, T, NOW);
	ptask_create_edf(task_potenza, T, C, T, NOW);
	ptask_create_edf(task_freccia, T, C, T, NOW);
}
