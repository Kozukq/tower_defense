#include "interface.h"
#include "fenetre.h"
#include "ncurses.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Définition de la palette.
 */
void palette() {
    
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_RED);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLUE);
}

/**
 * Création d'une interface.
 * @param jeu l'état du jeu
 * @return l'interface créée
 */
struct interface interface_creer(struct game* game) {
    
    int i;
    int j;
    struct interface interface;
    
    interface.current_tool = OUTIL_NONE;

    interface.info = fenetre_creer(INFOS_POSX, INFOS_POSY, INFOS_LARGEUR, INFOS_HAUTEUR, "Informations", TRUE);
    wprintw(interface.info->interieur, "Tapez 'Q' pour quitter");
    wprintw(interface.info->interieur, "\n1, 2, 3 pour retirer des vies aux adversaires");
    wprintw(interface.info->interieur, "\nV pour retirer des vies a l'utilisateur");
    wprintw(interface.info->interieur, "\n$ pour ajouter de l'argent");
    wprintw(interface.info->interieur, "\nF et U pour augmenter freeze et unfreeze");
    wrefresh(interface.info->interieur);


    interface.tools = fenetre_creer(OUTILS_POSX, OUTILS_POSY, OUTILS_LARGEUR, OUTILS_HAUTEUR, "Outils", FALSE);
    interface_MAJOutils(&interface, game);

    interface.status = fenetre_creer(ETAT_POSX, ETAT_POSY, ETAT_LARGEUR, ETAT_HAUTEUR, "Etat", FALSE);
    wprintw(interface.status->interieur, "Vies      %2d\n", game->health);
    wprintw(interface.status->interieur, "Argent %5d\n", game->money);
    wprintw(interface.status->interieur, "Freeze     0\n");
    wprintw(interface.status->interieur, "Unfreeze   0\n");
    wprintw(interface.status->interieur, "Adv. 1    10\n");
    wprintw(interface.status->interieur, "Adv. 2    10\n");
    wprintw(interface.status->interieur, "Adv. 3    10\n");    
    wrefresh(interface.status->interieur);

    interface.attacks = fenetre_creer(ATTAQUES_POSX, ATTAQUES_POSY, ATTAQUES_LARGEUR, ATTAQUES_HAUTEUR, "Attaques", FALSE);
    interface_MAJAttaques(&interface, game);
        
    interface.map = fenetre_creer(CARTE_POSX, CARTE_POSY, CARTE_LARGEUR, CARTE_HAUTEUR, "Carte", FALSE);
    for(i = 0; i < 15; i++) {
        for(j = 0; j < 15; j++) {
            if(game->board[i][j].background == CASE_VIDE) {
                waddch(interface.map->interieur, ' ' | COLOR_PAIR(COULEUR_VIDE));
            }
            else if((game->board[i][j].background >= CASE_MIN_JOUEUR) && (game->board[i][j].background <= CASE_MAX_JOUEUR)) {
                wattron(interface.map->interieur, COLOR_PAIR(COULEUR_JOUEUR));
                wprintw(interface.map->interieur, "%d", game->board[i][j].background);
            }
            else if(game->board[i][j].background == CASE_ORDI) {
                wattron(interface.map->interieur, COLOR_PAIR(COULEUR_ORDI));
                wprintw(interface.map->interieur, "O");
            }
            else if(game->board[i][j].background == CASE_FORT) {
                waddch(interface.map->interieur, 'F' | COLOR_PAIR(COULEUR_FORT));
            }
            else if((game->board[i][j].background >= CASE_MIN_CHEMIN) && (game->board[i][j].background <= CASE_MAX_CHEMIN)) {
                waddch(interface.map->interieur, ' ' | COLOR_PAIR(COULEUR_CHEMIN));
            }
        }
    }
    wrefresh(interface.map->interieur);

    return interface;
}

/**
 * Suppression d'une interface.
 * @param interface l'interface à supprimer
 */
void interface_supprimer(struct interface* interface) {
    
    fenetre_supprimer(&interface->info);
    fenetre_supprimer(&interface->map);
    fenetre_supprimer(&interface->tools);
    fenetre_supprimer(&interface->status);
    fenetre_supprimer(&interface->attacks);
}

/**
 * Met à jour la fenêtre 'Attaques'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJAttaques(struct interface* interface, struct game* game) {
    
    werase(interface->attacks->interieur);

    wattron(interface->attacks->interieur, COLOR_PAIR(1));
    if(game->money < SOLDAT_COUT) {
        wattron(interface->attacks->interieur, COLOR_PAIR(4));
    }
    wprintw(interface->attacks->interieur, "Soldat          %4d$ -> 123\n", SOLDAT_COUT);    
    
    if(game->money < COMMANDO_COUT) {
        wattron(interface->attacks->interieur, COLOR_PAIR(4));
    }
    wprintw(interface->attacks->interieur, "Commando        %4d$ -> 123\n", COMMANDO_COUT);
    
    if(game->money < VEHICULE_COUT) {
        wattron(interface->attacks->interieur, COLOR_PAIR(4));
    }
    wprintw(interface->attacks->interieur, "Vehicule blinde %4d$ -> 123\n", VEHICULE_COUT);
    
    if(game->money < MISSILE_COUT) {
        wattron(interface->attacks->interieur, COLOR_PAIR(4));
    }
    wprintw(interface->attacks->interieur, "Lance-missiles  %4d$ -> 123\n", MISSILE_COUT);

    if(game->money < CHAR_COUT) {
        wattron(interface->attacks->interieur, COLOR_PAIR(4));
    }
    wprintw(interface->attacks->interieur, "Char            %4d$ -> 123\n", CHAR_COUT);

    if(game->freeze == 10) {
        wattron(interface->attacks->interieur, COLOR_PAIR(1));
    }
    else {
        wattron(interface->attacks->interieur, COLOR_PAIR(4));
    }
    wprintw(interface->attacks->interieur, "Freeze                -> 123\n");

    wrefresh(interface->attacks->interieur);
}

/**
 * Met à jour la fenêtre 'Outils'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJOutils(struct interface* interface, struct game* game) {
    
    werase(interface->tools->interieur);

    if(game->money < TOUR_1_COUT) {

        wattron(interface->tools->interieur, COLOR_PAIR(4));
        
        if(interface->current_tool == OUTIL_TOUR_1) {
            interface->current_tool = OUTIL_NONE;
        }
    }
    else if(interface->current_tool == OUTIL_TOUR_1) {
        wattron(interface->tools->interieur, COLOR_PAIR(5));
    }
    else {
        wattron(interface->tools->interieur, COLOR_PAIR(1));
    }
    wprintw(interface->tools->interieur, "Tour 1 %4d$\n", TOUR_1_COUT);

    if(game->money < TOUR_2_COUT) {

        if(interface->current_tool == OUTIL_TOUR_2) {
            interface->current_tool = OUTIL_NONE;
        }
        
        wattron(interface->tools->interieur, COLOR_PAIR(4));
    }
    else if(interface->current_tool == OUTIL_TOUR_2) {
        wattron(interface->tools->interieur, COLOR_PAIR(5));
    }
    else {
        wattron(interface->tools->interieur, COLOR_PAIR(1));
    }
    wprintw(interface->tools->interieur, "Tour 2 %4d$\n", TOUR_2_COUT);

    if(game->money < TOUR_3_COUT) {

        if(interface->current_tool == OUTIL_TOUR_3) {
            interface->current_tool = OUTIL_NONE;
        }
        
        wattron(interface->tools->interieur, COLOR_PAIR(4));
    }
    else if(interface->current_tool == OUTIL_TOUR_3) {
        wattron(interface->tools->interieur, COLOR_PAIR(5));
    }
    else {
        wattron(interface->tools->interieur, COLOR_PAIR(1));
    }
    wprintw(interface->tools->interieur, "Tour 3 %4d$\n", TOUR_3_COUT);

    if(game->money < TOUR_4_COUT) {

        if(interface->current_tool == OUTIL_TOUR_4) {
            interface->current_tool = OUTIL_NONE;
        }
        
        wattron(interface->tools->interieur, COLOR_PAIR(4));
    }
    else if(interface->current_tool == OUTIL_TOUR_4) {
        wattron(interface->tools->interieur, COLOR_PAIR(5));
    }
    else {
        wattron(interface->tools->interieur, COLOR_PAIR(1));
    }
    wprintw(interface->tools->interieur, "Tour 4 %4d$\n", TOUR_4_COUT);

    if(game->money < TOUR_5_COUT) {

        if(interface->current_tool == OUTIL_TOUR_5) {
            interface->current_tool = OUTIL_NONE;
        }

        wattron(interface->tools->interieur, COLOR_PAIR(4));
    }
    else if(interface->current_tool == OUTIL_TOUR_5) {
        wattron(interface->tools->interieur, COLOR_PAIR(5));
    }
    else {
        wattron(interface->tools->interieur, COLOR_PAIR(1));
    }
    wprintw(interface->tools->interieur, "Tour 5 %4d$\n", TOUR_5_COUT);

    if(game->unfreeze < 10) {
        wattron(interface->tools->interieur, COLOR_PAIR(4));
    }
    else {
        wattron(interface->tools->interieur, COLOR_PAIR(1));
    }
    wprintw(interface->tools->interieur, "Unfreeze\n");

    wrefresh(interface->tools->interieur);
}

/**
 * Met à jour la fenêtre 'Etat'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJEtat(struct interface* interface, struct game* game) {

    mvwprintw(interface->status->interieur, 0, 8, "%4d", game->health);

    mvwprintw(interface->status->interieur, 1, 7, "%5d", game->money);
    
    mvwprintw(interface->status->interieur, 2, 8, "%4d", game->freeze);
    
    mvwprintw(interface->status->interieur, 3, 8, "%4d", game->unfreeze);
    
    mvwprintw(interface->status->interieur, 4, 8, "%4d", game->opponents_health[0]);
    
    mvwprintw(interface->status->interieur, 5, 8, "%4d", game->opponents_health[1]);
    
    mvwprintw(interface->status->interieur, 6, 8, "%4d", game->opponents_health[2]);
    
    wrefresh(interface->status->interieur);
}

/**
 * Gestion des actions dans la fenêtre des outils.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre 
 */
void interface_outils(struct interface* interface, struct game* game, int posX, int posY) {
    
    switch(posY) {

        case 0:

            if(game->money >= TOUR_1_COUT) {

                interface->current_tool = OUTIL_TOUR_1;
                interface_MAJOutils(interface, game);
                wprintw(interface->info->interieur, "\nOutil tour 1 selectionne. Cliquez sur un carre d'herbe.");
            }
            else {
                wprintw(interface->info->interieur, "\nPas assez d'argent pour acheter une tour 1.");
            }

            wrefresh(interface->info->interieur);

            break;

        case 1:

            if(game->money >= TOUR_2_COUT) {

                interface->current_tool = OUTIL_TOUR_2;
                interface_MAJOutils(interface, game);
                wprintw(interface->info->interieur, "\nOutil tour 2 selectionne. Cliquez sur un carre d'herbe.");
            }
            else {
                wprintw(interface->info->interieur, "\nPas assez d'argent pour acheter une tour 2.");
            }

            wrefresh(interface->info->interieur);
            
            break;

        case 2:

            if(game->money >= TOUR_3_COUT) {

                interface->current_tool = OUTIL_TOUR_3;
                interface_MAJOutils(interface, game);
                wprintw(interface->info->interieur, "\nOutil tour 3 selectionne. Cliquez sur un carre d'herbe.");
            }
            else {
                wprintw(interface->info->interieur, "\nPas assez d'argent pour acheter une tour 3.");
            }

            wrefresh(interface->info->interieur);
            
            break;

        case 3:

            if(game->money >= TOUR_4_COUT) {

                interface->current_tool = OUTIL_TOUR_4;
                interface_MAJOutils(interface, game);
                wprintw(interface->info->interieur, "\nOutil tour 4 selectionne. Cliquez sur un carre d'herbe.");
            }
            else {
                wprintw(interface->info->interieur, "\nPas assez d'argent pour acheter une tour 4.");
            }

            wrefresh(interface->info->interieur);
            
            break;

        case 4:

            if(game->money >= TOUR_5_COUT) {

                interface->current_tool = OUTIL_TOUR_5;
                interface_MAJOutils(interface, game);                        
                wprintw(interface->info->interieur, "\nOutil tour 5 selectionne. Cliquez sur un carre d'herbe.");
            }
            else {
                wprintw(interface->info->interieur, "\nPas assez d'argent pour acheter une tour 5.");
            }

            wrefresh(interface->info->interieur);
            
            break;

        case 5:

            if(game->unfreeze == 10) {

                interface->current_tool = OUTIL_NONE;
                game->unfreeze = 0;
                interface_MAJOutils(interface, game);
                interface_MAJEtat(interface, game);
                wprintw(interface->info->interieur, "\nUnfreeze lance... pour de faux !");
            }
            else {
                wprintw(interface->info->interieur, "\nPas assez d'energie pour lancer un unfreeze.");
            }

            wrefresh(interface->info->interieur);
            
            break;
    }
}

/**
 * Gestion des actions dans la fenêtre des attaques.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre 
 */
void interface_attaques(struct interface* interface, struct game* game, int posX, int posY) {
    
    if((posX >= 25) && (posX <= 27)) {

        switch(posY) {

            case 0:

                if(game->money < SOLDAT_COUT) {
                    wprintw(interface->info->interieur, "\nPas assez d'argent pour lancer un soldat.");
                }
                else if(game->opponents_health[posX - 25] == 0) {
                    wprintw(interface->info->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                }
                else {

                    game->money -= SOLDAT_COUT;
                    interface_MAJAttaques(interface, game);
                    interface_MAJEtat(interface, game);
                    interface_MAJOutils(interface, game);
                    wprintw(interface->info->interieur, "\nEnvoi d'un soldat à l'adversaire %d... pour de faux", (posX - 24));
                }

                break;

            case 1:

                if(game->money < COMMANDO_COUT) {
                    wprintw(interface->info->interieur, "\nPas assez d'argent pour lancer un commando.");
                }
                else if(game->opponents_health[posX - 25] == 0) {
                    wprintw(interface->info->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                }
                else {

                    game->money -= COMMANDO_COUT;
                    interface_MAJAttaques(interface, game);
                    interface_MAJEtat(interface, game);
                    interface_MAJOutils(interface, game);
                    wprintw(interface->info->interieur, "\nEnvoi d'un commando à l'adversaire %d... pour de faux", (posX - 24));
                }

                break;

            case 2:

                if(game->money < VEHICULE_COUT) {
                    wprintw(interface->info->interieur, "\nPas assez d'argent pour lancer un vehicule blinde.");
                }
                else if(game->opponents_health[posX - 25] == 0) {
                    wprintw(interface->info->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                }
                else {

                    game->money -= VEHICULE_COUT;
                    interface_MAJAttaques(interface, game);
                    interface_MAJEtat(interface, game);
                    interface_MAJOutils(interface, game);
                    wprintw(interface->info->interieur, "\nEnvoi d'un vehicule blinde à l'adversaire %d... pour de faux", (posX - 24));
                }

                break;
            
            case 3:

                if(game->money < MISSILE_COUT) {
                    wprintw(interface->info->interieur, "\nPas assez d'argent pour lancer un lance-missiles.");
                }
                else if(game->opponents_health[posX - 25] == 0) {
                    wprintw(interface->info->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                }
                else {

                    game->money -= MISSILE_COUT;
                    interface_MAJAttaques(interface, game);
                    interface_MAJEtat(interface, game);
                    interface_MAJOutils(interface, game);
                    wprintw(interface->info->interieur, "\nEnvoi d'un lance-missiles à l'adversaire %d... pour de faux", (posX - 24));
                }

                break;
            
            case 4:

                if(game->money < CHAR_COUT) {
                    wprintw(interface->info->interieur, "\nPas assez d'argent pour lancer un char.");
                }
                else if(game->opponents_health[posX - 25] == 0) {
                    wprintw(interface->info->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                }
                else {

                    game->money -= CHAR_COUT;
                    interface_MAJAttaques(interface, game);
                    interface_MAJEtat(interface, game);
                    interface_MAJOutils(interface, game);
                    wprintw(interface->info->interieur, "\nEnvoi d'un char à l'adversaire %d... pour de faux", (posX - 24));
                }

                break;

            case 5:

                if(game->freeze < 10) {
                    wprintw(interface->info->interieur, "\nPas assez d'énergie pour lancer un freeze.");
                }
                else if(game->opponents_health[posX - 25] == 0) {
                    wprintw(interface->info->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                }
                else {

                    game->freeze = 0;
                    interface_MAJAttaques(interface, game);
                    interface_MAJEtat(interface, game);
                    wprintw(interface->info->interieur, "\nEnvoi d'un freeze à l'adversaire %d... pour de faux", (posX - 24));
                }

                break;
        }

        wrefresh(interface->info->interieur);
    }
}

/**
 * Gestion des actions dans la fenêtre de la carte.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre
 */
void interface_carte(struct interface* interface, struct game* game, int posX, int posY) {

    int status;
    int enough_money;
    pthread_t tower_thread;
    struct tower tower;
    struct tower_state tower_state;

    /* verrouillage du mutex de la case concernée */
    pthread_mutex_lock(&game->board[posY][posX].mutex);

    /* initialisations */
    enough_money = TRUE;

    /* traitement du choix d'outil */
    if(interface->current_tool == OUTIL_NONE) {

        if(game->board[posY][posX].background == CASE_VIDE) {
            wprintw(interface->info->interieur, "\nOh !!! De l'herbe !!!");
        }
        else if((game->board[posY][posX].background >= CASE_MIN_JOUEUR) && (game->board[posY][posX].background <= CASE_MAX_JOUEUR)) {
            wprintw(interface->info->interieur, "\nLe point de depart des unites de l'adversaire %d", (game->board[posY][posX].background - CASE_MIN_JOUEUR + 1));
        }
        else if(game->board[posY][posX].background == CASE_ORDI) {
            wprintw(interface->info->interieur, "\nLe point de depart des vagues envoyees par l'ordinateur");
        }
        else if(game->board[posY][posX].background == CASE_FORT) {
            wprintw(interface->info->interieur, "\nLe fort a proteger");
        }
        else if((game->board[posY][posX].background >= CASE_MIN_CHEMIN) && (game->board[posY][posX].background <= CASE_MAX_CHEMIN)) {
            wprintw(interface->info->interieur, "\nUne route...");
        }
        else if (game->board[posY][posX].background == CASE_PRISE) {
            wprintw(interface->info->interieur, "\nCette case est prise !");
        }
    }
    else if(interface->current_tool == OUTIL_UNFREEZE) {

        if(game->freeze == 10) {
            
            game->freeze = 0;
            wprintw(interface->info->interieur, "\nFREEEEEZE !!!");
            interface_MAJEtat(interface, game);
        }
        else {
            interface->current_tool = OUTIL_NONE;
            wprintw(interface->info->interieur, "\nDesole, pas possible...");
        }

        interface_MAJOutils(interface, game);
    }
    else {

        switch(interface->current_tool) {

            case OUTIL_TOUR_1:

                if((game->board[posY][posX].background == CASE_VIDE) && (game->money >= TOUR_1_COUT)) {

                    /* initialisation aux valeurs prédifinies */
                    tower.damage[MIN] = TOUR_1_TIR_MIN;
                    tower.damage[MAX] = TOUR_1_TIR_MAX;
                    tower.position[X] = posX;
                    tower.position[Y] = posY;
                    tower.speed = TOUR_1_VITESSE;
                    tower.range = TOUR_1_PORTEE;

                    /* soustraction du cout de la tour */
                    game->money -= TOUR_1_COUT;

                    /* information */
                    wprintw(interface->info->interieur, "\nTour 1 posee.");
                }
                else {
                    enough_money = FALSE;
                    wprintw(interface->info->interieur, "\nDesole, pas possible...");
                }

                break;

            case OUTIL_TOUR_2:

                if((game->board[posY][posX].background == CASE_VIDE) && (game->money >= TOUR_2_COUT)) {

                    /* initialisation aux valeurs prédifinies */
                    tower.damage[MIN] = TOUR_2_TIR_MIN;
                    tower.damage[MAX] = TOUR_2_TIR_MAX;
                    tower.position[X] = posX;
                    tower.position[Y] = posY;
                    tower.speed = TOUR_2_VITESSE;
                    tower.range = TOUR_2_PORTEE;

                    /* soustraction du cout de la tour */
                    game->money -= TOUR_2_COUT;

                    /* information */
                    wprintw(interface->info->interieur, "\nTour 2 posee.");
                }
                else {
                    enough_money = FALSE;
                    wprintw(interface->info->interieur, "\nDesole, pas possible...");
                }

                break;

            case OUTIL_TOUR_3:

                if((game->board[posY][posX].background == CASE_VIDE) && (game->money >= TOUR_3_COUT)) {

                    /* initialisation aux valeurs prédifinies */
                    tower.damage[MIN] = TOUR_3_TIR_MIN;
                    tower.damage[MAX] = TOUR_3_TIR_MAX;
                    tower.position[X] = posX;
                    tower.position[Y] = posY;
                    tower.speed = TOUR_3_VITESSE;
                    tower.range = TOUR_3_PORTEE;

                    /* soustraction du cout de la tour */
                    game->money -= TOUR_3_COUT;

                    /* information */
                    wprintw(interface->info->interieur, "\nTour 3 posee.");
                }
                else {
                    enough_money = FALSE;
                    wprintw(interface->info->interieur, "\nDesole, pas possible...");
                }

                break;

            case OUTIL_TOUR_4:

                if((game->board[posY][posX].background == CASE_VIDE) && (game->money >= TOUR_4_COUT)) {

                    /* initialisation aux valeurs prédifinies */
                    tower.damage[MIN] = TOUR_4_TIR_MIN;
                    tower.damage[MAX] = TOUR_4_TIR_MAX;
                    tower.position[X] = posX;
                    tower.position[Y] = posY;
                    tower.speed = TOUR_4_VITESSE;
                    tower.range = TOUR_4_PORTEE;

                    /* soustraction du cout de la tour */
                    game->money -= TOUR_4_COUT;

                    /* information */
                    wprintw(interface->info->interieur, "\nTour 4 posee.");
                }
                else {
                    enough_money = FALSE;
                    wprintw(interface->info->interieur, "\nDesole, pas possible...");
                }

                break;

            case OUTIL_TOUR_5:

                if((game->board[posY][posX].background == CASE_VIDE) && (game->money >= TOUR_5_COUT)) {

                    /* initialisation aux valeurs prédifinies */
                    tower.damage[MIN] = TOUR_5_TIR_MIN;
                    tower.damage[MAX] = TOUR_5_TIR_MAX;
                    tower.position[X] = posX;
                    tower.position[Y] = posY;
                    tower.speed = TOUR_5_VITESSE;
                    tower.range = TOUR_5_PORTEE;

                    /* soustraction du cout de la tour */
                    game->money -= TOUR_5_COUT;

                    /* information */
                    wprintw(interface->info->interieur, "\nTour 5 posee.");
                }
                else {
                    enough_money = FALSE;
                    wprintw(interface->info->interieur, "\nDesole, pas possible...");
                }

                break;
        }

        if(enough_money == TRUE) {
            
            /* préparations de la structure pour le thread */
            tower_state.game = game;
            tower_state.tower = &tower;  

            /* mise à jour de la case du plateau */
            game->board[posY][posX].background = CASE_PRISE;
            game->board[posY][posX].unit_type = UNITE_TOUR;
            game->board[posY][posX].tower = tower;

            /* positionnement de la tour */
            mvwaddch(interface->map->interieur, posY, posX, 'T');

            /* création du thread traitant le comportement de la tour */
            status = pthread_create(&tower_thread, NULL, tower_behaviour, &tower_state);
            if(status != 0){
                fprintf(stderr,"\nErreur lors de la création d'un thread");
                exit(EXIT_FAILURE);
            }
        }
    }

    /* déverrouillage du mutex de la case concernée */
    pthread_mutex_unlock(&game->board[posY][posX].mutex);

        
    /* rafraichissements */
    wrefresh(interface->info->interieur);
    wrefresh(interface->map->interieur);
}

/**
 * Gestion des actions de l'utilisateur dans l'interface.
 * @param interface l'interface
 * @param jeu les paramètres de la partie
 * @param c la touche pressée
 */
void interface_main(struct interface* interface, struct game* game, int input_key) {
    
    int posX;
    int posY;
    int sourisX;
    int sourisY;
  
    if((input_key == KEY_MOUSE) && (souris_getpos(&sourisX, &sourisY, NULL) == OK)) {
    
        /* Gestion des actions de la souris */
        if(fenetre_getcoordonnees(interface->tools, sourisX, sourisY, &posX, &posY)) {
            interface_outils(interface, game, posX, posY);
        }
        else if(fenetre_getcoordonnees(interface->attacks, sourisX, sourisY, &posX, &posY)) {
            interface_attaques(interface, game, posX, posY);
        }
        else if(fenetre_getcoordonnees(interface->map, sourisX, sourisY, &posX, &posY)) {
            
            pthread_mutex_lock(&game->board[posY][posY].mutex);

            interface_carte(interface, game, posX, posY);
          
            pthread_mutex_unlock(&game->board[posY][posY].mutex);
        }
    }
    else {
    
        /* Gestion du clavier */
        switch(input_key) {
            
            case '1':
            case '2':
            case '3':
                
                /* Supprime une vie à un adversaire */
                if(game->opponents_health[input_key - '1'] != 0) {
                    
                    game->opponents_health[input_key - '1']--;
                    interface_MAJEtat(interface, game);
                }
                
                break;

            case 'V':
            case 'v':
                
                /* Supprime une vie au joueur */
                if(game->health > 0) {
                    
                    game->health--;
                    interface_MAJEtat(interface, game);
                }
          
                break;

            case '$':
                
                /* Ajoute de l'argent */
                game->money += 10;
      
                if(game->money > 99999) {
                    game->money = 99999;
                }

                interface_MAJEtat(interface, game);
                interface_MAJOutils(interface, game);
                interface_MAJAttaques(interface, game);
                
                break;
            
            case 'U':
            case 'u':
                
                /* Avance l'état du unfreeze */
                if(game->unfreeze < 10) {
                    
                    game->unfreeze++;
                    interface_MAJEtat(interface, game);
                    interface_MAJOutils(interface, game);
                }
                
                break;
    
            case 'F':
            case 'f':
                
                /* Avance l'état du freeze */
                if(game->freeze < 10) {
                    
                    game->freeze++;
                    interface_MAJEtat(interface, game);
                    interface_MAJAttaques(interface, game);
                }
                
                break;
    
            case 27:

                /* Touche ECHAP : annule l'outil sélectionné */
                interface->current_tool = OUTIL_NONE;
                interface_MAJOutils(interface, game);
                
                break;
    
            default:
                
                /* Utile en mode DEBUG si on souhaite afficher le caractère associé à la touche pressée */
                wprintw(interface->info->interieur, "\nTouche %d pressee", input_key);
                wrefresh(interface->info->interieur);
                
                break;
        }
    }
}
