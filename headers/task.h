#ifdef __TASK_H__
#define __TASK_H__

#define T 10

typedef struct portiere_t {
	int x, y;	// posizione del portiere
	float v;	// velocita' del portiere
	pthread_mutex_t m;
} portiere_t;

typedef struct vector_t {
	int x, y;
} vector_t;

typedef struct palla_t {
	int x, y;		// posizione della palla
	float v;		// velocità della palla
	vector_t *dir;	// direzione della palla
	pthread_mutex_t m;
	pthread_cond_t ready;
} palla_t;

extern palla_t palla;
extern portiere_t portiere;

// funzione per la preparazione di tutti gli attributi
void init_attr(void);
// funzione per l'avvio di tutti i task
void init_tasks(void);

#endif
