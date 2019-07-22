#include <ptask.h>

#include "task.h"
#include "util.h"


BITMAP *bground_b; 
BITMAP *portiere_b;
BITMAP *palla_b;
BITMAP *porta_b;

palla_t palla;
portiere_t portiere;
campo_t campo;

int low_x, up_x, low_y, up_y;
int user_id;

void task_portiere(void){
	// aggiorna la sua posizione in base alla 
	// traiettoria della palla
	int tmp_dir;
	int next_pos_palla;	// calcola la posizione futura della palla
						// per anticiparla 
 	
	/*int i;
	i = ptask_get_index();*/

	while(1){
		pthread_mutex_lock(&portiere.m);
		// trovo la direzione del portiere 
		// -> verrà usata come vettore velocità (scalandola)
		pthread_mutex_lock(&palla.m);
		next_pos_palla = (int)(	(double)palla.pos.x + 
								(double)palla.v.x/BALL_SCALING_FACTOR * 								POR_ANTICIPATION_MS);
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

		int ratio = abs(palla.v.y) / abs(palla.v.x);
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


void collisione_bordi(void){
	// variabili booleane che rilevano una collisione
	int outl;
	int outr;
	int outt;
	int outb;

	// margine della pallina, uguale per ogni asse tanto è quadrata
	int margin;
	
	margin = palla_b->w/2;
	//calcolo le collisioni contro i bordi
	outl = (palla.pos.x <= campo.border_x.low + margin);
	outr = (palla.pos.x >= campo.border_x.up - margin);
	outt = (palla.pos.y <= campo.border_y.low + margin);
	outb = (palla.pos.y >= campo.border_y.up - margin);
	
	if(outl)
		palla.pos.x = campo.border_x.low + margin;
	if(outr)
        palla.pos.x = campo.border_x.up - margin;
	if(outl || outr)
		palla.v.x = -palla.v.x;
	
	if(outt)
        palla.pos.y = campo.border_y.low + margin;
	if(outb)
        palla.pos.y = campo.border_y.up - margin;
	if(outt || outb)
        palla.v.y = -palla.v.y;
}        


void calcolo_rette(retta_t *r1, retta_t *r2, 
					retta_t *r3, retta_t *r4){

	// dichiaraz delle rette
	//			r1	
	//		---------
	//	r3	|       | r2
	//		|       |
	//		---------
	//			r4


	*r1 = retta_due_punti(	(vector_t){low_x, low_y}, 
							(vector_t){up_x, low_y});

	*r2 = retta_due_punti(	(vector_t){up_x, low_y}, 
							(vector_t){up_x, up_y});

	*r3 = retta_due_punti(	(vector_t){low_x, low_y}, 
							(vector_t){low_x, up_y});

	*r4 = retta_due_punti(	(vector_t){low_x, up_y}, 
							(vector_t){up_x, up_y});
}


// funzione che ritorna l'indice relativo al lato della collisione
int bordo_collisione(retta_t r1, retta_t r2, retta_t r3, retta_t r4){
	int i;
	int best = L4;
	double min = M;
	double d[LATI+1];

	d[1] = dist_punto_retta(palla.pos, r1);
	d[2] = dist_punto_retta(palla.pos, r2);
	d[3] = dist_punto_retta(palla.pos, r3);
	d[4] = dist_punto_retta(palla.pos, r4);
	
	for(i = 1 ; i < LATI+1; i++){
		if(d[i] < min){
			min = d[i];
			best = i;
		}
	}

	return best;
}


void collisione_portiere(void){
	int coll_x, coll_y;
	int col_border;
	retta_t r1, r2, r3, r4;

	// init dei bordi
	low_x = portiere.pos.x - portiere_b->w/2;
	up_x = portiere.pos.x + portiere_b->w/2;
	low_y = portiere.pos.y - portiere_b->h/2;
	up_y = portiere.pos.y + portiere_b->h/2;

	// è in collisione se la palla è compresa
	coll_x = (palla.pos.x <= up_x && palla.pos.x >= low_x);
	coll_y = (palla.pos.y <= up_y && palla.pos.y >= low_y);

	if(!(coll_x && coll_y)) return;

	// trovo le rette passanti per i lati della bitmap portiere
	calcolo_rette(&r1, &r2, &r3, &r4);
	
	// ho il bordo con cui è venuta la collisione
	col_border = bordo_collisione(r1, r2, r3, r4);

	// infine faccio controllo
	// rifletto lungo asse y
	if(col_border == L2){
		palla.pos.x = up_x;
		palla.v = reflect_y(palla.v);
	}
	if(col_border == L3){
		palla.pos.x = low_x;
		palla.v = reflect_y(palla.v);
	}

	// rifletto lungo asse x
	// NB --->	non controllo la collisione con il lato L1
	//			perchè significa che ho fatto goal, la pallina 
	// 			non deve rimbalzare quindi dietro al portiere
	//			semplicemente rimbalzerà contro il bordo e il
	//			portiere la butterà fuori dala porta
	/*if(col_border == L1){
		palla.pos.y = low_y;
		palla.v = reflect_x(palla.v);
	}*/
	if(col_border == L4){
		palla.pos.y = up_y;
		palla.v = reflect_x(palla.v);
	}
	
}

void leggi_coordinate(	int *palla_x, int *palla_y, 
						int *portiere_x, int *portiere_y){

	//ottengo le coordinate della palla
	*palla_x = palla.pos.x;
	*palla_y = palla.pos.y;

	//ottengo le coordinate del portiere
    *portiere_x = portiere.pos.x;
    *portiere_y = portiere.pos.y;

	//decentro palla e portiere
	*palla_x = decenter_x(palla_b, *palla_x);
	*palla_y = decenter_y(palla_b, *palla_y);
	*portiere_x = decenter_x(portiere_b, *portiere_x);
	*portiere_y = decenter_y(portiere_b, *portiere_y);
}

void task_grafico(void){
	int palla_x;
	int palla_y;
	int portiere_x;
    int portiere_y;
	
	/*int i;
	i = ptask_get_index();*/
	while(1){	
		leggi_coordinate(&palla_x, &palla_y, &portiere_x, &portiere_y);

		//disegno campo,portiere,palla
		blit(bground_b, screen, 0, 0, 
				campo.border_x.low, campo.border_y.low,
				SCREEN_W, SCREEN_H);
		draw_sprite(screen, portiere_b, portiere_x, portiere_y);
		draw_sprite(screen, palla_b, palla_x, palla_y);

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

	/*int i;
	i = ptask_get_index();*/
	ptask_wait_for_activation();
	while(1){
		//init freccia e potenza
		while(!key[KEY_SPACE]){
			//aggiornamento rotazione freccia
		}	
		//leggi rotazione e assegna a palla
		while(!key[KEY_SPACE]){
			//aggiornamento potenza
		}
		//leggi potenza e assegna a palla
		ptask_wait_for_activation();
	}

}

void init_tasks(void){
	ptask_init(SCHED_OTHER, GLOBAL, NO_PROTOCOL);
	user_id = ptask_create_prio(task_user, T, PRIO, NOW);
	ptask_create_edf(task_palla, T, C, T, NOW);
	ptask_create_edf(task_portiere, T, C, T, NOW);
	ptask_create_edf(task_grafico, T, C, T, NOW);
}
