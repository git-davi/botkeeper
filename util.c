// file per le funzioni ausialiarie
#include <math.h>
#include <stdio.h>
#include "util.h"
#include "task.h"

#define N 2

// trovare il centro della bitmap
int center_x(BITMAP *bmp, int x){
	return x + bmp->w/2;
} 

int center_y(BITMAP *bmp, int y){
	return y + bmp->h/2;
}

// passare dal centro al primo punto 
// (usato per disegnare la bitmap)
int decenter_x(BITMAP *bmp, int x){
	return x - bmp->w/2;
} 

int decenter_y(BITMAP *bmp, int y){
	return y - bmp->h/2;
}



vector_t reflect_x(vector_t v){
	v.y = - v.y;
	return v;
}

vector_t reflect_y(vector_t v){
	v.x = - v.x;
	return v;
}

double dist_punto_retta(vector_t p, retta_t r){
	double num, den;
	
	num = abs(r.a * p.x + r.b * p.y + r.c);
	den = sqrt(pow(r.a, 2) + pow(r.b, 2));

	return num/den;
}

retta_t retta_due_punti(vector_t p1, vector_t p2){
	retta_t r;

	r.a = p2.y - p1.y;
	r.b = -(p2.x - p1.x);
	r.c = (- p1.x) * (p2.y - p1.y) + p1.y * (p2.x - p1.x);

	return r;
}


int sign(int x){
	return (x > 0) - (x < 0);
}

void set_velocita_palla(int x, int y){

	// perchè il ratio diventerebbe nan
	if(x == 0) x = 1;	

	pthread_mutex_lock(&palla.m);	
	palla.v.x = x;
	palla.v.y = y;
	pthread_mutex_unlock(&palla.m);

}


void collisione_bordi(void){
	// variabili booleane che rilevano una collisione
	int outl;
	int outr;
	int outt;
	int outb;

	// margine della pallina, uguale per ogni asse tanto è quadrata
	int margin;
	
	margin = palla_b->w/2 + 10;
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
						int *portiere_x, int *portiere_y,
						int *barra_x, int *barra_y,
						int *indicatore_x, int *indicatore_y,
						int *freccia_x, int *freccia_y){

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

	//ottengo coordinate della freccia e barra
	pthread_mutex_lock(&freccia.m);
	*freccia_x = (int)freccia.x;
	*freccia_y = (int)freccia.y;
	pthread_mutex_unlock(&freccia.m);

	pthread_mutex_lock(&potenza.m);
	*barra_x = potenza.barra.x;
	*barra_y = potenza.barra.y;
	*indicatore_x = potenza.indicatore.x;
	*indicatore_y = potenza.indicatore.y;
	pthread_mutex_unlock(&potenza.m);

	//decentro palla e portiere
	*palla_x = decenter_x(palla_b, *palla_x);
	*palla_y = decenter_y(palla_b, *palla_y);
	*portiere_x = decenter_x(portiere_b, *portiere_x);
	*portiere_y = decenter_y(portiere_b, *portiere_y);
}


