#ifndef JEU_H
#define JEU_H

void JEU_DeplacementPacMan(int, char*);
void JEU_DeplacementFantomes(char*);
void JEU_initialisationParametres(char, char, char, char*);
int JEU_ifLevelDone(int);
void JEU_ChangementDeNiveau(void);
void JEU_SetListePACPOINTS(void);

char JEU_GetScore();
char JEU_GetNiveau();
char JEU_GetVies();

void JEU_CreationGrille(int* listeMURS, int *listePACPOINTS, char* grille, size_t tailleListeMURS, size_t tailleListePACPOINTS);

#endif JEU_H
