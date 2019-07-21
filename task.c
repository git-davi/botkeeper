#include <ptask.h>

#include "task.h"
#include "util.h"

#define SCALING_FACTOR 10
#define FRICTION_FACTOR 1
#define M 1000000000000000000

#define L1 1	 	//			L1	
#define L2 2		//		---------
#define L3 3		//	L3	|       | L2
#define L4 4		//		|       |
#define LATI 4		//		---------
					//			L4

BITMAP *bground_b; 
BITMAP *portiere_b;
BITMAP *palla_b;
BITMAP *porta_b;

palla_t palla;
portiere_t portiere;
campo_t campo;

int low_x, up_x, low_y, up_y;


void *task_portiere(void *arg){
	// aggiorna la sua posizione in base alla 
	// traiettoria della palla
	int tmp_dir;	

	pthread_mutex_lock(&portiere.m);
	// trovo la direzione del portiere 
	// -> verrà usata come vettore velocità (scalandola)
	pthread_mutex_lock(&palla.m);
	tmp_dir = palla.pos.x - portiere.pos.x;
	pthread_mutex_unlock(&palla.m);

	// calcolo nuova posizione del portiere
	portiere.pos.x = portiere.pos.x + tmp_dir/SCALING_FACTOR * T;
	pthread_mutex_unlock(&portiere.m);

	return NULL;
}

void *task_palla(void *arg){
	// aggiorno posizione della palla
	pthread_mutex_lock(&palla.m);
	palla.pos.x = palla.pos.x + palla.v.x * T;
	palla.pos.y = palla.pos.y + palla.v.y * T;
	// attrito rallenta la palla
/*	
	if(palla.v.x != 0)
		palla.v.x -= FRICTION_FACTOR;
	if(palla.v.y != 0)
		palla.v.y -= FRICTION_FACTOR;
*/
	if(palla.v.x==0 && palla.v.y==0){
		pthread_cond_signal(&palla.ready);
	}

	pthread_mutex_unlock(&palla.m);

	return NULL;
}


void collisione_bordi(void){
	int outl;
	int outr;
	int outt;
	int outb;

	//calcolo le collisioni contro i bordi
	outl = (palla.pos.x <= campo.border_x.low);
	outr = (palla.pos.x >= campo.border_x.up);
	outt = (palla.pos.y <= campo.border_y.low);
	outb = (palla.pos.y >= campo.border_y.up);
	
	if(outl)
		palla.pos.x = campo.border_x.low;
	if(outr)
        palla.pos.x = campo.border_x.up;
	if(outl || outr)
		palla.v.x = -palla.v.x;
	
	if(outt)
        palla.pos.y = campo.border_y.low;
	if(outb)
        palla.pos.y = campo.border_y.up;
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
	int best;
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
	if(col_border == L1){
		palla.pos.y = low_y;
		palla.v = reflect_x(palla.v);
	}
	if(col_border == L4){
		palla.pos.y = up_y;
		palla.v = reflect_x(palla.v);
	}
	
}

void leggi_coordinate(	int *palla_x, int *palla_y, 
						int *portiere_x, int *portiere_y){

	//ottengo le coordinate della palla
	pthread_mutex_lock(&palla.m);
	*palla_x = palla.pos.x;
	*palla_y = palla.pos.y;
	pthread_mutex_unlock(&palla.m);

	//ottengo le coordinate del portiere
	pthread_mutex_lock(&portiere.m);
    *portiere_x = portiere.pos.x;
    *portiere_y = portiere.pos.y;
    pthread_mutex_unlock(&portiere.m);

	//decentro palla e portiere
	*palla_x = decenter_x(palla_b, *palla_x);
	*palla_y = decenter_y(palla_b, *palla_y);
	*portiere_x = decenter_x(portiere_b, *portiere_x);
	*portiere_y = decenter_y(portiere_b, *portiere_y);
}

void *task_grafico(void *arg){
	int palla_x;
	int palla_y;
	int portiere_x;
    int portiere_y;
	
	leggi_coordinate(&palla_x, &palla_y, &portiere_x, &portiere_y);

	//disegno campo,portiere,palla
	blit(bground_b, screen, 0, 0, 
			campo.border_x.low, campo.border_y.low,
			SCREEN_W, SCREEN_H);
	draw_sprite(screen, portiere_b, portiere_x, portiere_y);
	draw_sprite(screen, palla_b, palla_x, palla_y);

	collisione_bordi();
	collisione_portiere();
	
	return NULL;
}

void *task_user(void *arg){

	while(1){
		while(palla.v.x!=0 || palla.v.y!=0){
			pthread_cond_wait(&palla.ready, &palla.m);
		}
		//init freccia e potenza
		while(!key[KEY_SPACE]){
			//aggiornamento rotazione freccia
		}	
		//leggi rotazione e assegna a palla
		while(!key[KEY_SPACE]){
			//aggiornamento potenza
		}
		//leggi potenza e assegna a palla

	}
	
	return NULL;
}

void init_tasks(void){

}

