ft_irc - 42
Description
ft_irc est un projet de 42 visant à créer un serveur IRC (Internet Relay Chat) minimaliste, conforme aux standards du protocole IRC.
L’objectif est de comprendre le fonctionnement des protocoles réseau, la gestion des connexions clients/serveurs et la communication en temps réel.
Objectifs pédagogiques
Comprendre les bases des sockets TCP/IP et du modèle client-serveur
Gérer plusieurs clients simultanément avec polling ou select
Implémenter un protocole simple pour l’envoi et la réception de messages
Manipuler les concepts de channels, nicknames, et commandes IRC
Assurer la robustesse et la gestion des erreurs dans un serveur réseau
Fonctionnalités
Connexion de plusieurs clients via TCP
Gestion des channels : création, liste, invitations
Commandes IRC principales :
/NICK : changer de pseudo
/USER : définir le nom et informations du client
/JOIN : rejoindre un channel
/PART : quitter un channel
/PRIVMSG : envoyer un message privé ou à un channel
Gestion des déconnexions propres et erreurs de protocole
