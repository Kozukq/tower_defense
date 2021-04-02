# Feuille de route

### Modélisation -> à faire
1) Données
2) Echanges
3) Fonctionnement du jeu
4) Problèmes de concurrence/threads

### Fichiers binaires et communications UDP/TCP 
__1) Lecture des fichiers binaires cartes et scénarios__

__=> Structure pour les cartes/scénarios__

__=> Lecture du contenu__

__2) Récupération des fichiers .bin dans les répertoires "cartes" et "scenarios"__

__=> Utilisation de opendir__

__3) Échanges UDP entre le client et le serveur (listes, choix de la configuration)__

__=> Quelles données ?__

__=> Quels échanges ?__

4) Développement de la partie TCP

=> Création de la socket TCP avec récupération du numéro de port aléatoire

=> Envoi du numéro de port au créateur

=> Sauvegarde des parties créées

=> Envoi du contenu de la carte aux clients

5) Démarrage de la partie

=> Test du scénario (les clients se contentent d'afficher le résultat)

### Développement du moteur de jeu
1) Affichage de la carte
=> Adaptation de l'interface fournie ou création d'une nouvelle
2) Déplacement des unités avec un thread
=> Création du thread
=> Déplacement (concurrence ?)
=> Mort (cas de destruction ?)
3) Gestion des tourelles avec les threads
=> Création du thread
=> Recherche d'une unité dans la portée
=> Gestion de la concurrence ?
=> Gestion du freeze/unfreeze ?

### Finalisation
1) Connexion des parties serveur et client
2) Rédaction du rapport
